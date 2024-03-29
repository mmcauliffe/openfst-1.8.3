// Copyright 2005-2024 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the 'License');
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an 'AS IS' BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// See www.openfst.org for extensive documentation on this weighted
// finite-state transducer library.
//
// Cache implementations for ExpanderFst.
//
// Expander caches must expose a State type and a FindOrExpand template method:
//
// class ExpanderCache {
//  public:
//   class State;
//
//   template <Expander>
//   State* FindOrExpander(Expander& expander, StateId id) {
//     if (id is found in cache) return cached_state;
//
//     // Use the provided expander to create a new cached state and cache it.
//     expander.Expand(id, &new_state);
//     insert new_state into cache;
//     return new_state;
//   }
// };
//
// Cache implementations must be copyable and assignable. It is up to the
// implementation whether this means it will discard the contents of the cache,
// copy all of the cache, share some of the cache etc. It is *REQUIRED* that the
// copy be "safe", the copy and the original must be usable from concurrent
// threads without accessing any internally shared state.

#ifndef FST_EXPANDER_CACHE_H_
#define FST_EXPANDER_CACHE_H_

#include <cstddef>
#include <deque>
#include <memory>
#include <utility>
#include <vector>

#include <fst/cache.h>
#include <fst/fst.h>
#include <unordered_map>
#include <unordered_map>

namespace fst {

// Stateful allocators can't be used without careful handling in threaded
// contexts, so arbitrary stl allocators aren't supported here.
template <class A>
class SimpleVectorCacheState {
 public:
  using Arc = A;
  using Weight = typename Arc::Weight;
  using StateId = typename Arc::StateId;

  void Reset() {
    final_weight_ = Weight::Zero();
    niepsilons_ = 0;
    noepsilons_ = 0;
    arcs_.clear();
  }

  Weight Final() const { return final_weight_; }

  size_t NumInputEpsilons() const { return niepsilons_; }

  size_t NumOutputEpsilons() const { return noepsilons_; }

  size_t NumArcs() const { return arcs_.size(); }

  const Arc &GetArc(size_t n) const { return arcs_[n]; }

  const Arc *Arcs() const { return arcs_.empty() ? nullptr : &arcs_[0]; }

  void SetFinal(Weight weight) { final_weight_ = weight; }

  void ReserveArcs(size_t n) { arcs_.reserve(n); }

  void AddArc(const Arc &arc) {
    if (arc.ilabel == 0) ++niepsilons_;
    if (arc.olabel == 0) ++noepsilons_;
    arcs_.push_back(arc);
  }

  void AddArc(Arc &&arc) {
    if (arc.ilabel == 0) ++niepsilons_;
    if (arc.olabel == 0) ++noepsilons_;
    arcs_.push_back(std::move(arc));
  }

  int *MutableRefCount() const { return nullptr; }

 private:
  Weight final_weight_ = Weight::Zero();
  size_t niepsilons_ = 0;  // Number of input epsilons.
  size_t noepsilons_ = 0;  // Number of output epsilons.
  std::vector<Arc> arcs_;
};

template <class A>
class NoGcKeepOneExpanderCache {
 public:
  using Arc = A;
  using StateId = typename Arc::StateId;

  // Reference-counted state.
  class State : public SimpleVectorCacheState<Arc> {
   public:
    int *MutableRefCount() { return &ref_count_; }

    void Reset() {
      SimpleVectorCacheState<Arc>::Reset();
      ref_count_ = 0;
    }

   private:
    int ref_count_ = 0;

    friend class NoGcKeepOneExpanderCache;
  };

  NoGcKeepOneExpanderCache() : state_(new State) {}

  NoGcKeepOneExpanderCache(const NoGcKeepOneExpanderCache &copy)
      : state_(new State(*copy.state_)) {}

  template <class Expander>
  State *FindOrExpand(Expander &expander, StateId state_id) {
    if (state_id == state_id_) return state_.get();
    if (state_->ref_count_ > 0) cache_[state_id_] = std::move(state_);
    state_id_ = state_id;
    if (cache_.empty()) {
      state_->Reset();
      expander.Expand(state_id_, state_.get());
      return state_.get();
    }
    if (auto i = cache_.find(state_id_); i != cache_.end()) {
      state_ = std::move(i->second);
    }
    if (state_ == nullptr) {
      state_ = std::make_unique<State>();
      expander.Expand(state_id_, state_.get());
    }
    return state_.get();
  }

  StateId state_id_ = kNoStateId;
  std::unique_ptr<State> state_;
  std::unordered_map<StateId, std::unique_ptr<State>> cache_;
};

template <class A>
class HashExpanderCache {
 public:
  using Arc = A;
  using StateId = typename Arc::StateId;

  using State = SimpleVectorCacheState<Arc>;

  HashExpanderCache(const HashExpanderCache &copy) { *this = copy; }

  HashExpanderCache &operator=(const HashExpanderCache &copy) {
    for (const auto &[id, state] : copy.cache_) {
      cache_[id] = std::make_unique<State>(*state);
    }
    return *this;
  }

  ~HashExpanderCache() = default;

  template <class Expander>
  State *FindOrExpand(Expander &expander, StateId state_id) {
    auto [it, inserted] = cache_.emplace(state_id, nullptr);
    if (inserted) {
      it->second = std::make_unique<State>();
      expander.Expand(state_id, it->second.get());
    }
    return it->second.get();
  }

 private:
  std::unordered_map<StateId, std::unique_ptr<State>> cache_;
};

template <class A>
class VectorExpanderCache {
 public:
  using Arc = A;
  using StateId = typename Arc::StateId;

  using State = SimpleVectorCacheState<Arc>;

  VectorExpanderCache() : vec_(0, nullptr) {}

  VectorExpanderCache(const VectorExpanderCache &copy) { *this = copy; }

  VectorExpanderCache &operator=(const VectorExpanderCache &copy) {
    vec_.resize(copy.vec_.size());
    for (StateId i = 0; i < copy.vec_.size(); ++i) {
      const auto *state = copy.vec_[i];
      if (state != nullptr) {
        states_.emplace_back(*state);
        vec_[i] = &states_.back();
      }
    }
    return *this;
  }

  template <class Expander>
  State *FindOrExpand(Expander &expander, StateId state_id) {
    if (state_id >= vec_.size()) vec_.resize(state_id + 1);
    auto **slot = &vec_[state_id];
    if (*slot == nullptr) {
      states_.emplace_back();
      *slot = &states_.back();
      expander.Expand(state_id, *slot);
    }
    return *slot;
  }

 private:
  std::deque<State> states_;
  std::vector<State *> vec_;
};

template <class Expander>
using DefaultExpanderCache = VectorExpanderCache<typename Expander::Arc>;

}  // namespace fst
#endif  // FST_EXPANDER_CACHE_H_

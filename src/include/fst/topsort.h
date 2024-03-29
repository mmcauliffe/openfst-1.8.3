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
// Topological sort of FSTs.

#ifndef FST_TOPSORT_H_
#define FST_TOPSORT_H_

#include <vector>

#include <fst/dfs-visit.h>
#include <fst/fst.h>
#include <fst/mutable-fst.h>
#include <fst/properties.h>
#include <fst/statesort.h>

namespace fst {

// DFS visitor class to return topological ordering.
template <class Arc>
class TopOrderVisitor {
 public:
  using StateId = typename Arc::StateId;

  // If acyclic, order[i] gives the topological position of StateId i;
  // otherwise it is unchanged. acyclic_ will be true iff the FST has no
  // cycles. The caller retains ownership of the state order vector.
  TopOrderVisitor(std::vector<StateId> *order, bool *acyclic)
      : order_(order), acyclic_(acyclic) {}

  void InitVisit(const Fst<Arc> &fst) {
    finish_.clear();
    *acyclic_ = true;
  }

  constexpr bool InitState(StateId, StateId) const { return true; }

  constexpr bool TreeArc(StateId, const Arc &) const { return true; }

  bool BackArc(StateId, const Arc &) { return (*acyclic_ = false); }

  constexpr bool ForwardOrCrossArc(StateId, const Arc &) const { return true; }

  void FinishState(StateId s, StateId, const Arc *) { finish_.push_back(s); }

  void FinishVisit() {
    if (*acyclic_) {
      order_->clear();
      for (StateId s = 0; s < finish_.size(); ++s) {
        order_->push_back(kNoStateId);
      }
      for (StateId s = 0; s < finish_.size(); ++s) {
        (*order_)[finish_[finish_.size() - s - 1]] = s;
      }
    }
  }

 private:
  std::vector<StateId> *order_;
  bool *acyclic_;
  // States in finish-time order.
  std::vector<StateId> finish_;
};

// Topologically sorts its input if acyclic, modifying it. Otherwise, the input
// is unchanged. When sorted, all transitions are from lower to higher state
// IDs.
//
// Complexity:
//
//   Time:  O(V + E)
//   Space: O(V + E)
//
// where V is the number of states and E is the number of arcs.
template <class Arc>
bool TopSort(MutableFst<Arc> *fst) {
  std::vector<typename Arc::StateId> order;
  bool acyclic;
  TopOrderVisitor<Arc> top_order_visitor(&order, &acyclic);
  DfsVisit(*fst, &top_order_visitor);
  if (acyclic) {
    StateSort(fst, order);
    fst->SetProperties(kAcyclic | kInitialAcyclic | kTopSorted,
                       kAcyclic | kInitialAcyclic | kTopSorted);
  } else {
    fst->SetProperties(kCyclic | kNotTopSorted, kCyclic | kNotTopSorted);
  }
  return acyclic;
}

}  // namespace fst

#endif  // FST_TOPSORT_H_

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
// Union-find algorithm for dense sets of non-negative integers, implemented
// using disjoint tree forests with rank heuristics and path compression.

#ifndef FST_UNION_FIND_H_
#define FST_UNION_FIND_H_

#include <vector>

namespace fst {

// Union-Find algorithm for dense sets of non-negative integers.
template <class T>
class UnionFind {
 public:
  // Creates a disjoint set forest for the range [0; max); 'fail' is a value
  // indicating that an element hasn't been initialized using MakeSet(...).
  // The upper bound of the range can be reset (increased) using MakeSet(...).
  UnionFind(T max, T fail) : parent_(max, fail), rank_(max), fail_(fail) {}

  // Finds the representative of the set 'item' belongs to, performing path
  // compression if necessary.
  T FindSet(T item) {
    if (item >= parent_.size() || item == fail_ || parent_[item] == fail_) {
      return fail_;
    }
    T root = item;
    while (root != parent_[root]) {
      root = parent_[root];
    }
    while (item != parent_[item]) {
      T parent = parent_[item];
      parent_[item] = root;
      item = parent;
    }
    return root;
  }

  // Creates the (destructive) union of the sets x and y belong to.
  void Union(T x, T y) { Link(FindSet(x), FindSet(y)); }

  // Initialization of an element: creates a singleton set containing 'item'.
  // The range [0; max) is reset if item >= max.
  T MakeSet(T item) {
    if (item >= parent_.size()) {
      // New value in parent_ should be initialized to fail_.
      const auto nitem = item > 0 ? 2 * item : 2;
      parent_.resize(nitem, fail_);
      rank_.resize(nitem);
    }
    parent_[item] = item;
    return item;
  }

  // Initialization of all elements starting from 0 to max - 1 to distinct sets.
  void MakeAllSet(T max) {
    parent_.resize(max);
    for (T item = 0; item < max; ++item) parent_[item] = item;
  }

  // For testing only.
  const T &Parent(const T &x) const { return parent_[x]; }

 private:
  // Links trees rooted in 'x' and 'y'.
  void Link(T x, T y) {
    if (x == y) return;
    if (rank_[x] > rank_[y]) {
      parent_[y] = x;
    } else {
      parent_[x] = y;
      if (rank_[x] == rank_[y]) {
        ++rank_[y];
      }
    }
  }

  UnionFind(const UnionFind &) = delete;

  UnionFind &operator=(const UnionFind &) = delete;

  std::vector<T> parent_;  // Parent nodes.
  std::vector<int> rank_;  // Rank of an element = min. depth in tree.
  T fail_;                 // Value indicating lookup failure.
};

}  // namespace fst

#endif  // FST_UNION_FIND_H_

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
// Function to sort states of an FST.

#ifndef FST_STATESORT_H_
#define FST_STATESORT_H_

#include <algorithm>
#include <utility>
#include <vector>

#include <fst/log.h>
#include <fst/fst.h>
#include <fst/mutable-fst.h>
#include <fst/properties.h>
#include <fst/util.h>

namespace fst {

// Sorts the input states of an FST. order[i] gives the state ID after
// sorting that corresponds to the state ID i before sorting; it must
// therefore be a permutation of the input FST's states ID sequence.
template <class Arc>
void StateSort(MutableFst<Arc> *fst,
               const std::vector<typename Arc::StateId> &order) {
  using StateId = typename Arc::StateId;
  using Weight = typename Arc::Weight;
  if (order.size() != fst->NumStates()) {
    FSTERROR() << "StateSort: Bad order vector size: " << order.size();
    fst->SetProperties(kError, kError);
    return;
  }
  if (fst->Start() == kNoStateId) return;
  const auto props = fst->Properties(kStateSortProperties, false);
  std::vector<bool> done(order.size(), false);
  std::vector<Arc> arcsa;
  std::vector<Arc> arcsb;
  fst->SetStart(order[fst->Start()]);
  for (StateIterator<MutableFst<Arc>> siter(*fst); !siter.Done();
       siter.Next()) {
    auto s1 = siter.Value();
    StateId s2;
    if (done[s1]) continue;
    auto final1 = fst->Final(s1);
    auto final2 = Weight::Zero();
    arcsa.clear();
    for (ArcIterator<MutableFst<Arc>> aiter(*fst, s1); !aiter.Done();
         aiter.Next()) {
      arcsa.push_back(aiter.Value());
    }
    for (; !done[s1]; s1 = s2, final1 = final2, std::swap(arcsa, arcsb)) {
      s2 = order[s1];
      if (!done[s2]) {
        final2 = fst->Final(s2);
        arcsb.clear();
        for (ArcIterator<MutableFst<Arc>> aiter(*fst, s2); !aiter.Done();
             aiter.Next()) {
          arcsb.push_back(aiter.Value());
        }
      }
      fst->SetFinal(s2, final1);
      fst->DeleteArcs(s2);
      for (auto arc : arcsa) {  // NOLINT(performance-for-range-copy)
        arc.nextstate = order[arc.nextstate];
        fst->AddArc(s2, arc);
      }
      done[s1] = true;
    }
  }
  fst->SetProperties(props, kFstProperties);
}

}  // namespace fst

#endif  // FST_STATESORT_H_

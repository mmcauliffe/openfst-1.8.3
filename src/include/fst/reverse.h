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
// Functions and classes to sort arcs in an FST.

#ifndef FST_REVERSE_H_
#define FST_REVERSE_H_

#include <algorithm>
#include <cstdint>
#include <vector>

#include <fst/cache.h>
#include <fst/cc-visitors.h>
#include <fst/dfs-visit.h>
#include <fst/expanded-fst.h>
#include <fst/fst.h>
#include <fst/mutable-fst.h>
#include <fst/properties.h>

namespace fst {

// Reverses an FST. The reversed result is written to an output mutable FST.
// If A transduces string x to y with weight a, then the reverse of A
// transduces the reverse of x to the reverse of y with weight a.Reverse().
//
// Typically, a = a.Reverse() and an arc is its own reverse (e.g., for
// TropicalWeight or LogWeight). In general, e.g., when the weights only form a
// left or right semiring, the output arc type must match the input arc type
// except having the reversed Weight type.
//
// When require_superinitial is false, a superinitial state is not created in
// the reversed FST iff the input FST has exactly one final state (which becomes
// the initial state of the reversed FST) with a final weight of semiring One,
// or if it does not belong to any cycle. When require_superinitial is true, a
// superinitial state is always created.
template <class FromArc, class ToArc>
void Reverse(const Fst<FromArc> &ifst, MutableFst<ToArc> *ofst,
             bool require_superinitial = true) {
  using StateId = typename FromArc::StateId;
  using Weight = typename FromArc::Weight;
  ofst->DeleteStates();
  ofst->SetInputSymbols(ifst.InputSymbols());
  ofst->SetOutputSymbols(ifst.OutputSymbols());
  if (std::optional<StateId> num_states = ifst.NumStatesIfKnown()) {
    ofst->ReserveStates(*num_states + 1);
  }
  StateId istart = ifst.Start();
  StateId ostart = kNoStateId;
  StateId offset = 0;
  uint64_t dfs_iprops = 0;
  uint64_t dfs_oprops = 0;
  if (!require_superinitial) {
    for (StateIterator<Fst<FromArc>> siter(ifst); !siter.Done(); siter.Next()) {
      const auto s = siter.Value();
      if (ifst.Final(s) == Weight::Zero()) continue;
      if (ostart != kNoStateId) {
        ostart = kNoStateId;
        break;
      } else {
        ostart = s;
      }
    }
    if (ostart != kNoStateId && ifst.Final(ostart) != Weight::One()) {
      std::vector<StateId> scc;
      SccVisitor<FromArc> scc_visitor(&scc, nullptr, nullptr, &dfs_iprops);
      DfsVisit(ifst, &scc_visitor);
      if (std::count(scc.begin(), scc.end(), scc[ostart]) > 1) {
        ostart = kNoStateId;
      } else {
        for (ArcIterator<Fst<FromArc>> aiter(ifst, ostart); !aiter.Done();
             aiter.Next()) {
          if (aiter.Value().nextstate == ostart) {
            ostart = kNoStateId;
            break;
          }
        }
      }
      if (ostart != kNoStateId) dfs_oprops = kInitialAcyclic;
    }
  }
  if (ostart == kNoStateId) {  // Super-initial requested or needed.
    ostart = ofst->AddState();
    offset = 1;
  }
  for (StateIterator<Fst<FromArc>> siter(ifst); !siter.Done(); siter.Next()) {
    const auto is = siter.Value();
    const auto os = is + offset;
    while (ofst->NumStates() <= os) ofst->AddState();
    if (is == istart) ofst->SetFinal(os);
    const auto weight = ifst.Final(is);
    if ((weight != Weight::Zero()) && (offset == 1)) {
      const ToArc oarc(0, 0, weight.Reverse(), os);
      ofst->AddArc(0, oarc);
    }
    for (ArcIterator<Fst<FromArc>> aiter(ifst, is); !aiter.Done();
         aiter.Next()) {
      const auto &iarc = aiter.Value();
      const auto nos = iarc.nextstate + offset;
      auto weight = iarc.weight.Reverse();
      if (!offset && (nos == ostart)) {
        weight = Times(ifst.Final(ostart).Reverse(), weight);
      }
      const ToArc oarc(iarc.ilabel, iarc.olabel, weight, os);
      while (ofst->NumStates() <= nos) ofst->AddState();
      ofst->AddArc(nos, oarc);
    }
  }
  ofst->SetStart(ostart);
  if (offset == 0 && ostart == istart) {
    ofst->SetFinal(ostart, ifst.Final(ostart).Reverse());
  }
  const auto iprops = ifst.Properties(kCopyProperties, false) | dfs_iprops;
  const auto oprops = ofst->Properties(kFstProperties, false) | dfs_oprops;
  ofst->SetProperties(ReverseProperties(iprops, offset == 1) | oprops,
                      kFstProperties);
}

}  // namespace fst

#endif  // FST_REVERSE_H_

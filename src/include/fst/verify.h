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
// Function to verify an FST's contents.

#ifndef FST_VERIFY_H_
#define FST_VERIFY_H_

#include <cstddef>
#include <cstdint>

#include <fst/log.h>
#include <fst/expanded-fst.h>
#include <fst/fst.h>
#include <fst/properties.h>
#include <fst/test-properties.h>

namespace fst {

// Verifies that an Fst's contents are sane.
template <class Arc>
bool Verify(const Fst<Arc> &fst, bool allow_negative_labels = false) {
  const auto start = fst.Start();
  const auto *isyms = fst.InputSymbols();
  const auto *osyms = fst.OutputSymbols();
  const auto ns = CountStates(fst);
  if (start == kNoStateId && ns > 0) {
    LOG(ERROR) << "Verify: FST start state ID not set";
    return false;
  } else if (start >= ns) {
    LOG(ERROR) << "Verify: FST start state ID exceeds number of states";
    return false;
  }
  for (StateIterator<Fst<Arc>> siter(fst); !siter.Done(); siter.Next()) {
    auto state = siter.Value();
    size_t na = 0;
    for (ArcIterator<Fst<Arc>> aiter(fst, state); !aiter.Done(); aiter.Next()) {
      const auto &arc = aiter.Value();
      if (!allow_negative_labels && arc.ilabel < 0) {
        LOG(ERROR) << "Verify: FST input label ID of arc at position " << na
                   << " of state " << state << " is negative";
        return false;
      } else if (isyms && !isyms->Member(arc.ilabel)) {
        LOG(ERROR) << "Verify: FST input label ID " << arc.ilabel
                   << " of arc at position " << na << " of state " << state
                   << " is missing from input symbol table \"" << isyms->Name()
                   << "\"";
        return false;
      } else if (!allow_negative_labels && arc.olabel < 0) {
        LOG(ERROR) << "Verify: FST output label ID of arc at position " << na
                   << " of state " << state << " is negative";
        return false;
      } else if (osyms && !osyms->Member(arc.olabel)) {
        LOG(ERROR) << "Verify: FST output label ID " << arc.olabel
                   << " of arc at position " << na << " of state " << state
                   << " is missing from output symbol table \"" << osyms->Name()
                   << "\"";
        return false;
      } else if (!arc.weight.Member()) {
        LOG(ERROR) << "Verify: FST weight of arc at position " << na
                   << " of state " << state << " is invalid";
        return false;
      } else if (arc.nextstate < 0) {
        LOG(ERROR) << "Verify: FST destination state ID of arc at position "
                   << na << " of state " << state << " is negative";
        return false;
      } else if (arc.nextstate >= ns) {
        LOG(ERROR) << "Verify: FST destination state ID of arc at position "
                   << na << " of state " << state
                   << " exceeds number of states";
        return false;
      }
      ++na;
    }
    if (!fst.Final(state).Member()) {
      LOG(ERROR) << "Verify: FST final weight of state " << state
                 << " is invalid";
      return false;
    }
  }
  const auto fst_props = fst.Properties(kFstProperties, /*test=*/false);
  if (fst_props & kError) {
    LOG(ERROR) << "Verify: FST error property is set";
    return false;
  }
  uint64_t known_props;
  uint64_t test_props =
      internal::ComputeProperties(fst, kFstProperties, &known_props);
  if (!internal::CompatProperties(fst_props, test_props)) {
    LOG(ERROR) << "Verify: Stored FST properties incorrect "
               << "(props1 = stored props, props2 = tested)";
    return false;
  } else {
    return true;
  }
}

}  // namespace fst

#endif  // FST_VERIFY_H_

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

#ifndef FST_SCRIPT_EQUIVALENT_H_
#define FST_SCRIPT_EQUIVALENT_H_

#include <tuple>

#include <fst/equivalent.h>
#include <fst/fst.h>
#include <fst/weight.h>
#include <fst/script/arg-packs.h>
#include <fst/script/fst-class.h>

namespace fst {
namespace script {

using FstEquivalentInnerArgs =
    std::tuple<const FstClass &, const FstClass &, float, bool *>;

using FstEquivalentArgs = WithReturnValue<bool, FstEquivalentInnerArgs>;

template <class Arc>
void Equivalent(FstEquivalentArgs *args) {
  const Fst<Arc> &fst1 = *std::get<0>(args->args).GetFst<Arc>();
  const Fst<Arc> &fst2 = *std::get<1>(args->args).GetFst<Arc>();
  args->retval =
      Equivalent(fst1, fst2, std::get<2>(args->args), std::get<3>(args->args));
}

bool Equivalent(const FstClass &fst1, const FstClass &fst2,
                float delta = kDelta, bool *error = nullptr);

}  // namespace script
}  // namespace fst

#endif  // FST_SCRIPT_EQUIVALENT_H_

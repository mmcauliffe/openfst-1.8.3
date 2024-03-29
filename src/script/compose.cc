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

#include <fst/script/compose.h>

#include <fst/compose.h>
#include <fst/properties.h>
#include <fst/script/fst-class.h>
#include <fst/script/script-impl.h>

namespace fst {
namespace script {

void Compose(const FstClass &ifst1, const FstClass &ifst2,
             MutableFstClass *ofst, const ComposeOptions &opts) {
  if (!internal::ArcTypesMatch(ifst1, ifst2, "Compose") ||
      !internal::ArcTypesMatch(*ofst, ifst1, "Compose")) {
    ofst->SetProperties(kError, kError);
    return;
  }
  FstComposeArgs args{ifst1, ifst2, ofst, opts};
  Apply<Operation<FstComposeArgs>>("Compose", ifst1.ArcType(), &args);
}

REGISTER_FST_OPERATION_3ARCS(Compose, FstComposeArgs);

}  // namespace script
}  // namespace fst

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

#include <fst/script/decode.h>

#include <fst/properties.h>
#include <fst/script/encodemapper-class.h>
#include <fst/script/fst-class.h>
#include <fst/script/script-impl.h>

namespace fst {
namespace script {

void Decode(MutableFstClass *fst, const EncodeMapperClass &mapper) {
  if (!internal::ArcTypesMatch(*fst, mapper, "Decode")) {
    fst->SetProperties(kError, kError);
    return;
  }
  FstDecodeArgs args{fst, mapper};
  Apply<Operation<FstDecodeArgs>>("Decode", fst->ArcType(), &args);
}

REGISTER_FST_OPERATION_3ARCS(Decode, FstDecodeArgs);

}  // namespace script
}  // namespace fst

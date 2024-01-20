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

#ifndef FST_SCRIPT_REVERSE_H_
#define FST_SCRIPT_REVERSE_H_

#include <tuple>

#include <fst/fst.h>
#include <fst/mutable-fst.h>
#include <fst/reverse.h>
#include <fst/script/fst-class.h>
#include <fst/exports/exports.h>

namespace fst {
namespace script {

using FstReverseArgs = std::tuple<const FstClass &, MutableFstClass *, bool>;

template <class Arc>
void Reverse(FstReverseArgs *args) {
  const Fst<Arc> &ifst = *std::get<0>(*args).GetFst<Arc>();
  MutableFst<Arc> *ofst = std::get<1>(*args)->GetMutableFst<Arc>();
  Reverse(ifst, ofst, std::get<2>(*args));
}

void fstscript_EXPORT Reverse(const FstClass &ifst, MutableFstClass *ofst,
             bool require_superinitial = true);

}  // namespace script
}  // namespace fst

#endif  // FST_SCRIPT_REVERSE_H_

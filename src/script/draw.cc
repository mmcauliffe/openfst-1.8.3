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

#include <fst/script/draw.h>

#include <ostream>
#include <string>

#include <fst/symbol-table.h>
#include <fst/script/fst-class.h>
#include <fst/script/script-impl.h>

namespace fst {
namespace script {

void Draw(const FstClass &fst, const SymbolTable *isyms,
          const SymbolTable *osyms, const SymbolTable *ssyms, bool accep,
          const std::string &title, float width, float height, bool portrait,
          bool vertical, float ranksep, float nodesep, int fontsize,
          int precision, const std::string &float_format, bool show_weight_one,
          std::ostream &ostrm, const std::string &dest) {
  FstDrawArgs args{
      fst,      isyms,     osyms,        ssyms,           accep,   title,
      width,    height,    portrait,     vertical,        ranksep, nodesep,
      fontsize, precision, float_format, show_weight_one, ostrm,   dest};
  Apply<Operation<FstDrawArgs>>("Draw", fst.ArcType(), &args);
}

REGISTER_FST_OPERATION_3ARCS(Draw, FstDrawArgs);

}  // namespace script
}  // namespace fst

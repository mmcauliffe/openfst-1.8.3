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
// Function that implements epsilon-normalization.

#ifndef FST_EPSNORMALIZE_H_
#define FST_EPSNORMALIZE_H_

#include <memory>

#include <fst/arc-map.h>
#include <fst/arc.h>
#include <fst/factor-weight.h>
#include <fst/fst.h>
#include <fst/invert.h>
#include <fst/mutable-fst.h>
#include <fst/rmepsilon.h>
#include <fst/string-weight.h>
#include <fst/symbol-table.h>
#include <fst/vector-fst.h>

namespace fst {

enum EpsNormalizeType { EPS_NORM_INPUT, EPS_NORM_OUTPUT };

// Returns an equivalent FST that is epsilon-normalized. An acceptor is
// epsilon-normalized if it is epsilon-removed. A transducer is input
// epsilon-normalized if additionally if on each path any epsilon input
// label follows all non-epsilon input labels. Output epsilon-normalized
// is defined similarly.
//
// For more information, see:
//
// Mohri, M. 2002. Generic epsilon-removal and input epsilon-normalization
// algorithms for weighted transducers. International Journal of Computer
// Science, 13(1): 129-143, 2002.
template <class Arc>
void EpsNormalize(const Fst<Arc> &ifst, MutableFst<Arc> *ofst,
                  EpsNormalizeType type = EPS_NORM_INPUT) {
  EpsNormalize<Arc, GALLIC>(ifst, ofst, type);
}

// Same as above, except allows specifying explicitly the gallic weight type.
template <class Arc, GallicType G>
void EpsNormalize(const Fst<Arc> &ifst, MutableFst<Arc> *ofst,
                  EpsNormalizeType type) {
  VectorFst<GallicArc<Arc, G>> gfst;
  std::unique_ptr<SymbolTable> symbols;
  if (type == EPS_NORM_INPUT) {
    ArcMap(ifst, &gfst, ToGallicMapper<Arc, G>());
    if (ifst.OutputSymbols()) symbols.reset(ifst.OutputSymbols()->Copy());
  } else {  // type == EPS_NORM_OUTPUT
    ArcMap(InvertFst<Arc>(ifst), &gfst, ToGallicMapper<Arc, G>());
    if (ifst.InputSymbols()) symbols.reset(ifst.InputSymbols()->Copy());
  }
  RmEpsilon(&gfst);
  FactorWeightFst<GallicArc<Arc, G>,
                  GallicFactor<typename Arc::Label, typename Arc::Weight, G>>
      fwfst(gfst);
  ArcMap(fwfst, ofst, FromGallicMapper<Arc, G>());
  ofst->SetOutputSymbols(symbols.get());
  if (type == EPS_NORM_OUTPUT) Invert(ofst);
}

}  // namespace fst

#endif  // FST_EPSNORMALIZE_H_

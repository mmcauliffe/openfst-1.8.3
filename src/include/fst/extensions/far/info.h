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

#ifndef FST_EXTENSIONS_FAR_INFO_H_
#define FST_EXTENSIONS_FAR_INFO_H_

#include <cmath>
#include <cstddef>
#include <iomanip>
#include <ios>
#include <iostream>
#include <memory>
#include <ostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <fst/log.h>
#include <fst/extensions/far/far.h>
#include <fst/extensions/far/getters.h>
#include <fst/fst.h>
#include <string_view>

namespace fst {

template <class Arc>
void AccumulateStatesAndArcs(const Fst<Arc> &fst, size_t *nstate, size_t *narc,
                             size_t *nfinal) {
  for (StateIterator<Fst<Arc>> siter(fst); !siter.Done();
       siter.Next(), ++(*nstate)) {
    ArcIterator<Fst<Arc>> aiter(fst, siter.Value());
    for (; !aiter.Done(); aiter.Next(), ++(*narc)) {
    }
    if (fst.Final(siter.Value()) != Arc::Weight::Zero()) ++(*nfinal);
  }
}

struct KeyInfo {
  std::string key;
  std::string type;
  size_t nstate = 0;
  size_t narc = 0;
  size_t nfinal = 0;
};

struct FarInfoData {
  std::vector<KeyInfo> key_infos;
  std::string far_type;
  std::string arc_type;
  size_t nfst = 0;
  size_t nstate = 0;
  size_t narc = 0;
  size_t nfinal = 0;
  std::set<std::string> fst_types;
};

template <class Arc>
void GetInfo(const std::vector<std::string> &sources,
             std::string_view begin_key, std::string_view end_key,
             const bool list_fsts, FarInfoData *far_info) {
  *far_info = FarInfoData();
  std::unique_ptr<FarReader<Arc>> reader(FarReader<Arc>::Open(sources));
  if (!reader) {
    LOG(ERROR) << "GetFarInfo: failed to create far reader.";
    return;
  }
  if (!begin_key.empty()) reader->Find(begin_key);

  for (; !reader->Done(); reader->Next()) {
    const auto &key = reader->GetKey();
    if (!end_key.empty() && end_key < key) break;
    ++far_info->nfst;
    const auto *fst = reader->GetFst();
    far_info->fst_types.insert(fst->Type());
    if (list_fsts) {
      KeyInfo info;
      info.key = key;
      info.type = fst->Type();
      AccumulateStatesAndArcs(*fst, &info.nstate, &info.narc, &info.nfinal);
      far_info->nstate += info.nstate;
      far_info->narc += info.narc;
      far_info->nfinal += info.nfinal;
      far_info->key_infos.push_back(info);
    } else {
      AccumulateStatesAndArcs(*fst, &far_info->nstate, &far_info->narc,
                              &far_info->nfinal);
    }
  }
  far_info->far_type = GetFarTypeString(reader->Type());
  far_info->arc_type = Arc::Type();
}

template <class Arc>
void Info(const std::vector<std::string> &sources, std::string_view begin_key,
          std::string_view end_key, const bool list_fsts) {
  FarInfoData info;
  GetInfo<Arc>(sources, begin_key, end_key, list_fsts, &info);
  if (!list_fsts) {
    std::cout << std::left << std::setw(50) << "far type" << info.far_type
              << '\n';
    std::cout << std::left << std::setw(50) << "arc type" << Arc::Type()
              << std::endl;
    std::cout << std::left << std::setw(50) << "fst type";
    for (auto iter = info.fst_types.begin(); iter != info.fst_types.end();
         ++iter) {
      if (iter != info.fst_types.begin()) std::cout << ",";
      std::cout << *iter;
    }
    std::cout << '\n';
    std::cout << std::left << std::setw(50) << "# of FSTs" << info.nfst << '\n';
    std::cout << std::left << std::setw(50) << "total # of states"
              << info.nstate << '\n';
    std::cout << std::left << std::setw(50) << "total # of arcs" << info.narc
              << '\n';
    std::cout << std::left << std::setw(50) << "total # of final states"
              << info.nfinal << '\n';
  } else {
    // FIXME(kbg): Grok, then document this.
    int wkey = 10;
    int wtype = 10;
    int wnstate = 14;
    int wnarc = 12;
    int wnfinal = 20;
    for (const auto &key_info : info.key_infos) {
      if (key_info.key.size() + 2 > wkey) wkey = key_info.key.size() + 2;
      if (key_info.type.size() + 2 > wtype) wtype = key_info.type.size() + 2;
      if (ceil(log10(key_info.nstate)) + 2 > wnstate) {
        wnstate = ceil(log10(key_info.nstate)) + 2;
      }
      if (ceil(log10(key_info.narc)) + 2 > wnarc) {
        wnarc = ceil(log10(key_info.narc)) + 2;
      }
      if (ceil(log10(key_info.nfinal)) + 2 > wnfinal) {
        wnfinal = ceil(log10(key_info.nfinal)) + 2;
      }
    }
    std::cout << std::left << std::setw(wkey) << "key" << std::setw(wtype)
              << "type" << std::right << std::setw(wnstate) << "# of states"
              << std::setw(wnarc) << "# of arcs" << std::setw(wnfinal)
              << "# of final states" << '\n';
    for (const auto &key_info : info.key_infos) {
      std::cout << std::left << std::setw(wkey) << key_info.key
                << std::setw(wtype) << key_info.type << std::right
                << std::setw(wnstate) << key_info.nstate << std::setw(wnarc)
                << key_info.narc << std::setw(wnfinal) << key_info.nfinal
                << '\n';
    }
  }
}

}  // namespace fst

#endif  // FST_EXTENSIONS_FAR_INFO_H_

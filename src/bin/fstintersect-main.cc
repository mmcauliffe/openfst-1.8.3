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
// Intersects two FSTs.

#include <cstring>
#include <memory>
#include <string>

#include <fst/flags.h>
#include <fst/log.h>
#include <fst/compose.h>
#include <fst/intersect.h>
#include <fst/script/fst-class.h>
#include <fst/script/getters.h>
#include <fst/script/intersect.h>

DECLARE_string(compose_filter);
DECLARE_bool(connect);

int fstintersect_main(int argc, char **argv) {
  namespace s = fst::script;
  using fst::ComposeFilter;
  using fst::IntersectOptions;
  using fst::script::FstClass;
  using fst::script::VectorFstClass;

  std::string usage = "Intersects two FSAs.\n\n  Usage: ";
  usage += argv[0];
  usage += " in1.fst in2.fst [out.fst]\n";
  usage += "  Flags: connect\n";

  SET_FLAGS(usage.c_str(), &argc, &argv, true);
  if (argc < 3 || argc > 4) {
    ShowUsage();
    return 1;
  }

  const std::string in1_name = strcmp(argv[1], "-") == 0 ? "" : argv[1];
  const std::string in2_name = strcmp(argv[2], "-") == 0 ? "" : argv[2];
  const std::string out_name =
      argc > 3 && strcmp(argv[3], "-") != 0 ? argv[3] : "";

  if (in1_name.empty() && in2_name.empty()) {
    LOG(ERROR) << argv[0] << ": Can't take both inputs from standard input";
    return 1;
  }

  std::unique_ptr<FstClass> ifst1(FstClass::Read(in1_name));
  if (!ifst1) return 1;
  std::unique_ptr<FstClass> ifst2(FstClass::Read(in2_name));
  if (!ifst2) return 1;

  VectorFstClass ofst(ifst1->ArcType());

  ComposeFilter compose_filter;
  if (!s::GetComposeFilter(FST_FLAGS_compose_filter,
                           &compose_filter)) {
    LOG(ERROR) << argv[0] << ": Unknown or unsupported compose filter type: "
               << FST_FLAGS_compose_filter;
    return 1;
  }

  const IntersectOptions opts(FST_FLAGS_connect, compose_filter);

  s::Intersect(*ifst1, *ifst2, &ofst, opts);

  return !ofst.Write(out_name);
}

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
// Google-style flag handling declarations and inline definitions.

#ifndef FST_FLAGS_H_
#define FST_FLAGS_H_

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

#include <fst/lock.h>

#include <fst/exports/exports.h>

// FLAGS USAGE:
//
// Definition example:
//
//    DEFINE_int32(length, 0, "length");
//
// This defines variable FST_FLAGS_length, initialized to 0.
//
// Declaration example:
//
//    DECLARE_int32(length);
//
// SET_FLAGS() can be used to set flags from the command line
// using, for example, '--length=2'.
//
// ShowUsage() can be used to print out command and flag usage.

#define DECLARE_bool(name) extern bool FST_FLAGS_ ## name
#define DECLARE_string(name) extern std::string FST_FLAGS_##name
#define DECLARE_int32(name) extern int32_t FST_FLAGS_##name
#define DECLARE_int64(name) extern int64_t FST_FLAGS_##name
#define DECLARE_uint64(name) extern uint64_t FST_FLAGS_##name
#define DECLARE_double(name) extern double FST_FLAGS_ ## name

#define DECLARE_export_bool(name, export_macro) extern bool export_macro FST_FLAGS_##name
#define DECLARE_export_string(name, export_macro) extern std::string export_macro FST_FLAGS_##name
#define DECLARE_export_int32(name, export_macro) extern int32_t export_macro FST_FLAGS_##name
#define DECLARE_export_int64(name, export_macro) extern int64_t export_macro FST_FLAGS_##name
#define DECLARE_export_uint64(name, export_macro) extern uint64_t export_macro FST_FLAGS_##name
#define DECLARE_export_double(name, export_macro) extern double export_macro FST_FLAGS_##name

template <typename T>
struct FlagDescription {
  FlagDescription(T *addr, std::string_view doc, std::string_view type,
                  std::string_view file, const T val)
      : address(addr),
    doc_string(doc),
    type_name(type),
    file_name(file),
    default_value(val) {}

  T *address;
  std::string_view doc_string;
  std::string_view type_name;
  std::string_view file_name;
  const T default_value;
};



#ifdef _WIN32
class FlagRegisterBase { };

class FlagSingleton {
public:

    template <class RegisterType>
    std::shared_ptr<RegisterType> GetRegister() {

        std::string type_name = typeid(RegisterType).name();
        if (registry.find(type_name) == registry.end()) {
            std::shared_ptr<RegisterType> r = std::make_shared<RegisterType>();
            registry[type_name] = std::static_pointer_cast <FlagRegisterBase>(r);
        }
        return  std::static_pointer_cast <RegisterType>(registry[type_name]);
    }
private:
    std::map<std::string, std::shared_ptr<FlagRegisterBase>> registry;
};

fst_EXPORT FlagSingleton& GetFlagSingleton();

#endif // _WIN32

template <typename T>
#ifdef _WIN32
class FlagRegister: public FlagRegisterBase {
#else
class FlagRegister {
#endif
 public:
  static FlagRegister<T> *GetRegister() {
    #ifdef _WIN32
      return GetFlagSingleton().GetRegister<FlagRegister<T>>().get();
    #else
      static auto reg = new FlagRegister<T>;
      return reg;
    #endif
  }

  const FlagDescription<T> &GetFlagDescription(const std::string &name) const {
    fst::MutexLock l(&flag_lock_);
    auto it = flag_table_.find(name);
    return it != flag_table_.end() ? it->second : 0;
  }

  void SetDescription(const std::string &name, const FlagDescription<T> &desc) {
    fst::MutexLock l(&flag_lock_);
    flag_table_.insert(std::make_pair(name, desc));
  }

  bool SetFlag(const std::string &val, bool *address) const {
    if (val == "true" || val == "1" || val.empty()) {
      *address = true;
      return true;
    } else if (val == "false" || val == "0") {
      *address = false;
      return true;
    } else {
      return false;
    }
  }

  bool SetFlag(const std::string &val, std::string *address) const {
    *address = val;
    return true;
  }

  bool SetFlag(const std::string &val, int32_t *address) const {
    char *p = nullptr;
    *address = strtol(val.c_str(), &p, 0);
    return !val.empty() && *p == '\0';
  }

  bool SetFlag(const std::string &val, int64_t *address) const {
    char *p = nullptr;
    *address = strtoll(val.c_str(), &p, 0);
    return !val.empty() && *p == '\0';
  }

  bool SetFlag(const std::string &val, uint64_t *address) const {
    char *p = nullptr;
    *address = strtoull(val.c_str(), &p, 0);
    return !val.empty() && *p == '\0';
  }

  bool SetFlag(const std::string &val, double *address) const {
    char *p = nullptr;
    *address = strtod(val.c_str(), &p);
    return !val.empty() && *p == '\0';
  }

  bool SetFlag(const std::string &arg, const std::string &val) const {
    for (const auto &kv : flag_table_) {
      const auto &name = kv.first;
      const FlagDescription<T> &desc = kv.second;
      if (arg == name) return SetFlag(val, desc.address);
    }
    return false;
  }

  void GetUsage(
      std::set<std::pair<std::string, std::string>> *usage_set) const {
    for (auto it = flag_table_.begin(); it != flag_table_.end(); ++it) {
      const auto &name = it->first;
      const FlagDescription<T> &desc = it->second;
      std::string usage = "  --" + name;
      usage += ": type = ";
      usage += desc.type_name;
      usage += ", default = ";
      usage += GetDefault(desc.default_value) + "\n  ";
      usage += desc.doc_string;
      usage_set->insert(std::make_pair(std::string(desc.file_name), usage));
    }
  }

 private:
  std::string GetDefault(bool default_value) const {
    return default_value ? "true" : "false";
  }

  std::string GetDefault(const std::string &default_value) const {
    return "\"" + default_value + "\"";
  }

  template <class V>
  std::string GetDefault(const V &default_value) const {
    std::ostringstream strm;
    strm << default_value;
    return strm.str();
  }

  mutable fst::Mutex flag_lock_;  // Multithreading lock.
  std::map<std::string, FlagDescription<T>> flag_table_;
};

template <typename T>
class FlagRegisterer {
 public:
  FlagRegisterer(const std::string &name, const FlagDescription<T> &desc) {
    #ifdef _WIN32
    GetFlagSingleton().GetRegister<FlagRegister<T>>()->SetDescription(name, desc);
    #else
    auto registr = FlagRegister<T>::GetRegister();
    registr->SetDescription(name, desc);
    #endif
  }

 private:
  FlagRegisterer(const FlagRegisterer &) = delete;
  FlagRegisterer &operator=(const FlagRegisterer &) = delete;
};


#define DEFINE_VAR(type, name, value, doc)                                    \
  type FST_FLAGS_ ## name = value;                                            \
  static FlagRegisterer<type>                                                 \
  name ## _flags_registerer(#name, FlagDescription<type>(&FST_FLAGS_ ## name, \
                                                         doc,                 \
                                                         #type,               \
                                                         __FILE__,            \
                                                         value))

#define DEFINE_bool(name, value, doc) DEFINE_VAR(bool, name, value, doc)
#define DEFINE_string(name, value, doc) \
  DEFINE_VAR(std::string, name, value, doc)
#define DEFINE_int32(name, value, doc) DEFINE_VAR(int32_t, name, value, doc)
#define DEFINE_int64(name, value, doc) DEFINE_VAR(int64_t, name, value, doc)
#define DEFINE_uint64(name, value, doc) DEFINE_VAR(uint64_t, name, value, doc)
#define DEFINE_double(name, value, doc) DEFINE_VAR(double, name, value, doc)


// Temporary directory.
DECLARE_export_string(tmpdir, fst_EXPORT);

void fst_EXPORT SetFlags(const char *usage, int *argc, char ***argv, bool remove_flags,
              const char *src = "");

// This is an unpleasant hack around SetFlag API.
template <typename Type, typename Value>
void SetFlag(Type *flag, Value value) {
  *flag = Type(value);
}

void fst_EXPORT FailedNewHandler();

#define SET_FLAGS(usage, argc, argv, rmflags) \
std::set_new_handler(FailedNewHandler); \
SetFlags(usage, argc, argv, rmflags, __FILE__)

// Deprecated; for backward compatibility.
inline void InitFst(const char *usage, int *argc, char ***argv, bool rmflags) {
  return SetFlags(usage, argc, argv, rmflags);
}

void fst_EXPORT ShowUsage(bool long_usage = true);

#endif  // FST_FLAGS_H_

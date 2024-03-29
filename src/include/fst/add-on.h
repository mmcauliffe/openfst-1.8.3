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
// FST implementation class to attach an arbitrary object with a read/write
// method to an FST and its file representation. The FST is given a new type
// name.

#ifndef FST_ADD_ON_H_
#define FST_ADD_ON_H_

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>

#include <fst/log.h>
#include <fst/fst.h>
#include <fst/properties.h>
#include <fst/util.h>
#include <string_view>

namespace fst {

// Identifies stream data as an add-on FST.
inline constexpr int32_t kAddOnMagicNumber = 446681434;

// Nothing to save.
class NullAddOn {
 public:
  NullAddOn() = default;

  static NullAddOn *Read(std::istream &strm, const FstReadOptions &opts) {
    return new NullAddOn();
  }

  bool Write(std::ostream &ostrm, const FstWriteOptions &opts) const {
    return true;
  }
};

// Create a new add-on from a pair of add-ons.
template <class A1, class A2>
class AddOnPair {
 public:
  // Argument reference count incremented.
  AddOnPair(std::shared_ptr<A1> a1, std::shared_ptr<A2> a2)
      : a1_(std::move(a1)), a2_(std::move(a2)) {}

  const A1 *First() const { return a1_.get(); }

  const A2 *Second() const { return a2_.get(); }

  std::shared_ptr<A1> SharedFirst() const { return a1_; }

  std::shared_ptr<A2> SharedSecond() const { return a2_; }

  static AddOnPair *Read(std::istream &istrm, const FstReadOptions &opts) {
    bool have_addon1 = false;
    ReadType(istrm, &have_addon1);
    std::unique_ptr<A1> a1;
    if (have_addon1) a1 = fst::WrapUnique(A1::Read(istrm, opts));

    bool have_addon2 = false;
    ReadType(istrm, &have_addon2);
    std::unique_ptr<A1> a2;
    if (have_addon2) a2 = fst::WrapUnique(A2::Read(istrm, opts));

    return new AddOnPair(std::move(a1), std::move(a2));
  }

  bool Write(std::ostream &ostrm, const FstWriteOptions &opts) const {
    bool have_addon1 = a1_ != nullptr;
    WriteType(ostrm, have_addon1);
    if (have_addon1) a1_->Write(ostrm, opts);
    bool have_addon2 = a2_ != nullptr;
    WriteType(ostrm, have_addon2);
    if (have_addon2) a2_->Write(ostrm, opts);
    return true;
  }

 private:
  std::shared_ptr<A1> a1_;
  std::shared_ptr<A2> a2_;
};

namespace internal {

// Adds an object of type T to an FST. T must support:
//
//     T* Read(std::istream &);
//     bool Write(std::ostream &);
//
// The resulting type is a new FST implementation.
template <class FST, class T>
class AddOnImpl : public FstImpl<typename FST::Arc> {
 public:
  using FstType = FST;
  using Arc = typename FST::Arc;
  using Label = typename Arc::Label;
  using StateId = typename Arc::StateId;
  using Weight = typename Arc::Weight;

  using FstImpl<Arc>::SetType;
  using FstImpl<Arc>::SetInputSymbols;
  using FstImpl<Arc>::SetOutputSymbols;
  using FstImpl<Arc>::SetProperties;
  using FstImpl<Arc>::WriteHeader;

  // We make a thread-safe copy of the FST by default since an FST
  // implementation is expected to not share mutable data between objects.
  AddOnImpl(const FST &fst, std::string_view type,
            std::shared_ptr<T> t = nullptr)
      : fst_(fst, true), t_(std::move(t)) {
    SetType(type);
    SetProperties(fst_.Properties(kFstProperties, false));
    SetInputSymbols(fst_.InputSymbols());
    SetOutputSymbols(fst_.OutputSymbols());
  }

  // Conversion from const Fst<Arc> & to F always copies the underlying
  // implementation.
  AddOnImpl(const Fst<Arc> &fst, std::string_view type,
            std::shared_ptr<T> t = nullptr)
      : fst_(fst), t_(std::move(t)) {
    SetType(type);
    SetProperties(fst_.Properties(kFstProperties, false));
    SetInputSymbols(fst_.InputSymbols());
    SetOutputSymbols(fst_.OutputSymbols());
  }

  // We make a thread-safe copy of the FST by default since an FST
  // implementation is expected to not share mutable data between objects.
  AddOnImpl(const AddOnImpl &impl) : fst_(impl.fst_, true), t_(impl.t_) {
    SetType(impl.Type());
    SetProperties(fst_.Properties(kCopyProperties, false));
    SetInputSymbols(fst_.InputSymbols());
    SetOutputSymbols(fst_.OutputSymbols());
  }

  StateId Start() const { return fst_.Start(); }

  Weight Final(StateId s) const { return fst_.Final(s); }

  size_t NumArcs(StateId s) const { return fst_.NumArcs(s); }

  size_t NumInputEpsilons(StateId s) const { return fst_.NumInputEpsilons(s); }

  size_t NumOutputEpsilons(StateId s) const {
    return fst_.NumOutputEpsilons(s);
  }

  size_t NumStates() const { return fst_.NumStates(); }

  static AddOnImpl *Read(std::istream &strm, const FstReadOptions &opts) {
    FstReadOptions nopts(opts);
    FstHeader hdr;
    if (!nopts.header) {
      hdr.Read(strm, nopts.source);
      nopts.header = &hdr;
    }
    // Using `new` to access private constructor for `AddOnImpl`.
    auto impl = fst::WrapUnique(new AddOnImpl(nopts.header->FstType()));
    if (!impl->ReadHeader(strm, nopts, kMinFileVersion, &hdr)) return nullptr;
    impl.reset();
    int32_t magic_number = 0;
    ReadType(strm, &magic_number);  // Ensures this is an add-on FST.
    if (magic_number != kAddOnMagicNumber) {
      LOG(ERROR) << "AddOnImpl::Read: Bad add-on header: " << nopts.source;
      return nullptr;
    }
    FstReadOptions fopts(opts);
    fopts.header = nullptr;  // Contained header was written out.
    std::unique_ptr<FST> fst(FST::Read(strm, fopts));
    if (!fst) return nullptr;
    std::shared_ptr<T> t;
    bool have_addon = false;
    ReadType(strm, &have_addon);
    if (have_addon) {  // Reads add-on object if present.
      t = std::shared_ptr<T>(T::Read(strm, fopts));
      if (!t) return nullptr;
    }
    return new AddOnImpl(*fst, nopts.header->FstType(), t);
  }

  bool Write(std::ostream &strm, const FstWriteOptions &opts) const {
    FstHeader hdr;
    FstWriteOptions nopts(opts);
    nopts.write_isymbols = false;  // Allows contained FST to hold any symbols.
    nopts.write_osymbols = false;
    WriteHeader(strm, nopts, kFileVersion, &hdr);
    WriteType(strm, kAddOnMagicNumber);  // Ensures this is an add-on FST.
    FstWriteOptions fopts(opts);
    fopts.write_header = true;  // Forces writing contained header.
    if (!fst_.Write(strm, fopts)) return false;
    bool have_addon = !!t_;
    WriteType(strm, have_addon);
    // Writes add-on object if present.
    if (have_addon) t_->Write(strm, opts);
    return true;
  }

  void InitStateIterator(StateIteratorData<Arc> *data) const {
    fst_.InitStateIterator(data);
  }

  void InitArcIterator(StateId s, ArcIteratorData<Arc> *data) const {
    fst_.InitArcIterator(s, data);
  }

  FST &GetFst() { return fst_; }

  const FST &GetFst() const { return fst_; }

  const T *GetAddOn() const { return t_.get(); }

  std::shared_ptr<T> GetSharedAddOn() const { return t_; }

  void SetAddOn(std::shared_ptr<T> t) { t_ = t; }

 private:
  explicit AddOnImpl(std::string_view type) : t_() {
    SetType(type);
    SetProperties(kExpanded);
  }

  // Current file format version.
  static constexpr int kFileVersion = 1;
  // Minimum file format version supported.
  static constexpr int kMinFileVersion = 1;

  FST fst_;
  std::shared_ptr<T> t_;

  AddOnImpl &operator=(const AddOnImpl &) = delete;
};

}  // namespace internal
}  // namespace fst

#endif  // FST_ADD_ON_H_

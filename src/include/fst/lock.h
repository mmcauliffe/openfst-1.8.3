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
// Google-compatibility locking declarations and inline definitions.

#ifndef FST_LOCK_H_
#define FST_LOCK_H_

#ifdef OPENFST_HAS_ABSL

namespace fst {

using Mutex;
using MutexLock;
using ReaderMutexLock;

}  // namespace fst

#else  // OPENFST_HAS_ABSL

#include <shared_mutex>  // NOLINT(build/c++14)

namespace fst {

class Mutex {
 public:
  Mutex() = default;

  inline void Lock() { mu_.lock(); }
  inline void Unlock() { mu_.unlock(); }

  inline void ReaderLock() { mu_.lock_shared(); }
  inline void ReaderUnlock() { mu_.unlock_shared(); }

 private:
  std::shared_mutex mu_;

  Mutex(const Mutex &) = delete;
  Mutex &operator=(const Mutex &) = delete;
};

class MutexLock {
 public:
  explicit MutexLock(Mutex *mu) : mu_(mu) { mu_->Lock(); }
  ~MutexLock() { mu_->Unlock(); }

 private:
  Mutex *const mu_;

  MutexLock(const MutexLock &) = delete;
  MutexLock &operator=(const MutexLock &) = delete;
};

class ReaderMutexLock {
 public:
  explicit ReaderMutexLock(Mutex *mu) : mu_(mu) { mu_->ReaderLock(); }
  ~ReaderMutexLock() { mu_->ReaderUnlock(); }

 private:
  Mutex *const mu_;

  ReaderMutexLock(const ReaderMutexLock &) = delete;
  ReaderMutexLock &operator=(const ReaderMutexLock &) = delete;
};

}  // namespace fst

#endif  // OPENFST_HAS_ABSL

#endif  // FST_LOCK_H_

#ifndef OMI_SUPPORT_IMMER_REFCONT_UNSAFEREFCOUNTPOLICY_HPP
#define OMI_SUPPORT_IMMER_REFCONT_UNSAFEREFCOUNTPOLICY_HPP

#include <atomic>
#include <utility>

#include "omi/Support/Immer/RefCount/NoRefCountPolicy.hpp"

namespace omi::support::immer {

/// not thread-safe, a reference counting policy using atomic int count.
struct unsafe_refcount_policy {
  mutable int refcount;

  unsafe_refcount_policy() : refcount{1} {};
  unsafe_refcount_policy(disowned) : refcount{0} {}

  void inc() { ++refcount; }
  bool dec() { return --refcount == 0; }
  bool unique() { return refcount == 1; }
};

}  // namespace omi::support::immer

#endif  // OMI_SUPPORT_IMMER_REFCONT_UNSAFEREFCOUNTPOLICY_HPP
#ifndef OMI_SUPPORT_IMMER_REFCOUNT_REFCOUNTPOLICY_HPP
#define OMI_SUPPORT_IMMER_REFCOUNT_REFCOUNTPOLICY_HPP

#include <atomic>
#include <cassert>
#include <utility>

#include "omi/Support/Immer/RefCount/NoRefCountPolicy.hpp"

namespace omi::support::immer {

/// thread-safe, a reference counting policy using atomic int count.
struct refcount_policy {
  mutable std::atomic<int> refcount;

  refcount_policy() : refcount{1} {};
  refcount_policy(disowned) : refcount{0} {}

  void inc() { refcount.fetch_add(1, std::memory_order_relaxed); }

  bool dec() { return 1 == refcount.fetch_sub(1, std::memory_order_acq_rel); }

  bool unique() { return refcount == 1; }
};

}  // namespace omi::support::immer

#endif  // OMI_SUPPORT_IMMER_REFCOUNT_REFCOUNTPOLICY_HPP
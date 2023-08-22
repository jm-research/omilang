#ifndef OMI_SUPPORT_IMMER_LOCK_NOLOCKPOLICY_HPP
#define OMI_SUPPORT_IMMER_LOCK_NOLOCKPOLICY_HPP

namespace omi::support::immer {

struct no_lock_policy {
  bool try_lock() { return true; }
  void lock() {}
  void unlock() {}

  struct scoped_lock {
    scoped_lock(no_lock_policy&) {}
  };
};

}  // namespace omi::support::immer

#endif  // OMI_SUPPORT_IMMER_LOCK_NOLOCKPOLICY_HPP
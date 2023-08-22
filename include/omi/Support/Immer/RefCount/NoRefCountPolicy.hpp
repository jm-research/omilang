#ifndef OMI_SUPPORT_IMMER_REFCOUNT_NOREFCOUNTPOLICY_HPP
#define OMI_SUPPORT_IMMER_REFCOUNT_NOREFCOUNTPOLICY_HPP

namespace omi::support::immer {

struct disowned {};

/// Disables reference counting.
struct no_refcount_policy {
  no_refcount_policy(){};
  no_refcount_policy(disowned) {}

  void inc() {}
  bool dec() { return false; }
  bool unique() { return false; }
};

}  // namespace omi::support::immer

#endif  // OMI_SUPPORT_IMMER_REFCOUNT_NOREFCOUNTPOLICY_HPP
#ifndef OMI_SUPPORT_IMMER_MEMORYPOLICY_HPP
#define OMI_SUPPORT_IMMER_MEMORYPOLICY_HPP

#include "omi/Support/Immer/Heap/CppHeap.hpp"
#include "omi/Support/Immer/Heap/HeapPolicy.hpp"
#include "omi/Support/Immer/Lock/NoLockPolicy.hpp"
#include "omi/Support/Immer/Lock/SpinLockPolicy.hpp"
#include "omi/Support/Immer/RefCount/NoRefCountPolicy.hpp"
#include "omi/Support/Immer/RefCount/RefCountPolicy.hpp"
#include "omi/Support/Immer/RefCount/UnsafeRefCountPolicy.hpp"
#include "omi/Support/Immer/Transience/GcTransiencePolicy.hpp"
#include "omi/Support/Immer/Transience/NoTransiencePolicy.hpp"

namespace omi::support::immer {

template <typename RefcountPolicy>
struct get_transience_policy
    : std::conditional<std::is_same<RefcountPolicy, no_refcount_policy>::value,
                       gc_transience_policy, no_transience_policy> {};

template <typename T>
using get_transience_policy_t = typename get_transience_policy<T>::type;

template <typename HeapPolicy>
struct get_prefer_fewer_bigger_objects
    : std::integral_constant<
          bool, std::is_same<HeapPolicy, heap_policy<cpp_heap>>::value> {};

template <typename T>
constexpr auto get_prefer_fewer_bigger_objects_v =
    get_prefer_fewer_bigger_objects<T>::value;

template <typename RefcountPolicy>
struct get_use_transient_rvalues
    : std::integral_constant<
          bool, !std::is_same<RefcountPolicy, no_refcount_policy>::value> {};

template <typename T>
constexpr auto get_use_transient_rvalues_v =
    get_use_transient_rvalues<T>::value;

template <typename HeapPolicy, typename RefcountPolicy, typename LockPolicy,
          typename TransiencePolicy = get_transience_policy_t<RefcountPolicy>,
          bool PreferFewerBiggerObjects =
              get_prefer_fewer_bigger_objects_v<HeapPolicy>,
          bool UseTransientRValues =
              get_use_transient_rvalues_v<RefcountPolicy>>
struct memory_policy {
  using heap = HeapPolicy;
  using refcount = RefcountPolicy;
  using transience = TransiencePolicy;
  using lock = LockPolicy;

  static constexpr bool prefer_fewer_bigger_objects = PreferFewerBiggerObjects;

  static constexpr bool use_transient_rvalues = UseTransientRValues;

  using transience_t = typename transience::template apply<heap>::type;
};

using default_heap_policy = free_list_heap_policy<cpp_heap>;

using default_refcount_policy = unsafe_refcount_policy;
using default_lock_policy = no_lock_policy;

using default_memory_policy =
    memory_policy<default_heap_policy, default_refcount_policy,
                  default_lock_policy>;

}  // namespace omi::support::immer

#endif  // OMI_SUPPORT_IMMER_MEMORYPOLICY_HPP
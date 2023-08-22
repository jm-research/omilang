#ifndef OMI_SUPPORT_IMMER_HEAP_IDENTITYHEAP_HPP
#define OMI_SUPPORT_IMMER_HEAP_IDENTITYHEAP_HPP

#include <cstdlib>

namespace omi::support::immer {

template <typename Base>
struct identity_heap : Base {
  template <typename... Tags>
  static void* allocate(std::size_t size, Tags... tags) {
    return Base::allocate(size, tags...);
  }

  template <typename... Tags>
  static void deallocate(std::size_t size, void* data, Tags... tags) {
    Base::deallocate(size, data, tags...);
  }
};

}  // namespace omi::support::immer

#endif  // OMI_SUPPORT_IMMER_HEAP_IDENTITYHEAP_HPP
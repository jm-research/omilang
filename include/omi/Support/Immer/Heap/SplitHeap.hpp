#ifndef OMI_SUPPORT_IMMER_HEAP_SPLITHEAP_HPP
#define OMI_SUPPORT_IMMER_HEAP_SPLITHEAP_HPP

#include <atomic>
#include <cassert>
#include <cstddef>

namespace omi::support::immer {

template <std::size_t Size, typename SmallHeap, typename BigHeap>
struct split_heap {
  template <typename... Tags>
  static void* allocate(std::size_t size, Tags... tags) {
    return size <= Size ? SmallHeap::allocate(size, tags...)
                        : BigHeap::allocate(size, tags...);
  }

  template <typename... Tags>
  static void deallocate(std::size_t size, void* data, Tags... tags) {
    if (size <= Size)
      SmallHeap::deallocate(size, data, tags...);
    else
      BigHeap::deallocate(size, data, tags...);
  }
};

}  // namespace omi::support::immer

#endif  // OMI_SUPPORT_IMMER_HEAP_SPLITHEAP_HPP
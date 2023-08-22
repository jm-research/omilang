#ifndef OMI_SUPPORT_IMMER_HEAP_HEAPPOLICY_HPP
#define OMI_SUPPORT_IMMER_HEAP_HEAPPOLICY_HPP

#include <algorithm>
#include <cstdlib>

#include "omi/Support/Immer/Heap/DebugSizeHeap.hpp"
#include "omi/Support/Immer/Heap/FreeListHeap.hpp"
#include "omi/Support/Immer/Heap/SplitHeap.hpp"
#include "omi/Support/Immer/Heap/ThreadLocalFreeListHeap.hpp"

namespace omi::support::immer {

template <typename Heap>
struct heap_policy {
  using type = Heap;

  template <std::size_t>
  struct optimized {
    using type = Heap;
  };
};

const auto default_bits = 5;
const auto default_free_list_size = 1 << 10;

template <typename Heap, std::size_t Limit = default_free_list_size>
struct free_list_heap_policy {
  using type = debug_size_heap<Heap>;

  template <std::size_t Size>
  struct optimized {
    using type = split_heap<
        Size,
        with_free_list_node<thread_local_free_list_heap<
            Size, Limit, free_list_heap<Size, Limit, debug_size_heap<Heap>>>>,
        debug_size_heap<Heap>>;
  };
};

}  // namespace omi::support::immer

#endif  // OMI_SUPPORT_IMMER_HEAP_HEAPPOLICY_HPP
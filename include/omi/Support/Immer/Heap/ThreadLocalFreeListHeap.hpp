#ifndef OMI_SUPPORT_IMMER_HEAP_THREADLOCALFREELISTHEAP_HPP
#define OMI_SUPPORT_IMMER_HEAP_THREADLOCALFREELISTHEAP_HPP

#include <cstddef>

#include "omi/Support/Immer/Heap/UnsafeFreeListHeap.hpp"

namespace omi::support::immer {
namespace detail {

template <typename Heap>
struct thread_local_free_list_storage {
  struct head_t {
    free_list_node* data;
    std::size_t count;

    ~head_t() { Heap::clear(); }
  };

  static head_t& head() {
    thread_local static head_t head_{nullptr, 0};
    return head_;
  }
};

}  // namespace detail

template <std::size_t Size, std::size_t Limit, typename Base>
struct thread_local_free_list_heap
    : detail::unsafe_free_list_heap_impl<detail::thread_local_free_list_storage,
                                         Size, Limit, Base> {};

}  // namespace omi::support::immer

#endif  // OMI_SUPPORT_IMMER_HEAP_THREADLOCALFREELISTHEAP_HPP
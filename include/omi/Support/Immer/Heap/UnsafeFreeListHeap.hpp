#ifndef OMI_SUPPORT_IMMER_HEAP_UNSAFEFREELISTHEAP_HPP
#define OMI_SUPPORT_IMMER_HEAP_UNSAFEFREELISTHEAP_HPP

#include <cassert>
#include <cstddef>

#include "omi/Support/Immer/Heap/FreeListHeap.hpp"

namespace omi::support::immer {
namespace detail {

template <typename Heap>
struct unsafe_free_list_storage {
  struct head_t {
    free_list_node* data;
    std::size_t count;
  };

  static head_t& head() {
    static head_t head_{nullptr, 0};
    return head_;
  }
};

template <template <class> class Storage, std::size_t Size, std::size_t Limit,
          typename Base>
class unsafe_free_list_heap_impl : Base {
  using storage = Storage<unsafe_free_list_heap_impl>;

 public:
  using base_t = Base;

  template <typename... Tags>
  static void* allocate(std::size_t size, Tags...) {
    assert(size <= sizeof(free_list_node) + Size);
    assert(size >= sizeof(free_list_node));

    auto n = storage::head().data;
    if (!n) {
      auto p = base_t::allocate(Size + sizeof(free_list_node));
      return static_cast<free_list_node*>(p);
    }
    --storage::head().count;
    storage::head().data = n->next;
    return n;
  }

  template <typename... Tags>
  static void deallocate(std::size_t size, void* data, Tags...) {
    assert(size <= sizeof(free_list_node) + Size);
    assert(size >= sizeof(free_list_node));

    if (storage::head().count >= Limit)
      base_t::deallocate(Size + sizeof(free_list_node), data);
    else {
      auto n = static_cast<free_list_node*>(data);
      n->next = storage::head().data;
      storage::head().data = n;
      ++storage::head().count;
    }
  }

  static void clear() {
    while (storage::head().data) {
      auto n = storage::head().data->next;
      base_t::deallocate(Size + sizeof(free_list_node), storage::head().data);
      storage::head().data = n;
      --storage::head().count;
    }
  }
};

}  // namespace detail

template <std::size_t Size, std::size_t Limit, typename Base>
struct unsafe_free_list_heap
    : detail::unsafe_free_list_heap_impl<detail::unsafe_free_list_storage, Size,
                                         Limit, Base> {};

}  // namespace omi::support::immer

#endif  // OMI_SUPPORT_IMMER_HEAP_UNSAFEFREELISTHEAP_HPP
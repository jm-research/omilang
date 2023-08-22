#ifndef OMI_SUPPORT_IMMER_HEAP_FREELISTHEAP_HPP
#define OMI_SUPPORT_IMMER_HEAP_FREELISTHEAP_HPP

#include <atomic>
#include <cassert>
#include <cstddef>

#include "omi/Support/Immer/Heap/FreeListNode.hpp"
#include "omi/Support/Immer/Heap/WithData.hpp"

namespace omi::support::immer {

template <std::size_t Size, std::size_t Limit, typename Base>
struct free_list_heap : Base {
  using base_t = Base;

  template <typename... Tags>
  static void* allocate(std::size_t size, Tags...) {
    assert(size <= sizeof(free_list_node) + Size);
    assert(size >= sizeof(free_list_node));

    free_list_node* n;
    do {
      n = head().data;
      if (!n) {
        auto p = base_t::allocate(Size + sizeof(free_list_node));
        return static_cast<free_list_node*>(p);
      }
    } while (!head().data.compare_exchange_weak(n, n->next));
    head().count.fetch_sub(1u, std::memory_order_relaxed);
    return n;
  }

  template <typename... Tags>
  static void deallocate(std::size_t size, void* data, Tags...) {
    assert(size <= sizeof(free_list_node) + Size);
    assert(size >= sizeof(free_list_node));

    // we use relaxed, because we are fine with temporarily having
    // a few more/less buffers in free list
    if (head().count.load(std::memory_order_relaxed) >= Limit) {
      base_t::deallocate(Size + sizeof(free_list_node), data);
    } else {
      auto n = static_cast<free_list_node*>(data);
      do {
        n->next = head().data;
      } while (!head().data.compare_exchange_weak(n->next, n));
      head().count.fetch_add(1u, std::memory_order_relaxed);
    }
  }

 private:
  struct head_t {
    std::atomic<free_list_node*> data;
    std::atomic<std::size_t> count;
  };

  static head_t& head() {
    static head_t head_{{nullptr}, {0}};
    return head_;
  }
};

}  // namespace omi::support::immer

#endif  // OMI_SUPPORT_IMMER_HEAP_FREELISTHEAP_HPP
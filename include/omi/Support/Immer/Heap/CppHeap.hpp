#ifndef OMI_SUPPORT_IMMER_HEAP_CPPHEAP_HPP
#define OMI_SUPPORT_IMMER_HEAP_CPPHEAP_HPP

#include <cstddef>
#include <memory>

namespace omi::support::immer {

/// A heap that uses `operator new` and `operator delete`.
struct cpp_heap {
  template <typename... Tags>
  static void* allocate(std::size_t size, Tags...) {
    return ::operator new(size);
  }

  static void deallocate(std::size_t, void* data) { ::operator delete(data); }
};

}  // namespace omi::support::immer

#endif  // OMI_SUPPORT_IMMER_HEAP_CPPHEAP_HPP
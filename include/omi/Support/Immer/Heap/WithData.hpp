#ifndef OMI_SUPPORT_IMMER_HEAP_WITHDATA_HPP
#define OMI_SUPPORT_IMMER_HEAP_WITHDATA_HPP

#include <cstdio>

namespace omi::support::immer {

template <typename T, typename Base>
struct with_data : Base {
  using base_t = Base;

  template <typename... Tags>
  static void* allocate(std::size_t size, Tags... tags) {
    auto p = base_t::allocate(size + sizeof(T), tags...);
    return new (p) T{} + 1;
  }

  template <typename... Tags>
  static void deallocate(std::size_t size, void* p, Tags... tags) {
    auto dp = static_cast<T*>(p) - 1;
    dp->~T();
    base_t::deallocate(size + sizeof(T), dp, tags...);
  }
};

}  // namespace omi::support::immer

#endif  // OMI_SUPPORT_IMMER_HEAP_WITHDATA_HPP
#ifndef OMI_RUNTIME_HASH_HPP
#define OMI_RUNTIME_HASH_HPP

#include <functional>

#include "omi/Type/Type.hpp"

namespace omi::runtime::detail {

/// borrowed from boost.
template <typename T>
native_integer hash_combine(size_t const seed, T const& t) {
  static std::hash<T> hasher{};
  return static_cast<native_integer>(seed ^ hasher(t) + 0x9e3779b9 +
                                                (seed << 6) + (seed >> 2));
}

}  // namespace omi::runtime::detail

#endif  // OMI_RUNTIME_HASH_HPP
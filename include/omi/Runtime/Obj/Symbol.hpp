#ifndef OMI_RUNTIME_OBJ_SYMBOL_HPP
#define OMI_RUNTIME_OBJ_SYMBOL_HPP

#include <functional>

#include "omi/Runtime/Behavior/Metadatable.hpp"
#include "omi/Runtime/Hash.hpp"
#include "omi/Runtime/Object.hpp"

namespace omi::runtime::obj {

struct symbol : object, behavior::metadatable {
  static constexpr bool pointer_free = true;

  symbol() = default;
  symbol(symbol&&) = default;
  symbol(const symbol&) = default;
  symbol(const native_string& d);
  symbol(native_string&& d);
  symbol(const native_string& ns, const native_string& n);
  symbol(native_string&& ns, native_string&& n);
  ~symbol() = default;

  native_bool equal(const object&) const final;
  native_string to_string() const final;
  void to_string(fmt::memory_buffer& buff) const final;
  native_integer to_hash() const final;

  const symbol* as_symbol() const final;

  object_ptr with_meta(object_ptr m) const final;
  const behavior::metadatable* as_metadatable() const final;

  bool operator==(const symbol& other) const;

  symbol& operator=(const symbol&) = default;
  symbol& operator=(symbol&&) = default;

  native_string ns;
  native_string name;
};

using symbol_ptr = native_box<symbol>;

}  // namespace omi::runtime::obj

namespace std {

template <>
struct hash<omi::runtime::obj::symbol> {
  size_t operator()(const omi::runtime::obj::symbol& o) const noexcept {
    return static_cast<size_t>(omi::runtime::detail::hash_combine(
        std::hash<omi::native_string>{}(o.ns),
        std::hash<omi::native_string>{}(o.name)));
  }
};

template <>
struct hash<omi::runtime::obj::symbol_ptr> {
  size_t operator()(const omi::runtime::obj::symbol_ptr& o) const noexcept {
    static auto hasher(std::hash<omi::runtime::obj::symbol>{});
    return hasher(*o);
  }
};

template <>
struct equal_to<omi::runtime::obj::symbol_ptr> {
  bool operator()(const omi::runtime::obj::symbol_ptr& lhs,
                  const omi::runtime::obj::symbol_ptr& rhs) const noexcept {
    if (!lhs) {
      return !rhs;
    } else if (!rhs) {
      return !lhs;
    }
    return lhs->equal(*rhs);
  }
};

}  // namespace std

#endif  // OMI_RUNTIME_OBJ_SYMBOL_HPP
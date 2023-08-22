#ifndef OMI_RUNTIME_OBJ_KEYWORD_HPP
#define OMI_RUNTIME_OBJ_KEYWORD_HPP

#include <functional>

#include "omi/Runtime/Hash.hpp"
#include "omi/Runtime/Obj/Symbol.hpp"
#include "omi/Runtime/Object.hpp"

namespace omi::runtime::obj {

struct keyword : object {
  static constexpr bool pointer_free = true;

  keyword() = default;
  keyword(keyword&&) = default;
  keyword(const keyword&) = default;
  keyword(const symbol& s, bool resolved);
  keyword(symbol&& s, bool resolved);
  ~keyword() = default;

  native_bool equal(const object&) const final;
  native_string to_string() const final;
  void to_string(fmt::memory_buffer& buff) const final;
  native_integer to_hash() const final;

  const keyword* as_keyword() const final;

  bool operator==(const keyword& other) const;

  symbol sym;

  /// false: keyword
  /// true: ns alias
  bool resolved{true};

  /// https://github.com/clojure/clojure/blob/master/src/jvm/clojure/lang/Keyword.java
  /// */
  static constexpr size_t hash_magic{0x9e3779b9};
};

using keyword_ptr = native_box<keyword>;

}  // namespace omi::runtime::obj

namespace std {

template <>
struct hash<omi::runtime::obj::keyword> {
  size_t operator()(const omi::runtime::obj::keyword& o) const noexcept {
    return static_cast<size_t>(omi::runtime::detail::hash_combine(
        o.sym.to_hash(), omi::runtime::obj::keyword::hash_magic));
  }
};

template <>
struct hash<omi::runtime::obj::keyword_ptr> {
  size_t operator()(const omi::runtime::obj::keyword_ptr& o) const noexcept {
    static auto hasher(std::hash<omi::runtime::obj::keyword>{});
    return hasher(*o);
  }
};

template <>
struct equal_to<omi::runtime::obj::keyword_ptr> {
  bool operator()(const omi::runtime::obj::keyword_ptr& lhs,
                  const omi::runtime::obj::keyword_ptr& rhs) const noexcept {
    if (!lhs) {
      return !rhs;
    } else if (!rhs) {
      return !lhs;
    }
    return lhs->equal(*rhs);
  }
};

}  // namespace std

#endif  // OMI_RUNTIME_OBJ_KEYWORD_HPP
#ifndef OMI_RUNTIME_OBJECT_HPP
#define OMI_RUNTIME_OBJECT_HPP

#include <fmt/format.h>

#include <cstddef>

#include "omi/Runtime/Behavior/AssociativelyReadable.hpp"
#include "omi/Type/NativeBox.hpp"
#include "omi/Type/Type.hpp"

namespace omi::runtime {
namespace obj {

struct nil;
struct boolean;
struct integer;
struct real;
struct number;
struct string;
struct symbol;
struct keyword;
struct list;
struct vector;
struct map;
struct set;
struct function;

}  // namespace obj

namespace behavior {

struct callable;
struct seqable;
struct countable;
struct metadatable;
struct associatively_readable;
struct associatively_writable;
struct consable;

}  // namespace behavior

struct var;
struct ns;

using object_ptr = native_box<struct object>;

struct object : gc {
  virtual native_bool equal(const object& other) const;
  native_bool equal(object_ptr) const;

  virtual native_string to_string() const = 0;
  virtual void to_string(fmt::memory_buffer& buf) const = 0;
  virtual native_integer to_hash() const = 0;

  bool operator<(const object&) const;

  // clang-format off
  virtual const var* as_var() const { return nullptr; }
  virtual const ns* as_ns() const { return nullptr; }
  virtual const obj::nil* as_nil() const { return nullptr; }
  virtual const obj::boolean* as_boolean() const { return nullptr; }
  virtual const obj::integer* as_integer() const { return nullptr; }
  virtual const obj::real* as_real() const { return nullptr; }
  virtual const obj::number* as_number() const { return nullptr; }
  virtual const obj::string* as_string() const { return nullptr; }
  virtual const obj::symbol* as_symbol() const { return nullptr; }
  virtual const obj::keyword* as_keyword() const { return nullptr; }
  virtual const obj::list* as_list() const { return nullptr; }
  virtual const obj::vector* as_vector() const { return nullptr; }
  virtual const obj::map* as_map() const { return nullptr; }
  virtual const obj::set* as_set() const { return nullptr; }
  virtual const obj::function* as_function() const { return nullptr; }
  virtual const behavior::seqable* as_seqable() const { return nullptr; }
  virtual const behavior::callable* as_callable() const { return nullptr; }
  virtual const behavior::metadatable* as_metadatable() const { return nullptr; }
  virtual const behavior::countable* as_countable() const { return nullptr; }
  virtual const behavior::associatively_readable* as_associatively_readable() const { return nullptr; }
  virtual const behavior::associatively_writable* as_associatively_writable() const { return nullptr; }
  virtual const behavior::consable* as_consable() const { return nullptr; }
  // clang-format on
};

std::ostream& operator<<(std::ostream& os, const object& obj);

namespace detail {

struct object_ptr_equal {
  static bool equal(object_ptr l, object_ptr r) {
    return l == r || l->equal(*r);
  }

  inline bool operator()(object_ptr l, object_ptr r) const {
    return equal(l, r);
  }
};

struct object_ptr_less {
  inline bool operator()(object_ptr l, object_ptr r) const {
    auto const l_hash(l->to_hash());
    auto const r_hash(r->to_hash());
    return l_hash < r_hash;
  }
};

}  // namespace detail

namespace obj {

struct nil : object, omi::runtime::behavior::associatively_readable {
  static constexpr bool pointer_free{true};

  nil() = default;
  nil(nil&&) noexcept = default;
  nil(const nil&) = default;
  virtual ~nil() = default;

  native_bool equal(const object&) const final;
  native_string to_string() const final;
  native_integer to_hash() const final;

  const nil* as_nil() const final;

  const behavior::associatively_readable* as_associatively_readable()
      const final;
  object_ptr get(object_ptr key) const;
  object_ptr get(object_ptr key, object_ptr fallback) const;
};

using nil_ptr = native_box<nil>;

extern native_box<boolean> OMI_TRUE;
extern native_box<boolean> OMI_FALSE;
}  // namespace obj
extern obj::nil_ptr OMI_NIL;

}  // namespace omi::runtime

#endif  // OMI_RUNTIME_OBJECT_HPP
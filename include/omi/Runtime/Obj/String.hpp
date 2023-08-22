#ifndef OMI_RUNTIME_OBJ_STRING_HPP
#define OMI_RUNTIME_OBJ_STRING_HPP

#include "omi/Runtime/Behavior/Countable.hpp"
#include "omi/Runtime/Behavior/Metadatable.hpp"
#include "omi/Runtime/Object.hpp"

namespace omi::runtime::obj {

struct string : object, behavior::countable, behavior::metadatable {
  static constexpr bool pointer_free = true;

  string() = default;
  string(string&&) = default;
  string(const string&) = default;
  string(native_string&& d);
  string(const native_string& d);
  ~string() = default;

  native_bool equal(const object&) const final;
  native_string to_string() const final;
  void to_string(fmt::memory_buffer& buff) const final;
  native_integer to_hash() const final;

  const string* as_string() const final;

  size_t count() const final;

  object_ptr with_meta(object_ptr m) const final;
  const behavior::metadatable* as_metadatable() const final;

  native_string data;
};

using string_ptr = native_box<string>;

}  // namespace omi::runtime::obj

#endif  // OMI_RUNTIME_OBJ_STRING_HPP
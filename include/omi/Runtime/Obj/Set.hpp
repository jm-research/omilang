#ifndef OMI_RUNTIME_OBJ_SET_HPP
#define OMI_RUNTIME_OBJ_SET_HPP

#include "omi/Runtime/Behavior/Countable.hpp"
#include "omi/Runtime/Behavior/Metadatable.hpp"
#include "omi/Runtime/Behavior/Seqable.hpp"
#include "omi/Runtime/Object.hpp"

namespace omi::runtime::obj {

struct set : virtual object,
             behavior::seqable,
             behavior::countable,
             behavior::metadatable {
  static constexpr bool pointer_free = false;

  set() = default;
  set(set&&) = default;
  set(const set&) = default;
  set(runtime::detail::persistent_set&& d);
  set(const runtime::detail::persistent_set& d);
  ~set() = default;

  native_bool equal(const object&) const final;
  native_string to_string() const final;
  void to_string(fmt::memory_buffer& buff) const final;
  native_integer to_hash() const final;

  const set* as_set() const final;
  const behavior::seqable* as_seqable() const final;

  behavior::sequence_ptr seq() const final;
  behavior::sequence_ptr fresh_seq() const final;

  size_t count() const final;

  object_ptr with_meta(object_ptr m) const final;
  const behavior::metadatable* as_metadatable() const final;

  runtime::detail::persistent_set data;
};

}  // namespace omi::runtime::obj

#endif  // OMI_RUNTIME_OBJ_SET_HPP
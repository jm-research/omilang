#ifndef OMI_RUNTIME_OBJ_LIST_HPP
#define OMI_RUNTIME_OBJ_LIST_HPP

#include "omi/Runtime/Behavior/Consable.hpp"
#include "omi/Runtime/Behavior/Countable.hpp"
#include "omi/Runtime/Behavior/Metadatable.hpp"
#include "omi/Runtime/Behavior/Seqable.hpp"
#include "omi/Runtime/Object.hpp"

namespace omi::runtime::obj {

struct list : virtual object,
              behavior::seqable,
              behavior::countable,
              behavior::consable,
              behavior::metadatable {
  static constexpr bool pointer_free = false;

  list() = default;
  list(list&&) = default;
  list(const list&) = default;
  list(runtime::detail::persistent_list&& d);
  list(const runtime::detail::persistent_list& d);
  template <typename... Args>
  list(Args&&... args) : data(std::forward<Args>(args)...) {}
  ~list() = default;

  static native_box<list> create(const behavior::sequence_ptr& s);
  native_bool equal(const object& other) const final;
  native_string to_string() const final;
  void to_string(fmt::memory_buffer& buff) const final;
  native_integer to_hash() const final;

  const list* as_list() const final;
  const behavior::seqable* as_seqable() const final;

  behavior::sequence_ptr seq() const final;
  behavior::sequence_ptr fresh_seq() const final;
  size_t count() const final;

  const behavior::consable* as_consable() const final;
  native_box<behavior::sequence> cons(object_ptr head) const final;

  object_ptr with_meta(object_ptr m) const final;
  const behavior::metadatable* as_metadatable() const final;

  runtime::detail::persistent_list data;
};

using list_ptr = native_box<list>;

}  // namespace omi::runtime::obj

#endif  // OMI_RUNTIME_OBJ_LIST_HPP
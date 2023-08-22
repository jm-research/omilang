#ifndef OMI_RUNTIME_OBJ_VECTOR_HPP
#define OMI_RUNTIME_OBJ_VECTOR_HPP

#include "omi/Runtime/Behavior/AssociativelyReadable.hpp"
#include "omi/Runtime/Behavior/Consable.hpp"
#include "omi/Runtime/Behavior/Countable.hpp"
#include "omi/Runtime/Behavior/Metadatable.hpp"
#include "omi/Runtime/Behavior/Seqable.hpp"
#include "omi/Runtime/Object.hpp"

namespace omi::runtime::obj {

struct vector : virtual object,
                behavior::seqable,
                behavior::countable,
                behavior::consable,
                behavior::metadatable,
                behavior::associatively_readable {
  static constexpr bool pointer_free = false;

  vector() = default;
  vector(vector&&) = default;
  vector(const vector&) = default;
  vector(runtime::detail::persistent_vector&& d);
  vector(const runtime::detail::persistent_vector& d);
  template <typename... Args>
  vector(Args&&... args) : data(std::forward<Args>(args)...) {}
  ~vector() = default;

  static native_box<vector> create(const behavior::sequence_ptr& s);
  static native_box<vector> create(const runtime::detail::persistent_vector& v);

  native_bool equal(const object& other) const final;
  native_string to_string() const final;
  void to_string(fmt::memory_buffer& buff) const final;
  native_integer to_hash() const final;

  const vector* as_vector() const final;
  const behavior::seqable* as_seqable() const final;

  behavior::sequence_ptr seq() const final;
  behavior::sequence_ptr fresh_seq() const final;
  size_t count() const final;

  const behavior::consable* as_consable() const final;
  native_box<behavior::sequence> cons(object_ptr head) const final;

  object_ptr with_meta(object_ptr m) const final;
  const behavior::metadatable* as_metadatable() const final;

  const behavior::associatively_readable* as_associatively_readable()
      const final;
  object_ptr get(object_ptr key) const final;
  object_ptr get(object_ptr key, object_ptr default_value) const final;

  runtime::detail::persistent_vector data;
};

using vector_ptr = native_box<vector>;

}  // namespace omi::runtime::obj

#endif  // OMI_RUNTIME_OBJ_VECTOR_HPP
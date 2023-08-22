#ifndef OMI_RUNTIME_OBJ_MAP_HPP
#define OMI_RUNTIME_OBJ_MAP_HPP

#include "omi/Runtime/Behavior/AssociativelyReadable.hpp"
#include "omi/Runtime/Behavior/AssociativelyWritable.hpp"
#include "omi/Runtime/Behavior/Countable.hpp"
#include "omi/Runtime/Behavior/Metadatable.hpp"
#include "omi/Runtime/Behavior/Seqable.hpp"
#include "omi/Runtime/Object.hpp"

namespace omi::runtime::obj {

struct map : object,
             behavior::seqable,
             behavior::countable,
             behavior::metadatable,
             behavior::associatively_readable,
             behavior::associatively_writable {
  using value_type = runtime::detail::persistent_map;

  static constexpr bool pointer_free = false;

  map() = default;
  map(map&&) = default;
  map(const map&) = default;
  map(runtime::detail::persistent_map&& d);
  map(const runtime::detail::persistent_map& d);
  template <typename... Args>
  map(runtime::detail::in_place_unique, Args&&... args)
      : data(runtime::detail::in_place_unique{}, std::forward<Args>(args)...) {}
  ~map() = default;

  static native_box<map> create(const runtime::detail::persistent_map& m);

  native_bool equal(const object& other) const final;
  native_string to_string() const final;
  void to_string(fmt::memory_buffer& buff) const final;
  native_integer to_hash() const final;

  const map* as_map() const final;
  const behavior::seqable* as_seqable() const final;

  behavior::sequence_ptr seq() const final;
  behavior::sequence_ptr fresh_seq() const final;

  size_t count() const final;

  object_ptr with_meta(object_ptr m) const final;
  const behavior::metadatable* as_metadatable() const final;

  const behavior::associatively_readable* as_associatively_readable()
      const final;
  object_ptr get(object_ptr key) const final;
  object_ptr get(object_ptr key, object_ptr fallback) const final;

  const behavior::associatively_writable* as_associatively_writable()
      const final;
  object_ptr assoc(object_ptr key, object_ptr value) const final;

  runtime::detail::persistent_map data;
};

using map_ptr = native_box<map>;

}  // namespace omi::runtime::obj

#endif  // OMI_RUNTIME_OBJ_MAP_HPP
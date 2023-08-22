#ifndef OMI_RUNTIME_OBJ_ITERATOR_HPP
#define OMI_RUNTIME_OBJ_ITERATOR_HPP

#include "omi/Runtime/Behavior/Callable.hpp"
#include "omi/Runtime/Behavior/Seqable.hpp"

namespace omi::runtime::obj {

struct iterator : behavior::sequence {
  static constexpr bool pointer_free = false;

  iterator(behavior::callable_ptr fn, object_ptr start);

  behavior::sequence_ptr seq() const final;
  behavior::sequence_ptr fresh_seq() const final;
  object_ptr first() const final;
  behavior::sequence_ptr next() const final;
  behavior::sequence_ptr next_in_place() final;
  object_ptr next_in_place_first() final;

  void to_string(fmt::memory_buffer& buff) const final;
  native_string to_string() const final;

  behavior::callable_ptr fn{};
  object_ptr current{};
  object_ptr previous{};
  mutable behavior::sequence_ptr cached_next{};
};

}  // namespace omi::runtime::obj

#endif  // OMI_RUNTIME_OBJ_ITERATOR_HPP
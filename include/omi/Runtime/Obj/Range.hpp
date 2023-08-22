#ifndef OMI_RUNTIME_OBJ_RANGE_HPP
#define OMI_RUNTIME_OBJ_RANGE_HPP

#include "omi/Runtime/Behavior/Seqable.hpp"

namespace omi::runtime::obj {

struct range : behavior::sequence {
  static constexpr bool pointer_free = false;

  range(const object_ptr end);
  range(const object_ptr start, const object_ptr end);
  range(const object_ptr start, const object_ptr end, const object_ptr step);

  behavior::sequence_ptr seq() const final;
  behavior::sequence_ptr fresh_seq() const final;
  object_ptr first() const final;
  behavior::sequence_ptr next() const final;
  behavior::sequence_ptr next_in_place() final;
  object_ptr next_in_place_first() final;

  void to_string(fmt::memory_buffer& buff) const final;
  native_string to_string() const final;

  object_ptr start{};
  object_ptr end{};
  object_ptr step{};
  mutable behavior::sequence_ptr cached_next{};
};

}  // namespace omi::runtime::obj

#endif  // OMI_RUNTIME_OBJ_RANGE_HPP
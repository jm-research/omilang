#include "omi/Runtime/Behavior/Seqable.hpp"

#include <omi/Type/Type.hpp>

#include "omi/Runtime/Hash.hpp"
#include "omi/Runtime/Obj/Cons.hpp"

namespace omi::runtime::behavior {

native_box<consable> sequence::cons(object_ptr head) const {
  return make_box<obj::cons>(head, const_cast<sequence_ptr>(this));
}

const behavior::seqable* sequence::as_seqable() const { return this; }

native_bool sequence::equal(const object& obj) const {
  const auto* obj_seqable(obj.as_seqable());
  if (!obj_seqable) {
    return false;
  }

  auto obj_seq{obj_seqable->seq()};
  auto this_seq{seq()};
  while (this_seq != nullptr && obj_seq != nullptr) {
    if (!this_seq->first()->equal(*obj_seq->first())) {
      return false;
    }

    this_seq = this_seq->next_in_place();
    obj_seq = obj_seq->next_in_place();
  }
  return true;
}

native_integer sequence::to_hash() const {
  if (hash != 0) {
    return static_cast<native_integer>(hash);
  }

  for (auto it(this); it != nullptr; it = it->next()) {
    hash = runtime::detail::hash_combine(hash, it->first()->to_hash());
  }

  return static_cast<native_integer>(hash);
}

void vector_sequence::to_string(fmt::memory_buffer& buff) const {
  detail::to_string(arr.begin() + index, arr.end(), '(', ')', buff);
}

native_string vector_sequence::to_string() const {
  fmt::memory_buffer buff;
  detail::to_string(arr.begin() + index, arr.end(), '(', ')', buff);
  return native_string{buff.data(), buff.size()};
}

native_integer vector_sequence::to_hash() const {
  return reinterpret_cast<native_integer>(this);
}

sequence_ptr vector_sequence::seq() const {
  return static_cast<sequence_ptr>(const_cast<vector_sequence*>(this));
}

sequence_ptr vector_sequence::fresh_seq() const {
  return make_box<vector_sequence>(arr, index);
}

const countable* vector_sequence::as_countable() const { return this; }

size_t vector_sequence::count() const { return arr.size(); }

object_ptr vector_sequence::first() const { return arr[index]; }

sequence_ptr vector_sequence::next() const {
  auto idx(index);
  ++idx;
  if (idx == arr.size()) {
    return nullptr;
  }
  return make_box<vector_sequence>(arr, idx);
}

sequence_ptr vector_sequence::next_in_place() {
  ++index;
  if (index == arr.size()) {
    return nullptr;
  }
  return this;
}

object_ptr vector_sequence::next_in_place_first() {
  ++index;
  if (index == arr.size()) {
    return nullptr;
  }
  return arr[index];
}

}  // namespace omi::runtime::behavior
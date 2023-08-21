#ifndef OMI_RUNTIME_BEHAVIOR_SEQABLE_HPP
#define OMI_RUNTIME_BEHAVIOR_SEQABLE_HPP

#include "omi/Runtime/Behavior/Consable.hpp"
#include "omi/Runtime/Behavior/Countable.hpp"
#include "omi/Type/NativeBox.hpp"

namespace omi::runtime {

namespace obj {
struct cons;
}  // namespace obj

namespace behavior {

struct seqable {
  virtual ~seqable() = default;

  virtual native_box<struct sequence> seq() const = 0;
  virtual native_box<struct sequence> fresh_seq() const = 0;
};

using seqable_ptr = native_box<const seqable>;

struct sequence : virtual object, seqable, consable {
  using sequence_ptr = native_box<sequence>;

  virtual object_ptr first() const = 0;
  virtual sequence_ptr next() const = 0;

  /// 每次调用next()都会分配一个新的sequence_ptr，但当在一个大序列上迭代时
  /// 会发生大量的内存分配。但是如果拥有sequence_ptr，可以使用此函数对其进行
  /// in place修改则不会发生内存分配。
  /// 如果不拥有sequence_ptr，则可以对它调用一次next()来获得一个sequence_ptr，
  /// 然后对它调用next_in_place()。
  virtual sequence_ptr next_in_place() = 0;
  virtual object_ptr next_in_place_first() = 0;

  native_box<consable> cons(object_ptr head) const final;
  const seqable* as_seqable() const final;
  native_bool equal(const object& other) const final;
  native_integer to_hash() const override;

  mutable size_t hash{};
};

using sequence_ptr = sequence::sequence_ptr;

namespace detail {

template <typename It>
void to_string(It const& begin, It const& end, char const open,
               char const close, fmt::memory_buffer& buff) {
  auto inserter(std::back_inserter(buff));
  inserter = open;
  for (auto i(begin); i != end; ++i) {
    (*i)->to_string(buff);
    auto n(i);
    if (++n != end) {
      inserter = ' ';
    }
  }
  inserter = close;
}

}  // namespace detail

template <typename Iterator>
struct basic_iterator_wrapper : sequence, countable {
  static constexpr bool pointer_free = false;

  basic_iterator_wrapper() = default;
  basic_iterator_wrapper(const object_ptr& c, const Iterator& b,
                         const Iterator& e, const size_t s)
      : coll(c), begin(b), end(e), size(s) {
    if (begin == end) {
      throw std::runtime_error{"basic_iterator_wrapper for empty sequence"};
    }
  }

  native_string to_string() const final {
    fmt::memory_buffer buff;
    detail::to_string(begin, end, '(', ')', buff);
    return native_string{buff.data(), buff.size()};
  }

  void to_string(fmt::memory_buffer& buff) const final {
    detail::to_string(begin, end, '(', ')', buff);
  }

  native_integer to_hash() const final {
    return reinterpret_cast<native_integer>(this);
  }

  sequence_ptr seq() const final {
    return static_cast<sequence_ptr>(
        const_cast<basic_iterator_wrapper<Iterator>*>(this));
  }

  sequence_ptr fresh_seq() const final {
    return make_box<basic_iterator_wrapper<Iterator>>(coll, begin, end, size);
  }

  const countable* as_countable() const final { return this; }
  size_t count() const final { return size; }

  object_ptr first() const final { return *begin; }
  sequence_ptr next() const final {
    auto it(begin);
    ++it;
    if (it == end) {
      return nullptr;
    }
    return make_box<basic_iterator_wrapper<Iterator>>(coll, it, end, size);
  }

  sequence_ptr next_in_place() final {
    ++begin;
    if (begin == end) {
      return nullptr;
    }
    return this;
  }

  object_ptr next_in_place_first() final {
    ++begin;
    if (begin == end) {
      return nullptr;
    }
    return *begin;
  }

  object_ptr coll{};
  Iterator begin, end;
  size_t size{};
};

template <size_t N>
struct array_sequence : sequence, countable {
  static constexpr bool pointer_free = false;

  array_sequence() = default;
  array_sequence(const std::array<object_ptr, N>& arr, const size_t index)
      : arr(arr), index(index) {}
  array_sequence(std::array<object_ptr, N>&& arr, const size_t index)
      : arr(std::move(arr)), index(index) {}
  array_sequence(std::array<object_ptr, N>&& arr) : arr(std::move(arr)) {}
  template <typename... Args>
  array_sequence(object_ptr first, Args... rest) : arr(first, rest...) {}

  void to_string(fmt::memory_buffer& buff) const final {
    detail::to_string(arr.begin() + index, arr.end(), '(', ')', buff);
  }

  native_string to_string() const final {
    fmt::memory_buffer buff;
    detail::to_string(arr.begin() + index, arr.end(), '(', ')', buff);
    return native_string{buff.data(), buff.size()};
  }

  native_integer to_hash() const final {
    return reinterpret_cast<native_integer>(this);
  }

  sequence_ptr seq() const final {
    return static_cast<sequence_ptr>(const_cast<array_sequence*>(this));
  }

  sequence_ptr fresh_seq() const final {
    return make_box<array_sequence<N>>(arr, index);
  }

  const countable* as_countable() const final { return this; }
  size_t count() const final { return N; }

  object_ptr first() const final { return arr[index]; }
  sequence_ptr next() const final {
    auto idx(index);
    ++idx;
    if (idx == N) {
      return nullptr;
    }
    return make_box<array_sequence<N>>(arr, idx);
  }

  sequence_ptr next_in_place() final {
    ++index;
    if (index == N) {
      return nullptr;
    }
    return this;
  }

  object_ptr next_in_place_first() final {
    ++index;
    if (index == N) {
      return nullptr;
    }
    return arr[index];
  }

  std::array<object_ptr, N> arr;
  size_t index{};
};

struct vector_sequence : sequence, countable {
  static constexpr bool pointer_free = false;

  vector_sequence() = default;
  vector_sequence(const native_vector<object_ptr>& arr, const size_t index)
      : arr(arr), index(index) {}
  vector_sequence(native_vector<object_ptr>&& arr, const size_t index)
      : arr(std::move(arr)), index(index) {}
  vector_sequence(native_vector<object_ptr>&& arr) : arr(std::move(arr)) {}

  void to_string(fmt::memory_buffer& buff) const final;
  native_string to_string() const final;
  native_integer to_hash() const final;

  sequence_ptr seq() const final;
  sequence_ptr fresh_seq() const final;
  const countable* as_countable() const final;
  size_t count() const final;
  object_ptr first() const final;
  sequence_ptr next() const final;
  sequence_ptr next_in_place() final;
  object_ptr next_in_place_first() final;

  native_vector<object_ptr> arr;
  size_t index{};
};

}  // namespace behavior

}  // namespace omi::runtime

#endif  // OMI_RUNTIME_BEHAVIOR_SEQABLE_HPP
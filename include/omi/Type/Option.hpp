#ifndef OMI_TYPE_OPTION_HPP
#define OMI_TYPE_OPTION_HPP

#include <utility>

namespace omi {

struct none_t {};

namespace detail {
template <bool OK, typename T>
struct result;
}

template <typename T>
struct option {
  using storage_type = char[sizeof(T)];

  option() = default;

  option(const option<T>& other) : set(other.set) {
    if (set) {
      new (reinterpret_cast<T*>(data))
          T(*reinterpret_cast<const T*>(other.data));
    }
  }

  option(option&& other) noexcept : set{std::move(other.set)} {
    other.set = false;
    if (set) {
      new (reinterpret_cast<T*>(data))
          T{std::move(*reinterpret_cast<const T*>(other.data))};
    }
  }

  ~option() { reset(); }

  void reset() noexcept {
    if (set) {
      reinterpret_cast<T*>(data)->~T();
    }
    set = false;
  }

  bool set{};
  alignas(alignof(T)) storage_type data{};
};

}  // namespace omi

#endif  // OMI_TYPE_OPTION_HPP

#ifndef OMI_TYPE_OPTION_HPP
#define OMI_TYPE_OPTION_HPP

#include <cassert>
#include <ostream>
#include <utility>

namespace omi {

struct none_t {};

namespace detail {
template <bool OK, typename T>
struct result;
}  // namespace detail

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

  template <typename D = T>
  option(D&& d, std::enable_if_t<std::is_constructible_v<T, D> &&
                                 !std::is_same_v<std::decay_t<D>, option<T>>>* =
                    nullptr)
      : set{true} {
    new (reinterpret_cast<T*>(data)) T{std::forward<D>(d)};
  }

  template <typename D>
  option(const option<D>& o,
         std::enable_if_t<std::is_constructible_v<T, D>>* = nullptr)
      : set{o.set} {
    if (set) {
      new (reinterpret_cast<T*>(data)) T{*reinterpret_cast<const D*>(o.data)};
    }
  }

  template <typename D>
  option(option<D>&& o,
         std::enable_if_t<std::is_constructible_v<T, D>>* = nullptr)
      : set{std::move(o.set)} {
    if (set) {
      new (reinterpret_cast<T*>(data))
          T{std::move(*reinterpret_cast<D*>(o.data))};
    }
    o.reset();
  }

  option(const none_t&) {}

  option<T>& operator=(const option<T>& rhs) {
    if (this == &rhs) {
      return *this;
    }
    reset();

    set = rhs.set;
    if (set) {
      new (reinterpret_cast<T*>(data)) T{*reinterpret_cast<const T*>(rhs.data)};
    }
    return *this;
  }

  option<T>& operator=(option<T>&& rhs) noexcept {
    if (this == &rhs) {
      return *this;
    }
    reset();

    set = std::move(rhs.set);
    if (set) {
      new (reinterpret_cast<T*>(data))
          T{std::move(*reinterpret_cast<const T*>(rhs.data))};
    }
    rhs.reset();
    return *this;
  }

  option<T>& operator=(const none_t&) {
    reset();
    return *this;
  }

  template <typename D>
  std::enable_if_t<std::is_constructible_v<T, D>, option<T>&> operator=(
      D&& rhs) {
    reset();

    set = true;
    new (reinterpret_cast<T*>(data)) T{std::forward<D>(rhs)};
    return *this;
  }

  void reset() noexcept {
    if (set) {
      reinterpret_cast<T*>(data)->~T();
    }
    set = false;
  }

  bool is_some() const { return set; }
  bool is_none() const { return !set; }

  T& unwrap() {
    assert(set);
    return *reinterpret_cast<T*>(data);
  }

  const T& unwrap() const {
    assert(set);
    return *reinterpret_cast<const T*>(data);
  }

  T& unwrap_or(T& fallback) {
    if (set) {
      return *reinterpret_cast<T*>(data);
    }
    return fallback;
  }

  T unwrap_or(T fallback) const {
    if (set) {
      return *reinterpret_cast<const T*>(data);
    }
    return std::move(fallback);
  }

  bool operator!=(const option<T>& rhs) const {
    if (set != rhs.set) {
      return true;
    } else if (set) {
      return *reinterpret_cast<const T*>(data) !=
             *reinterpret_cast<const T*>(rhs.data);
    }
    return true;
  }

  bool operator==(const option<T>& rhs) const { return !(*this != rhs); }

  bool operator!=(const T& rhs) const {
    return !set || (*reinterpret_cast<const T*>(data) != rhs);
  }

  bool operator==(const T& rhs) const { return !(*this != rhs); }

  bool set{};
  alignas(alignof(T)) storage_type data{};
};

template <typename T, typename Decayed = std::decay_t<T>>
option<Decayed> some(T&& t) {
  return {std::forward<T>(t)};
}

inline constexpr none_t none = none_t{};

template <typename T>
std::ostream& operator<<(std::ostream& os, const option<T>& o) {
  if (o.is_none()) {
    return os << "none";
  }
  return os << "some(" << o.unwrap() << ")";
}

}  // namespace omi

#endif  // OMI_TYPE_OPTION_HPP

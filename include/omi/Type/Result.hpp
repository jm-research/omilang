#ifndef OMI_TYPE_RESULT_HPP
#define OMI_TYPE_RESULT_HPP

#include <cstdlib>
#include <iostream>
#include <variant>

#include "omi/Type/Option.hpp"

namespace omi {

namespace detail {

template <bool OK, typename T>
struct result {
  result(const T& t) : data(t) {}
  result(T&& t) : data(std::move(t)) {}

  T data;
};

}  // namespace detail

template <typename R, typename Decayed = std::decay_t<R>>
detail::result<true, Decayed> ok(R&& data) {
  return {std::forward<R>(data)};
}

template <typename E, typename Decayed = std::decay_t<E>>
detail::result<false, Decayed> err(E&& data) {
  return {std::forward<E>(data)};
}

template <typename R, typename E>
struct result {
  result(detail::result<true, R>&& r) : data{std::move(r.data)} {}
  result(detail::result<false, E>&& e) : data{std::move(e.data)} {}
  /// Allow implicit construction of R and E from their ctor args.
  template <typename T>
  result(T&& t, std::enable_if_t<std::is_constructible_v<R, T>>* = nullptr)
      : data{std::forward<T>(t)} {}
  template <typename T>
  result(T&& t, std::enable_if_t<std::is_constructible_v<E, T>>* = nullptr)
      : data{std::forward<T>(t)} {}
  template <typename T>
  result(const detail::result<true, T>& t,
         std::enable_if_t<std::is_constructible_v<R, T>>* = nullptr)
      : data{t.data} {}
  template <typename T>
  result(const detail::result<false, T>& t,
         std::enable_if_t<std::is_constructible_v<E, T>>* = nullptr)
      : data{t.data} {}

  bool is_ok() const { return data.index() == 0; }
  bool is_err() const { return data.index() == 1; }

  void assert_ok() const {
    if (is_ok()) {
      return;
    }

    auto const& err(expect_err());
    std::cout << "error: expected ok result, but found: " << err << std::endl;
    throw err;
  }

  const R& expect_ok() const {
    assert_ok();
    return std::get<0>(data);
  }

  R* expect_ok_ptr() {
    assert_ok();
    return &std::get<R>(data);
  }

  const R* expect_ok_ptr() const {
    assert_ok();
    return &std::get<R>(data);
  }

  R expect_ok_move() {
    assert_ok();
    return std::move(std::get<R>(data));
  }

  option<R> ok() {
    if (is_ok()) {
      return some(std::get<R>(data));
    }
    return none;
  }

  const E& expect_err() const { return std::get<E>(data); }

  E* expect_err_ptr() { return &std::get<E>(data); }

  const E* expect_err_ptr() const { return &std::get<E>(data); }

  E expect_err_move() { return std::move(std::get<E>(data)); }

  option<E> err() {
    if (is_err()) {
      return some(std::get<E>(data));
    }
    return none;
  }

  /// move value.
  R unwrap_move() {
    if (!is_ok()) {
      std::abort();
    }
    return std::move(std::get<R>(data));
  }

  bool operator==(const result& rhs) const { return rhs.data == data; }
  bool operator!=(const result& rhs) const { return rhs.data != data; }
  bool operator==(const R& rhs) const {
    return data.index() == 0 && rhs == std::get<R>(data);
  }
  bool operator==(const E& rhs) const {
    return data.index() == 1 && rhs == std::get<E>(data);
  }

  std::variant<R, E> data;
};

template <typename R, typename E>
std::ostream& operator<<(std::ostream& os, const result<R, E>& r) {
  if (r.is_ok()) {
    return os << "ok(" << std::get<R>(r.data) << ")";
  }
  return os << "err(" << std::get<E>(r.data) << ")";
}

}  // namespace omi

#endif  // OMIT_TYPE_RESULT_HPP
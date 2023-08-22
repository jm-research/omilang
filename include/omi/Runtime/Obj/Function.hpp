#ifndef OMI_RUNTIME_OBJ_FUNCTION_HPP
#define OMI_RUNTIME_OBJ_FUNCTION_HPP

#include <any>
#include <functional>
#include <iostream>
#include <type_traits>

#include "omi/Runtime/Behavior/Callable.hpp"
#include "omi/Runtime/Behavior/Metadatable.hpp"
#include "omi/Runtime/Object.hpp"

namespace omi::runtime::obj {

namespace detail {

struct function_type {
  template <typename T>
  using value_type = std::function<T>;

  function_type() = default;

  template <typename R, typename... Args>
  function_type(R (*const f)(Args...))
      : function_type(value_type<R(Args...)>(f)) {}

  template <typename R, typename... Args>
  function_type(value_type<R(Args...)>&& f) : value(std::move(f)) {}

  template <typename R, typename... Args>
  function_type(const value_type<R(Args...)>& f) : value(f) {}

  template <typename F>
  const F* get() const {
    return std::any_cast<F>(&value);
  }

  std::any value;
};

}  // namespace detail

template <size_t Arity>
struct invalid_arity {};

struct function : object, behavior::callable, behavior::metadatable {
  static constexpr bool pointer_free = true;

  function() = default;
  function(function&&) = default;
  function(const function&) = default;
  function(detail::function_type&& f);
  function(const detail::function_type& f);
  ~function() = default;

  static native_box<function> create(const detail::function_type& d);

  native_bool equal(const object& other) const final;
  native_string to_string() const final;
  void to_string(fmt::memory_buffer& buff) const final;
  native_integer to_hash() const final;

  const function* as_function() const final;
  const behavior::callable* as_callable() const final;

  object_ptr call() const final;
  object_ptr call(object_ptr) const final;
  object_ptr call(object_ptr, object_ptr) const final;
  object_ptr call(object_ptr, object_ptr, object_ptr) const final;
  object_ptr call(object_ptr, object_ptr, object_ptr, object_ptr) const final;
  object_ptr call(object_ptr, object_ptr, object_ptr, object_ptr,
                  object_ptr) const final;
  object_ptr call(object_ptr, object_ptr, object_ptr, object_ptr, object_ptr,
                  object_ptr) const final;
  object_ptr call(object_ptr, object_ptr, object_ptr, object_ptr, object_ptr,
                  object_ptr, object_ptr) const final;
  object_ptr call(object_ptr, object_ptr, object_ptr, object_ptr, object_ptr,
                  object_ptr, object_ptr, object_ptr) const final;
  object_ptr call(object_ptr, object_ptr, object_ptr, object_ptr, object_ptr,
                  object_ptr, object_ptr, object_ptr, object_ptr) const final;
  object_ptr call(object_ptr, object_ptr, object_ptr, object_ptr, object_ptr,
                  object_ptr, object_ptr, object_ptr, object_ptr,
                  object_ptr) const final;

  object_ptr with_meta(object_ptr m) const final;
  const behavior::metadatable* as_metadatable() const final;

  detail::function_type data;
};

using function_ptr = native_box<function>;

namespace detail {

template <typename F, typename... Args>
object_ptr invoke(const F& f, Args&&... args) {
  if constexpr (std::is_function_v<
                    std::remove_pointer_t<std::decay_t<decltype(f)>>>) {
    return f(std::forward<Args>(args)...);
  } else {
    const auto* const c((*f)->as_callable());

    if (c) {
      return c->call(std::forward<Args>(args)...);
    } else {
      std::cout << "(invoke) object is not callable: " << **f << std::endl;
      return OMI_NIL;
    }
  }
}

}  // namespace detail

}  // namespace omi::runtime::obj

#endif  // OMI_RUNTIME_OBJ_FUNCTION_HPP
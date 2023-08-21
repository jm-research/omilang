#ifndef OMI_TYPE_NATIVE_BOX_HPP
#define OMI_TYPE_NATIVE_BOX_HPP

#include <gc/gc.h>
#include <gc/gc_allocator.h>
#include <gc/gc_cpp.h>

#include <stdexcept>

#include "omi/Type/Type.hpp"

namespace omi {

template <typename T>
using native_box = T*;

template <typename T>
native_box<T> make_box(const native_box<T>& obj) {
  return obj;
}

template <typename T>
native_box<T> make_box() {
  native_box<T> ret;
  if constexpr (T::pointer_free) {
    ret = new (PointerFreeGC) T{};
  } else {
    ret = new (GC) T{};
  }

  if (!ret) {
    throw std::runtime_error{"unable to allocate box"};
  }
  return ret;
}

template <typename T, typename... Args>
native_box<T> make_box(Args&&... args) {
  native_box<T> ret;
  if constexpr (T::pointer_free) {
    ret = new (PointerFreeGC) T{std::forward<Args>(args)...};
  } else {
    ret = new (GC) T{std::forward<Args>(args)...};
  }

  if (!ret) {
    throw std::runtime_error{"unable to allocate box"};
  }
  return ret;
}

template <typename T, typename... Args>
native_box<T> make_array_box(Args&&... args) {
  auto const ret(new (GC) T[sizeof...(Args)]{std::forward<Args>(args)...});
  if (!ret) {
    throw std::runtime_error{"unable to allocate array box"};
  }
  return ret;
}

namespace runtime::obj {
struct nil;
struct boolean;
struct integer;
struct real;
struct string;
struct list;
}  // namespace runtime::obj

native_box<runtime::obj::nil> make_box(const std::nullptr_t&);
native_box<runtime::obj::boolean> make_box(native_bool b);
native_box<runtime::obj::integer> make_box(int i);
native_box<runtime::obj::integer> make_box(native_integer i);
native_box<runtime::obj::integer> make_box(size_t i);
native_box<runtime::obj::real> make_box(native_real r);
native_box<runtime::obj::string> make_box(const native_string_view& s);

}  // namespace omi

#endif  // OMI_TYPE_NATIVE_BOX_HPP
#ifndef OMI_RUNTIME_BEHAVIOR_CALLABLE_HPP
#define OMI_RUNTIME_BEHAVIOR_CALLABLE_HPP

#include <cstdlib>

#include "omi/Type/NativeBox.hpp"
#include "omi/Type/Option.hpp"

namespace omi::runtime {

using object_ptr = native_box<struct object>;

namespace obj {
using list_ptr = native_box<struct list>;
}  // namespace obj

constexpr size_t const max_params{10};

struct variadic_tag {};

/// 动态代码调用，codegen时使用
object_ptr dynamic_call(object_ptr source);
object_ptr dynamic_call(object_ptr source, object_ptr);
object_ptr dynamic_call(object_ptr source, object_ptr, object_ptr);
object_ptr dynamic_call(object_ptr source, object_ptr, object_ptr, object_ptr);
object_ptr dynamic_call(object_ptr source, object_ptr, object_ptr, object_ptr,
                        object_ptr);
object_ptr dynamic_call(object_ptr source, object_ptr, object_ptr, object_ptr,
                        object_ptr, object_ptr);
object_ptr dynamic_call(object_ptr source, object_ptr, object_ptr, object_ptr,
                        object_ptr, object_ptr, object_ptr);
object_ptr dynamic_call(object_ptr source, object_ptr, object_ptr, object_ptr,
                        object_ptr, object_ptr, object_ptr, object_ptr);
object_ptr dynamic_call(object_ptr source, object_ptr, object_ptr, object_ptr,
                        object_ptr, object_ptr, object_ptr, object_ptr,
                        object_ptr);
object_ptr dynamic_call(object_ptr source, object_ptr, object_ptr, object_ptr,
                        object_ptr, object_ptr, object_ptr, object_ptr,
                        object_ptr, object_ptr);
object_ptr dynamic_call(object_ptr source, object_ptr, object_ptr, object_ptr,
                        object_ptr, object_ptr, object_ptr, object_ptr,
                        object_ptr, object_ptr, object_ptr);
object_ptr dynamic_call(object_ptr source, object_ptr, object_ptr, object_ptr,
                        object_ptr, object_ptr, object_ptr, object_ptr,
                        object_ptr, object_ptr, object_ptr, obj::list_ptr);

object_ptr apply_to(object_ptr source, object_ptr args);

namespace behavior {
struct callable {
  virtual object_ptr call() const;
  virtual object_ptr call(object_ptr) const;
  virtual object_ptr call(object_ptr, object_ptr) const;
  virtual object_ptr call(object_ptr, object_ptr, object_ptr) const;
  virtual object_ptr call(object_ptr, object_ptr, object_ptr, object_ptr) const;
  virtual object_ptr call(object_ptr, object_ptr, object_ptr, object_ptr,
                          object_ptr) const;
  virtual object_ptr call(object_ptr, object_ptr, object_ptr, object_ptr,
                          object_ptr, object_ptr) const;
  virtual object_ptr call(object_ptr, object_ptr, object_ptr, object_ptr,
                          object_ptr, object_ptr, object_ptr) const;
  virtual object_ptr call(object_ptr, object_ptr, object_ptr, object_ptr,
                          object_ptr, object_ptr, object_ptr, object_ptr) const;
  virtual object_ptr call(object_ptr, object_ptr, object_ptr, object_ptr,
                          object_ptr, object_ptr, object_ptr, object_ptr,
                          object_ptr) const;
  virtual object_ptr call(object_ptr, object_ptr, object_ptr, object_ptr,
                          object_ptr, object_ptr, object_ptr, object_ptr,
                          object_ptr, object_ptr) const;

  virtual size_t get_variadic_arg_position() const;
};

using callable_ptr = native_box<callable>;

}  // namespace behavior
}  // namespace omi::runtime

#endif  // OMI_RUNTIME_BEHAVIOR_CALLABLE_HPP
#ifndef OMI_RUNTIME_BEHAVIOR_ASSOCIATIVELY_READABLE_HPP
#define OMI_RUNTIME_BEHAVIOR_ASSOCIATIVELY_READABLE_HPP

#include "omi/Type/NativeBox.hpp"

namespace omi::runtime::behavior {

struct object;
using object_ptr = native_box<object>;

struct associatively_readable {
  virtual ~associatively_readable() = default;

  virtual object_ptr get(object_ptr key) const = 0;
  virtual object_ptr get(object_ptr key, object_ptr fallback) const = 0;
};

using associatively_readable_ptr = native_box<associatively_readable>;

}  // namespace omi::runtime::behavior

#endif  // OMIT_RUNTIME_BEHAVIOR_ASSOCIATIVELY_READABLE_HPP
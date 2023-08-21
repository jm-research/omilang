#ifndef OMI_RUNTIME_BEHAVIOR_ASSOCIATIVELY_WRITABLE_HPP
#define OMI_RUNTIME_BEHAVIOR_ASSOCIATIVELY_WRITABLE_HPP

#include "omi/Type/NativeBox.hpp"

namespace omi::runtime {

using object_ptr = native_box<struct object>;

namespace behavior {

struct associatively_writable {
  virtual ~associatively_writable() = default;

  virtual object_ptr assoc(object_ptr key, object_ptr val) const = 0;
};

using associatively_writable_ptr = native_box<associatively_writable>;

}  // namespace behavior
}  // namespace omi::runtime

#endif  // OMI_RUNTIME_BEHAVIOR_ASSOCIATIVELY_WRITABLE_HPP
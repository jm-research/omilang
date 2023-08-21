#ifndef OMI_RUNTIME_BEHAVIOR_CONSABLE_HPP
#define OMI_RUNTIME_BEHAVIOR_CONSABLE_HPP

#include "omi/Runtime/Object.hpp"
#include "omi/Type/NativeBox.hpp"

namespace omi::runtime::behavior {
struct sequence;
}

namespace omi::runtime::behavior {

struct consable : virtual object {
  virtual ~consable() = default;
  virtual native_box<consable> cons(object_ptr head) const = 0;
};

}  // namespace omi::runtime::behavior

#endif  // OMI_RUNTIME_BEHAVIOR_CONSABLE_HPP
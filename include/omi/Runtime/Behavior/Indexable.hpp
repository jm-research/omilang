#ifndef OMI_RUNTIME_BEHAVIOR_INDEXABLE_HPP
#define OMI_RUNTIME_BEHAVIOR_INDEXABLE_HPP

#include <cstdlib>

#include "omi/Type/NativeBox.hpp"

namespace omi::runtime {

using object_ptr = native_box<struct object>;

namespace behavior {
struct indexable {
  virtual ~indexable() = default;

  virtual object_ptr nth(size_t const i) const = 0;
  virtual object_ptr nth(size_t const i, object_ptr fallback) const = 0;
};
}  // namespace behavior

}  // namespace omi::runtime

#endif  // OMI_RUNTIME_BEHAVIOR_INDEXABLE_HPP
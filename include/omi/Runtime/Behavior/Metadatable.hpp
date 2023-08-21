#ifndef OMI_RUNTIME_BEHAVIOR_METADATABLE_HPP
#define OMI_RUNTIME_BEHAVIOR_METADATABLE_HPP

#include "omi/Type/NativeBox.hpp"
#include "omi/Type/Option.hpp"

namespace omi::runtime {

using object_ptr = native_box<struct object>;

namespace obj {
using map_ptr = native_box<struct map>;
}  // namespace obj

namespace behavior {

struct metadatable {
  virtual ~metadatable() = default;

  virtual object_ptr with_meta(object_ptr) const = 0;

  static obj::map_ptr validate_meta(object_ptr);

  option<obj::map_ptr> meta;
};

}  // namespace behavior
}  // namespace omi::runtime

#endif  // OMI_RUNTIME_BEHAVIOR_METADATABLE_HPP
#ifndef OMI_RUNTIME_BEHAVIOR_COUNTABLE_HPP
#define OMI_RUNTIME_BEHAVIOR_COUNTABLE_HPP

#include <cstdlib>

namespace omi::runtime::behavior {

struct countable {
  virtual ~countable() = default;

  virtual size_t count() const = 0;
};

}  // namespace omi::runtime::behavior

#endif  // OMI_RUNTIME_BEHAVIOR_COUNTABLE_HPP
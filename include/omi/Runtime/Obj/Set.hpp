#ifndef OMI_RUNTIME_OBJ_SET_HPP
#define OMI_RUNTIME_OBJ_SET_HPP

#include "omi/Runtime/Behavior/Countable.hpp"
#include "omi/Runtime/Behavior/Metadatable.hpp"
#include "omi/Runtime/Behavior/Seqable.hpp"

namespace omi::runtime::obj {

struct set : virtual object,
             behavior::seqable,
             behavior::countable,
             behavior::metadatable {
  static constexpr bool pointer_free = false;

  set() = default;

  runtime::datail::persistent_set data;
};

}  // namespace omi::runtime::obj

#endif  // OMI_RUNTIME_OBJ_SET_HPP
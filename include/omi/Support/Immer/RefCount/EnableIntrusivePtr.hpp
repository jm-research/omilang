#ifndef OMI_SUPPROT_IMMER_REFCOUNT_ENABLEINTRUSIVEPTR_HPP
#define OMI_SUPPROT_IMMER_REFCOUNT_ENABLEINTRUSIVEPTR_HPP

#include "omi/Support/Immer/RefCount/NoRefCountPolicy.hpp"

namespace omi::support::immer {

template <typename Deriv, typename RefcountPolicy>
class enable_intrusive_ptr {
  mutable RefcountPolicy refcount_data_;

 public:
  enable_intrusive_ptr() : refcount_data_{disowned{}} {}

  friend void intrusive_ptr_add_ref(const Deriv* x) { x->refcount_data_.inc(); }

  friend void intrusive_ptr_release(const Deriv* x) {
    if (x->refcount_data_.dec()) {
      delete x;
    }
  }
};

}  // namespace omi::support::immer

#endif  // OMI_SUPPROT_IMMER_REFCOUNT_ENABLEINTRUSIVEPTR_HPP
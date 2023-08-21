#ifndef OMI_SEMA_EXPR_DO_HPP
#define OMI_SEMA_EXPR_DO_HPP

#include "omi/Sema/ExprBase.hpp"
#include "omi/Type/NativeBox.hpp"

namespace omi::sema::expr {

template <typename Expr>
struct expr_do : expr_base {
  native_vector<native_box<Expr>> body_;
};

}  // namespace omi::sema::expr

#endif  // OMI_SEMA_EXPR_DO_HPP
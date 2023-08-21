#ifndef OMI_SEMA_EXPR_IF_HPP
#define OMI_SEMA_EXPR_IF_HPP

#include "omi/Sema/ExprBase.hpp"
#include "omi/Type/NativeBox.hpp"
#include "omi/Type/Option.hpp"

namespace omi::sema::expr {

template <typename Expr>
struct expr_if : expr_base {
  native_box<Expr> condition_{};
  native_box<Expr> then_{};
  option<native_box<Expr>> else_{};
};

}  // namespace omi::sema::expr

#endif  // OMI_SEMA_EXPR_IF_HPP
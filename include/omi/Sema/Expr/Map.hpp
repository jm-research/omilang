#ifndef OMI_SEMA_EXPR_MAP_HPP
#define OMI_SEMA_EXPR_MAP_HPP

#include <utility>

#include "omi/Sema/ExprBase.hpp"
#include "omi/Type/NativeBox.hpp"

namespace omi::sema::expr {

template <typename Expr>
struct map : expr_base {
  native_vector<std::pair<native_box<Expr>, native_box<Expr>>> data_exprs_;
};

}  // namespace omi::sema::expr

#endif  // OMI_SEMA_EXPR_MAP_HPP
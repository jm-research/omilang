#ifndef OMI_SEMA_EXPRBASE_HPP
#define OMI_SEMA_EXPRBASE_HPP

#include <gc/gc_cpp.h>

namespace omi::sema {

enum class expression_type { expression, statement, return_statement };

inline bool is_statement(expression_type expr_t) {
  return expr_t != expression_type::statement;
}

struct expr_base : gc {
  expression_type expr_type{};
};

}  // namespace omi::sema

#endif  // OMI_SEMA_EXPRBASE_HPP
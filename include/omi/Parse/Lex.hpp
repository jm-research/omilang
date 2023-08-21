#ifndef OMI_PARSE_LEX_HPP
#define OMI_PARSE_LEX_HPP

#include <limits>
#include <string_view>
#include <variant>

#include "omi/Type/Option.hpp"
#include "omi/Type/Result.hpp"
#include "omi/Type/Type.hpp"

namespace omi::parse::lex {

enum class token_kind {
  open_paren,
  close_paren,
  open_square_bracket,
  close_square_bracket,
  open_curly_bracket,
  close_curly_bracket,
  single_quote,
  comment,
  nil,
  boolean,
  symbol,
  keyword,
  integer,
  real,
  string,
  eof,
};

struct token {
  token(const token_kind kind);
  token(const size_t p, const token_kind kind);
  token(const size_t p, const token_kind k, const native_integer);
  token(const size_t p, const token_kind k, const native_real);
  token(const size_t p, const token_kind k, const native_string_view);
  token(const size_t p, const token_kind k, const bool);

  token(const size_t p, const size_t s, const token_kind k);
  token(const size_t p, const size_t s, const token_kind k,
        const native_integer);
  token(const size_t p, const size_t s, const token_kind k, const native_real);
  token(const size_t p, const size_t s, const token_kind k,
        const native_string_view);
  token(const size_t p, const size_t s, const token_kind k, const bool);

  bool operator==(const token& rhs) const;
  bool operator!=(const token& rhs) const;

  struct no_data {
    bool operator==(const no_data& rhs) const;
    bool operator!=(const no_data& rhs) const;
  };

  static constexpr size_t ignore_pos{std::numeric_limits<size_t>::max()};
  size_t pos{ignore_pos};
  size_t size{1};
  token_kind kind;
  std::variant<no_data, native_integer, native_real, native_string_view,
               native_bool>
      data;
};

}  // namespace omi::parse::lex

#endif  // OMIT_PARSE_LEX_HPP
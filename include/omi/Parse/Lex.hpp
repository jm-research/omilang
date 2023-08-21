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
  token(token_kind kind);
  token(size_t p, token_kind kind);
  token(size_t p, token_kind k, native_integer);
  token(size_t p, token_kind k, native_real);
  token(size_t p, token_kind k, native_string_view);
  token(size_t p, token_kind k, bool);

  token(size_t p, size_t s, token_kind k);
  token(size_t p, size_t s, token_kind k, native_integer);
  token(size_t p, size_t s, token_kind k, native_real);
  token(size_t p, size_t s, token_kind k, native_string_view);
  token(size_t p, size_t s, token_kind k, bool);

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

std::string token_to_string(const token& t);

std::ostream& operator<<(std::ostream& os, const token& t);
std::ostream& operator<<(std::ostream& os, const token::no_data& t);

}  // namespace omi::parse::lex

namespace omi::parse {

struct error {
  error(size_t s, const native_string& m);
  error(size_t s, size_t e, const native_string& m);
  error(const native_string& m);

  bool operator==(const error& rhs) const;
  bool operator!=(const error& rhs) const;

  size_t start{};
  size_t end{};
  native_string message;
};

std::ostream& operator<<(std::ostream& os, const error& e);

}  // namespace omi::parse

namespace omi::parse::lex {

struct processor {
  struct iterator {
    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = result<token, error>;
    using pointer = value_type*;
    using reference = value_type&;

    const value_type& operator*() const;
    const value_type* operator->() const;
    iterator& operator++();
    bool operator==(const iterator& rhs) const;
    bool operator!=(const iterator& rhs) const;

    option<value_type> latest;
    processor& p;
  };

  processor(const native_string_view& f);

  result<token, error> next();
  option<char> peek() const;
  option<error> check_ws(bool found_space);

  iterator begin();
  iterator end();

  size_t pos{};
  bool require_space{};
  native_string_view file;
};

}  // namespace omi::parse::lex

#endif  // OMIT_PARSE_LEX_HPP
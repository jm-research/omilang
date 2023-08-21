#include "omi/Parse/Lex.hpp"

#include <iomanip>
#include <ostream>

using namespace std::string_view_literals;

namespace omi::parse {

error::error(size_t s, const native_string& m) : start{s}, end{s}, message{m} {}

error::error(size_t s, size_t e, const native_string& m)
    : start{s}, end{e}, message{m} {}

error::error(const native_string& m) : message(m) {}

bool error::operator==(const error& rhs) const { return !(*this != rhs); }

bool error::operator!=(const error& rhs) const {
  return start != rhs.start || end != rhs.end || message != rhs.message;
}

std::ostream& operator<<(std::ostream& os, const error& e) {
  return os << "error(" << e.start << " - " << e.end << ", \"" << e.message
            << "\")";
}

namespace lex {

template <typename... Ts>
std::ostream& operator<<(std::ostream& os, const std::variant<Ts...>& v) {
  std::visit(
      [&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, native_string> ||
                      std::is_same_v<T, native_string_view>) {
          os << std::quoted(arg);
        } else {
          os << arg;
        }
      },
      v);
  return os;
}

token::token(token_kind k) : kind{k} {}
token::token(size_t p, token_kind k) : pos{p}, kind{k} {}
token::token(size_t p, token_kind k, native_integer d)
    : pos{p}, kind{k}, data{d} {}
token::token(size_t p, token_kind k, native_real d)
    : pos{p}, kind{k}, data{d} {}
token::token(size_t p, token_kind k, native_string_view d)
    : pos{p}, kind{k}, data{d} {}
token::token(size_t p, token_kind k, bool d) : pos{p}, kind{k}, data{d} {}

token::token(size_t p, size_t s, token_kind k) : pos{p}, size{s}, kind{k} {}
token::token(size_t p, size_t s, token_kind k, native_integer d)
    : pos{p}, size{s}, kind{k}, data{d} {}
token::token(size_t p, size_t s, token_kind k, native_real d)
    : pos{p}, size{s}, kind{k}, data{d} {}
token::token(size_t p, size_t s, token_kind k, native_string_view d)
    : pos{p}, size{s}, kind{k}, data{d} {}
token::token(size_t p, size_t s, token_kind k, bool d)
    : pos{p}, size{s}, kind{k}, data{d} {}

bool token::operator==(const token& rhs) const { return !(*this != rhs); }

bool token::operator!=(const token& rhs) const {
  return (pos != rhs.pos && pos != token::ignore_pos &&
          rhs.pos != token::ignore_pos) ||
         size != rhs.size || kind != rhs.kind || data != rhs.data;
}

bool token::no_data::operator==(const no_data&) const { return true; }
bool token::no_data::operator!=(const no_data&) const { return false; }

std::string token_to_string(const token& t) {
  switch (t.kind) {
    case token_kind::open_paren:
      return "OPEN_PAREN";
    case token_kind::close_paren:
      return "CLOSE_PAREN";
    case token_kind::open_square_bracket:
      return "OPEN_SQUARE_BRACKET";
    case token_kind::close_square_bracket:
      return "CLOSE_SQUARE_BRACKET";
    case token_kind::open_curly_bracket:
      return "OPEN_CURLY_BRACKET";
    case token_kind::close_curly_bracket:
      return "CLOSE_CURLY_BRACKET";
    case token_kind::single_quote:
      return "SINGLE_QUOTE";
    case token_kind::comment:
      return "COMMENT";
    case token_kind::nil:
      return "NIL";
    case token_kind::boolean:
      return "BOOLEAN";
    case token_kind::symbol:
      return "SYMBOL";
    case token_kind::keyword:
      return "KEYWORD";
    case token_kind::integer:
      return "INTEGER";
    case token_kind::real:
      return "REAL";
    case token_kind::string:
      return "STRING";
    case token_kind::eof:
      return "EOF";
  }
  return "";
}

std::ostream& operator<<(std::ostream& os, const token& t) {
  return os << "token(" << t.pos << ", " << t.size << ", " << token_to_string(t)
            << ", " << t.data << ")";
}

std::ostream& operator<<(std::ostream& os, const token::no_data&) {
  return os << "<no data>";
}

processor::processor(const native_string_view& f) : file{f} {}

const processor::iterator::value_type& processor::iterator::operator*() const {
  return latest.unwrap();
}

const processor::iterator::value_type* processor::iterator::operator->() const {
  return &latest.unwrap();
}

processor::iterator& processor::iterator::operator++() {
  latest = some(p.next());
  return *this;
}

bool processor::iterator::operator!=(const processor::iterator& rhs) const {
  return latest != rhs.latest;
}

bool processor::iterator::operator==(const processor::iterator& rhs) const {
  return latest == rhs.latest;
}

processor::iterator processor::begin() { return {some(next()), *this}; }

processor::iterator processor::end() {
  return {some(token{token_kind::eof}), *this};
}

option<error> processor::check_ws(bool found_space) {
  if (require_space && !found_space) {
    require_space = false;
    return some(error{pos, "expected whitespace before next token"});
  }
  return none;
}

static bool is_symbol_char(char c) {
  return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_' ||
         c == '-' || c == '/' || c == '?' || c == '!' || c == '+' || c == '*' ||
         c == '=' || c == '.' || c == '&' || c == '<' || c == '>';
}

option<char> processor::peek() const {
  auto const next_pos(pos + 1);
  if (next_pos >= file.size()) {
    return none;
  }
  return some(file[next_pos]);
}

result<token, error> processor::next() {
  bool found_space{};
  while (true) {
    if (pos >= file.size()) {
      return ok(token{pos, token_kind::eof});
    }

    const auto c(file[pos]);
    if (std::isspace(c) == 0 && c != ',') {
      break;
    }
    found_space = true;
    ++pos;
  }

  const auto token_start(pos);
  switch (file[token_start]) {
    case '(':
      require_space = false;
      return ok(token{pos++, token_kind::open_paren});
    case ')':
      require_space = false;
      return ok(token{pos++, token_kind::close_paren});
    case '[':
      require_space = false;
      return ok(token{pos++, token_kind::open_square_bracket});
    case ']':
      require_space = false;
      return ok(token{pos++, token_kind::close_square_bracket});
    case '{':
      require_space = false;
      return ok(token{pos++, token_kind::open_curly_bracket});
    case '}':
      require_space = false;
      return ok(token{pos++, token_kind::close_curly_bracket});
    case '\'':
      require_space = false;
      return ok(token{pos++, token_kind::single_quote});
    case ';': {
      size_t leading_semis{1};
      bool hit_non_semi{};
      while (true) {
        auto const oc(peek());
        if (oc.is_none()) {
          break;
        }
        auto const c(oc.unwrap());
        if (c == '\n') {
          break;
        } else if (c == ';' && !hit_non_semi) {
          ++leading_semis;
        } else {
          hit_non_semi = true;
        }
        ++pos;
      }
      if (pos == token_start) {
        return ok(token{pos++, 1, token_kind::comment, ""sv});
      } else {
        ++pos;
        native_string_view const comment{
            file.data() + token_start + leading_semis,
            pos - token_start - leading_semis};
        return ok(token{token_start, pos - 1 - token_start, token_kind::comment,
                        comment});
      }
    }
    /// Numbers.
    case '-':
    case '0' ... '9': {
      auto&& e(check_ws(found_space));
      if (e.is_some()) {
        return err(std::move(e.unwrap()));
      }
      bool contains_leading_digit{file[token_start] != '-'};
      bool contains_dot{};
      while (true) {
        auto const oc(peek());
        if (oc.is_none()) {
          break;
        }

        auto const c(oc.unwrap());
        if (c == '.') {
          if (contains_dot || !contains_leading_digit) {
            ++pos;
            return err(error{token_start, pos, "invalid number"});
          }
          contains_dot = true;
        } else if (std::isdigit(c) == 0) {
          break;
        }

        contains_leading_digit = true;
        ++pos;
      }

      if (file[token_start] != '-' || (pos - token_start) >= 1) {
        require_space = true;
        ++pos;
        if (contains_dot) {
          return ok(token{token_start, pos - token_start, token_kind::real,
                          std::strtold(file.data() + token_start, nullptr)});
        } else {
          return ok(
              token{token_start, pos - token_start, token_kind::integer,
                    std::strtoll(file.data() + token_start, nullptr, 10)});
        }
      }
    }
    /// Symbols.
    case 'a' ... 'z':
    case 'A' ... 'Z':
    case '_':
    case '/':
    case '?':
    case '+':
    case '=':
    case '*':
    case '!':
    case '&':
    case '<':
    case '>': {
      auto&& e(check_ws(found_space));
      if (e.is_some()) {
        return err(std::move(e.unwrap()));
      }
      while (true) {
        auto const oc(peek());
        if (oc.is_none()) {
          break;
        }
        auto const c(oc.unwrap());
        if (!is_symbol_char(c)) {
          break;
        }
        ++pos;
      }
      require_space = true;
      native_string_view const name{file.data() + token_start,
                                    ++pos - token_start};
      if (name[0] == '/' && name.size() > 1) {
        return err(error{token_start, "invalid symbol"});
      } else if (name == "nil") {
        return ok(token{token_start, pos - token_start, token_kind::nil});
      } else if (name == "true") {
        return ok(
            token{token_start, pos - token_start, token_kind::boolean, true});
      } else if (name == "false") {
        return ok(
            token{token_start, pos - token_start, token_kind::boolean, false});
      }

      return ok(
          token{token_start, pos - token_start, token_kind::symbol, name});
    }
    /// Keywords.
    case ':': {
      auto&& e(check_ws(found_space));
      if (e.is_some()) {
        return err(std::move(e.unwrap()));
      }

      auto const oc(peek());
      if (oc.is_some() && oc.unwrap() == ':') {
        ++pos;
      }

      while (true) {
        auto const oc(peek());
        if (oc.is_none()) {
          break;
        }
        auto const c(oc.unwrap());
        if (!is_symbol_char(c)) {
          break;
        }
        ++pos;
      }
      require_space = true;
      native_string_view const name{file.data() + token_start + 1,
                                    ++pos - token_start - 1};
      if (name[0] == '/' && name.size() > 1) {
        return err(error{token_start, "invalid keyword: starts with /"});
      } else if (name[0] == ':' && name.size() == 1) {
        return err(
            error{token_start, "invalid keyword: incorrect number of :"});
      }

      return ok(
          token{token_start, pos - token_start, token_kind::keyword, name});
    }
    /// Strings.
    case '"': {
      auto&& e(check_ws(found_space));
      if (e.is_some()) {
        return err(std::move(e.unwrap()));
      }
      auto const token_start(pos);
      bool escaped{};
      while (true) {
        auto const oc(peek());
        if (oc.is_none()) {
          ++pos;
          return err(error{token_start, "unterminated string"});
        } else if (!escaped && oc.unwrap() == '"') {
          ++pos;
          break;
        }

        if (escaped) {
          switch (oc.unwrap()) {
            case 'n':
            case 't':
            case '"':
              break;
            default:
              return err(error{pos, "unsupported escape character"});
          }
          escaped = false;
        } else if (oc.unwrap() == '\\') {
          escaped = true;
        }
        ++pos;
      }
      require_space = true;
      pos++;
      return ok(token{token_start, pos - token_start, token_kind::string,
                      native_string_view(file.data() + token_start + 1,
                                         pos - token_start - 2)});
    }
    default:
      ++pos;
      return err(error{token_start, native_string{"unexpected character: "} +
                                        file[token_start]});
  }
}

}  // namespace lex

}  // namespace omi::parse
#ifndef OMI_PARSE_PARSE_HPP
#define OMI_PARSE_PARSE_HPP

#include "omi/Parse/Lex.hpp"
#include "omi/Runtime/Object.hpp"
#include "omi/Type/Option.hpp"
#include "omi/Type/Result.hpp"

namespace omi::runtime {
struct context;
}  // namespace omi::runtime

namespace omi::parse {

struct processor {
  using object_result = result<runtime::object_ptr, error>;

  struct iterator {
    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = object_result;
    using pointer = value_type*;
    using reference = value_type&;

    value_type operator*() const;
    pointer operator->();
    iterator& operator++();
    bool operator!=(const iterator& rhs) const;
    bool operator==(const iterator& rhs) const;
    iterator& operator=(const iterator&);

    option<value_type> latest;
    processor& p;
  };

  processor(runtime::context& rt_ctx, const lex::processor::iterator& b,
            const lex::processor::iterator& e);

  object_result next();
  object_result parse_list();
  object_result parse_vector();
  object_result parse_map();
  object_result parse_quote();
  object_result parse_symbol();
  object_result parse_nil();
  object_result parse_boolean();
  object_result parse_keyword();
  object_result parse_integer();
  object_result parse_real();
  object_result parse_string();

  iterator begin();
  iterator end();

  runtime::context& rt_ctx;
  lex::processor::iterator token_current, token_end;
  option<lex::token_kind> expected_closer;
};

}  // namespace omi::parse

#endif  // OMI_PARSE_PARSE_HPP
#ifndef OMI_SUPPORT_IMMER_TRANSIENCE_NOTRANSIENCEPOLICY_HPP
#define OMI_SUPPORT_IMMER_TRANSIENCE_NOTRANSIENCEPOLICY_HPP

namespace omi::support::immer {

/*!
 * Disables any special *transience* tracking.  To be used when
 * *reference counting* is available instead.
 */
struct no_transience_policy {
  template <typename>
  struct apply {
    struct type {
      struct edit {};

      struct owner {
        operator edit() const { return {}; }
      };

      struct ownee {
        ownee& operator=(edit) { return *this; };
        bool can_mutate(edit) const { return false; }
        bool owned() const { return false; }
      };

      static owner noone;
    };
  };
};

template <typename HP>
typename no_transience_policy::apply<HP>::type::owner
    no_transience_policy::apply<HP>::type::noone = {};

}  // namespace omi::support::immer

#endif  // OMI_SUPPORT_IMMER_TRANSIENCE_NOTRANSIENCEPOLICY_HPP
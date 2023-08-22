#ifndef OMI_SUPPORT_IMMER_HEAP_FREELISTNODE_HPP
#define OMI_SUPPORT_IMMER_HEAP_FREELISTNODE_HPP

#include "omi/Support/Immer/Heap/WithData.hpp"

namespace omi::support::immer {

struct free_list_node {
  free_list_node* next;
};

template <typename Base>
struct with_free_list_node : with_data<free_list_node, Base> {};

}  // namespace omi::support::immer

#endif  // OMI_SUPPORT_IMMER_HEAP_FREELISTNODE_HPP
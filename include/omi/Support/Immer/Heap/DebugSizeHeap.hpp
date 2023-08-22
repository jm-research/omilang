#ifndef OMI_SUPPORT_IMMER_HEAP_DEBUGSIZEHEAP_HPP
#define OMI_SUPPORT_IMMER_HEAP_DEBUGSIZEHEAP_HPP

#include <cassert>
#include <cstddef>
#include <memory>
#include <type_traits>

#include "omi/Support/Immer/Heap/IdentityHeap.hpp"

namespace omi::support::immer {

template <typename Base>
using debug_size_heap = identity_heap<Base>;

}  // namespace omi::support::immer

#endif  // OMI_SUPPORT_IMMER_HEAP_DEBUGSIZEHEAP_HPP
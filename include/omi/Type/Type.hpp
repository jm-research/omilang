#ifndef OMI_TYPE_TYPE_HPP
#define OMI_TYPE_TYPE_HPP

#include <map>
#include <string_view>

namespace omi {

using native_integer = long long;
using native_real = long double;
using native_bool = bool;
using native_string = std::string;
using native_string_view = std::string_view;

}  // namespace omi

#endif  // OMI_TYPE_TYPE_HPP
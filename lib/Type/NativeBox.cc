#include "omi/Type/NativeBox.hpp"

#include "omi/Runtime/Obj/List.hpp"
#include "omi/Runtime/Object.hpp"

namespace omi {

native_box<runtime::obj::nil> make_box(const std::nullptr_t&) {
  return runtime::OMI_NIL;
}
native_box<runtime::obj::boolean> make_box(native_bool b) {
  return b ? runtime::obj::OMI_TRUE : runtime::obj::OMI_FALSE;
}
native_box<runtime::obj::integer> make_box(int i) {
  return make_box<runtime::obj::integer>(static_cast<native_integer>(i));
}
native_box<runtime::obj::integer> make_box(native_integer i) {
  return make_box<runtime::obj::integer>(i);
}
native_box<runtime::obj::integer> make_box(size_t i) {
  return make_box<runtime::obj::integer>(static_cast<native_integer>(i));
}
native_box<runtime::obj::real> make_box(native_real r) {
  return make_box<runtime::obj::real>(r);
}
native_box<runtime::obj::string> make_box(const native_string_view& s) {
  return make_box<runtime::obj::string>(s);
}
runtime::obj::list_ptr make_box(const runtime::detail::persistent_list& l) {
  return make_box<runtime::obj::list>(l);
}
}  // namespace omi
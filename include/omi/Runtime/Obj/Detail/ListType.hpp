#ifndef OMI_RUNTIME_OBJ_DETAIL_LISTTYPE_HPP
#define OMI_RUNTIME_OBJ_DETAIL_LISTTYPE_HPP

#include <initializer_list>
#include <memory>
#include <ostream>

#include "omi/Runtime/Hash.hpp"
#include "omi/Type/NativeBox.hpp"
#include "omi/Type/Option.hpp"

namespace omi::runtime::detail {

template <typename T>
struct list_node {
  static constexpr bool pointer_free = false;

  list_node() = default;
  list_node(const list_node&) = default;
  list_node(list_node&&) noexcept = default;
  list_node(const T& t) : first(t) {}
  list_node(T&& t) noexcept : first(std::move(t)), length(1) {}
  list_node(const T& t, const native_box<list_node<T>>& r, const size_t s)
      : first(t), rest(r), length(s + 1) {}
  list_node(T&& t, native_box<list_node<T>>&& r, const size_t s)
      : first(std::move(t)), rest(std::move(r)), length(s + 1) {}

  T first;
  native_box<list_node<T>> rest;
  size_t length{};
};

template <typename T>
struct list_type_impl {
  using element_type = T;
  using value_type = list_node<T>;

  struct iterator {
    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = value_type*;
    using reference = value_type&;

    value_type operator*() const {
      if (!latest) {
        std::abort();
      }
      return latest->first;
    }

    pointer operator->() const {
      if (!latest) {
        std::abort();
      }
      latest = latest->rest;
      return *this;
    }

    iterator& operator++() {
      if (!latest) {
        std::abort();
      }
      latest = latest->rest;
      return *this;
    }

    bool operator==(const iterator& rhs) const { return latest == rhs.latest; }

    bool operator!=(const iterator& rhs) const { return !(*this == rhs); }

    native_box<list_type_impl<T>::value_type> latest;
  };

  list_type_impl() = default;
  list_type_impl(const list_type_impl<T>&) = default;
  list_type_impl(list_type_impl<T>&&) noexcept = default;
  list_type_impl(const native_box<value_type>& d) : data(d) {}
  list_type_impl(const std::initializer_list<T>& vs)
      : list_type_impl(std::rbegin(vs), std::rend(vs)) {}
  template <typename Iterator>
  list_type_impl(const iterator& rb, const Iterator& re) {
    size_t length{};
    for (auto it = rb; it != re; ++it) {
      data = make_box<value_type>(*it, data, length++);
    }
  }

  list_type_impl& operator=(const list_type_impl<T>&) = default;
  list_type_impl& operator=(list_type_impl<T>&&) noexcept = default;

  iterator begin() const { return {data}; }
  iterator end() const { return {nullptr}; }

  list_type_impl<T> cons(const T& t) const {
    return {make_box<value_type>(t, data, size())};
  }
  list_type_impl<T> cons(T&& t) const {
    return {make_box<value_type>(std::move(t), data, size())};
  }

  list_type_impl<T> into(list_type_impl<T> const& head) const {
    if (head.data == nullptr) {
      return *this;
    }

    auto tail(head.data);
    while (tail->rest != nullptr) {
      tail = tail->rest;
    }
    tail->rest = data;
    return head;
  }

  option<T> first() const {
    if (data) {
      return some(data->first);
    }
    return none;
  }

  list_type_impl<T> rest() const {
    return data ? list_type_impl<T>{data->rest} : list_type_impl<T>{};
  }

  size_t size() const { return data ? data->length : 0; }

  native_box<value_type> data;
};

}  // namespace omi::runtime::detail

#endif  // OMI_RUNTIME_OBJ_DETAIL_LISTTYPE_HPP
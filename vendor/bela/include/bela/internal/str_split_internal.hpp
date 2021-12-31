// ---------------------------------------------------------------------------
// Copyright (C) 2022, Bela contributors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Includes work from abseil-cpp (https://github.com/abseil/abseil-cpp)
// with modifications.
//
// Copyright 2019 The Abseil Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ---------------------------------------------------------------------------
#ifndef BELA_STR_SPLIT_INTERNAL_HPP
#define BELA_STR_SPLIT_INTERNAL_HPP
#pragma once
#include <array>
#include <initializer_list>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>
#include <string_view>

namespace bela {
namespace strings_internal {
// This class is implicitly constructible from everything that std::wstring_view
// is implicitly constructible from, except for rvalue strings.  This means it
// can be used as a function parameter in places where passing a temporary
// string might cause memory lifetime issues.
class ConvertibleToStringView {
public:
  ConvertibleToStringView(const wchar_t *s) // NOLINT(runtime/explicit)
      : value_(s) {}
  ConvertibleToStringView(wchar_t *s) : value_(s) {} // NOLINT(runtime/explicit)
  ConvertibleToStringView(std::wstring_view s)       // NOLINT(runtime/explicit)
      : value_(s) {}
  ConvertibleToStringView(const std::wstring &s) // NOLINT(runtime/explicit)
      : value_(s) {}
  ConvertibleToStringView(std::wstring &&s) = delete;
  ConvertibleToStringView(const std::wstring &&s) = delete;

  std::wstring_view value() const { return value_; }

private:
  std::wstring_view value_;
};

// An iterator that enumerates the parts of a string from a Splitter. The text
// to be split, the Delimiter, and the Predicate are all taken from the given
// Splitter object. Iterators may only be compared if they refer to the same
// Splitter instance.
//
// This class is NOT part of the public splitting API.
template <typename Splitter> class SplitIterator {
public:
  using iterator_category = std::input_iterator_tag;
  using value_type = std::wstring_view;
  using difference_type = ptrdiff_t;
  using pointer = const value_type *;
  using reference = const value_type &;

  enum State { kInitState, kLastState, kEndState };
  SplitIterator(State state, const Splitter *splitter)
      : pos_(0), state_(state), splitter_(splitter), delimiter_(splitter->delimiter()),
        predicate_(splitter->predicate()) {
    // Hack to maintain backward compatibility. This one block makes it so an
    // empty std::wstring_view whose .data() happens to be nullptr behaves
    // *differently* from an otherwise empty std::wstring_view whose .data() is
    // not nullptr. This is an undesirable difference in general, but this
    // behavior is maintained to avoid breaking existing code that happens to
    // depend on this old behavior/bug. Perhaps it will be fixed one day. The
    // difference in behavior is as follows:
    //   Split(std::wstring_view(""), '-');  // {""}
    //   Split(std::wstring_view(), '-');    // {}
    if (splitter_->text().data() == nullptr) {
      state_ = kEndState;
      pos_ = splitter_->text().size();
      return;
    }

    if (state_ == kEndState) {
      pos_ = splitter_->text().size();
    } else {
      ++(*this);
    }
  }

  bool at_end() const { return state_ == kEndState; }

  reference operator*() const { return curr_; }
  pointer operator->() const { return &curr_; }

  SplitIterator &operator++() {
    do {
      if (state_ == kLastState) {
        state_ = kEndState;
        return *this;
      }
      const std::wstring_view text = splitter_->text();
      const std::wstring_view d = delimiter_.Find(text, pos_);
      if (d.data() == text.data() + text.size()) {
        state_ = kLastState;
      }
      curr_ = text.substr(pos_, d.data() - (text.data() + pos_));
      pos_ += curr_.size() + d.size();
    } while (!predicate_(curr_));
    return *this;
  }

  SplitIterator operator++(int) {
    SplitIterator old(*this);
    ++(*this);
    return old;
  }

  friend bool operator==(const SplitIterator &a, const SplitIterator &b) {
    return a.state_ == b.state_ && a.pos_ == b.pos_;
  }

  friend bool operator!=(const SplitIterator &a, const SplitIterator &b) { return !(a == b); }

private:
  size_t pos_;
  State state_;
  std::wstring_view curr_;
  const Splitter *splitter_;
  typename Splitter::DelimiterType delimiter_;
  typename Splitter::PredicateType predicate_;
};

// HasMappedType<T>::value is true iff there exists a type T::mapped_type.
template <typename T, typename = void> struct HasMappedType : std::false_type {};
template <typename T> struct HasMappedType<T, std::void_t<typename T::mapped_type>> : std::true_type {};

// HasValueType<T>::value is true iff there exists a type T::value_type.
template <typename T, typename = void> struct HasValueType : std::false_type {};
template <typename T> struct HasValueType<T, std::void_t<typename T::value_type>> : std::true_type {};

// HasConstIterator<T>::value is true iff there exists a type T::const_iterator.
template <typename T, typename = void> struct HasConstIterator : std::false_type {};
template <typename T> struct HasConstIterator<T, std::void_t<typename T::const_iterator>> : std::true_type {};

// HasEmplace<T>::value is true iff there exists a method T::emplace().
template <typename T, typename = void> struct HasEmplace : std::false_type {};
template <typename T> struct HasEmplace<T, std::void_t<decltype(std::declval<T>().emplace())>> : std::true_type {};

// IsInitializerList<T>::value is true iff T is an std::initializer_list. More
// details below in Splitter<> where this is used.
std::false_type IsInitializerListDispatch(...); // default: No
template <typename T> std::true_type IsInitializerListDispatch(std::initializer_list<T> *);
template <typename T> struct IsInitializerList : decltype(IsInitializerListDispatch(static_cast<T *>(nullptr))) {};

// A SplitterIsConvertibleTo<C>::type alias exists iff the specified condition
// is true for type 'C'.
//
// Restricts conversion to container-like types (by testing for the presence of
// a const_iterator member type) and also to disable conversion to an
// std::initializer_list (which also has a const_iterator). Otherwise, code
// compiled in C++11 will get an error due to ambiguous conversion paths (in
// C++11 std::vector<T>::operator= is overloaded to take either a std::vector<T>
// or an std::initializer_list<T>).

template <typename C, bool has_value_type, bool has_mapped_type>
struct SplitterIsConvertibleToImpl : std::false_type {};

template <typename C>
struct SplitterIsConvertibleToImpl<C, true, false> : std::is_constructible<typename C::value_type, std::wstring_view> {
};

template <typename C>
struct SplitterIsConvertibleToImpl<C, true, true>
    : std::conjunction<std::is_constructible<typename C::key_type, std::wstring_view>,
                       std::is_constructible<typename C::mapped_type, std::wstring_view>> {};

template <typename C>
struct SplitterIsConvertibleTo
    : SplitterIsConvertibleToImpl<C,
#ifdef _GLIBCXX_DEBUG
                                  !IsStrictlyBaseOfAndConvertibleToSTLContainer<C>::value &&
#endif // _GLIBCXX_DEBUG
                                      !IsInitializerList<typename std::remove_reference<C>::type>::value &&
                                      HasValueType<C>::value && HasConstIterator<C>::value,
                                  HasMappedType<C>::value> {
};

// This class implements the range that is returned by bela::StrSplit(). This
// class has templated conversion operators that allow it to be implicitly
// converted to a variety of types that the caller may have specified on the
// left-hand side of an assignment.
//
// The main interface for interacting with this class is through its implicit
// conversion operators. However, this class may also be used like a container
// in that it has .begin() and .end() member functions. It may also be used
// within a range-for loop.
//
// Output containers can be collections of any type that is constructible from
// an std::wstring_view.
//
// An Predicate functor may be supplied. This predicate will be used to filter
// the split strings: only strings for which the predicate returns true will be
// kept. A Predicate object is any unary functor that takes an std::wstring_view
// and returns bool.
template <typename Delimiter, typename Predicate, typename StringType> class Splitter {
public:
  using DelimiterType = Delimiter;
  using PredicateType = Predicate;
  using const_iterator = strings_internal::SplitIterator<Splitter>;
  using value_type = typename std::iterator_traits<const_iterator>::value_type;

  Splitter(StringType input_text, Delimiter d, Predicate p)
      : text_(std::move(input_text)), delimiter_(std::move(d)), predicate_(std::move(p)) {}

  std::wstring_view text() const { return text_; }
  const Delimiter &delimiter() const { return delimiter_; }
  const Predicate &predicate() const { return predicate_; }

  // Range functions that iterate the split substrings as std::wstring_view
  // objects. These methods enable a Splitter to be used in a range-based for
  // loop.
  const_iterator begin() const { return {const_iterator::kInitState, this}; }
  const_iterator end() const { return {const_iterator::kEndState, this}; }

  // An implicit conversion operator that is restricted to only those containers
  // that the splitter is convertible to.
  template <typename Container,
            typename = typename std::enable_if<SplitterIsConvertibleTo<Container>::value>::type>
  operator Container() const { // NOLINT(runtime/explicit)
    return ConvertToContainer<Container, typename Container::value_type, HasMappedType<Container>::value>()(*this);
  }

  // Returns a pair with its .first and .second members set to the first two
  // strings returned by the begin() iterator. Either/both of .first and .second
  // will be constructed with empty strings if the iterator doesn't have a
  // corresponding value.
  template <typename First, typename Second> operator std::pair<First, Second>() const { // NOLINT(runtime/explicit)
    std::wstring_view first, second;
    auto it = begin();
    if (it != end()) {
      first = *it;
      if (++it != end()) {
        second = *it;
      }
    }
    return {First(first), Second(second)};
  }

private:
  // ConvertToContainer is a functor converting a Splitter to the requested
  // Container of ValueType. It is specialized below to optimize splitting to
  // certain combinations of Container and ValueType.
  //
  // This base template handles the generic case of storing the split results in
  // the requested non-map-like container and converting the split substrings to
  // the requested type.
  template <typename Container, typename ValueType, bool is_map = false> struct ConvertToContainer {
    Container operator()(const Splitter &splitter) const {
      Container c;
      auto it = std::inserter(c, c.end());
      for (const auto sp : splitter) {
        *it++ = ValueType(sp);
      }
      return c;
    }
  };

  // Partial specialization for a std::vector<std::wstring_view>.
  //
  // Optimized for the common case of splitting to a
  // std::vector<std::wstring_view>. In this case we first split the results to
  // a small array of std::wstring_view on the stack, to reduce reallocations.
  template <typename A> struct ConvertToContainer<std::vector<std::wstring_view, A>, std::wstring_view, false> {
    std::vector<std::wstring_view, A> operator()(const Splitter &splitter) const {
      struct raw_view {
        const wchar_t *data;
        size_t size;
        operator std::wstring_view() const { // NOLINT(runtime/explicit)
          return {data, size};
        }
      };
      std::vector<std::wstring_view, A> v;
      std::array<raw_view, 16> ar;
      for (auto it = splitter.begin(); !it.at_end();) {
        size_t index = 0;
        do {
          ar[index].data = it->data();
          ar[index].size = it->size();
          ++it;
        } while (++index != ar.size() && !it.at_end());
        v.insert(v.end(), ar.begin(), ar.begin() + index);
      }
      return v;
    }
  };

  // Partial specialization for a std::vector<std::wstring>.
  //
  // Optimized for the common case of splitting to a std::vector<std::wstring>.
  // In this case we first split the results to a std::vector<std::wstring_view>
  // so the returned std::vector<std::wstring> can have space reserved to avoid
  // std::wstring moves.
  template <typename A> struct ConvertToContainer<std::vector<std::wstring, A>, std::wstring, false> {
    std::vector<std::wstring, A> operator()(const Splitter &splitter) const {
      const std::vector<std::wstring_view> v = splitter;
      return std::vector<std::wstring, A>(v.begin(), v.end());
    }
  };

  // Partial specialization for containers of pairs (e.g., maps).
  //
  // The algorithm is to insert a new pair into the map for each even-numbered
  // item, with the even-numbered item as the key with a default-constructed
  // value. Each odd-numbered item will then be assigned to the last pair's
  // value.
  template <typename Container, typename First, typename Second>
  struct ConvertToContainer<Container, std::pair<const First, Second>, true> {
    using iterator = typename Container::iterator;

    Container operator()(const Splitter &splitter) const {
      Container m;
      iterator it;
      bool insert = true;
      for (const std::wstring_view sv : splitter) {
        if (insert) {
          it = InsertOrEmplace(&m, sv);
        } else {
          it->second = Second(sv);
        }
        insert = !insert;
      }
      return m;
    }
    // Inserts the key and an empty value into the map, returning an iterator to
    // the inserted item. We use emplace() if available, otherwise insert().
    template <typename M>
    static std::enable_if_t<HasEmplace<M>::value, iterator> InsertOrEmplace(M *m, std::wstring_view key) {
      // Use piecewise_construct to support old versions of gcc in which pair
      // constructor can't otherwise construct string from string_view.
      return ToIter(m->emplace(std::piecewise_construct, std::make_tuple(key), std::tuple<>()));
    }
    template <typename M>
    static std::enable_if_t<!HasEmplace<M>::value, iterator> InsertOrEmplace(M *m, std::wstring_view key) {
      return ToIter(m->insert(std::make_pair(First(key), Second(L""))));
    }

    static iterator ToIter(std::pair<iterator, bool> pair) { return pair.first; }
    static iterator ToIter(iterator iter) { return iter; }
  };

  StringType text_;
  Delimiter delimiter_;
  Predicate predicate_;
};

} // namespace strings_internal
} // namespace bela

#endif

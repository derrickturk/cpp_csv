#ifndef CSV_H
#define CSV_H

#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <utility>
#include <stdexcept>

namespace csv {

class iterator {
  public:
      using value_type = std::vector<std::string>;
      using difference_type = void;
      using pointer = value_type*;
      using reference = value_type&;
      using iterator_category = std::input_iterator_tag;

      iterator() noexcept
          : it_(), line_(), done_(true) { }

      iterator(std::istream& is)
          : it_(is), line_(), done_(false)
      {
          parse_line_();
      }

      bool operator==(const iterator& other) const noexcept
      {
          return it_ == other.it_ && done_ == other.done_;
      }

      reference operator*() noexcept
      {
          return line_;
      }

      pointer operator->() noexcept
      {
          return &line_;
      }

      const value_type& operator*() const noexcept
      {
          return line_;
      }

      const value_type* operator->() const noexcept
      {
          return &line_;
      }

      iterator& operator++()
      {
          if (it_ == std::istreambuf_iterator<char>()) {
              done_ = true;
          } else {
              parse_line_();
          }
          return *this;
      }

      iterator operator++(int)
      {
          iterator other(*this);
          ++(*this);
          return other;
      }

  private:
      std::istreambuf_iterator<char> it_;
      std::vector<std::string> line_;
      bool done_;

      enum class parse_state_ {
          normal,
          quoted,
          quote_in_quoted,
      };

      void parse_line_();
};

class range {
  public:
    constexpr range(std::istream& is) noexcept
        : is_(is) { }

    iterator begin()
    {
        return iterator(is_);
    }

    iterator end() noexcept
    {
        return iterator();
    }

  private:
    std::istream& is_;
};

}

#endif

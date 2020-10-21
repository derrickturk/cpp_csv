#ifndef NAMED_H
#define NAMED_H

#include <algorithm>
#include <cstddef>
#include <tuple>
#include <unordered_map>

#include "csv.h"
#include "parse.h"

namespace named {

template<std::size_t N>
struct symbol {
    char name[N];

    constexpr symbol(const char (&str)[N]) noexcept
    {
        std::ranges::copy(str, name);
    }
};

template<class T, symbol Name>
struct named {
    using type = T;
    static inline constexpr symbol name = Name;
    static inline constexpr const char *name_c_str = Name.name;
};

template<auto... Names>
class name_sequence { };

template<class... Cols>
struct tuple
{
    using names = name_sequence<Cols::name...>;
    std::tuple<typename Cols::type...> values;
};

template<class... Cols>
class tuple_iterator {
  public:
    using value_type = tuple<Cols...>;
    using difference_type = void;
    using pointer = void;
    using reference = value_type; // we don't want to hold a tuple
    using iterator_category = std::input_iterator_tag;

    tuple_iterator() noexcept
        : it_(), cols_() { }

    tuple_iterator(std::istream& is)
        : it_(is), cols_()
    {
        auto header = *it_++;

        for (std::size_t i = 0; auto& key : header) {
            cols_.emplace(std::move(key), i++);
        }

        if (!cols_valid_(typename tuple<Cols...>::names()))
            throw std::runtime_error("required columns not found in header");
    }

    bool operator==(const tuple_iterator& other) const noexcept
    {
        return it_ == other.it_;
    }

    reference operator*() noexcept
    {
        auto line = *it_;
        return {{
            parse::parse<typename Cols::type>(line[cols_[Cols::name.name]])...
        }};
    }

    tuple_iterator& operator++()
    {
        ++it_;
        return *this;
    }

    tuple_iterator operator++(int)
    {
        tuple_iterator other(*this);
        ++(*this);
        return other;
    }

  private:
    csv::iterator it_;
    std::unordered_map<std::string, std::size_t> cols_;

    template<auto... Names>
    bool cols_valid_(name_sequence<Names...>) const noexcept
    {
        return (... && cols_.contains(Names.name));
    }
};

template<class... Cols>
class tuple_range {
  public:
    constexpr tuple_range(std::istream& is) noexcept
        : is_(is) { }

    tuple_iterator<Cols...> begin()
    {
        return { is_ };
    }

    tuple_iterator<Cols...> end() noexcept
    {
        return { };
    }

  private:
    std::istream& is_;
};

namespace literals {

template<symbol S>
constexpr auto operator""_sym() noexcept
{
    return S;
}

}

}

#endif

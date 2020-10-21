#ifndef PARSE_H
#define PARSE_H

#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

namespace parse {

template<class T>
struct parser;

#define MAKE_BASIC_PARSER(ty, fn)\
template <>\
struct parser<ty> {\
    ty operator()(std::string s)\
    {\
        return fn(s);\
    }\
}

MAKE_BASIC_PARSER(int, std::stoi);
MAKE_BASIC_PARSER(long, std::stol);
MAKE_BASIC_PARSER(long long, std::stoll);
MAKE_BASIC_PARSER(unsigned long, std::stoul);
MAKE_BASIC_PARSER(unsigned long long, std::stoull);
MAKE_BASIC_PARSER(float, std::stof);
MAKE_BASIC_PARSER(double, std::stod);
MAKE_BASIC_PARSER(long double, std::stold);

#undef MAKE_BASIC_PARSER

template<>
struct parser<std::string> {
    std::string operator()(std::string s) noexcept
    {
        return s;
    }
};

template<>
struct parser<unsigned> {
    unsigned operator()(std::string s)
    {
        auto ul = parser<unsigned long>()(s);
        if (ul > std::numeric_limits<unsigned>::max())
            throw std::out_of_range("parse<unsigned>");
        return ul;
    }
};

template<class T>
struct parser<std::optional<T>> {
    std::optional<T> operator()(std::string s)
      noexcept(noexcept(parser<T>()(s)))
    {
        if (s.empty())
            return {};
        return parser<T>()(s);
    }
};

template<class T>
T parse(std::string s)
{
    return parser<T>()(std::move(s));
}

}

#endif

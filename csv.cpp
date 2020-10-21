#include "csv.h"

namespace csv {

void iterator::parse_line_()
{
    line_.clear();
    parse_state_ state = parse_state_::normal;
    std::string word;

    while (it_ != std::istreambuf_iterator<char>()) {
        char c = *it_++;
        switch (state) {
            case parse_state_::normal:
                switch (c) {
                    case ',':
                        line_.emplace_back(std::move(word));
                        word.clear();
                        break;

                    case '"':
                        state = parse_state_::quoted;
                        break;

                    case '\n':
                        if (!word.empty())
                            line_.emplace_back(std::move(word));
                        return;

                    default:
                        word.push_back(c);
                }
                break;

            case parse_state_::quoted:
                switch (c) {
                    case '"':
                        state = parse_state_::quote_in_quoted;
                        break;

                    default:
                        word.push_back(c);
                }
                break;

            case parse_state_::quote_in_quoted:
                switch (c) {
                    case '"':
                        state = parse_state_::quoted;
                        word.push_back('"');
                        break;

                    case ',':
                        state = parse_state_::normal;
                        line_.emplace_back(std::move(word));
                        word.clear();
                        break;

                    case '\n':
                        if (!word.empty())
                            line_.emplace_back(std::move(word));
                        return;

                    default:
                        throw std::runtime_error(
                          "Invalid character after \"");
                }
                break;
        }
    }

    switch (state) {
        case parse_state_::normal:
            if (line_.empty()) {
                done_ = true;
                break;
            }
            [[fallthrough]];
        case parse_state_::quote_in_quoted:
            throw std::runtime_error("No terminating newline.");
        case parse_state_::quoted:
            throw std::runtime_error("Unterminated \" quote.");
    }
}

}

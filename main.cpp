#include <iostream>
#include <fstream>

#include "csv.h"
#include "named.h"
#include "parse.h"

int main(int argc, char *argv[])
{
    named::tuple<
        named::named<int, "x">,
        named::named<double, "y">
    > r { { 1, 2.3 } };

    auto [x, y] = r.values;
    std::cout << x << ", " << y << '\n';

    unsigned u = parse::parse<unsigned>("12345");
    std::cout << "parsed " << u << '\n';

    auto o = parse::parse<std::optional<int>>("123");
    if (o)
        std::cout << "parsed " << o.value() << '\n';

    auto o2 = parse::parse<std::optional<int>>("");
    if (o2)
        std::cout << "parsed " << o2.value() << '\n';

    for (int i = 1; i < argc; ++i) {
        std::cout << argv[i] << ":\n";
        std::ifstream csv_file(argv[i]);

        for (const auto& row : named::tuple_range<
            named::named<int, "x">,
            named::named<double, "y">>(csv_file)) {

            auto [x, y] = row.values;
            std::cout << "row: x = " << x << ", y = " << y << '\n';
        }

        /*
        for (const auto& line : csv::range(csv_file)) {
            const char *sep = "";
            for (const auto &word : line) {
                std::cout << sep << word;
                sep = ", ";
            }
            std::cout << '\n';
        }
        */
    }
}

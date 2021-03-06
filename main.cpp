#include <iostream>
#include <fstream>

#include "csv.h"
#include "named.h"
#include "parse.h"

int main(int argc, char *argv[])
{
    using row_type = named::tuple<
        named::named<int, "x">,
        named::named<double, "y">,
        named::named<std::optional<float>, "z">
    >;

    for (int i = 1; i < argc; ++i) {
        std::cout << argv[i] << ":\n";
        std::ifstream csv_file(argv[i]);
        csv::skip_bom(csv_file);

        for (const auto& row : named::tuple_range<row_type>(csv_file)) {
            const auto [x, y, z] = row;
            std::cout << "row: x = " << x << ", y = " << y << ", z = "
              << (z.has_value() ? z.value() : 0.0) << '\n';

            // or!
            std::cout << row << '\n';

            auto also_y = get<"y">(row);
            std::cout << "y = " << also_y << '\n';
        }
    }
}

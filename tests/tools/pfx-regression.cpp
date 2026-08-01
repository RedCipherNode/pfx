#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "regression.hpp"

#include <pfx/engine.hpp>

int main()
{
    std::ifstream input(
        "tests/data/deterministic/input.csv");

    if (!input)
    {
        std::cerr
            << "Failed to open input.csv\n";

        return 1;
    }

    std::ofstream generated(
        pfx::regression::create_filename());

    if (!generated)
    {
        std::cerr
            << "Failed to create generated.csv\n";

        return 1;
    }

    std::string line;

    // Skip Header
    std::getline(
        input,
        line);

    generated
        << "Id,"
        << "Category,"
        << "Input,"
        << "Compatibility,"
        << "Standard,"
        << "Maximum\n";

    size_t total = 0;

    while (std::getline(input, line))
    {
        std::stringstream stream(line);

        std::string id;
        std::string category;
        std::string plaintext;

        std::getline(
            stream,
            id,
            ',');

        std::getline(
            stream,
            category,
            ',');

        std::getline(
            stream,
            plaintext);

        if (id.empty() ||
            category.empty() ||
            plaintext.empty())
        {
            continue;
        }

        auto result =
            pfx::transform(plaintext);

        generated
            << id
            << ','
            << category
            << ','
            << plaintext
            << ','
            << result.compatibility
            << ','
            << result.standard
            << ','
            << result.maximum
            << '\n';

        ++total;
    }

    std::cout
        << "Regression dataset generated.\n";

    std::cout
        << "Cases : "
        << total
        << '\n';

    std::cout
        << "Output: tests/data/v1.x/regression/generated.csv\n";

    return 0;
}
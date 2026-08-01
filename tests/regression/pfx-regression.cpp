#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <pfx/engine.hpp>

int main()
{
    std::ifstream input(
        "tests/data/v1/input.csv");

    if (!input)
    {
        std::cerr
            << "Failed to open input.csv\n";

        return 1;
    }

    std::ofstream golden(
        "tests/data/v1/golden.csv");

    if (!golden)
    {
        std::cerr
            << "Failed to create golden.csv\n";

        return 1;
    }

    std::string line;

    // Skip Header
    std::getline(
        input,
        line);

    golden
        << "Category,"
        << "Input,"
        << "Compatibility,"
        << "Standard,"
        << "Maximum\n";

    size_t total = 0;

    while (std::getline(input, line))
    {
        std::stringstream stream(line);

        std::string category;
        std::string plaintext;

        std::getline(
            stream,
            category,
            ',');

        std::getline(
            stream,
            plaintext);

        auto result =
            pfx::transform(plaintext);

        golden
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
        << "Golden dataset generated.\n";

    std::cout
        << "Cases : "
        << total
        << '\n';

    std::cout
        << "Output: tests/data/v1/golden.csv\n";

    return 0;
}
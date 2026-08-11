#include <iostream>
#include <stdexcept>

#include "regression.hpp"

int main()
{
    try
    {
        const auto run =
            pfx::regression::run_current();

        const auto path =
            pfx::regression::save(run);

        std::cout
            << "Regression dataset generated.\n";

        std::cout
            << "Cases : "
            << run.cases.size()
            << '\n';

        std::cout
            << "Output: "
            << path
            << '\n';

        return 0;
    }
    catch (const std::exception &error)
    {
        std::cerr
            << error.what()
            << '\n';

        return 1;
    }
}
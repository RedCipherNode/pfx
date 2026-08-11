#include "regression.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <vector>

namespace
{
    struct Divergence
    {
        std::size_t index;

        char compatibility;
        char standard;
        char maximum;
    };

    struct Analysis
    {
        std::vector<Divergence> divergences;
    };

    Analysis analyze_case(
        const pfx::regression::Case &test_case)
    {
        Analysis analysis;

        const auto &compatibility =
            test_case.compatibility;

        const auto &standard =
            test_case.standard;

        const auto &maximum =
            test_case.maximum;

        const auto length =
            std::min(
                compatibility.size(),
                std::min(
                    standard.size(),
                    maximum.size()));

        for (std::size_t index = 0;
             index < length;
             ++index)
        {
            if (compatibility[index] ==
                    standard[index] &&
                standard[index] ==
                    maximum[index])
            {
                continue;
            }

            analysis.divergences.push_back(
                {index,
                 compatibility[index],
                 standard[index],
                 maximum[index]});
        }

        return analysis;
    }
}

int main()
{
    try
    {
        const auto run =
            pfx::regression::load_latest();

        if (!run)
        {
            std::cerr
                << "No regression dataset found.\n";

            return 1;
        }

        std::size_t aligned_cases = 0;
        std::size_t diverged_cases = 0;

        for (const auto &test_case : run->cases)
        {
            const auto analysis =
                analyze_case(test_case);

            if (analysis.divergences.empty())
            {
                ++aligned_cases;
                continue;
            }

            ++diverged_cases;

            std::cout
                << test_case.id
                << '\n';

            for (const auto &divergence :
                 analysis.divergences)
            {
                std::cout
                    << "  index "
                    << divergence.index
                    << '\n';

                std::cout
                    << "    Compatibility : "
                    << divergence.compatibility
                    << '\n';

                std::cout
                    << "    Standard      : "
                    << divergence.standard
                    << '\n';

                std::cout
                    << "    Maximum       : "
                    << divergence.maximum
                    << '\n';
            }

            std::cout << '\n';
        }

        std::cout
            << "PFX ANALYSIS\n"
            << "============\n\n";

        std::cout
            << "File  : "
            << run->path
            << '\n';

        std::cout
            << "Cases : "
            << run->cases.size()
            << "\n\n";

        std::cout
            << "Aligned  : "
            << aligned_cases
            << '\n';

        std::cout
            << "Diverged : "
            << diverged_cases
            << '\n';

        return 0;
    }
    catch (const std::exception &error)
    {
        std::cerr
            << "Analysis failed: "
            << error.what()
            << '\n';

        return 1;
    }
}

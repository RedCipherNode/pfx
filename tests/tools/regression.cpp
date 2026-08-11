#include <pfx/engine.hpp>
#include "regression.hpp"

#include <chrono>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <utility>
#include <optional>

namespace pfx::regression
{
    std::string get_timestamp()
    {
        auto now =
            std::chrono::system_clock::now();

        auto time =
            std::chrono::system_clock::to_time_t(now);

        std::tm local{};

#ifdef _WIN32
        localtime_s(&local, &time);
#else
        local = *std::localtime(&time);
#endif

        std::ostringstream stream;

        stream
            << std::put_time(
                   &local,
                   "%Y%m%d_%H%M%S");

        return stream.str();
    }

    int next_sequence()
    {
        namespace fs = std::filesystem;

        constexpr std::string_view kRegressionDirectory =
            "tests/data/v1.x/regression/";

        int highest = 0;

        for (const auto &entry : fs::directory_iterator(kRegressionDirectory))
        {
            if (!entry.is_regular_file())
            {
                continue;
            }

            auto filename =
                entry.path().filename().string();

            if (filename.size() < 3)
            {
                continue;
            }

            if (!std::isdigit(filename[0]) ||
                !std::isdigit(filename[1]) ||
                !std::isdigit(filename[2]))
            {
                continue;
            }

            int sequence =
                std::stoi(
                    filename.substr(0, 3));

            highest =
                std::max(
                    highest,
                    sequence);
        }

        return highest + 1;
    }

    std::string create_filename()
    {
        constexpr std::string_view kRegressionDirectory =
            "tests/data/v1.x/regression/";

        std::ostringstream sequence;

        sequence
            << std::setw(3)
            << std::setfill('0')
            << next_sequence();

        return std::string(kRegressionDirectory) +
               sequence.str() +
               "_" +
               get_timestamp() +
               ".csv";
    }

    std::optional<std::string> latest_filename()
    {
        namespace fs = std::filesystem;

        constexpr std::string_view kRegressionDirectory =
            "tests/data/v1.x/regression/";

        int highest = 0;
        std::optional<std::string> latest;

        for (const auto &entry :
             fs::directory_iterator(kRegressionDirectory))
        {
            if (!entry.is_regular_file())
            {
                continue;
            }

            auto filename =
                entry.path().filename().string();

            if (filename.size() < 3)
            {
                continue;
            }

            if (!std::isdigit(filename[0]) ||
                !std::isdigit(filename[1]) ||
                !std::isdigit(filename[2]))
            {
                continue;
            }

            int sequence =
                std::stoi(
                    filename.substr(0, 3));

            if (sequence > highest)
            {
                highest = sequence;
                latest = entry.path().string();
            }
        }

        return latest;
    }

    Run load(
        const std::string &path)
    {
        std::ifstream input(path);

        if (!input)
        {
            throw std::runtime_error(
                "Failed to open regression dataset: " +
                path);
        }

        Run run;

        run.path = path;

        std::string line;

        // Skip header
        std::getline(
            input,
            line);

        while (std::getline(input, line))
        {
            if (line.empty())
            {
                continue;
            }

            std::stringstream stream(line);

            Case test_case;

            std::getline(
                stream,
                test_case.id,
                ',');

            std::getline(
                stream,
                test_case.category,
                ',');

            std::getline(
                stream,
                test_case.input,
                ',');

            std::getline(
                stream,
                test_case.compatibility,
                ',');

            std::getline(
                stream,
                test_case.standard,
                ',');

            std::getline(
                stream,
                test_case.maximum);

            if (test_case.id.empty() ||
                test_case.category.empty() ||
                test_case.input.empty())
            {
                continue;
            }

            run.cases.push_back(
                std::move(test_case));
        }

        return run;
    }

    std::optional<Run> load_latest()
    {
        const auto path =
            latest_filename();

        if (!path)
        {
            return std::nullopt;
        }

        return load(*path);
    }

    Run run_current()
    {
        const std::string kDeterministicInput =
            "tests/data/deterministic/input.csv";

        std::ifstream input(
            kDeterministicInput);

        if (!input)
        {
            throw std::runtime_error(
                "Failed to open deterministic input: " +
                std::string(kDeterministicInput));
        }

        Run run;

        std::string line;

        // Skip Header
        std::getline(
            input,
            line);

        while (std::getline(input, line))
        {
            if (line.empty())
            {
                continue;
            }

            std::stringstream stream(line);

            Case test_case;

            std::getline(
                stream,
                test_case.id,
                ',');

            std::getline(
                stream,
                test_case.category,
                ',');

            std::getline(
                stream,
                test_case.input);

            if (test_case.id.empty() ||
                test_case.category.empty() ||
                test_case.input.empty())
            {
                continue;
            }

            const auto result =
                pfx::transform(test_case.input);

            test_case.compatibility =
                result.compatibility;

            test_case.standard =
                result.standard;

            test_case.maximum =
                result.maximum;

            run.cases.push_back(
                std::move(test_case));
        }

        return run;
    }

    std::string save(
        const Run &run)
    {
        const auto path =
            create_filename();

        std::ofstream generated(path);

        if (!generated)
        {
            throw std::runtime_error(
                "Failed to create regression dataset: " +
                path);
        }

        generated
            << "Id,"
            << "Category,"
            << "Input,"
            << "Compatibility,"
            << "Standard,"
            << "Maximum\n";

        for (const auto &test_case : run.cases)
        {
            generated
                << test_case.id
                << ','
                << test_case.category
                << ','
                << test_case.input
                << ','
                << test_case.compatibility
                << ','
                << test_case.standard
                << ','
                << test_case.maximum
                << '\n';
        }

        return path;
    }

}
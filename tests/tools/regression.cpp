#include "regression.hpp"

#include <chrono>
#include <filesystem>
#include <iomanip>
#include <sstream>

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
}
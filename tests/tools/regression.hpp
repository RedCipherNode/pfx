#pragma once

#include <optional>
#include <string>
#include <vector>

namespace pfx::regression
{
    struct Case
    {
        std::string id;
        std::string category;
        std::string input;

        std::string compatibility;
        std::string standard;
        std::string maximum;
    };

    struct Run
    {
        std::string path;
        std::vector<Case> cases;
    };

    int next_sequence();

    std::string get_timestamp();

    std::string create_filename();

    std::optional<std::string> latest_filename();

    Run load(
        const std::string &path);

    std::optional<Run> load_latest();

    Run run_current();

    std::string save(
        const Run &run);
}
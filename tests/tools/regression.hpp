#pragma once

#include <string>

namespace pfx::regression
{
    int next_sequence();

    std::string get_timestamp();

    std::string create_filename();
}
#pragma once

#include <string>
#include <string_view>

namespace pfx
{
    struct Result
    {
        std::string compatibility;
        std::string standard;
        std::string maximum;
    };

    Result transform(std::string_view plaintext);
}
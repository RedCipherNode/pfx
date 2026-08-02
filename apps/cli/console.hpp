#pragma once

#include <string_view>

namespace pfx::cli::console
{
    enum class Color
    {
        Default,
        Red,
        Green,
        Yellow,
        Cyan
    };

    void set_color(Color color);

    void reset_color();

    void banner();

    void print_prompt();

    void print(std::string_view text);

    void println(std::string_view text);

    void error(std::string_view text);

    void success(std::string_view text);

    void warning(std::string_view text);

    void option(std::string_view text);

    void heading(std::string_view text);
}
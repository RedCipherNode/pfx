#include "console.hpp"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace pfx::cli::console
{

#ifdef _WIN32

    namespace
    {
        HANDLE console_handle()
        {
            return GetStdHandle(STD_OUTPUT_HANDLE);
        }
    }

#endif

    void set_color(Color color)
    {
#ifdef _WIN32

        WORD attribute = FOREGROUND_RED |
                         FOREGROUND_GREEN |
                         FOREGROUND_BLUE;

        switch (color)
        {
        case Color::Default:
            break;

        case Color::Red:
            attribute = FOREGROUND_RED |
                        FOREGROUND_INTENSITY;
            break;

        case Color::Green:
            attribute = FOREGROUND_GREEN |
                        FOREGROUND_INTENSITY;
            break;

        case Color::Yellow:
            attribute = FOREGROUND_RED |
                        FOREGROUND_GREEN |
                        FOREGROUND_INTENSITY;
            break;

        case Color::Cyan:
            attribute = FOREGROUND_GREEN |
                        FOREGROUND_BLUE |
                        FOREGROUND_INTENSITY;
            break;
        }

        SetConsoleTextAttribute(
            console_handle(),
            attribute);

#endif
    }

    void banner()
    {

        std::cout << '\n';

        set_color(Color::Green);

        std::cout
            << " ____  _____ __  __\n"
            << "|  _ \\|  ___|\\ \\/ /\n"
            << "| |_) | |_    \\  /\n"
            << "|  __/|  _|   /  \\\n"
            << "|_|   |_|    /_/\\_\\ \n";

        reset_color();

        std::cout
            << "\nDeterministic Password Transformer\n";
    }

    void heading(
        std::string_view text)
    {
        set_color(
            Color::Cyan);

        std::cout
            << text;

        reset_color();
    }

    void option(
        std::string_view text)
    {
        set_color(
            Color::Green);

        std::cout
            << text;

        reset_color();
    }

    void reset_color()
    {
        set_color(
            Color::Default);
    }

    void print_prompt()
    {
        set_color(
            Color::Green);

        std::cout
            << "\\pfx> ";

        reset_color();
    }

    void print(
        std::string_view text)
    {
        std::cout
            << text;
    }

    void println(
        std::string_view text)
    {
        std::cout
            << text
            << '\n';
    }

    void error(
        std::string_view text)
    {
        set_color(
            Color::Red);

        std::cout
            << text
            << '\n';

        reset_color();
    }

    void success(
        std::string_view text)
    {
        set_color(
            Color::Green);

        std::cout
            << text
            << '\n';

        reset_color();
    }

    void warning(
        std::string_view text)
    {
        set_color(
            Color::Yellow);

        std::cout
            << text
            << '\n';

        reset_color();
    }

}
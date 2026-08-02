#include "cli.hpp"

#include "commands.hpp"

#include <string_view>

namespace pfx::cli
{

    int run(
        int argc,
        char *argv[])
    {
        if (argc < 2)
        {
            return commands::help();
        }

        std::string_view command = argv[1];

        if (command == "--help" ||
            command == "-h")
        {
            return commands::help();
        }

        if (command == "--version" ||
            command == "-v")
        {
            return commands::version();
        }

        return commands::transform(
            argc,
            argv);
    }

}
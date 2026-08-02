#include "commands.hpp"
#include "console.hpp"

#include <pfx/clipboard.hpp>
#include <pfx/engine.hpp>

#include <iostream>
#include <string>
#include <string_view>

namespace
{

    void print_and_copy(
        std::string_view password,
        bool copy)
    {
        std::cout
            << password
            << '\n';

        if (!copy)
        {
            return;
        }

        if (pfx::clipboard::copy(password))
        {
            pfx::cli::console::success(
                "Success: Copied to clipboard.");
        }
        else
        {
            pfx::cli::console::warning(
                "Warning: Failed to copy to clipboard.");
        }
    }

}

int pfx::cli::commands::help()
{
    console::banner();

    std::cout << '\n';

    console::heading("Usage");

    std::cout << "\n\n";

    std::cout
        << "  pfx <plaintext> [";

    console::option("--compat");

    std::cout << "|";

    console::option("--std");

    std::cout << "|";

    console::option("--max");

    std::cout << "] [";

    console::option("--copy");

    std::cout << "]\n";

    return 0;
}

int pfx::cli::commands::version()
{
    console::print_prompt();
    console::println("--version");

    std::cout
        << "\nPFX 0.1.0\n";

    return 0;
}

int pfx::cli::commands::transform(
    int argc,
    char *argv[])
{
    enum class Profile
    {
        All,
        Compatibility,
        Standard,
        Maximum
    };

    std::string_view plaintext = argv[1];

    auto result = pfx::transform(plaintext);
    Profile profile =
        Profile::All;
    bool copy = false;

    for (int i = 2; i < argc; ++i)
    {
        std::string_view arg = argv[i];

        if (arg == "--compat")
        {
            profile = Profile::Compatibility;
        }
        else if (arg == "--std")
        {
            profile = Profile::Standard;
        }
        else if (arg == "--max")
        {
            profile = Profile::Maximum;
        }
        else if (arg == "--copy")
        {
            copy = true;
        }
    }

    console::print_prompt();

    std::cout << plaintext;

    for (int i = 2; i < argc; ++i)
    {
        std::cout << ' ';
        console::option(argv[i]);
    }

    std::cout << "\n\n";

    switch (profile)
    {
    case Profile::All:

        console::heading("Compatibility");
        std::cout
            << "\n  "
            << result.compatibility
            << "\n\n";

        console::heading("Standard");
        std::cout
            << "\n  "
            << result.standard
            << "\n\n";

        console::heading("Maximum");
        std::cout
            << "\n  "
            << result.maximum
            << '\n';

        if (copy)
        {
            console::warning(
                "Warning: Please specify a profile when using --copy.");
        }

        break;

    case Profile::Compatibility:

        print_and_copy(
            result.compatibility,
            copy);

        break;

    case Profile::Standard:

        print_and_copy(
            result.standard,
            copy);

        break;

    case Profile::Maximum:

        print_and_copy(
            result.maximum,
            copy);

        break;
    }

    return 0;
}
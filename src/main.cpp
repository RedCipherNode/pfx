#include <iostream>
#include <string>

#include <pfx/engine.hpp>
#include <pfx/clipboard.hpp>

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
        std::cout
            << "Copied to clipboard.\n";
    }
    else
    {
        std::cout
            << "Failed to copy to clipboard.\n";
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage:\n";
        std::cout << "    pfx <plaintext>\n";
        std::cout << "    pfx <plaintext> --compat\n";
        std::cout << "    pfx <plaintext> --std\n";
        std::cout << "    pfx <plaintext> --max\n";
        return 1;
    }

    enum class Profile
    {
        All,
        Compatibility,
        Standard,
        Maximum
    };

    auto result = pfx::transform(argv[1]);
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

    switch (profile)
    {
    case Profile::All:

        std::cout
            << "\nCompatibility : "
            << result.compatibility
            << '\n';

        std::cout
            << "Standard      : "
            << result.standard
            << '\n';

        std::cout
            << "Maximum       : "
            << result.maximum
            << '\n';

        if (copy)
        {
            std::cout
                << "\nPlease specify a profile when using --copy.\n";
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

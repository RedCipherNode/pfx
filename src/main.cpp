#include <iostream>
#include <string>

#include <pfx/engine.hpp>

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

    if (argc >= 3)
    {
        std::string_view arg =
            argv[2];

        if (arg == "--compat")
        {
            profile =
                Profile::Compatibility;
        }
        else if (arg == "--std")
        {
            profile =
                Profile::Standard;
        }
        else if (arg == "--max")
        {
            profile =
                Profile::Maximum;
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

        break;

    case Profile::Compatibility:

        std::cout
            << result.compatibility
            << '\n';

        break;

    case Profile::Standard:

        std::cout
            << result.standard
            << '\n';

        break;

    case Profile::Maximum:

        std::cout
            << result.maximum
            << '\n';

        break;
    }

    return 0;
}

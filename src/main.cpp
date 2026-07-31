#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>

#include <pfx/engine.hpp>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage:\n";
        std::cout << "    pfx <plaintext>\n";
        std::cout << "    pfx <plaintext> --save\n";
        std::cout << "    pfx <plaintext> --save <file>\n";
        return 1;
    }

    bool save = false;
    std::string output = "tests/output.txt";

    if (argc >= 3)
    {
        if (std::string_view(argv[2]) == "--save")
        {
            save = true;

            if (argc >= 4)
            {
                output = argv[3];
            }
        }
    }

    auto result = pfx::transform(argv[1]);
    std::filesystem::create_directories("tests");

    bool new_file =
        !std::filesystem::exists("tests/result.csv");

    std::ofstream csv(
        "tests/result.csv",
        std::ios::app);
    std::cout << "\n";
    std::cout << "Compatibility : " << result.compatibility << '\n';
    std::cout << "Standard      : " << result.standard << '\n';
    std::cout << "Maximum       : " << result.maximum << '\n';

    if (save)
    {
        std::ofstream file(output, std::ios::app);

        file << "========================================\n";
        file << "Input         : " << argv[1] << '\n';
        file << "Compatibility : " << result.compatibility << '\n';
        file << "Standard      : " << result.standard << '\n';
        file << "Maximum       : " << result.maximum << '\n';
        file << '\n';
    }

    return 0;
}
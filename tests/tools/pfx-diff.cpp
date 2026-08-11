#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "regression.hpp"

namespace
{
    struct Divergence
    {
        std::size_t index;
    };

    std::vector<Divergence> find_divergences(
        const pfx::regression::Case &test_case)
    {
        std::vector<Divergence> result;

        const auto length =
            std::min(
                test_case.compatibility.size(),
                std::min(
                    test_case.standard.size(),
                    test_case.maximum.size()));

        for (std::size_t index = 0;
             index < length;
             ++index)
        {
            const char compatibility =
                test_case.compatibility[index];

            const char standard =
                test_case.standard[index];

            const char maximum =
                test_case.maximum[index];

            if (compatibility == standard &&
                standard == maximum)
            {
                continue;
            }

            result.push_back(
                {index});
        }

        return result;
    }

    bool is_divergence(
        const std::size_t index,
        const std::vector<Divergence> &divergences)
    {
        for (const auto &divergence :
             divergences)
        {
            if (divergence.index == index)
            {
                return true;
            }
        }

        return false;
    }

    std::string escape_html(
        const std::string &value)
    {
        std::string result;

        for (const char character : value)
        {
            switch (character)
            {
            case '&':
                result += "&amp;";
                break;

            case '<':
                result += "&lt;";
                break;

            case '>':
                result += "&gt;";
                break;

            case '"':
                result += "&quot;";
                break;

            case '\'':
                result += "&#39;";
                break;

            default:
                result += character;
                break;
            }
        }

        return result;
    }

    std::string render_character(
        const char character,
        const bool highlighted)
    {
        std::ostringstream html;

        html
            << "<span class=\"char";

        if (highlighted)
        {
            html
                << " divergence";
        }

        html
            << "\">"
            << escape_html(
                   std::string(1, character))
            << "</span>";

        return html.str();
    }

    std::string render_profile(
        const std::string &label,
        const std::string &value,
        const std::vector<Divergence> &divergences,
        const std::size_t width)
    {
        std::ostringstream html;

        html
            << "            <div class=\"profile\">\n"
            << "                <div class=\"label\">"
            << label
            << "</div>\n"
            << "                <div class=\"characters\">";

        for (std::size_t index = 0;
             index < width;
             ++index)
        {
            if (index >= value.size())
            {
                html
                    << "<span class=\"char empty\">"
                    << " "
                    << "</span>";

                continue;
            }

            html
                << render_character(
                       value[index],
                       is_divergence(
                           index,
                           divergences));
        }

        html
            << "</div>\n"
            << "            </div>\n";

        return html.str();
    }

    std::string render_index_row(
        const std::size_t width,
        const std::vector<Divergence> &divergences)
    {
        std::ostringstream html;

        html
            << "            <div class=\"index-row\">\n"
            << "                <div class=\"label\"></div>\n"
            << "                <div class=\"characters\">";

        for (std::size_t index = 0;
             index < width;
             ++index)
        {
            const bool highlighted =
                is_divergence(
                    index,
                    divergences);

            html
                << "<span class=\"index";

            if (highlighted)
            {
                html
                    << " divergence-index";
            }

            html
                << "\">"
                << index
                << "</span>";
        }

        html
            << "</div>\n"
            << "            </div>\n";

        return html.str();
    }

    std::string render_case(
        const pfx::regression::Case &test_case,
        const std::vector<Divergence> &divergences)
    {
        std::ostringstream html;

        const auto width =
            std::max(
                test_case.compatibility.size(),
                std::max(
                    test_case.standard.size(),
                    test_case.maximum.size()));

        html
            << "        <section class=\"case\">\n"

            << "            <div class=\"case-header\">\n"
            << "                <h2>"
            << escape_html(test_case.id)
            << "</h2>\n"

            << "                <div class=\"category\">"
            << escape_html(test_case.category)
            << "</div>\n"

            << "                <div class=\"input\">"
            << "Input: "
            << escape_html(test_case.input)
            << "</div>\n"

            << "            </div>\n\n";

        html
            << render_index_row(
                   width,
                   divergences);

        html
            << render_profile(
                   "Compatibility",
                   test_case.compatibility,
                   divergences,
                   width);

        html
            << render_profile(
                   "Standard",
                   test_case.standard,
                   divergences,
                   width);

        html
            << render_profile(
                   "Maximum",
                   test_case.maximum,
                   divergences,
                   width);

        html
            << "        </section>\n";

        return html.str();
    }

    std::string build_html(
        const pfx::regression::Run &run,
        const std::string &snapshot_name)
    {
        std::ostringstream html;

        std::size_t divergent_cases = 0;
        std::size_t divergent_positions = 0;

        for (const auto &test_case :
             run.cases)
        {
            const auto divergences =
                find_divergences(test_case);

            if (divergences.empty())
            {
                continue;
            }

            ++divergent_cases;

            divergent_positions +=
                divergences.size();
        }

        html
            << "<!DOCTYPE html>\n"
            << "<html>\n"
            << "<head>\n"
            << "    <meta charset=\"UTF-8\">\n"
            << "    <meta name=\"viewport\" "
               "content=\"width=device-width, initial-scale=1.0\">\n"
            << "    <title>PFX Diff — "
            << escape_html(snapshot_name)
            << "</title>\n\n"

            << "    <style>\n"

            << "        body {\n"
            << "            margin: 32px;\n"
            << "            background: #111;\n"
            << "            color: #eee;\n"
            << "            font-family: Consolas, "
               "\"Courier New\", monospace;\n"
            << "        }\n\n"

            << "        h1 {\n"
            << "            margin: 0;\n"
            << "            font-size: 24px;\n"
            << "        }\n\n"

            << "        .meta {\n"
            << "            margin-top: 8px;\n"
            << "            margin-bottom: 32px;\n"
            << "            color: #888;\n"
            << "            line-height: 1.6;\n"
            << "        }\n\n"

            << "        .case {\n"
            << "            margin-bottom: 28px;\n"
            << "            padding: 20px;\n"
            << "            background: #181818;\n"
            << "            border: 1px solid #303030;\n"
            << "        }\n\n"

            << "        .case-header {\n"
            << "            margin-bottom: 16px;\n"
            << "        }\n\n"

            << "        .case h2 {\n"
            << "            margin: 0;\n"
            << "            font-size: 20px;\n"
            << "        }\n\n"

            << "        .category {\n"
            << "            margin-top: 4px;\n"
            << "            color: #888;\n"
            << "        }\n\n"

            << "        .input {\n"
            << "            margin-top: 4px;\n"
            << "            color: #aaa;\n"
            << "        }\n\n"

            << "        .profile,\n"
            << "        .index-row {\n"
            << "            display: flex;\n"
            << "            margin-top: 8px;\n"
            << "        }\n\n"

            << "        .label {\n"
            << "            width: 18ch;\n"
            << "            flex-shrink: 0;\n"
            << "        }\n\n"

            << "        .characters {\n"
            << "            display: flex;\n"
            << "        }\n\n"

            << "        .char,\n"
            << "        .index {\n"
            << "            width: 1.2ch;\n"
            << "            min-width: 1.2ch;\n"
            << "            height: 1.5em;\n"
            << "            box-sizing: border-box;\n"
            << "            text-align: center;\n"
            << "        }\n\n"

            << "        .index {\n"
            << "            color: #444;\n"
            << "            font-size: 10px;\n"
            << "        }\n\n"

            << "        .divergence {\n"
            << "            background: #7f1d1d;\n"
            << "            color: #fff;\n"
            << "        }\n\n"

            << "        .divergence-index {\n"
            << "            color: #ff7777;\n"
            << "        }\n\n"

            << "        .empty {\n"
            << "            color: transparent;\n"
            << "        }\n\n"

            << "    </style>\n"
            << "</head>\n\n"

            << "<body>\n"

            << "    <h1>PFX DIFF — "
            << escape_html(snapshot_name)
            << "</h1>\n\n"

            << "    <div class=\"meta\">\n"
            << "        Snapshot: "
            << escape_html(run.path)
            << "<br>\n"
            << "        Cases: "
            << run.cases.size()
            << "<br>\n"
            << "        Divergent cases: "
            << divergent_cases
            << "<br>\n"
            << "        Divergent positions: "
            << divergent_positions
            << "\n"
            << "    </div>\n";

        for (const auto &test_case :
             run.cases)
        {
            const auto divergences =
                find_divergences(test_case);

            if (divergences.empty())
            {
                continue;
            }

            html
                << '\n'
                << render_case(
                       test_case,
                       divergences);
        }

        html
            << "\n"
            << "</body>\n"
            << "</html>\n";

        return html.str();
    }
}

int main()
{
    try
    {
        const auto run =
            pfx::regression::load_latest();

        if (!run)
        {
            std::cerr
                << "No regression dataset found.\n";

            return 1;
        }

        namespace fs = std::filesystem;

        const fs::path regression_path =
            run->path;

        const std::string snapshot_name =
            regression_path.stem().string();

        const fs::path output_directory =
            "tests/data/v1.x/bug";

        fs::create_directories(
            output_directory);

        const fs::path output_path =
            output_directory /
            (snapshot_name + "_diff.html");

        std::ofstream output(
            output_path);

        if (!output)
        {
            std::cerr
                << "Failed to create diff report: "
                << output_path.string()
                << '\n';

            return 1;
        }

        output
            << build_html(
                   *run,
                   snapshot_name);

        std::cout
            << "PFX diff report generated.\n";

        std::cout
            << "Snapshot : "
            << run->path
            << '\n';

        std::cout
            << "Output   : "
            << output_path.string()
            << '\n';

        return 0;
    }
    catch (const std::exception &error)
    {
        std::cerr
            << "Diff failed: "
            << error.what()
            << '\n';

        return 1;
    }
}
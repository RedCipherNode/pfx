#include <pfx/engine.hpp>

#include <array>
#include <string_view>
#include <argon2.h>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <vector>

namespace
{
    // Config
    constexpr uint32_t kIterations = 3;
    constexpr uint32_t kMemory = 1 << 16;
    constexpr uint32_t kParallelism = 1;
    constexpr size_t kHashLength = 32;
    constexpr std::string_view kSalt = "PFX-Default-Salt";

    // Schedule
    std::array<uint8_t, kHashLength> build_schedule(
        const std::array<uint8_t, kHashLength> &hash)
    {
        std::array<uint8_t, kHashLength> schedule{};

        for (size_t i = 0; i < kHashLength; ++i)
        {
            schedule[i] = static_cast<uint8_t>(i);
        }

        for (size_t i = kHashLength - 1; i > 0; --i)
        {
            size_t j = hash[i] % (i + 1);

            std::swap(
                schedule[i],
                schedule[j]);
        }

        return schedule;
    }

    // Character Sets
    constexpr std::string_view kUppercase =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    constexpr std::string_view kLowercase =
        "abcdefghijklmnopqrstuvwxyz";

    constexpr std::string_view kDigits =
        "0123456789";

    constexpr std::string_view kSymbols =
        "!@#$%^&*()-_=+";

    constexpr std::string_view kCharset =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "!@#$%^&*()-_=+";

    // Formatter
    std::string format(
        const std::array<uint8_t, kHashLength> &hash,
        const std::array<uint8_t, kHashLength> &schedule,
        std::size_t length)
    {
        std::string output;
        output.reserve(length);

        for (std::size_t i = 0; i < length; ++i)
        {
            output += kCharset[hash[schedule[i]] % kCharset.size()];
        }

        return output;
    }

    // Policy Validator
    struct PolicyReport
    {
        bool has_upper = false;
        bool has_lower = false;
        bool has_digit = false;
        bool has_symbol = false;

        bool valid() const
        {
            return has_upper &&
                   has_lower &&
                   has_digit &&
                   has_symbol;
        }
    };

    bool has_upper(std::string_view password)
    {
        for (char c : password)
        {
            if (std::isupper(static_cast<unsigned char>(c)))
            {
                return true;
            }
        }

        return false;
    }

    bool has_lower(std::string_view password)
    {
        for (char c : password)
        {
            if (std::islower(static_cast<unsigned char>(c)))
            {
                return true;
            }
        }

        return false;
    }

    bool has_digit(std::string_view password)
    {
        for (char c : password)
        {
            if (std::isdigit(static_cast<unsigned char>(c)))
            {
                return true;
            }
        }

        return false;
    }

    bool has_symbol(std::string_view password)
    {
        for (char c : password)
        {
            if (!std::isalnum(static_cast<unsigned char>(c)))
            {
                return true;
            }
        }

        return false;
    }

    PolicyReport validate_policy(std::string_view password)
    {
        PolicyReport report;

        report.has_upper = has_upper(password);
        report.has_lower = has_lower(password);
        report.has_digit = has_digit(password);
        report.has_symbol = has_symbol(password);

        return report;
    }

    void print_policy(const PolicyReport &report)
    {
        std::cout << "\nPolicy\n\n";

        std::cout
            << "Upper : "
            << (report.has_upper ? "PASS" : "FAIL")
            << '\n';

        std::cout
            << "Lower : "
            << (report.has_lower ? "PASS" : "FAIL")
            << '\n';

        std::cout
            << "Digit : "
            << (report.has_digit ? "PASS" : "FAIL")
            << '\n';

        std::cout
            << "Symbol: "
            << (report.has_symbol ? "PASS" : "FAIL")
            << '\n';
    }

    // Policy Repair Planner

    char select_upper(
        const std::array<uint8_t, kHashLength> &hash,
        const std::array<uint8_t, kHashLength> &schedule)
    {
        return kUppercase[hash[schedule[3]] % kUppercase.size()];
    }

    char select_lower(
        const std::array<uint8_t, kHashLength> &hash,
        const std::array<uint8_t, kHashLength> &schedule)
    {
        return kLowercase[hash[schedule[7]] % kLowercase.size()];
    }

    char select_digit(
        const std::array<uint8_t, kHashLength> &hash,
        const std::array<uint8_t, kHashLength> &schedule)
    {
        return kDigits[hash[schedule[11]] % kDigits.size()];
    }

    char select_symbol(
        const std::array<uint8_t, kHashLength> &hash,
        const std::array<uint8_t, kHashLength> &schedule)
    {
        return kSymbols[hash[schedule[15]] % kSymbols.size()];
    }

    size_t select_position(
        size_t seed,
        std::size_t length)
    {
        return seed % length;
    }

    // Policy Repair Apply
    void apply_policy(
        std::string &password,
        const std::array<uint8_t, kHashLength> &hash,
        const std::array<uint8_t, kHashLength> &schedule)
    {
        if (!has_upper(password))
        {
            password[select_position(
                schedule[3],
                password.size())] =
                select_upper(
                    hash,
                    schedule);
        }

        if (!has_lower(password))
        {
            password[select_position(
                schedule[7],
                password.size())] =
                select_lower(
                    hash,
                    schedule);
        }

        if (!has_digit(password))
        {
            password[select_position(
                schedule[11],
                password.size())] =
                select_digit(
                    hash,
                    schedule);
        }

        if (!has_symbol(password))
        {
            password[select_position(
                schedule[15],
                password.size())] =
                select_symbol(
                    hash,
                    schedule);
        }
    }

}

// Transform
pfx::Result pfx::transform(std::string_view plaintext)
{
    std::array<uint8_t, kHashLength> hash{};

    int rc = argon2id_hash_raw(
        kIterations,
        kMemory,
        kParallelism,

        plaintext.data(),
        plaintext.size(),

        kSalt.data(),
        kSalt.size(),

        hash.data(),
        hash.size());

    if (rc != ARGON2_OK)
    {
        return {};
    }

    auto schedule = build_schedule(hash);
    auto compatibility =
        format(hash, schedule, 16);

    auto standard =
        format(hash, schedule, 24);

    auto maximum =
        format(hash, schedule, 32);

    apply_policy(
        compatibility,
        hash,
        schedule);

    apply_policy(
        standard,
        hash,
        schedule);

    apply_policy(
        maximum,
        hash,
        schedule);

    return {
        compatibility,
        standard,
        maximum};
}

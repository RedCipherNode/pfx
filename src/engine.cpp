#include <pfx/engine.hpp>

#include <array>
#include <string_view>
#include <argon2.h>
#include <algorithm>
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

    return {
        compatibility,
        standard,
        maximum};
}

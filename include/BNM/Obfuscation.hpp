#pragma once

#include <cstdint>
#include <string_view>
#include <cstring>

namespace BNM::Obfuscation {

constexpr uint32_t Fnv1a32(const char *str, size_t n) {
    uint32_t h = 2166136261u;
    for (size_t i = 0; i < n; ++i) h = (h ^ (uint8_t) str[i]) * 16777619u;
    return h;
}

constexpr uint64_t Mix64(uint64_t x) {
    x ^= x >> 30;
    x *= 0xbf58476d1ce4e5b9ull;
    x ^= x >> 27;
    x *= 0x94d049bb133111ebull;
    x ^= x >> 31;
    return x;
}

template<size_t N>
struct Cipher {
    uint64_t keyA;
    uint64_t keyB;
    uint8_t blob[N];

    constexpr Cipher(const char (&str)[N], uint64_t a, uint64_t b) : keyA(a), keyB(b) {
        for (size_t i = 0; i < N; ++i) {
            uint64_t k = Mix64(keyA ^ (uint64_t) i * keyB);
            uint8_t c = (uint8_t) str[i];
            blob[i] = (uint8_t) (c ^ (uint8_t) (k >> 56) ^ (uint8_t) (k >> 32) ^ (uint8_t) (k >> 8) ^ (uint8_t) k ^ (uint8_t) (keyB >> ((i % 8) * 8)));
        }
    }

    size_t Decrypt(char *out) const {
        for (size_t i = 0; i < N; ++i) {
            uint64_t k = Mix64(keyA ^ (uint64_t) i * keyB);
            uint8_t c = ((volatile uint8_t *) blob)[i];
            ((volatile char *) out)[i] = (char) (c ^ (uint8_t) (k >> 56) ^ (uint8_t) (k >> 32) ^ (uint8_t) (k >> 8) ^ (uint8_t) k ^ (uint8_t) (keyB >> ((i % 8) * 8)));
        }
        return N;
    }
};

template<size_t N>
const char *Decrypt(const Cipher<N> &cipher) {
    static thread_local char buf[N];
    cipher.Decrypt(buf);
    return buf;
}

template<size_t N>
constexpr size_t StrSize(const char (&)[N]) { return N; }

constexpr uint32_t BuildSalt() { return Fnv1a32(__TIME__, 8); }

}

#define BNM_OBFUSCATE_IMPL(str, salt) \
    ([]() noexcept { \
        constexpr auto bnm_obf_cipher = ::BNM::Obfuscation::Cipher<::BNM::Obfuscation::StrSize(str)>(str, \
            ::BNM::Obfuscation::Mix64((uint64_t) ::BNM::Obfuscation::Fnv1a32(str, sizeof(str) - 1) ^ (uint64_t) (salt) ^ (uint64_t) (__COUNTER__) * 0x9e3779b97f4a7c15ull), \
            ::BNM::Obfuscation::Mix64((uint64_t) (__LINE__) * 0x9e3779b97f4a7c15ull ^ (uint64_t) (__COUNTER__) ^ (uint64_t) (salt))); \
        return ::BNM::Obfuscation::Decrypt(bnm_obf_cipher); \
    }())

#define BNM_OBFUSCATE(str) BNM_OBFUSCATE_IMPL(str, ::BNM::Obfuscation::BuildSalt())
#define BNM_OBFUSCATE_TMP(str) BNM_OBFUSCATE(str)

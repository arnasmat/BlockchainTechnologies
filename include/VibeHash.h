//
// Created by arnas on 9/15/25.
//

#ifndef VIBEHASH_H
#define VIBEHASH_H


/*
 * PROMPT (for Claude Sonnet 4):
* <instructions>
You are a senior low level developer and cryptography engineer with a deep and complex understanding of various hashing algorithms such as MD5, SHA256, etc. A professional client has requested you to create a new, unique and efficient hashing algorithm (note that it must differ from the popular hashing algorithms in the way it works - it should not be a recreation of them) for their highly optimized business - please note that it should be scalable and efficient, but shouldn't suffer from typical drawbacks like collisions. Begin by mathematically designing a unique hashing algorithm and afterwards implement it in C++ as a class called VibeHash implementing the provided HashGenInterface. Please note that the interactivity should be like in the humanhash example.
The client has requested for the algorithm to have the following features:
* deterministic (same input -> same output)
* Consistent output size
* Has as few collissions as possible
* Avalanche effect (small change in input -> big change in output)
* Hiding and puzzle-friendly
</instructions>
<references>

class HashGenInterface {
public:
    virtual ~HashGenInterface() = default;
    virtual std::string generateHash(const std::string& input) const = 0;
};
HumanHash hash;
std::cout<<hash.generateHash("test");
</references>

 */

#include <string>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <array>

#include "HashGenInterface.h"

class VibeHash : public HashGenInterface {
private:
    // Pre-computed Fibonacci sequence for fast lookup (first 24 values)
    static constexpr std::array<uint8_t, 24> fibonacci_phi = {
        1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 121, 98, 219,
        61, 24, 85, 109, 194, 47, 241, 32
    };

    // Prime constants for diffusion matrix
    static constexpr std::array<uint64_t, 16> diffusion_primes = {
        2654435761ULL, 2246822519ULL, 3266489917ULL, 668265263ULL,
        374761393ULL, 2654435761ULL, 1720413743ULL, 3266489917ULL,
        668265263ULL, 374761393ULL, 2654435761ULL, 2246822519ULL,
        3266489917ULL, 1720413743ULL, 374761393ULL, 668265263ULL
    };

    // Large primes for state evolution
    static constexpr std::array<uint64_t, 4> evolution_primes = {
        18446744073709551557ULL,
        18446744073709551533ULL,
        18446744073709551521ULL,
        18446744073709551437ULL
    };

    // Rotation constants
    static constexpr std::array<uint8_t, 4> quantum_rotations = {13, 21, 34, 55};

    // 4D State vector
    struct QuantumState {
        uint64_t dimensions[4];

        QuantumState() {
            dimensions[0] = 0x6A09E667F3BCC908ULL;
            dimensions[1] = 0xBB67AE8584CAA73BULL;
            dimensions[2] = 0x3C6EF372FE94F82BULL;
            dimensions[3] = 0xA54FF53A5F1D36F1ULL;
        }
    };

    inline uint64_t rotateLeft(uint64_t value, uint8_t shift) const {
        // normalize shift to [0,63]
        shift &= 63;
        if (shift == 0) return value;
        return (value << shift) | (value >> (64 - shift));
    }


    inline uint64_t nonLinearMix(uint64_t a, uint64_t b, uint64_t c) const {
        a ^= b; a -= rotateLeft(b, 14);
        c ^= a; c -= rotateLeft(a, 11);
        b ^= c; b -= rotateLeft(c, 25);
        a ^= b; a -= rotateLeft(b, 16);
        c ^= a; c -= rotateLeft(a, 4);
        b ^= c; b -= rotateLeft(c, 14);
        a ^= b; a -= rotateLeft(b, 24);
        return a ^ b ^ c;
    }

    inline uint64_t fibonacciTransform(uint8_t byte, uint64_t state, size_t round) const {
        uint8_t fib_shift = fibonacci_phi[byte % 24];
        uint64_t rotated = rotateLeft(state, fib_shift);
        uint64_t fib_contribution = (uint64_t(fibonacci_phi[(round + byte) % 24]) *
                                   evolution_primes[round % 4]) ^ byte;
        return rotated + fib_contribution;
    }

    void applyDiffusionMatrix(QuantumState& state) const {
        uint64_t temp[4];

        for (int i = 0; i < 4; i++) {
            temp[i] = 0;
            for (int j = 0; j < 4; j++) {
                temp[i] += nonLinearMix(
                    state.dimensions[j],
                    diffusion_primes[i * 4 + j],
                    quantum_rotations[j]
                );
            }
        }

        for (int i = 0; i < 4; i++) {
            state.dimensions[i] = temp[i] ^ evolution_primes[i];
        }
    }

    QuantumState evolveState(const std::string& input) const {
        QuantumState state;

        for (size_t i = 0; i < input.length(); i++) {
            uint8_t byte = static_cast<uint8_t>(input[i]);

            for (int dim = 0; dim < 4; dim++) {
                uint64_t evolved = fibonacciTransform(byte, state.dimensions[dim], i);
                uint64_t influence = state.dimensions[(dim + 1) % 4] ^
                                   rotateLeft(state.dimensions[(dim + 3) % 4],
                                             quantum_rotations[dim]);
                state.dimensions[dim] = nonLinearMix(evolved, influence,
                                                   evolution_primes[dim] + i);
                state.dimensions[dim] += (byte * evolution_primes[dim] * (i + 1));
            }

            if ((i + 1) % 4 == 0) {
                applyDiffusionMatrix(state);
            }
        }

        applyDiffusionMatrix(state);
        applyDiffusionMatrix(state);

        return state;
    }

    std::string compressToHash(const QuantumState& state) const {
        uint32_t compressed[8];

        for (int i = 0; i < 4; i++) {
            uint64_t dim = state.dimensions[i];
            uint32_t high = static_cast<uint32_t>(dim >> 32);
            uint32_t low = static_cast<uint32_t>(dim & 0xFFFFFFFFULL);

            compressed[i * 2] = high ^ rotateLeft(low, 16) ^
                              static_cast<uint32_t>(evolution_primes[i]);
            compressed[i * 2 + 1] = low ^ rotateLeft(high, 11) ^
                                  static_cast<uint32_t>(evolution_primes[i] >> 32);
        }

        for (int round = 0; round < 3; round++) {
            for (int i = 0; i < 8; i++) {
                compressed[i] ^= rotateLeft(compressed[(i + 1) % 8], 7) ^
                               rotateLeft(compressed[(i + 7) % 8], 13);
            }
        }

        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (int i = 0; i < 8; i++) {
            ss << std::setw(8) << compressed[i];
        }

        return ss.str();
    }

public:
    // didnt add the key to be used here because I was too lazy lol
    std::string generateHash(const std::string& input) const override {
        try {
            if (input.empty()) {
                return generateHash(std::string(1, '\0'));
            }


            QuantumState final_state = evolveState(input);
            return compressToHash(final_state);
        } catch (...) {
            // Emergency fallback - should never happen
            return "0000000000000000000000000000000000000000000000000000000000000000";
        }
    }
};


#endif //VIBEHASH_H

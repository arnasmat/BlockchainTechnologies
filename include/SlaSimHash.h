#ifndef SLASIMHASH_H
#define SLASIMHASH_H
// CREATED BY Emilis Slabada and Martynas Simanavicius, code taken from - https://github.com/Specialist13/2025-uni-blockchain

#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <cmath>
#include <sstream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <cstdint>

#include "HashGenInterface.h"
class SlaSimHash: public HashGenInterface {
    std::string slaSimHash(const std::string& input) const {
        if (input.empty()) {
            return "";
        }
        std::vector<int> binary;
        std::string output;

        for (char c : input) {
            std::string bin = std::bitset<8>(c).to_string();
            for (char b : bin) {
                binary.push_back(b == '1' ? 1 : 0);
            }
        }

        std::vector<int> numbers;

        int num=0;

        for (int i = 0; i < binary.size(); i++) {
            num += binary[i] << (7 - i%8);
            if (i % 8 == 7) {
                numbers.push_back(num*(i/8+1));
                num = 0;
            }
        }

        long long total = 0;
        for (int n : numbers) {
            total += n;
        }

        std::vector<int> xor_results={binary[0], binary[1], binary[2], binary[3]};
        std::vector<int> temp;
        for (int i = 4; i < binary.size(); i++) {
            temp.push_back(xor_results[i%4] ^ binary[i]);
            if (i%4==3){
                xor_results = temp;
                temp.clear();
            }
        }


        long long xor_numerical_value = 0;
        for (int i = 0; i < xor_results.size(); i++) {
            xor_numerical_value += xor_results[i] << (xor_results.size() - 1 - i);
        }

        long long offset = 1;
        for (int i = 0; i < xor_numerical_value; i++) {
            offset *= total;
            offset %= (1LL << 8);
        }

        for (int i = 0; i < 8; i++) {
            int zero_count = 0;
            int one_count = 0;

            for (int j = input.size()*i; j < input.size()*(i+1); j ++) {
                if (binary[j] == 0) {
                    zero_count++;
                } else {
                    one_count++;
                }
            }

            long long partial_numerical_hash=1;
            for (int j = 0; j < one_count+offset + 10; j++) {
                partial_numerical_hash *= (zero_count + total + 10 + partial_numerical_hash%10);
                partial_numerical_hash % (1LL << 32) == 0 ? partial_numerical_hash -= 1 : partial_numerical_hash;
                partial_numerical_hash %= (1LL << 32);
            }

            std::stringstream ss;
            ss << std::hex << std::setw(8) << std::setfill('0') << partial_numerical_hash;
            output += ss.str();
        }

        return output;
    }

    // Rotate helpers
    static inline uint32_t rotl32(uint32_t x, int r) {
        return (x << r) | (x >> (32 - r));
    }
    static inline uint64_t rotl64(uint64_t x, int r) {
        return (x << r) | (x >> (64 - r));
    }

    std::string AIImprovedSlaSimHash(std::string& input) {
        if (input.empty()) {
            return "";
        }

        std::vector<int> binary;
        binary.reserve(input.size() * 8);

        // Convert input to binary (fast)
        for (unsigned char c : input) {
            for (int i = 7; i >= 0; --i) {
                binary.push_back((c >> i) & 1);
            }
        }

        // Stage 1: derive per-byte numbers with stronger mixing
        std::vector<uint32_t> numbers;
        numbers.reserve(input.size());

        for (size_t i = 0; i < input.size(); i++) {
            uint32_t val = 0;
            for (int b = 0; b < 8; b++) {
                val |= (binary[i * 8 + b] << (7 - b));
            }
            // multiply by position and add rotation for diffusion
            uint32_t mixed = rotl32(val * (i + 1), (i % 7) + 1) ^ (0x9E3779B9u * (i + 1));
            numbers.push_back(mixed);
        }

        // Stage 2: accumulate total (64-bit to avoid overflow)
        uint64_t total = 0;
        for (uint32_t n : numbers) {
            total += n;
            total = rotl64(total, 3) ^ 0xA5A5A5A5A5A5A5A5ULL; // avalanche each step
        }


        // Stage 3: XOR chain (expanded to 8-way instead of 4-way)
        std::vector<int> xor_results(binary.begin(), binary.begin() + 8);
        std::vector<int> temp;
        for (size_t i = 8; i < binary.size(); i++) {
            temp.push_back(xor_results[i % 8] ^ binary[i]);
            if (i % 8 == 7) {
                xor_results = temp;
                temp.clear();
            }
        }

        // Collapse xor_results into a number
        uint32_t xor_numerical_value = 0;
        for (size_t i = 0; i < xor_results.size(); i++) {
            xor_numerical_value |= (xor_results[i] << (xor_results.size() - 1 - i));
        }

        // Stage 4: offset (use LCG-style iteration to avoid huge multiplications)
        uint64_t offset = 0xDEADBEEF;
        for (uint32_t i = 0; i < xor_numerical_value; i++) {
            offset = offset * 6364136223846793005ULL + (total ^ (i * 0x9E37));
            offset &= 0xFF; // keep in [0,255] like original idea
        }

        // Stage 5: construct 8 blocks (32-bit each → 256-bit total)
        std::string output;
        output.reserve(64); // hex length

        for (int block = 0; block < 8; block++) {
            int zero_count = 0;
            int one_count = 0;

            size_t start = input.size() * block;
            size_t end = std::min(start + input.size(), binary.size());

            for (size_t j = start; j < end; j++) {
                if (binary[j] == 0) zero_count++;
                else one_count++;
            }

            uint64_t partial = (uint64_t)zero_count ^ (total >> (block * 3));
            partial = rotl64(partial + one_count + offset + block * 0x12345, (block + 5));

            // Run controlled multiplicative loop (shorter, non-quadratic)
            for (int k = 0; k < (one_count % 16) + 4; k++) {
                partial = (partial * (0x9E3779B97F4A7C15ULL ^ (partial >> 7))) & 0xFFFFFFFFULL;
                partial ^= rotl64(total, k + block) & 0xFFFFFFFFULL;
            }

            std::stringstream ss;
            ss << std::hex << std::setw(8) << std::setfill('0') << (uint32_t)partial;
            output += ss.str();
        }

        return output;
    }

    void base_input(std::istream& in, std::string& a) {
        std::getline(in, a);
    }

public:
    std::string generateHash(const std::string &input) const override {
        return slaSimHash(input);
    };
};


#endif //SLASIMHASH_H

#ifndef HELPER_H
#define HELPER_H
#include <bitset>
#include <map>

// assumes hash is 64 characters long
std::bitset<256> hashToBits(const std::string& hash) {
    std::bitset<256> bits;
    std::map<char, std::bitset<4>> charToBits = {
        {'0', 0x0},
        {'1', 0x1},
        {'2', 0x2},
        {'3', 0x3},
        {'4', 0x4},
        {'5', 0x5},
        {'6', 0x6},
        {'7', 0x7},
        {'8', 0x8},
        {'9', 0x9},
        {'a', 0xA},
        {'b', 0xB},
        {'c', 0xC},
        {'d', 0xD},
        {'e', 0xE},
        {'f', 0xF}
    };

    for (size_t i = 0; i < hash.size() && i < 64; ++i) {
        std::bitset<4> charBits = charToBits.at(hash[i]);
        for (size_t j = 0; j < 4; ++j) {
            bits[i * 4 + j] = charBits[3 - j];
        }
    }
    return bits;
}


#endif //HELPER_H

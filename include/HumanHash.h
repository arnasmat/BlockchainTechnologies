#ifndef HUMANHASH_H
#define HUMANHASH_H

#include <bitset>
#include <sstream>
#include <string>

#include "HashGenInterface.h"

// TODO: this is a random stupid hash idea that hasn't been tested lmso
class HumanHash: public HashGenInterface {
public:
    std::string generateHash(const std::string& input) const override {
        int hash[64] = {};

        for (const char c : input) {
            std::string bin = std::bitset<8>(static_cast<unsigned char>(c)).to_string();

            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    int index = i * j;
                    if (index >= 64) continue;

                    if (bin[i] == '1') {
                        hash[index] += 1;
                    } else if (index > 0) {
                        hash[index] += hash[index - 1];
                    }
                }
            }
        }

        std::string output;
        for (int i : hash) {
            std::stringstream ss;
            ss << std::hex << i;
            output += ss.str();
        }

        return output;
    }
};



#endif //HUMANHASH_H

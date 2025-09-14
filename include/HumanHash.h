#ifndef HUMANHASH_H
#define HUMANHASH_H

#include <iomanip>

#include "HashGenInterface.h"

// TODO: this is a random stupid hash idea that hasn't been tested lmso
class HumanHash: public HashGenInterface {
public:
    std::string generateHash(const std::string& input) const override {
        int hash[64] = {};

        for (const char c : input) {
            std::string bin = std::bitset<8>(static_cast<unsigned char>(c)).to_string();

            for (int i = 1; i <= 8; i++) {
                for (int j = 1; j <= 8; j++) {
                    const int index = i * j - 1;
                    if (index >= 64) continue;

                    if (bin[i] == '1') {
                        hash[index] += 1;
                    } else if (index > 0) {
                        hash[index] += hash[index - 1];
                    }
                }
            }
        }

        // goofy hex casting
        std::string output;
        for (const int i : hash) {
            output += "0123456789abcdef"[i % 16];
        }

        // std::cout<<output.size()<<" "<<output<<"\n";
        return output;
    }
};



#endif //HUMANHASH_H

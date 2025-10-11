#ifndef HUMANHASH_H
#define HUMANHASH_H

#include <iomanip>
#include <bitset>

#include "HashGenInterface.h"

class HumanHash: public HashGenInterface {
public:
    std::string generateHash(const std::string& input) const override {
        try {
            int hash[64]{};

            for (const char c : input) {
                const std::bitset<8> inputCharBits(c);
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++) {
                        const bool bit = inputCharBits.test(j);
                        const int index = 8 * j + i;

                        if (bit) {
                            hash[index] += 1;
                        } else if (index > 0) {
                            hash[index] += hash[index - 1];
                        } else {
                            hash[0] += hash[input.size() % 64];
                        }
                    }
                }
            }

            return castToHex(hash, 64);
        } catch (std::exception& e) {
            std::cerr <<"An error occured: "<< e.what() << '\n';
            // should probabyl rethrow but idrc lmao
            return "";
        }
    }
};


#endif //HUMANHASH_H
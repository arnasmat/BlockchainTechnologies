#ifndef HUMANHASH_H
#define HUMANHASH_H

#include <iomanip>

#include "HashGenInterface.h"

// TODO: make matrixhash reliant on this hash -> also make more consistent naming as currently it's super inconsistent (prolly done)
// i.e. hash[] in HumanHash, but hashArray[] in MatrixHash
// TODO: make hashes work with file inputs as well

// TODO: implement trycatches in code, clean up ts
class HumanHash: public HashGenInterface {
public:
    explicit HumanHash(const int& inputKey=0) : HashGenInterface(inputKey) {}
    std::string generateHash(const std::string& input) const override {
        int hash[64]{};

        for (const char c : input) {
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {

                    const bool bit = (c >> j) & 1;
                    const int index = 8 * j + i;

                    if (bit) {
                        hash[index] += 1;
                    } else if (index > 0) {
                        hash[index] += hash[index - 1];
                    } else {
                        hash[0] += hash[0] + key;
                    }
                }
            }
        }

        return castToHex(hash, 64);
    }
};


#endif //HUMANHASH_H
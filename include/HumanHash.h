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
    std::string generateHash(const std::string& input, int* hashed=nullptr) const override {
        int* hash = new int[64];
        if(hashed != nullptr) hash = hashed;
        

        for (const char c : input) {
            std::string bin = std::bitset<8>(static_cast<unsigned char>(c)).to_string();

            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    const int index = 8 * j + i;
                    if (index >= 64) continue;

                    if (bin[i] == '1') {
                        hash[index] += 1;
                    } else if (index > 0) {
                        hash[index] += hash[index - 1];
                    } else {
                        //goofy ass way to make it edit the first element to prevent some issues
                        hash[0] += hash[i+j];
                    }
                }
            }
        }

        // goofy hex casting
        std::string output;
        for (int i = 0; i < 64; ++i) {
            output += "0123456789abcdef"[hash[i] % 16];
        }

        // std::cout<<output.size()<<" "<<output<<"\n";
        delete[] hash;
        return output;
    }
};


#endif //HUMANHASH_H
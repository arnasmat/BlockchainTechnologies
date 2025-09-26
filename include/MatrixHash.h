//
// Created by arnas on 9/14/25.
//

#ifndef MATRIXHASH_H
#define MATRIXHASH_H
#include <ranges>

#include "HashGenInterface.h"
#include "HumanHash.h"


class MatrixHash : public HashGenInterface {
public:
    std::string generateHash(const std::string &input) const override {
        try{
            int hash[64]{};
            int oneSum{0};

            for (int i = 0; i < input.size(); i++) {
                const unsigned char inputChar = input[i];
                const std::bitset<8> inputCharBits(inputChar);

                for (int j = 0; j < 8; j++) {
                    const bool bit = inputCharBits.test(j);
                    const int index = (i * 8 + j) % 64;

                    if (bit) {
                        hash[index] += 1;
                        oneSum++;
                    } else if (index > 0) {
                        hash[index] += hash[index - 1];
                    } else {
                        hash[0] += hash[input.size() % 64] + oneSum;
                    }

                    hash[index] += (i + j + oneSum % 64) * bit;
                }
            }

            // This is pretty much the same as humanhash, but hash[0] has onesum added and that makes a huge difference lol
            // (and no, converting hash to hex and passing it as a key doesn't work :( )
            for (const int c: hash) {
                for (int i = 0; i < 8; i++) {
                    std::bitset<8> charBits(c);
                    for (int j = 0; j < 8; j++) {
                        const bool bit = charBits.test(j);
                        const int index = 8 * j + i;

                        if (bit) {
                            hash[index] += 1;
                        } else if (index > 0) {
                            hash[index] += hash[index - 1];
                        } else {
                            //The onesum here makes a huge difference lol
                            hash[0] += hash[input.size() % 64] + oneSum;
                        }
                    }
                }
            }

            return castToHex(hash, 64);
        } catch (std::exception& e) {
            std::cerr <<"An error occured: "<< e.what() << '\n';
            return "";
        }
    }
};


#endif //MATRIXHASH_H

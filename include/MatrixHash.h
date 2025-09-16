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
    explicit MatrixHash(const int &inputKey = 0) : HashGenInterface(inputKey) {}

    std::string generateHash(const std::string &input) const override {
        int hash[64]{};
        int oneSum{0};

        for (int i = 0; i < input.size(); i++) {
            const unsigned char inputChar = input[i];

            for (int j = 0; j < 8; j++) {
                //https://stackoverflow.com/a/13823765
                // instead of bitset<8> and casting to string which caused a lot of our issues lol
                const bool bit = (inputChar >> j) & 1;
                const int index = (i * 8 + j) % 64;

                if (bit) {
                    hash[index] += 1;
                    oneSum++;
                } else if (index > 0) {
                    hash[index] += hash[index - 1] + key;
                } else {
                    hash[0] += hash[input.size() % 64] + oneSum;
                }
                hash[index] += (i + j) * bit;
            }
        }

        // This is pretty much the same as humanhash, but hash[0] has onesum added and that makes a huge difference lol
        // (and no, converting hash to hex and passing it as a key doesn't work :( )
        for (const int c: hash) {
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    const bool bit = (c >> j) & 1;
                    const int index = 8 * j + i;

                    if (bit == 1) {
                        hash[index] += 1;
                    } else if (index > 0) {
                        hash[index] += hash[index - 1];
                    } else {
                        //The onesum here makes a huge difference lol
                        hash[0] += hash[0] + oneSum;
                    }
                }
            }
        }

        return castToHex(hash, 64);
    }
};


#endif //MATRIXHASH_H

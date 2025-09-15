//
// Created by arnas on 9/14/25.
//

#ifndef MATRIXHASH_H
#define MATRIXHASH_H
#include "HashGenInterface.h"


class MatrixHash: public HashGenInterface {
public:
    std::string generateHash(const std::string &input) const override {
        const int arrayDimensions = input.size() * 8;
        int tempArray[arrayDimensions]{};
        int hashArray[64]{};
        int oneSum{0};

        for (int i=0; i<input.size(); i++) {
            unsigned char inputChar = input[i];

            for (int j=0; j<8; j++) {
                //https://stackoverflow.com/a/13823765
                // instead of bitset<8> and casting to string which caused a lot of our issues lol
                const bool bit = (inputChar >> j) & 1;
                const int index = (i * 8 + j)%64;

                if (bit == 1) {
                    hashArray[index] += 1;
                    oneSum++;
                } else if (index > 0) {
                    hashArray[index] += hashArray[index - 1];
                } else {
                    hashArray[0] += hashArray[input.size() % 64] + oneSum;
                }

                hashArray[index] += (i + j) * bit;
            }
        }


        for (const int c : hashArray) {
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    const bool bit = (c >> j) & 1;

                    const int index = 8 * j + i;

                    if (bit == 1) {
                        hashArray[index] += 1;
                    } else if (index > 0) {
                        hashArray[index] += hashArray[index - 1];
                    } else {
                        //goofy way to make it edit the first element to prevent some issues
                        hashArray[index] += hashArray[i+j] + oneSum;
                    }
                }
            }
        }

        std::string output;
        for (const int i : hashArray) {
            output += "0123456789abcdef"[abs(i) % 16];
        }

        return output;
    }
};



#endif //MATRIXHASH_H

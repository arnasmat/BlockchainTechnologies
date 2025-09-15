//
// Created by arnas on 9/14/25.
//

#ifndef MATRIXHASH_H
#define MATRIXHASH_H
#include "HashGenInterface.h"
#include "HumanHash.h"


class MatrixHash: public HashGenInterface {
public:
    std::string generateHash(const std::string &input, int* hashed=nullptr) const override {
        const int arrayDimensions = input.size() * 8;
        int tempArray[arrayDimensions]{};
        int* hash = new int[64]();
        int oneSum{0};

        for (int i=0; i<input.size(); i++) {
            unsigned char inputChar = input[i];

            for (int j=0; j<8; j++) {
                //https://stackoverflow.com/a/13823765
                // instead of bitset<8> and casting to string which caused a lot of our issues lol
                const bool bit = (inputChar >> j) & 1;
                const int index = (i * 8 + j)%64;

                if (bit == 1) {
                    hash[index] += 1;
                    oneSum++;
                } else if (index > 0) {
                    hash[index] += hash[index - 1];
                } else {
                    hash[0] += hash[input.size() % 64] + oneSum;
                }

                hash[index] += (i + j) * bit;
            }
        }

       HumanHash finalHash;
       return finalHash.generateHash(input, hash);

    }
};



#endif //MATRIXHASH_H

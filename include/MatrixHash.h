//
// Created by arnas on 9/14/25.
//

#ifndef MATRIXHASH_H
#define MATRIXHASH_H
#include "HashGenInterface.h"


class MatrixHash: public HashGenInterface {
public:
    std::string generateHash(const std::string &input) const override {
        int arrayDimensions = input.size() * 8;
        int tempArray[arrayDimensions]{};

        for (int i=0; i<input.size(); i++) {
            std::string bin = std::bitset<8>(static_cast<unsigned char>(input[i])).to_string();
            for (int j=0; j<8; j++) {
                tempArray[i*8 + j] = (bin[j] == '1') ? 1 : 0;
            }
        }

        int hashArray[64]{};

        for (int i = 0; i < 64; i++) {
            for (int j = 0; j < arrayDimensions; j++) {
                hashArray[i] += (i + j) * tempArray[j];
            }
        }


        std::string output;
        for (const int i : hashArray) {
            output += "0123456789abcdef"[i % 16];
        }

        return output;
    }
};



#endif //MATRIXHASH_H

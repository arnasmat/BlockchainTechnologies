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
            std::string bin = std::bitset<8>(static_cast<unsigned char>(input[i])).to_string();
            for (int j=0; j<8; j++) {
                if ((bin[j] == '1')) {
                    tempArray[i * 8 + j] = 1;
                    oneSum ++;
                    hashArray[(i * 8 + j)%64] = oneSum;
                }
                else {
                    tempArray[i * 8 + j] = 0;
                    hashArray[(i * 8 + j)%64] = oneSum % 3;
                }
            }
        }


        for (int i = 0; i < 64; i++) {
            for (int j = 0; j < arrayDimensions; j++) {
                hashArray[i] += (i + j) * tempArray[j];
            }
        }

        for (const int c : hashArray) {
            std::string bin = std::bitset<8>(static_cast<unsigned char>(c)).to_string();

            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    const int index = 8 * j + i;

                    if (bin[i] == '1') {
                        hashArray[index] += 1;
                    } else if (index > 0) {
                        hashArray[index] += hashArray[index - 1];
                    } else {
                        //goofy way to make it edit the first element to prevent some issues
                        hashArray[index] += hashArray[i+j];
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

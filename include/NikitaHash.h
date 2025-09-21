//
// Created by arnas on 9/21/25.
//

#ifndef NIKITAHASH_H
#define NIKITAHASH_H
#include <random>
#include <vector>

#include "HashGenInterface.h"

// CODE BY NIKITA. NOT OURS. IS HERE FOR COMPARISONS

class NikitaHash: public HashGenInterface {
    private:

    std::string HashFunction(const std::string &input) const {
        constexpr int HASH_LENGTH = 64;
        std::vector<int> integerVector;
        integerVector.reserve(input.length()*2);
        long int uniqueStringNumber = 1;
        int counter = 0;
        for (char c : input) {
            int castedChar = static_cast<int>(c);
            if (castedChar < 0) castedChar*= -1;
            integerVector.push_back(castedChar);
            if (counter % 4 == 0) uniqueStringNumber = std::abs(uniqueStringNumber) - 255;
            if (counter % 3 == 0) uniqueStringNumber = (uniqueStringNumber+1) * -1;
            if (counter % 2 == 0) uniqueStringNumber+=castedChar+1;
            else uniqueStringNumber-=castedChar;
            counter++;
        }
        std::mt19937 engine(uniqueStringNumber+3);
        std::uniform_int_distribution<int> indexDistribution(0, HASH_LENGTH - 1);
        std::uniform_int_distribution<int> randomDistribution(0, 15);
        int hash[HASH_LENGTH];
        for (int i = 0; i < HASH_LENGTH; i++) {
            hash[i]=0;
        }
        for (auto el : integerVector) {
            hash[indexDistribution(engine)]+= el;
        }
        for (int i = 0; i < HASH_LENGTH; i++) {
            hash[i]+= randomDistribution(engine);
            hash[i]=hash[i]%16;
        }
        // convert to string
        std::ostringstream oss;
        for (int i = 0; i < HASH_LENGTH; i++) {
            oss << std::hex << hash[i];
        }
        std::string hexstr = oss.str();
        return hexstr;
    }

public:

    std::string generateHash(const std::string &input) const override {
        return HashFunction(input);
    }
};

#endif //NIKITAHASH_H

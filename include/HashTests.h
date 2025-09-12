#ifndef HASHTESTS_H
#define HASHTESTS_H

#include <string>
#include <algorithm>
#include <random>
#include "HashGenInterface.h"

#include <bitset>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace HashTests {
    void determinismTest(const HashGenInterface* hashGen, const std::string& input);
    void collisionSearch(const HashGenInterface* hashGen);
    std::string generateRandomString(const size_t length, const std::string& validSymbols, std::mt19937& rng);
    double calculateSimilarityPercentageBit(std::string hash1, std::string hash2);
    double calculateSimilarityPercentageHex(std::string hash1, std::string hash2);
    void avalancheEffect(const HashGenInterface *hashGen);
    double calculateSimilarityPercentage(std::string hash1, std::string hash2);
}

#endif // HASHTESTS_H

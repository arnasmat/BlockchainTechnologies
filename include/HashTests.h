#ifndef HASHTESTS_H
#define HASHTESTS_H

#include "HashGenInterface.h"
#include <string>
#include <algorithm>
#include <random>
#include <bitset>
#include <iomanip>
#include <set>
#include <filesystem>
#include <iostream>
#include <sstream>

namespace std::filesystem::__cxx11 {
    class path;
}

namespace HashTests {
    void runAllTests(const HashGenInterface* hashGen);

    void determinismTest(const HashGenInterface* hashGen, const std::string& input);
    void collisionSearchPairs(const HashGenInterface* hashGen);
    void collisionSearchSets(const HashGenInterface *hashGen);
    void outputSizeTest(const HashGenInterface* hashgen);
    void efficiencyTest(const HashGenInterface* hashGen, const std::filesystem::__cxx11::path &inputFile);
    double effTestHelper(const HashGenInterface* hashGen, const std::string& input);
    std::string generateRandomString(const size_t length, const std::string& validSymbols, std::mt19937& rng);
    double calculateSimilarityPercentageBit(std::string hash1, std::string hash2);
    double calculateSimilarityPercentageHex(std::string hash1, std::string hash2);
    void avalancheEffect(const HashGenInterface *hashGen);
    double calculateSimilarityPercentage(std::string hash1, std::string hash2);
}

#endif // HASHTESTS_H

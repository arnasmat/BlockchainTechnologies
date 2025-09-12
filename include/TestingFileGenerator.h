//
// Created by arnas on 9/12/25.
//

#ifndef TESTINGFILEGENERATOR_H
#define TESTINGFILEGENERATOR_H

#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>
#include <cmath>
#include <random>

namespace TestingFileGenerator {
    void ensureTestFolders(const std::filesystem::path &dirPath);
    void openUniqueFile(const std::filesystem::path &testFileNameNoTxt, std::ofstream &out);
    char getRandomSymbol(const std::string &validSymbols, std::mt19937 &rng);
    std::filesystem::path manyRandomSymbolsFileGen(std::mt19937 &rng, const std::filesystem::path &testDir,
                                                          const std::string &validSymbols);

    void modifyRandomSymbol(std::mt19937 &rng, const std::filesystem::path &testFileNameNoTxt, const std::string &validSymbols);

    void oneSymbolFiles(const std::string& symbols = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    void manyRandomSymbolsFiles(const std::string& validSymbols = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$%^&*()_+-=[]{}|;:',.<>/?`~\"\\");
    void similarSymbolsFiles(const std::string& validSymbols = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$%^&*()_+-=[]{}|;:',.<>/?`~\"\\");

    void emptyFile();
};




#endif //TESTINGFILEGENERATOR_H

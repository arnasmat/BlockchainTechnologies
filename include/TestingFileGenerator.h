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

class TestingFileGenerator {
private:
    static void ensureTestFolders(const std::filesystem::path &dirPath);
    static void openUniqueFile(const std::filesystem::path &testFileNameNoTxt, std::ofstream &out);
    static char getRandomSymbol(const std::string &validSymbols, std::mt19937 &rng);
    static std::filesystem::path manyRandomSymbolsFileGen(std::mt19937 &rng, const std::filesystem::path &testDir,
                                                          const std::string &validSymbols);

    static void modifyRandomSymbol(std::mt19937 &rng, const std::filesystem::path &testFileNameNoTxt, const std::string &validSymbols);


public:
    static void oneSymbolFiles(const std::string& symbols = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    static void manyRandomSymbolsFiles(const std::string& validSymbols = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$%^&*()_+-=[]{}|;:',.<>/?`~\"\\");
    static void similarSymbolsFiles(const std::string& validSymbols = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$%^&*()_+-=[]{}|;:',.<>/?`~\"\\");

    static void emptyFile();
};




#endif //TESTINGFILEGENERATOR_H

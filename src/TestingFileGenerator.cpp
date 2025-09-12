//
// Created by arnas on 9/12/25.
//

#include "../include/TestingFileGenerator.h"

#include <random>
// todo: better naming lol

// File names shortened to OSF - One Symbol Files
void TestingFileGenerator::oneSymbolFiles(const std::string &symbols) {
    std::filesystem::path testDir("../data/input/test/OSF/");
    ensureTestFolders(testDir);
    for (char c: symbols) {
        std::ofstream out(testDir.string() + std::string(1, c) + ".txt");
        out << c;
    }
}

// File names shortened to MRS - Many Random Symbols
void TestingFileGenerator::manyRandomSymbolsFiles() {
    std::filesystem::path testDir("../data/input/test/MRS/");
    ensureTestFolders(testDir);
    std::string validSymbols =
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$%^&*()_+-=[]{}|;:',.<>/?`~\"\\";

    static std::mt19937 rng(std::random_device{}());

    for (int i = 0; i < 10; i++) {
        manyRandomSymbolsFileGen(rng, testDir, validSymbols);
    }
}

void TestingFileGenerator::manyRandomSymbolsFileGen(std::mt19937& rng, const std::filesystem::path& testDir, const std::string& validSymbols) {
    std::string outputContent;
    int fileLenght = std::uniform_int_distribution<int>(1000, 10000)(rng);
    for (int i = 0; i < fileLenght; i++) {
        outputContent += getRandomSymbol(validSymbols, rng);
    }

    std::filesystem::path testFileNameNoTxt = testDir.string() + std::to_string(fileLenght) + "_sz";
    std::ofstream out;
    openUniqueFile(testFileNameNoTxt, out);
    out << outputContent;
}

void TestingFileGenerator::ensureTestFolders(const std::filesystem::path &dirPath) {
    if (!std::filesystem::exists(dirPath)) {
        std::filesystem::create_directories(dirPath);
    }
}



void TestingFileGenerator::openUniqueFile(const std::filesystem::path& testFileNameNoTxt, std::ofstream& out) {
    if (!std::filesystem::exists(testFileNameNoTxt.string() + ".txt")) {
        out.open(testFileNameNoTxt.string() + ".txt");
    } else {
        int i = 2;
        while (std::filesystem::exists(testFileNameNoTxt.string() + std::to_string(i) + ".txt")) {
            i++;
        }
        out.open(testFileNameNoTxt.string() + std::to_string(i) + ".txt");
    }
}

char TestingFileGenerator::getRandomSymbol(const std::string &validSymbols, std::mt19937 &rng) {
    std::uniform_int_distribution<size_t> dist(0, validSymbols.size() - 1);
    return validSymbols[dist(rng)];
}

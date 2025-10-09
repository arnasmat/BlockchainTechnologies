//
// Created by arnas on 9/16/25.
//

#ifndef CLIARGHANDLER_H
#define CLIARGHANDLER_H

#include <string>
#include <iostream>
#include "HashTests.h"
#include "../HashAlg/HumanHash.h"
#include "../HashAlg/MatrixHash.h"
#include "../HashAlg/VibeHash.h"

enum HashAlgorithm { HUMAN, VIBE, MATRIX };

struct ArgsToRun {
    bool help = false;
    bool tests = false;
    std::vector<std::filesystem::path> inputFilePath{};
    std::filesystem::path outputFilePath = "";
    HashAlgorithm hashAlgorithm = MATRIX;
};

ArgsToRun handleCliInput(int argc, char *argv[]);
void handleCliArgs(const ArgsToRun& argsToRun);
void printHelpInfo();
void runTestsWithAll();
std::string handleFileInput(const HashAlgorithm hashAlgorithm, const std::filesystem::path& inputFilePath);
void handleFileOutput(const std::string& output, const std::filesystem::path& outputFilePath);

#endif //CLIARGHANDLER_H

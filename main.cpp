#include <iostream>
#include <string>
#include <bitset>
#include <sstream>

#include "HashTests.h"
#include "HumanHash.h"
#include "TestingFileGenerator.h"

int main(int argc, char *argv[]) {
    // Args to make:
    // galbut su cxxopts, idk kaip handlint args normaliai - https://www.v0rkath.com/blog/cxxopts-guide/
    // -f <file> : input file
    // -o <file> : output file (default is stdout)
    // --help    : help
    // -h      : our (human) crytpo hashing algorithm version (default)
    // -v     : vibe coded (ai) crypto hashing algorithm

    TestingFileGenerator::generateAllFiles();
    HumanHash hash;
    HashTests::runAllTests(&hash);
}

#include <iostream>
#include <string>
#include <bitset>
#include <sstream>

#include "CliArgHandler.h"
#include "HashTests.h"
#include "HumanHash.h"
#include "MatrixHash.h"
#include "TestingFileGenerator.h"
#include "VibeHash.h"
#include <CliArgHandler.h>

// TODO: name which file is being opened in the eff test
int main(int argc, char *argv[]) {
    ArgsToRun argsToRun = handleCliInput(argc, argv);

    handleCliArgs(argsToRun);
}

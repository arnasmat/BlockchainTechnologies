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

#include "other-hash/MonikaHash.h"

int main(int argc, char *argv[]) {
    ArgsToRun argsToRun = handleCliInput(argc, argv);

    handleCliArgs(argsToRun);
}

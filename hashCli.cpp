#include "include/HashAlg/CLI/CliArgHandler.h"

int main(int argc, char *argv[]) {
    ArgsToRun argsToRun = handleCliInput(argc, argv);

    handleCliArgs(argsToRun);
}

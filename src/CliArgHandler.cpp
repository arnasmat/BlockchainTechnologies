//
// Created by arnas on 9/16/25.
//

#include <CliArgHandler.h>

//TODO: improve ts, currently the input file path thing is dogshit
// also error handling ig
// TODO: allow for multiple algorithms to input
ArgsToRun handleCliInput(int argc, char *argv[]) {
    ArgsToRun args;

    for (int i = 1; i < argc; i++) {
        std::string arg{argv[i]};
        // why can't we use switches in c++!!!

        if (arg == "-t" || arg == "--tests") {
            args.tests = true;
        } else if (arg == "-h" || arg == "--help") {
            args.help = true;
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                args.outputFilePath = argv[++i];
            } else {
                std::cerr << "Error: Missing output file path after " << arg << std::endl;
                args.help = true;
            }
        } else if (arg == "-i" || arg == "--input") {
            if (i + 1 < argc) {
                args.inputFilePath = argv[++i];
            } else {
                std::cerr << "Error: Missing input file path after " << arg << std::endl;
                args.help = true;
            }
        } else if (arg == "-v" || arg == "--vibe") {
            args.hashAlgorithm = VIBE;
        } else if (arg == "-m" || arg == "--matrix") {
            args.hashAlgorithm = MATRIX;
        } else if (arg == "-u" || arg == "--human") {
            args.hashAlgorithm = HUMAN;
        } else {
            args.inputFilePath = arg;
        }
    }

    return args;
}

void handleCliArgs(const ArgsToRun &argsToRun) {
    if (argsToRun.help) {
        // If we run help - all other args are ignored
        printHelpInfo();
    } else if (argsToRun.tests) {
        runTestsWithAll();
    } else {
        std::string output = handleFileInput(argsToRun.hashAlgorithm, argsToRun.inputFilePath);

        handleFileOutput(output, argsToRun.outputFilePath);
    }
}

void printHelpInfo() {
    std::ostringstream ss;
    ss << "Usage: hash_program [options] <input_file>\n"
            << "Options:\n"
            << "  -h, --help            Show this help message and exit\n"
            << "  -t, --tests           Run all tests (ignores all other input flags)\n"
            << "  -o, --output <file>   Specify output file (default is stdout)\n"
            << "  -i, --input <file>    Specify input file (doesn't do anything lol)\n"
            << "  -m, --matrix         Use MatrixHash algorithm (Default)\n"
            << "  -u, --human          Use HumanHash algorithm\n"
            << "  -v, --vibe           Use VibeHash algorithm\n";

    std::cout << ss.str();
}

void runTestsWithAll() {
    // TODO: let ppl choose which tests to run
    // TODO: let ppl choose which algorithms to test
    // If we run tests - all other inputs are ignored)
    HumanHash humanHash;
    MatrixHash matrixHash;
    VibeHash vibeHash;

    std::cout<<"Running all tests with MatrixHash: \n";
    HashTests::runAllTests(&matrixHash);
    std::cout<<"\n\nRunning all tests with HumanHash: \n";
    HashTests::runAllTests(&humanHash);
    std::cout<<"\n\nRunning all tests with VibeHash: \n";
    HashTests::runAllTests(&vibeHash);
}

std::string handleFileInput(const HashAlgorithm hashAlgorithm, const std::filesystem::path &inputFilePath) {
    // TODO: allow input from stdin
    // TODO: make output to file work
    // TODO: allow for multiple hash algs?
    if (inputFilePath.empty()) {
        std::cerr << "Error: no input file path provided. Run app with -h for more information\n";
        return "";
    }

    if (hashAlgorithm == HUMAN) {
        HumanHash humanHash;
        return humanHash.hashFromFile(inputFilePath);
    }
    if (hashAlgorithm == VIBE) {
        VibeHash vibeHash;
        return vibeHash.hashFromFile(inputFilePath);
    }
    if (hashAlgorithm == MATRIX) {
        MatrixHash matrixHash;
        return matrixHash.hashFromFile(inputFilePath);
    }
    return ""; // should never reach here as HashAlgorithm is enum, but ok compiler don't be mad
}

void handleFileOutput(std::string output, std::filesystem::path outputFilePath) {
    if (outputFilePath.empty()) {
        std::cout << output;
    } else {
        std::ofstream outputFile(outputFilePath);
        if (!outputFile) {
            std::cerr << "Error: Could not open output file: " << outputFilePath << std::endl;
        } else {
            outputFile << output;
            outputFile.close();
        }
    }
}

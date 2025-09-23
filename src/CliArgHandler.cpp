//
// Created by arnas on 9/16/25.
//

#include <CliArgHandler.h>

#include "OtherAlgorithms.h"

// also error handling ig
// TODO: allow for multiple algorithms to input
ArgsToRun handleCliInput(int argc, char *argv[]) {
    ArgsToRun args;

    try {
        for (int i = 1; i < argc; i++) {
            std::string arg{argv[i]};
            // why can't we use switches with strings in c++!!!

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
                    args.inputFilePath.push_back(argv[++i]);
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
                args.inputFilePath.push_back(arg);
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error parsing command line arguments: " << e.what() << std::endl;
        args.help = true;
    }

    return args;
}

void handleCliArgs(const ArgsToRun &argsToRun) {
    try {
        if (argsToRun.help) {
            // If we run help - all other args are ignored
            printHelpInfo();
        } else if (argsToRun.tests) {
            runTestsWithAll();
        } else {
            std::string output{};
            for (auto &path: argsToRun.inputFilePath) {
                output += handleFileInput(argsToRun.hashAlgorithm, path);
                output += '\n';
            }
            handleFileOutput(output, argsToRun.outputFilePath);
        }
    } catch (const std::exception &e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
}

void printHelpInfo() {
    std::ostringstream ss;
    ss << "Usage: hash_program [options] <input_file>\n"
            <<"(If you input more than one file, all of them will be used. The output will be seen in the specified output file)\n"
            <<"You can also use input from stdin. I.e. echo -n 'test' | ./BlockchainTechnologies OR run ./BlockchainTechnologies, input some data into stdin and click ctrl D\n\n"
            << "Options:\n"
            << "  -h, --help            Show this help message and exit\n"
            << "  -t, --tests           Run all tests (ignores all other input flags)\n"
            << "  -o, --output <file>   Specify output file (default is stdout)\n"
            << "  -i, --input <file>    Specify input file (doesn't do anything lol)\n"
            << "  -m, --matrix         Use MatrixHash algorithm (Default)\n"
            << "  -u, --human          Use HumanHash algorithm\n"
            << "  -v, --vibe           Use VibeHash algorithm\n";
// TODO: maybe make -i be the input key thing
    // TODO: move help info into another, easier editable file lol

    std::cout << ss.str();
}

void runTestsWithAll() {
    // TODO: let ppl choose which tests to run
    // TODO: let ppl choose which algorithms to test
    // note - If we run tests - all other inputs are ignored)
    HumanHash humanHash;
    MatrixHash matrixHash;
    VibeHash vibeHash;

    std::cout<<"Running all tests with MatrixHash: \n";
    HashTests::runAllTests(&matrixHash);
    std::cout<<"\n\nRunning all tests with HumanHash: \n";
    HashTests::runAllTests(&humanHash);
    std::cout<<"\n\nRunning all tests with VibeHash: \n";
    HashTests::runAllTests(&vibeHash);

    // Other algs - Comment out from here until the end of this function if you don't want to use OpenSSL
    Sha256 sha256;
    Md5 md5;

    std::cout<<"\n\nRUNNING TESTS WITH OTHER ALGORITHMS\n(Algorithm implementations by OpenSSL)\n";
    std::cout<<"\n\nRunning all tests with SHA256: \n";
    HashTests::runAllTests(&sha256);
    std::cout<<"\n\nRunning all tests with MD5: \n";
    HashTests::runAllTests(&md5);
}

std::string handleFileInput(const HashAlgorithm hashAlgorithm, const std::filesystem::path &inputFilePath) {
    // TODO: allow input from stdin
    // TODO: make output to file work
    // TODO: allow for multiple hash algs?
    try {
        std::string inputData{};
        if (inputFilePath.empty() ) {
            if (std::cin.peek() == EOF) {
                std::cerr<<"Error: No input (file) provided. Run app with -h for more information\n";
                return "";
            }

            std::ostringstream buffer;
            buffer << std::cin.rdbuf();
            inputData = buffer.str();
            if (hashAlgorithm == HUMAN) {
                HumanHash humanHash;
                return humanHash.generateHash(inputData);
            }
            if (hashAlgorithm == VIBE) {
                VibeHash vibeHash;
                return vibeHash.generateHash(inputData);
            }
            if (hashAlgorithm == MATRIX) {
                MatrixHash matrixHash;
                return matrixHash.generateHash(inputData);
            }
        }

        if (!std::filesystem::exists(inputFilePath)) {
            std::cerr << "Error: file does not exist. Run app with -h for more information\n";
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
    } catch (std::exception &e) {
        std::cerr << "An error occurred while handling file input: " << e.what() << std::endl;
    }
    return ""; // should never reach here as HashAlgorithm is enum, but ok compiler don't be mad
}

void handleFileOutput(const std::string& output, const std::filesystem::path& outputFilePath) {
    try {
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
    } catch (std::exception &e) {
        std::cerr<< "An error occurred while handling file output: " << e.what() << std::endl;
    }
}

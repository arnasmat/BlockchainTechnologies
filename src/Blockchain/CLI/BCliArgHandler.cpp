#include "Blockchain/CLI/BCliArgHandler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "RandomGenerator.h"
#include "Blockchain/User.h"
#include "Blockchain/Transaction.h"
#include "Blockchain/MiningSimulator.h"
#include "Blockchain/Blockchain.h"

BArgsToRun handleBCliInput(int argc, char *argv[]) {
    BArgsToRun args;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        // TODO: Make it so providing args like -u or -b or whatever WITH -r adds that amount of users to the blockchain
        if (arg == "-h" || arg == "--help") {
            args.help = true;
            return args;
        }
        if ((arg == "-r" || arg == "--read") && i + 1 < argc) {
            args.inputFolderPath = std::filesystem::path(argv[++i]);
            args.outputFolderPath = args.inputFolderPath;
            return args;
        }
        if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
            args.outputFolderPath = std::filesystem::path(argv[++i]);
        } else if ((arg == "-u" || arg == "--users") && i + 1 < argc) {
            args.numberOfUsers = std::stoi(argv[++i]);
        } else if ((arg == "-b" || arg == "--blocks") && i + 1 < argc) {
            args.numberOfBlocks = std::stoi(argv[++i]);
        } else if ((arg == "-t" || arg == "--transactions") && i + 1 < argc) {
            args.numberOfTransactions = std::stoi(argv[++i]);
        }
    }

    return args;
}

void handleBCliArgs(const BArgsToRun &argsToRun) {
    if (argsToRun.help) {
        printBHelpInfo();
        return;
    }

    if (!argsToRun.inputFolderPath.empty()) {
        handleFileInput(argsToRun);
    } else {
        handleFileGeneration(argsToRun);
    }
}

void printBHelpInfo() {
    std::cout << "Blockchain CLI - Usage:\n\n";
    std::cout << "  -h, --help                    Display this help message\n";
    std::cout <<
            "  -r, --read <input_folder>     Read input from folder (requires " << USERS_FILE << " " <<
            TRANSACTIONS_FILE << " and " << BLOCKS_DIRECTORY << ")\n";
    std::cout << "  -o, --output <output_folder>  Output folder (default: " << DEFAULT_OUTPUT_DIR << ")\n";
    std::cout << "  -u, --users <number>          Number of users to generate (default: 1000)\n";
    std::cout << "  -b, --blocks <number>         Number of blocks to mine (default: 0 = mine until stopped)\n";
    std::cout << "  -t, --transactions <number>   Number of transactions to generate (default: 100000)\n\n";
    // TODO: Add example usage
}

void handleFileInput(const BArgsToRun &argsToRun) {
    if (!doBlockchainFilesExist(argsToRun.inputFolderPath)) {
        std::cerr << "Error: Input folder must contain " << USERS_FILE << " " << TRANSACTIONS_FILE << " and " <<
                BLOCKS_DIRECTORY << "\n";
        return;
    }

    std::cout << "Reading blockchain data from " << argsToRun.inputFolderPath << "...\n";
    // TODO: Implement file reading logic based on your file format
}

bool doBlockchainFilesExist(const std::filesystem::path &folderPath) {
    std::filesystem::path usersFile = folderPath / USERS_FILE;
    std::filesystem::path txFile = folderPath / TRANSACTIONS_FILE;
    std::filesystem::path blocksDir = folderPath / BLOCKS_DIRECTORY;

    return std::filesystem::exists(usersFile) &&
           std::filesystem::exists(txFile) &&
           std::filesystem::exists(blocksDir);
}

void handleFileGeneration(const BArgsToRun &argsToRun) {
    if (!std::filesystem::exists(argsToRun.outputFolderPath)) {
        std::filesystem::create_directories(argsToRun.outputFolderPath);
        // TODO: add handling to continue already existing folder instead of overwriting if not specified to override?
    }

    std::filesystem::path blocksDir = argsToRun.outputFolderPath / BLOCKS_DIRECTORY;
    if (!std::filesystem::exists(blocksDir)) {
        std::filesystem::create_directories(blocksDir);
    }

    generateBlockchainMetadata(argsToRun.outputFolderPath);
    // TODO: think of how to fix this. idk how I should do it. currently it saves users twice. Once w/o utxos
    // and once with. idk bro lmao
    std::vector<User *> users = generateAndSaveUsersToFile(argsToRun.numberOfUsers, argsToRun.outputFolderPath);

    std::cout << "Starting mining simulation...\n";
    MiningSimulator mineSim(users);
    Block *previousBlock = mineSim.getGenesisBlock();

    unsigned int blocksMined = 0;

    const bool mineIndefinitely = (argsToRun.numberOfBlocks == 0);
    const unsigned int blocksBeforeTransactions = mineIndefinitely
                                                      ? BLOCKS_BEFORE_TRANSACTIONS
                                                      : argsToRun.numberOfBlocks;

    std::cout << "Mining " << blocksBeforeTransactions << " blocks to build UTXO pool...\n";
    while (blocksMined < blocksBeforeTransactions) {
        std::vector<Transaction *> emptyTransactions;
        previousBlock = mineSim.mineBlockParallel(emptyTransactions, previousBlock);
        blocksMined++;

        saveBlockToFile(blocksDir, previousBlock, blocksMined);
    }

    generateAndSaveMempool(argsToRun.numberOfTransactions, users, argsToRun.outputFolderPath);

    generateAndSaveUsersToFile(argsToRun.numberOfUsers, argsToRun.outputFolderPath);

    std::cout << "Initial output saved to " << argsToRun.outputFolderPath << "\n";

    // TODO: Make this start mining the amount of blocks the user asked for
}

void saveBlockToFile(const std::filesystem::path &blocksDir, Block *previousBlock, unsigned int &blocksMined) {
    std::string fileName = std::to_string(blocksMined) + BLOCKS_FILE;
    std::filesystem::path blockFile = blocksDir / fileName;
    std::ofstream outFile(blockFile);
    if (outFile.is_open()) {
        // npote: no space between the hashes since they're always a specified length (in general.h)
        outFile << previousBlock->getBlockHash() << previousBlock->getMinerPublicKey() << " "
                << previousBlock->getHeight() << " "
                << previousBlock->getTimestamp() << " "
                << previousBlock->getDifficultyTarget() << " "
                << previousBlock->getTransactions().size() << "\n";

        for (const auto *tx: previousBlock->getTransactions()) {
            // no space, same reason as before, since it uses the same hash alg for trans hash and sender pk hash
            outFile << tx->getTransactionId() << tx->getSenderPublicKey() << "\n";
            for (const auto &output: tx->getOutputs()) {
                outFile << output.second << " " << output.first << "\n";
            }
            outFile << tx->getTransactionTime() << "\n";
        }

        outFile.close();
    }
}

void generateBlockchainMetadata(std::filesystem::path outputDir) {
    std::filesystem::path metaFile = outputDir / METADATA_FILE;
    std::ofstream out(metaFile);
    if (!out.is_open()) {
        std::cerr << "Failed to open metadata file " << metaFile << "\n";
        return;
    }

    // idk theoretically order shouldn't change in metadata, but prolly bad to do it like this. oh well lmao
    out << SYSTEM_NAME << "\n";
    out << SYSTEM_VERSION << "\n";
    out << hashAlgorithmToString(HASH_TYPE) << "\n";
    out << HASH_LENGTH << "\n";
    out << DEFAULT_DIFFICULTY << "\n";
    out << DIFFICULTY_TARGET_INCREASE_INTERVAL << "\n";
    out << TARGET_BLOCK_TIME << "\n";
    out << TARGET_TOLERANCE << "\n";
    out << INITIAL_REWARD << "\n";
    out << HEIGHT_FOR_HALVING_REWARD << "\n";
    out << BLOCKS_BEFORE_TRANSACTIONS << "\n";

    out.close();
    std::cout << "Metadata saved to " << metaFile << "\n";
}

void generateAndSaveMempool(const unsigned int numberOfTransactions, const std::vector<User *> &users,
                            const std::filesystem::path &outputFolderPath) {
    std::cout << "\nGenerating " << numberOfTransactions << " transactions...\n";
    std::vector<Transaction *> transactions = blockchainRandomGenerator::generateValidTransactions(
        users, numberOfTransactions
    );
    // TODO: FIX THIS WITH NEWER MEMPOOL SYSTEM.

    std::filesystem::path txFile = outputFolderPath / TRANSACTIONS_FILE;
    std::ofstream txOutFile(txFile);
    if (txOutFile.is_open()) {
        for (const auto *tx: transactions) {
            txOutFile << tx->getTransactionId() << " " << tx->getSenderPublicKey() << "\n";
            for (const auto &output: tx->getOutputs()) {
                txOutFile << output.second << " " << output.first << "\n";
            }
            txOutFile << tx->getTransactionTime() << "\n";
        }
        txOutFile.close();
    }
}

std::vector<User *> generateAndSaveUsersToFile(const unsigned int numberOfUsers, std::filesystem::path outputFolder) {
    std::cout << "Saving " << numberOfUsers << " users...\n";
    std::vector<User *> users = blockchainRandomGenerator::generateUsers(numberOfUsers);

    std::filesystem::path usersFile = outputFolder / USERS_FILE;
    std::ofstream usersOutFile(usersFile);
    if (usersOutFile.is_open()) {
        for (const auto *user: users) {
            usersOutFile << user->getPublicKey() << "\n";
            auto utxos = UtxoSystem::getInstance().getUtxosForUser(user->getPublicKey());
            for (const auto *utxo: utxos) {
                // TODO: Fix this to new utxo system
                usersOutFile << utxo->getTransaction()->getTransactionId() << " "
                        << utxo->getVout() << " "
                        << utxo->getAmount() << "\n";
            }
            usersOutFile << "\n";
        }
        usersOutFile.close();
        std::cout << "Users saved to " << usersFile << "\n";
    } else {
        std::cerr << "Unable to open file oopsie daisy!\n";
    }
    return users;
}




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
    std::cout << "Examples:\n";
    std::cout << "  ./blockchain -u 500 -t 50000 -b 10\n";
    std::cout << "  ./blockchain -r ./input_data -o ./results\n";
}

void handleFileInput(const BArgsToRun &argsToRun) {
    std::filesystem::path usersFile = argsToRun.inputFolderPath / USERS_FILE;
    std::filesystem::path txFile = argsToRun.inputFolderPath / TRANSACTIONS_FILE;
    std::filesystem::path blocksDir = argsToRun.inputFolderPath / BLOCKS_DIRECTORY;

    if (!std::filesystem::exists(usersFile) ||
        !std::filesystem::exists(txFile) ||
        !std::filesystem::exists(blocksDir)) {
        std::cerr << "Error: Input folder must contain " << USERS_FILE << " " << TRANSACTIONS_FILE << " and " <<
                BLOCKS_DIRECTORY << "\n";
        return;
    }

    std::cout << "Reading blockchain data from " << argsToRun.inputFolderPath << "...\n";
    // TODO: Implement file reading logic based on your file format
}

void handleFileGeneration(const BArgsToRun &argsToRun) {
    if (!std::filesystem::exists(argsToRun.outputFolderPath)) {
        std::filesystem::create_directories(argsToRun.outputFolderPath);
    }

    std::filesystem::path blocksDir = argsToRun.outputFolderPath / BLOCKS_DIRECTORY;
    if (!std::filesystem::exists(blocksDir)) {
        std::filesystem::create_directories(blocksDir);
    }
    // TODO: fix so it doesnt do it twice? idk how I should do it. currently it saves users twice. Once w/o utxos
    // and once with. idk bro lmao
    std::vector<User *> users = generateAndSaveUsersToFile(argsToRun.numberOfUsers, argsToRun.outputFolderPath);

    std::cout << "Starting mining simulation...\n";
    MiningSimulator mineSim(users);
    Block *previousBlock = mineSim.getGenesisBlock();

    unsigned int blocksMined = 0;

    bool mineIndefinitely = (argsToRun.numberOfBlocks == 0);
    unsigned int blocksBeforeTransactions = mineIndefinitely ? BLOCKS_BEFORE_TRANSACTIONS : argsToRun.numberOfBlocks;

    std::cout << "Mining " << blocksBeforeTransactions << " blocks to build UTXO pool...\n";
    while (blocksMined < blocksBeforeTransactions) {
        std::vector<Transaction *> emptyTransactions;
        previousBlock = mineSim.mineBlockParallel(emptyTransactions, previousBlock);
        blocksMined++;

        // Save block in compact format
        std::filesystem::path blockFile = blocksDir / (std::to_string(blocksMined) + BLOCKS_FILE);
        std::ofstream outFile(blockFile);
        if (outFile.is_open()) {
            // Header line: Hash Miner Height Timestamp Difficulty TxCount
            outFile << previousBlock->getBlockHash() << previousBlock->getMinerPublicKey() << " "
                    << previousBlock->getHeight() << " "
                    << previousBlock->getTimestamp() << " "
                    << previousBlock->getDifficultyTarget() << " "
                    << previousBlock->getTransactions().size() << "\n";

            // Transaction lines
            for (const auto *tx: previousBlock->getTransactions()) {
                outFile << tx->getTransactionId() << " " << tx->getSenderPublicKey() << "\n";
                for (const auto &output: tx->getOutputs()) {
                    outFile << output.second << " " << output.first << "\n";
                }
                outFile << tx->getTransactionTime() << "\n";
            }

            outFile.close();
        }
    }

    // Phase 2: Generate and save transactions
    std::cout << "\nGenerating " << argsToRun.numberOfTransactions << " transactions...\n";
    std::vector<Transaction *> transactions = blockchainRandomGenerator::generateValidTransactions(
        users, argsToRun.numberOfTransactions
    );

    std::filesystem::path txFile = argsToRun.outputFolderPath / TRANSACTIONS_FILE;
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

    generateAndSaveUsersToFile(argsToRun.numberOfUsers, argsToRun.outputFolderPath);

    std::cout << "\nMining complete. Mined " << blocksMined << " blocks.\n";
    std::cout << "Generated " << transactions.size() << " transactions.\n";
    std::cout << "Output saved to " << argsToRun.outputFolderPath << "\n";
}

std::vector<User *> generateAndSaveUsersToFile(const unsigned int numberOfUsers, std::filesystem::path outputFolder) {
    std::cout << "Generating " << numberOfUsers << " users...\n";
    std::vector<User *> users = blockchainRandomGenerator::generateUsers(numberOfUsers);

    std::filesystem::path usersFile = outputFolder / USERS_FILE;
    std::ofstream usersOutFile(usersFile);
    if (usersOutFile.is_open()) {
        for (const auto *user: users) {
            usersOutFile << user->getPublicKey() << "\n";
            auto utxos = UtxoSystem::getInstance().getUtxosForUser(user->getPublicKey());
            for (const auto *utxo: utxos) {
                usersOutFile << utxo->getTransaction()->getTransactionId() << " "
                        << utxo->getVout() << " "
                        << utxo->getAmount() << "\n";
            }
        }
        usersOutFile << "\n";
        usersOutFile.close();
        std::cout << "Users saved to " << usersFile << "\n";
    } else {
        std::cerr << "Unable to open file oopsie daisy!\n";
    }
    return users;
}




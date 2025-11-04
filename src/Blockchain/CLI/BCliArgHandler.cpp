#include "Blockchain/CLI/BCliArgHandler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "RandomGenerator.h"
#include "Blockchain/User.h"
#include "Blockchain/Transaction.h"
#include "Blockchain/MiningSimulator.h"
#include "Blockchain/Blockchain.h"
#include "Blockchain/TransactionQueue.h"
#include "Blockchain/CLI/BCliReader.h"

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
    // TODO: move consts used here
    // TODO: Add example usage
}

void handleFileInput(const BArgsToRun &argsToRun) {
    if (!doBlockchainFilesExist(argsToRun.inputFolderPath)) {
        std::cerr << "Error: Input folder must contain " << USERS_FILE << ", "
                << TRANSACTIONS_DIRECTORY << ", and " << BLOCKS_DIRECTORY << "\n";
        return;
    }

    std::cout << "Reading blockchain data from " << argsToRun.inputFolderPath << "...\n";

    // Read users from file
    std::filesystem::path usersFile = argsToRun.inputFolderPath / USERS_FILE;
    std::vector<User *> users = readUsersFromFile(usersFile);

    // Read all transactions from directory
    std::filesystem::path txDir = argsToRun.inputFolderPath / TRANSACTIONS_DIRECTORY;
    std::vector<Transaction *> allTransactions = readAllTransactionsFromDir(txDir);

    // Test reading all blocks
    std::filesystem::path blocksDir = argsToRun.inputFolderPath / BLOCKS_DIRECTORY;
    testReadAllBlocks(blocksDir, allTransactions, users);
}


void testReadAllBlocks(const std::filesystem::path &blocksDir,
                       const std::vector<Transaction *> &allTransactions,
                       const std::vector<User *> &users) {
    if (!std::filesystem::exists(blocksDir)) {
        std::cerr << "Blocks directory does not exist: " << blocksDir << "\n";
        return;
    }

    std::cout << "Testing block reading from " << blocksDir << "...\n\n";
    MiningSimulator mineSim(users);
    Block *previousBlock = mineSim.getGenesisBlock();
    std::cout << previousBlock->getBlockHash() << "\n";
    unsigned int blockIndex = 0;
    unsigned int successfulBlocks = 0;
    unsigned int failedBlocks = 0;

    while (true) {
        std::string fileName = std::to_string(blockIndex) + BLOCKS_FILE;
        std::filesystem::path blockFile = blocksDir / fileName;

        if (!std::filesystem::exists(blockFile)) {
            std::cout << "No more block files found after " << blockIndex << " blocks\n";
            break;
        }

        std::cout << "Reading block " << blockIndex << " from " << fileName << "...\n";

        BlockData blockData = readBlockDataFromFile(blockFile, allTransactions);
        if (blockData.expectedBlockHash.empty()) {
            std::cerr << "Failed to read block data from " << fileName << "\n";
            failedBlocks++;
            blockIndex++;
            continue;
        }

        Block *reconstructedBlock = reconstructAndVerifyBlock(blockData, previousBlock);

        if (reconstructedBlock) {
            std::cout << "Block " << blockIndex << " verified successfully!\n";
            std::cout << "  Hash:       " << reconstructedBlock->getBlockHash() << "\n";
            std::cout << "  Height:     " << reconstructedBlock->getHeight() << "\n";
            std::cout << "  Timestamp:  " << reconstructedBlock->getTimestamp() << "\n";
            std::cout << "  Nonce:      " << reconstructedBlock->getNonce() << "\n";
            std::cout << "  Difficulty: " << reconstructedBlock->getDifficultyTarget() << "\n";
            std::cout << "  Tx Count:   " << reconstructedBlock->getTransactions().size() << "\n\n";

            previousBlock = reconstructedBlock;
            successfulBlocks++;
        } else {
            std::cerr << "Failed to verify block " << blockIndex << "\n";
            std::cerr << "Chain is broken - cannot continue verification\n\n";
            failedBlocks++;
            break;
        }

        blockIndex++;
    }

    std::cout << "\n=== Block Reading Summary ===\n";
    std::cout << "Total blocks read: " << blockIndex << "\n";
    std::cout << "Successfully verified: " << successfulBlocks << "\n";
    std::cout << "Failed verification: " << failedBlocks << "\n";
}


bool doBlockchainFilesExist(const std::filesystem::path &folderPath) {
    std::filesystem::path usersFile = folderPath / USERS_FILE;
    std::filesystem::path txDir = folderPath / TRANSACTIONS_DIRECTORY; // Changed from TRANSACTIONS_FILE
    std::filesystem::path blocksDir = folderPath / BLOCKS_DIRECTORY;

    return std::filesystem::exists(usersFile) &&
           std::filesystem::exists(txDir) &&
           std::filesystem::is_directory(txDir) &&
           std::filesystem::exists(blocksDir) &&
           std::filesystem::is_directory(blocksDir);
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

    bool shouldContinueMining = argsToRun.numberOfBlocks > BLOCKS_BEFORE_TRANSACTIONS || argsToRun.numberOfBlocks == 0;
    unsigned int blocksBeforeTransactions = shouldContinueMining
                                                ? BLOCKS_BEFORE_TRANSACTIONS
                                                : argsToRun.numberOfBlocks;
    std::cout << "Mining " << blocksBeforeTransactions << " blocks to build UTXO pool...\n";
    for (unsigned int blocksMined = 0; blocksMined < blocksBeforeTransactions; blocksMined++) {
        std::vector<Transaction *> emptyTransactions;
        previousBlock = mineSim.mineBlockParallel(emptyTransactions, previousBlock);

        saveBlockToFile(blocksDir, previousBlock, blocksMined);
    }

    std::vector<Transaction *> mempool = generateAndSaveMempool(argsToRun.numberOfTransactions, users,
                                                                argsToRun.outputFolderPath);

    generateAndSaveUsersToFile(argsToRun.numberOfUsers, argsToRun.outputFolderPath);

    std::cout << "Initial output saved to " << argsToRun.outputFolderPath << "\n";

    // TODO: Generic function for mining.
    if (shouldContinueMining) {
        continueMining(mineSim, mempool, previousBlock, blocksDir, BLOCKS_BEFORE_TRANSACTIONS,
                       argsToRun.numberOfBlocks - BLOCKS_BEFORE_TRANSACTIONS);
    }
}

void continueMining(MiningSimulator &mineSim, std::vector<Transaction *> &mempool, Block *previousBlock,
                    const std::filesystem::path &blocksDir,
                    unsigned int startIndex,
                    const unsigned int leftToMine) {
    std::cout << "Continuing mining for " << leftToMine << " more blocks...\n";
    unsigned int fileIndex = startIndex;
    if (leftToMine == 0) {
        while (true) {
            previousBlock = miningHelper(mineSim, mempool, previousBlock, blocksDir, fileIndex);
        }
    } else {
        for (unsigned int blocksMined = 0; blocksMined < leftToMine; ++blocksMined) {
            previousBlock = miningHelper(mineSim, mempool, previousBlock, blocksDir, fileIndex);
        }
    }
    std::cout << "Finished mining. Transactions left in mempool: " << mempool.size() << " \n";
}

Block *miningHelper(MiningSimulator &mineSim, std::vector<Transaction *> &mempool, Block *previousBlock,
                    const std::filesystem::path &blocksDir,
                    unsigned int &fileIndex) {
    std::vector<Transaction *> batchMempool = TransactionQueue::pickValidTransactions(
        mempool, MAX_TRANSACTIONS_IN_BLOCK);
    previousBlock = mineSim.mineBlockParallel(batchMempool, previousBlock);

    saveBlockToFile(blocksDir, previousBlock, fileIndex);
    fileIndex++;
    TransactionQueue::freeMempoolFromMinedTransaction(mempool); // TODO: update transactions file after mining?
    return previousBlock;
}

void saveBlockToFile(const std::filesystem::path &blocksDir, Block *previousBlock, unsigned int &blockIndex) {
    std::string fileName = std::to_string(blockIndex) + BLOCKS_FILE;
    std::filesystem::path blockFile = blocksDir / fileName;
    std::ofstream outFile(blockFile);
    if (outFile.is_open()) {
        // npote: no space between the hashes since they're always a specified length (in general.h)
        outFile << previousBlock->getBlockHash() << previousBlock->getMinerPublicKey() << " "
                << previousBlock->getHeight() << " "
                << previousBlock->getTimestamp() << " "
                << previousBlock->getDifficultyTarget() << " "
                << previousBlock->getNonce() << " "
                << previousBlock->getTransactions().size() << "\n";

        for (const auto *tx: previousBlock->getTransactions()) {
            // !!! only id, no info about it here - that exists in transactions file and has to be gotten from there
            outFile << tx->getTransactionId() << "\n";
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

std::vector<Transaction *> generateAndSaveMempool(const unsigned int numberOfTransactions,
                                                  const std::vector<User *> &users,
                                                  const std::filesystem::path &outputFolderPath) {
    std::cout << "\nGenerating " << numberOfTransactions << " transactions...\n";

    std::vector<Transaction *> mempool = blockchainRandomGenerator::generateValidTransactions(
        users, numberOfTransactions);

    std::filesystem::path txDir = outputFolderPath / TRANSACTIONS_DIRECTORY;
    if (!std::filesystem::exists(txDir)) {
        std::filesystem::create_directories(txDir);
    }

    unsigned int txIndex = 0;
    for (const auto *tx: mempool) {
        std::string fileName = std::to_string(txIndex) + TRANSACTIONS_FILE;
        std::filesystem::path txFile = txDir / fileName;
        std::ofstream txOutFile(txFile);
        if (txOutFile.is_open()) {
            txOutFile << tx->getTransactionId() << tx->getSenderPublicKey() << " " << tx->getTransactionTime() << "\n";
            auto outputs = tx->getOutputs();
            for (const auto &[amount, receiverPublicKey]: outputs) {
                txOutFile << amount << " " << receiverPublicKey << "\n";
            }
            txOutFile.close();
        } else {
            std::cerr << "Failed to save transaction " << txIndex << "\n";
        }

        txIndex++;
    }

    std::cout << "Saved " << txIndex << " transactions to " << txDir << "\n";
    return mempool;
}

std::vector<User *> generateAndSaveUsersToFile(const unsigned int numberOfUsers,
                                               const std::filesystem::path &outputFolder) {
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



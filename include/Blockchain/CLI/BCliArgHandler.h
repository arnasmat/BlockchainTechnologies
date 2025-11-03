#ifndef BCLIARGHANDLER_H
#define BCLIARGHANDLER_H
#include <filesystem>
#include <vector>

#include "general.h"

// h - overrides others. outputs help

// -r <input_folder> : input from folder (Must have users.txt, transactions.txt and blockchainHistory.txt)
// If -r is not provided - then the arguments work:

// -o <output_folder> (make sure it auto makes the folder) : output to folder (default "./output") - overrides content!!
// -u <number_of_users> : number of users to generate (default 1000)
// -b <number_of_blocks> : number of blocks to mine (default - mines until application stop)
// -t <number_of_transactions> : number of transactions to generate (default 100000)

class MiningSimulator;
class Transaction;
class Block;

struct BArgsToRun {
    bool help = false; // -h --help
    std::filesystem::path inputFolderPath{""}; // -r <input_folder> --read <input_folder>
    // std::filesystem::path inputFolderPath{DEFAULT_OUTPUT_DIR}; // -r <input_folder> --read <input_folder>

    std::filesystem::path outputFolderPath{std::filesystem::path(DEFAULT_OUTPUT_DIR)};
    unsigned int numberOfUsers{1000};
    unsigned int numberOfBlocks{0}; // 0 - mine until app force close
    unsigned int numberOfTransactions{100000};
};

BArgsToRun handleBCliInput(int argc, char *argv[]);

void handleBCliArgs(const BArgsToRun &argsToRun);

void printBHelpInfo();

void handleFileInput(const BArgsToRun &argsToRun);

void handleFileGeneration(const BArgsToRun &argsToRun);

void generateUsers(const unsigned int numberOfUsers);

void generateTransactions(const unsigned int numberOfTransactions, const std::vector<class User *> &users);

std::vector<User *> generateAndSaveUsersToFile(const unsigned int numberOfUsers, std::filesystem::path outputFolder);

void saveBlockToFile(const std::filesystem::path &blocksDir, Block *previousBlock, unsigned int &blockIndex);

std::vector<Transaction *> generateAndSaveMempool(const unsigned int numberOfTransactions,
                                                  const std::vector<User *> &users,
                                                  const std::filesystem::path &outputFolderPath);

bool doBlockchainFilesExist(const std::filesystem::path &folderPath);

void generateBlockchainMetadata(std::filesystem::path outputDir);

void continueMining(MiningSimulator &mineSim, std::vector<Transaction *> &mempool, Block *previousBlock,
                    const std::filesystem::path &blocksDir,
                    unsigned int startIndex,
                    const unsigned int leftToMine);

Block *miningHelper(MiningSimulator &mineSim, std::vector<Transaction *> &mempool, Block *previousBlock,
                    const std::filesystem::path &blocksDir,
                    unsigned int &fileIndex);


#endif //BCLIARGHANDLER_H

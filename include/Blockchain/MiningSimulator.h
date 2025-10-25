#ifndef MININGSIMULATOR_H
#define MININGSIMULATOR_H
#include <queue>
#include <random>
#include <vector>
#include <omp.h>

#include "Blockchain.h"
#include "SystemAlgorithm.h"
#include "User.h"


class MiningSimulator : SystemAlgorithm {
private:
    std::vector<User *> users;
    bool isMining{true};
    Block *genesisBlock{nullptr};

    User *pickMiner() {
        static std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, users.size() - 1);
        return users.at(distrib(gen));
    }

    void announceNewBlock(const Block *newBlock) {
        std::stringstream ss;
        ss << "A new Block was mined by User " << newBlock->getMinerPublicKey() << "\n"
                << "Hash " << newBlock->getBlockHash() << "\n"
                << "Height " << newBlock->getHeight() << "\n"
                << "Timestamp " << newBlock->getTimestamp() << "\n"
                << "Difficulty " << newBlock->getDifficultyTarget() << "\n"
                << "Reward " << newBlock->calculateBlockReward() << "\n";
        const auto txs = newBlock->getTransactions();
        ss << "This block contains the following transactions: \n";
        for (const auto &tx: txs) {
            ss << "\nTransaction " << tx->getTransactionId() << ": \n";
            for (const auto &output: tx->getOutputs()) {
                ss << "From: " << tx->getSenderPublicKey();
                ss << ", To: " << output.second;
                ss << ", Amount: " << output.first;
                ss << "\n";
            }
        }
        ss << "\n----------------------\n";

        std::cout << ss.str();
    }

public:
    MiningSimulator(const std::vector<User *> &_users): users(_users) {
        isMining = true;
        genesisBlock = new Block(nullptr, "SYSTEM", "1.0", 0, {});
    };

    void stopMining() {
        isMining = false;
    }

    Block *getGenesisBlock() const {
        return genesisBlock;
    }


    Block *mineBlockParallel(
        std::vector<Transaction *> &processedTransactions,
        const Block *previousBlock
    ) {
        bool blockFound{false};
        Block *minedBlock = nullptr;

        // TODO: make sure this works with for example difficulty 1
#pragma omp parallel default(none) shared(blockFound, minedBlock, previousBlock, processedTransactions, users, isMining)
        {
            int threadId = omp_get_thread_num();
            const User *miner = users[threadId % users.size()]; // kzn random priskiriam zmogui
            int nonce = threadId * 1000000;

            while (isMining && !blockFound) {
                Block *localBlock = new Block(previousBlock, miner->getPublicKey(), SYSTEM_VERSION, nonce++,
                                              processedTransactions);

                if (!localBlock->isBlockValid()) {
                    delete localBlock;
                } else {
                    if (blockFound || previousBlock->getHeight() >= localBlock->getHeight()) {
                        delete localBlock;
                    } else {
                        minedBlock = localBlock;
                        announceNewBlock(minedBlock);
                        blockFound = true;
                    }
                }
            }
        }

        return minedBlock;
    }

    Block *mineBlock(
        std::vector<Transaction *> &processedTransactions,
        const Block *previousBlock,
        int nonceStart = 0
    ) {
        const User *miner{pickMiner()};
        int nonce{nonceStart};

        Block *newBlock{nullptr};

        while (isMining) {
            newBlock = new Block(previousBlock, miner->getPublicKey(), SYSTEM_VERSION, nonce++, processedTransactions);
            if (!newBlock->isBlockValid()) {
                newBlock->updateNonce();
            } else {
                announceNewBlock(newBlock);
                break;
            }
        }

        return newBlock;
    }

    std::queue<Transaction *> getBlockTransactions(const Block *newBlock) {
        std::queue<Transaction *> processedTransactions;

        const auto txs = newBlock->getTransactions();
        for (const auto &tx: txs) {
            processedTransactions.push(tx);
        }

        return processedTransactions;
    }
};


#endif //MININGSIMULATOR_H

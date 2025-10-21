#ifndef MININGSIMULATOR_H
#define MININGSIMULATOR_H
#include <queue>
#include <random>
#include <thread>
#include <vector>

#include "Blockchain.h"
#include "SystemAlgorithm.h"
#include "User.h"


class MiningSimulator : SystemAlgorithm {
private:
    std::vector<User *> users;
    bool isMining{true};

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
                    for(const auto &output: tx->getOutputs()) {
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
    };

    void stopMining() {
        isMining = false;
    }

    Block *mineBlock(
        std::queue<Transaction *> &processedTransactions,
        const Block *previousBlock
    ) {
        const User *miner{pickMiner()};
        int nonce{0};

        Block *newBlock{nullptr};

        while (isMining) {
            newBlock = new Block(previousBlock, miner->getPublicKey(), SYSTEM_VERSION, nonce++, {});
            if (!newBlock->isBlockValid()) {
                delete newBlock;
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

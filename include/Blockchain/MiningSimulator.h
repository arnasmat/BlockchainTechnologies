#ifndef MININGSIMULATOR_H
#define MININGSIMULATOR_H
#include <queue>
#include <random>
#include <vector>
#include <set>
#include <atomic>
#include <omp.h>

#include "general.h"
#include "Blockchain.h"
#include "SystemAlgorithm.h"
#include "User.h"
#include "UTXOSystem.h"
#include "HeadBlock.h"

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
        ss << "This block contains " << txs.size() << " transactions \n";
        // for (const auto &tx: txs) {
        //     ss << "\nTransaction " << tx->getTransactionId() << ": \n";
        //     for (const auto &output: tx->getOutputs()) {
        //         ss << "From: " << tx->getSenderPublicKey();
        //         ss << ", To: " << output.second;
        //         ss << ", Amount: " << output.first;
        //         ss << "\n";
        //     }
        // }
        ss << "\n----------------------\n";

        std::cout << ss.str();
    }

public:
    MiningSimulator(const std::vector<User *> &_users): users(_users) {
        isMining = true;
        std::vector<Transaction *> emptyVector{};
        genesisBlock = new Block(nullptr, users.at(0)->getPublicKey(), "1.0", 0, emptyVector);
        HeadBlock::getInstance().updateHeadBlock(genesisBlock);
    };

    void stopMining() {
        isMining = false;
    }

    Block *getGenesisBlock() const {
        return genesisBlock;
    }


    Block *mineBlockParallel(
        std::vector<Transaction *> &mempool,
        const Block *previousBlock
    ) {
        isMining = true;
        Block *minedBlock = nullptr;
        std::atomic<bool> blockFound = false; //first to mine a block in a batch gets accepted

        // TODO: make sure this works with for example difficulty 1
#pragma omp parallel
        {
            int threadId = omp_get_thread_num();
            const User *miner = users[threadId % users.size()]; // kzn random priskiriam zmogui
            int nonce = threadId * 1000000;

            std::vector<Transaction *> threadMempool{};
            threadMempool.reserve(100);

            static std::random_device rd;
            std::mt19937 gen(rd() + threadId);
            std::uniform_int_distribution<> distrib(0, users.size() - 1);

            for (int i = 0; i < mempool.size(); i++) {
                Transaction *pendingTransaction = mempool[i];
                threadMempool.push_back(pendingTransaction);
            }

            Block *localBlock = new Block(previousBlock, miner->getPublicKey(), SYSTEM_VERSION, threadId,
                                          threadMempool);

            while (isMining) {
                if (!localBlock->isBlockValid()) {
                    localBlock->updateNonce(omp_get_num_threads());
                } else {
                    bool expected = false;
                    if (blockFound.compare_exchange_strong(expected, true)) {
                        minedBlock = localBlock;
                        HeadBlock::getInstance().updateHeadBlock(minedBlock);
                        announceNewBlock(minedBlock);
                        isMining = false;
                    } else {
                        delete localBlock;
                        break;
                    }
                }
            }
        }

        // if (minedBlock) {
        //     auto minedTxs = minedBlock->getTransactions();
        //     mempool.erase(
        //         std::remove_if(mempool.begin(), mempool.end(),
        //             [&minedTxs](Transaction* tx) {
        //                 return std::find(minedTxs.begin(), minedTxs.end(), tx) != minedTxs.end();
        //             }),
        //         mempool.end()
        //     );
        // }

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

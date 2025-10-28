#ifndef MININGSIMULATOR_H
#define MININGSIMULATOR_H
#include <queue>
#include <random>
#include <vector>
#include <omp.h>

#include "Blockchain.h"
#include "SystemAlgorithm.h"
#include "User.h"
#include "UTXOSystem.h"


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
        std::vector<Transaction *> emptyVector{};
        genesisBlock = new Block(nullptr, users.at(0)->getPublicKey(), "1.0", 0, emptyVector);
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
        bool blockFound{false};
        Block *minedBlock = nullptr;

        // TODO: make sure this works with for example difficulty 1

        if(mempool.size()) {
            omp_set_num_threads(4);
        }

#pragma omp parallel default(none) shared(blockFound, minedBlock, previousBlock, mempool, users, isMining)
        {
            int threadId = omp_get_thread_num();
            const User *miner = users[threadId % users.size()]; // kzn random priskiriam zmogui
            int nonce = threadId * 1000000;

            std::vector<Transaction*> threadMempool{};
            threadMempool.reserve(10);

            for (size_t i = threadId; i < mempool.size(); i+=omp_get_num_threads()) 
            while(threadMempool.size() < 1)
            {
                Transaction *pendingTransaction = mempool[i];
                if(pendingTransaction->reserveTransaction()) {
                    std::vector<Utxo*> chosenUtxos = std::move(UtxoSystem::getInstance().findUtxosThatSatisfySum(pendingTransaction->getSenderPublicKey(), pendingTransaction->getOutputs()[0].first));
                    if(chosenUtxos.size()) {
                        pendingTransaction->fillTransaction(chosenUtxos);
                        threadMempool.push_back(pendingTransaction);
                    } else {
                       pendingTransaction->unreserveTransaction(); 
                    }
                }
            }
            
            while (isMining && !blockFound) {
                Block *localBlock = new Block(previousBlock, miner->getPublicKey(), SYSTEM_VERSION, nonce++,
                                              threadMempool);

                if (!localBlock->isBlockValid()) {
                    delete localBlock;
                } else {
                    #pragma omp critical
                    {
                        if (!blockFound) {
                            minedBlock = localBlock;
                            for(auto &transaction : minedBlock->getTransactions()) {
                                transaction->updateTransactionUtxosAfterBeingMined();
                            }
                            announceNewBlock(minedBlock);
                            blockFound = true;
                        } else {
                            delete localBlock;
                        }
                    }
                }
            }
        }

        if (minedBlock) {
            auto minedTxs = minedBlock->getTransactions();
            mempool.erase(
                std::remove_if(mempool.begin(), mempool.end(),
                    [&minedTxs](Transaction* tx) {
                        return std::find(minedTxs.begin(), minedTxs.end(), tx) != minedTxs.end();
                    }),
                mempool.end()
            );
        }     
        
        for (auto* tx : mempool) {
            tx->unreserveTransaction();
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

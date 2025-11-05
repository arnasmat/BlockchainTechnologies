
#include "general.h"
#include "RandomGenerator.h"
#include "Blockchain/Blockchain.h"
#include "Blockchain/MiningSimulator.h"
#include "Blockchain/User.h"
#include "Blockchain/CLI/BCliArgHandler.h"

#include "Blockchain/HeadBlock.h"
#include "Blockchain/TransactionQueue.h"

constexpr unsigned int NUMBER_OF_USERS{1000};

<<<<<<< HEAD
// int main() {
//     std::vector<User *> users{blockchainRandomGenerator::generateUsers(NUMBER_OF_USERS)};
//
//     // TODO: transaction queue
//
//     MiningSimulator mineSim(users);
//     std::vector<Transaction *> processedTransactions;
//     Block *previousBlock = mineSim.getGenesisBlock();
//     for (int i = 0; i < 50; i++) {
//         previousBlock = mineSim.mineBlockParallel(processedTransactions, previousBlock);
//     }
//
//     for (int i = 0; i < 5; i++) {
//         std::cout << "------------------------------------\n";
//     }
//
//     std::vector<Transaction *> transactions = blockchainRandomGenerator::generateValidTransactions(users, 100);
//     for (const auto *tx: transactions) {
//         std::cout << "Transaction: " << tx->getTransactionId() << "\n";
//         std::cout << "  From: " << tx->getSenderPublicKey() << "\n";
//
//         // Print outputs (amount -> recipient) and compute total transferred
//         double totalTransferred = 0.0;
//         const auto outputs = tx->getOutputs();
//         for (const auto &out: outputs) {
//             std::cout << "    To: " << out.second << ", Amount: " << out.first << "\n";
//             totalTransferred += out.first;
//         }
//         std::cout << "  Total transferred: " << totalTransferred << "\n";
//     }
//
//
//     return 0;
// }
//
// int main() {
//     std::vector<User *> users{blockchainRandomGenerator::generateUsers(NUMBER_OF_USERS)};
//
//     MiningSimulator mineSim(users);
//     std::vector<Transaction *> processedTransactions{};
//     mineSim.getGenesisBlock();
//     for (int i = 0; i < 50; i++) {
//         mineSim.mineBlockParallel(processedTransactions, HeadBlock::getInstance().getHeadBlock());
//     }
//
//     for (int i = 0; i < 5; i++) {
//         std::cout << "------------------------------------\n";
//     }
//
//     std::vector<Transaction *> mempool = blockchainRandomGenerator::generateValidTransactions(users, 100000);
//     // for (const auto *tx: mempool) {
//     //     std::cout << "Transaction: " << tx->getTransactionId() << "\n";
//     //     std::cout << "  From: " << tx->getSenderPublicKey() << "\n";
//
//     //     // Print outputs (amount -> recipient) and compute total transferred
//     //     double totalTransferred = 0.0;
//     //     const auto outputs = tx->getOutputs();
//     //     for (const auto &out: outputs) {
//     //         std::cout << "    To: " << out.second << ", Amount: " << out.first << "\n";
//     //         totalTransferred += out.first;
//     //     }
//     //     std::cout << "  Total amount to be transferred: " << totalTransferred << "\n";
//     // }
//
//     TransactionQueue::findPossibleMempoolTransaction(mempool);
//     std::cout << "Amount of transactions to be processed: " << mempool.size() << std::endl;
//
//     while (mempool.size()) {
//         std::vector<Transaction *> batchMempool = TransactionQueue::pickValidTransactions(
//             mempool, MAX_TRANSACTIONS_IN_BLOCK);
//         std::cout << "batch size of transactions taken (without coinbase transaction): " << batchMempool.size() <<
//                 ", total transactions in mempool left: " << mempool.size() << std::endl;
//         mineSim.mineBlockParallel(batchMempool, HeadBlock::getInstance().getHeadBlock());
//         TransactionQueue::freeMempoolFromMinedTransaction(mempool);
//     }
//
//     for (auto &user: users) {
//         std::cout << user->getPublicKey() << " " << UtxoSystem::getInstance().getBalanceOfPublicKey(
//             user->getPublicKey()) << std::endl;
//     }
=======
int main() {
    std::vector<User *> users{blockchainRandomGenerator::generateUsers(NUMBER_OF_USERS)};

    MiningSimulator mineSim(users);
    std::vector<Transaction *> processedTransactions{};
    for (int i = 0; i < 50; i++) {
        mineSim.mineBlockParallel(processedTransactions, HeadBlock::getInstance().getHeadBlock());
    }

    for (int i = 0; i < 5; i++) {
        std::cout << "------------------------------------\n";
    }

    std::vector<Transaction *> mempool = blockchainRandomGenerator::generateValidTransactions(users, 100000);
    // for (const auto *tx: mempool) {
    //     std::cout << "Transaction: " << tx->getTransactionId() << "\n";
    //     std::cout << "  From: " << tx->getSenderPublicKey() << "\n";

    //     // Print outputs (amount -> recipient) and compute total transferred
    //     double totalTransferred = 0.0;
    //     const auto outputs = tx->getOutputs();
    //     for (const auto &out: outputs) {
    //         std::cout << "    To: " << out.second << ", Amount: " << out.first << "\n";
    //         totalTransferred += out.first;
    //     }
    //     std::cout << "  Total amount to be transferred: " << totalTransferred << "\n";
    // }

    TransactionQueue::findPossibleMempoolTransaction(mempool);
    std::cout << "Amount of transactions to be processed: " << mempool.size() << std::endl;

    while (!mempool.empty()) {
        std::vector<Transaction *> batchMempool = TransactionQueue::pickValidTransactions(
            mempool, MAX_TRANSACTIONS_IN_BLOCK);
        std::cout << "batch size of transactions taken (without coinbase transaction): " << batchMempool.size() <<
                ", total transactions in mempool left: " << mempool.size() << std::endl;
        mineSim.mineBlockParallel(batchMempool, HeadBlock::getInstance().getHeadBlock());
        TransactionQueue::freeMempoolFromMinedTransaction(mempool);
    }

    for (auto &user: users) {
        std::cout << user->getPublicKey() << " " << UtxoSystem::getInstance().getBalanceOfPublicKey(
            user->getPublicKey()) << std::endl;
    }
>>>>>>> origin

int main(int argc, char *argv[]) {
    BArgsToRun args = handleBCliInput(argc, argv);
    handleBCliArgs(args);
    return 0;
}


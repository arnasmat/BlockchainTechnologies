
#include "libs.h"
#include "RandomGenerator.h"
#include "Blockchain/Blockchain.h"
#include "Blockchain/MiningSimulator.h"
#include "Blockchain/User.h"

constexpr unsigned int NUMBER_OF_USERS{10};

int main() {
    std::vector<User *> users{blockchainRandomGenerator::generateUsers(NUMBER_OF_USERS)};

    MiningSimulator mineSim(users);
    std::vector<Transaction *> processedTransactions{};
    Block *previousBlock = mineSim.getGenesisBlock();
    for (int i = 0; i < 50; i++) {
        previousBlock = mineSim.mineBlockParallel(processedTransactions, previousBlock);
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

    std::cout<<"Amount of transactions to be processed: "<<mempool.size()<<std::endl;

    while(mempool.size()) {
        previousBlock = mineSim.mineBlockParallel(mempool, previousBlock);
    }

    for(auto &user : users) {
        std::cout<<user->getPublicKey()<<" "<<UtxoSystem::getInstance().getBalanceOfPublicKey(user->getPublicKey())<<std::endl;
    }

    return 0;
}

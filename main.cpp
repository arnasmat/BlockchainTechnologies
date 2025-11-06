
#include "libs.h"
#include "RandomGenerator.h"
#include "Blockchain/Blockchain.h"
#include "Blockchain/MiningSimulator.h"
#include "Blockchain/User.h"
#include "Blockchain/HeadBlock.h"
#include "Blockchain/TransactionQueue.h"

constexpr unsigned int NUMBER_OF_USERS{1000};

int main() {
    std::vector<User *> users{blockchainRandomGenerator::generateUsers(NUMBER_OF_USERS)};

    MiningSimulator mineSim(users);
    std::vector<Transaction *> processedTransactions{};
    mineSim.getGenesisBlock();
    for (int i = 0; i < 50; i++) {
        mineSim.mineBlockParallel(processedTransactions, HeadBlock::getInstance().getHeadBlock());
    }

    for (int i = 0; i < 5; i++) {
        std::cout << "------------------------------------\n";
    }

    std::vector<Transaction *> mempool = blockchainRandomGenerator::generateValidTransactions(users, 100000);
    std::cout<<"Amount of transactions to be processed: "<<mempool.size()<<std::endl;

    while(mempool.size()) {
        std::vector<Transaction*> batchMempool = TransactionQueue::pickValidTransactions(mempool, MAX_TRANSACTIONS_IN_BLOCK);
        //std::cout << "batch size of transactions taken (without coinbase transaction): "<< batchMempool.size() <<", total transactions in mempool left: " << mempool.size() << std::endl;
        mineSim.mineBlockParallel(batchMempool, HeadBlock::getInstance().getHeadBlock());
        TransactionQueue::freeMempoolFromMinedTransaction(mempool);
    }

    for(auto &user : users) {
        std::cout<<user->getPublicKey()<<" "<<UtxoSystem::getInstance().getBalanceOfPublicKey(user->getPublicKey())<<std::endl;
    }

    return 0;
}

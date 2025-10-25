#include <iostream>
#include <queue>
#include <random>

#include "Blockchain/Blockchain.h"
#include "Blockchain/MiningSimulator.h"
#include "Blockchain/User.h"
#include "Blockchain/UTXOSystem.h"

int main() {
    std::vector<User *> users;
    for (int i = 1; i <= 1000; i++) {
        users.push_back(new User(i));
    }

    // TODO: transaction queue

    MiningSimulator mineSim(users);
    std::vector<Transaction *> processedTransactions;
    Block *previousBlock = mineSim.getGenesisBlock();
    while (true) {
        previousBlock = mineSim.mineBlockParallel(processedTransactions, previousBlock);
    }

    return 0;
}

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

    std::vector<Transaction *> processedTransactions;
    // TODO: transaction queue

    MiningSimulator mineSim(users);
    Block *genesisBlock = new Block(nullptr, "SYSTEM", "1.0", 0, {});
    std::cout << "GENESIS: " << genesisBlock->getBlockHash() << "\n";
    Block *previousBlock = genesisBlock;
    while (true) {
        previousBlock = mineSim.mineBlock(processedTransactions, previousBlock);
    }

    return 0;
}

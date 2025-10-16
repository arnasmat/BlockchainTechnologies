#include <iostream>
#include <queue>
#include <random>

#include "Blockchain/Blockchain.h"
#include "Blockchain/MiningSimulator.h"
#include "Blockchain/User.h"

int main() {
    std::vector<User*> users;
    for (int i = 1; i <= 1000; i++) {
        users.push_back(new User(i));
    }

    std::queue<Transaction*> transactionList;
    // TODO: transaction queue

    MiningSimulator mineSim(users);
    Block* genesisBlock = new Block(nullptr, "SYSTEM", time(nullptr), 1.0, 0, {});
    Block* previousBlock = genesisBlock;
    while (true) {
        previousBlock = mineSim.mineBlock(transactionList, previousBlock);
    }

    return 0;
}
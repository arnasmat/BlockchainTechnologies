#include <iostream>
#include <queue>

#include "Blockchain/Blockchain.h"
#include "Blockchain/User.h"

int main() {

    Block* previousBlock = nullptr;
    unsigned int blockCount = 0;
    User miner(0);

    std::queue<Transaction*> transactionList;

    // this miner simulator here is vibe coded I just wanted to test it
    while (true) {
        int nonce = 0;
        Block* newBlock = nullptr;

        // Mine a block that meets the difficulty target
        while (true) {
            newBlock = new Block(previousBlock, miner.getPublicKey(), time(nullptr), 1.0, nonce++, {});
            if (newBlock->isBlockValid()){
                break;
            }
            delete newBlock;
        }

        std::cout << "Block " << blockCount << " | Difficulty: " << newBlock->getDifficultyTarget()
                  << " | Hash: " << newBlock->getBlockHash() << std::endl;

        // TODO: remove this later
        if (newBlock->getDifficultyTarget() >= 17)
            break;

        previousBlock = newBlock;
        blockCount++;
    }

    std::cout<<previousBlock->getBlockHash()<<"\n"<<previousBlock->getHeight()<<"\n"<<previousBlock->getTimestamp()<<"\n"<<previousBlock->getDifficultyTarget()<<"\n";
}

#include <iostream>
#include "Blockchain/Blockchain.h"

int main() {
    Block* previousBlock = nullptr;
    unsigned int blockCount = 0;

    // this miner simulator here is vibe coded i just wanted to test it
    while (true) {
        int nonce = 0;
        Block* newBlock = nullptr;

        // Mine a block that meets the difficulty target
        while (true) {
            newBlock = new Block(previousBlock, time(nullptr), 1.0, nonce++, {});
            if (newBlock->getBlockHash().substr(0, newBlock->getDifficultyTarget()) ==
                std::string(newBlock->getDifficultyTarget(), '0')) {
                break;
            }
            delete newBlock;
        }

        std::cout << "Block " << blockCount << " | Difficulty: " << newBlock->getDifficultyTarget()
                  << " | Hash: " << newBlock->getBlockHash() << std::endl;

        // Stop when difficulty target reaches 5
        if (newBlock->getDifficultyTarget() >= 5)
            break;

        previousBlock = newBlock;
        blockCount++;
    }
}

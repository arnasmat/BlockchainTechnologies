#include "Blockchain/CLI/BCliReader.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Blockchain/Transaction.h"
#include "Blockchain/Blockchain.h"
#include "Blockchain/MerkleTree.h"
#include "general.h"

BlockData readBlockDataFromFile(const std::filesystem::path &blockFile,
                                const std::vector<Transaction *> &allTransactions) {
    std::ifstream inFile(blockFile);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open block file " << blockFile << "\n";
        return {};
    }

    BlockData data;

    // Read first line - block header
    std::string line;
    std::getline(inFile, line);

    // Parse: blockHash (64 chars) + minerPublicKey (64 chars) + other fields
    data.expectedBlockHash = line.substr(0, HASH_LENGTH);
    data.minerPublicKey = line.substr(HASH_LENGTH, HASH_LENGTH);

    // Parse remaining fields
    std::istringstream iss(line.substr(HASH_LENGTH * 2));
    iss >> data.height >> data.timestamp >> data.difficultyTarget >> data.nonce;

    unsigned int txCount;
    iss >> txCount;

    // Read transaction IDs and match them to full transactions
    for (unsigned int i = 0; i < txCount; i++) {
        std::string txId;
        std::getline(inFile, txId);

        // Skip the first transaction (coinbase) as it will be recreated
        if (i == 0) {
            continue;
        }

        auto it = std::ranges::find_if(allTransactions,
                                       [&txId](const Transaction *tx) {
                                           return tx->getTransactionId() == txId;
                                       });

        if (it != allTransactions.end()) {
            data.transactions.push_back(*it);
        } else {
            std::cerr << "Transaction " << txId << " not found in transaction pool\n";
        }
    }

    inFile.close();

    data.version = SYSTEM_VERSION;

    return data;
}

Block *reconstructAndVerifyBlock(const BlockData &blockData, Block *previousBlock) {
    // Reconstruct block using the constructor that accepts timestamp
    Block *reconstructedBlock = new Block(
        previousBlock,
        blockData.minerPublicKey,
        blockData.version,
        blockData.nonce,
        blockData.timestamp,
        blockData.transactions
    );

    // Verify the reconstructed block hash matches expected hash
    std::string reconstructedHash = reconstructedBlock->getBlockHash();

    if (reconstructedHash != blockData.expectedBlockHash) {
        std::cerr << "ERROR: Block hash mismatch!\n";
        std::cerr << "  Expected:  " << blockData.expectedBlockHash << "\n";
        std::cerr << "  Got:       " << reconstructedHash << "\n";
        std::cerr << "  Height:    " << blockData.height << "\n";
        std::cerr << "  Timestamp: " << blockData.timestamp << "\n";
        std::cerr << "  Nonce:     " << blockData.nonce << "\n";

        delete reconstructedBlock;
        return nullptr;
    }

    std::cout << "Block " << blockData.height << " verified successfully (hash matches)\n";
    return reconstructedBlock;
}

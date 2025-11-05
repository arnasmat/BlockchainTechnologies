#include "Blockchain/CLI/BCliReader.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Blockchain/Transaction.h"
#include "Blockchain/Blockchain.h"
#include "Blockchain/MerkleTree.h"
#include "general.h"
#include "Blockchain/User.h"

class User;

BlockData readBlockDataFromFile(const std::filesystem::path &blockFile,
                                const std::vector<Transaction *> &allTransactions) {
    std::ifstream inFile(blockFile);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open block file " << blockFile << "\n";
        return {};
    }

    BlockData data;
    std::string line;
    std::getline(inFile, line);

    // Parse: currentHash (64) + minerPubKey (64) + fields
    data.expectedBlockHash = line.substr(0, HASH_LENGTH);
    data.minerPublicKey = line.substr(HASH_LENGTH, HASH_LENGTH);

    std::istringstream iss(line.substr(HASH_LENGTH * 2));
    unsigned int txCount;
    iss >> data.height >> data.timestamp >> data.difficultyTarget >> data.nonce >> txCount;

    for (unsigned int i = 0; i < txCount; i++) {
        std::string txId;
        std::getline(inFile, txId);
        if (i == 0) continue; // Skip coinbase

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
    // Get the expected previous hash from the previous block
    std::string expectedPrevHash = previousBlock
                                       ? previousBlock->getBlockHash()
                                       : "0000000000000000000000000000000000000000000000000000000000000000";

    // Reconstruct block
    Block *reconstructedBlock = new Block(
        previousBlock,
        blockData.minerPublicKey,
        blockData.version,
        blockData.nonce,
        blockData.timestamp,
        blockData.transactions
    );

    std::string reconstructedHash = reconstructedBlock->getBlockHash();

    if (reconstructedHash != blockData.expectedBlockHash) {
        std::cerr << "ERROR: Block hash mismatch!\n";
        std::cerr << "  Expected:  " << blockData.expectedBlockHash << "\n";
        std::cerr << "  Got:       " << reconstructedHash << "\n";
        std::cerr << "  Height:    " << blockData.height << "\n";
        std::cerr << "  Timestamp: " << blockData.timestamp << "\n";
        std::cerr << "  Nonce:     " << blockData.nonce << "\n";
        std::cerr << "  Prev Hash: " << expectedPrevHash << "\n";

        delete reconstructedBlock;
        return nullptr;
    }

    return reconstructedBlock;
}


TransactionData readTransactionFromFile(const std::filesystem::path &txFile) {
    std::ifstream inFile(txFile);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open transaction file " << txFile << "\n";
        return {};
    }

    TransactionData data;
    // Read first line: transactionId + senderPublicKey + timestamp
    std::string line;
    std::getline(inFile, line);
    if (line.length() < HASH_LENGTH * 2) {
        std::cerr << "Invalid transaction file format: " << txFile << "\n";
        return {};
    }
    data.transactionId = line.substr(0, HASH_LENGTH);
    data.senderPublicKey = line.substr(HASH_LENGTH, HASH_LENGTH);
    // Parse timestamp from remaining part
    std::istringstream iss(line.substr(HASH_LENGTH * 2));
    iss >> data.timestamp;

    // Read outputs
    while (std::getline(inFile, line)) {
        std::istringstream outputIss(line);
        double amount;
        std::string receiverPublicKey;
        if (outputIss >> amount >> receiverPublicKey) {
            data.outputs.emplace_back(amount, receiverPublicKey);
        }
    }

    inFile.close();
    return data;
}


bool isCoinbaseTransaction(const TransactionData &txData) {
    return txData.senderPublicKey == SYSTEM_NAME ||
           txData.senderPublicKey.find_first_not_of('0') == std::string::npos;
}

std::vector<Transaction *> readAllTransactionsFromDir(const std::filesystem::path &txDir) {
    std::vector<Transaction *> transactions;
    if (!std::filesystem::exists(txDir)) {
        std::cerr << "Transaction directory does not exist: " << txDir << "\n";
        return transactions;
    }
    // Collect all transaction files
    std::vector<std::filesystem::path> txFiles;
    for (const auto &entry: std::filesystem::directory_iterator(txDir)) {
        if (entry.path().extension() == FILE_EXTENTION) {
            txFiles.push_back(entry.path());
        }
    }
    // Sort by transaction index
    std::sort(txFiles.begin(), txFiles.end(),
              [](const std::filesystem::path &a, const std::filesystem::path &b) {
                  std::string aName = a.stem().string();
                  std::string bName = b.stem().string();
                  int aNum = std::stoi(aName.substr(0, aName.find('_')));
                  int bNum = std::stoi(bName.substr(0, bName.find('_')));
                  return aNum < bNum;
              });
    std::cout << "Reading " << txFiles.size() << " transactions...\n";
    for (const auto &txFile: txFiles) {
        TransactionData txData = readTransactionFromFile(txFile);
        if (txData.transactionId.empty()) {
            std::cerr << "Skipping invalid transaction file: " << txFile << "\n";
            continue;
        }
        // Skip coinbase transactions as they'll be recreated during block reconstruction
        if (isCoinbaseTransaction(txData)) {
            std::cout << "Skipping coinbase transaction: " << txData.transactionId << "\n";
            continue;
        }
        // Create transaction object
        Transaction *tx = new Transaction(txData.senderPublicKey, txData.outputs, txData.timestamp,
                                          txData.transactionId);
        // Verify transaction ID matches
        if (tx->getTransactionId() != txData.transactionId) {
            std::cerr << "WARNING: Transaction ID mismatch for " << txFile << "\n";
            std::cerr << "  Expected: " << txData.transactionId << "\n";
            std::cerr << "  Got:      " << tx->getTransactionId() << "\n";
        }

        transactions.push_back(tx);
    }

    std::cout << "Loaded " << transactions.size() << " transactions (excluding coinbase)\n";
    return transactions;
}

std::vector<User *> readUsersFromFile(const std::filesystem::path &usersFile) {
    std::vector<User *> users;
    std::ifstream inFile(usersFile);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open users file " << usersFile << "\n";
        return users;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;

        // Line contains just the public key
        std::string publicKey = line;
        users.push_back(new User(publicKey));

        // Skip UTXO lines until empty line
        while (std::getline(inFile, line) && !line.empty()) {
            // We can ignore UTXOs for now since they'll be rebuilt from blocks
        }
    }

    inFile.close();
    std::cout << "Loaded " << users.size() << " users from file\n";
    return users;
}

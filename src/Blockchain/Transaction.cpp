#include "Blockchain/Transaction.h"
#include "Blockchain/UTXO.h"
#include "Blockchain/UTXOSystem.h"
#include "Blockchain/UserSystem.h"
#include "Blockchain/User.h"

Transaction::Transaction(const std::string &senderPk, const std::string &receiverPk, const double amount)
    : senderPublicKey(senderPk) {
    outputs.push_back({amount, receiverPk});
    std::string content = std::to_string(transactionTime) + std::to_string(amount) + receiverPk;
    
    // Coinbase transactions (from SYSTEM) don't need signatures
    if (senderPk == SYSTEM_NAME) {
        transactionId = hash->generateHash(content);
    } else {
        User* sender = UserSystem::getInstance().findUserMatchingPublicKey(senderPk);
        if (sender) {
            transactionId = sender->makeSignature(content);
        } else {
            transactionId = hash->generateHash(content); //prolly will never happen, just in case
        }
    }
}

void Transaction::fillTransaction(const std::vector<Utxo *> &chosenUtxos) {
    userUtxos = chosenUtxos;
    double achievedSum = 0;
    for (auto &eachUtxo: userUtxos) {
        inputs.push_back({eachUtxo->getTransaction(), eachUtxo->getVout()});
        achievedSum += eachUtxo->getAmount();
    }

    if (senderPublicKey != SYSTEM_NAME) {
        double fee = achievedSum - outputs[0].first;
        if (fee > 0) {
            outputs.push_back({fee, senderPublicKey}); // so far it is simply being sent back to sender as change
        }
    }
}

void Transaction::updateTransactionUtxosAfterBeingFinalised() {
    UtxoSystem::getInstance().deleteUtxo(senderPublicKey, userUtxos);
    UtxoSystem::getInstance().addNewUtxos(outputs, this);
}

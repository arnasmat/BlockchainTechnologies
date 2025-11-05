#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <vector>
#include <string>
#include <utility>
#include <ctime>
#include <atomic>

#include "SystemAlgorithm.h"
#include "HashAlg/HashGenInterface.h"
#include "UTXO.h"
#include "UTXOSystem.h"
#include "general.h"

class Transaction : SystemAlgorithm {
    std::string content{};
    std::string transactionId{};
    std::string senderPublicKey{};
    std::vector<std::pair<const Transaction *, unsigned int> > inputs;
    // transaction and index of valid output of other transaction
    std::vector<std::pair<double, std::string> > outputs; // amount and public key of receiver
    std::time_t transactionTime{time(nullptr)};
    std::vector<Utxo *> userUtxos;

public:
    Transaction(const std::string &senderPk, const std::string &receiverPk, const double amount)
        : senderPublicKey(senderPk), transactionTime(time(nullptr)) {
        outputs.push_back({amount, receiverPk});
        content += std::to_string(transactionTime) + std::to_string(amount);
        transactionId = hash->generateHash(senderPublicKey + content); //will be used if it is a coinbase transaction
    }

    // Coinbase transaction with timestamp parameter
    Transaction(const std::string &senderPk, const std::string &receiverPk, const double amount, const time_t txTime)
        : senderPublicKey(senderPk), transactionTime(txTime) {
        outputs.push_back({amount, receiverPk});
        content += std::to_string(transactionTime) + std::to_string(amount);
        transactionId = hash->generateHash(senderPublicKey + content);
    }

    Transaction(const std::string &senderPk, const std::vector<std::pair<double, std::string> > &outputs,
                const time_t txTime)
        : senderPublicKey(senderPk), outputs(outputs), transactionTime(txTime) {
        content += std::to_string(transactionTime) + std::to_string(outputs[0].first);
        transactionId = hash->generateHash(senderPublicKey + content);
    }


    // Constructor for reconstructing transactions from saved files with known transaction ID
    Transaction(const std::string &senderPk, const std::vector<std::pair<double, std::string> > &outputs,
                const time_t txTime, const std::string &txId)
        : senderPublicKey(senderPk), outputs(outputs), transactionTime(txTime), transactionId(txId) {
        content += std::to_string(transactionTime) + std::to_string(outputs[0].first);
    }


    //if user has enough utxos for this transaction, we associate these utxos with this transaction
    void fillTransaction(const std::vector<Utxo *> &chosenUtxos) {
        userUtxos = std::move(chosenUtxos);
        double achievedSum = 0;
        for (auto &eachUtxo: userUtxos) {
            inputs.push_back({eachUtxo->getTransaction(), eachUtxo->getVout()});
            achievedSum += eachUtxo->getAmount();
            content += eachUtxo->getTransaction()->getTransactionId();
        }

        if (senderPublicKey != SYSTEM_NAME) {
            double fee = achievedSum - outputs[0].first;
            if (fee > 0) {
                content += std::to_string(fee);
                outputs.push_back({fee, senderPublicKey}); // so far it is simply being sent back to sender as change
            }
        }
        transactionId = hash->generateHash(senderPublicKey + outputs[0].second + content);
    }

    std::string getTransactionId() const {
        return transactionId;
    }

    std::string getSenderPublicKey() const {
        return senderPublicKey;
    }

    time_t getTransactionTime() const {
        return transactionTime;
    }

    const std::vector<std::pair<const Transaction *, unsigned int> > &getInputs() const {
        return inputs;
    }

    const std::vector<std::pair<double, std::string> > &getOutputs() const {
        return outputs;
    }

    void updateTransactionUtxosAfterBeingFinalised() {
        UtxoSystem::getInstance().deleteUtxo(senderPublicKey, userUtxos);
        UtxoSystem::getInstance().addNewUtxos(outputs, this);
    }

    std::string getOutputsAsString() const {
        std::string result;
        for (const auto &[amount, receiverPk]: outputs) {
            result += std::to_string(amount) + receiverPk;
        }
        return result;
    }
};

#endif //TRANSACTION_H

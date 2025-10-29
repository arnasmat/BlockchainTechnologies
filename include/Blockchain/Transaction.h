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
#include "libs.h"

class Transaction : SystemAlgorithm {

    std::string content{};
    std::string transactionId{};
    std::string senderPublicKey{};
    std::vector<std::pair<const Transaction *, unsigned int> > inputs;
    // transaction and index of valid output of other transaction
    std::vector<std::pair<double, std::string> > outputs; // amount and public key of receiver
    std::time_t transactionTime{time(nullptr)};
    std::atomic<bool> isReserved = false;
    std::vector<Utxo*> userUtxos;

public:
    Transaction(const std::string &senderPk, const std::string &receiverPk, const double amount)
        : senderPublicKey(senderPk) {
            outputs.push_back({amount, receiverPk});
            content += std::to_string(transactionTime) + std::to_string(amount);
            //so that every transasctionID be different
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

    void updateTransactionUtxosAfterBeingMined() {
        UtxoSystem::getInstance().deleteUtxo(senderPublicKey, userUtxos);
        UtxoSystem::getInstance().addNewUtxos(outputs, this);

    }

    //multi-thread safe appproach
    bool reserveTransaction() {
        bool expected = false;
        return isReserved.compare_exchange_strong(expected, true);
    }

    //since only one thread has reserved this transaction, we can unreserve it this way 
    void unreserveTransaction() {
        isReserved = false;
    }

};

#endif //TRANSACTION_H

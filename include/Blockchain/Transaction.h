#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <vector>
#include <string>
#include <utility>
#include <ctime>
#include <atomic>

#include "SystemAlgorithm.h"
#include "HashAlg/HashGenInterface.h"
#include "libs.h"

// Forward declarations to avoid circular dependency
class Utxo;
class UtxoSystem;
class UserSystem;

class Transaction : SystemAlgorithm {

    std::string transactionId{};
    std::string senderPublicKey{};
    std::vector<std::pair<const Transaction *, unsigned int> > inputs;
    // transaction and index of valid output of other transaction
    std::vector<std::pair<double, std::string> > outputs; // amount and public key of receiver
    std::time_t transactionTime{time(nullptr)};
    std::vector<Utxo*> userUtxos;

public:
    Transaction(const std::string &senderPk, const std::string &receiverPk, const double amount);

    //if user has enough utxos for this transaction, we associate these utxos with this transaction
    void fillTransaction(const std::vector<Utxo *> &chosenUtxos);

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

    void updateTransactionUtxosAfterBeingFinalised();

};

#endif //TRANSACTION_H

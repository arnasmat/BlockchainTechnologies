#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <string>

#include "SystemAlgorithm.h"
#include "HashAlg/HashGenInterface.h"

class Transaction : SystemAlgorithm {
    std::string transactionId{};
    std::string senderPublicKey{};
    std::string receiverPublicKey{};
    unsigned int amount{};

public:
    Transaction(const std::string &senderPk, const std::string &receiverPk, const int amount)
        : senderPublicKey(senderPk),
          receiverPublicKey(receiverPk),
          amount(amount) {
        transactionId = hash->generateHash(senderPublicKey + receiverPublicKey + std::to_string(amount));
    }

    std::string getTransactionId() {
        return transactionId;
    }

    unsigned int getAmount() const {
        return amount;
    }

    std::string getSenderPublicKey() {
        return senderPublicKey;
    }

    std::string getReceiverPublicKey() {
        return receiverPublicKey;
    }
};

#endif //TRANSACTION_H

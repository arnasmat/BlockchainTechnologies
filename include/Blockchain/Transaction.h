#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <string>

#include "HashAlg/HashGenInterface.h"

class Transaction : SystemAlgorithm {
    std::string _transactionId{};
    std::string _senderPublicKey{};
    std::string _receiverPublicKey{};
    unsigned int _amount{};

public:
    Transaction(const std::string &senderPk, const std::string &receiverPk, const int amount)
        : _senderPublicKey(senderPk),
          _receiverPublicKey(receiverPk),
          _amount(amount) {
        _transactionId = _hashAlg->generateHash(_senderPublicKey + _receiverPublicKey + std::to_string(_amount));
    }

    std::string getTransactionId() {
        return _transactionId;
    }

    unsigned int getAmount() {
        return _amount;
    }

    std::string getSenderPublicKey() {
        return _senderPublicKey;
    }

    std::string getReceiverPublicKey() {
        return _receiverPublicKey;
    }
};

#endif //TRANSACTION_H

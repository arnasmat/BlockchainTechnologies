#ifndef UTOX_H
#define UTOX_H
#include <atomic>
#include "SystemAlgorithm.h"
#include "Transaction.h"

//i think its safer to have this class so amount and publicKey couldn't be mutated
class Utxo : SystemAlgorithm{
private:
  std::string id;
  Transaction *transaction;
  unsigned int vout;
  double amount;
  std::string receiversPublicKey;
  bool isReserved{false};

public:
  Utxo(Transaction *transaction, unsigned int vout, double amount, std::string publicKey);

  const Transaction *getTransaction() const {
    return transaction;
  }

  unsigned int getVout() const {
    return vout;
  }

  double getAmount() const {
    return amount;
  }

  const std::string &getReceiversPublicKey() const {
    return receiversPublicKey;
  }

  bool reserveUtxo() {
    if(!isReserved) {
      isReserved = true;
      return true;
    }
    return false;
  }

  void unreserveUtxo() {
    isReserved = false;
  }

  std::string getId() const {
    return id;
  }

};

#endif
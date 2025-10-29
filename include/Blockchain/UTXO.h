#ifndef UTOX_H
#define UTOX_H
class Transaction;
#include <atomic>

//i think its safer to have this class so amount and publicKey couldn't be mutated
class Utxo {
private:
  Transaction *transaction;
  unsigned int vout;
  double amount;
  std::string receiversPublicKey;
  bool isReserved{false};

public:
  Utxo(Transaction *transaction, unsigned int vout, double amount, std::string publicKey) : transaction(transaction),
    vout(vout), amount(amount), receiversPublicKey(publicKey) {
  }

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

};

#endif
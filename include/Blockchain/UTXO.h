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
  std::atomic<bool> isReserved{false};

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

  //multi-thread safe appproach
  bool reserveUtxo() {
    bool expected = false;
    return isReserved.compare_exchange_strong(expected, true);
  }

  //since only one thread has reserved this utxo, we can unreserve it this way
  void unreserveUtxo() {
    isReserved = false;
  }

};

#endif
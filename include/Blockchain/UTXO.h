#ifndef UTOX_H
#define UTOX_H

#include "SystemAlgorithm.h"
class Transaction;

//i think its safer to have this class so amount and publicKey couldn't be mutated
class Utxo {
    private:
        Transaction *transaction;
        unsigned int vout;
        double amount; 
        std::string receiversPublicKey; 

    public:
      Utxo(Transaction* transaction, unsigned int vout, double amount, std::string publicKey) : transaction(transaction), vout(vout), amount(amount), receiversPublicKey(publicKey) {
      }

      const Transaction* getTransaction() const {
        return transaction;
      }

      unsigned int getVout() const {
        return vout;
      }

      double getAmount() const {
        return amount;
      }

      const std::string& getReceiversPublicKey() const {
        return receiversPublicKey;
      }
   
};

#endif
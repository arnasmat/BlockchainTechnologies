#include "Blockchain/UTXO.h"
#include "Blockchain/Transaction.h"

Utxo::Utxo(Transaction *transaction, unsigned int vout, double amount, std::string publicKey) 
    : transaction(transaction), vout(vout), amount(amount), receiversPublicKey(publicKey) {
    id = hash->generateHash(transaction->getTransactionId() + std::to_string(vout) + receiversPublicKey);
}
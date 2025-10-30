//
// Created by arnas on 10/11/25.
//

#ifndef SYSTEMALGORITHM_H
#define SYSTEMALGORITHM_H
#include "HashAlg/HashGenInterface.h"
#include "HashAlg/MatrixHash.h"
class Transaction;

class SystemAlgorithm {
protected:
    HashGenInterface *hash;
    static constexpr unsigned short int MAX_HASH_LENGTH{64};
    const std::string SYSTEM_VERSION{"1.0"};
    Transaction *root = nullptr;

    SystemAlgorithm(): hash(new MatrixHash()) {
    }

    ~SystemAlgorithm() {
        delete hash;
    }
};


#endif //SYSTEMALGORITHM_H

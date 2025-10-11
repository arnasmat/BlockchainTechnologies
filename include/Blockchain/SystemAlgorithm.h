//
// Created by arnas on 10/11/25.
//

#ifndef SYSTEMALGORITHM_H
#define SYSTEMALGORITHM_H
#include "HashAlg/HashGenInterface.h"
#include "HashAlg/MatrixHash.h"


class SystemAlgorithm {
public:
    HashGenInterface* _hashAlg;

    SystemAlgorithm(): _hashAlg(new MatrixHash()) {}
    ~SystemAlgorithm() {
        delete _hashAlg;
    }
};



#endif //SYSTEMALGORITHM_H

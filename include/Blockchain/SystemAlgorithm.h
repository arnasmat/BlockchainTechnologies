//
// Created by arnas on 10/11/25.
//

#ifndef SYSTEMALGORITHM_H
#define SYSTEMALGORITHM_H
#include "HashAlg/HashGenInterface.h"
#include "HashAlg/MatrixHash.h"
#include "HashAlg/VibeHash.h"
#include "general.h"


class SystemAlgorithm {
protected:
    HashGenInterface *hash;

    SystemAlgorithm() {
        switch (HASH_TYPE) {
            case HUMAN:
                hash = new HumanHash();
                break;
            case VIBE:
                hash = new VibeHash();
                break;
            case MATRIX:
                hash = new MatrixHash();
                break;
        }
    }


    ~SystemAlgorithm() {
        delete hash;
    }
};


#endif //SYSTEMALGORITHM_H

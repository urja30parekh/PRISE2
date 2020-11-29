#include "Triple.h"

Triple::Triple()
{
    //this->Pr = NULL;
    this->Pb = NULL;
    this->marked = false;
    this->pctgInNonTarSeqs = 0;
    this->pctgInTargetSeqs = 0;
}

Triple::~Triple()
{
    //this->Pr = NULL;
    //delete this->Pb;
    this->Pb = NULL;
}

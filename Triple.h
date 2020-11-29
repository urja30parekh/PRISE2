#ifndef TRIPLE_H
#define TRIPLE_H

//#include "PrimerPair.h"
#include "Probe.h"

class Triple
{
public:
    Triple();
    ~Triple();

    //PrimerPair *Pr;
    Probe *Pb;

    float pctgInTargetSeqs;
    float pctgInNonTarSeqs;
    int pbPrComp;
    //int pbPr3Comp;
    bool marked;

};

#endif // TRIPLE_H

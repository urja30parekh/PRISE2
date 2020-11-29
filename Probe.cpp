#include "Probe.h"


Probe::Probe(){
    probeSeq = "";
    pctgInTargetSeqs = 0;
    pctgInNonTarSeqs = 0;

    //isEstimateTar = true;
    //isEstimateNonTar = true;

    tmProbe = 0;
    GCNum = 0;
    //self3Comp = 0;
    selfComp = 0;

    //marked = false;
}

Probe::~Probe(){

    delete[] posBindingTar[0];
    delete[] posBindingTar[1];
    delete[] posBindingTar;

    delete[] posBindingNontar[0];
    delete[] posBindingNontar[1];
    delete[] posBindingNontar;
}

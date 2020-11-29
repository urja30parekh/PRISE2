#include "PrimerPair.h"


PrimerPair::PrimerPair(){
    left = "";
    right = "";
    rightRevsComp = "";
    pctgInTargetSeqs = 0;
    pctgInNonTarSeqs = 0;
    bindPosTarSeqsAvgLeft = 0;
    bindPosTarSeqsAvgRight = 0;
    pcrSizeAvgTarSeqs = 0;
    pcrSizeAvgNonTarSeqs = 0;
    //isEstimateTar = true;
    pcrSizeMinTarSeqs = 0;
    pcrSizeMinNonTarSeqs = 0;
    pcrSizeMaxTarSeqs = 0;
    pcrSizeMaxNonTarSeqs = 0;
    //isEstimateNonTar = true;

    pctgInTarSeqsLeft = 0;
    pctgInTarSeqsRight = 0;
    pctgInNonTarSeqsLeft = 0;
    pctgInNonTarSeqsRight = 0;

    tmLeft = 0;
    tmRight = 0;
    GCLeft = 0;
    GCRight = 0;
    prPrComp = 0;
    prPr3Comp = 0;
    selfCompLeft = 0;
    selfCompRight = 0;
    self3CompLeft = 0;
    self3CompRight = 0;

    marked = false;
    //Triple* a;
}


PrimerPair::~PrimerPair(){

    //short **posBindingTar, **posBindingNontar;


    delete[] posBindingTar[0];//[k];
    delete[] posBindingTar[1];//[k];
    delete[] posBindingTar;

    delete[] posBindingNontar[0];//[k];
    delete[] posBindingNontar[1];//[k];
    delete[] posBindingNontar;


    tripleList.clear();
}



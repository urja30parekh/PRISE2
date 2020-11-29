#ifndef PRIMERPAIR_H
#define PRIMERPAIR_H
#include<QString>
#include "Triple.h"
#include <QVector>
#include "Probe.h"


class PrimerPair
{
public:
    PrimerPair();
    ~PrimerPair();


    QString left;
    QString right;
    QString rightRevsComp;

    float pctgInTargetSeqs;
    float pctgInNonTarSeqs;

    float bindPosTarSeqsAvgLeft;
    float bindPosTarSeqsAvgRight;

    float pcrSizeAvgTarSeqs;
    float pcrSizeAvgNonTarSeqs;

//    bool isEstimateTar;
//    bool isEstimateNonTar;
    int pcrSizeMinTarSeqs;
    int pcrSizeMinNonTarSeqs;
    int pcrSizeMaxTarSeqs;
    int pcrSizeMaxNonTarSeqs;

    float pctgInTarSeqsLeft;
    float pctgInTarSeqsRight;
    float pctgInNonTarSeqsLeft;
    float pctgInNonTarSeqsRight;

    float tmLeft;
    float tmRight;
    float GCLeft;
    float GCRight;
    int prPrComp;
    int prPr3Comp;
    int selfCompLeft;
    int selfCompRight;
    int self3CompLeft;
    int self3CompRight;

    int numAmbiguousTargetsLeft;
    int numAmbiguousTargetsRight;
    int numAmbiguousBasesLeft;
    int numAmbiguousBasesRight;

    bool marked;

    short **posBindingTar, **posBindingNontar;
    QVector<Triple*> tripleList;
};

#endif // PRIMERPAIR_H

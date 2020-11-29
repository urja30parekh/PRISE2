#ifndef PROBE_H
#define PROBE_H
#include<QString>

class Probe
{
public:
    Probe();
    ~Probe();

    QString probeSeq;
    float pctgInTargetSeqs;
    float pctgInNonTarSeqs;
    float bindPosTarSeqsAvg[2];
    //bool isEstimateTar;
    //bool isEstimateNonTar;

    float tmProbe;
    float GCNum;
    int selfComp;
    //int self3Comp;

    int numAmbiguousTargets;
    int numAmbiguousBases;

    bool marked;

    short **posBindingTar, **posBindingNontar;
};

#endif // PROBE_H

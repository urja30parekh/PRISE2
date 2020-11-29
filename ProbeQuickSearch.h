#ifndef PROBEQUICKSEARCH_H
#define PROBEQUICKSEARCH_H
#include <QMessageBox>
#include "GlobalDef.h"
#include "PRISESetting.h"
#include "Data.h"

class ProbeQuickSearch
{
public:
    ProbeQuickSearch();
    ~ProbeQuickSearch();

    void initMember(short* pr, int prNum, short* prLen);
    int searchProbesInTarSeq(short* seq, short* revSeq);
    //int searchProbesInTarSeq2(short*);
    int searchProbesInNonTarSeq(short* seq, short* revSeq);
    //int searchProbesInNonTarSeq2(short*);

    int bindingPosStart[MAX_PROBE_LEN];
    int bindingPosEnd[MAX_PROBE_LEN];

private:

    int allocMemScoringTable();
    void cleanMemoryScoringTable(void);


    int lft_rht;
    short probe[MAX_PRIMER_LEN];
    int probesLen[MAX_PRIMER_LEN];
    int probeNum;


    // score tables for 0, 1, 2, 3 indels.
    // row for primer and column for sequence
    float ***T;
    float ***revT;
    // pointer tables for 1, 2, 3 indels
    // 0-from diagonal, 1-from left, 2-from above
    int ***P;
    int ***revP;
};

#endif // PROBEQUICKSEARCH_H

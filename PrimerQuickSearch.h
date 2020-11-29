#ifndef PRIMERQUICKSEARCH_H
#define PRIMERQUICKSEARCH_H
#include <QMessageBox>
#include "GlobalDef.h"
#include "PRISESetting.h"
#include "Data.h"

class PrimerQuickSearch
{
public:
    PrimerQuickSearch();
    ~PrimerQuickSearch();

    void initMember(short* pr, int prNum, short* prLen, int lft_rht);
    int searchPrimersInTarSeq(short*);
    int searchPrimersInNonTarSeq(short*);


    int bindingPosStart[MAX_PRIMER_LEN];
    int bindingPosEnd[MAX_PRIMER_LEN];

private:
    int allocMemScoringTable();
    void cleanMemoryScoringTable(void);

    int lft_rht;
    short primer[MAX_PRIMER_LEN];
    int primersLen[MAX_PRIMER_LEN];
    int primerNum;


    // score tables for 0, 1, 2, 3 indels.
    // row for primer and column for sequence
    float ***T;
    // pointer tables for 1, 2, 3 indels
    // 0-from diagonal, 1-from left, 2-from above
    int ***P;
};

#endif // PRIMERQUICKSEARCH_H

#ifndef PRIMERSEARCH_H
#define PRIMERSEARCH_H
#include <QVector>
#include <QMessageBox>
#include "GlobalDef.h"
#include "Data.h"

class PrimerSearch
{
public:
    short primer[MAX_PRIMER_LEN];
    int primerLen;
    QVector<int> bindingPosStartTarget;
    QVector<int> bindingPosEndTarget;
    QVector<int> bindingPosStartNonTar;
    QVector<int> bindingPosEndNonTar;

    PrimerSearch(short* pr, int lft_rht);
    ~PrimerSearch(void);
    int searchPrimerInTarSeqs(QVector<short*>, int lft_rht);
    int searchPrimerInNonTarSeqs(QVector<short*>, int lft_rht);
    int appxSearchTarSeqs(short*, int, int&, int&);
    int appxSearchNonTarSeqs(short*, int, int&, int&);
    //float minimum(float, float, float);
    //int minPointer(float, float, float);

private:
    int allocMemScoringTable();
    void cleanMemoryScoringTable(void);

    // score tables for 0, 1, 2, 3 indels.
    // row for primer and column for sequence
    float ***T;
    // pointer tables for 1, 2, 3 indels
    // 0-from diagonal, 1-from left, 2-from above
    int ***P;
};

#endif // PRIMERSEARCH_H

#ifndef SEQALIGN_H
#define SEQALIGN_H

#include <QObject>
#include <QString>
#include <QMessageBox>
#include <deque>
#include "tools.h"
#include "GlobalDef.h"
#include "PRISESetting.h"
using namespace std;

/*extern int mismatchPenalty, gapOpenPenalty, gapExtnPenalty;
extern countGapPolicy policy;*/

class SeqAlign : public QObject
{
public:
    SeqAlign(int);
    ~SeqAlign(void);

private:
    // the maximum sequence length
    int maxSeqLen;
    // score tables
    short **S;
    short **D;
    short **I;
    // trace back pointer table
    short **P;
    short score[15][15];

public:
    void initializeScore();
    float NWalign(short* A, int lenA, short* B, int lenB);
    float NWalign(short* A, int lenA, short* B, int lenB, QStringList *result);
};

#endif // SEQALIGN_H

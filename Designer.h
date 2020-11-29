#ifndef DESIGNER_H
#define DESIGNER_H

#define TTMATH_NOASM 1 // There is porblem with building in Qt Creator, with linking ttmathuint_x86_64_msvc.asm . TODO: fix

#include <ttmath/ttmath.h>
#include "time.h"
#include "Data.h"
#include "DataTools.h"
#include "gmp.h"
#include "PrimerDesignProgressDlg.h"
#include "SeqAlignSettingDlg.h"
#include "PrimerQuickSearch.h"
#include "ProbeQuickSearch.h"
#include "MessageDlg.h"
#include <assert.h>
//#include "PrimerReportWindow.h"
//#include "TripleReportWindow.h"

typedef ttmath::Big<TTMATH_BITS(64), TTMATH_BITS(128)> MyBig;
class Designer
{
public:
    Designer();
    static int startPrimerDesign();
    static int startProbeDesign();
    static int checkTriple(PrimerPair* pr);
    //static int updataPrimerInfo(PrimerPair* pair);
    static int updataPrimerInfo(QVector<PrimerPair*> pair);
    static int updataProbeInfo(QVector<Probe*> probeList);

    static int tripleInterComplementarity(QString probe, QString left, QString right);
    //static int triple3InterComplementarity(QString probe,QString left, QString right);

private:
    static PrimerDesignProgressDlg* pdProgDlg;
    static MyBig constA;
    static void getLogInfo();
    static void getProbeLogInfo();
    static int allocateMemory();


    static int readSequence(QString filename,QStringList& seqNames, QVector<short *>& seqs);
    static int readSequence(void);

    static bool areSameSeqs( short *s1, short *s2);
    static int filterSeqsOutRange(QStringList& dumpList, QStringList& seqNames, QVector<short*>& seqs, QString tarOrNontar);
    static int filterSeqSDup(QStringList& dumpList, QStringList& seqNames, QVector<short*>& seqs, QString tarOrNontar);
    static int filterSequence(void);

    static int readPrimer(QString primer, QVector <short*>& primerCandid, QVector <float>& primerBindPosAvg, int leftOrRight);
    static int readPrimer(void);
    static int readProbe(void);
    static int copySeqs(QVector <short*>& copyTo ,QVector<short*> from );
    static int pp2int(MyBig& pCode, short *myPrimer);
    static int ppHash(MyBig key);
    static int pp2int_quick(MyBig& pCode, int head, int tail, int pLength);
    static int pp2int_quick(MyBig& pCode, int tail);
    static int choosePrimerCandid(void); //structure need to be adjust.
    static int chooseProbeCandid(); //structure need to be adjust.
    static bool continuity(QString primer);
    static float GCcontent(QString primer);
    static float meltingTemp(QString primer);
    static int ppIntraComplementarity(QString pp);
    static int pp3IntraComplementarity(QString pp, int intraBase3end);
    static int ppInterComplementarity(QString p1, QString p2);
    static int pp3InterComplementarity(QString p1, QString p2, int interBase3end);

    static int primerInterComplementarity(QString left, QString right);
    static int primer3InterComplementarity(QString left, QString right);

    static int lprimerComp(short* p1, short* p2);
    static int probeComp(short* p1, short* p2);
    static int rprimerComp(short* p1, short* p2);
    static int filterPrimerCandid(void);   //structure need to be adjust.
    static int filterProbeCandid(void);   //structure need to be adjust.
    static int allocAnnealingPosVar( short ***&bindPosTarSeqs, short ***&bindPosNonTarSeqs, bool isL);
    static void freeAnnealingPosVar( short ***&bindPosTarSeqs, short ***&bindPosNonTarSeqs, bool isLeft );
    static void freeSeqs(QVector<short*>& tar,QVector<short*>& nontar);
    static void prepareReversedSeqs(QVector<short*>& reversedTarSeqs, QVector<short*>& reversedNonTarSeqs  );
    static int isPrefixLeftPrimer(short* p1, short* p2);
    static int isPrefixRightPrimer(short* p1, short* p2);
    static int isPrefixProbe(short* p1, short* p2);
    static int quickSearchPrimerInSeqs( PrimerQuickSearch* ptr ,
                                 QVector<short*> primerCandid,QVector<short*> seqs,
                                 int lft_rht, int tar_nontar,
                                 short ***&lPrBindPosTarSeqs, short ***&lPrBindPosNonTarSeqs,
                                 short ***&rPrBindPosTarSeqs, short ***&rPrBindPosNonTarSeqs,
                                 QVector<short*> &reversedTarSeqs, QVector<short*> &reversedNonTarSeqs);
    static void rand100position(int total, QVector<short> &sel);
    static int quickSearchPrimerInSeqs(void);
    static int quickSearchProbeInSeqs(void);


};



#endif // DESIGNER_H

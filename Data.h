#ifndef DATA_H
#define DATA_H

#include <QProgressDialog>
#include <QMessageBox>
#include <QtNetwork>
#include <QHash>
#include "SequenceList.h"
#include "SeqAlign.h"
#include "PrimerPair.h"
#include "Probe.h"
#include "Triple.h"


extern bool firstPrimer;
extern bool firstProbe;
extern int baseMatrix[15][5]; //one base in each row, column for ambiguous bases

/****************************************************************************************/
/*This part is for Sequence Data*/
extern SeqAlign *sAlign;
extern QStringList* queryNames;
extern QStringList* querySeqs;
extern QVector<struct QueryList*> qList;         // (DANIEL TAN): I believe 'qList' = queryList????
extern QVector<struct SequenceList*> sList;      // (DANIEL TAN): I believe 'sList' = sequenceList i.e. the data.
                                                 // This is a global variable that represents what is displayed.
extern QVector<struct SequenceList*> missingList;
extern QVector<struct SequenceList*> notDownloadList;
extern QVector<struct SequenceList*> compList;		// comparing sequence list
extern QVector<struct SequenceList*> notInSeqList;	// sequences in sequence file but not appear in sequence table
extern QVector<struct SequenceList*> notInFileList; // sequences in sequence table but not appear in sequence file
extern QStringList accTooLongSeqToDownload;
extern QStringList accTooLongSeqToAlign;
/****************************************************************************************/



/****************************************************************************************/
/*This part is for Primer calculation*/


/*
// score tables for 0, 1, 2, 3 indels.
// row for primer and column for sequence
extern float ***T;
// pointer tables for 1, 2, 3 indels
// 0-from diagonal, 1-from left, 2-from above
extern int ***P;*/


extern QVector <PrimerPair *> primerPairList;
extern QVector <PrimerPair *> primerPairShowList;

extern short ***lPrBindPosTarSeqs, ***lPrBindPosNonTarSeqs;
extern short ***rPrBindPosTarSeqs, ***rPrBindPosNonTarSeqs;

extern QVector <PrimerPair *> Primers4Probe;

extern QVector <Probe *> probeList;

//extern QVector <Triple *> tripleList;
//extern QVector <Triple *> tripleShowList;



// primer design log message
extern QString logDesign;

extern QStringList tarSeqNames;
extern QVector<short*> tarSeqs;
extern QVector<short*> tarSeqs4ProbeCandid;
extern QStringList nontarSeqNames;
extern QVector<short*> nontarSeqs;

extern QVector <short*> lPrimerCandid;
extern QVector <short*> rPrimerCandid;

extern QVector <float> lPrimerBindPosAvg;
extern QVector <float> rPrimerBindPosAvg;

extern QVector <short*> probeCandid;
extern QVector <float> probeBindPosAvg;
/****************************************************************************************/
#endif // DATA_H

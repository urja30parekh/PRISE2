#include "Data.h"


bool firstPrimer = true;
bool firstProbe = true;
int baseMatrix[15][5];	//one base in each row, column for ambiguous bases

/****************************************************************************************/
/*This part is for Sequence Data*/
SeqAlign *sAlign = new SeqAlign(MAX_ALIGN_SIZE);
QStringList* queryNames;
QStringList* querySeqs;
QVector<struct QueryList*> qList;
QVector<struct SequenceList*> sList;
QVector<struct SequenceList*> missingList;
QVector<struct SequenceList*> notDownloadList;
QVector<struct SequenceList*> compList;		// comparing sequence list
QVector<struct SequenceList*> notInSeqList;	// sequences in sequence file but not appear in sequence table
QVector<struct SequenceList*> notInFileList; // sequences in sequence table but not appear in sequence file
QStringList accTooLongSeqToDownload;
QStringList accTooLongSeqToAlign;
/****************************************************************************************/



/****************************************************************************************/
/*This part is for Primer calculation*/
/*
// score tables for 0, 1, 2, 3 indels.
// row for primer and column for sequence
float ***T;
// pointer tables for 1, 2, 3 indels
// 0-from diagonal, 1-from left, 2-from above
int ***P;*/


QVector <PrimerPair *> primerPairList;
QVector <PrimerPair *> primerPairShowList;
// binding position
short ***lPrBindPosTarSeqs, ***lPrBindPosNonTarSeqs;
short ***rPrBindPosTarSeqs, ***rPrBindPosNonTarSeqs;

QVector <PrimerPair *> Primers4Probe;

QVector <Probe *> probeList;

//QVector <Triple *> tripleList;
//QVector <Triple *> tripleShowList;

// primer design log message
QString logDesign;

QStringList tarSeqNames;
QVector<short*> tarSeqs;
QVector<short*> tarSeqs4ProbeCandid;
QStringList nontarSeqNames;
QVector<short*> nontarSeqs;

QVector <short*> lPrimerCandid;
QVector <short*> rPrimerCandid;

QVector <float> lPrimerBindPosAvg;
QVector <float> rPrimerBindPosAvg;

QVector <short*> probeCandid;
QVector <float> probeBindPosAvg;
/****************************************************************************************/

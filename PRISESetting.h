#ifndef PRISESETTING_H
#define PRISESETTING_H
#include <QString>
#include <QStringList>
#include "GlobalDef.h"

#define TTMATH_NOASM 1 // There is porblem with building in Qt Creator, with linking ttmathuint_x86_64_msvc.asm . TODO: fix

#include <ttmath/ttmath.h>

extern int numLogFiles;
extern QString pwd;
/****************************************************************************************/
/*This part is for Sequence Data*/
extern bool isSimple;
extern bool downloadAllSeqs;

//query list
extern QStringList queryName;
//alignment settings
extern int mismatchPenalty, gapOpenPenalty, gapExtnPenalty;
extern countGapPolicy policy;
extern int queryId;

extern bool markAllData;  // deal with complete data
extern bool markConstrainted;  // deal with data that satisfies constraints.
extern bool markWithinSelected;  // false: range all.              true: range selected.
extern float markIdMin;
extern float markIdMax;
extern int markLenMin;
extern int markLenMax;
extern QString markDefConstraint;
extern QString markOrgConstraint;
extern QString markAuthorConstraint;
extern QString markTitleConstraint;
extern QString markExtnStr1, markExtnStr2;
extern int markConditionalCheck[6];
extern bool markContain[4];    //false: not contain.           true: contain.

// for target and non-target sequence design: find next sequence dialog
extern float findIdMin;
extern float findIdMax;
extern int findLenMin;
extern int findLenMax;
extern QString findDefConstraint;
extern QString findOrgConstraint;
extern QString findAuthorConstraint;
extern QString findTitleConstraint;
extern QString findExtnStr1, findExtnStr2;
extern int findConditionalCheck[6];
extern bool findContain[4];    //false: not contain.           true: contain.
/****************************************************************************************/
extern int task;
/****************************************************************************************/
/*This part is for Primer calculation*/
extern QString targetSeqFilename;
extern QString nontargetSeqFilename;
extern QString dupSeqFilename;
extern QString lenOutRangeSeqFilename;
extern bool rmDupSeqs;		// 1: remove duplicated sequences. 0: dump to file.
extern bool rmLenOutRangeSeqs;	// 1: remove sequences not within range. 0: dump to file
extern bool dpDupSeqs;		// 1: remove duplicated sequences. 0: dump to file.
extern bool dpLenOutRangeSeqs;	// 1: remove sequences not within range. 0: dump to file
extern int seqLenMin;
extern int seqLenMax;


extern bool design;             //true: new Primer/Probe,  false: load previous Primer/Probe
extern bool usrLoad;
extern QString primerFilename;
//extern int primerSettingOption;	// 0 for start now, 1 for load setting, 2 for show setting
//extern bool showSimMeasure;
//extern bool showSimMeasureAdv;

extern QString probeFilename;
//extern int probeSettingOption;	// 0 for start now, 1 for load setting, 2 for show setting
//extern bool showProbeSimMeasure;
//extern bool showProbeSimMeasureAdv;

// primer settings
extern int primerLenMin;
extern int primerLenMax;
extern float primerCandidFrequency;
extern float primerCandidTopPctg;
extern int primerPcrSizeMin;
extern int primerPcrSizeMax;
extern bool primerAvoidACT;
extern bool primerAvoidG;
extern bool primerAvoidCGat3end;
extern int primerNumACT;
extern int primerNumG;
extern int primerNumCGat3end;
//extern bool primerEstimate;	// true: estimate PCR Info. false: exact PCR Info.
extern float primerTmMin;
extern float primerTmMax;
extern float primerTmDiff;
extern float primerNaConsent;
extern float primerGCMin;
extern float primerGCMax;
extern int primerMaxPrPrComp;
extern int primerMax3PrPrComp;
extern int primerInterBase3end;
extern int primerMaxSelfComp;
extern int primerMax3SelfComp;
extern int primerIntraBase3end;
extern int maxIndel;
extern float indelCost;
extern bool primerShowAdvance;

// probe settings
extern int probeLenMin;
extern int probeLenMax;
extern float probeCandidFrequency;
extern float probeCandidTopPctg;
extern int probeMinGap;

extern bool probeAvoidACT;
extern bool probeAvoidG;
extern bool probeAvoidGat5end;
extern int probeNumACT;
extern int probeNumG;
//extern int probeNumGat5end;
//extern bool probeEstimate;	// true: estimate PCR Info. false: exact PCR Info.
extern float probeTmMin;
extern float probeTmMax;
extern float probeTmGreaterMin;
extern float probeTmGreaterMax;
extern float probeNaConsent;
extern float probeGCMin;
extern float probeGCMax;
extern int maxPbPrComp;
//extern int max3PbPrComp;
//extern int tripleInterBase3end;
extern int probeMaxSelfComp;
//extern int probeMax3SelfComp;
//extern int probeIntraBase3end;
extern bool probeShowAdvance;


extern int primerBindBitmap_tar1to3;		// values 0-6: primer-sequence annealing pictures for base 0-3 on 3' end: indicate 100% to 0% chance
extern int primerBindBitmap_nontar1to3;	// same for non-target
extern int primerBindBitmap_tar4;		// values 0-2: high, medium, low. primer-sequence annealing for base 4 to 5' end.
extern int primerBindBitmap_nontar4;	// same for non-target

extern int probeBindContMatches_tar;		// values 0-15 : probe-template mismatch allowance from the middle of probe.
extern int probeBindContMatches_nontar;	// same for non-target

extern float simCost[15][15];
extern int bindingContentDivider3end;  // for primer annealing content user constraints: divider divides primer into 2 parts
extern float bindingContentMatchPctg5end;      // max mismatch percentage of 5' end
extern float bindingContentMatchPctg3end;      // max mismatch percentage of 3' end
extern int keepPairNum;




//primer report setting
extern float primerPosWtTar[MAX_PRIMER_LEN];
extern float primerPosWtNonTar[MAX_PRIMER_LEN];
extern QStringList primerColumns;
extern QVector<int> primerColumnDisp;
extern QVector<int> primerColumnHide;
extern bool primerCostType;

extern int dispPrimerCondTitle[4];			// for display primer list conditionally dialog, there are 4 conditions
extern int dispPrimerCondGtLt[4];
extern float dispPrimerCondValue[4];
extern bool dispPrimerCondAllowCancel;
extern int dispPrimerAllorTopX;
extern int dispPrimerTopNum;

extern int deletePrimerCondTitle[4];			// for delete primer pairs conditionally dialog, there are 4 conditions
extern int deletePrimerCondGtLt[4];
extern float deletePrimerCondValue[4];

extern bool primerExcelSaveColumns[A_NUM_COLUMNS];	// true for save this column, false otherwise
extern int dispPrimerSel;			// right click indicator for primer result
extern bool addPrimerMauallyReady;	// user add a primer pair manually ready or not; hit Enter or Esc

extern bool savePrimerPairs;
extern int primerListSaveAllDispMark;   // for primer list (.primer file) save: 0 for save all | 1 for save displayed | 2 for save marked
extern int primerExcelSaveAllDispMark; // for primer tab delimited file (Excel) save: 0 for save all | 1 for save displayed | 2 for save marked

extern int primerColWidth[A_NUM_COLUMNS];
extern int primerColSortOrder[A_NUM_COLUMNS]; //0: ascend    1: descend
extern int primerSortBy[4];
extern int primerSortOrder[4];



/****************************************************************************************/
//triple report setting
extern float probePosWtTar[MAX_PROBE_LEN/2+1];
extern float probePosWtNonTar[MAX_PROBE_LEN/2+1];
extern bool probeCostType;

extern QStringList tripleColumns;
extern int dispTripleCondTitle[4];			// for display probe list conditionally dialog, there are 4 conditions
extern int dispTripleCondGtLt[4];
extern float dispTripleCondValue[4];
extern bool dispTripleCondAllowCancel;
extern int dispTripleAllorTopX;
extern int dispTripleTopNum;

extern int deleteTripleCondTitle[4];			// for delete triples conditionally dialog, there are 4 conditions
extern int deleteTripleCondGtLt[4];
extern float deleteTripleCondValue[4];

extern bool tripleExcelSaveColumns[B_NUM_COLUMNS];	// true for save this column, false otherwise
extern int dispTripleSel;			// right click indicator for primer result
extern bool addTripleMauallyReady;	// user add a primer pair manually ready or not; hit Enter or Esc

extern bool saveTriple;
extern int tripleListSaveAllDispMark;   // for triple list (.probe file) save: 0 for save all | 1 for save displayed | 2 for save marked
extern int tripleExcelSaveAllDispMark; // for triple tab delimited file (Excel) save: 0 for save all | 1 for save displayed | 2 for save marked

//extern int tripleColWidth[B_NUM_COLUMNS];
//extern int tripleColSortOrder[B_NUM_COLUMNS]; //0: ascend    1: descend
extern int tripleSortBy[4];
extern int tripleSortOrder[4];

//extern int bindingContentDivider3end;  // for primer annealing content user constraints: divider divides primer into 2 parts
//extern float bindingContentMatchPctg5end;      // max mismatch percentage of 5' end
//extern float bindingContentMatchPctg3end;      // max mismatch percentage of 3' end


//If we need to clean scoring table
//extern bool allocScTab;

/****************************************************************************************/
// blast settings
extern bool blastOption;

//extern QStringList* blastQueryNames;
//extern QStringList* blastQuerySeqs;
extern QString blastBin;
extern QString blastSeedFile;
extern QString saveHitTableTo;

extern QString databasePath;
extern QString databaseName;
extern QString entrezQuery;

extern QVector<int> optionMaxTarSeq;
extern QVector<int> optionWordSize;

extern QVector<int> optionReward;
extern QVector<int> optionPenalty;

extern QVector<int> optionGapOpen;
extern QVector<int> optionGapExtend;


extern int idxMaxTarSeqs;
extern int eVal;
extern int idxWordSize;

extern int idxMatchScore;
extern int idxGap;

extern bool dustFilter;
extern bool lowercaseMask;

extern QString blastType;  // 0 : megablast, 1 : de megablast, 2: blastn;


#endif // PRISESETTING_H

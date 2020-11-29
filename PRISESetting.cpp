#include "PRISESetting.h"
#include "QVector"
#include <QDir>


int numLogFiles = 10;
QString pwd;
/****************************************************************************************/
/*This part is for Sequence Data*/
bool isSimple;
bool downloadAllSeqs = 0;

//query list
QStringList queryName;
//alignment settings
int mismatchPenalty= 0, gapOpenPenalty=2, gapExtnPenalty=1;
countGapPolicy policy = NonGap;
int queryId;

bool markAllData = false;  // deal with complete data
bool markConstrainted = false;  // deal with data that satisfies constraints.
bool markWithinSelected = false;  // false: range all.              true: range selected.
float markIdMin = 0;
float markIdMax = 0;
int markLenMin = 0;
int markLenMax = 0;
QString markDefConstraint;
QString markOrgConstraint;
QString markAuthorConstraint;
QString markTitleConstraint;
QString markExtnStr1, markExtnStr2;
int markConditionalCheck[6];
bool markContain[4] = {true, true, true, true};    //false: not contain.           true: contain.

// for target and non-target sequence design: find next sequence dialog
float findIdMin;
float findIdMax;
int findLenMin;
int findLenMax;
QString findDefConstraint;
QString findOrgConstraint;
QString findAuthorConstraint;
QString findTitleConstraint;
QString findExtnStr1, findExtnStr2;
int findConditionalCheck[6];
bool findContain[4];    //false: not contain.           true: contain.
/****************************************************************************************/
int task = 0;
/****************************************************************************************/
/*This part is for Primer calculation*/

QString targetSeqFilename;
QString nontargetSeqFilename;
QString dupSeqFilename;
QString lenOutRangeSeqFilename;
bool rmDupSeqs;		// false: remove duplicated sequences. true: dump to file.
bool rmLenOutRangeSeqs;	// false: remove sequences not within range. true: dump to file
bool dpDupSeqs;		// false: remove duplicated sequences. true: dump to file.
bool dpLenOutRangeSeqs;	// false: remove sequences not within range. true: dump to file
int seqLenMin = 60;
int seqLenMax = 4000;

bool design = true; //true: new Primer false:
bool usrLoad = false;
QString primerFilename;
//int primerSettingOption = 0;	// 0 for start now, 1 for load setting, 2 for show setting
//bool showSimMeasure = false;
//bool showSimMeasureAdv = false;

QString probeFilename;
//int probeSettingOption = 0;	// 0 for start now, 1 for load setting, 2 for show setting
//bool showProbeSimMeasure = false;
//bool showProbeSimMeasureAdv = false;

// primer settings
int primerLenMin = 14;
int primerLenMax = 22;
float primerCandidFrequency = 100;
float primerCandidTopPctg = 80.0f;
int primerPcrSizeMin = 75;
int primerPcrSizeMax = 175;
bool primerAvoidACT = true;
bool primerAvoidG = true;
bool primerAvoidCGat3end = true;
int primerNumACT = 4;
int primerNumG = 4;
int primerNumCGat3end = 3;
//bool primerEstimate = true;	// true: estimate PCR Info. false: exact PCR Info.
float primerTmMin = 48;
float primerTmMax = 58;
float primerTmDiff = 2;
float primerNaConsent = 0.1f;
float primerGCMin =25;
float primerGCMax = 75;
int primerMaxPrPrComp = 10;
int primerMax3PrPrComp = 4;
int primerInterBase3end = 8;
int primerMaxSelfComp = 10;
int primerMax3SelfComp = 4;
int primerIntraBase3end = 8;
int maxIndel = 0;
float indelCost = 5;
bool primerShowAdvance;


// probe settings
int probeLenMin = 14;
int probeLenMax = 30;
float probeCandidFrequency = 100;
float probeCandidTopPctg = 100;
int probeMinGap = 2;

bool probeAvoidACT = true;
bool probeAvoidG = true;
bool probeAvoidGat5end = true;
int probeNumACT;
int probeNumG;
//int probeNumGat5end;
//bool probeEstimate;	// true: estimate PCR Info. false: exact PCR Info.
float probeTmMin;
float probeTmMax;
float probeTmGreaterMin = 6;
float probeTmGreaterMax = 10;
float probeNaConsent;
float probeGCMin;
float probeGCMax;
int maxPbPrComp;
//int max3PbPrComp;
//int tripleInterBase3end;
int probeMaxSelfComp;
//int probeMax3SelfComp;
//int probeIntraBase3end;
bool probeShowAdvance;

int primerBindBitmap_tar1to3 = 0;		// values 0-6: primer-sequence annealing pictures for base 0-3 on 3' end: indicate 100% to 0% chance
int primerBindBitmap_nontar1to3 = 6;	// same for non-target
int primerBindBitmap_tar4 = 0;		// values 0-2: high, medium, low. primer-sequence annealing for base 4 to 5' end.
int primerBindBitmap_nontar4 = 2;	    // same for non-target

int probeBindContMatches_tar = 15;		// values 0-15: probe-sequence annealing
int probeBindContMatches_nontar = 0;	// same for non-target


int bindingContentDivider3end;  // for primer annealing content user constraints: divider divides primer into 2 parts
float bindingContentMatchPctg5end;      // max mismatch percentage of 5' end
float bindingContentMatchPctg3end;      // max mismatch percentage of 3' end

int keepPairNum = 30000;

/****************************************************************************************/
//primer report setting
float simCost[15][15];
float primerPosWtTar[MAX_PRIMER_LEN];
float primerPosWtNonTar[MAX_PRIMER_LEN];
bool primerCostType;

QStringList primerColumns;
QVector<int> primerColumnDisp;
QVector<int> primerColumnHide;
int dispPrimerCondTitle[4];			// for display primer list conditionally dialog, there are 4 conditions
int dispPrimerCondGtLt[4];
float dispPrimerCondValue[4];
bool dispPrimerCondAllowCancel;

int dispPrimerAllorTopX;
int dispPrimerTopNum;

int deletePrimerCondTitle[4];			// for delete primer pairs conditionally dialog, there are 4 conditions
int deletePrimerCondGtLt[4];
float deletePrimerCondValue[4];

bool primerExcelSaveColumns[A_NUM_COLUMNS];	// true for save this column, false otherwise
int dispPrimerSel;			// right click indicator for primer result
bool addPrimerMauallyReady;	// user add a primer pair manually ready or not; hit Enter or Esc

bool savePrimerPairs;
int primerListSaveAllDispMark;   // for primer list (.primer file) save: 0 for save all | 1 for save displayed | 2 for save marked
int primerExcelSaveAllDispMark; // for primer tab delimited file (Excel) save: 0 for save all | 1 for save displayed | 2 for save marked

int primerColWidth[A_NUM_COLUMNS];
int primerColSortOrder[A_NUM_COLUMNS]; //0: ascend    1: descend
int primerSortBy[4];
int primerSortOrder[4];

/****************************************************************************************/
//probe report setting

float probePosWtTar[MAX_PROBE_LEN/2+1];
float probePosWtNonTar[MAX_PROBE_LEN/2+1];
bool probeCostType;

QStringList tripleColumns;
int dispTripleCondTitle[4];			// for display triple list conditionally dialog, there are 4 conditions
int dispTripleCondGtLt[4];
float dispTripleCondValue[4];
bool dispTripleCondAllowCancel;

int dispTripleAllorTopX;
int dispTripleTopNum;

int deleteTripleCondTitle[4];			// for delete Triple conditionally dialog, there are 4 conditions
int deleteTripleCondGtLt[4];
float deleteTripleCondValue[4];

bool tripleExcelSaveColumns[B_NUM_COLUMNS];	// true for save this column, false otherwise
int dispTripleSel;			// right click indicator for triple result
bool addTripleMauallyReady;	// user add a triple manually ready or not; hit Enter or Esc

bool saveTriples;
int tripleListSaveAllDispMark;   // for primer list (.primer file) save: 0 for save all | 1 for save displayed | 2 for save marked
int tripleExcelSaveAllDispMark; // for primer tab delimited file (Excel) save: 0 for save all | 1 for save displayed | 2 for save marked
//int bindingContentDivider3end;  // for primer annealing content user constraints: divider divides primer into 2 parts
//float bindingContentMatchPctg5end;      // max mismatch percentage of 5' end
//float bindingContentMatchPctg3end;      // max mismatch percentage of 3' end

/*
int tripleColWidth[B_NUM_COLUMNS];
int tripleColSortOrder[B_NUM_COLUMNS]; //0: ascend    1: descend*/
int tripleSortBy[4];
int tripleSortOrder[4];


/****************************************************************************************/
// blast settings
bool blastOption;

//QStringList* blastQueryNames;
//QStringList* blastQuerySeqs;
QString blastBin;
QString blastSeedFile;
QString saveHitTableTo;

QString databasePath;
QString databaseName;
QString entrezQuery;


//QVector<int> optionMaxTarSeq( QVector<int>() << 10 << 50 << 100 << 250 << 5 );
QVector<int> optionMaxTarSeq( QVector<int>() << 10 << 50 << 100 << 250 << 500<< 1000<< 5000<< 10000<< 20000);
QVector<int> optionWordSize (QVector<int>() << 7 << 11 << 16 << 20 << 24 << 28 << 32 << 48 << 64 << 128 << 256);

QVector<int> optionReward (QVector<int>()  << 1     << 1    << 1    << 2    << 4    << 1);
QVector<int> optionPenalty (QVector<int>() << -2    << -3   << -4   << -3   << -5   << -1);

QVector<int> optionGapOpen(QVector<int>() << 0 << 5 << 2 << 1 << 0 << 3 << 2 << 1);
QVector<int> optionGapExtend(QVector<int>() << 0 << 2 << 2 << 2 << 2 << 1 << 1 << 1);


int idxMaxTarSeqs = 4;
int eVal = 10;
int idxWordSize = 3;

int idxMatchScore = 0;
int idxGap = 0;

bool dustFilter = true;
bool lowercaseMask = false;

QString blastType = "megablast";




//If we need to clean scoring table
//bool allocScTab = false;


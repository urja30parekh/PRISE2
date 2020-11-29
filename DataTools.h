#ifndef DATATOOLS_H
#define DATATOOLS_H

#include <QObject>
#include <QFile>
#include <type_traits>
#include <QProgressDialog>
#include <QMessageBox>
#include <QtNetwork>
#include <QHash>
#include <assert.h>
#include "SequenceList.h"
#include "tools.h"
#include "GlobalDef.h"
#include "MyProgressDlg.h"
#include "Data.h"
#include "PRISESetting.h"
#include "PrimerSearch.h"
#include "PrimerSortDlg.h"
#include "TripleSortDlg.h"
#include "SeqReportWindow.h"
//#include "ProbeSearch.h"
#include "PrimerReportWindow.h"
#include "QApplication"
#include <QSslSocket>
#include <cstdlib>
#include <ctime>
class PrimerReportWindow;

class DataTools: public QObject
{

public:
    DataTools();
    ~DataTools();
    static int a;
//    static bool isrev;

    static qint64 lastNCBIRequestTime;
/****************************************************************************************/
/*This part is for Sequence Data*/
public :
    static QNetworkReply::NetworkError downloadURL(QUrl url, QByteArray &data);
    static QString getSequence(QString sGetFromURL);
    static bool SearchInputAccs(QString Accs);             //Search sequence by input Accessions
    static bool SearchInputAccs(QString Accs, QStringList* qNameList, QStringList* qSeqList);             //Search sequence by input Accessions
    static bool GetInputSeedSeqs(QString SeedFile);    //Get sequence from input file.
    static bool GetInputSeedSeqs(QString SeedFile, QStringList* qNameList, QStringList* qSeqList);    //Get sequence from input file.
    static bool loadSeed(QString SeedAccs, QString SeedFile);  //load sequence
    static bool ParseHitTable(const QString &Fname);
    static bool getFastaSequences(QString FASTAFile);
    static bool UpdateIdentity();
    static void SaveQueryList();
    //void ReSearchNCBI();
    static int SearchNCBI();
    static int SearchGIinNCBI(struct SequenceList* seqPtr, int fudgeTolerance, int fudgeInclusion, int maxSeedLength);
    static int assignSeq(struct SequenceList* seqPtr, QHash<QString, QString> hashInfo);
    static void copySequenceList(struct SequenceList* dest, struct SequenceList* source);
    static bool readSeqFile(QString filename);
    static void compareSeq();
    static void loadSeqList(QString filename, QVector<struct SequenceList*>& loadList);
    static void saveSeqList(QString filename, QVector<struct SequenceList*>saveList);
    static void saveSeqList2TXT(QString filename, QVector<struct SequenceList*>saveList, QStringList headers);
    static void saveFASTASeqListAs(QString filename, QString FASTAContent);
    static void appendFASTASeqListTo(QString filename, QString FASTAContent);

    static inline void registerOnInsertData(   // The register function,
            void (*func)(struct SequenceList* seqPt)){        // take one argument, a function pointer
        _OnInsertData = func ;    // set the function
    }
    static inline void registerOnCreateModel(   // The register function,
            void (*func)()){        // take one argument, a function pointer
        _OnCreateModel = func ;    // set the function
    }

    static void cleanMemorySeqs();

    static void writeErrorLog(QString str);

private:
    static QString GetAccessionFromHitTable(QString line);
    static QString GetSeqNameFromHitTable(const QString &line);

    static QHash<QString, QString> parseSeqInfo(QString Information);
    static QString getSequence(QHash<QString, QString> hashinfo);
    static QString getSequenceFASTA(QString gi);
    static bool getSOURCEandORGANISM (QHash<QString, QString> hashinfo, QString& source, QString& organism );
    static bool getAUTHORSandTITLE (QHash<QString, QString> hashinfo, QString& authors, QString& title );
    static int getLengthInfo (QHash<QString, QString> hashinfo);
    static bool getGI (QHash<QString, QString> hashinfo, QString& gi);

    static int FindSeqWithGI(int start, QString gi);
    static void (*_OnInsertData)(struct SequenceList* seqPt) ;
    static void (*_OnCreateModel)();
/****************************************************************************************/



/****************************************************************************************/
/*This part is for Primer calculation*/
public:
    static void cleanMemoryAllData(void);
    static void cleanMemoryPrimer(MyProgressDlg* pDlg);
    static void cleanMemoryPrimerCands();
    static void cleanMemoryPrimerCands(MyProgressDlg* pDlg);
    static void cleanMemoryProbeData(void);
    static void cleanMemoryProbe();
    static void cleanMemoryProbe(MyProgressDlg* pDlg);
    static void cleanMemoryProbeCands();
    static void cleanMemoryProbeCands(MyProgressDlg* pDlg);
    static void cleanMemoryTriple();
    static void cleanMemoryTriple(PrimerPair* pr);
    static float getCostForAmbigBase(int base1, int base2, bool binaryCost);
    static void buildSimCost(bool binaryCost);
    static void defaultPrimer();
    static void defaultProbe();
    static void Initialize();
    static void setPrimerPosWtTarget(int num, ...);
    static void setPrimerPosWtNonTarget(int num, ...);
    static void setProbePosWtTarget(int num, ...);
    static void setProbePosWtNonTarget(int num, ...);
    static void setPrimerMismatchAllowance(int tarPosSel_1to3, int tarPosSel_4, int nonPosSel_1to3, int nonPosSel_4);
    static void setProbeMismatchAllowance(int contMatches_tar, int contMatches_nontar);
    static void restoreDefaultSetting();
    static int searchPrPbInNonbindingSeq(short *primer, short *sequence, int lft_rht);
    static float get3endMismatchInfo(PrimerPair *pair);
    static float getPrimerInfo(PrimerPair *pair, int col);
    static float getTripleInfo(Triple *t, int col);
    static void loadPpResult(QString filename, PrimerReportWindow* parent);
    static void loadPrimerList(QString filename, PrimerReportWindow* parent);
    static void savePrimerList(QString filename, QVector<int> pMarked);
    static void savePrimerList2TXT(QString filename, QVector<int> pMarked);

    static void saveText2File(QString filename, QString text);
    static void savePrimerPairs(QString filename, QVector<int> pMarked);

    static void sortPrimerList();
    static void sortTripleList(QVector<Triple*> &tripleShowList);
    //static void addDisplayPrimerList();
    static int deletePrimerFromPrimerMem(QString primer, QVector<PrimerPair*> &pList, bool destroyMem);
    static int deleteTripleFromTripleMem(QString probe, QVector<Triple*> &pList, bool destroyMem);

};

#endif // DATATOOLS_H

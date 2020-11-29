#ifndef TRIPLEREPORTFRAME_H
#define TRIPLEREPORTFRAME_H

#include <QFrame>
#include <QMenu>
#include <QSortFilterProxyModel>
#include <QtGui/QStandardItemModel>
#include <QDesktopServices>
#include <QMessageBox>
#include "MyProgressDlg.h"
#include "PrimerPair.h"
#include "Triple.h"
#include "Data.h"
#include "DataTools.h"
#include "TripleDispListDlg.h"
#include "ProbeItemDelegate.h"



namespace Ui {
    class TripleReportFrame;
}

class TripleReportFrame : public QFrame
{
    Q_OBJECT
    
public:
    explicit TripleReportFrame(PrimerPair* pair = 0,QWidget *parent = 0);
    ~TripleReportFrame();

    QVector<int>  tripleColumnDisp;
    QVector<int>  tripleColumnHide;


    int tripleColWidth[B_NUM_COLUMNS];
    int tripleColSortOrder[B_NUM_COLUMNS]; //0: ascend    1: descend

    //bool showFrame;

private:
    Ui::TripleReportFrame *ui;
    QStandardItemModel *tableModel;
    QVector<int> markedList;
    QVector<Triple *> tripleShowList;
    int dispTripleSel;			// right click indicator for primer result
    PrimerPair *pr;

public slots:
    void emptyModel();
    bool close();

    int getTripleListSize();
    int getTripleShowListSize();
    int getMarkedNum();

    void getMarkedList();
    void displayData();
    //void setTableColumnWidth(int column, int width);
    //void setTableColumnHidden(int column, bool hidden);
    QString getInfoText();
    QString getPrimerSeq();
    PrimerPair* getPrimerPair();
    void addTripleShowList( Triple* tr, int pos);


    void loadTripleList(QString filename);
    void saveTripleList(QString filename);
    void saveTripleList2TXT(QString filename);
    void saveTriples(QString filename);

    void displayAllColumn();
    void hideDispColumns();
    void hideDispTriples();

    void sortTripleList();

    void addTripleManually();
    void deleteTriplesConditionally();
    void deleteMarkedTriples();
    void deleteSeletedTriples();

    void markSelectedTriples();
    void unmarkSelectedTriples();

    void dispCompleteInfo();
    void reportBindingContent();
    void targetWTriple();
    void targetWoTriple();
    void nonTargetWTriple();
    void nonTargetWoTriple();

    void probeIntraComp();
    //void probe3IntraComp();
    void tripleInterComp();
    //void triple3InterComp();

    void showPrimerPairInfo();
    void blastProbe();

private slots:
    bool createModel();

    void copyDispTripleList();
    bool checkTripleWithCondition(Triple *tp, int *condTitle, int *condGtLt, float *condValue);
    void sortData(int idx);
    void insertData(int dataIdx, int rowIdx = -1);

    void insertShownTripleList();
    void appendSeq2Rich(QString seqName, short *seqInt, int *pos);

    int getCurrSelectedItem();
    void deleteTriple();

    void click(const QModelIndex & idx);
    void rightClick (const QPoint & point);
    void showRightClickMenu ();





};

#endif // TRIPLEREPORTFRAME_H

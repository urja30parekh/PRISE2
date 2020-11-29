#ifndef SEQREPORTWINDOW_H
#define SEQREPORTWINDOW_H


#include <algorithm>
#include "sorting.h" //sorting function for data.
#include <QDesktopServices>
#include <QCloseEvent>
#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QtGui/QStandardItemModel>
#include "LoadSeedHitTableDlg.h"
#include "SeqAlignSettingDlg.h"
#include "SeqMarkUnmarkDlg.h"
#include "SeqDiffWindow.h"
#include "SeqAlignShowWindow.h"
#include "SeqAlign.h"
#include "MessageDlg.h"
#include "PRISESetting.h"
#include "SeqFindDlg.h"
#include "Data.h"
#include "DataTools.h"
#include "prise.h"
#include "seqreportmodel.h"

namespace Ui {
    class SeqReportWindow;
}
class SeqReportWindow : public QMainWindow {

    Q_OBJECT
public:
    SeqReportWindow(QWidget *parent = Q_NULLPTR);
    ~SeqReportWindow();
    void emptyModel();
    void cleanModel();
    bool createModel();
    friend void OnInsertData(struct SequenceList* seqPt);
protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);

private:
    void showRightClickMenu ();
    void withinAllData(bool selectedOnly, QVector<int> selectedSeqList, bool SetMarked);
    void withinConstraintedData(bool selectedOnly, QVector<int> selectedSeqList, bool setMarked);
    bool contains(QString text, QString pattern);
    bool checkOrdeOK();
    QString removeDuplicateSpaces(QString description);

    Ui::SeqReportWindow *ui;
    int clickedRow;
    QVector<bool> sortOrder; //true: ascend    false: descend
    SeqReportModel *report_model;

private slots:
    void on_actionPRISE_Tuturial_triggered();
    void on_actionPRISE_Manual_triggered();
    void on_actionExit_triggered();
    void on_actionDisplay_Sequences_not_in_GenBank_File_triggered();
    void on_actionDisplay_Sequences_not_in_Sequence_List_triggered();
    void on_actionLoad_GenBank_gb_File_to_Be_compared_triggered();
    void on_actionFind_Next_triggered();
    void on_actionFind_Sequence_triggered();
    void on_actionClear_FASTA_Sequence_Box_triggered();
    void on_actionDelete_Selected_Sequences_triggered();
    void on_actionDelete_Marked_Sequences_triggered();
    void on_actionUpdate_Identity_for_all_Sequences_triggered();
    void on_actionChange_Sequence_Alignment_Setting_triggered();
    void on_actionMove_Marked_Sequences_to_FASTA_Sequence_Box_triggered();
    void on_actionReverse_Marked_and_Unmark_triggered();
    void on_actionUnmark_Sequences_triggered();
    void on_actionMark_Sequences_triggered();
    void on_actionAdd_FASTA_Sequences_to_An_Existing_File_triggered();
    void on_actionSave_FASTA_Sequences_as_triggered();
    void on_actionSave_Seed_Sequence_List_as_Tab_Delimited_File_Excel_File_triggered();
    void on_actionLoad_Seed_Sequence_List_sequence_File_triggered();
    void on_actionSave_Seed_Sequence_List_sequence_File_triggered();
    void on_actionLoad_Seed_Sequence_and_Hit_Table_triggered();
    void rightClick (const QPoint & point);
    void diaplayAlignment();
    void instantBlast();

    void insertShownSeqList();
};

#endif // SEQREPORTWINDOW_H

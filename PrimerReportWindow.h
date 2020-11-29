#ifndef PRIMERREPORTWINDOW_H
#define PRIMERREPORTWINDOW_H

#include <QMainWindow>
//#include <QSortFilterProxyModel>
#include <QtGui/QStandardItemModel>
#include <QFileDialog>
#include <algorithm>
#include <assert.h>
#include <QStringList>
#include "sorting.h" //sorting function for data.
#include <QDesktopServices>
#include <QCloseEvent>
#include <prise.h>
#include "GlobalDef.h"
#include "MyProgressDlg.h"
#include "Data.h"
#include "PrimerDispListDlg.h"
#include "DataTools.h"
#include "PRISESetting.h"
#include "PrimerSaveListDlg.h"
#include "PrimerSaveExcelDlg.h"
#include "PrimerColumnDlg.h"
#include "PrimerDeleteCondDlg.h"
#include "PrimerSettingReportDlg.h"
#include "AnnealInfoDlg.h"
#include "DesignWizard.h"
#include "Designer.h"
#include "TripleReportWindow.h"
#include "PrimerItemDelegate.h"


//#include "TripleReportWindow.h"
//#include "PrimerSearch.h"

class TripleReportWindow;
namespace Ui {
    class PrimerReportWindow;
}


class PrimerReportWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PrimerReportWindow(QWidget *parent = 0);
    ~PrimerReportWindow();
    bool createModel();
    void displayData();
    void emptyModel();

    bool checkPrimerWithCondition(PrimerPair *pair, int *condTitle, int *condGtLt, float *condValue);
    void copyDispPrimerList();
    //void insertData();
    void insertData(int dataIdx, int rowIdx = -1);
    //void cleanMemory();
    void displayAllColumn();
    void getMarkedList();

    QStandardItemModel *tableModel;
    QVector<int> markedList;
    bool showWindow;

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::PrimerReportWindow *ui;
    int dispPrimerSel;			// right click indicator for primer result
    TripleReportWindow *tpWindow;

private slots:
    void on_actionPRISE_Tutorial_triggered();
    void on_actionPRISE_Manual_triggered();
    void on_actionBLAST_Reverse_Primer_triggered();
    void on_actionBLAST_Forward_Primer_triggered();
    void on_actionDesign_Probes_for_marked_primer_pairs_triggered();
    void on_actionView_Primer_Design_Setting_triggered();
    void on_actionPrimer_3_Intra_Complementarity_triggered();
    void on_actionPrimer_Intra_Complementarity_triggered();
    void on_actionPrimer_3_Inter_Complementarity_triggered();
    void on_actionPrimer_Inter_Complementarity_triggered();
    //void on_actionUpdate_Estimated_Information_to_Exact_Information_triggered();
    void on_actionNon_Target_Sequences_Annealing_with_Primer_triggered();
    void on_actionNon_Target_Sequences_Not_Annealing_with_Primer_triggered();
    void on_actionTarget_Sequences_Not_Annealing_with_Primer_triggered();
    void on_actionTarget_Sequences_Annealing_with_Primer_triggered();
    void on_actionPercentage_of_Each_Nucleotide_in_Target_and_Non_Target_Sequences_in_Relation_to_Primer_Sequences_triggered();
    void on_actionAnnealing_Info_triggered();
    void on_actionUnmark_Selected_Primer_Pairs_triggered();
    void on_actionMark_Selected_Primer_Pairs_triggered();
    void on_actionDelete_Seleted_Primer_Pairs_triggered();
    void on_actionDelete_Marked_Primer_Pairs_triggered();
    void on_actionDelete_Primer_Pairs_Conditionally_triggered();
    void on_actionAdd_Primer_Pair_Manually_triggered();
    //void checkInputPrimer(QStandardItem *item);
    void on_actionSort_Primer_List_triggered();
    void on_actionHide_Display_Primer_Pairs_triggered();
    void on_actionHide_Display_Columns_triggered();
    void on_actionDisplay_All_Columns_triggered();
    void on_actionExit_triggered();
    void on_actionSave_Primer_Pairs_Only_triggered();
    void on_actionSave_Primer_Information_Window_Content_triggered();
    void on_actionSave_Primer_List_as_Tab_Delimited_File_Excel_triggered();
    void on_actionSave_Primer_List_primer_File_triggered();
    void on_actionLoad_Primer_List_primer_triggered();
    void click(const QModelIndex & idx);
    void rightClick (const QPoint & point);

    void dispCompleteInfo();
    void reportBindingContent();
    void targetWPrimer();
    void targetWoPrimer();
    void nonTargetWPrimer();
    void nonTargetWoPrimer();
    //void getExactPCRinfo();
    void primerInterComp();
    void primer3InterComp();
    void primerIntraComp();
    void primer3IntraComp();
    void sortData(int idx);
    void blastForwardPrimer();
    void blastReversePrimer();
    void designProbes4markedPrimerPairs();
    void designProbes4markedPrimerPairs(QVector<int> primers4ProbeDesign);
    void deletePrimerPair();
    void on_actionCompare_with_PP_data_triggered();
    //void testSlot(int x);


    //void on_actionProbe_3_Intra_Complementarity_triggered();


private:
    //int searchPrimerInNonbindingSeq(short *primer, short *sequence, int lft_rht);
    void appendSeq2Rich(QString seqName, short *seqInt, int *pos);
    void insertShownPrimerList();
    void markselectedprimerpairs();
    void unmarkselectedprimerpairs();
    void showRightClickMenu ();
};

#endif // PRIMERREPORTWINDOW_H

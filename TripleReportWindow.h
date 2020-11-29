#ifndef TripleReportWindow_H
#define TripleReportWindow_H

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QtGui/QStandardItemModel>
#include <QFileDialog>
#include "GlobalDef.h"
#include "MyProgressDlg.h"
#include "Data.h"
#include "TripleDispListDlg.h"
#include "DataTools.h"
#include "PRISESetting.h"
#include "TripleSaveListDlg.h"
#include "TripleSaveExcelDlg.h"
#include "TripleColumnDlg.h"
#include "TripleDeleteCondDlg.h"
#include "ProbeSettingReportDlg.h"
#include "AnnealInfoDlg.h"

namespace Ui {
    class TripleReportWindow;
}

class TripleReportWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TripleReportWindow(QWidget *parent = 0);
    ~TripleReportWindow();
protected:
    void closeEvent(QCloseEvent *event);
public slots:
    //bool close();
    void addTab(QWidget *widget, QString tabLabel);

private:
    Ui::TripleReportWindow *ui;

private slots:
    void closeTab(int tabIdx);

    //void on_actionLoad_Triple_List_Triple_File_triggered();
    void on_actionSave_Triple_List_Triple_File_triggered();
    void on_actionSave_Triple_List_as_Tab_Delimited_File_Excel_triggered();
    void on_actionSave_Triple_Information_Window_Content_triggered();
    void on_actionSave_Triples_Only_triggered();
    void on_actionExit_triggered();

    void on_actionDisplay_all_columns_triggered();
    void on_actionHide_Display_columns_triggered();
    void on_actionHide_Display_triples_triggered();

    void on_actionSort_Triple_List_triggered();

    void on_actionAdd_Triple_Manually_triggered();
    void on_actionDelete_Triples_Conditionally_triggered();
    void on_actionDelete_Marked_Triples_triggered();
    void on_actionDelete_Seleted_Triples_triggered();

    void on_actionMark_Selected_Triples_triggered();
    void on_actionUnmark_Selected_Triples_triggered();

    void on_actionTriple_Annealing_Position_Information_triggered();
    void on_actionPercentage_of_Each_Nucleotide_in_Target_and_Non_Target_Sequences_in_Relation_to_Triple_Sequences_triggered();
    void on_actionTarget_Sequences_Annealing_with_Triple_triggered();
    void on_actionTarget_Sequences_Not_Annealing_with_Triple_triggered();
    void on_actionNon_target_Sequences_Annealing_with_Triple_triggered();
    void on_actionNon_target_Sequences_Not_Annealing_with_Triple_triggered();

    void on_actionProbe_Intra_Complementarity_triggered();
    //void on_actionProbe_3_Intra_Complementarity_triggered();
    void on_actionTriple_Inter_Complementarity_triggered();
    //void on_actionTriple_3_Inter_Complementarity_triggered();

    void on_actionShow_PrimerPair_Info_triggered();

    void on_actionView_Probe_Design_Setting_triggered();

    void on_actionBlast_Probe_triggered();

    void on_actionPRISE_Manual_triggered();
    void on_actionPRISE_Tutorial_triggered();
};

#endif // TripleReportWindow_H










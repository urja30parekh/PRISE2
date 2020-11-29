#ifndef SEQDIFFWINDOW_H
#define SEQDIFFWINDOW_H

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QtGui/QStandardItemModel>
#include <QFileDialog>
#include "PRISESetting.h"
#include "SeqAlignSettingDlg.h"
#include "MyProgressDlg.h"
#include "MessageDlg.h"
#include "Data.h"
#include "DataTools.h"

namespace Ui {
    class SeqDiffWindow;
}

class SeqDiffWindow : public QMainWindow {
    Q_OBJECT
public:
    SeqDiffWindow(QVector<struct SequenceList*> showList, QWidget *parent = 0);
    ~SeqDiffWindow();
    void initialize();
    void emptyModel();
    void cleanModel();
    bool createModel();
    void insertData(struct SequenceList* seqPt, int pos);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SeqDiffWindow *ui;
    QStandardItemModel *tableModel;
    QVector<struct SequenceList*> seqShowList;

private slots:
    void on_actionPRISE_Tutorial_triggered();
    void on_actionPRISE_Manual_triggered();
    void on_actionUpdate_Identity_for_All_Sequences_triggered();
    void on_actionExit_triggered();
    void on_actionSave_Seed_Sequence_List_as_Tab_Delimited_File_Excel_File_triggered();
    void on_actionSave_Sequence_List_sequence_file_triggered();
};

#endif // SEQDIFFWINDOW_H

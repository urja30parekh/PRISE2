#ifndef PRIMERSAVEEXCELDLG_H
#define PRIMERSAVEEXCELDLG_H

#include <QDialog>
#include<QtAlgorithms>
#include "PrimerReportWindow.h"

namespace Ui {
    class PrimerSaveExcelDlg;
}

class PrimerSaveExcelDlg : public QDialog
{
    Q_OBJECT

public:
    explicit PrimerSaveExcelDlg(QWidget *parent);
    ~PrimerSaveExcelDlg();

public slots:
    void accept();
    void addColumn();
    void deleteColumn();

private:
    Ui::PrimerSaveExcelDlg *ui;
    int nMarkedPrimerPairs;
};

#endif // PRIMERSAVEEXCELDLG_H

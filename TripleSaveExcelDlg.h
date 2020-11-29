#ifndef TRIPLESAVEEXCELDlg_H
#define TRIPLESAVEEXCELDlg_H

#include <QDialog>
#include<QtAlgorithms>
#include "TripleReportFrame.h"

namespace Ui {
    class TripleSaveExcelDlg;
}

class TripleSaveExcelDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TripleSaveExcelDlg(QWidget* parent);
    ~TripleSaveExcelDlg();

public slots:
    void accept();
    void addColumn();
    void deleteColumn();

private:
    Ui::TripleSaveExcelDlg *ui;
    int nMarkedTriples;
};

#endif // TRIPLESAVEEXCELDlg_H

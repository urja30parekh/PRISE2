#ifndef TripleSaveListDlg_H
#define TripleSaveListDlg_H

#include <QDialog>
#include <QMessageBox>
#include "Data.h"
#include "TripleReportWindow.h"

namespace Ui {
    class TripleSaveListDlg;
}

class TripleSaveListDlg : public QDialog
{
    Q_OBJECT

public:
    //explicit TripleSaveListDlg(QWidget *parent = 0);
    TripleSaveListDlg(/*bool savePrimerPairs = false ,*/QWidget *parent);
    ~TripleSaveListDlg();
    void accept();

private:
    bool saveProbes;
    int nMarkedTriples;
    Ui::TripleSaveListDlg *ui;
};

#endif // TripleSaveListDlg_H

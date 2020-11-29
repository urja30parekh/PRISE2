#ifndef PRIMERSAVELISTDLG_H
#define PRIMERSAVELISTDLG_H

#include <QDialog>
#include <QMessageBox>
#include "Data.h"
#include "PrimerReportWindow.h"

namespace Ui {
    class PrimerSaveListDlg;
}

class PrimerSaveListDlg : public QDialog
{
    Q_OBJECT

public:
    //explicit PrimerSaveListDlg(QWidget *parent = 0);
    PrimerSaveListDlg(/*bool savePrimerPairs = false ,*/QWidget *parent);
    ~PrimerSaveListDlg();
    void accept();

private:
    bool savePrimerPairs;
    int nMarkedPrimerPairs;
    Ui::PrimerSaveListDlg *ui;
};

#endif // PRIMERSAVELISTDLG_H

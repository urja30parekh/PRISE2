#ifndef PRIMERDISPLISTDLG_H
#define PRIMERDISPLISTDLG_H

#include <QDialog>
#include <QPushButton>
#include "GlobalDef.h"
#include "PrimerReportWindow.h"

namespace Ui {
    class PrimerDispListDlg;
}

class PrimerDispListDlg : public QDialog
{
    Q_OBJECT

public:
    explicit PrimerDispListDlg(QWidget *parent);
    ~PrimerDispListDlg();
public slots:
    void accept();
    void reject();
    void changeIdx_1(int i);
    void changeIdx_2(int i);
    void changeIdx_3(int i);
    void changeIdx_4(int i);
    void displayAll();
    void displayTop();

private:
    Ui::PrimerDispListDlg *ui;

};

#endif // PRIMERDISPLISTDLG_H

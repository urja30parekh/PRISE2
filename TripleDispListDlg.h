#ifndef TripleDispListDlg_H
#define TripleDispListDlg_H

#include <QDialog>
#include <QPushButton>
#include "GlobalDef.h"
#include "Triple.h"
#include "Data.h"
#include "TripleReportFrame.h"

namespace Ui {
    class TripleDispListDlg;
}

class TripleDispListDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TripleDispListDlg(QWidget *parent);
    ~TripleDispListDlg();
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
    Ui::TripleDispListDlg *ui;

};

#endif // TripleDispListDlg_H

#ifndef PRIMERDELETECONDDLG_H
#define PRIMERDELETECONDDLG_H

#include <QDialog>
#include <QMessageBox>
#include "GlobalDef.h"
#include "PRISESetting.h"

namespace Ui {
    class PrimerDeleteCondDlg;
}

class PrimerDeleteCondDlg : public QDialog
{
    Q_OBJECT

public:
    explicit PrimerDeleteCondDlg(QWidget *parent);
    ~PrimerDeleteCondDlg();

private:
    Ui::PrimerDeleteCondDlg *ui;

public slots:
    void accept();
    void changeIdx_1(int i);
    void changeIdx_2(int i);
    void changeIdx_3(int i);
    void changeIdx_4(int i);
};

#endif // PRIMERDELETECONDDLG_H

#ifndef PRIMERCOLUMNDLG_H
#define PRIMERCOLUMNDLG_H

#include <QDialog>
#include "PRISESetting.h"

namespace Ui {
    class PrimerColumnDlg;
}

class PrimerColumnDlg : public QDialog
{
    Q_OBJECT

public:
    explicit PrimerColumnDlg(QWidget *parent);
    ~PrimerColumnDlg();

public slots:
    void accept();
    void dispColumn();
    void hideColumn();

private:
    Ui::PrimerColumnDlg *ui;
};

#endif // PRIMERCOLUMNDLG_H

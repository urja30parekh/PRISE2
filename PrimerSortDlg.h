#ifndef PRIMERSORTDLG_H
#define PRIMERSORTDLG_H

#include <QDialog>
#include "GlobalDef.h"
#include "PRISESetting.h"

namespace Ui {
    class PrimerSortDlg;
}

class PrimerSortDlg : public QDialog
{
    Q_OBJECT

public:
    explicit PrimerSortDlg(QWidget *parent);
    ~PrimerSortDlg();
public slots:
    void accept();

private:
    Ui::PrimerSortDlg *ui;
};

#endif // PRIMERSORTDLG_H

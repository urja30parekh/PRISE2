#ifndef TripleSortDlg_H
#define TripleSortDlg_H

#include <QDialog>
#include "GlobalDef.h"
#include "PRISESetting.h"
#include "TripleReportFrame.h"

namespace Ui {
    class TripleSortDlg;
}

class TripleSortDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TripleSortDlg(QWidget *parent);
    ~TripleSortDlg();
public slots:
    void accept();

private:
    Ui::TripleSortDlg *ui;
};

#endif // TripleSortDlg_H

#ifndef TripleColumnDlg_H
#define TripleColumnDlg_H

#include <QDialog>
#include "PRISESetting.h"
#include "TripleReportFrame.h"

namespace Ui {
    class TripleColumnDlg;
}

class TripleColumnDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TripleColumnDlg(QWidget *parent);
    ~TripleColumnDlg();

public slots:
    void accept();
    void dispColumn();
    void hideColumn();

private:
    Ui::TripleColumnDlg *ui;
    QVector<int>* cDisp;
    QVector<int>* cHide;
};

#endif // TripleColumnDlg_H

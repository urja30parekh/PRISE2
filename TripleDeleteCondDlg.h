#ifndef TripleDeleteCondDlg_H
#define TripleDeleteCondDlg_H

#include <QDialog>
#include <QMessageBox>
#include "GlobalDef.h"
#include "PRISESetting.h"

namespace Ui {
    class TripleDeleteCondDlg;
}

class TripleDeleteCondDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TripleDeleteCondDlg(QWidget *parent);
    ~TripleDeleteCondDlg();

private:
    Ui::TripleDeleteCondDlg *ui;

public slots:
    void accept();
    void changeIdx_1(int i);
    void changeIdx_2(int i);
    void changeIdx_3(int i);
    void changeIdx_4(int i);
};

#endif // TripleDeleteCondDlg_H

#ifndef SEQFINDDLG_H
#define SEQFINDDLG_H

#include <QDialog>
#include "PRISESetting.h"
#include "tools.h"

namespace Ui {
    class SeqFindDlg;
}

class SeqFindDlg : public QDialog {
    Q_OBJECT
public:
    SeqFindDlg(QWidget *parent = 0);
    ~SeqFindDlg();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SeqFindDlg *ui;
private slots:
    void accept();
};

#endif // SEQFINDDLG_H

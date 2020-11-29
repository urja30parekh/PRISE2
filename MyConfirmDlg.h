#ifndef MYCONFIRMDLG_H
#define MYCONFIRMDLG_H

#include <QDialog>

namespace Ui {
    class MyConfirmDlg;
}

class MyConfirmDlg : public QDialog {
    Q_OBJECT
public:
    MyConfirmDlg(QString labelText, QWidget *parent = 0);
    ~MyConfirmDlg();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MyConfirmDlg *ui;

};

#endif // MYCONFIRMDLG_H

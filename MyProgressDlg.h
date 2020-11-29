#ifndef MYPROGRESSDLG_H
#define MYPROGRESSDLG_H

#include <QProgressDialog>
#include "MyConfirmDlg.h"

class MyProgressDlg : public QProgressDialog
{
    Q_OBJECT
public:
    MyProgressDlg(QWidget *parent = 0);
    MyProgressDlg(QString labelText, int minimum, int maximum, QWidget *parent = 0);
    MyProgressDlg(QString labelText, QWidget *parent = 0);

signals:

private slots:
    void confirmCancel();

};

#endif // MYPROGRESSDLG_H

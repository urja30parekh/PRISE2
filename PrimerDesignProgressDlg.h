#ifndef PRIMERDESIGNPROGRESSDLG_H
#define PRIMERDESIGNPROGRESSDLG_H

#include <QProgressDialog>
#include "MyConfirmDlg.h"
#include "Data.h"

class PrimerDesignProgressDlg : public QProgressDialog
{
    Q_OBJECT
public:
    explicit PrimerDesignProgressDlg(QWidget *parent = 0);
    void computation(int part);

signals:

public slots:

private slots:
    void confirmCancel();
};

#endif // PRIMERDESIGNPROGRESSDLG_H

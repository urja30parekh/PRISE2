#ifndef PROBEDESIGNPROGRESSDLG_H
#define PROBEDESIGNPROGRESSDLG_H
#include <QProgressDialog>
#include "MyConfirmDlg.h"
#include "Data.h"

class ProbeDesignProgressDlg : public QProgressDialog
{
   Q_OBJECT
public:
    ProbeDesignProgressDlg();    
    void computation(int part);

private slots:
    void confirmCancel();
};

#endif // PROBEDESIGNPROGRESSDLG_H

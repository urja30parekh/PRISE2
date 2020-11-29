#include "MyProgressDlg.h"

MyProgressDlg::MyProgressDlg(QWidget *parent) :
    QProgressDialog(parent)
{
}

MyProgressDlg::MyProgressDlg(QString labelText, int minimum, int maximum, QWidget *parent ) :
    QProgressDialog(parent)
{
    disconnect(this, SIGNAL(canceled()),this, SLOT(cancel()));
    connect(this, SIGNAL(canceled()),this, SLOT(confirmCancel()));
    this->setLabelText(labelText);
    this->setRange(minimum, maximum);
    this->setValue(0);
    this->setWindowModality(Qt::ApplicationModal);
#ifdef MAC
    p->resize(width()+50, height()+100);
#endif


}


MyProgressDlg::MyProgressDlg(QString labelText,  QWidget *parent ) :
    QProgressDialog(parent)
{
    this->setLabelText(labelText);
    this->setValue(0);
    MyProgressDlg::cancel();
}

void MyProgressDlg::confirmCancel()
{
    MyConfirmDlg cDlg("Do you really want to terminate the computation?\n\n\n\n"
                                          "Click OK to terminate the program, Cancel to resume",this);
    if(cDlg.exec()){
        QProgressDialog::cancel();
        return;
    }
}

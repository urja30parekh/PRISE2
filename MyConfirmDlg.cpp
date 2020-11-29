#include "MyConfirmDlg.h"
#include "ui_MyConfirmDlg.h"

MyConfirmDlg::MyConfirmDlg(QString labelText, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyConfirmDlg)
{
    ui->setupUi(this);
    ui->label->setText(labelText);

#ifdef MAC
    p->resize(width()+50, height()+100);
#endif

    this->activateWindow();
}

MyConfirmDlg::~MyConfirmDlg()
{
    delete ui;
}

void MyConfirmDlg::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

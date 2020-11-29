#include "SeqAlignSettingDlg.h"
#include "ui_SeqAlignSettingDlg.h"

SeqAlignSettingDlg::SeqAlignSettingDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SeqAlignSettingDlg)
{
    ui->setupUi(this);
    ui->lineEdit_mismatch->setText( QString("%1").arg(mismatchPenalty));
    ui->lineEdit_openGap->setText(QString("%1").arg(gapOpenPenalty));
    ui->lineEdit_extenGap->setText(QString("%1").arg(gapExtnPenalty));

    switch(policy){
    case NonGap:
        ui->radioButton_countNon->setChecked(true);
        break;
    case Internal:
        ui->radioButton_countInternal->setChecked(true);
        break;
    case Internal_End:
        ui->radioButton_countBoth->setChecked(true);
        break;
    }
}

SeqAlignSettingDlg::~SeqAlignSettingDlg()
{
    delete ui;
}

/*void SeqAlignSettingDlg::getAlignSetting(int& panlity_mismatch,
                                         int& panlity_gapopen,
                                         int& panlity_gapextn,
                                         countGapPolicy& policy)
{
    panlity_mismatch = mismatch;
    panlity_gapopen = gapopen;
    panlity_gapextn = gapextn;
    policy=CGPolicy;
}*/


void SeqAlignSettingDlg::accept()
{
    bool bMismatch, bOpenGap, bExtenGap;
    mismatchPenalty = ui->lineEdit_mismatch->text().toInt(&bMismatch);
    gapOpenPenalty = ui->lineEdit_openGap->text().toInt(&bOpenGap);
    gapExtnPenalty = ui->lineEdit_extenGap->text().toInt(&bExtenGap);
    int iPolicy = -1;

     QAbstractButton * checked = ui->buttonGroup->checkedButton();
    if( checked == ui->radioButton_countInternal)
        iPolicy = Internal;
    else if (checked == ui->radioButton_countBoth )
        iPolicy = Internal_End;
    else if (checked == ui->radioButton_countNon)
        iPolicy = NonGap;

    if (!bMismatch ||
        !bOpenGap ||
        !bExtenGap  ||
       policy == -1)
    {
        QMessageBox::about(this, tr("parameters imcomplete"), tr("Please check if all parameters are inputed "));
        return;
    }

    policy = (countGapPolicy)iPolicy;
    QDialog::accept();
}


void SeqAlignSettingDlg::reset()
{
    ui->lineEdit_mismatch->setText("0");
    ui->lineEdit_openGap->setText("2");
    ui->lineEdit_extenGap->setText("1");

    ui->radioButton_countInternal->setChecked(true);
}


void SeqAlignSettingDlg::changeEvent(QEvent *e)
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

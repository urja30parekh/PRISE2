#include "AnnealInfoDlg.h"
#include "ui_AnnealInfoDlg.h"

AnnealInfoDlg::AnnealInfoDlg(int *divider3end, float *match5end, float *match3end, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnnealInfoDlg)
{
    ui->setupUi(this);
    bindingContentDivider3end = divider3end;
    bindingContentMatchPctg5end = match5end;
    bindingContentMatchPctg3end = match3end;

    ui->label->setText("Settings for the percentage of each nucleotide in non-target sequences in relation to "
                                                    "the primer sequences\n\nNote that these primer annealing settings "
                                                    "should be less stringent than those used for primer design");

    ui->horizontalSlider->setValue(*bindingContentDivider3end );

    ui->lineEdit_match5end->setText(QString("%1").arg(*bindingContentMatchPctg5end));
    ui->lineEdit_match3end->setText(QString("%1").arg(*bindingContentMatchPctg3end));
}

AnnealInfoDlg::~AnnealInfoDlg()
{
    delete ui;
}

void AnnealInfoDlg::accept()
{
    *bindingContentDivider3end = ui->horizontalSlider->value();

    bool b;
    float match5end = ui->lineEdit_match5end->text().toFloat(&b);
    if(!b){
        QMessageBox::about(this,"Condition Error","Please input an integer percentage\n");
        return;
    }
    *bindingContentMatchPctg5end = match5end;

    float match3end = ui->lineEdit_match3end->text().toFloat(&b);
    if(!b){
        QMessageBox::about(this,"Condition Error","Please input an integer percentage\n");
        return;
    }
    *bindingContentMatchPctg3end = match3end;
    QDialog::accept();
}

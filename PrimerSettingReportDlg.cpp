#include "PrimerSettingReportDlg.h"
#include "ui_PrimerSettingReportDlg.h"

PrimerSettingReportDlg::PrimerSettingReportDlg(bool enable, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrimerSettingReportDlg)
{
    ui->setupUi(this);
    this->setEnabled(enable);

    //ui->radioButton_estimated->setChecked(primerEstimate);
    //ui->radioButton_exact->setChecked(!primerEstimate);

    ui->lineEdit_primerLenMin->setText(QString("%1").arg(primerLenMin));
    ui->lineEdit_primerLenMax->setText(QString("%1").arg(primerLenMax));
    ui->lineEdit_candidFrequency->setText(QString("%1").arg(primerCandidFrequency));
    ui->lineEdit_candidTopPctg->setText(QString("%1").arg(primerCandidTopPctg));
    ui->lineEdit_pcrSizeMin->setText(QString("%1").arg(primerPcrSizeMin));
    ui->lineEdit_pcrSizeMax->setText(QString("%1").arg(primerPcrSizeMax));

    ui->checkBox_avoidACT->setChecked(primerAvoidACT);
    ui->checkBox_avoidG->setChecked(primerAvoidG);
    ui->checkBox_avoidCGat3->setChecked(primerAvoidCGat3end);
    ui->lineEdit_numACT->setText(QString("%1").arg(primerNumACT));
    ui->lineEdit_numG->setText(QString("%1").arg(primerNumG));
    ui->lineEdit_numCGat3end->setText(QString("%1").arg(primerNumCGat3end));
    ui->lineEdit_numACT->setEnabled(primerAvoidACT );
    ui->lineEdit_numG->setEnabled(primerAvoidG);
    ui->lineEdit_numCGat3end->setEnabled(primerAvoidCGat3end);

    ui->lineEdit_tmMin->setText(QString("%1").arg(primerTmMin));
    ui->lineEdit_tmMax->setText(QString("%1").arg(primerTmMax));
    ui->lineEdit_tmDiff->setText(QString("%1").arg(primerTmDiff));
    ui->lineEdit_NaConsent->setText(QString("%1").arg(primerNaConsent));
    ui->lineEdit_GCMin->setText(QString("%1").arg(primerGCMin));
    ui->lineEdit_GCMax->setText(QString("%1").arg(primerGCMax));
    ui->lineEdit_maxPrPrComp->setText(QString("%1").arg(primerMaxPrPrComp));
    ui->lineEdit_max3PrPrComp->setText(QString("%1").arg(primerMax3PrPrComp));
    ui->lineEdit_interBase3end->setText(QString("%1").arg(primerInterBase3end));
    ui->lineEdit_maxSelfComp->setText(QString("%1").arg(primerMaxSelfComp));
    ui->lineEdit_max3SelfComp->setText(QString("%1").arg(primerMax3SelfComp));
    ui->lineEdit_intraBase3end->setText(QString("%1").arg(primerIntraBase3end));
    ui->lineEdit_cost->setText(QString("%1").arg(indelCost));

    ui->comboBox_maxGap->setCurrentIndex(maxIndel );

    // primerCostType
    // false: regular
    // true: binary
    ui->radioButton_regular->setChecked(!primerCostType);
    ui->radioButton_binary->setChecked(primerCostType);

    ui->lineEdit_AA->setText("0");
    ui->lineEdit_CC->setText("0");
    ui->lineEdit_GG->setText("0");
    ui->lineEdit_TT->setText("0");

    ui->lineEdit_CA->setText(QString("%1").arg(simCost[BASE_C][BASE_A]));
    ui->lineEdit_GA->setText(QString("%1").arg(simCost[BASE_G][BASE_A]));
    ui->lineEdit_TA->setText(QString("%1").arg(simCost[BASE_T][BASE_A]));
    ui->lineEdit_GC->setText(QString("%1").arg(simCost[BASE_G][BASE_C]));
    ui->lineEdit_TC->setText(QString("%1").arg(simCost[BASE_T][BASE_C]));
    ui->lineEdit_TG->setText(QString("%1").arg(simCost[BASE_T][BASE_G]));

    ui->lineEdit_1->setText(QString("%1").arg(primerPosWtTar[0]));
    ui->lineEdit_2->setText(QString("%1").arg(primerPosWtTar[1]));
    ui->lineEdit_3->setText(QString("%1").arg(primerPosWtTar[2]));
    ui->lineEdit_4->setText(QString("%1").arg(primerPosWtTar[3]));
    ui->lineEdit_5->setText(QString("%1").arg(primerPosWtTar[4]));
    ui->lineEdit_6->setText(QString("%1").arg(primerPosWtTar[5]));
    ui->lineEdit_7->setText(QString("%1").arg(primerPosWtTar[6]));
    ui->lineEdit_8->setText(QString("%1").arg(primerPosWtTar[7]));
    ui->lineEdit_9->setText(QString("%1").arg(primerPosWtTar[8]));
    ui->lineEdit_10->setText(QString("%1").arg(primerPosWtTar[9]));
    ui->lineEdit_11->setText(QString("%1").arg(primerPosWtTar[10]));
    ui->lineEdit_12->setText(QString("%1").arg(primerPosWtTar[11]));
    ui->lineEdit_13->setText(QString("%1").arg(primerPosWtTar[12]));
    ui->lineEdit_14->setText(QString("%1").arg(primerPosWtTar[13]));
    ui->lineEdit_15->setText(QString("%1").arg(primerPosWtTar[14]));
    ui->lineEdit_16->setText(QString("%1").arg(primerPosWtTar[15]));
    ui->lineEdit_17->setText(QString("%1").arg(primerPosWtTar[16]));
    ui->lineEdit_18->setText(QString("%1").arg(primerPosWtTar[17]));
    ui->lineEdit_19->setText(QString("%1").arg(primerPosWtTar[18]));
    ui->lineEdit_20->setText(QString("%1").arg(primerPosWtTar[19]));
    ui->lineEdit_21->setText(QString("%1").arg(primerPosWtTar[20]));
    ui->lineEdit_22->setText(QString("%1").arg(primerPosWtTar[21]));
    ui->lineEdit_23->setText(QString("%1").arg(primerPosWtTar[22]));
    ui->lineEdit_24->setText(QString("%1").arg(primerPosWtTar[23]));
    ui->lineEdit_25->setText(QString("%1").arg(primerPosWtTar[24]));
    ui->lineEdit_26->setText(QString("%1").arg(primerPosWtTar[25]));
    ui->lineEdit_27->setText(QString("%1").arg(primerPosWtTar[26]));
    ui->lineEdit_28->setText(QString("%1").arg(primerPosWtTar[27]));
    ui->lineEdit_29->setText(QString("%1").arg(primerPosWtTar[28]));
    ui->lineEdit_30->setText(QString("%1").arg(primerPosWtTar[29]));

    ui->lineEdit_n1->setText(QString("%1").arg(primerPosWtNonTar[0]));
    ui->lineEdit_n2->setText(QString("%1").arg(primerPosWtNonTar[1]));
    ui->lineEdit_n3->setText(QString("%1").arg(primerPosWtNonTar[2]));
    ui->lineEdit_n4->setText(QString("%1").arg(primerPosWtNonTar[3]));
    ui->lineEdit_n5->setText(QString("%1").arg(primerPosWtNonTar[4]));
    ui->lineEdit_n6->setText(QString("%1").arg(primerPosWtNonTar[5]));
    ui->lineEdit_n7->setText(QString("%1").arg(primerPosWtNonTar[6]));
    ui->lineEdit_n8->setText(QString("%1").arg(primerPosWtNonTar[7]));
    ui->lineEdit_n9->setText(QString("%1").arg(primerPosWtNonTar[8]));
    ui->lineEdit_n10->setText(QString("%1").arg(primerPosWtNonTar[9]));
    ui->lineEdit_n11->setText(QString("%1").arg(primerPosWtNonTar[10]));
    ui->lineEdit_n12->setText(QString("%1").arg(primerPosWtNonTar[11]));
    ui->lineEdit_n13->setText(QString("%1").arg(primerPosWtNonTar[12]));
    ui->lineEdit_n14->setText(QString("%1").arg(primerPosWtNonTar[13]));
    ui->lineEdit_n15->setText(QString("%1").arg(primerPosWtNonTar[14]));
    ui->lineEdit_n16->setText(QString("%1").arg(primerPosWtNonTar[15]));
    ui->lineEdit_n17->setText(QString("%1").arg(primerPosWtNonTar[16]));
    ui->lineEdit_n18->setText(QString("%1").arg(primerPosWtNonTar[17]));
    ui->lineEdit_n19->setText(QString("%1").arg(primerPosWtNonTar[18]));
    ui->lineEdit_n20->setText(QString("%1").arg(primerPosWtNonTar[19]));
    ui->lineEdit_n21->setText(QString("%1").arg(primerPosWtNonTar[20]));
    ui->lineEdit_n22->setText(QString("%1").arg(primerPosWtNonTar[21]));
    ui->lineEdit_n23->setText(QString("%1").arg(primerPosWtNonTar[22]));
    ui->lineEdit_n24->setText(QString("%1").arg(primerPosWtNonTar[23]));
    ui->lineEdit_n25->setText(QString("%1").arg(primerPosWtNonTar[24]));
    ui->lineEdit_n26->setText(QString("%1").arg(primerPosWtNonTar[25]));
    ui->lineEdit_n27->setText(QString("%1").arg(primerPosWtNonTar[26]));
    ui->lineEdit_n28->setText(QString("%1").arg(primerPosWtNonTar[27]));
    ui->lineEdit_n29->setText(QString("%1").arg(primerPosWtNonTar[28]));
    ui->lineEdit_n30->setText(QString("%1").arg(primerPosWtNonTar[29]));
}

PrimerSettingReportDlg::~PrimerSettingReportDlg()
{
    delete ui;
}

void PrimerSettingReportDlg::changeEvent(QEvent *e)
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

#include "ProbeSettingReportDlg.h"
#include "ui_ProbeSettingReportDlg.h"

ProbeSettingReportDlg::ProbeSettingReportDlg(bool enable, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProbeSettingReportDlg)
{
    ui->setupUi(this);
    this->setEnabled(enable);

    //ui->radioButton_estimated->setChecked(probeEstimate);
    //ui->radioButton_exact->setChecked(!probeEstimate);

    ui->lineEdit_probeLenMin->setText(QString("%1").arg(probeLenMin));
    ui->lineEdit_probeLenMax->setText(QString("%1").arg(probeLenMax));
    ui->lineEdit_candidFrequency->setText(QString("%1").arg(probeCandidFrequency));
    ui->lineEdit_candidTopPctg->setText(QString("%1").arg(probeCandidTopPctg));
    ui->lineEdit_minGap->setText(QString("%1").arg(probeMinGap));

    ui->checkBox_avoidACT->setChecked(probeAvoidACT);
    ui->checkBox_avoidG->setChecked(probeAvoidG);
    ui->checkBox_avoidGat5end->setChecked(probeAvoidGat5end);
    ui->lineEdit_numACT->setText(QString("%1").arg(probeNumACT));
    ui->lineEdit_numG->setText(QString("%1").arg(probeNumG));
    //ui->lineEdit_numGat5end->setText(QString("%1").arg(probeNumGat5end));
    ui->lineEdit_numACT->setEnabled(probeAvoidACT );
    ui->lineEdit_numG->setEnabled(probeAvoidG);
    //ui->lineEdit_numGat5end->setEnabled(probeAvoidGat5end);
    //ui->lineEdit_pcrSizeMin->setText(QString("%1").arg(primerPcrSizeMin));
    //ui->lineEdit_pcrSizeMax->setText(QString("%1").arg(primerPcrSizeMax));
    ui->lineEdit_tmMin->setText(QString("%1").arg(probeTmMin));
    ui->lineEdit_tmMax->setText(QString("%1").arg(probeTmMax));
    ui->lineEdit_tmDiffMin->setText(QString("%1").arg(probeTmGreaterMin));
    ui->lineEdit_tmDiffMax->setText(QString("%1").arg(probeTmGreaterMax));
    ui->lineEdit_NaConsent->setText(QString("%1").arg(probeNaConsent));
    ui->lineEdit_GCMin->setText(QString("%1").arg(probeGCMin));
    ui->lineEdit_GCMax->setText(QString("%1").arg(probeGCMax));
    ui->lineEdit_maxPbPrComp->setText(QString("%1").arg(maxPbPrComp));
    //ui->lineEdit_max3PbPrComp->setText(QString("%1").arg(max3PbPrComp));
    //ui->lineEdit_interBase3end->setText(QString("%1").arg(tripleInterBase3end));
    ui->lineEdit_maxSelfComp->setText(QString("%1").arg(probeMaxSelfComp));
    //ui->lineEdit_max3SelfComp->setText(QString("%1").arg(probeMax3SelfComp));
    //ui->lineEdit_intraBase3end->setText(QString("%1").arg(probeIntraBase3end));
    ui->lineEdit_cost->setText(QString("%1").arg(indelCost));

    ui->comboBox_maxGap->setCurrentIndex(maxIndel );

    // probeCostType
    // false: regular
    // true: binary
    ui->radioButton_regular->setChecked(!probeCostType);
    ui->radioButton_binary->setChecked(probeCostType);

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

    ui->lineEdit_1->setText(QString("%1").arg(probePosWtTar[14]));
    ui->lineEdit_2->setText(QString("%1").arg(probePosWtTar[13]));
    ui->lineEdit_3->setText(QString("%1").arg(probePosWtTar[12]));
    ui->lineEdit_4->setText(QString("%1").arg(probePosWtTar[11]));
    ui->lineEdit_5->setText(QString("%1").arg(probePosWtTar[10]));
    ui->lineEdit_6->setText(QString("%1").arg(probePosWtTar[9]));
    ui->lineEdit_7->setText(QString("%1").arg(probePosWtTar[8]));
    ui->lineEdit_8->setText(QString("%1").arg(probePosWtTar[7]));
    ui->lineEdit_9->setText(QString("%1").arg(probePosWtTar[6]));
    ui->lineEdit_10->setText(QString("%1").arg(probePosWtTar[5]));
    ui->lineEdit_11->setText(QString("%1").arg(probePosWtTar[4]));
    ui->lineEdit_12->setText(QString("%1").arg(probePosWtTar[3]));
    ui->lineEdit_13->setText(QString("%1").arg(probePosWtTar[2]));
    ui->lineEdit_14->setText(QString("%1").arg(probePosWtTar[1]));
    ui->lineEdit_15->setText(QString("%1").arg(probePosWtTar[0]));
    ui->lineEdit_16->setText(QString("%1").arg(probePosWtTar[0]));
    ui->lineEdit_17->setText(QString("%1").arg(probePosWtTar[1]));
    ui->lineEdit_18->setText(QString("%1").arg(probePosWtTar[2]));
    ui->lineEdit_19->setText(QString("%1").arg(probePosWtTar[3]));
    ui->lineEdit_20->setText(QString("%1").arg(probePosWtTar[4]));
    ui->lineEdit_21->setText(QString("%1").arg(probePosWtTar[5]));
    ui->lineEdit_22->setText(QString("%1").arg(probePosWtTar[6]));
    ui->lineEdit_23->setText(QString("%1").arg(probePosWtTar[7]));
    ui->lineEdit_24->setText(QString("%1").arg(probePosWtTar[8]));
    ui->lineEdit_25->setText(QString("%1").arg(probePosWtTar[9]));
    ui->lineEdit_26->setText(QString("%1").arg(probePosWtTar[10]));
    ui->lineEdit_27->setText(QString("%1").arg(probePosWtTar[11]));
    ui->lineEdit_28->setText(QString("%1").arg(probePosWtTar[12]));
    ui->lineEdit_29->setText(QString("%1").arg(probePosWtTar[13]));
    ui->lineEdit_30->setText(QString("%1").arg(probePosWtTar[14]));

    ui->lineEdit_n1->setText(QString("%1").arg(probePosWtNonTar[14]));
    ui->lineEdit_n2->setText(QString("%1").arg(probePosWtNonTar[13]));
    ui->lineEdit_n3->setText(QString("%1").arg(probePosWtNonTar[12]));
    ui->lineEdit_n4->setText(QString("%1").arg(probePosWtNonTar[11]));
    ui->lineEdit_n5->setText(QString("%1").arg(probePosWtNonTar[10]));
    ui->lineEdit_n6->setText(QString("%1").arg(probePosWtNonTar[9]));
    ui->lineEdit_n7->setText(QString("%1").arg(probePosWtNonTar[8]));
    ui->lineEdit_n8->setText(QString("%1").arg(probePosWtNonTar[7]));
    ui->lineEdit_n9->setText(QString("%1").arg(probePosWtNonTar[6]));
    ui->lineEdit_n10->setText(QString("%1").arg(probePosWtNonTar[5]));
    ui->lineEdit_n11->setText(QString("%1").arg(probePosWtNonTar[4]));
    ui->lineEdit_n12->setText(QString("%1").arg(probePosWtNonTar[3]));
    ui->lineEdit_n13->setText(QString("%1").arg(probePosWtNonTar[2]));
    ui->lineEdit_n14->setText(QString("%1").arg(probePosWtNonTar[1]));
    ui->lineEdit_n15->setText(QString("%1").arg(probePosWtNonTar[0]));
    ui->lineEdit_n16->setText(QString("%1").arg(probePosWtNonTar[0]));
    ui->lineEdit_n17->setText(QString("%1").arg(probePosWtNonTar[1]));
    ui->lineEdit_n18->setText(QString("%1").arg(probePosWtNonTar[2]));
    ui->lineEdit_n19->setText(QString("%1").arg(probePosWtNonTar[3]));
    ui->lineEdit_n20->setText(QString("%1").arg(probePosWtNonTar[4]));
    ui->lineEdit_n21->setText(QString("%1").arg(probePosWtNonTar[5]));
    ui->lineEdit_n22->setText(QString("%1").arg(probePosWtNonTar[6]));
    ui->lineEdit_n23->setText(QString("%1").arg(probePosWtNonTar[7]));
    ui->lineEdit_n24->setText(QString("%1").arg(probePosWtNonTar[8]));
    ui->lineEdit_n25->setText(QString("%1").arg(probePosWtNonTar[9]));
    ui->lineEdit_n26->setText(QString("%1").arg(probePosWtNonTar[10]));
    ui->lineEdit_n27->setText(QString("%1").arg(probePosWtNonTar[11]));
    ui->lineEdit_n28->setText(QString("%1").arg(probePosWtNonTar[12]));
    ui->lineEdit_n29->setText(QString("%1").arg(probePosWtNonTar[13]));
    ui->lineEdit_n30->setText(QString("%1").arg(probePosWtNonTar[14]));
}

ProbeSettingReportDlg::~ProbeSettingReportDlg()
{
    delete ui;
}

void ProbeSettingReportDlg::changeEvent(QEvent *e)
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

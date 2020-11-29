#include "ProbeSettingSlctvtAdvIIPage.h"
#include "ui_ProbeSettingSlctvtAdvIIPage.h"
#include "DataTools.h"
#include <QDesktopWidget>

ProbeSettingSlctvtAdvIIPage::ProbeSettingSlctvtAdvIIPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ProbeSettingSlctvtAdvIIPage)
{
    ui->setupUi(this);
}

ProbeSettingSlctvtAdvIIPage::~ProbeSettingSlctvtAdvIIPage()
{
    delete ui;
}

void ProbeSettingSlctvtAdvIIPage::initializePage()
{
#ifdef MAC
    this->resize(885, 600);
#else
    this->resize(885, 550);
#endif

    QWizard* p = this->wizard();
    prevWidth = p->width();
    prevHeight = p->height();
#ifdef MAC
    p->resize(width()+200, height()+600);
#else
    p->resize(width()+200, height()+200);
#endif

    QDesktopWidget *desktop = QApplication::desktop();
    p->move((desktop->width() - p->width()) / 2, 0);

    ui->comboBox_maxGap->setCurrentIndex(maxIndel);
    ui->lineEdit_cost->setText(QString("%1").arg(indelCost));

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

    // probeCostType
    // false: regular
    // true: binary
    ui->radioButton_regular->setChecked(!probeCostType);
    ui->radioButton_binary->setChecked(probeCostType);

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

void ProbeSettingSlctvtAdvIIPage::cleanupPage()
{
    QWizard* p = wizard();
    p->resize(prevWidth, prevHeight);
}

int ProbeSettingSlctvtAdvIIPage::nextId() const
{
    return -1;
}

void ProbeSettingSlctvtAdvIIPage::changeEvent(QEvent *e)
{
    QWizardPage::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ProbeSettingSlctvtAdvIIPage::reset()
{
    DataTools::setProbeMismatchAllowance(probeBindContMatches_tar, probeBindContMatches_nontar );
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

/*
void ProbeSettingSlctvtAdvIIPage::onEditTar_1()
{
    QString strA, strB;
    strA = ui->lineEdit_1->text();
    strB = ui->lineEdit_2->text();

    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_2->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_2()
{
    QString strA, strB;
    strA = ui->lineEdit_2->text();
    strB = ui->lineEdit_3->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_3->setText(strA);

    strB = ui->lineEdit_1->text();
    A = strA.toInt(&b);
    B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_1->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_3()
{
    QString strA, strB;
    strA = ui->lineEdit_3->text();
    strB = ui->lineEdit_4->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_4->setText(strA);

    strB = ui->lineEdit_2->text();
    A = strA.toInt(&b);
    B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_2->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_4()
{
    QString strA, strB;
    strA = ui->lineEdit_4->text();
    strB = ui->lineEdit_5->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_5->setText(strA);

    strB = ui->lineEdit_3->text();
    A = strA.toInt(&b);
    B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_3->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_5()
{
    QString strA, strB;
    strA = ui->lineEdit_5->text();
    strB = ui->lineEdit_6->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_6->setText(strA);

    strB = ui->lineEdit_4->text();
    A = strA.toInt(&b);
    B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_4->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_6()
{
    QString strA, strB;
    strA = ui->lineEdit_6->text();
    strB = ui->lineEdit_7->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_7->setText(strA);

    strB = ui->lineEdit_5->text();
    A = strA.toInt(&b);
    B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_5->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_7()
{
    QString strA, strB;
    strA = ui->lineEdit_7->text();
    strB = ui->lineEdit_8->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_8->setText(strA);

    strB = ui->lineEdit_6->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_6->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_8()
{
    QString strA, strB;
    strA = ui->lineEdit_8->text();
    strB = ui->lineEdit_9->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_9->setText(strA);

    strB = ui->lineEdit_7->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_7->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_9()
{
    QString strA, strB;
    strA = ui->lineEdit_9->text();
    strB = ui->lineEdit_10->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_10->setText(strA);

    strB = ui->lineEdit_8->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_8->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_10()
{
    QString strA, strB;
    strA = ui->lineEdit_10->text();
    strB = ui->lineEdit_11->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_11->setText(strA);

    strB = ui->lineEdit_9->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_9->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_11()
{
    QString strA, strB;
    strA = ui->lineEdit_11->text();
    strB = ui->lineEdit_12->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_12->setText(strA);

    strB = ui->lineEdit_10->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_10->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_12()
{
    QString strA, strB;
    strA = ui->lineEdit_12->text();
    strB = ui->lineEdit_13->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_13->setText(strA);

    strB = ui->lineEdit_11->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_11->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_13()
{
    QString strA, strB;
    strA = ui->lineEdit_13->text();
    strB = ui->lineEdit_14->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_14->setText(strA);

    strB = ui->lineEdit_12->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_12->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_14()
{
    QString strA, strB;
    strA = ui->lineEdit_14->text();
    strB = ui->lineEdit_15->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_15->setText(strA);

    strB = ui->lineEdit_13->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_13->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_15()
{
    QString strA, strB;
    strA = ui->lineEdit_15->text();
    strB = ui->lineEdit_16->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_16->setText(strA);

    strB = ui->lineEdit_14->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_14->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_16()
{
    QString strA, strB;
    strA = ui->lineEdit_16->text();
    strB = ui->lineEdit_17->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_17->setText(strA);

    strB = ui->lineEdit_15->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_15->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_17()
{
    QString strA, strB;
    strA = ui->lineEdit_17->text();
    strB = ui->lineEdit_18->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_18->setText(strA);

    strB = ui->lineEdit_16->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_16->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_18()
{
    QString strA, strB;
    strA = ui->lineEdit_18->text();
    strB = ui->lineEdit_19->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_19->setText(strA);

    strB = ui->lineEdit_17->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_17->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_19()
{
    QString strA, strB;
    strA = ui->lineEdit_19->text();
    strB = ui->lineEdit_20->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_20->setText(strA);

    strB = ui->lineEdit_18->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_18->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_20()
{
    QString strA, strB;
    strA = ui->lineEdit_20->text();
    strB = ui->lineEdit_21->text();

    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_21->setText(strA);

    strB = ui->lineEdit_19->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_19->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_21()
{
    QString strA, strB;
    strA = ui->lineEdit_21->text();
    strB = ui->lineEdit_22->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_22->setText(strA);

    strB = ui->lineEdit_20->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_20->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_22()
{
    QString strA, strB;
    strA = ui->lineEdit_22->text();
    strB = ui->lineEdit_23->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_23->setText(strA);

    strB = ui->lineEdit_21->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_21->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_23()
{
    QString strA, strB;
    strA = ui->lineEdit_23->text();
    strB = ui->lineEdit_24->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_24->setText(strA);

    strB = ui->lineEdit_22->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_22->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_24()
{
    QString strA, strB;
    strA = ui->lineEdit_24->text();
    strB = ui->lineEdit_25->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_25->setText(strA);

    strB = ui->lineEdit_23->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_23->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_25()
{
    QString strA, strB;
    strA = ui->lineEdit_25->text();
    strB = ui->lineEdit_26->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_26->setText(strA);

    strB = ui->lineEdit_24->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_24->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_26()
{
    QString strA, strB;
    strA = ui->lineEdit_26->text();
    strB = ui->lineEdit_27->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_27->setText(strA);

    strB = ui->lineEdit_25->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_25->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_27()
{
    QString strA, strB;
    strA = ui->lineEdit_27->text();
    strB = ui->lineEdit_28->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_28->setText(strA);

    strB = ui->lineEdit_26->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_26->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_28()
{
    QString strA, strB;
    strA = ui->lineEdit_28->text();
    strB = ui->lineEdit_29->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_29->setText(strA);

    strB = ui->lineEdit_27->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_27->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_29()
{
    QString strA, strB;
    strA = ui->lineEdit_29->text();
    strB = ui->lineEdit_30->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_30->setText(strA);

    strB = ui->lineEdit_28->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_28->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_30()
{
    QString strA, strB;
    strA = ui->lineEdit_30->text();
    strB = ui->lineEdit_29->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_29->setText(strA);
}

void ProbeSettingSlctvtAdvIIPage::onEditNonTar_1()
{
    QString strA, strB;
    strA = ui->lineEdit_n1->text();
    strB = ui->lineEdit_n2->text();

    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n2->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_2()
{
    QString strA, strB;
    strA = ui->lineEdit_n2->text();
    strB = ui->lineEdit_n3->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n3->setText(strA);

    strB = ui->lineEdit_n1->text();
    A = strA.toInt(&b);
    B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n1->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_3()
{
    QString strA, strB;
    strA = ui->lineEdit_n3->text();
    strB = ui->lineEdit_n4->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n4->setText(strA);

    strB = ui->lineEdit_n2->text();
    A = strA.toInt(&b);
    B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n2->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_4()
{
    QString strA, strB;
    strA = ui->lineEdit_n4->text();
    strB = ui->lineEdit_n5->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n5->setText(strA);

    strB = ui->lineEdit_n3->text();
    A = strA.toInt(&b);
    B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n3->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_5()
{
    QString strA, strB;
    strA = ui->lineEdit_n5->text();
    strB = ui->lineEdit_n6->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n6->setText(strA);

    strB = ui->lineEdit_n4->text();
    A = strA.toInt(&b);
    B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n4->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_6()
{
    QString strA, strB;
    strA = ui->lineEdit_n6->text();
    strB = ui->lineEdit_n7->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n7->setText(strA);

    strB = ui->lineEdit_n5->text();
    A = strA.toInt(&b);
    B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n5->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_7()
{
    QString strA, strB;
    strA = ui->lineEdit_n7->text();
    strB = ui->lineEdit_n8->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n8->setText(strA);

    strB = ui->lineEdit_n6->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n6->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_8()
{
    QString strA, strB;
    strA = ui->lineEdit_n8->text();
    strB = ui->lineEdit_n9->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n9->setText(strA);

    strB = ui->lineEdit_n7->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n7->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_9()
{
    QString strA, strB;
    strA = ui->lineEdit_n9->text();
    strB = ui->lineEdit_n10->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n10->setText(strA);

    strB = ui->lineEdit_n8->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n8->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_10()
{
    QString strA, strB;
    strA = ui->lineEdit_n10->text();
    strB = ui->lineEdit_n11->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n11->setText(strA);

    strB = ui->lineEdit_n9->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n9->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_11()
{
    QString strA, strB;
    strA = ui->lineEdit_n11->text();
    strB = ui->lineEdit_n12->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n12->setText(strA);

    strB = ui->lineEdit_n10->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n10->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_12()
{
    QString strA, strB;
    strA = ui->lineEdit_n12->text();
    strB = ui->lineEdit_n13->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n13->setText(strA);

    strB = ui->lineEdit_n11->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n11->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_13()
{
    QString strA, strB;
    strA = ui->lineEdit_n13->text();
    strB = ui->lineEdit_n14->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n14->setText(strA);

    strB = ui->lineEdit_n12->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n12->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_14()
{
    QString strA, strB;
    strA = ui->lineEdit_n14->text();
    strB = ui->lineEdit_n15->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n15->setText(strA);

    strB = ui->lineEdit_n13->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n13->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_15()
{
    QString strA, strB;
    strA = ui->lineEdit_n15->text();
    strB = ui->lineEdit_n16->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n16->setText(strA);

    strB = ui->lineEdit_n14->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n14->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_16()
{
    QString strA, strB;
    strA = ui->lineEdit_n16->text();
    strB = ui->lineEdit_n17->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n17->setText(strA);

    strB = ui->lineEdit_n15->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n15->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_17()
{
    QString strA, strB;
    strA = ui->lineEdit_n17->text();
    strB = ui->lineEdit_n18->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n18->setText(strA);

    strB = ui->lineEdit_n16->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n16->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_18()
{
    QString strA, strB;
    strA = ui->lineEdit_n18->text();
    strB = ui->lineEdit_n19->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n19->setText(strA);

    strB = ui->lineEdit_n17->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n17->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_19()
{
    QString strA, strB;
    strA = ui->lineEdit_n19->text();
    strB = ui->lineEdit_n20->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n20->setText(strA);

    strB = ui->lineEdit_n18->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n18->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_20()
{
    QString strA, strB;
    strA = ui->lineEdit_n20->text();
    strB = ui->lineEdit_n21->text();

    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n21->setText(strA);

    strB = ui->lineEdit_n19->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n19->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_21()
{
    QString strA, strB;
    strA = ui->lineEdit_n21->text();
    strB = ui->lineEdit_n22->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n22->setText(strA);

    strB = ui->lineEdit_n20->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n20->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_22()
{
    QString strA, strB;
    strA = ui->lineEdit_n22->text();
    strB = ui->lineEdit_n23->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n23->setText(strA);

    strB = ui->lineEdit_n21->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n21->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_23()
{
    QString strA, strB;
    strA = ui->lineEdit_n23->text();
    strB = ui->lineEdit_n24->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n24->setText(strA);

    strB = ui->lineEdit_n22->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n22->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_24()
{
    QString strA, strB;
    strA = ui->lineEdit_n24->text();
    strB = ui->lineEdit_n25->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n25->setText(strA);

    strB = ui->lineEdit_n23->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n23->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_25()
{
    QString strA, strB;
    strA = ui->lineEdit_n25->text();
    strB = ui->lineEdit_n26->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n26->setText(strA);

    strB = ui->lineEdit_n24->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n24->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_26()
{
    QString strA, strB;
    strA = ui->lineEdit_n26->text();
    strB = ui->lineEdit_n27->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n27->setText(strA);

    strB = ui->lineEdit_n25->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n25->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_27()
{
    QString strA, strB;
    strA = ui->lineEdit_n27->text();
    strB = ui->lineEdit_n28->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n28->setText(strA);

    strB = ui->lineEdit_n26->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n26->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_28()
{
    QString strA, strB;
    strA = ui->lineEdit_n28->text();
    strB = ui->lineEdit_n29->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n29->setText(strA);

    strB = ui->lineEdit_n27->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n27->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_29()
{
    QString strA, strB;
    strA = ui->lineEdit_n29->text();
    strB = ui->lineEdit_n30->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( A > B )
        ui->lineEdit_n30->setText(strA);

    strB = ui->lineEdit_n28->text();
     A = strA.toInt(&b);
     B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n28->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_30()
{
    QString strA, strB;
    strA = ui->lineEdit_n30->text();
    strB = ui->lineEdit_n29->text();
    bool b;
    int A = strA.toInt(&b);
    int B = strB.toInt(&b);
    if( strA.length() > 0 && A < B )
        ui->lineEdit_n29->setText(strA);
}*/


void ProbeSettingSlctvtAdvIIPage::onEditTar_1()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_1   ->text();
    ui->lineEdit_30->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_1->setText("");
            ui->lineEdit_30->setText("");
        }
        return;
    }

    strB = ui->lineEdit_2->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_2->setText(strA);


}
void ProbeSettingSlctvtAdvIIPage::onEditTar_2()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_2->text();
    ui->lineEdit_29->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_2->setText("");
            ui->lineEdit_29->setText("");
        }
        return;
    }

    strB = ui->lineEdit_1->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_1->setText(strA);

    strB = ui->lineEdit_3->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_3->setText(strA);
}

void ProbeSettingSlctvtAdvIIPage::onEditTar_3()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_3->text();
    ui->lineEdit_28->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_3->setText("");
            ui->lineEdit_28->setText("");
        }
        return;
    }

    strB = ui->lineEdit_2->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_2->setText(strA);

    strB = ui->lineEdit_4->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_4->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_4()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_4->text();
    ui->lineEdit_27->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_4->setText("");
            ui->lineEdit_27->setText("");
        }
        return;
    }

    strB = ui->lineEdit_3->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_3->setText(strA);

    strB = ui->lineEdit_5->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_5->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_5()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_5->text();
    ui->lineEdit_26->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_5->setText("");
            ui->lineEdit_26->setText("");
        }
        return;
    }

    strB = ui->lineEdit_4->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_4->setText(strA);

    strB = ui->lineEdit_6->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_6->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_6()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_6->text();
    ui->lineEdit_25->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_6->setText("");
            ui->lineEdit_25->setText("");
        }
        return;
    }

    strB = ui->lineEdit_5->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_5->setText(strA);

    strB = ui->lineEdit_7->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_7->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_7()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_7->text();
    ui->lineEdit_24->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_7->setText("");
            ui->lineEdit_24->setText("");
        }
        return;
    }

    strB = ui->lineEdit_6->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_6->setText(strA);

    strB = ui->lineEdit_8->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_8->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_8()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_8->text();
    ui->lineEdit_23->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_8->setText("");
            ui->lineEdit_23->setText("");
        }
        return;
    }

    strB = ui->lineEdit_7->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_7->setText(strA);

    strB = ui->lineEdit_9->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_9->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_9()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_9->text();
    ui->lineEdit_22->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_9->setText("");
            ui->lineEdit_22->setText("");
        }
        return;
    }

    strB = ui->lineEdit_8->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_8->setText(strA);

    strB = ui->lineEdit_10->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_10->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_10()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_10->text();
    ui->lineEdit_21->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_10->setText("");
            ui->lineEdit_21->setText("");
        }
        return;
    }

    strB = ui->lineEdit_9->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_9->setText(strA);

    strB = ui->lineEdit_11->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_11->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_11()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_11->text();
    ui->lineEdit_20->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_11->setText("");
            ui->lineEdit_20->setText("");
        }
        return;
    }

    strB = ui->lineEdit_10->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_10->setText(strA);

    strB = ui->lineEdit_12->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_12->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_12()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_12->text();
    ui->lineEdit_19->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_12->setText("");
            ui->lineEdit_19->setText("");
        }
        return;
    }

    strB = ui->lineEdit_11->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_11->setText(strA);

    strB = ui->lineEdit_13->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_13->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_13()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_13->text();
    ui->lineEdit_18->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_13->setText("");
            ui->lineEdit_18->setText("");
        }
        return;
    }

    strB = ui->lineEdit_12->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_12->setText(strA);

    strB = ui->lineEdit_14->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_14->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_14()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_14->text();
    ui->lineEdit_17->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_14->setText("");
            ui->lineEdit_17->setText("");
        }
        return;
    }

    strB = ui->lineEdit_13->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_13->setText(strA);

    strB = ui->lineEdit_15->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_15->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditTar_15()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_15->text();
    ui->lineEdit_16->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( !b){
        if(strA.length()>= 0){
            ui->lineEdit_15->setText("");
            ui->lineEdit_16->setText("");
        }
        return;
    }

    strB = ui->lineEdit_14->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_14->setText(strA);
}


/****************************************************************************************/


void ProbeSettingSlctvtAdvIIPage::onEditNonTar_1()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n1->text();
    ui->lineEdit_n30->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n1->setText("");
            ui->lineEdit_n30->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n2->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n2->setText(strA);

}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_2()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n2->text();
    ui->lineEdit_n29->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n2->setText("");
            ui->lineEdit_n29->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n1->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n1->setText(strA);

    strB = ui->lineEdit_n3->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n3->setText(strA);
}

void ProbeSettingSlctvtAdvIIPage::onEditNonTar_3()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n3->text();
    ui->lineEdit_n28->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n3->setText("");
            ui->lineEdit_n28->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n2->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n2->setText(strA);

    strB = ui->lineEdit_n4->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n4->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_4()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n4->text();
    ui->lineEdit_n27->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n4->setText("");
            ui->lineEdit_n27->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n3->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n3->setText(strA);

    strB = ui->lineEdit_n5->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n5->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_5()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n5->text();
    ui->lineEdit_n26->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n5->setText("");
            ui->lineEdit_n26->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n4->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n4->setText(strA);

    strB = ui->lineEdit_n6->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n6->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_6()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n6->text();
    ui->lineEdit_n25->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n6->setText("");
            ui->lineEdit_n25->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n5->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n5->setText(strA);

    strB = ui->lineEdit_n7->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n7->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_7()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n7->text();
    ui->lineEdit_n24->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n7->setText("");
            ui->lineEdit_n24->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n6->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n6->setText(strA);

    strB = ui->lineEdit_n8->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n8->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_8()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n8->text();
    ui->lineEdit_n23->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n8->setText("");
            ui->lineEdit_n23->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n7->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n7->setText(strA);

    strB = ui->lineEdit_n9->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n9->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_9()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n9->text();
    ui->lineEdit_n22->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n9->setText("");
            ui->lineEdit_n22->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n8->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n8->setText(strA);

    strB = ui->lineEdit_n10->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n10->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_10()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n10->text();
    ui->lineEdit_n21->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n10->setText("");
            ui->lineEdit_n21->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n9->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n9->setText(strA);

    strB = ui->lineEdit_n11->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n11->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_11()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n11->text();
    ui->lineEdit_n20->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n11->setText("");
            ui->lineEdit_n20->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n10->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n10->setText(strA);

    strB = ui->lineEdit_n12->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n12->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_12()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n12->text();
    ui->lineEdit_n19->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n12->setText("");
            ui->lineEdit_n19->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n11->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n11->setText(strA);

    strB = ui->lineEdit_n13->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n13->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_13()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n13->text();
    ui->lineEdit_n18->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n13->setText("");
            ui->lineEdit_n18->setText("");
        }
        return;
    }
    strB = ui->lineEdit_n12->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n12->setText(strA);

    strB = ui->lineEdit_n14->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n14->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_14()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n14->text();
    ui->lineEdit_n17->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n14->setText("");
            ui->lineEdit_n17->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n13->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n13->setText(strA);

    strB = ui->lineEdit_n15->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n15->setText(strA);
}
void ProbeSettingSlctvtAdvIIPage::onEditNonTar_15()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n15->text();
    ui->lineEdit_n16->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( !b ){
        if(strA.length()>= 0){
            ui->lineEdit_n15->setText("");
            ui->lineEdit_n16->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n14->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n14->setText(strA);

}


bool ProbeSettingSlctvtAdvIIPage::validatePage()
{
    probeCostType = ui->radioButton_binary->isChecked();

    bool b;
    simCost[BASE_C][BASE_A] = ui->lineEdit_CA->text().toFloat(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    simCost[BASE_G][BASE_A] = ui->lineEdit_GA->text().toFloat(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    simCost[BASE_T][BASE_A] = ui->lineEdit_TA->text().toFloat(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    simCost[BASE_G][BASE_C] = ui->lineEdit_GC->text().toFloat(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    simCost[BASE_T][BASE_C] = ui->lineEdit_TC->text().toFloat(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    simCost[BASE_T][BASE_G] = ui->lineEdit_TG->text().toFloat(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }

    maxIndel = ui->comboBox_maxGap->currentText().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    indelCost = ui->lineEdit_cost->text().toFloat(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }

    probePosWtTar[14] = ui->lineEdit_1->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[13] = ui->lineEdit_2->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[12] = ui->lineEdit_3->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[11] = ui->lineEdit_4->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[10] = ui->lineEdit_5->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[9] = ui->lineEdit_6->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[8] = ui->lineEdit_7->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[7] = ui->lineEdit_8->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[6] = ui->lineEdit_9->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[5] = ui->lineEdit_10->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[4] = ui->lineEdit_11->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[3] = ui->lineEdit_12->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[2] = ui->lineEdit_13->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[1] = ui->lineEdit_14->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[0] = ui->lineEdit_15->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    for( int i = 15; i < MAX_PROBE_LEN/2+1; i++ )
        probePosWtTar[i] = probePosWtTar[14];


    probePosWtNonTar[14] = ui->lineEdit_n1->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[13] = ui->lineEdit_n2->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[12] = ui->lineEdit_n3->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[11] = ui->lineEdit_n4->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[10] = ui->lineEdit_n5->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[9] = ui->lineEdit_n6->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[8] = ui->lineEdit_n7->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[7] = ui->lineEdit_n8->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[6] = ui->lineEdit_n9->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[5] = ui->lineEdit_n10->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[4] = ui->lineEdit_n11->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[3] = ui->lineEdit_n12->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[2] = ui->lineEdit_n13->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[1] = ui->lineEdit_n14->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[0] = ui->lineEdit_n15->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    for( int i = 15; i < MAX_PROBE_LEN/2+1; i++ )
        probePosWtNonTar[i] = probePosWtNonTar[14];

    return ((QWizardPage)this).validatePage();
}

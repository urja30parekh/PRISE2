#include "ProbeSettingPropertyPage.h"
#include "ui_ProbeSettingPropertyPage.h"
#include <QDesktopWidget>

ProbeSettingPropertyPage::ProbeSettingPropertyPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ProbeSettingPropertyPage)
{
    ui->setupUi(this);

    ui->lineEdit_probeLenMin->setText( QString("%1").arg(probeLenMin));
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

    //ui->radioButton_estimated->setChecked(probeEstimate);
    //ui->radioButton_exact->setChecked(!probeEstimate);
    //ui->radioButton_regular->setChecked(!probeCostType);
    //ui->radioButton_binary->setChecked(probeCostType);

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
}

ProbeSettingPropertyPage::~ProbeSettingPropertyPage()
{
    delete ui;
}

void ProbeSettingPropertyPage::initializePage()
{

#ifdef MAC
    this->resize(950, 620);
#else
    this->resize(900, 520);
#endif

    QWizard* p = wizard();
    prevWidth = p->width();
    prevHeight = p->height();


#ifdef MAC
    p->resize(width(), height());
#else
    p->resize(width(), height());
#endif

    QDesktopWidget *desktop = QApplication::desktop();
    p->move((desktop->width() - p->width()) / 2, (desktop->height() - p->height()) / 2);

}

int ProbeSettingPropertyPage::nextId() const
{
    return Page_probeSettingBasic;
}

void ProbeSettingPropertyPage::cleanupPage()
{
    QWizard* p = wizard();
    p->resize(prevWidth, prevHeight);
}

bool ProbeSettingPropertyPage::clickAvoidACT()
{
    ui->lineEdit_numACT->setEnabled(ui->checkBox_avoidACT->isChecked());
    return 0;
}
bool ProbeSettingPropertyPage::clickAvoidG()
{
    ui->lineEdit_numG->setEnabled(ui->checkBox_avoidG->isChecked());
    return 0;
}
/*bool ProbeSettingPropertyPage::clickAvoidGat5end()
{
    //ui->lineEdit_numGat5end->setEnabled(ui->checkBox_avoidGat5end->isChecked());
}*/


void ProbeSettingPropertyPage::changeEvent(QEvent *e)
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

bool ProbeSettingPropertyPage::validatePage()
{

    //probeCostType = ui->radioButton_binary->isChecked();

    bool b;
    probeLenMin = ui->lineEdit_probeLenMin->text().toInt(&b);
    if(!b)  {
        QMessageBox::about(this,"Probe length Error","Please check the minimum probe length");
        return false;
    }
    probeLenMax = ui->lineEdit_probeLenMax->text().toInt(&b);
    if(!b)  {
        QMessageBox::about(this,"Probe length Error","Please check the maximum probe length");
        return false;
    }

    if(probeLenMax >= MAX_PROBE_LEN){
        QMessageBox::about(this,"Probe Length Error",QString("The maximum probe length cannot exceed %1!").arg(MAX_PROBE_LEN));
        return false;
    }

    probeCandidFrequency = ui->lineEdit_candidFrequency->text().toFloat(&b);
    if(!b){
        QMessageBox::about(this,"Input Error","Please check the % of target sequence");
        return false;
    }
    probeCandidTopPctg = ui->lineEdit_candidTopPctg->text().toFloat(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the top %");
        return false;
    }
    //pcrSizeMin = ui->lineEdit_pcrSizeMin->text().toInt(&b);
    //if(!b)  QMessageBox::about(this,"Input Error","Please check the minimum PCR product size");
    //pcrSizeMax = ui->lineEdit_pcrSizeMax->text().toInt(&b);
    //if(!b)  QMessageBox::about(this,"Input Error","Please check the maximum PCR product size");

    probeMinGap = ui->lineEdit_minGap->text().toFloat(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the minimum gap between primer and probe");
        return false;
    }

    if(ui->checkBox_avoidACT->isChecked()){
        probeAvoidACT = true;
        probeNumACT = ui->lineEdit_numACT->text().toInt(&b);
        if(!b)  {
            QMessageBox::about(this,"Input Error","Please check the continuity constraint on A,C,T");
            return false;
        }
        if( probeNumACT <= 1){
            QMessageBox::about(this,"constraint Error","the continuity constraint on A,C,T can not less than 2!");
            return false;
        }
    }else probeAvoidACT = false;

    if(ui->checkBox_avoidG->isChecked()){
        probeAvoidG = true;
        probeNumG = ui->lineEdit_numG->text().toInt(&b);
        if(!b){
            QMessageBox::about(this,"Input Error","Please check the continuity constraint on G");
            return false;
        }
        if( probeNumG <= 1){
            QMessageBox::about(this,"constraint Error","the continuity constraint on G can not less than 2!");
            return false;
        }
    }else probeAvoidG = false;

    if(ui->checkBox_avoidGat5end->isChecked()){
        probeAvoidGat5end = true;
        /*probeNumGat5end = ui->lineEdit_numGat5end->text().toInt(&b);
        if(!b)  QMessageBox::about(this,"Input Error","Please check the constraint on base G at 5\' end");
        if( probeNumGat5end < 0|| probeNumGat5end >=5){
            QMessageBox::about(this,"constraint on base G Error","the constraint on base G at 5\' end can not exceed 5!");
            return false;
        }*/
    }else probeAvoidGat5end = false;


    //probeEstimate = ui->radioButton_estimated->isChecked();

    probeTmMin = ui->lineEdit_tmMin->text().toFloat(&b);
    if(!b){
        QMessageBox::about(this,"Input Error","Please check the minimum probe Tm");
        return false;
    }
    probeTmMax = ui->lineEdit_tmMax->text().toFloat(&b);
    if(!b) {
        QMessageBox::about(this,"Input Error","Please check the maximum probe Tm");
        return false;
    }
    probeTmGreaterMin = ui->lineEdit_tmDiffMin->text().toFloat(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the minimum value of probe Tm diff");
        return false;
    }

    probeTmGreaterMax = ui->lineEdit_tmDiffMax->text().toFloat(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the maximum value of probe Tm diff");
        return false;
    }
    probeNaConsent = ui->lineEdit_NaConsent->text().toFloat(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the Na+ concentrations");
        return false;
    }
    probeGCMin = ui->lineEdit_GCMin->text().toFloat(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the minimum GC %");
        return false;
    }
    probeGCMax = ui->lineEdit_GCMax->text().toFloat(&b);
    if(!b) {
        QMessageBox::about(this,"Input Error","Please check the maximum GC %");
        return false;
    }
    maxPbPrComp = ui->lineEdit_maxPbPrComp->text().toInt(&b);
    if(!b) {
        QMessageBox::about(this,"Input Error","Please check the Max probe inter-complementarity ");
        return false;
    }
    /*max3PbPrComp=ui->lineEdit_max3PbPrComp->text().toInt(&b);
    if(!b) {
        QMessageBox::about(this,"Input Error","Please check the Max probe 3' inter-complementarity ");
        return false;
    }
    tripleInterBase3end = ui->lineEdit_interBase3end->text().toInt(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the inter-complementarity count from 3\'");
        return false;
    }*/
    probeMaxSelfComp = ui->lineEdit_maxSelfComp->text().toInt(&b);
    if(!b) {
        QMessageBox::about(this,"Input Error","Please check the Max probe intra-complementarity");
        return false;
    }
 /*   probeMax3SelfComp = ui->lineEdit_max3SelfComp->text().toInt(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the Max probe 3' intra-complementarity");
        return false;
    }
    probeIntraBase3end = ui->lineEdit_intraBase3end->text().toInt(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the intra-complementarity count from 3\'");
        return false;
    }*/

    if( probeLenMin > probeLenMax ){
            QMessageBox::about(this,"probe Length Error","Probe length setting error!");
            return false;
    }
    /*if( pcrSizeMin > pcrSizeMax ){
            QMessageBox::about(this,"PCR Size Error","PCR size setting error!");
            return false;
    }*/
    if( probeTmMin > probeTmMax ){
            QMessageBox::about(this,"Tm Error","Tm setting error!");
            return false;
    }
    if( probeGCMin > probeGCMax ){
            QMessageBox::about(this,"GC Error","GC content setting error!");
            return false;
    }
    return ((QWizardPage)this).validatePage();
}

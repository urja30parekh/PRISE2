#include "PrimerSettingPropertyPage.h"
#include "ui_PrimerSettingPropertyPage.h"
#include <QDesktopWidget>

PrimerSettingPropertyPage::PrimerSettingPropertyPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::PrimerSettingPropertyPage)
{
    ui->setupUi(this);

    ui->lineEdit_primerLenMin->setText( QString("%1").arg(primerLenMin));
    ui->lineEdit_primerLenMax	->setText(QString("%1").arg(primerLenMax));
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

    //ui->radioButton_estimated->setChecked(primerEstimate);
    //ui->radioButton_exact->setChecked(!primerEstimate);
    // primerCostType
    // false: regular
    // true: binary

    //ui->radioButton_regular->setChecked(!primerCostType);
    //ui->radioButton_binary->setChecked(primerCostType);

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
}

PrimerSettingPropertyPage::~PrimerSettingPropertyPage()
{
    delete ui;
}

void PrimerSettingPropertyPage::initializePage()
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
    p->resize(width(), height()+180);
#else
    p->resize(width(), height()+180);
#endif

    QDesktopWidget *desktop = QApplication::desktop();
    p->move((desktop->width() - p->width()) / 2, (desktop->height() - p->height()) / 2);

}

void PrimerSettingPropertyPage::cleanupPage()
{
    QWizard* p = wizard();
    p->resize(prevWidth, prevHeight);
}

bool PrimerSettingPropertyPage::clickAvoidACT()
{
    ui->lineEdit_numACT->setEnabled(ui->checkBox_avoidACT->isChecked());
    return 0;
}
bool PrimerSettingPropertyPage::clickAvoidG()
{
    ui->lineEdit_numG->setEnabled(ui->checkBox_avoidG->isChecked());
    return 0;
}
bool PrimerSettingPropertyPage::clickAvoidCGat3end()
{
    ui->lineEdit_numCGat3end->setEnabled(ui->checkBox_avoidCGat3->isChecked());
    return 0;
}

int PrimerSettingPropertyPage::nextId() const
{
    return Page_primerSetttingBasic;
}

void PrimerSettingPropertyPage::changeEvent(QEvent *e)
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

bool PrimerSettingPropertyPage::validatePage()
{
    //primerCostType = ui->radioButton_binary->isChecked();

    bool b;
    primerLenMin = ui->lineEdit_primerLenMin->text().toInt(&b);
    if(!b)  {
        QMessageBox::about(this,"Primer length Error","Please check the minimum primer length");
        return false;
    }
    primerLenMax = ui->lineEdit_primerLenMax	->text().toInt(&b);
    if(!b)  {
        QMessageBox::about(this,"Primer length Error","Please check the maximum primer length");
        return false;
    }

    if(primerLenMax >= MAX_PRIMER_LEN){
        QMessageBox::about(this,"primer Length Error",QString("The Maximum primer length can not exceed %1").arg(MAX_PRIMER_LEN));
        return false;
    }

    primerCandidFrequency = ui->lineEdit_candidFrequency->text().toFloat(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the % of target sequence");
        return false;
    }
    primerCandidTopPctg = ui->lineEdit_candidTopPctg->text().toFloat(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the top %");
        return false;
    }
    primerPcrSizeMin = ui->lineEdit_pcrSizeMin->text().toInt(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the minimum PCR product size");
        return false;
    }
    primerPcrSizeMax = ui->lineEdit_pcrSizeMax->text().toInt(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the maximum PCR product size");
        return false;
    }

    if(ui->checkBox_avoidACT->isChecked()){
        primerAvoidACT = true;
        primerNumACT = ui->lineEdit_numACT->text().toInt(&b);
        if(!b)  {
            QMessageBox::about(this,"Input Error","Please check the continuity constraint on A,C,T");
            return false;
        }
        if( primerNumACT <= 1){
            QMessageBox::about(this,"constraint Error","the continuity constraint on A,C,T can not less than 2!");
            return false;
        }
    }else primerAvoidACT = false;

    if(ui->checkBox_avoidG->isChecked()){
        primerAvoidG = true;
        primerNumG = ui->lineEdit_numG->text().toInt(&b);
        if(!b)  {
            QMessageBox::about(this,"Input Error","Please check the continuity constraint on G");
            return false;
        }
        if( primerNumG <= 1){
            QMessageBox::about(this,"constraint Error","the continuity constraint on G can not less than 2!");
            return false;
        }
    }else primerAvoidG = false;

    if(ui->checkBox_avoidCGat3->isChecked()){
        primerAvoidCGat3end = true;
        primerNumCGat3end = ui->lineEdit_numCGat3end->text().toInt(&b);
        if(!b)  {
            QMessageBox::about(this,"Input Error","Please check the constraint on CG content at 3\' end");
            return false;
        }
        if( primerNumCGat3end < 0|| primerNumCGat3end >=5){
            QMessageBox::about(this,"CG content Error","the constraint on CG content at 3\' end can not exceed 5!");
            return false;
        }
    }else primerAvoidCGat3end = false;


    //primerEstimate = ui->radioButton_estimated->isChecked();

    primerTmMin = ui->lineEdit_tmMin->text().toFloat(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the minimum primer Tm");
        return false;
    }
    primerTmMax = ui->lineEdit_tmMax->text().toFloat(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the maximum primer Tm");
        return false;
    }
    primerTmDiff = ui->lineEdit_tmDiff->text().toFloat(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the limit of primer Tm diff");
        return false;
    }
    primerNaConsent = ui->lineEdit_NaConsent->text().toFloat(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the Na+ concentrations");
        return false;
    }
    primerGCMin = ui->lineEdit_GCMin->text().toFloat(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the minimum GC %");
        return false;
    }
    primerGCMax = ui->lineEdit_GCMax->text().toFloat(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the maximum GC %");
        return false;
    }
    primerMaxPrPrComp = ui->lineEdit_maxPrPrComp->text().toInt(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the Max primer inter-complementarity ");
        return false;
    }
    primerMax3PrPrComp=ui->lineEdit_max3PrPrComp->text().toInt(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the Max primer 3' inter-complementarity ");
        return false;
    }
    primerInterBase3end = ui->lineEdit_interBase3end	->text().toInt(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the inter-complementarity count from 3\'");
        return false;
    }
    primerMaxSelfComp = ui->lineEdit_maxSelfComp->text().toInt(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the Max primer intra-complementarity");
        return false;
    }
    primerMax3SelfComp = ui->lineEdit_max3SelfComp->text().toInt(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the Max primer 3' intra-complementarity");
        return false;
    }
    primerIntraBase3end = ui->lineEdit_intraBase3end->text().toInt(&b);
    if(!b)  {
        QMessageBox::about(this,"Input Error","Please check the intra-complementarity count from 3\'");
        return false;
    }

    if( primerLenMin > primerLenMax ){
            QMessageBox::about(this,"primer Length Error","Primer length setting error!");
            return false;
    }
    if( primerPcrSizeMin > primerPcrSizeMax ){
            QMessageBox::about(this,"PCR Size Error","PCR size setting error!");
            return false;
    }
    if( primerTmMin > primerTmMax ){
            QMessageBox::about(this,"Tm Error","Tm setting error!");
            return false;
    }
    if( primerGCMin > primerGCMax ){
            QMessageBox::about(this,"GC Error","GC content setting error!");
            return false;
    }
    return ((QWizardPage)this).validatePage();
}

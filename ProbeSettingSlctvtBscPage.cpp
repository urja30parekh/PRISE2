#include "ProbeSettingSlctvtBscPage.h"
#include "ui_ProbeSettingSlctvtBscPage.h"
#include "DataTools.h"

ProbeSettingSlctvtBscPage::ProbeSettingSlctvtBscPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ProbeSettingSlctvtBscPage)
{
    ui->setupUi(this);
    ui->radioButton_default->setChecked(!probeShowAdvance);
    ui->radioButton_showAdv->setChecked(probeShowAdvance);

    setFinalPage(!probeShowAdvance);

    ui->horizontalSlider_target->setValue(probeBindContMatches_tar);
    ui->horizontalSlider_nontarget->setValue(probeBindContMatches_nontar);

    ui->lineEdit_ContMatchesTarget->setText(QString("%1").arg(probeBindContMatches_tar));
    ui->lineEdit_ContMatchesNontarget->setText(QString("%1").arg(probeBindContMatches_nontar));
}

ProbeSettingSlctvtBscPage::~ProbeSettingSlctvtBscPage()
{
    delete ui;
}

void ProbeSettingSlctvtBscPage::initializePage()
{

#ifdef MAC
    this->resize(1040, 520);
#else
    this->resize(850, 450);
#endif

    QWizard* p = this->wizard();
    prevWidth = p->width();
    prevHeight = p->height();

#ifdef MAC
    p->resize(width()+150, height()+450);
#else
    p->resize(width()+150, height()+150);
#endif

}

void ProbeSettingSlctvtBscPage::cleanupPage()
{
    QWizard* p = wizard();
    p->resize(prevWidth, prevHeight);
}

void ProbeSettingSlctvtBscPage::changeEvent(QEvent *e)
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

void ProbeSettingSlctvtBscPage::activateWindow()
{

}

void ProbeSettingSlctvtBscPage::onScrollTar()
{
    int posSel;

    posSel = ui->horizontalSlider_target->value();
    int numMatches = posSel;

    ui->lineEdit_ContMatchesTarget->setText(QString("%1").arg(numMatches));
}

void ProbeSettingSlctvtBscPage::onScrollNontar()
{
    int posSel;

    posSel = ui->horizontalSlider_nontarget->value();
    int numMatches = posSel;

    ui->lineEdit_ContMatchesNontarget->setText(QString("%1").arg(numMatches));
}


int ProbeSettingSlctvtBscPage::nextId() const
{
    if (ui->radioButton_default->isChecked()) {
        return -1;
    } else {
        return Page_probeSettingAdvII;
    }
}

void ProbeSettingSlctvtBscPage::setPosWtTarget(int num, ...)
{
    // to store the list of arguments
    va_list args;
    // initialize arguments to store all values
    va_start(args, num);

    for( int i = 0; i < num; i++)
        probePosWtTar[i] = (float)va_arg(args, int);
    for( int i = num; i < MAX_PROBE_LEN/2+1; i++ )
        probePosWtTar[i] = probePosWtTar[num-1];

    // clean up the list
    va_end(args);
}

void ProbeSettingSlctvtBscPage::setPosWtNonTarget(int num, ...)
{
    // to store the list of arguments
    va_list args;
    // initialize arguments to strore all values
    va_start(args, num);

    for( int i = 0; i < num; i++)
        probePosWtNonTar[i] = (float)va_arg(args, int);
    for( int i = num; i < MAX_PROBE_LEN/2+1; i++ )
        probePosWtNonTar[i] =probePosWtNonTar[num-1];

    // clean up the list
    va_end(args);
}

/*void ProbeSettingSlctvtBscPage::getUserParameter()
{
    probeBindContMatches_tar = ui->horizontalSlider_target->value();
    switch( probeBindContMatches_tar*2+4 ){
    case 4:
        setPosWtTarget(15, 0,0,1,1,1,1,1,2,2,2,2,2,3,3,3);
        break;
    case 6:
        setPosWtTarget(15, 0,0,0,1,1,1,1,1,2,2,2,2,3,3,3);
        break;
    case 8:
        setPosWtTarget(15, 0,0,0,0,1,1,1,1,1,2,2,2,2,3,3);
        break;
    case 10:
        setPosWtTarget(15, 0,0,0,0,0,1,1,1,1,2,2,2,2,3,3);
        break;
    case 12:
        setPosWtTarget(15, 0,0,0,0,0,0,1,1,1,1,2,2,2,2,3);
        break;
    default:
        QMessageBox::about(this, "Error value","Wrong parameter, pleasen contact the programmer");
        exit(1);
        break;
    }

    probeBindContMatches_nontar = ui->horizontalSlider_nontarget->value();
    switch( probeBindContMatches_nontar*2+4 ){
    case 4:
        setPosWtNonTarget(15, 0,0,1,1,1,1,2,2,3,3,4,4,4,5,5);
        break;
    case 6:
        setPosWtNonTarget(15, 0,0,0,1,1,1,1,2,2,3,3,4,4,5,5);
        break;
    case 8:
        setPosWtNonTarget(15, 0,0,0,0,1,1,1,2,2,2,3,3,4,4,5);
        break;
    case 10:
        setPosWtNonTarget(15, 0,0,0,0,0,1,1,1,1,2,2,3,3,4,4);
        break;
    case 12:
        setPosWtNonTarget(15, 0,0,0,0,0,0,1,1,1,1,2,2,2,3,4);
        break;
    default:
        QMessageBox::about(this, "Error value","Wrong parameter, pleasen contact the programmer");
        exit(1);
        break;

    }
}*/

bool ProbeSettingSlctvtBscPage::validatePage()
{

    probeBindContMatches_tar = ui->horizontalSlider_target->value();
    probeBindContMatches_nontar = ui->horizontalSlider_nontarget->value();

    DataTools::setProbeMismatchAllowance(probeBindContMatches_tar, probeBindContMatches_nontar);
    probeShowAdvance = ui->radioButton_showAdv->isChecked();
    return ((QWizardPage)this).validatePage();
}

void ProbeSettingSlctvtBscPage::onDefault()
{
    if (this->nextId() == -1)
        setFinalPage(true);
    else
        setFinalPage(false);
}

void ProbeSettingSlctvtBscPage::onShowAdv()
{
    //this->setFinalPage(false);
    if (this->nextId() == -1)
        setFinalPage(true);
    else
        setFinalPage(false);
}


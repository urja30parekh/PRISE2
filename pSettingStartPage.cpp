#include "pSettingStartPage.h"
#include "ui_pSettingStartPage.h"
#include "DataTools.h"

pSettingStartPage::pSettingStartPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::pSettingStartPage)
{
    ui->setupUi(this);
}

void pSettingStartPage::initializePage()
{

#ifdef MAC
    this->resize(600, 220);
#else
    this->resize(600, 220);
#endif

    QWizard* p = wizard();
    prevWidth = p->width();
    prevHeight = p->height();


#ifdef MAC
    p->resize(width()+50, height()+80);
#else
    p->resize(width()+50, height()+80);
#endif

    p->button(QWizard::NextButton)->hide();

    /*switch(primerSettingOption){
    case 0:
        ui->radioButton_default->setChecked(true);
        break;
    case 1:
        ui->radioButton_prev->setChecked(true);
        break;
    case 2:
        ui->radioButton_show->setChecked(true);
        break;
    }*/

    switch(task){
    case 0://priemrs
    {
        if(firstPrimer){
            ui->radioButton_default->setChecked(true);
            ui->radioButton_prev->setEnabled(false);
        }else{
            ui->radioButton_prev->setEnabled(true);
            ui->radioButton_prev->setChecked(true);
        }
        break;
    }
    case 1://probes
    {
        if(firstProbe){
            ui->radioButton_default->setChecked(true);
            ui->radioButton_prev->setEnabled(false);
        }else{
            ui->radioButton_prev->setEnabled(true);
            ui->radioButton_prev->setChecked(true);
        }
        break;
    }
    default://error
        QMessageBox::about(Q_NULLPTR, "Design process Error", "Neither primers nor probes. Please contact the developer");
        exit(1);
    }



}

void pSettingStartPage::cleanupPage()
{
    QWizard* p = wizard();
    p->resize(prevWidth, prevHeight);
}

pSettingStartPage::~pSettingStartPage()
{
    delete ui;
}

void pSettingStartPage::changeEvent(QEvent *e)
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


void pSettingStartPage::setButtonTextPrimers()
{
    ui->label->setText("This primer design wizard will help you to choose most of the settings for designing primers");
    ui->radioButton_default->setText("Use all default settings (and start designing primers now)");
}

void pSettingStartPage::setButtonTextProbes()
{
    ui->label->setText("This probe design wizard will help you to choose most of the settings for designing probes");
    ui->radioButton_default->setText("Use all default settings (and start designing probes now)");
}

int pSettingStartPage::nextId() const
{
    if (ui->radioButton_default->isChecked() || ui->radioButton_prev->isChecked()) {
        return -1;
    } else {
        switch(task){
        case 0://priemrs
            return Page_primerProperty;
        case 1://probes
            return Page_probeProperty;
        default://error
            QMessageBox::about(Q_NULLPTR, "Design process Error", "Neither primers nor probes. Please contact the developer");
            return -1;
        }
    }
}

void pSettingStartPage::onFinishSetting()
{
    if (this->nextId() == -1)
        setFinalPage(true);
    else
        setFinalPage(false);
}

void pSettingStartPage::onContinue()
{
    if (this->nextId() == -1)
        setFinalPage(true);
    else
        setFinalPage(false);
}


/*bool pSettingStartPage::loadSetting( QString settingFname)
{
}*/

bool pSettingStartPage::validatePage()
{
    if( ui->radioButton_default->isChecked() ){ //load user previous settings

        switch(task){
        case 0://priemrs
            DataTools::defaultPrimer();
            break;
        case 1://probes
            DataTools::defaultProbe();
            break;
        default://error
            QMessageBox::about(Q_NULLPTR, "Design process Error", "Neither primers nor probes. Please contact the developer");
            return -1;
        }

        //DataTools::Initialize();
        /*QFileDialog file_dialog(this);
        file_dialog.setFileMode( QFileDialog::ExistingFile );
        QString filename = file_dialog.getOpenFileName( this, tr( "Load Candidates File" ), "./", tr( "PRISE Setting Files (*.primersetting);;All File (*.*)" )) ;
        if(filename.isEmpty()){
            QMessageBox::about(0, "File Error", "Please input PRISE setting profile");
            return false;
        }

        //Open Sequence File.
        QFile file(filename);
        if (!file.open(QFile::ReadOnly))
        {
            QMessageBox::about(0, "File Error", "File can not be opened\nPlease check your file, the file name should not exceed 64 characters");
            return false;
        }
        file.close();*/
    }
    return QWizardPage::validatePage();
}

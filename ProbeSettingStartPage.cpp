#include "ProbeSettingStartPage.h"
#include "ui_ProbeSettingStartPage.h"

ProbeSettingStartPage::ProbeSettingStartPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ProbeSettingStartPage)
{
    ui->setupUi(this);
}

void  ProbeSettingStartPage::initializePage()
{
    QWizard* pWizard = wizard();
    pWizard->button(QWizard::NextButton)->hide();

    switch(probeSettingOption){
    case 0:
        ui->radioButton_default->setChecked(true);
        break;
    case 1:
        ui->radioButton_saved->setChecked(true);
        break;
    case 2:
        ui->radioButton_show->setChecked(true);
        break;
    }
}

ProbeSettingStartPage::~ProbeSettingStartPage()
{
    delete ui;
}

void ProbeSettingStartPage::changeEvent(QEvent *e)
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

int ProbeSettingStartPage::nextId() const
{
    if (ui->radioButton_default->isChecked() || ui->radioButton_saved->isChecked()) {
        return -1;
    } else {
        return QWizardPage::nextId();
    }
}

void ProbeSettingStartPage::onFinishSetting()
{
    if (this->nextId() == -1)
        setFinalPage(true);
    else
        setFinalPage(false);
}

void ProbeSettingStartPage::onContinue()
{
    if (this->nextId() == -1)
        setFinalPage(true);
    else
        setFinalPage(false);
}


/*bool ProbeSettingStartPage::loadSetting( QString settingFname)
{
}*/

bool ProbeSettingStartPage::validatePage()
{
    // load user previously saved settings
    if( ui->radioButton_saved->isChecked() ){
        QFileDialog file_dialog(this);
        file_dialog.setFileMode( QFileDialog::ExistingFile );
        QString filename = file_dialog.getOpenFileName( this, tr( "Load Candidates File" ), "./", tr( "PRISE Setting Files (*.probesetting);;All File (*.*)" )) ;
        if(filename.isEmpty()){
            QMessageBox::about(0, "File Error", "Please input PRISE setting profile");
            return false;
        }

        //Open Sequence File.
        QFile file(filename);
        if (!file.open(QFile::ReadOnly))
        {
            QMessageBox::about(0, "File Error", "File can not be opened.\nPlease check your file, the file name should not exceed 64 characters.");
            return false;
        }
        file.close();
    }
    return ((QWizardPage)this).validatePage();
}


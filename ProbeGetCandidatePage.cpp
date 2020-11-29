#include "ProbeGetCandidatePage.h"
#include "ui_ProbeGetCandidatePage.h"

ProbeGetCandidatePage::ProbeGetCandidatePage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ProbeGetCandidatePage)
{
    ui->setupUi(this);
    ui->lineEdit_CandFilename->setText(probeFilename);
    ui->radioButton_designByInput ->setChecked(!usrLoad);
    ui->radioButton_loadCands->setChecked(usrLoad);
}

ProbeGetCandidatePage::~ProbeGetCandidatePage()
{
    delete ui;
}

void ProbeGetCandidatePage::changeEvent(QEvent *e)
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

void ProbeGetCandidatePage::browseCandidatesFile()
{
    ui->radioButton_loadCands->setChecked(true);
    QFileDialog file_dialog(this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getOpenFileName( this, tr( "Load Candidates File" ), QDir::currentPath(), tr( "TXT File (*.txt);;All File (*.*)" )) ;
    if(filename.isEmpty()){
        return;
    }

    //Open Sequence File.
    QFile file(filename);
    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::about(Q_NULLPTR, "File Error", "File can not be opened.\nPlease check your file, the file name should not exceed 64 characters.");
        return;
    }
    file.close();
    ui->lineEdit_CandFilename->setText( filename);
    setTitle(filename);
}

bool ProbeGetCandidatePage::validatePage()
{
    usrLoad = ui->radioButton_loadCands->isChecked();
    if( usrLoad)
    {
        probeFilename = ui->lineEdit_CandFilename->text();
        if(probeFilename.length() <= 1 ){
            QMessageBox::about(this, "Missing Info","Please locate your probe candidate file by clicking \"Load user's probe candidates\".");
            return false;
        }
    }
    return QWizardPage::validatePage();
}

#include "pGetCandidatePage.h"
#include "ui_pGetCandidatePage.h"
#include <QDesktopWidget>

pGetCandidatePage::pGetCandidatePage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::pGetCandidatePage)
{
    ui->setupUi(this);
    ui->lineEdit_CandFilename->setText(primerFilename);
    ui->radioButton_designByInput ->setChecked(!usrLoad);
    ui->radioButton_loadCands->setChecked(usrLoad);
}

pGetCandidatePage::~pGetCandidatePage()
{
    delete ui;
}

void pGetCandidatePage::initializePage()
{

#ifdef MAC
    this->resize(630, 250);
#else
    this->resize(630, 250);
#endif

    QWizard* p = wizard();
    prevWidth = p->width();
    prevHeight = p->height();

#ifdef MAC
    p->resize(width()+50, height()+150);
#else
    p->resize(width()+50, height()+50);
#endif

    QDesktopWidget *desktop = QApplication::desktop();
    p->move((desktop->width() - p->width()) / 2, (desktop->height() - p->height()) / 2);
}

void pGetCandidatePage::cleanupPage()
{
    QWizard* p = wizard();
    p->resize(prevWidth, prevHeight);
}


void pGetCandidatePage::changeEvent(QEvent *e)
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

void pGetCandidatePage::browseCandidatesFile()
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


void pGetCandidatePage::setButtonTextPrimers()
{
    ui->groupBox->setTitle("Extract primer candidates from target sequences or load user's primer candidates");
    ui->radioButton_designByInput->setText("Design primers based on target and non-target sequences");
    ui->radioButton_loadCands->setText("Load user's primer candidates");
    ui->label->setText("Note: If you provide your own primer candidates, the program will not\nconsider the primer candidates extracted from target sequences");
}

void pGetCandidatePage::setButtonTextProbes()
{
    ui->groupBox->setTitle("Extract probe candidates from target sequences or load user's probe candidates");
    ui->radioButton_designByInput->setText("Design probes based on target and non-target sequences");
    ui->radioButton_loadCands->setText("Load user's probe candidates");
    ui->label->setText("Note: If you provide your own probe candidates, the program will not\nconsider the probe candidates extracted from target sequences");
}

int pGetCandidatePage::nextId() const
{
    return Page_pSettingStart;
}

bool pGetCandidatePage::validatePage()
{
    usrLoad = ui->radioButton_loadCands->isChecked();
    if( usrLoad)
    {
        primerFilename = ui->lineEdit_CandFilename->text();
        if(primerFilename.length() <= 1 ){
            QMessageBox::about(this, "Missing Info","Please locate your primer candidate file by clicking \"Load user's primer candidates\".");
            return false;
        }
    }
    return QWizardPage::validatePage();
}

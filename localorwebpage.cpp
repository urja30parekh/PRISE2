#include "localorwebpage.h"
#include "ui_localorwebpage.h"

LocalOrWebPage::LocalOrWebPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::LocalOrWebPage)
{
    ui->setupUi(this);


    QSettings settings(".ncbirc", QSettings::IniFormat);
    blastBin  = settings.value("BLAST/BLASTBIN", "").toString();
    databasePath = settings.value("BLAST/BLASTDB", "").toString();


    ui->radioButton_web->setChecked(true);
    ui->lineEdit_blastBin->setText(blastBin);
    ui->lineEdit_blastBin->setEnabled(false);
    ui->pushButton_browse->setEnabled(false);
    ui->lineEdit_dbPath->setText(databasePath);
    ui->lineEdit_dbPath->setEnabled(false);
    ui->pushButton_browse2->setEnabled(false);


}

LocalOrWebPage::~LocalOrWebPage()
{
    delete ui;
}


void LocalOrWebPage::initializePage()
{
    ui->lineEdit_dbPath->setText(databasePath);

//    QWizard *p = this->wizard();
//    p->button(QWizard::FinishButton)->setVisible(false);
//    p->button(QWizard::CommitButton)->setVisible(true);
/*
#ifdef MAC
    this->resize(400, 150);
#else
    this->resize(400, 150);
#endif


    QWizard* p = wizard();

#ifdef MAC
    p->resize(width()+50, height()+150);
#else
    p->resize(width()+50, height()+50);
#endif*/
}

void LocalOrWebPage::on_radioButton_web_clicked()
{
    setFinalPage(true);
    ui->lineEdit_blastBin->setEnabled(false);
    ui->pushButton_browse->setEnabled(false);
    ui->lineEdit_dbPath->setEnabled(false);
    ui->pushButton_browse2->setEnabled(false);
//    QWizard *p = this->wizard();
//    p->button(QWizard::FinishButton)->setVisible(false);
//    p->button(QWizard::CommitButton)->setVisible(true);
}

void LocalOrWebPage::on_radioButton_local_clicked()
{
    setFinalPage(false);
    ui->lineEdit_blastBin->setEnabled(true);
    ui->pushButton_browse->setEnabled(true);
    ui->lineEdit_dbPath->setEnabled(true);
    ui->pushButton_browse2->setEnabled(true);
}


bool LocalOrWebPage::validatePage()
{

    blastBin = ui->lineEdit_blastBin->text();
    QSettings settings(".ncbirc", QSettings::IniFormat);
    settings.setValue("BLAST/BLASTBIN", blastBin);
    if( ui->radioButton_local->isChecked() && (blastBin.isEmpty() || !blastBin.endsWith("bin")) ){
        QMessageBox::about(this, "Wrong blast path","Please input a valid blast path which ends with \"blast-[version]\\bin\"");
        return false;
    }


    databasePath = ui->lineEdit_dbPath->text();
    settings.setValue("BLAST/BLASTDB", databasePath);
    if( ui->radioButton_local->isChecked() && databasePath.isEmpty() ){
        QMessageBox::about(this, "Missing Info","Please input the database directory for blast search");
        return false;
    }

    //TODO: best example of WHY global variables should not be used.....
    blastOption = ui->radioButton_local->isChecked() == true;
    return QWizardPage::validatePage();
}

void LocalOrWebPage::changeEvent(QEvent *e)
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

int LocalOrWebPage::nextId() const
{

    if(ui->radioButton_web->isChecked()){
        return -1;
    }else
        return Page_querAndSearchSet;
}

void LocalOrWebPage::on_pushButton_browse_clicked()
{
    QFileDialog file_dialog(this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );

    QString filename = QFileDialog::getExistingDirectory( this, tr( "Open Blast path" ), pwd) ;
    if(filename.isEmpty()){
        return;
    }
    this->ui->lineEdit_blastBin->setText(filename);
}

void LocalOrWebPage::on_pushButton_browse2_clicked()
{
    QFileDialog file_dialog(this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );

    QString filename = QFileDialog::getExistingDirectory( this, tr( "Open Blast database path" ), pwd) ;
    if(filename.isEmpty()){
        return;
    }
    this->ui->lineEdit_dbPath->setText(filename);
}

#include "LoadSeedHitTableDlg.h"
#include "ui_LoadSeedHitTableDlg.h"

LoadSeedHitTableDlg::LoadSeedHitTableDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadSeedHitTableDlg)
{
    ui->setupUi(this);
    ui->lineEdit_seedFile->setText(blastSeedFile);
    ui->lineEdit_hitTableFile->setText(saveHitTableTo);
    ui->checkBox_allSeqs->setChecked(downloadAllSeqs);
}

LoadSeedHitTableDlg::~LoadSeedHitTableDlg()
{
    delete ui;
}

void LoadSeedHitTableDlg::getFilePath(QString &SF,  QString &HTF,  QString &FF)
{
    SF = this->SeedFile;
    HTF = this->HitTableFile;
    FF = this->FASTAFile;
}

void LoadSeedHitTableDlg::getAccessions(QString &SeedAccs)
{
    SeedAccs = this->SeedAccessions;
}

void LoadSeedHitTableDlg::browseSeedFile()
{
    QFileDialog file_dialog(this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );

    //QString filename = file_dialog.getOpenFileName( this, tr( "Open Seed File" ), pwd, tr( "TEXT (*.txt)| *.txt" )) ;
    QString filename = file_dialog.getOpenFileName( this, tr( "Open Seed File" ), QDir::currentPath(), tr( "TEXT (*.txt)| *.txt" )) ;
    if(filename.isEmpty()){
        return;
    }
    this->ui->lineEdit_seedFile->setText(filename);
}

void LoadSeedHitTableDlg::browseHitTableFile()
{
    QFileDialog file_dialog(this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );

    //QString filename = file_dialog.getOpenFileName( this, tr( "Open Hit Table File" ), pwd, tr( "TEXT (*.txt)| *.txt" )) ;
    QString filename = file_dialog.getOpenFileName( this, tr( "Open Hit Table File" ), QDir::currentPath(), tr( "TEXT (*.txt)| *.txt" )) ;
    if(filename.isEmpty()){
        return;
    }
    this->ui->lineEdit_hitTableFile->setText(filename);

}

void LoadSeedHitTableDlg::browseFASTAFile()
{
    QFileDialog file_dialog(this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );

//    QString filename = file_dialog.getOpenFileName( this,  tr("Open FASTA File") , pwd, tr("TEXT (*.txt)| *.txt")) ;
    QString filename = file_dialog.getOpenFileName( this,  tr("Open FASTA File") , QDir::currentPath(), tr("TEXT (*.txt)| *.txt")) ;
    if(filename.isEmpty()){
        return;
    }

    this->ui->lineEdit_FASTAFile->setText(filename);
}


void LoadSeedHitTableDlg::accept()
{
    SeedAccessions = ui->plainTextEdit_seedAccession->toPlainText();
    SeedFile = ui->lineEdit_seedFile->text();
    HitTableFile = ui->lineEdit_hitTableFile->text();
    FASTAFile = ui->lineEdit_FASTAFile->text();
    downloadAllSeqs = ui->checkBox_allSeqs->isChecked();

    QAbstractButton * checked = ui->buttonGroup->checkedButton();
    if( checked == ui->radioButton_extractSimple)
        isSimple = true;
    else if (checked == ui->radioButton_extractFull )
        isSimple = false;
    else
    {
        QMessageBox::about(this, "Mode Error", "You must choose simple or full mode");
        return;
    }

    if(SeedAccessions == ""  && SeedFile == "")
    {
        QMessageBox::about(this, "No seed sequence", "You must input Seed sequence");
        return;
    }
    if(HitTableFile == "" && FASTAFile == "")
    {
        QMessageBox::about(this, "No hit table or FASTA file", "You must input  hit table or FASTA file");
        return;
    }

    QDialog::accept();
}


void LoadSeedHitTableDlg::changeEvent(QEvent *e)
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

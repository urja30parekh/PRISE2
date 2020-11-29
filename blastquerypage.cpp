#include "blastquerypage.h"
#include "ui_blastquerypage.h"
#include <assert.h>

BlastQueryPage::BlastQueryPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::BlastQueryPage)
{
    ui->setupUi(this);
}

BlastQueryPage::~BlastQueryPage()
{
    delete ui;
}

void BlastQueryPage::initializePage()
{
/*
#ifdef MAC
    this->resize(650, 500);
#else
    this->resize(650, 500);
#endif

    QWizard* p = wizard();
    prevWidth = p->width();
    prevHeight = p->height();

#ifdef MAC
    p->resize(width()+50, height()+150);
#else
    p->resize(width()+50, height()+50);
#endif
*/


    ui->lineEdit_SeedSeqFile->setText(blastSeedFile);
    ui->lineEdit_database->setText(databaseName);
    ui->lineEdit_entrezQuery->setText(entrezQuery);
    ui->radioButton_megablast->setChecked(true);

    if(blastType == "megablast")
        ui->radioButton_megablast->setChecked(true);
    else if (blastType == "dc-megablast")
        ui->radioButton_discMegablast->setChecked(true);
    else if (blastType == "blastn")
        ui->radioButton_blastn->setChecked(true);
    else{
        assert(blastType == "blastn-short");
        ui->radioButton_blastn_short->setChecked(true);
    }
}


int BlastQueryPage::nextId() const
{
    return Page_blastParameters;
}


void BlastQueryPage::changeEvent(QEvent *e)
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

void BlastQueryPage::on_toolButton_browse_clicked()
{
    QFileDialog file_dialog(this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );

    QString filename = file_dialog.getOpenFileName( this, tr( "Open Seed File" ), QDir::currentPath(), tr( "TEXT (*.txt)| *.txt" )) ;
    if(filename.isEmpty()){
        return;
    }
    this->ui->lineEdit_SeedSeqFile->setText(filename);
}

bool BlastQueryPage::validatePage()
{
    blastSeedFile = ui->lineEdit_SeedSeqFile->text();
    if(blastSeedFile.isEmpty() ){
        QMessageBox::about(this, "Missing Info","Please input seed sequences in FASTA format");
        return false;
    }

    databaseName = ui->lineEdit_database->text();
    if(databaseName.isEmpty()){
        QMessageBox::about(this, "Missing Info","Please input a database name to search");
        return false;
    }

    entrezQuery = ui->lineEdit_entrezQuery->text();

    if(ui->radioButton_megablast->isChecked()){
        blastType = "megablast";
    }else if (ui->radioButton_discMegablast->isChecked()){
        blastType = "dc-megablast";
    }else if (ui->radioButton_blastn->isChecked()){
        blastType = "blastn";
    }else{
        assert(ui->radioButton_blastn_short->isChecked());
        blastType = "blastn-short";
    }

    return QWizardPage::validatePage();
}

/*void BlastQueryPage::on_plainTextEdit_seq_textChanged()
{
    ui->lineEdit_SeedSeqFile->clear();
}


void BlastQueryPage::on_lineEdit_SeedSeqFile_textEdited(const QString &arg1)
{
    ui->plainTextEdit_seq->clear();
}*/



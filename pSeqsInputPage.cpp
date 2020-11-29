#include "pSeqsInputPage.h"
#include "ui_pSeqsInputPage.h"
#include <QDesktopWidget>
#include <QStyle>

pSeqsInputPage::pSeqsInputPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::pSeqsInputPage)
{
    ui->setupUi(this);

    ui->lineEdit_tar->setText(targetSeqFilename);
    ui->lineEdit_nonTar->setText(nontargetSeqFilename);
    ui->lineEdit_dupFile->setText(dupSeqFilename);
    ui->lineEdit_lenOutRangeFile->setText(lenOutRangeSeqFilename);

    ui->radioButton_keepDup->setChecked(!rmDupSeqs);
    on_radioButton_keepDup_clicked(!rmDupSeqs);

    ui->radioButton_removeDup->setChecked(rmDupSeqs);
    on_radioButton_removeDup_clicked(rmDupSeqs);

    ui->radioButton_keepLenOutRange->setChecked(!rmLenOutRangeSeqs);
    on_radioButton_keepLenOutRange_clicked(!rmLenOutRangeSeqs);

    ui->radioButton_removeLenOutRange->setChecked(rmLenOutRangeSeqs);
    on_radioButton_removeLenOutRange_clicked(rmLenOutRangeSeqs);

    ui->lineEdit_lenMin->setText(QString("%1").arg(seqLenMin));
    ui->lineEdit_lenMax->setText(QString("%1").arg(seqLenMax));

}


int pSeqsInputPage::nextId() const
{
    return Page_pCand;
}

pSeqsInputPage::~pSeqsInputPage()
{
    delete ui;
}

void pSeqsInputPage::initializePage()
{

#ifdef MAC
    this->resize(500, 600);
#else
    this->resize(500, 500);
#endif

    QWizard* p = wizard();
    prevWidth = p->width();
    prevHeight = p->height();

#ifdef MAC
    p->resize(width()+100, height()+120);
#else
    p->resize(width()+100, height()+120);
#endif

    QDesktopWidget *desktop = QApplication::desktop();
    p->move((desktop->width() - p->width()) / 2, (desktop->height() - p->height()) / 2);
}

void pSeqsInputPage::cleanupPage()
{
    QWizard* p = wizard();
    p->resize(prevWidth, prevHeight);
}

void pSeqsInputPage::changeEvent(QEvent *e)
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

void pSeqsInputPage::browseTargetSeq()
{
    QFileDialog file_dialog(this);

    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getOpenFileName( this, tr( "Target Sequences File" ), QDir::currentPath(), tr( "TXT File (*.txt);;All File (*.*)" )) ;
    if(filename.isEmpty()){
        return;
    }

    //Open Sequence File.
    QFile file(filename);
    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::about(0, "File Error", "File can not be opened.\nPlease check your file, the file name should not exceed 64 characters.");
        return;
    }
    file.close();
    ui->lineEdit_tar->setText( filename);
}

void pSeqsInputPage::browseNontargetSeq()
{
    QFileDialog file_dialog(this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getOpenFileName( this, tr( "Non-target Sequences File" ), QDir::currentPath(), tr( "TXT File (*.txt);;All File (*.*)" )) ;
    if(filename.isEmpty()){
        return;
    }

    //Open Sequence File.
    QFile file(filename);
    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::about(0, "File Error", "File can not be opened.\nPlease check your file, the file name should not exceed 64 characters.");
        return;
    }
    file.close();
    ui->lineEdit_nonTar->setText( filename);
}

void pSeqsInputPage::browseDumpFileDup()
{
    QFileDialog file_dialog(this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getSaveFileName( this, tr( "Dump duplicated sequences to" ), pwd, tr( "TXT File (*.txt);;All File (*.*)" )) ;
    if(filename.isEmpty()){
        return;
    }

    //Open Sequence File.
    QFile file(filename);
    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::about(0, "File Error", "File can not be opened.\nPlease check your file, the file name should not exceed 64 characters.");
        return;
    }
    file.close();
    ui->lineEdit_dupFile->setText( filename);
}

void pSeqsInputPage::browseDumpFileLenOutRange()
{
    QFileDialog file_dialog(this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getSaveFileName( this, tr( "Dump sequences with out-of-range lengths to" ), pwd, tr( "TXT File (*.txt);;All File (*.*)" )) ;
    if(filename.isEmpty()){
        return;
    }

    //Open Sequence File.
    QFile file(filename);
    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::about(0, "File Error", "File can not be opened.\nPlease check your file, the file name should not exceed 64 characters.");
        return;
    }
    file.close();
    ui->lineEdit_lenOutRangeFile->setText( filename);
}

bool pSeqsInputPage::validatePage()
{
    if( ui->lineEdit_tar->text().length() <= 1 ){
        QMessageBox::about(this, "MIssing Info","Please input your target sequence!");
        return false;
    }

    if( ui->lineEdit_nonTar->text().length() <= 1 ){
        QMessageBox::about(this, "MIssing Info","Please input your non-target sequence!");
        return false;
    }

    if( ui->checkBox_dumpDup->isChecked() && ui->lineEdit_dupFile->text().length() <= 1 ){
        QMessageBox::about(this, "MIssing Info","Please locate the file to output duplicated sequences!\nYou may choose to delete those sequences.");
        return false;
    }

    if( ui->checkBox_dumpLenOutRange->isChecked() && ui->lineEdit_lenOutRangeFile->text().length() <= 1 ){
        QMessageBox::about(this, "MIssing Info","Please locate the file to output sequences not within the specified range!\nYou may choose to delete those sequences.");
        return false;
    }

    int minLen, maxLen;
    bool bMin, bMax;
    minLen = ui->lineEdit_lenMin->text().toInt(&bMin);
    maxLen = ui->lineEdit_lenMax->text().toInt(&bMax);

    if(minLen == 0 || maxLen == 0 ||
       !bMin || !bMax){
        QMessageBox::about(this, "Error Length Range","Please make sure that the range of sequence length is valided .");
        return false;
    }else if( minLen >= maxLen ){
        QMessageBox::about(this, "Error Length Range","Min length must be smaller than Max length.");
        return false;
    }else if( maxLen > MAX_SEQUENCE_LEN ){
        QMessageBox::about(this, "Error Length Range","Sequence length cannot exceed 8000 nucleotides!\n");
        return false;
    }

    targetSeqFilename = ui->lineEdit_tar->text();
    nontargetSeqFilename = ui->lineEdit_nonTar->text();
    dupSeqFilename = ui->lineEdit_dupFile->text();
    lenOutRangeSeqFilename = ui->lineEdit_lenOutRangeFile->text();
   
    rmDupSeqs = ui->radioButton_removeDup->isChecked();
    rmLenOutRangeSeqs = ui->radioButton_removeLenOutRange->isChecked();

    dpDupSeqs = ui->checkBox_dumpDup->isChecked();
    dpLenOutRangeSeqs = ui->checkBox_dumpLenOutRange->isChecked();

    seqLenMin = minLen;
    seqLenMax = maxLen;

    return ((QWizardPage)this).validatePage();
}

void pSeqsInputPage::on_radioButton_keepDup_clicked(bool checked)
{
    if(checked){
        ui->checkBox_dumpDup->setChecked(false);
        ui->checkBox_dumpDup->setCheckable(false);
        ui->lineEdit_dupFile->setEnabled(false);
    }
}

void pSeqsInputPage::on_radioButton_removeDup_clicked(bool checked)
{
    if(checked){
        ui->checkBox_dumpDup->setCheckable(true);
        ui->lineEdit_dupFile->setEnabled(true);
    }
}

void pSeqsInputPage::on_radioButton_keepLenOutRange_clicked(bool checked)
{
    if(checked){
        ui->checkBox_dumpLenOutRange ->setChecked(false);
        ui->checkBox_dumpLenOutRange->setCheckable(false);
        ui->lineEdit_lenOutRangeFile ->setEnabled(false);
    }
}

void pSeqsInputPage::on_radioButton_removeLenOutRange_clicked(bool checked)
{
    if(checked){
        ui->checkBox_dumpLenOutRange->setCheckable(true);
        ui->lineEdit_lenOutRangeFile->setEnabled(true);
    }
}

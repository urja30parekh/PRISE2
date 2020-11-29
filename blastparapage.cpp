#include "blastparapage.h"
#include "ui_blastparapage.h"
#include <assert.h>

BlastParaPage::BlastParaPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::BlastParaPage)
{
    ui->setupUi(this);
    //QVector<int> v({1, 2, 3, 4, 5});

//    QVector<int> v = QVector<int>::QVector({1,23,4,5});

    for(int i = 0; i < optionMaxTarSeq.size(); i++){
        ui->comboBox_maxTarSeqs->addItem(QString::number(optionMaxTarSeq[i]));
    }




    for(int i = 0; i < optionWordSize.size(); i++){
        ui->comboBox_wordSize->addItem(QString::number(optionWordSize[i]));
    }





    assert(optionReward.size() == optionPenalty.size());
    for(int i = 0; i < optionReward.size(); i++){
        ui->comboBox_reward_penalty->addItem(QString("%1, %2").arg(optionReward[i]).arg(optionPenalty[i]));
    }



    ui->comboBox_gapCost->addItem("Linear");
    assert(optionGapOpen.size() == optionGapExtend.size());
    for(int i = 1; i < optionGapOpen.size(); i++){
        ui->comboBox_gapCost->addItem(QString("%1, %2").arg(optionGapOpen[i]).arg(optionGapExtend[i]));
    }
}

BlastParaPage::~BlastParaPage()
{
    delete ui;
}

int BlastParaPage::nextId() const
{
    return -1;
}

void BlastParaPage::changeEvent(QEvent *e)
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


bool BlastParaPage::validatePage()
{
    idxMaxTarSeqs = ui->comboBox_maxTarSeqs->currentIndex();
    eVal = ui->spinBox_evalue->value();
    idxWordSize = ui->comboBox_wordSize->currentIndex();
    idxMatchScore = ui->comboBox_reward_penalty->currentIndex();
    idxGap = ui->comboBox_gapCost->currentIndex();
    dustFilter = ui->checkBox_dust->isChecked();
    lowercaseMask = ui->checkBox_lowerCase->isChecked();
//    blastSeedFile = ui->lineEdit_SeedSeqFile->text();
//    if(blastSeedFile.isEmpty() ){
//        QMessageBox::about(this, "MIssing Info","Please input seed sequences in FASTA format");
//        return false;
//    }

//    databaseName = ui->lineEdit_database->text();
//    if(databaseName.isEmpty()){
//        QMessageBox::about(this, "MIssing Info","Please input a database name to search");
//        return false;
//    }

//    entrezQuery = ui->lineEdit_entrezQuery->text();

//    if(ui->radioButton_megablast->isChecked()){
//        blastType = 0;
//    }else if (ui->radioButton_discMegablast->isChecked()){
//        blastType = 1;
//    }else{
//        assert(ui->radioButton_blastn->isChecked());
//        blastType = 2;
//    }

    return QWizardPage::validatePage();
}

void BlastParaPage::initializePage()
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

    ui->comboBox_maxTarSeqs->setCurrentIndex(idxMaxTarSeqs);
    ui->spinBox_evalue->setValue(eVal);

    if(blastType == "megablast")
        idxWordSize = 5;
    else if (blastType == "blastn-short")
        idxWordSize = 0;
    else
        idxWordSize = 1;


    if(blastType == "megablast")
        idxMatchScore = 0;
    else if (blastType == "blastn-short")
        idxMatchScore = 1;
    else
        idxMatchScore = 3;




    if(blastType == "megablast")
        idxGap = 0;
    else
        idxGap = 1;


    ui->comboBox_wordSize->setCurrentIndex(idxWordSize);
    ui->comboBox_reward_penalty->setCurrentIndex(idxMatchScore);

    ui->comboBox_gapCost->setCurrentIndex(idxGap);

    ui->checkBox_dust->setChecked(dustFilter);
    ui->checkBox_lowerCase->setChecked(lowercaseMask);

}



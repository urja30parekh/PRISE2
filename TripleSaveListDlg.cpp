#include "TripleSaveListDlg.h"
#include "ui_TripleSaveListDlg.h"

TripleSaveListDlg::TripleSaveListDlg(/*bool saveProbes,*/QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TripleSaveListDlg)
{
    ui->setupUi(this);
    setWindowTitle("Save Primer-Probe Sets");

    /*this->saveProbes = saveProbes;
    if(saveProbes){
        this->setWindowTitle("Save Probes Nucleotides Only");
    }*/

    //TripleReportWindow *pWindow = (TripleReportWindow*)this->parentWidget();
    //int n1 = tripleList.size();
    //int n2 = tripleShowList.size();
    /*nMarkedTriples = 0;
    int nShowData = pWindow->tableModel->rowCount();
    for(int i = 0; i < pWindow->tableModel->rowCount(); i++){
        if(pWindow->tableModel->item(i,0)->checkState() == Qt::Checked)
            nMarkedTriples++;
    }*/

    // probe pair # in memory
    int n1 = ((TripleReportFrame*)parent)->getTripleListSize();
    // probe pair # in probe table
    int n2 = ((TripleReportFrame*)parent)->getTripleShowListSize();
    // count marked probe pairs
    nMarkedTriples = ((TripleReportFrame*)parent)->getMarkedNum();

    QString str;
    str = QString("%1 probes in memory.\n"
                  "%2 probes are displayed in probe list table.\n"
                  "%3 probes are marked.").arg(n1).arg(n2).arg(nMarkedTriples);
    ui->label->setText(str);

    if(nMarkedTriples == 0){
        ui->radioButton_marked->setEnabled(false);
        if(tripleExcelSaveAllDispMark == 2)
            tripleExcelSaveAllDispMark = 1;
    }

    switch(tripleListSaveAllDispMark){
    case 0:
        ui->radioButton_all->setChecked(true);
        ui->radioButton_displayed->setChecked(false);
        ui->radioButton_marked->setChecked(false);
        break;
    case 1:
        ui->radioButton_all->setChecked(false);
        ui->radioButton_displayed->setChecked(true);
        ui->radioButton_marked->setChecked(false);
        break;
    case 2:
        ui->radioButton_all->setChecked(false);
        ui->radioButton_displayed->setChecked(false);
        ui->radioButton_marked->setChecked(true);
        break;
    }
}

TripleSaveListDlg::~TripleSaveListDlg()
{
    delete ui;
}

void TripleSaveListDlg::accept()
{
    if(ui->radioButton_marked->isChecked() && nMarkedTriples <= 0){
        QMessageBox::about(this, "Empty List", "There is no probe pair marked");
        return;
    }

    if(ui->radioButton_all->isChecked()){
        tripleListSaveAllDispMark = 0;          //0 for save all
    }else if(ui->radioButton_displayed->isChecked()){
        tripleListSaveAllDispMark = 1;          //1 for save displayed
    }else if(ui->radioButton_marked->isChecked()){
        tripleListSaveAllDispMark = 2;          //2 for save marked
    }
    QDialog::accept();
}

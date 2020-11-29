#include "PrimerSaveListDlg.h"
#include "ui_PrimerSaveListDlg.h"

/*PrimerSaveListDlg::PrimerSaveListDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrimerSaveListDlg)
{
    ui->setupUi(this);
}*/

PrimerSaveListDlg::PrimerSaveListDlg(/*bool savePrimerPairs,*/QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrimerSaveListDlg)
{
    ui->setupUi(this);
    this->setWindowTitle("Save Primer Pairs");

    /*this->savePrimerPairs = savePrimerPairs;
    if(savePrimerPairs){
        this->setWindowTitle("Save Primer pairs Nucleotides Only");
    }*/

    PrimerReportWindow *pWindow = (PrimerReportWindow*)this->parentWidget();

    int n1 = primerPairList.size();
    int n2 = primerPairShowList.size();

    nMarkedPrimerPairs = 0;
    //int nShowData = pWindow->tableModel->rowCount();
    for(int i = 0; i < pWindow->tableModel->rowCount(); i++){
        if(pWindow->tableModel->item(i,0)->checkState() == Qt::Checked)
            nMarkedPrimerPairs++;
    }

    QString str;
    str = QString("%1 primer pairs in memory\n"
                  "%2 primer pairs displayed in primer list table\n"
                  "%3 primer pairs marked").arg(n1).arg(n2).arg(nMarkedPrimerPairs);
    ui->label->setText(str);

    switch(primerListSaveAllDispMark){
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

PrimerSaveListDlg::~PrimerSaveListDlg()
{
    delete ui;
}

void PrimerSaveListDlg::accept()
{
    if(ui->radioButton_marked->isChecked() && nMarkedPrimerPairs <= 0){
        QMessageBox::about(this, "Empty List", "There is no primer pair marked");
        return;
    }

    if(ui->radioButton_all->isChecked()){
        primerListSaveAllDispMark = 0;          //0 for save all
    }else if(ui->radioButton_displayed->isChecked()){
        primerListSaveAllDispMark = 1;          //1 for save displayed
    }else if(ui->radioButton_marked->isChecked()){
        primerListSaveAllDispMark = 2;          //2 for save marked
    }
    QDialog::accept();
}

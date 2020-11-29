#include "PrimerSaveExcelDlg.h"
#include "ui_PrimerSaveExcelDlg.h"

PrimerSaveExcelDlg::PrimerSaveExcelDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrimerSaveExcelDlg)
{
    ui->setupUi(this);
    setWindowTitle("Save Primer Pairs");

    // get PrimerReportDlg window (parent window)
    PrimerReportWindow *pWindow = (PrimerReportWindow*)this->parentWidget();

    // primer pair # in memory
    int n1 = (int) primerPairList.size();
    // primer pair # in primer table
    int n2 = (int) primerPairShowList.size();
    // count marked primer pairs
    nMarkedPrimerPairs = 0;
    for(int i = 0; i < pWindow->tableModel->rowCount(); i++){
        if(pWindow->tableModel->item(i,0)->checkState() == Qt::Checked)
            nMarkedPrimerPairs++;
    }

    QString str;
    str = QString("%1 primer pairs in memory\n"
                  "%2 primer pairs displayed in primer list table\n"
                  "%3 primer pairs marked").arg(n1).arg(n2).arg(nMarkedPrimerPairs);
    ui->label->setText(str);

    switch(primerExcelSaveAllDispMark){
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


    // display "add" and "delete" columns
    for( int i = 0; i < A_NUM_COLUMNS; i++ ){
        str = QString("%1").arg(i+1);
        if( i < 9){
            str = "col  " + str;
        }else{
            str = "col " + str;
        }


        QString colName = primerColumns[i];
        colName.replace("-\n","");
        colName.replace('\n',' ');

        if(primerExcelSaveColumns[i]){
            QListWidgetItem *_item = new QListWidgetItem(ui->listWidget_add);
            _item->setText(QApplication::translate("PrimerSaveExcelDlg", QString(str + ". " + colName).toLatin1(), 0));
        }else{
            QListWidgetItem *_item = new QListWidgetItem(ui->listWidget_delete);
            _item->setText(QApplication::translate("PrimerSaveExcelDlg", QString(str + ". " + colName).toLatin1(), 0));
        }
    }
}

PrimerSaveExcelDlg::~PrimerSaveExcelDlg()
{
    delete ui;
}

void PrimerSaveExcelDlg::accept()
{
    if( ui->radioButton_marked->isChecked() && nMarkedPrimerPairs <= 0 ){
        QMessageBox::about(this, "Empty List", "There's no primer pair marked");
        return;
    }

    if( ui->radioButton_all->isChecked() )
        primerExcelSaveAllDispMark = 0;	// 0 for save all
    else if( ui->radioButton_displayed->isChecked() )
        primerExcelSaveAllDispMark = 1;	// 1 for save displayed
    else if( ui->radioButton_marked->isChecked() )
        primerExcelSaveAllDispMark = 2;	// 2 for save marked


    QString itemText;

    for( int i = 0; i < ui->listWidget_add->count(); i++ ){
        itemText = ui->listWidget_add->item(i)->text();
        itemText.remove(0,8);
        for( int j = 0; j < A_NUM_COLUMNS; j++ ){

            QString colName = primerColumns[j];
            colName.replace("-\n","");
            colName.replace('\n',' ');

            if( itemText == colName ){
                primerExcelSaveColumns[j] = 1;
                break;
            }
        }
    }
    for( int i = 0; i < ui->listWidget_delete->count(); i++ ){
        itemText = ui->listWidget_delete->item(i)->text();
        itemText.remove(0, 8);
        for( int j = 0; j < A_NUM_COLUMNS; j++ ){

            QString colName = primerColumns[j];
            colName.replace("-\n","");
            colName.replace('\n',' ');

            if( itemText == colName ){
                primerExcelSaveColumns[j] = 0;
                break;
            }
        }
    }
    QDialog::accept();
}

void PrimerSaveExcelDlg::addColumn()
{
    QList<QListWidgetItem*> selected = ui->listWidget_delete->selectedItems();

    int nSel = selected.size();
    if( nSel <= 0 )	// nothing is selected
            return;

    // add selected items to Disp box
    // delete selected items from Hide box
    int i = 0;
    while( i < nSel){
        int row = ui->listWidget_delete->row(selected[i]);
        QListWidgetItem* item = ui->listWidget_delete->takeItem(row);
        ui->listWidget_add->addItem(item);
        i++;
    }
}

void PrimerSaveExcelDlg::deleteColumn()
{
    QList<QListWidgetItem*> selected = ui->listWidget_add->selectedItems();

    int nSel = selected.size();
    if( nSel <= 0 )	// nothing is selected
            return;

    // add selected items to Hide box
    // delete selected items from Disp box
    int i = 0;
    while( i < nSel){
        int row = ui->listWidget_add->row(selected[i]);
        QListWidgetItem* item = ui->listWidget_add->takeItem(row);
        ui->listWidget_delete->addItem(item);
        i++;
    }
}

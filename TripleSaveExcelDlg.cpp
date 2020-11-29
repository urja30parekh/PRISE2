#include "TripleSaveExcelDlg.h"
#include "ui_TripleSaveExcelDlg.h"

TripleSaveExcelDlg::TripleSaveExcelDlg(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::TripleSaveExcelDlg)
{
    ui->setupUi(this);
    setWindowTitle("Save Primer-Probe Sets");


    // get ProbeReportDlg window (parent window)
    //TripleReportFrame *pWidget = (TripleReportFrame*)this->parentWidget();

    // probe pair # in memory
    int n1 = ((TripleReportFrame*)parent)->getTripleListSize();
    // probe pair # in probe table
    int n2 = ((TripleReportFrame*)parent)->getTripleShowListSize();
    // count marked probe pairs
    nMarkedTriples = ((TripleReportFrame*)parent)->getMarkedNum();

    QString str;
    str = QString("%1 probes in memory\n"
                  "%2 probes displayed in probe list table\n"
                  "%3 probes are marked").arg(n1).arg(n2).arg(nMarkedTriples);
    ui->label->setText(str);

    if(nMarkedTriples == 0){
        ui->radioButton_marked->setEnabled(false);
        if(tripleExcelSaveAllDispMark == 2)
            tripleExcelSaveAllDispMark = 1;
    }

    switch(tripleExcelSaveAllDispMark){
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
    for( int i = 0; i < B_NUM_COLUMNS; i++ ){
        str = QString("%1").arg(i+1);
        if( i < 9){
            str = "col  " + str;
        }else{
            str = "col " + str;
        }


        QString colName = tripleColumns[i];
        colName.replace("-\n","");
        colName.replace('\n',' ');


        if(tripleExcelSaveColumns[i]){
            QListWidgetItem *_item = new QListWidgetItem(ui->listWidget_add);
            _item->setText(QApplication::translate("TripleSaveExcelDlg", QString(str + ". " + colName).toLatin1(), 0));
        }else{
            QListWidgetItem *_item = new QListWidgetItem(ui->listWidget_delete);
            _item->setText(QApplication::translate("TripleSaveExcelDlg", QString(str + ". " + colName).toLatin1(), 0));
        }
    }
}

TripleSaveExcelDlg::~TripleSaveExcelDlg()
{
    delete ui;
}

void TripleSaveExcelDlg::accept()
{
    if( ui->radioButton_marked->isChecked() && nMarkedTriples <= 0 ){
        QMessageBox::about(this, "Empty List", "There's no probe marked");
        return;
    }

    if( ui->radioButton_all->isChecked() )
        tripleExcelSaveAllDispMark = 0;	// 0 for save all
    else if( ui->radioButton_displayed->isChecked() )
        tripleExcelSaveAllDispMark = 1;	// 1 for save displayed
    else if( ui->radioButton_marked->isChecked() )
        tripleExcelSaveAllDispMark = 2;	// 2 for save marked


    QString itemText;

    for( int i = 0; i < ui->listWidget_add->count(); i++ ){
        itemText = ui->listWidget_add->item(i)->text();
        itemText.remove(0,8);
        for( int j = 0; j < B_NUM_COLUMNS; j++ ){

            QString colName = tripleColumns[j];
            colName.replace("-\n","");
            colName.replace('\n',' ');

            if( itemText == colName ){
                tripleExcelSaveColumns[j] = 1;
                break;
            }
        }
    }
    for( int i = 0; i < ui->listWidget_delete->count(); i++ ){
        itemText = ui->listWidget_delete->item(i)->text();
        itemText.remove(0, 8);
        for( int j = 0; j < B_NUM_COLUMNS; j++ ){

            QString colName = tripleColumns[j];
            colName.replace("-\n","");
            colName.replace('\n',' ');

            if( itemText == colName ){
                tripleExcelSaveColumns[j] = 0;
                break;
            }
        }
    }
    QDialog::accept();
}

void TripleSaveExcelDlg::addColumn()
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

void TripleSaveExcelDlg::deleteColumn()
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

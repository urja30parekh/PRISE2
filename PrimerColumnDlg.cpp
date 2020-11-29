#include "PrimerColumnDlg.h"
#include "ui_PrimerColumnDlg.h"
#include <algorithm> //sorting function for data.

inline bool lt(int a, int b) { return a < b; }

PrimerColumnDlg::PrimerColumnDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrimerColumnDlg)
{
    ui->setupUi(this);

    int index;
    QString str;

    for( int i = 0; i < primerColumnDisp.size(); i++ ){
        // find the correct index in original column
        /*index = 0;
        while( index < primerColumns.size() ){
            if( primerColumnDisp[i] == primerColumns[index] )
                break;
            else
                index++;
        }*/
        index = primerColumnDisp[i];
        str = QString("%1").arg(index+1);
        if( index < 9 )
            str = "col  " + str;
        else
            str = "col " + str;

        QListWidgetItem *_item = new QListWidgetItem(ui->listWidget_disp);
        _item->setText(QApplication::translate("PrimerSaveExcelDlg", QString(str + ". " + primerColumns[index]).toLatin1().replace("-\n", "").replace('\n', ' '), 0));
    }
    for( int i = 0; i < primerColumnHide.size(); i++ ){
        // find the correct index in original column
        /*index = 0;
        while( index < primerColumns.size() ){
            if( primerColumnHide[i] == primerColumns[index] )
                break;
            else
                index++;
        }*/
        index = primerColumnHide[i];
        str = QString("%1").arg(index+1);
        if( index < 9 )
            str = "col  " + str;
        else
            str = "col " + str;

        QListWidgetItem *_item = new QListWidgetItem(ui->listWidget_hide);
        _item->setText(QApplication::translate("PrimerSaveExcelDlg", QString(str + ". " + primerColumns[index]).toLatin1().replace("-\n", "").replace('\n', ' '), 0));
    }
}

PrimerColumnDlg::~PrimerColumnDlg()
{
    delete ui;
}


void PrimerColumnDlg::hideColumn()
{

    QList<QListWidgetItem*> selected = ui->listWidget_disp->selectedItems();

    int nSel = selected.size();
    if( nSel <= 0 )	// nothing is selected
            return;

    // add selected items to Hide box
    // delete selected items from Disp box
    int i = 0;
    while( i < nSel){
        int row = ui->listWidget_disp->row(selected[i]);
        int col_num = primerColumnDisp[row];
        QListWidgetItem* item = ui->listWidget_disp->takeItem(row);
        ui->listWidget_hide->addItem(item);
        primerColumnHide.push_back(col_num);
        primerColumnDisp.remove(row);
        i++;
    }
    /*while(i >= 0){
        int row = ui->listWidget_disp->row(selected[i]);
        int col_num = primerColumnDisp[row];
        QListWidgetItem* item = ui->listWidget_disp->takeItem(row);
        ui->listWidget_hide->addItem(item);
        primerColumnHide.push_front(primerColumnDisp[row]);
        primerColumnDisp.remove(row);
        i--;
    }*/

}

void PrimerColumnDlg::dispColumn()
{

    QList<QListWidgetItem*> selected = ui->listWidget_hide->selectedItems();

    int nSel = selected.size();
    if( nSel <= 0 )	// nothing is selected
            return;

    // add selected items to Hide box
    // delete selected items from Disp box
    int i = 0;
    while( i < nSel){
        int row = ui->listWidget_hide->row(selected[i]);
        int col_num = primerColumnHide[row];
        QListWidgetItem* item = ui->listWidget_hide->takeItem(row);
        ui->listWidget_disp->addItem(item);
        primerColumnDisp.push_back(col_num);
        primerColumnHide.remove(row);
        i++;
    }
    /*int i = nSel-1;
    while(i >= 0){
        int row = ui->listWidget_disp->row(selected[i]);
        int col_num = primerColumnDisp[row];
        QListWidgetItem* item = ui->listWidget_disp->takeItem(row);
        ui->listWidget_hide->addItem(item);
        primerColumnDisp.push_front(primerColumnHide[row]);
        primerColumnHide.remove(row);
        i--;
    }*/
}

void PrimerColumnDlg::accept()
{
    QString itemText;

    std::stable_sort( primerColumnDisp.begin(), primerColumnDisp.end(), lt );
    std::stable_sort( primerColumnHide.begin(), primerColumnHide.end(), lt );
/*
    primerColumnHide.clear();
    for( int i = 0; i < ui->listWidget_hide->count(); i++ ){
        itemText = ui->listWidget_hide->item(i)->text();
        itemText.remove(0,8);

        primerColumnHide.push_back(itemText);
    }
    primerColumnDisp.clear();
    for( int i = 0; i < ui->listWidget_disp->count(); i++ ){
        itemText = ui->listWidget_disp->item(i)->text();
        itemText.remove(0,8);

        primerColumnDisp.push_back(itemText);
    }*/

    QDialog::accept();
}

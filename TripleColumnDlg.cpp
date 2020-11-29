#include "TripleColumnDlg.h"
#include "ui_TripleColumnDlg.h"
#include <algorithm> //sorting function for data.

inline bool lt(int a, int b) { return a < b; }

TripleColumnDlg::TripleColumnDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TripleColumnDlg)
{
    ui->setupUi(this);

    int index;
    QString str;

    cDisp = &((TripleReportFrame*)parent)->tripleColumnDisp;
    for( int i = 0; i < cDisp->size(); i++ ){
        // find the correct index in original column
        index = cDisp->at(i);
        str = QString("%1").arg(index+1);
        if( index < 9 )
            str = "col  " + str;
        else
            str = "col " + str;

        QListWidgetItem *_item = new QListWidgetItem(ui->listWidget_disp);
        _item->setText(QApplication::translate("TripleColumnDlg", QString(str + ". " + tripleColumns[index]).toLatin1().replace("-\n", "").replace('\n', ' '), 0));
    }

    cHide = &((TripleReportFrame*)parent)->tripleColumnHide;
    for( int i = 0; i < cHide->size(); i++ ){
        // find the correct index in original column
        index = cHide->at(i);
        str = QString("%1").arg(index+1);
        if( index < 9 )
            str = "col  " + str;
        else
            str = "col " + str;

        QListWidgetItem *_item = new QListWidgetItem(ui->listWidget_hide);
        _item->setText(QApplication::translate("TripleColumnDlg", QString(str + ". " + tripleColumns[index]).toLatin1().replace("-\n", "").replace('\n', ' '), 0));
    }
}

TripleColumnDlg::~TripleColumnDlg()
{
    delete ui;
}


void TripleColumnDlg::hideColumn()
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
        int col_num = cDisp->at(row);
        QListWidgetItem* item = ui->listWidget_disp->takeItem(row);
        ui->listWidget_hide->addItem(item);
        cHide->push_back(col_num);
        cDisp->remove(row);
        i++;
    }
}

void TripleColumnDlg::dispColumn()
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
        int col_num = cHide->at(row);
        QListWidgetItem* item = ui->listWidget_hide->takeItem(row);
        ui->listWidget_disp->addItem(item);
        cDisp->push_back(col_num);
        cHide->remove(row);
        i++;
    }
}

void TripleColumnDlg::accept()
{
    std::stable_sort( cDisp->begin(), cDisp->end(), lt );
    std::stable_sort( cHide->begin(), cHide->end(), lt );

    QDialog::accept();
}

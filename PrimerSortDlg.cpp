#include "PrimerSortDlg.h"
#include "ui_PrimerSortDlg.h"

PrimerSortDlg::PrimerSortDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrimerSortDlg)
{
    ui->setupUi(this);
    ui->comboBox->addItem("Selectivity formula");
    ui->comboBox_2->addItem("Selectivity formula");
    ui->comboBox_3->addItem("Selectivity formula");
    ui->comboBox_4->addItem("Selectivity formula");

    ui->comboBox->addItem("Selectivity formula");
    ui->comboBox_2->addItem("Selectivity formula");
    ui->comboBox_3->addItem("Selectivity formula");
    ui->comboBox_4->addItem("Selectivity formula");

    ui->comboBox->addItem("3' end mismatch rank in non-target (slow)");
    ui->comboBox_2->addItem("3' end mismatch rank in non-target (slow)");
    ui->comboBox_3->addItem("3' end mismatch rank in non-target (slow)");
    ui->comboBox_4->addItem("3' end mismatch rank in non-target (slow)");

    // skip columns[0]: "primer #"; columns[1]: "primer pair"
    for( int i = A_TARGET_PCTG; i < primerColumns.size(); i++ ){
        QString str;
        str = primerColumns[i].replace(QString("-\n"),QString(""));
        str = str.replace('\n',' ');

        ui->comboBox->addItem(str);
        ui->comboBox_2->addItem(str);
        ui->comboBox_3->addItem(str);
        ui->comboBox_4->addItem(str);
    }
    ui->comboBox->setCurrentIndex(primerSortBy[0]);
    ui->comboBox_2->setCurrentIndex(primerSortBy[1]);
    ui->comboBox_3->setCurrentIndex(primerSortBy[2]);
    ui->comboBox_4->setCurrentIndex(primerSortBy[3]);

    ui->radioButton_1a->setChecked(1 - primerSortOrder[0]);// 0 for ascending, 1 for descending
    ui->radioButton_1d->setChecked(primerSortOrder[0]);// 0 for ascending, 1 for descending

    ui->radioButton_2a->setChecked(1 - primerSortOrder[1]);// 0 for ascending, 1 for descending
    ui->radioButton_2d->setChecked(primerSortOrder[1]);// 0 for ascending, 1 for descending

    ui->radioButton_3a->setChecked(1 - primerSortOrder[2]);// 0 for ascending, 1 for descending
    ui->radioButton_3d->setChecked(primerSortOrder[2]);// 0 for ascending, 1 for descending

    ui->radioButton_4a->setChecked(1 - primerSortOrder[3]);// 0 for ascending, 1 for descending
    ui->radioButton_4d->setChecked(primerSortOrder[3]);// 0 for ascending, 1 for descending
}

PrimerSortDlg::~PrimerSortDlg()
{
    delete ui;
}

void PrimerSortDlg::accept()
{
    // get PrimerReportDlg window (parent window)
    primerSortBy[0] = ui->comboBox->currentIndex();
    primerSortBy[1] = ui->comboBox_2->currentIndex();
    primerSortBy[2] = ui->comboBox_3->currentIndex();
    primerSortBy[3] = ui->comboBox_4->currentIndex();

    primerSortOrder[0] = 1 - ui->radioButton_1a->isChecked();	// 0 for ascending, 1 for descending
    primerSortOrder[1] = 1 - ui->radioButton_2a->isChecked();	// 0 for ascending, 1 for descending
    primerSortOrder[2] = 1 - ui->radioButton_3a->isChecked();	// 0 for ascending, 1 for descending
    primerSortOrder[3] = 1 - ui->radioButton_4a->isChecked();	// 0 for ascending, 1 for descending

    QDialog::accept();
}

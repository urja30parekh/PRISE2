#include "TripleSortDlg.h"
#include "ui_TripleSortDlg.h"

TripleSortDlg::TripleSortDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TripleSortDlg)
{
    ui->setupUi(this);
    ui->comboBox->addItem("Selectivity Formula");
    ui->comboBox_2->addItem("Selectivity Formula");
    ui->comboBox_3->addItem("Selectivity Formula");
    ui->comboBox_4->addItem("Selectivity Formula");

    ui->comboBox->addItem("sequence");
    ui->comboBox_2->addItem("Sequence");
    ui->comboBox_3->addItem("Sequence");
    ui->comboBox_4->addItem("Sequence");

    // skip columns[0]: "probe #"; columns[1]: "probe"
    for( int i = B_TARGET_PCTG; i < tripleColumns.size(); i++ ){
        QString str;
        str = tripleColumns[i].replace(QString("-\n"),QString(""));
        str = str.replace('\n',' ');

        ui->comboBox->addItem(str);
        ui->comboBox_2->addItem(str);
        ui->comboBox_3->addItem(str);
        ui->comboBox_4->addItem(str);
    }
    ui->comboBox->setCurrentIndex(tripleSortBy[0]);
    ui->comboBox_2->setCurrentIndex(tripleSortBy[1]);
    ui->comboBox_3->setCurrentIndex(tripleSortBy[2]);
    ui->comboBox_4->setCurrentIndex(tripleSortBy[3]);

    ui->radioButton_1a->setChecked(1 - tripleSortOrder[0]);// 0 for ascending, 1 for descending
    ui->radioButton_1d->setChecked(tripleSortOrder[0]);// 0 for ascending, 1 for descending

    ui->radioButton_2a->setChecked(1 - tripleSortOrder[1]);// 0 for ascending, 1 for descending
    ui->radioButton_2d->setChecked(tripleSortOrder[1]);// 0 for ascending, 1 for descending

    ui->radioButton_3a->setChecked(1 - tripleSortOrder[2]);// 0 for ascending, 1 for descending
    ui->radioButton_3d->setChecked(tripleSortOrder[2]);// 0 for ascending, 1 for descending

    ui->radioButton_4a->setChecked(1 - tripleSortOrder[3]);// 0 for ascending, 1 for descending
    ui->radioButton_4d->setChecked(tripleSortOrder[3]);// 0 for ascending, 1 for descendin
}

TripleSortDlg::~TripleSortDlg()
{
    delete ui;
}

void TripleSortDlg::accept()
{
    // get PrimerReportDlg window (parent window)
    tripleSortBy[0] = ui->comboBox->currentIndex();
    tripleSortBy[1] = ui->comboBox_2->currentIndex();
    tripleSortBy[2] = ui->comboBox_3->currentIndex();
    tripleSortBy[3] = ui->comboBox_4->currentIndex();

    tripleSortOrder[0] = 1 - ui->radioButton_1a->isChecked();	// 0 for ascending, 1 for descending
    tripleSortOrder[1] = 1 - ui->radioButton_2a->isChecked();	// 0 for ascending, 1 for descending
    tripleSortOrder[2] = 1 - ui->radioButton_3a->isChecked();	// 0 for ascending, 1 for descending
    tripleSortOrder[3] = 1 - ui->radioButton_4a->isChecked();	// 0 for ascending, 1 for descending

    QDialog::accept();
}

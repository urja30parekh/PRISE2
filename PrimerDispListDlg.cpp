#include "PrimerDispListDlg.h"
#include "ui_PrimerDispListDlg.h"

PrimerDispListDlg::PrimerDispListDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrimerDispListDlg)
{
    ui->setupUi(this);
    ui->plaintext->insertPlainText(QString("There are %1 primer pairs.\n").arg(primerPairList.size())+"Recommended number of primer pairs to display: 500-1000.\nChoosing more pairs will slow the program.");
    //PrimerReportWindow* p = (PrimerReportWindow*)this->parent();
    for(int i = A_TARGET_PCTG; i < primerColumns.size(); i++){
        QString str;
        str = primerColumns[i].replace(QString("-\n"),QString(""));
        str = str.replace("\n"," ");

        ui->comboBox_cond1->addItem(str);
        ui->comboBox_cond2->addItem(str);
        ui->comboBox_cond3->addItem(str);
        ui->comboBox_cond4->addItem(str);
    }

    ui->comboBox_cond1->setCurrentIndex(-1);
    ui->comboBox_cond2->setCurrentIndex(-1);
    ui->comboBox_cond3->setCurrentIndex(-1);
    ui->comboBox_cond4->setCurrentIndex(-1);
    ui->comboBox_GTLT1->setCurrentIndex(-1);
    ui->comboBox_GTLT2->setCurrentIndex(-1);
    ui->comboBox_GTLT3->setCurrentIndex(-1);
    ui->comboBox_GTLT4->setCurrentIndex(-1);
    ui->lineEdit_value1->setText("0");
    ui->lineEdit_value2->setText("0");
    ui->lineEdit_value3->setText("0");
    ui->lineEdit_value4->setText("0");
    ui->label_percent1->setText("");
    ui->label_percent2->setText("");
    ui->label_percent3->setText("");
    ui->label_percent4->setText("");

    ui->checkBox_all->setChecked(false);
    ui->checkBox_topPrimers->setChecked(true);
}

PrimerDispListDlg::~PrimerDispListDlg()
{
    delete ui;
}

void PrimerDispListDlg::accept()
{
    // error checking

    if(ui->comboBox_cond1->currentIndex() >= 0 && ui->comboBox_GTLT1->currentIndex() < 0 ){
        QMessageBox::about(this,"Condition Error","Please choose the constraint for condition 1.");
        return;
    }
    if(ui->comboBox_cond2->currentIndex() >= 0 && ui->comboBox_GTLT2->currentIndex() < 0){
        QMessageBox::about(this,"Condition Error","Please choose the constraint for condition 2.");
        return;
    }
    if(ui->comboBox_cond3->currentIndex() >= 0 && ui->comboBox_GTLT3->currentIndex() < 0){
        QMessageBox::about(this,"Condition Error","Please choose the constraint for condition 3.");
        return;
    }
    if(ui->comboBox_cond4->currentIndex() >= 0 && ui->comboBox_GTLT4->currentIndex() < 0){
        QMessageBox::about(this,"Condition Error","Please choose the constraint for condition 4.");
        return;
    }


    // get PrimerPrimerReportDlg window (parent window)
    PrimerReportWindow* p = (PrimerReportWindow*)this->parent();

    if( ui->checkBox_all->isChecked())
        dispPrimerAllorTopX = 1;
    else if(ui->checkBox_topPrimers->isChecked())
        dispPrimerAllorTopX = 2;
    else
        dispPrimerAllorTopX = 0;

    bool b;
    if(ui->checkBox_topPrimers->isChecked())
    {
        int topNum;
        topNum = ui->lineEdit_topNum->text().toInt(&b);
        if(!b){
            QMessageBox::about(this,"Condition Error","Please input an integer percentage\n");
            return;
        }
        dispPrimerTopNum = topNum;
    }


    // skip columns[0]: "primer #"; columns[1]: "primer pair"
    dispPrimerCondTitle[0] = ui->comboBox_cond1->currentIndex() + 2;
    dispPrimerCondTitle[1] = ui->comboBox_cond2->currentIndex() + 2;
    dispPrimerCondTitle[2] = ui->comboBox_cond3->currentIndex() + 2;
    dispPrimerCondTitle[3] = ui->comboBox_cond4->currentIndex() + 2;
    dispPrimerCondGtLt[0] = ui->comboBox_GTLT1->currentIndex();
    dispPrimerCondGtLt[1] = ui->comboBox_GTLT2->currentIndex();
    dispPrimerCondGtLt[2] = ui->comboBox_GTLT3->currentIndex();
    dispPrimerCondGtLt[3] = ui->comboBox_GTLT4->currentIndex();


    QString tmpstr = ui->lineEdit_value1->text();
    float value = ui->lineEdit_value1->text().toFloat(&b);
    if(!b){
        QMessageBox::about(this,"Condition Error","Please input an integer percentage\n");
        return;
    }
    dispPrimerCondValue[0] = value;
    value = ui->lineEdit_value2->text().toFloat(&b);
    if(!b){
        QMessageBox::about(this,"Condition Error","Please input an integer percentage\n");
        return;
    }
    dispPrimerCondValue[1] = value;
    value = ui->lineEdit_value3->text().toFloat(&b);
    if(!b){
        QMessageBox::about(this,"Condition Error","Please input an integer percentage\n");
        return;
    }
    dispPrimerCondValue[2] = value;
    value = ui->lineEdit_value4->text().toFloat(&b);
    if(!b){
        QMessageBox::about(this,"Condition Error","Please input an integer percentage\n");
        return;
    }
    dispPrimerCondValue[3] = value;

    QDialog::accept();
}

void PrimerDispListDlg::reject()
{
    QDialog::reject();
}



void PrimerDispListDlg::changeIdx_1(int i)
{
    if(i < 0)
        return;
    ui->checkBox_all->setChecked(false);
    ui->checkBox_topPrimers->setChecked(false);

    QString title = ui->comboBox_cond1->itemText(i);
    if (title.indexOf('%') > 0)
        ui->label_percent1->setText("%");
    else
        ui->label_percent1->setText(" ");
}

void PrimerDispListDlg::changeIdx_2(int i)
{
    if(i < 0)
        return;
    ui->checkBox_all->setChecked(false);
    ui->checkBox_topPrimers->setChecked(false);

    QString title = ui->comboBox_cond2->itemText(i);
    if (title.indexOf('%') > 0)
        ui->label_percent2->setText("%");
    else
        ui->label_percent2->setText(" ");
}

void PrimerDispListDlg::changeIdx_3(int i)
{
    if(i < 0)
        return;
    ui->checkBox_all->setChecked(false);
    ui->checkBox_topPrimers->setChecked(false);

    QString title = ui->comboBox_cond3->itemText(i);
    if (title.indexOf('%') > 0)
        ui->label_percent3->setText("%");
    else
        ui->label_percent3->setText(" ");
}

void PrimerDispListDlg::changeIdx_4(int i)
{
    if(i < 0)
        return;
    ui->checkBox_all->setChecked(false);
    ui->checkBox_topPrimers->setChecked(false);

    QString title = ui->comboBox_cond4->itemText(i);
    if (title.indexOf('%') > 0)
        ui->label_percent4->setText("%");
    else
        ui->label_percent4->setText(" ");
}

void PrimerDispListDlg::displayAll()
{
    if(ui->checkBox_all->isChecked()){
        ui->checkBox_topPrimers->setChecked(false);
        ui->comboBox_cond1->setCurrentIndex(-1);
        ui->comboBox_cond2->setCurrentIndex(-1);
        ui->comboBox_cond3->setCurrentIndex(-1);
        ui->comboBox_cond4->setCurrentIndex(-1);
        ui->comboBox_GTLT1->setCurrentIndex(-1);
        ui->comboBox_GTLT2->setCurrentIndex(-1);
        ui->comboBox_GTLT3->setCurrentIndex(-1);
        ui->comboBox_GTLT4->setCurrentIndex(-1);
        ui->lineEdit_value1->setText("0");
        ui->lineEdit_value2->setText("0");
        ui->lineEdit_value3->setText("0");
        ui->lineEdit_value4->setText("0");
        ui->label_percent1->setText("");
        ui->label_percent2->setText("");
        ui->label_percent3->setText("");
        ui->label_percent4->setText("");
    }
}

void PrimerDispListDlg::displayTop()
{
    if(ui->checkBox_topPrimers->isChecked()){
        ui->checkBox_all->setChecked(false);
        ui->comboBox_cond1->setCurrentIndex(-1);
        ui->comboBox_cond2->setCurrentIndex(-1);
        ui->comboBox_cond3->setCurrentIndex(-1);
        ui->comboBox_cond4->setCurrentIndex(-1);
        ui->comboBox_GTLT1->setCurrentIndex(-1);
        ui->comboBox_GTLT2->setCurrentIndex(-1);
        ui->comboBox_GTLT3->setCurrentIndex(-1);
        ui->comboBox_GTLT4->setCurrentIndex(-1);
        ui->lineEdit_value1->setText("0");
        ui->lineEdit_value2->setText("0");
        ui->lineEdit_value3->setText("0");
        ui->lineEdit_value4->setText("0");
        ui->label_percent1->setText("");
        ui->label_percent2->setText("");
        ui->label_percent3->setText("");
        ui->label_percent4->setText("");
    }
}

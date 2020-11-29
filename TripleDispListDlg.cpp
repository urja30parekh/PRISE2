#include "TripleDispListDlg.h"
#include "ui_TripleDispListDlg.h"

TripleDispListDlg::TripleDispListDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TripleDispListDlg)
{
    ui->setupUi(this);
    ui->plaintext->insertPlainText(QString("There are %1 triples.\n").arg(((TripleReportFrame*)parent)->getTripleListSize())+"It's recommended to display top 500~1000 triples.");
    //PrimerReportWindow* p = (PrimerReportWindow*)this->parent();
    for(int i = B_TARGET_PCTG; i < tripleColumns.size(); i++){
        QString str;
        str = tripleColumns[i].replace(QString("-\n"),QString(""));
        str = str.replace('\n',' ');


        ui->comboBox_cond1->addItem(str);
        ui->comboBox_cond2->addItem(str);
        ui->comboBox_cond3->addItem(str);
        ui->comboBox_cond4->addItem(str);
    }

    //ui->buttonBox->button(QDialogButtonBox::Cancel)->hide();
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
    ui->checkBox_topProbes->setChecked(true);
}

TripleDispListDlg::~TripleDispListDlg()
{
    delete ui;
}

void TripleDispListDlg::accept()
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


    // get ProbeReportDlg window (parent window)
    //TripleReportWindow* p = (TripleReportWindow*)this->parent();

    if( ui->checkBox_all->isChecked())
        dispTripleAllorTopX = 1;
    else if(ui->checkBox_topProbes->isChecked())
        dispTripleAllorTopX = 2;
    else
        dispTripleAllorTopX = 0;

    bool b;

    if(ui->checkBox_topProbes->isChecked())
    {
        int topNum;
        topNum = ui->lineEdit_topNum->text().toInt(&b);
        if(!b){
            QMessageBox::about(this,"Condition Error","Please input an integer percentage\n");
            return;
        }
        dispTripleTopNum = topNum;
    }

    // skip columns[0]: "probe #"; columns[1]: "probe sequence"
    dispTripleCondTitle[0] = ui->comboBox_cond1->currentIndex() + 2;
    dispTripleCondTitle[1] = ui->comboBox_cond2->currentIndex() + 2;
    dispTripleCondTitle[2] = ui->comboBox_cond3->currentIndex() + 2;
    dispTripleCondTitle[3] = ui->comboBox_cond4->currentIndex() + 2;
    dispTripleCondGtLt[0] = ui->comboBox_GTLT1->currentIndex();
    dispTripleCondGtLt[1] = ui->comboBox_GTLT2->currentIndex();
    dispTripleCondGtLt[2] = ui->comboBox_GTLT3->currentIndex();
    dispTripleCondGtLt[3] = ui->comboBox_GTLT4->currentIndex();


    QString tmpstr = ui->lineEdit_value1->text();
    float value = ui->lineEdit_value1->text().toFloat(&b);
    if(!b){
        QMessageBox::about(this,"Condition Error","Please input an integer percentage\n");
        return;
    }
    dispTripleCondValue[0] = value;
    value = ui->lineEdit_value2->text().toFloat(&b);
    if(!b){
        QMessageBox::about(this,"Condition Error","Please input an integer percentage\n");
        return;
    }
    dispTripleCondValue[1] = value;
    value = ui->lineEdit_value3->text().toFloat(&b);
    if(!b){
        QMessageBox::about(this,"Condition Error","Please input an integer percentage\n");
        return;
    }
    dispTripleCondValue[2] = value;
    value = ui->lineEdit_value4->text().toFloat(&b);
    if(!b){
        QMessageBox::about(this,"Condition Error","Please input an integer percentage\n");
        return;
    }
    dispTripleCondValue[3] = value;

    QDialog::accept();
}

void TripleDispListDlg::reject()
{
    QDialog::reject();
}

void TripleDispListDlg::changeIdx_1(int i)
{
    if(i < 0)
        return;
    ui->checkBox_all->setChecked(false);
    ui->checkBox_topProbes->setChecked(false);

    QString title = ui->comboBox_cond1->itemText(i);
    if (title.indexOf('%') > 0)
        ui->label_percent1->setText("%");
    else
        ui->label_percent1->setText(" ");
}

void TripleDispListDlg::changeIdx_2(int i)
{
    if(i < 0)
        return;
    ui->checkBox_all->setChecked(false);
    ui->checkBox_topProbes->setChecked(false);

    QString title = ui->comboBox_cond2->itemText(i);
    if (title.indexOf('%') > 0)
        ui->label_percent2->setText("%");
    else
        ui->label_percent2->setText(" ");
}

void TripleDispListDlg::changeIdx_3(int i)
{
    if(i < 0)
        return;
    ui->checkBox_all->setChecked(false);
    ui->checkBox_topProbes->setChecked(false);

    QString title = ui->comboBox_cond3->itemText(i);
    if (title.indexOf('%') > 0)
        ui->label_percent3->setText("%");
    else
        ui->label_percent3->setText(" ");
}

void TripleDispListDlg::changeIdx_4(int i)
{
    if(i < 0)
        return;
    ui->checkBox_all->setChecked(false);
    ui->checkBox_topProbes->setChecked(false);

    QString title = ui->comboBox_cond4->itemText(i);
    if (title.indexOf('%') > 0)
        ui->label_percent4->setText("%");
    else
        ui->label_percent4->setText(" ");
}

void TripleDispListDlg::displayAll()
{
    //bool test = ui->checkBox_all->isChecked();
    if(ui->checkBox_all->isChecked()){
        ui->checkBox_topProbes->setChecked(false);
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
    //test = ui->checkBox_all->isChecked();
}

void TripleDispListDlg::displayTop()
{
    //bool test = ui->checkBox_topProbes->isChecked();
    if(ui->checkBox_topProbes->isChecked()){
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
    //test = ui->checkBox_topProbes->isChecked();
}

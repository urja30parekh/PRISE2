#include "SeqFindDlg.h"
#include "ui_SeqFindDlg.h"

SeqFindDlg::SeqFindDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SeqFindDlg)
{
    ui->setupUi(this);
    setWindowTitle("Find Sequences");

    if(findConditionalCheck[0]) this->ui->checkBox_len->setCheckState(Qt::Checked);
    if(findConditionalCheck[1]) this->ui->checkBox_id->setCheckState(Qt::Checked);
    if(findConditionalCheck[2]) this->ui->checkBox_def->setCheckState(Qt::Checked);
    if(findConditionalCheck[3]) this->ui->checkBox_org->setCheckState(Qt::Checked);

    if (! isSimple){
        ui->comboBox_1->addItem("--select a key--");
        ui->comboBox_1->addItem("Sequence SOURCE");
        ui->comboBox_1->addItem("Sequence FEATURES");
        ui->comboBox_1->addItem("Sequence AUTHORS");
        ui->comboBox_1->addItem("Sequence TITLE");

        ui->comboBox_2->addItem("--select a key--");
        ui->comboBox_2->addItem("Sequence SOURCE");
        ui->comboBox_2->addItem("Sequence FEATURES");
        ui->comboBox_2->addItem("Sequence AUTHORS");
        ui->comboBox_2->addItem("Sequence TITLE");
    }else{
        ui->comboBox_1->addItem("--   N. A.   --");
        ui->comboBox_2->addItem("--   N. A.   --");
    }
    ui->comboBox_1->setCurrentIndex(0);
    ui->comboBox_2->setCurrentIndex(0);

    ui->radioButton_def_c->setChecked(findContain[0]);
    ui->radioButton_org_c->setChecked(findContain[1]);;
    ui->radioButton_3_c->setChecked(findContain[2]);
    ui->radioButton_4_c->setChecked(findContain[3]);
    ui->radioButton_def_n->setChecked(!findContain[0]);
    ui->radioButton_org_n->setChecked(!findContain[1]);
    ui->radioButton_3_n->setChecked(!findContain[2]);
    ui->radioButton_4_n->setChecked(!findContain[3]);

    for (int i = 0; i < queryName.length(); i++){
        ui->comboBox_id->addItem(queryName[i]);
    }
    ui->comboBox_id->setCurrentIndex(queryId);
}

SeqFindDlg::~SeqFindDlg()
{
    delete ui;
}

void SeqFindDlg::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
void SeqFindDlg::accept()
{
    bool succ;
    findIdMin = ui->lineEdit_minId->text().toFloat(&succ);
    findIdMax = ui->lineEdit_maxId->text().toFloat(&succ);
    findLenMin = ui->lineEdit_minLen->text().toInt(&succ);
    findLenMax = ui->lineEdit_maxLen->text().toInt(&succ);
    findDefConstraint = ui->lineEdit_defConstraint->text().toLower();
    findOrgConstraint = ui->lineEdit_orgConstraint->text().toLower();
    findExtnStr1 = ui->lineEdit_1->text().toLower();
    findExtnStr2 = ui->lineEdit_2->text().toLower();

    if(ui->checkBox_len->isChecked()){
        findConditionalCheck[0] = 1;
    }
    if(ui->checkBox_id->isChecked()){
        findConditionalCheck[1] = 1;
    }
    if(ui->checkBox_def->isChecked()){
        findConditionalCheck[2] = 1;
    }
    if(ui->checkBox_org->isChecked()){
        findConditionalCheck[3] = 1;
    }
    findConditionalCheck[4] = ui->comboBox_1->currentIndex();
    findConditionalCheck[5] = ui->comboBox_2->currentIndex();

    findContain[0] = ui->radioButton_def_c->isChecked();
    findContain[1] = ui->radioButton_org_c->isChecked();
    findContain[2] = ui->radioButton_3_c->isChecked();
    findContain[3] = ui->radioButton_4_c->isChecked();

    queryId = ui->comboBox_id->currentIndex();

    QString str;
    // parse error on "DEFINITION" term
    if( findConditionalCheck[2] && tools::parseConstraint(findDefConstraint) < 0 ){
            str = "Syntax error for \"Sequence DEFINITION\" string!\t\n";
            str	+= "Usage examples:\n18S rRNA\n\"18S rRNA\" and \"Entyloma ficariae\"\n";
            str += "\"18S rRNA\" or \"Entyloma ficariae\" and \"tilletiopsis cremea\"\t";
            QMessageBox::about(Q_NULLPTR, "Error input constraint",str);
            return;
    }
    // parse error on "ORGANISM" term
    if( findConditionalCheck[3] && tools::parseConstraint(findOrgConstraint) < 0 ){
            str = "Syntax error for \"Sequence ORGANISM\" string!\t\n";
            str	+= "Usage examples:\n18S rRNA\n\"18S rRNA\" and \"Entyloma ficariae\"\n";
            str += "\"18S rRNA\" or \"Entyloma ficariae\" and \"tilletiopsis cremea\"\t";
            QMessageBox::about(Q_NULLPTR, "Error input constraint",str);
            return;
    }
    // parse error on combobox1 term
    if( findConditionalCheck[4] > 0 && tools::parseConstraint(findExtnStr1) < 0 ){
            str = "Syntax error for ";
            QString str2 =  ui->comboBox_1->currentText();
            str	+= str2 + " string!\t\nUsage examples:\n18S rRNA\n\"18S rRNA\" and \"Entyloma ficariae\"\n";
            str += "\"18S rRNA\" or \"Entyloma ficariae\" and \"tilletiopsis cremea\"\t";
            QMessageBox::about(Q_NULLPTR, "Error input constraint",str);
            return;
    }
    // parse error on combobox2 term
    if( findConditionalCheck[5] > 0 && tools::parseConstraint(findExtnStr2) < 0 ){
            str = "Syntax error for ";
            QString str2 = ui->comboBox_2->currentText();
            str	+= str2 + " string!\t\nUsage examples:\n18S rRNA\n\"18S rRNA\" and \"Entyloma ficariae\"\n";
            str += "\"18S rRNA\" or \"Entyloma ficariae\" and \"tilletiopsis cremea\"\t";
            QMessageBox::about(Q_NULLPTR, "Error input constraint",str);
            return;
    }
    QDialog::accept();
}

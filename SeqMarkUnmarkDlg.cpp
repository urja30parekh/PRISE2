#include "SeqMarkUnmarkDlg.h"
#include "ui_SeqMarkUnmarkDlg.h"
using namespace std;

SeqMarkUnmarkDlg::SeqMarkUnmarkDlg(int mode, bool isSimple, QStringList queryName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SeqMarkUnmarkDlg)
{
    ui->setupUi(this);
    setWindowTitle("Mark/Unmark Sequences");
    //initParameters();
    switch(mode){
    case 1:
        ui->groupBox_all->setTitle("Mark All / Selected Sequences:");
        ui->checkBox_all->setText("Mark all sequences");
        ui->checkBox_selected->setText("Mark all selected sequences");
        ui->groupBox_conditional->setTitle("Mark Sequences Satisfying Selected Conditions:");
        ui->groupBox_range->setTitle("Mark within:");
        break;
    case 0:
        ui->groupBox_all->setTitle("Unmark All / Selected Sequences:");
        ui->checkBox_all->setText("Unmark all sequences");
        ui->checkBox_selected->setText("Unmark all selected sequences");
        ui->groupBox_conditional->setTitle("Unmark Sequences Satisfying Selected Conditions:");
        ui->groupBox_range->setTitle("Unmark within:");
        break;
    default:
        QMessageBox::about(Q_NULLPTR, "Error Mode","Neither mark nor unmark");
        break;
    }

    for (int i = 0; i < queryName.length(); i++){
        ui->comboBox_id->addItem(queryName[i]);
    }

    if (! isSimple){
        ui->comboBox_1->addItem("--select a key--");
        ui->comboBox_1->addItem("Sequence source");
        ui->comboBox_1->addItem("Sequence features");
        ui->comboBox_1->addItem("Sequence authors");
        ui->comboBox_1->addItem("Sequence title");

        ui->comboBox_2->addItem("--select a key--");
        ui->comboBox_2->addItem("Sequence source");
        ui->comboBox_2->addItem("Sequence features");
        ui->comboBox_2->addItem("Sequence authors");
        ui->comboBox_2->addItem("Sequence title");
    }else{
        ui->comboBox_1->addItem("--   N. A.   --");
        ui->comboBox_2->addItem("--   N. A.   --");
    }
    ui->comboBox_1->setCurrentIndex(0);
    ui->comboBox_2->setCurrentIndex(0);
}

SeqMarkUnmarkDlg::~SeqMarkUnmarkDlg()
{
    delete ui;
}

void SeqMarkUnmarkDlg::changeEvent(QEvent *e)
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

void SeqMarkUnmarkDlg::checkAll()
{
    if(!ui->checkBox_all->isChecked()){
        return;
    }

    ui->checkBox_selected->setChecked(false);
    ui->radioButton_all->setChecked(false);
    ui->radioButton_selected->setChecked(false);
    ui->checkBox_len->setChecked(false);
    ui->checkBox_id->setChecked(false);
    ui->checkBox_def->setChecked(false);
    ui->checkBox_org->setChecked(false);
}

void SeqMarkUnmarkDlg::checkSelected()
{
    if(!ui->checkBox_selected->isChecked()){
        return;
    }

    ui->checkBox_all->setChecked(false);
    ui->radioButton_all->setChecked(false);
    ui->radioButton_selected->setChecked(false);
    ui->checkBox_len->setChecked(false);
    ui->checkBox_id->setChecked(false);
    ui->checkBox_def->setChecked(false);
    ui->checkBox_org->setChecked(false);
}

void SeqMarkUnmarkDlg::radioAll()
{
    if(!ui->radioButton_all->isChecked()){
        return;
    }
    ui->radioButton_selected->setChecked(false);
    ui->checkBox_all->setChecked(false);
    ui->checkBox_selected->setChecked(false);
}

void SeqMarkUnmarkDlg::radioSelected()
{
    if(!ui->radioButton_selected->isChecked()){
        return;
    }
    ui->radioButton_all->setChecked(false);
    ui->checkBox_all->setChecked(false);
    ui->checkBox_selected->setChecked(false);
}

void SeqMarkUnmarkDlg::checkConditional()
{
    if(!ui->checkBox_len->isChecked() &&
       !ui->checkBox_id->isChecked()&&
       !ui->checkBox_def->isChecked() &&
       !ui->checkBox_org->isChecked()){
        return;
    }

    ui->checkBox_all->setChecked(false);
    ui->checkBox_selected->setChecked(false);

    if(!ui->radioButton_all->isChecked() &&
       !ui->radioButton_selected->isChecked()){
        ui->radioButton_all->setChecked(true);
    }
}

void SeqMarkUnmarkDlg::accept()
{
    bool succ;
    markIdMin = ui->lineEdit_minId->text().toFloat(&succ);
    markIdMax = ui->lineEdit_maxId->text().toFloat(&succ);
    markLenMin = ui->lineEdit_minLen->text().toInt(&succ);
    markLenMax = ui->lineEdit_maxLen->text().toInt(&succ);
    markDefConstraint = ui->lineEdit_defConstraint->text().toLower();
    markOrgConstraint = ui->lineEdit_orgConstraint->text().toLower();
    markExtnStr1 = ui->lineEdit_1->text().toLower();
    markExtnStr2 = ui->lineEdit_2->text().toLower();

    if(ui->checkBox_len->isChecked()){
        markConditionalCheck[0] = 1;
    }
    if(ui->checkBox_id->isChecked()){
        markConditionalCheck[1] = 1;
    }
    if(ui->checkBox_def->isChecked()){
        markConditionalCheck[2] = 1;
    }
    if(ui->checkBox_org->isChecked()){
        markConditionalCheck[3] = 1;
    }
    markConditionalCheck[4] = ui->comboBox_1->currentIndex();
    markConditionalCheck[5] = ui->comboBox_2->currentIndex();

    markAllData = ui->checkBox_all->isChecked() || ui->checkBox_selected->isChecked();
    markConstrainted = ui->radioButton_all->isChecked() || ui->radioButton_selected->isChecked();
    markWithinSelected = ui->checkBox_selected->isChecked() || ui->radioButton_selected->isChecked();

    markContain[0] = ui->radioButton_def_c->isChecked();
    markContain[1] = ui->radioButton_org_c->isChecked();
    markContain[2] = ui->radioButton_3_c->isChecked();
    markContain[3] = ui->radioButton_4_c->isChecked();

    queryId = ui->comboBox_id->currentIndex();

    QString str;
    // parse error on "DEFINITION" term
    if( markConditionalCheck[2] && tools::parseConstraint(markDefConstraint) < 0 ){
            str = "Syntax error for \"Sequence DEFINITION\" string!\t\n";
            str	+= "Usage examples:\n18S rRNA\n\"18S rRNA\" and \"Entyloma ficariae\"\n";
            str += "\"18S rRNA\" or \"Entyloma ficariae\" and \"tilletiopsis cremea\"\t";
            QMessageBox::about(Q_NULLPTR, "Error input constraint",str);
            return;
    }
    // parse error on "ORGANISM" term
    if( markConditionalCheck[3] && tools::parseConstraint(markOrgConstraint) < 0 ){
            str = "Syntax error for \"Sequence ORGANISM\" string!\t\n";
            str	+= "Usage examples:\n18S rRNA\n\"18S rRNA\" and \"Entyloma ficariae\"\n";
            str += "\"18S rRNA\" or \"Entyloma ficariae\" and \"tilletiopsis cremea\"\t";
            QMessageBox::about(Q_NULLPTR, "Error input constraint",str);
            return;
    }
    // parse error on combobox1 term
    if( markConditionalCheck[4] > 0 && tools::parseConstraint(markExtnStr1) < 0 ){
            str = "Syntax error for ";
            QString str2 =  ui->comboBox_1->currentText();
            str	+= str2 + " string!\t\nUsage examples:\n18S rRNA\n\"18S rRNA\" and \"Entyloma ficariae\"\n";
            str += "\"18S rRNA\" or \"Entyloma ficariae\" and \"tilletiopsis cremea\"\t";
            QMessageBox::about(Q_NULLPTR, "Error input constraint",str);
            return;
    }
    // parse error on combobox2 term
    if( markConditionalCheck[5] > 0 && tools::parseConstraint(markExtnStr2) < 0 ){
            str = "Syntax error for ";
            QString str2 = ui->comboBox_2->currentText();
            str	+= str2 + " string!\t\nUsage examples:\n18S rRNA\n\"18S rRNA\" and \"Entyloma ficariae\"\n";
            str += "\"18S rRNA\" or \"Entyloma ficariae\" and \"tilletiopsis cremea\"\t";
            QMessageBox::about(Q_NULLPTR, "Error input constraint",str);
            return;
    }
    QDialog::accept();
}

/*
int SeqMarkUnmarkDlg::parseConstraint(QString pattern)
{
        // empty string not allowed
        if( pattern == "" )
                return -1;
        // pattern is a single term string
        if( pattern.indexOf("\"") < 0 )
                return 0;

        // parse the compound string "pattern"
        QRegExp re(" ");
        pattern = pattern.remove(re);
        re.setPattern("and(\".+\")");
        pattern = pattern.remove(re);
        re.setPattern("or(\".+\")");
        pattern = pattern.remove(re);
        re.setPattern("(\".+\")");
        pattern = pattern.remove(re);
        if(pattern.length()!= 0){
            return -1;
        }
        return 0;
}
*/



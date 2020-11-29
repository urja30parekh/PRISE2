#include "MessageDlg.h"
#include "ui_MessageDlg.h"

MessageDlg::MessageDlg(QString labelText, QString message, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageDlg)
{
    ui->setupUi(this);
    ui->label->setText(labelText);
    ui->MessageTextArea->setText(QApplication::translate("MessageDlg", message.toLatin1(), Q_NULLPTR));

    retry = false;
    cont = false;
}

MessageDlg::~MessageDlg()
{
    delete ui;
}

void MessageDlg::insertRichText(QString insertText, QTextCharFormat format)
{
    QTextCursor cursor(ui->MessageTextArea->textCursor());
    cursor.movePosition(QTextCursor::Start);
    cursor.insertText(insertText, format);
}

void MessageDlg::hideRetry(){
    ui->buttonRestart->hide();
    ui->buttonRestart->setEnabled(false);
}

void MessageDlg::enableOK(bool enable)
{
    ui->buttonOK->setEnabled(enable);
}

void MessageDlg::accept()
{
    cont = true;
    QDialog::accept();
}

void MessageDlg::cancel()
{
    cont = false;
    QDialog::reject();
}


void MessageDlg::redesign()
{
    retry = true;
    QDialog::accept();
}

void MessageDlg::saveLog()
{
    QFileDialog file_dialog(this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getSaveFileName( this, tr( "Save Log File" ), pwd, tr( "TEXT (*.txt);;All File (*.*)" )) ;

    if(filename.isEmpty()){
        return;
    }
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::about(Q_NULLPTR, "File Error", "Cannot open file");
        return ;
    }

    // check file type
    QTextStream out(&file );
    out << "abcde"<<"\n";
    file.close();

}

void MessageDlg::changeEvent(QEvent *e)
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

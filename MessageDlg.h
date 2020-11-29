#ifndef MESSAGEDLG_H
#define MESSAGEDLG_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <MyConfirmDlg.h>
#include <QTextCharFormat>
#include "PRISESetting.h"

namespace Ui {
    class MessageDlg;
}

class MessageDlg : public QDialog {
    Q_OBJECT
public:
    MessageDlg(QString labelText, QString message, QWidget *parent = 0);
    ~MessageDlg();
    void insertRichText(QString insertText, QTextCharFormat format = QTextCharFormat());
    void hideRetry();
    void enableOK(bool enable);

    bool retry;
    bool cont;
    short type;


protected:
    void changeEvent(QEvent *e);

private slots:
    void accept();
    void cancel();
    void redesign();
    void saveLog();

private:
    Ui::MessageDlg *ui;
};

#endif // MESSAGEDLG_H

#ifndef BLASTRESULTDIALOG_H
#define BLASTRESULTDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QProcess>
#include "PRISESetting.h"
#include <stdlib.h>

namespace Ui {
class BlastResultDialog;
}

class BlastResultDialog : public QDialog
{
    Q_OBJECT

public:
    BlastResultDialog(QString comm, QWidget *parent = 0);
    ~BlastResultDialog();
public slots:
    void attachOutput();
    void attachErrorMsg();
    void updateExit();
    void reject();
    bool runSuccessufully();

private slots:
    void on_pushButton_clicked();

private:
    Ui::BlastResultDialog *ui;

    bool fileSave(QString filename, QString info4write);

    QProcess *blastProcess;
    void execBlast(QString command);
    bool isRunning;
    bool firstMsg;
    bool succRunBlast;
};

#endif // BLASTRESULTDIALOG_H

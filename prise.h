#ifndef PRISE_H
#define PRISE_H

#include <QWidget>
#include "SeqReportWindow.h"
#include "DesignWizard.h"
#include "Designer.h"
#include "TripleReportWindow.h"
#include "TripleReportFrame.h"
#include "InstructionDlg.h"
//#include "BlastDialog.h"
#include "blastwizard.h"
#include "blastresultdialog.h"

namespace Ui {
    class PRISE;
}

class PRISE : public QWidget {
    Q_OBJECT
public:
    PRISE(QWidget *parent = 0);
    ~PRISE();

    void removeFlat();
    void blast();
    void execBlast(QString command);

public slots:
    void selectSeqs();
    void designPrimers();
    void seedHitTableHelp();
    //void openInstruction();

    void openManual();
    void openTutorial();
    void aboutPRISE();

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_pushButton_HitTable_clicked();

private:
    Ui::PRISE *ui;

};

#endif // PRISE_H

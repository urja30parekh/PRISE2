#ifndef pSeqsInputPage_H
#define pSeqsInputPage_H

#include <QWizardPage>
#include <QFileDialog>
#include <QMessageBox>
#include "GlobalDef.h"
#include "Data.h"
#include "WizardPageOrder.h"

namespace Ui {
    class pSeqsInputPage;
}

class pSeqsInputPage : public QWizardPage {
    Q_OBJECT
public:
    pSeqsInputPage(QWidget *parent = 0);
    ~pSeqsInputPage();
    void initializePage();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::pSeqsInputPage *ui;
    int prevWidth;
    int prevHeight;

private slots:
    void browseTargetSeq();
    void browseNontargetSeq();
    void browseDumpFileDup();
    void browseDumpFileLenOutRange();   
    bool validatePage();
    int nextId() const;
    void cleanupPage();

    void on_radioButton_keepDup_clicked(bool checked);
    void on_radioButton_removeDup_clicked(bool checked);
    void on_radioButton_keepLenOutRange_clicked(bool checked);
    void on_radioButton_removeLenOutRange_clicked(bool checked);
};

#endif // pSeqsInputPage_H

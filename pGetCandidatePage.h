#ifndef PGETCANDIDATEPAGE_H
#define PGETCANDIDATEPAGE_H

#include <QWizardPage>
#include <QFileDialog>
#include <QMessageBox>
#include "Data.h"
#include "WizardPageOrder.h"

namespace Ui {
    class pGetCandidatePage;
}

class pGetCandidatePage : public QWizardPage {
    Q_OBJECT
public:
    pGetCandidatePage(QWidget *parent = 0);
    ~pGetCandidatePage();
    void setButtonTextPrimers();
    void setButtonTextProbes();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::pGetCandidatePage *ui;
    int prevWidth;
    int prevHeight;

private slots:
    int nextId() const;
    void initializePage();
    void browseCandidatesFile();
    bool validatePage();
    void cleanupPage();
};

#endif // PGETCANDIDATEPAGE_H

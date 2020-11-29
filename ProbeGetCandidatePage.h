#ifndef PROBEGETCANDIDATEPAGE_H
#define PROBEGETCANDIDATEPAGE_H

#include <QWizardPage>
#include <QFileDialog>
#include <QMessageBox>
#include "Data.h"

namespace Ui {
    class ProbeGetCandidatePage;
}

class ProbeGetCandidatePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ProbeGetCandidatePage(QWidget *parent = 0);
    ~ProbeGetCandidatePage();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ProbeGetCandidatePage *ui;
private slots:
    void browseCandidatesFile();
    bool validatePage();
};

#endif // PROBEGETCANDIDATEPAGE_H

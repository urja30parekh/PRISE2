#ifndef PRIMERSETTINGPROPERTYPAGE_H
#define PRIMERSETTINGPROPERTYPAGE_H

#include <QWizardPage>
#include "Data.h"
#include "WizardPageOrder.h"

namespace Ui {
    class PrimerSettingPropertyPage;
}

class PrimerSettingPropertyPage : public QWizardPage {
    Q_OBJECT
public:
    PrimerSettingPropertyPage(QWidget *parent = 0);
    ~PrimerSettingPropertyPage();
    void initializePage();

protected:
    void changeEvent(QEvent *e);
    int nextId() const;

private:
    Ui::PrimerSettingPropertyPage *ui;
    int prevWidth;
    int prevHeight;

private slots:
    bool validatePage();
    void cleanupPage();
    bool clickAvoidACT();
    bool clickAvoidG();
    bool clickAvoidCGat3end();
};

#endif // PRIMERSETTINGPROPERTYPAGE_H

#ifndef PROBESETTINGPROPERTYPAGE_H
#define PROBESETTINGPROPERTYPAGE_H

#include <QWizardPage>
#include "Data.h"
#include "WizardPageOrder.h"

namespace Ui {
    class ProbeSettingPropertyPage;
}

class ProbeSettingPropertyPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ProbeSettingPropertyPage(QWidget *parent = 0);
    ~ProbeSettingPropertyPage();
    void initializePage();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ProbeSettingPropertyPage *ui;
    int prevWidth;
    int prevHeight;

private slots:
    //overwrite slots
    bool validatePage();
    int nextId() const;
    void cleanupPage();

    bool clickAvoidACT();
    bool clickAvoidG();
    //bool clickAvoidGat5end();

};

#endif // PROBESETTINGPROPERTYPAGE_H

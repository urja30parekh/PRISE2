#ifndef PROBESETTINGPROPERTYPAGE_H
#define PROBESETTINGPROPERTYPAGE_H

#include <QWizardPage>
#include "Data.h"

namespace Ui {
    class ProbeSettingPropertyPage;
}

class ProbeSettingPropertyPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ProbeSettingPropertyPage(QWidget *parent = 0);
    ~ProbeSettingPropertyPage();

private:
    Ui::ProberSettingPropertyPage *ui;
};

#endif // PROBESETTINGPROPERTYPAGE_H

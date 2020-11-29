#ifndef PROBESETTINGSTARTPAGE_H
#define PROBESETTINGSTARTPAGE_H

#include <QWizardPage>
#include <QFileDialog>
#include "Data.h"

namespace Ui {
    class ProbeSettingStartPage;
}

class ProbeSettingStartPage : public QWizardPage
{
    Q_OBJECT

public:
    ProbeSettingStartPage(QWidget *parent = 0);
    ~ProbeSettingStartPage();
    void initializePage();

protected:
    void changeEvent(QEvent *e);

private:
    //bool loadSetting( QString settingFname);
    Ui::ProbeSettingStartPage *ui;

private slots:
    void onFinishSetting();
    void onContinue();
    bool validatePage();
    int nextId() const;
};

#endif // PROBESETTINGSTARTPAGE_H

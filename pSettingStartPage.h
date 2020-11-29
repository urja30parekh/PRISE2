#ifndef pSettingStartPage_H
#define pSettingStartPage_H

#include <QWizardPage>
#include <QFileDialog>
#include "Data.h"
#include "WizardPageOrder.h"

namespace Ui {
    class pSettingStartPage;
}

class pSettingStartPage : public QWizardPage
{
    Q_OBJECT

public:
    pSettingStartPage(QWidget *parent = 0);
    ~pSettingStartPage();
    void initializePage();
    void setButtonTextPrimers();
    void setButtonTextProbes();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::pSettingStartPage *ui;
    int prevWidth;
    int prevHeight;

private slots:
    //bool loadSetting( QString settingFname);
    int nextId() const;
    void onFinishSetting();
    void onContinue();  
    bool validatePage();
    void cleanupPage();

};

#endif // pSettingStartPage_H

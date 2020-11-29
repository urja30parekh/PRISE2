#ifndef ProbeSettingSlctvtBscPage_H
#define ProbeSettingSlctvtBscPage_H

#include <QWizardPage>
#include <Data.h>
#include "WizardPageOrder.h"

namespace Ui {
    class ProbeSettingSlctvtBscPage;
}

class ProbeSettingSlctvtBscPage : public QWizardPage {
    Q_OBJECT
public:
    ProbeSettingSlctvtBscPage(QWidget *parent = 0);
    ~ProbeSettingSlctvtBscPage();
    void initializePage();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ProbeSettingSlctvtBscPage *ui;
    //QString bindBitmaps[7];
    int prevWidth;
    int prevHeight;

    void setPosWtTarget(int num, ...);
    void setPosWtNonTarget(int num, ...);

private slots:
    //overwrite slots
    int nextId() const;
    bool validatePage();
    void cleanupPage();

    //void getUserParameter();
    void onScrollTar();
    void onScrollNontar();
    void activateWindow();
    void onDefault();
    void onShowAdv();
};

#endif // ProbeSettingSlctvtBscPage_H

#ifndef PrimerSettingSlctvtBscPage_H
#define PrimerSettingSlctvtBscPage_H

#include <QWizardPage>
#include <Data.h>
#include "WizardPageOrder.h"


namespace Ui {
    class PrimerSettingSlctvtBscPage;
}

class PrimerSettingSlctvtBscPage : public QWizardPage {
    Q_OBJECT
public:
    PrimerSettingSlctvtBscPage(QWidget *parent = 0);
    ~PrimerSettingSlctvtBscPage();
    void initializePage();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PrimerSettingSlctvtBscPage *ui;
    QString bindBitmaps[7];
    int prevWidth;
    int prevHeight;



private slots:
    //overwrite slots
    int nextId() const;
    bool validatePage();
    void cleanupPage();

    void onScrollTar();
    void onScrollNontar();
    void activateWindow();
    void onDefault();
    void onShowAdv();


};

#endif // PrimerSettingSlctvtBscPage_H

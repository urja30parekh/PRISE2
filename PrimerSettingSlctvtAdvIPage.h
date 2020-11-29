#ifndef PrimerSettingSlctvtAdvIPage_H
#define PrimerSettingSlctvtAdvIPage_H

#include <QWizardPage>
#include "Data.h"
#include "WizardPageOrder.h"

namespace Ui {
    class PrimerSettingSlctvtAdvIPage;
}

class PrimerSettingSlctvtAdvIPage : public QWizardPage
{
    Q_OBJECT

public:
    PrimerSettingSlctvtAdvIPage(QWidget *parent = 0);
    ~PrimerSettingSlctvtAdvIPage();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PrimerSettingSlctvtAdvIPage *ui;
    int prevWidth;
    int prevHeight;


private slots:
    void initializePage();
    bool validatePage();
    int nextId() const;
    void cleanupPage();

    void onCurrentSetting();
    void onShowMtx();
    void onEditTar_1();
    void onEditTar_2();
    void onEditTar_3();
    void onEditTar_4();
    void onEditTar_5();
    void onEditTar_6();
    void onEditTar_7();
    void onEditTar_8();
    void onEditTar_9();
    void onEditTar_10();
    void onEditTar_11();
    void onEditTar_12();
    void onEditTar_13();
    void onEditTar_14();
    void onEditTar_15();
    void onEditTar_16();
    void onEditTar_17();
    void onEditTar_18();
    void onEditTar_19();
    void onEditTar_20();
    void onEditTar_21();
    void onEditTar_22();
    void onEditTar_23();
    void onEditTar_24();
    void onEditTar_25();
    void onEditTar_26();
    void onEditTar_27();
    void onEditTar_28();
    void onEditTar_29();
    void onEditTar_30();


    void onEditNonTar_1();
    void onEditNonTar_2();
    void onEditNonTar_3();
    void onEditNonTar_4();
    void onEditNonTar_5();
    void onEditNonTar_6();
    void onEditNonTar_7();
    void onEditNonTar_8();
    void onEditNonTar_9();
    void onEditNonTar_10();
    void onEditNonTar_11();
    void onEditNonTar_12();
    void onEditNonTar_13();
    void onEditNonTar_14();
    void onEditNonTar_15();
    void onEditNonTar_16();
    void onEditNonTar_17();
    void onEditNonTar_18();
    void onEditNonTar_19();
    void onEditNonTar_20();
    void onEditNonTar_21();
    void onEditNonTar_22();
    void onEditNonTar_23();
    void onEditNonTar_24();
    void onEditNonTar_25();
    void onEditNonTar_26();
    void onEditNonTar_27();
    void onEditNonTar_28();
    void onEditNonTar_29();
    void onEditNonTar_30();
};

#endif // PrimerSettingSlctvtAdvIPage_H

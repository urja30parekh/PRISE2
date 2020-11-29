#ifndef PROBESETTINGSLCTVTADVIIPAGE_H
#define PROBESETTINGSLCTVTADVIIPAGE_H

#include <QWizardPage>
#include "Data.h"

namespace Ui {
    class ProbeSettingSlctvtAdvIIPage;
}

class ProbeSettingSlctvtAdvIIPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ProbeSettingSlctvtAdvIIPage(QWidget *parent = 0);
    ~ProbeSettingSlctvtAdvIIPage();


protected:
    void changeEvent(QEvent *e);

private:
    Ui::ProbeSettingSlctvtAdvIIPage *ui;
    int prevWidth;
    int prevHeight;

private slots:
    void initializePage();
    int nextId() const;
    bool validatePage();
    void cleanupPage();

    void reset();
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

};

#endif // PROBESETTINGSLCTVTADVIIPAGE_H

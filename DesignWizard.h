#ifndef DESIGNWIZARD_H
#define DESIGNWIZARD_H

#include <QWizard>
//#include "GlobalDef.h"
#include "WizardPageOrder.h"
#include "TaskPage.h"
#include "LoadOrNewPage.h"
#include "pSeqsInputPage.h"
#include "pGetCandidatePage.h"
#include "pSettingStartPage.h"
#include "PrimerSettingPropertyPage.h"
#include "PrimerSettingSlctvtBscPage.h"
//#include "PrimerSettingSlctvtAdvIPage.h"
#include "PrimerSettingSlctvtAdvIIPage.h"
#include "ProbeGetCandidatePage.h"
//#include "ProbeSettingStartPage.h"
#include "ProbeSettingPropertyPage.h"
#include "ProbeSettingSlctvtBscPage.h"
//#include "ProbeSettingSlctvtAdvIPage.h"
#include "ProbeSettingSlctvtAdvIIPage.h"
#include "pGetCandidatePage.h"

namespace Ui {
    class DesignWizard;
}

class DesignWizard : public QWizard {
    Q_OBJECT
public:
    DesignWizard(QWidget *parent);
    void forPrimers();
    void forProbes();
    ~DesignWizard();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DesignWizard *ui;

    LoadOrNewPage* lnPage;
    pSeqsInputPage* psiPage;
    pGetCandidatePage* pcPage;
    pSettingStartPage* pssPage;


    PrimerSettingPropertyPage* pmspPage;
    PrimerSettingSlctvtBscPage* pmsbPage;
    //PrimerSettingSlctvtAdvIPage* pmsAdvIPage;
    PrimerSettingSlctvtAdvIIPage* pmsAdvIIPage;

    ProbeSettingPropertyPage* pbspPage;
    ProbeSettingSlctvtBscPage* pbsbPage;
    //ProbeSettingSlctvtAdvIPage* pbsAdvIPage;
    ProbeSettingSlctvtAdvIIPage* pbsAdvIIPage;

};

#endif // DESIGNWIZARD_H

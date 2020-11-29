#include "DesignWizard.h"
#include "ui_DesignWizard.h"
#include <QRect>
#include <QDesktopWidget>

DesignWizard::DesignWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::DesignWizard)
{
    ui->setupUi(this);

    lnPage = new LoadOrNewPage(this);
    psiPage = new pSeqsInputPage(this);
    pcPage = new pGetCandidatePage(this);
    pssPage = new pSettingStartPage(this);

    pmspPage = new PrimerSettingPropertyPage(this);
    pmsbPage = new PrimerSettingSlctvtBscPage(this);
    //pmsAdvIPage = new PrimerSettingSlctvtAdvIPage(this);
    pmsAdvIIPage = new PrimerSettingSlctvtAdvIIPage(this);

    pbspPage = new ProbeSettingPropertyPage(this);
    pbsbPage = new ProbeSettingSlctvtBscPage(this);
    //pbsAdvIPage = new ProbeSettingSlctvtAdvIPage(this);
    pbsAdvIIPage = new ProbeSettingSlctvtAdvIIPage(this);

    setPage(Page_loadOrNew, lnPage);
    setPage(Page_seqInput, psiPage);
    setPage(Page_pCand, pcPage);
    setPage(Page_pSettingStart, pssPage);
    setPage(Page_primerProperty, pmspPage);
    setPage(Page_primerSetttingBasic, pmsbPage);
    //setPage(Page_primerSettingAdvI, pmsAdvIPage);
    setPage(Page_primerSettingAdvII, pmsAdvIIPage);
    setPage(Page_probeProperty, pbspPage);
    setPage(Page_probeSettingBasic, pbsbPage);
    //setPage(Page_probeSettingAdvI, pbsAdvIPage);
    setPage(Page_probeSettingAdvII, pbsAdvIIPage);

    setStartId(Page_loadOrNew);
}

DesignWizard::~DesignWizard()
{
    delete ui;
}

void DesignWizard::forPrimers()
{
    this->pcPage->setTitle("Extract/Load Primer Candidates");
    this->pssPage->setTitle("Primer Design Settings");
    this->pcPage->setButtonTextPrimers();
    this->pssPage->setButtonTextPrimers();
}

void DesignWizard::forProbes()
{
    this->pcPage->setTitle("Extract/Load Probe Candidates");
    this->pssPage->setTitle("Probe Design Settings");
    this->pcPage->setButtonTextProbes();
    this->pssPage->setButtonTextProbes();
}

void DesignWizard::changeEvent(QEvent *e)
{
    QWizard::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

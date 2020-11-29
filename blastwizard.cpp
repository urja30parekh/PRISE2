#include "blastwizard.h"
#include "ui_blastwizard.h"

BlastWizard::BlastWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::BlastWizard)
{
    ui->setupUi(this);

    lwPage = new LocalOrWebPage(this);
    queryPage = new BlastQueryPage(this);
    paraPage = new BlastParaPage(this);


    setPage(Page_localOrWebBlast, lwPage);
    setPage(Page_querAndSearchSet, queryPage);
    setPage(Page_blastParameters, paraPage);

    setStartId(Page_localOrWebBlast);
    this->setButtonText(QWizard::FinishButton, "OK");
}

BlastWizard::~BlastWizard()
{
    delete ui;
}

#include "LoadOrNewPage.h"
#include "ui_LoadOrNewPage.h"

LoadOrNewPage::LoadOrNewPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::LoadOrNewPage)
{
    ui->setupUi(this);

    ui->radioButton_load->setChecked(!design);
    ui->radioButton_new->setChecked(design);
    /*switch(design){
    case true:
        setFinalPage(false);
        break;
    case false:
        setFinalPage(true);
        QWizard* pWizard = (QWizard*)this->parent();
        pWizard->button(QWizard::NextButton)->hide();
        break;

    }*/
}



LoadOrNewPage::~LoadOrNewPage()
{
    delete ui;
}
//MX
void LoadOrNewPage::initializePage()
{

#ifdef MAC
    this->resize(300, 200);
#else
    this->resize(300, 200);
#endif


    QWizard* p = wizard();

#ifdef MAC
    p->resize(width()+50, height()+20);
#else
    p->resize(width()+50, height()+20);
#endif
}

void LoadOrNewPage::loadPrev()
{
    //this->update();
    setFinalPage(true);
    //QWizard* pWizard = this->wizard();
    //pWizard->button(QWizard::NextButton)->hide();
}

void LoadOrNewPage::designNew()
{
    //this->update();
    setFinalPage(false);
}


int LoadOrNewPage::nextId() const
{
    return ui->radioButton_load->isChecked()? -1 : Page_seqInput;
}

bool LoadOrNewPage::validatePage()
{
    if(ui->radioButton_load->isChecked()){
        design = false;
    }else{
        design = true;
    }
    return QWizardPage::validatePage();
}

void LoadOrNewPage::changeEvent(QEvent *e)
{
    QWizardPage::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

#include "PrimerSettingSlctvtBscPage.h"
#include "ui_PrimerSettingSlctvtBscPage.h"

#include <DataTools.h>

#define _ADDRESSOF(v)   ( &reinterpret_cast<const char &>(v) )
#define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )

PrimerSettingSlctvtBscPage::PrimerSettingSlctvtBscPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::PrimerSettingSlctvtBscPage)
{
    ui->setupUi(this);
    ui->radioButton_default->setChecked(!primerShowAdvance);
    ui->radioButton_showAdv->setChecked(primerShowAdvance);

    setFinalPage(!primerShowAdvance);

    bindBitmaps[0] = QString::fromUtf8(":/new/prefix1/images/bind000.bmp");
    bindBitmaps[1] = QString::fromUtf8(":/new/prefix1/images/bind100.bmp");
    bindBitmaps[2] = QString::fromUtf8(":/new/prefix1/images/bind110.bmp");
    bindBitmaps[3] = QString::fromUtf8(":/new/prefix1/images/bind210.bmp");
    bindBitmaps[4] = QString::fromUtf8(":/new/prefix1/images/bind211.bmp");
    bindBitmaps[5] = QString::fromUtf8(":/new/prefix1/images/bind221.bmp");
    bindBitmaps[6] = QString::fromUtf8(":/new/prefix1/images/bind321.bmp");

    ui->verticalSlider_tarBase1to3->setValue(primerBindBitmap_tar1to3);
    ui->verticalSlider_tarBase4->setValue(primerBindBitmap_tar4);

    ui->verticalSlider_nontarBase1to3->setValue(primerBindBitmap_nontar1to3);
    ui->verticalSlider_nontarBase4->setValue(primerBindBitmap_nontar4);
    emit completeChanged();
}

PrimerSettingSlctvtBscPage::~PrimerSettingSlctvtBscPage()
{
    delete ui;
}

void PrimerSettingSlctvtBscPage::initializePage()
{

#ifdef MAC
    this->resize(770, 620);
#else
    this->resize(770, 620);
#endif

    QWizard* p = this->wizard();
    prevWidth = p->width();
    prevHeight = p->height();

#ifdef MAC
    p->resize(width()+150, height()+450);
#else
    p->resize(width()+150, height()+150);
#endif

}

void PrimerSettingSlctvtBscPage::cleanupPage()
{
    QWizard* p = wizard();
    p->resize(prevWidth, prevHeight);
}

void PrimerSettingSlctvtBscPage::changeEvent(QEvent *e)
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

void PrimerSettingSlctvtBscPage::activateWindow()
{

}

void PrimerSettingSlctvtBscPage::onScrollTar()
{
    int posSel;

    posSel = ui->verticalSlider_tarBase1to3->value();
    ui->label_bitmapTar->setPixmap(QPixmap(bindBitmaps[posSel]));
}

void PrimerSettingSlctvtBscPage::onScrollNontar()
{
    int posSel;

    posSel = ui->verticalSlider_nontarBase1to3->value();
    ui->label_bitmapNontar->setPixmap(QPixmap(bindBitmaps[posSel]));
}


int PrimerSettingSlctvtBscPage::nextId() const
{
    if (ui->radioButton_default->isChecked()) {
        return -1;
    } else {
        return Page_primerSettingAdvII;
    }
}



bool PrimerSettingSlctvtBscPage::validatePage()
{    

    int posSel_1to3;
    int posSel_4;

    posSel_1to3 = ui->verticalSlider_tarBase1to3->value();
    primerBindBitmap_tar1to3 = posSel_1to3;
    posSel_4 = ui->verticalSlider_tarBase4->value();
    primerBindBitmap_tar4 = posSel_4;


    posSel_1to3 = ui->verticalSlider_nontarBase1to3->value();
    primerBindBitmap_nontar1to3 = posSel_1to3;
    posSel_4 = ui->verticalSlider_nontarBase4->value();
    primerBindBitmap_nontar4 = posSel_4;

    DataTools::setPrimerMismatchAllowance(primerBindBitmap_tar1to3, primerBindBitmap_tar4, primerBindBitmap_nontar1to3, primerBindBitmap_nontar4 );
    primerShowAdvance = ui->radioButton_showAdv->isChecked();
    return ((QWizardPage)this).validatePage();
}

void PrimerSettingSlctvtBscPage::onDefault()
{
    /*this->setFinalPage(true);
    QWizard* pWizard = wizard();
    pWizard->button(QWizard::NextButton)->hide();*/
    if (this->nextId() == -1)
        setFinalPage(true);
    else
        setFinalPage(false);
}

void PrimerSettingSlctvtBscPage::onShowAdv()
{
    //this->setFinalPage(false);
    if (this->nextId() == -1)
        setFinalPage(true);
    else
        setFinalPage(false);
}


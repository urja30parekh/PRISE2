#include "TaskPage.h"
#include "ui_TaskPage.h"

TaskPage::TaskPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::TaskPage)
{
    ui->setupUi(this);
    ui->radioButton_primer->setChecked(true);
}

TaskPage::~TaskPage()
{
    delete ui;
}

bool TaskPage::validatePage()
{
    if (ui->radioButton_primer->isChecked())
        task = 0;
    else if (ui->radioButton_probe->isChecked())
        task = 1;
    else
        task = 2;
    return QWizardPage::validatePage();
}

void TaskPage::changeEvent(QEvent *e)
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

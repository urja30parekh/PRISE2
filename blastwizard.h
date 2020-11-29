#ifndef BLASTWIZARD_H
#define BLASTWIZARD_H

#include <QWizard>
#include "WizardPageOrder.h"
#include "localorwebpage.h"
#include "blastquerypage.h"
#include "blastparapage.h"

namespace Ui {
class BlastWizard;
}

class BlastWizard : public QWizard
{
    Q_OBJECT

public:
    explicit BlastWizard(QWidget *parent = 0);
    ~BlastWizard();

private:
    Ui::BlastWizard *ui;

    LocalOrWebPage* lwPage;
    BlastQueryPage* queryPage;
    BlastParaPage* paraPage;
};

#endif // BLASTWIZARD_H

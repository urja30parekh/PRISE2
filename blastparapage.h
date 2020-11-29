#ifndef BLASTPARAPAGE_H
#define BLASTPARAPAGE_H

#include <QWizardPage>
#include "PRISESetting.h"

namespace Ui {
class BlastParaPage;
}

class BlastParaPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BlastParaPage(QWidget *parent = 0);
    ~BlastParaPage();

protected:
    void changeEvent(QEvent *e);
    int nextId() const;
    int prevWidth;
    int prevHeight;

private:
    Ui::BlastParaPage *ui;

    bool validatePage();
private slots:
    void initializePage();
};

#endif // BLASTPARAPAGE_H

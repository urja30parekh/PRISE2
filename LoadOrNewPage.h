#ifndef LoadOrNewPage_H
#define LoadOrNewPage_H

#include <QWizardPage>
#include "PRISESetting.h"
#include "WizardPageOrder.h"

namespace Ui {
    class LoadOrNewPage;
}

class LoadOrNewPage : public QWizardPage {
    Q_OBJECT
public:
    LoadOrNewPage(QWidget *parent = 0);
    ~LoadOrNewPage();

protected:
    void changeEvent(QEvent *e);
    int nextId() const;

private:
    Ui::LoadOrNewPage *ui;

private slots:
    void initializePage();
    void loadPrev();
    void designNew();
    bool validatePage();
};

#endif // LoadOrNewPage_H

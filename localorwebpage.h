#ifndef LOCALORWEBPAGE_H
#define LOCALORWEBPAGE_H

#include <QWizardPage>
#include "PRISESetting.h"
#include "WizardPageOrder.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

namespace Ui {
class LocalOrWebPage;
}

class LocalOrWebPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit LocalOrWebPage(QWidget *parent = 0);
    ~LocalOrWebPage();

protected:
    void changeEvent(QEvent *e);
    int nextId() const;

private slots:
    void initializePage();
    bool validatePage();
    void on_radioButton_web_clicked();
    void on_radioButton_local_clicked();

    void on_pushButton_browse_clicked();
    void on_pushButton_browse2_clicked();

private:
    Ui::LocalOrWebPage *ui;
};

#endif // LOCALORWEBPAGE_H

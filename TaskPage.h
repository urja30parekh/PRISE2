#ifndef TASKPAGE_H
#define TASKPAGE_H

#include <QWizardPage>
#include <Data.h>

namespace Ui {
    class TaskPage;
}

class TaskPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit TaskPage(QWidget *parent = 0);
    ~TaskPage();

private:
    Ui::TaskPage *ui;

protected:
    void changeEvent(QEvent *e);

private slots:
    bool validatePage();
};

#endif // TASKPAGE_H

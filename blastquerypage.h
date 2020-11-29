#ifndef BLASTQUERYPAGE_H
#define BLASTQUERYPAGE_H

#include <QWizardPage>
#include <QFileDialog>
#include "PRISESetting.h"
#include "WizardPageOrder.h"
#include <QMessageBox>

namespace Ui {
class BlastQueryPage;
}

class BlastQueryPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BlastQueryPage(QWidget *parent = 0);
    ~BlastQueryPage();
protected:
    void changeEvent(QEvent *e);
    int nextId() const;
    int prevWidth;
    int prevHeight;

private slots:
    void on_toolButton_browse_clicked();
    bool validatePage();
    void initializePage();
    //void on_plainTextEdit_seq_textChanged();

    //void on_lineEdit_SeedSeqFile_textEdited(const QString &arg1);

private:
    Ui::BlastQueryPage *ui;
};

#endif // BLASTQUERYPAGE_H

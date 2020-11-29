#ifndef INSTRUCTIONDLG_H
#define INSTRUCTIONDLG_H

#include <QDialog>
#include <GlobalDef.h>

namespace Ui {
class InstructionDlg;
}

class InstructionDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit InstructionDlg(QWidget *parent = 0);
    ~InstructionDlg();
    
private:
    Ui::InstructionDlg *ui;

private slots:
    void openManual();
    void openTutorial();
};

#endif // INSTRUCTIONDLG_H

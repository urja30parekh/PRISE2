#ifndef AnnealInfoDlg_H
#define AnnealInfoDlg_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
    class AnnealInfoDlg;
}

class AnnealInfoDlg : public QDialog
{
    Q_OBJECT

public:
    AnnealInfoDlg(int *divider3end, float *match5end, float *match3end, QWidget *parent = 0);// standard constructor
    ~AnnealInfoDlg();
public slots:
    void accept();

private:
    Ui::AnnealInfoDlg *ui;

    int *bindingContentDivider3end;
    float *bindingContentMatchPctg5end;
    float *bindingContentMatchPctg3end;
};

#endif // AnnealInfoDlg_H

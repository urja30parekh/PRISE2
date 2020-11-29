#ifndef PRIMERSETTINGREPORTDLG_H
#define PRIMERSETTINGREPORTDLG_H

#include <QDialog>
#include "Data.h"

namespace Ui {
    class PrimerSettingReportDlg;
}

class PrimerSettingReportDlg : public QDialog {
    Q_OBJECT
public:
    PrimerSettingReportDlg(bool enable, QWidget *parent);
    ~PrimerSettingReportDlg();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PrimerSettingReportDlg *ui;
};

#endif // PRIMERSETTINGREPORTDLG_H

#ifndef PROBESETTINGREPORTDLG_H
#define PROBESETTINGREPORTDLG_H

#include <QDialog>
#include "Data.h"

namespace Ui {
    class ProbeSettingReportDlg;
}

class ProbeSettingReportDlg : public QDialog {
    Q_OBJECT
public:
    ProbeSettingReportDlg(bool enable, QWidget *parent);
    ~ProbeSettingReportDlg();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ProbeSettingReportDlg *ui;
};

#endif // PROBESETTINGREPORTDLG_H

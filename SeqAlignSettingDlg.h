#ifndef SEQALIGNSETTINGDLG_H
#define SEQALIGNSETTINGDLG_H

#include <QDialog>
#include <QMessageBox>
//#include <qfile.h>
//#include <qtextstream.h>
#include <QTextStream>
#include <QFile>
#include "GlobalDef.h"
#include "PRISESetting.h"

#define TINY_IDENTITY_VALUE 0.0001f

namespace Ui {
    class SeqAlignSettingDlg;
}

class SeqAlignSettingDlg : public QDialog {
    Q_OBJECT
public:
    SeqAlignSettingDlg(QWidget *parent);
    ~SeqAlignSettingDlg();

protected:
    void changeEvent(QEvent *e);

private:
    static const int mismatchDefault = 0, gapOpenDefault  = 2, gapExtnDefault = 1;
    static const countGapPolicy policyDefault = Internal;

    Ui::SeqAlignSettingDlg *ui;

private slots:
    void reset();
    void accept();
};

#endif // SEQALIGNSETTINGDLG_H

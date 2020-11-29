#ifndef SEQMARKUNMARKDLG_H
#define SEQMARKUNMARKDLG_H

#include <QDialog>
#include <QString>
#include <QMessageBox>
#include "tools.h"
#include "PRISESetting.h"

namespace Ui {
    class SeqMarkUnmarkDlg;
}

class SeqMarkUnmarkDlg : public QDialog {
    Q_OBJECT
public:
    SeqMarkUnmarkDlg(int mode, bool isSimple, QStringList queryName, QWidget *parent = 0);     //mode = 0: mark.  mode = 1:unmark
    ~SeqMarkUnmarkDlg();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SeqMarkUnmarkDlg *ui;

private slots:
    void checkConditional();
    void radioSelected();
    void radioAll();
    void checkSelected();
    void checkAll();    
    void accept();

};

#endif // SEQMARKUNMARKDLG_H

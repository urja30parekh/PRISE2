#ifndef LOADSEEDHITTABLEDLG_H
#define LOADSEEDHITTABLEDLG_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include "PRISESetting.h"

namespace Ui {
    class LoadSeedHitTableDlg;
}

class LoadSeedHitTableDlg : public QDialog {
    Q_OBJECT
public:
    LoadSeedHitTableDlg(QWidget *parent);
    ~LoadSeedHitTableDlg();
    void getFilePath( QString &SF,  QString &HTF,  QString &FF);
    void getAccessions(QString &SeedAccs);

protected:
    void changeEvent(QEvent *e);

private slots:
    void browseSeedFile();
    void browseHitTableFile();
    void browseFASTAFile();
    void accept();
private:
    Ui::LoadSeedHitTableDlg *ui;

    QString SeedAccessions;
    QString SeedFile;
    QString HitTableFile;
    QString FASTAFile;
};

#endif // LOADSEEDHITTABLEDLG_H

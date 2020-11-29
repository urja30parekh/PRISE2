#ifndef SEQALIGNSHOWWINDOW_H
#define SEQALIGNSHOWWINDOW_H

#include <QMainWindow>
#include "PRISESetting.h"
#include "Data.h"

namespace Ui {
    class SeqAlignShowWindow;
}

class SeqAlignShowWindow : public QMainWindow {
    Q_OBJECT
public:
    SeqAlignShowWindow(short* oSeq, QString oAcc, int oLen, QVector<short*> qSeqs, QVector<QString> qAccs, QVector<int> qLens, QWidget *parent = 0);
    ~SeqAlignShowWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SeqAlignShowWindow *ui;
    short* objSeq;
    int objLen;
    QString objAcc;
    QVector<short*> querySeqs;
    QVector<int> queryLens;
    QVector<QString> queryAccs;

    void runAlign();
};

#endif // SEQALIGNSHOWWINDOW_H

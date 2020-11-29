#include "SeqAlignShowWindow.h"
#include "ui_SeqAlignShowWindow.h"

SeqAlignShowWindow::SeqAlignShowWindow(short* oSeq, QString oAcc, int oLen, QVector<short*> qSeqs, QVector<QString> qAccs, QVector<int> qLens,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SeqAlignShowWindow)
{
    ui->setupUi(this);
    setCentralWidget(this->ui->widget);
    objSeq = oSeq;
    objAcc = oAcc;
    objLen = oLen;
    querySeqs = qSeqs;
    queryAccs = qAccs;
    queryLens = qLens;
    runAlign();
}

SeqAlignShowWindow::~SeqAlignShowWindow()
{
    delete ui;
}

void SeqAlignShowWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SeqAlignShowWindow::runAlign()
{
        // find maximu sequence length
        int maxLen = objLen;
        for( int i = 0; i < queryLens.size(); i++ )
                if( queryLens[i] > maxLen )	maxLen = queryLens[i];

        float myId;

        QString str;
        for( int i = 0; i < queryLens.size(); i++ ){
                QStringList result;
                myId = sAlign->NWalign(querySeqs[i], queryLens[i]+1, objSeq, objLen+1,  &result);

                str = QString("%1%%").arg(static_cast<double>(myId));
                ui->plainTextEdit->insertPlainText("Seed sequence " + queryAccs[i] + " V.S. subject sequence " + objAcc + ": " + str + "% identity.\n" );
                ui->plainTextEdit->insertPlainText(result[0]+"\n" );

                for( int j = 1; j < result.size(); ){
                        ui->plainTextEdit->insertPlainText( "Sed " + result[j]+ "\n" );
                        j++;
                        ui->plainTextEdit->insertPlainText( "    " + result[j] +"\n");
                        j++;
                        ui->plainTextEdit->insertPlainText( "Sbj " + result[j] + "\n\n" );
                        j++;
                }
               ui->plainTextEdit->insertPlainText("\n");
        }
}

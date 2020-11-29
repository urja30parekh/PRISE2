#include "prise.h"
#include "ui_prise.h"
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QTextCharFormat>

PRISE::PRISE(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PRISE)
{
    ui->setupUi(this);
    DataTools::Initialize();
}

PRISE::~PRISE()
{
    delete ui;
}

void PRISE::selectSeqs()
{
    this->ui->pushButton_2->setFlat(true);
    SeqReportWindow* srWin = new SeqReportWindow(this);
    srWin->show();
}

// click on step 2
void PRISE::designPrimers()
{

    this->ui->pushButton_3->setFlat(true);
    DesignWizard* pWprimer = new DesignWizard(this);
    pWprimer->forPrimers();
    task = 0; //design primers

    while(pWprimer->exec()){
        firstPrimer = false;
        pWprimer->restart();
        pWprimer->next();
        if (!design)  { //Load saved primer list
            QFileDialog file_dialog(this);
            file_dialog.setFileMode( QFileDialog::ExistingFile );
            QString filename = file_dialog.getOpenFileName( this, tr( "Load Primer/Probe List File" ), QDir::currentPath(), tr( "Primer List File (*.primer);;Triple List File (*.ppset);;All File (*.*)" )) ;
            if(filename.isEmpty()){
                return;
            }

            if(filename.endsWith(".primer")){ //Load Primer list
                PrimerReportWindow  *prWin = new PrimerReportWindow(this);
                DataTools::loadPrimerList(filename, prWin);
                prWin->show();
            }else if(filename.endsWith(".ppset")){  //Load triple list
                TripleReportWindow  *tpWin = new TripleReportWindow(this);
                TripleReportFrame *tpFm = new TripleReportFrame();
                tpFm->loadTripleList(filename);
                tpWin->addTab(tpFm, tpFm->getPrimerSeq());
                tpWin->show();
            }
            break;
        }else{ //Design new primer　pairs
            /*QString logFilePath = "";
            #ifdef MAC
                QString appPath = QDir::currentPath();
                //logFilePath += "file:/";
                logFilePath += appPath;
                logFilePath += "/";
            #endif

            #ifdef WIN
                logFilePath += "Log\\";
            #else
                logFilePath += "Log/";
            #endif


            QDate date = QDate::currentDate();
            QString dateString = date.toString();

            QFile file(logFilePath+dateString+"_Log.txt");


            if(!file.exists()){
                QDir dir(logFilePath);
                QStringList entities = dir.entryList(QDir::Files, QDir::Time);

                for(int i = numLogFiles-1; i < entities.size() ; i++){
                    QMessageBox::about(this,"",entities[i]);
                    dir.remove(entities[i]);
                }
            }



            file.open(QIODevice::Append| QIODevice::Text);
            QTextStream out(&file);
            QTime time = QTime::currentTime();
            out << time.toString()<<"\t Start Primer Design Process\n";
            out << "Settings:\n";
            out << "sequences: " << targetSeqFilename << " " << nontargetSeqFilename<< "\n";
            out << "sequence length: "<< seqLenMin << " " << seqLenMax << "\n";
            out << "Primer Length: " << primerLenMin << " " << primerLenMax << "\n";
            out << "Primer Freq: " << primerCandidFrequency << " " << primerCandidTopPctg << "\n";
            out << "PCR size: " << primerPcrSizeMin << " " << primerPcrSizeMax << "\n";
            out << "Avoid: " << primerAvoidACT << " " <<primerAvoidG << " " << primerAvoidCGat3end << "\n";
            out << "avoid #: " <<primerNumACT << " " << primerNumG << " " <<primerNumCGat3end << "\n";
            out << "Primer Tm: " << primerTmMin << " " << primerTmMax << " " << primerTmDiff;
            out << "Na Consent: " << primerNaConsent << "\n";
            out << "GC: " << primerGCMin << " " << primerGCMax << "\n";
            out << "Comp: " <<  primerMaxPrPrComp << " " << primerMax3PrPrComp << " " << primerInterBase3end << " " << primerMaxSelfComp << " " <<primerMax3SelfComp << " " << primerIntraBase3end << "\n";
            out << "Indel: "<< maxIndel << " " << indelCost << "\n";
            file.close();*/

            QString errorlog = "";

            QTime time = QTime::currentTime();
            errorlog += time.toString() + "\t Start Primer Design Process\n";
            errorlog += "Settings:\n";
            errorlog += "sequences: " + targetSeqFilename + " " + nontargetSeqFilename + "\n";
            errorlog +="sequence length: " + QString::number(seqLenMin) + " " + QString::number(seqLenMax) + "\n";
            errorlog +="Primer Length: " + QString::number(primerLenMin) + " " + QString::number(primerLenMax) + "\n";
            errorlog +="Primer Freq: " + QString::number(primerCandidFrequency) + " " + QString::number(primerCandidTopPctg) + "\n";
            errorlog +="PCR size: " + QString::number(primerPcrSizeMin) + " " + QString::number(primerPcrSizeMax) + "\n";
            errorlog +="Avoid: " + QString::number(primerAvoidACT) + " " + QString::number(primerAvoidG) + " " + QString::number(primerAvoidCGat3end) + "\n";
            errorlog +="avoid #: " + QString::number(primerNumACT) + " " + QString::number(primerNumG) + " " + QString::number(primerNumCGat3end) + "\n";
            errorlog +="Primer Tm: " + QString::number(primerTmMin) + " " + QString::number(primerTmMax) + " " + QString::number(primerTmDiff);
            errorlog +="Na Consent: " + QString::number(primerNaConsent) + "\n";
            errorlog +="GC: " + QString::number(primerGCMin) + " " + QString::number(primerGCMax) + "\n";
            errorlog +="Comp: " + QString::number(primerMaxPrPrComp) + " " + QString::number(primerMax3PrPrComp) + " " + QString::number(primerInterBase3end) + " " + QString::number(primerMaxSelfComp) + " " + QString::number(primerMax3SelfComp) + " " + QString::number(primerIntraBase3end) + "\n";
            errorlog +="Indel: " + QString::number(maxIndel) + " " + QString::number(indelCost) + "\n";
            DataTools::writeErrorLog(errorlog);


            DataTools::buildSimCost(primerCostType); // false:regular  true:binary
            Designer::startPrimerDesign();

            MessageDlg* MDlg = new MessageDlg("Primer Design Report", logDesign);

            QTextCharFormat emFormat = QTextCharFormat();
            emFormat.setForeground(QColor("#ff0000"));
            emFormat.setFontPointSize(12);

            QString tmpStr;
            if(primerPairList.size() != 0) {
                tmpStr = QString("%1 primer pairs found.\r\n\r\n").arg(primerPairList.size());
                MDlg->insertRichText(tmpStr);
            }else{
                tmpStr = QString("No primer pairs found.\r\n\r\n");
                MDlg->insertRichText(tmpStr, emFormat);
                MDlg->enableOK(false);
            }


            MDlg->exec();
            if(!MDlg->retry && MDlg->cont){
                delete MDlg;
                if(primerPairList.size() != 0){
                    PrimerReportWindow  *prWin = new PrimerReportWindow(this);
                    if(prWin->showWindow)
                        prWin->show();
                }
                break;
            }
            else{
                DataTools::cleanMemoryAllData();
                if(!MDlg->retry){
                    break;
                }
            }
        }
    }
    this->removeFlat();

    pWprimer->close();
}


void PRISE::blast()
{
    // Get parameters
    QString comm = QString("blastn -db \"%1\" -query \"%2\" -task %3").arg(databaseName).arg(blastSeedFile).arg(blastType);

    if(!entrezQuery.isEmpty())
        comm += QString(" -entrez_query \"%1\"").arg(entrezQuery);


    int maxTarSeqs = optionMaxTarSeq[idxMaxTarSeqs];
    int wordSize = optionWordSize[idxWordSize];
    int reward = optionReward[idxMatchScore];
    int penalty = optionPenalty[idxMatchScore];


    comm += QString(" -max_target_seqs %1 -evalue %2"
                     " -word_size %3 -reward %4 -penalty %5").arg(maxTarSeqs).arg(eVal).arg(wordSize).arg(reward).arg(penalty);

    if(idxGap != 0){
        int gapOpen_blast = optionGapOpen[idxGap];
        int gapExtn_blast = optionGapExtend[idxGap];

        comm += QString(" -gapopen %6 -gapextend %7").arg(gapOpen_blast).arg(gapExtn_blast);
    }
    if(dustFilter)
        comm += " -dust yes";
    if(lowercaseMask)
        comm += " -lcase_masking";


    comm += " -outfmt \"7 qseqid sseqid pident length mismatch gapopen qstart qend sstart send evalue bitscore\"";
//    QMessageBox::about(this, "Paras",comm);
//    qDebug() << comm << endl;


    BlastResultDialog* brd = new BlastResultDialog(comm, this);
    brd->show();
    if (!brd->runSuccessufully())
        brd->close();
    //execBlast(comm);
}

/*
void PRISE::execBlast(QString command){



    process.start(command);
    process.waitForFinished(-1); // will wait forever until finished

    output = process.readAllStandardOutput();
    QString stdError = process.readAllStandardError();


    BlastResultDialog* brd = new BlastResultDialog(this);

    connect(process, SIGNAL(readyReadStdError()), this, SLOT(attachErrorMsg()));
    connect(process, SIGNAL(readyReadStdOutput()), this, SLOT(attachOutput()));

    connect(myProcess, SIGNAL(readyReadStdError()), this, SLOT(updateError()));
    connect(myProcess, SIGNAL(readyReadStdOutput()), this, SLOT(updateText()));


//    brd->attachOutput(output);
//    brd->attachErrorMsg(stdError);
    brd->exec();


    //QMessageBox::about(this, "result",output);
    //QMessageBox::about(this, "result",stdError);
}*/




void PRISE::seedHitTableHelp()
{
    //TODO: open up a pdf document.... or do something to display it.
    const char *const url = ":/instructions/Step_1.1_Instructions.pdf";
    QFile helpFile(url);
    if(!helpFile.exists()){
        QMessageBox::warning(this, "Help file is missing", QString("Cannot find the help file below:\n%1").arg(url));
    }else{
        QDesktopServices::openUrl(QUrl::fromLocalFile(url));
    }
}

void PRISE::openManual()
{
    //TODO: open up a pdf document.... or do something to display it.
    const char *const url = ":/instructions/PRISE_Manual.pdf";
    if(QFile::exists(url)) {

    }
    //QFile helpFile(url);
    //if(!helpFile.exists()){
    //    QMessageBox::warning(this, "Help file is missing", QString("Cannot find the help file below:\n%1").arg(url));
    //}else{
    //    QDesktopServices::openUrl(QUrl::fromLocalFile(url));
    //}
}

void PRISE::openTutorial()
{
    //TODO: open up a pdf document.... or do something to display it.
    const char *const url = ":/instructions/PRISE_Tutorial.pdf";
    if(QFile::exists(url)) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(url));
    }
}

void PRISE::aboutPRISE()
{
    const char *const message =

    "PRISE Version: 3.0\n"
    "Build Date: 10/4/2016\n"
    "Copyright ©2016. The Regents of the University of California.\n"
    "All Rights Reserved.\n";

    QMessageBox::about(0, "About PRISE", message);
}

void PRISE::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void PRISE::removeFlat()
{
    ui->pushButton_2->setFlat(false);
    ui->pushButton_3->setFlat(false);
}


void PRISE::on_pushButton_HitTable_clicked()
{


    BlastWizard* bW = new BlastWizard(this);
    if(bW->exec()){

        //TODO: best example of WHY global variables are terrible.....
        //TODO: i don't know how BlastWizard is being modified... there could be a million ways
        //this variable could be manipulated....
        //TODO: fix the blast option!!!!
        if(!blastOption)
            seedHitTableHelp();
        else{
            blast();
        }
    }
//    BlastWizard* blastwiz = new BlastWizard(this);
//    blastwiz->exec();

//    if(!blastOption){
//        QString url = "";
//    #ifdef MAC
//        QString appPath = QDir::currentPath();
//        url += "file:/";
//        url += appPath;
//        url += "/";
//    #endif
//    #ifdef WIN
//        url += "instructions\\Step_1.1_Instructions.pdf";
//    #else
//        url += "instructions/Step_1.1_Instructions.pdf";
//    #endif
//        QDesktopServices::openUrl(QUrl(url));
//    }
}

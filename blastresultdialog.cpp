#include "blastresultdialog.h"
#include "ui_blastresultdialog.h"
#include "stdlib.h"
#include <QDebug>

BlastResultDialog::BlastResultDialog(QString comm, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BlastResultDialog)
{
    ui->setupUi(this);
    blastProcess = new QProcess(this);

    connect(blastProcess, SIGNAL(readyReadStandardError()), this, SLOT(attachErrorMsg()));
    connect(blastProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(attachOutput()));
    connect(blastProcess, SIGNAL(finished(int)), this, SLOT(updateExit()));
    ui->plainTextEdit_stdout->clear();
    ui->plainTextEdit_stdout->clear();
    isRunning = false;
    firstMsg = true;
    this->execBlast(comm);
}

BlastResultDialog::~BlastResultDialog()
{
    delete ui;
    delete blastProcess;
}

void BlastResultDialog::reject(){
    if(isRunning){

        QMessageBox::StandardButton reply;

        reply = QMessageBox::question(this, "Running", "Blast is still Running.\n"
                                      "Do you really want to quit?",
                                        QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::No) {
            return;
        }
    }

    blastProcess->kill();
    isRunning = false;
    QDialog::reject();
}


void BlastResultDialog::attachOutput(){
    if(firstMsg){
        ui->plainTextEdit_stdout->clear();
        firstMsg = false;
    }
    QString output = blastProcess->readAllStandardOutput();
    ui->plainTextEdit_stdout->appendPlainText( output);
}

void BlastResultDialog::attachErrorMsg(){
//    if(firstMsg){
//        //ui->plainTextEdit_stdout->clear();
//        //firstMsg = false;
//    }
    QString err = blastProcess->readAllStandardError();
    ui->plainTextEdit_stderr->appendPlainText( err);
}


void BlastResultDialog::updateExit(){
    isRunning = false;
    ui->pushButton->setEnabled(true);
    if(firstMsg){
        QMessageBox::information(this, "Blast is done", "No blast output.\n Please check the error message,\nand make sure your blast program works.");
        ui->plainTextEdit_stdout->clear();
    }
    else
        QMessageBox::information(this, "Blast is done", "Blast is done, now you can save the output as hit table.");
}

void BlastResultDialog::on_pushButton_clicked()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save result as Hit table file..."),
                                              pwd, tr("txt files (*.txt);;All Files (*)"));
    if (fn.isEmpty())
        return;
    if (!fn.endsWith(".txt", Qt::CaseInsensitive ))
        fn += ".txt"; // default

    saveHitTableTo = fn;
    QString output = ui->plainTextEdit_stdout->toPlainText();
    if (!fileSave(fn, output))
        QMessageBox::warning(this, "Error writing hit table file", "Cannot save hit table file successfully, please check your path and space");
}


bool BlastResultDialog::fileSave(QString filename, QString info4write)
{
    QFile file(filename);
    //file.open(QIODevice::WriteOnly | QIODevice::Text);


    if ( file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream( &file );
        stream << info4write;
        file.close();
        return true;
    }
    return false;
}

void BlastResultDialog::execBlast(QString command){
    succRunBlast = true;
    isRunning = true;
    ui->pushButton->setEnabled(false);
    ui->plainTextEdit_stdout->insertPlainText("#Please wait while blast is running...\n");
    ui->plainTextEdit_stdout->insertPlainText("#When blast is done, you will see a popup message.\n");
    ui->plainTextEdit_stdout->insertPlainText("==================================================\n");


    //QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    //QMessageBox::warning(this, "Error starting blast", " 0 env path = "+env.value("Path"));
    //env.insert("PATH", env.value("Path") + ";" + blastBin);
    //QMessageBox::warning(this, "Error starting blast", " 1 env path = "+env.value("Path"));
    //blastProcess->setProcessEnvironment(env);
//    QMessageBox::warning(this, "Error starting blast", " 2 env path = "+blastProcess->processEnvironment().value("Path"));

    QString blastPath=QString::fromLocal8Bit( qgetenv("PATH").constData() );
    QString dbPath=QString::fromLocal8Bit( qgetenv("BLASTDB").constData() );
#ifdef WIN

    blastPath += ";"+blastBin;
    _putenv_s("PATH", blastPath.toStdString().c_str());

    dbPath += ";"+databasePath;
    _putenv_s("BLASTDB", dbPath.toStdString().c_str());
//    QString value=QString::fromLocal8Bit( qgetenv("PATH").constData() );
//    QMessageBox::warning(this, "Error starting blast", " 1 env path = "+value.replace(";","\n"));

//    value.clear();
//    value=QString::fromLocal8Bit( qgetenv("BLASTDB").constData() );
//    QMessageBox::warning(this, "Error starting blast", " 1 db path = "+value.replace(";","\n"));


#else
#ifdef MAC
    blastPath += ":"+blastBin;
    setenv("PATH",blastPath.toStdString().c_str(),1);

    dbPath += ":"+databasePath;
    setenv("BLASTDB",dbPath.toStdString().c_str(),1);
    //QString value=QString::fromLocal8Bit( qgetenv("PATH").constData() );
    //QMessageBox::warning(this, "Error starting blast", " 1 env path = "+value.replace(":","\n"));
#endif
#endif

    //TODO: ???????
    //blastn -db "" -query "" -task megablast -max_target_seqs 500 -evalue 10 -word_size 20 -reward 1 -penalty -2 -dust yes -outfmt

    blastProcess->start(command);

    if(!blastProcess->waitForStarted()){
        QMessageBox::warning(this, "Error when starting blast", "Cannot start blast, please check if you give the correct blast path.");
        isRunning = false;
        ui->pushButton->setEnabled(true);
        ui->plainTextEdit_stdout->clear();
        succRunBlast = false;
    }
//    blastProcess->waitForFinished(-1); // will wait forever until finished

//    output = process.readAllStandardOutput();
//    QString stdError = process.readAllStandardError();


//    BlastResultDialog* brd = new BlastResultDialog(this);



//    connect(myProcess, SIGNAL(readyReadStdError()), this, SLOT(updateError()));
//    connect(myProcess, SIGNAL(readyReadStdOutput()), this, SLOT(updateText()));


//    brd->attachOutput(output);
//    brd->attachErrorMsg(stdError);
//    brd->exec();


    //QMessageBox::about(this, "result",output);
    //QMessageBox::about(this, "result",stdError);
}

bool BlastResultDialog::runSuccessufully(){
    return succRunBlast;
}

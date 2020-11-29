#include "SeqDiffWindow.h"
#include "ui_SeqDiffWindow.h"
#include <QDesktopServices>

SeqDiffWindow::SeqDiffWindow(QVector<struct SequenceList*> showList,
                             QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SeqDiffWindow)
{
    ui->setupUi(this);
    setCentralWidget(this->ui->widget);
    tableModel =  new QStandardItemModel();

    this->seqShowList = showList;
}

SeqDiffWindow::~SeqDiffWindow()
{
    delete ui;
}

void SeqDiffWindow::changeEvent(QEvent *e)
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

void SeqDiffWindow::initialize()
{
    createModel();
    int range = seqShowList.size();
    MyProgressDlg pDlg("Loading different sequence list",0, range);
    pDlg.setWindowModality(Qt::ApplicationModal);

    tableModel->insertRows(0,range);
    for(int i = 0; i < range; i++){
        pDlg.setValue(i);
        insertData(seqShowList[i],i);
    }
}

//Set TableView headers and fields.
bool SeqDiffWindow::createModel()
{
    cleanModel();
    if(isSimple){
        tableModel =  new QStandardItemModel(0, 5+qList.size(), this);
    }else{
        tableModel =  new QStandardItemModel(0, 9+qList.size(), this);
    }
    ui->tableView->setModel(tableModel);
    ui->tableView->setSortingEnabled(true);

    //tableModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Seq #"));
    tableModel->setHeaderData(0, Qt::Horizontal, QObject::tr("NCBI GI#"));
    tableModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Accession #"));

    if( qList.size() == 1 ){
        tableModel->setHeaderData(2, Qt::Horizontal, QObject::tr("% identity"));
    } else{
        //for( int i = 0; i < qList.size(); i++ ){
        //    tableModel->setHeaderData(i+2, Qt::Horizontal, QObject::tr("%id : %1").arg(qList.at(i)->gi));
        //}
    }
    tableModel->setHeaderData(qList.size()+2, Qt::Horizontal, QObject::tr("Length"));
    tableModel->setHeaderData(qList.size()+3, Qt::Horizontal, QObject::tr("DEFINITION"));
    tableModel->setHeaderData(qList.size()+4, Qt::Horizontal, QObject::tr("ORGANISM"));

    ui->tableView->setColumnWidth(0,40);
    ui->tableView->setColumnWidth(qList.size()+2,50);
    ui->tableView->setColumnWidth(qList.size()+3,300);
    ui->tableView->setColumnWidth(qList.size()+4,300);

    if( ! isSimple ){
        tableModel->setHeaderData(qList.size()+5, Qt::Horizontal, QObject::tr("SOURCE"));
        tableModel->setHeaderData(qList.size()+6, Qt::Horizontal, QObject::tr("FEATURES"));
        tableModel->setHeaderData(qList.size()+7, Qt::Horizontal, QObject::tr("AUTHORS"));
        tableModel->setHeaderData(qList.size()+8, Qt::Horizontal, QObject::tr("TITLE"));

        ui->tableView->setColumnWidth(qList.size()+5,200);
        ui->tableView->setColumnWidth(qList.size()+6,300);
        ui->tableView->setColumnWidth(qList.size()+7,200);
        ui->tableView->setColumnWidth(qList.size()+8,300);
    }
    return true;
}

void SeqDiffWindow::insertData(struct SequenceList* seqPt, int pos)
{
    //tableModel->setData(tableModel->index(pos, 0), seqPt->gi);
    tableModel->setData(tableModel->index(pos, 1), seqPt->accession);

    for( int i = 0; i < qList.size(); i++ ){
        tableModel->setData(tableModel->index(pos, i+2), seqPt->identity[i]);
    }

    tableModel->setData(tableModel->index(pos, qList.size()+2), seqPt->length);
    tableModel->setData(tableModel->index(pos, qList.size()+3), seqPt->definition);
    tableModel->setData(tableModel->index(pos, qList.size()+4), seqPt->organism);

    if( ! isSimple ){
        tableModel->setData(tableModel->index(pos,qList.size()+5), seqPt->source);
        tableModel->setData(tableModel->index(pos,qList.size()+6), seqPt->features);
        tableModel->setData(tableModel->index(pos,qList.size()+7), seqPt->authors);
        tableModel->setData(tableModel->index(pos,qList.size()+8), seqPt->title);
    }
}

void SeqDiffWindow::emptyModel()
{
    for(int i = 0; i < tableModel->rowCount(); i++){
        delete tableModel->item(i,0);
    }
    tableModel->removeRows(0,tableModel->rowCount());
}

void SeqDiffWindow::cleanModel()
{
    this->emptyModel();
    tableModel->clear();
}


void SeqDiffWindow::on_actionSave_Sequence_List_sequence_file_triggered()
{
    if(this->seqShowList.size() == 0){
        QMessageBox::about(Q_NULLPTR, "Empty List", "Nothing to be saved");
        return;
    }

    QFileDialog file_dialog( this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getSaveFileName( this, tr( "Save Sequence List File" ), pwd, tr( "Sequence List File (*.sequence);;All File (*.*)"));
    if(filename.isEmpty()){
        return;
    }
    DataTools::saveSeqList(filename, seqShowList);
}

void SeqDiffWindow::on_actionSave_Seed_Sequence_List_as_Tab_Delimited_File_Excel_File_triggered()
{
    if(sList.size() == 0){
        QMessageBox::about(this, "Empty List", "Nothing to be saved");
        return;
    }

    QFileDialog file_dialog( this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getSaveFileName( this, tr( "Save Sequence List File to TXT" ), pwd, tr( "Text File (*.txt);;All File (*.*)"));
    if(filename.isEmpty()){
        return;
    }

    QStringList headers;
    for( int i = 1; i < this->tableModel->columnCount(); i++ ){
        QString header = tableModel->headerData(i, Qt::Horizontal).toString();
        headers.push_back(header);
    }
    DataTools::saveSeqList2TXT(filename,sList, headers);
}

void SeqDiffWindow::on_actionExit_triggered()
{
    this->close();
}

void SeqDiffWindow::on_actionUpdate_Identity_for_All_Sequences_triggered()
{
    if (sList.size() == 0){
        return;
    }
    //Get alignment user settings
    SeqAlignSettingDlg saDlg(this);

    if(!saDlg.exec()){
        return;
    }

    this->emptyModel();
    if (!DataTools::UpdateIdentity()){
        QMessageBox::about(Q_NULLPTR, "Alignment Error", "Updating % identity was not finished.\n"
                           "The displayed information might be incomplete and inaccurate.\n");
    }
}

void SeqDiffWindow::on_actionPRISE_Manual_triggered()
{
    QString url = ":/instructions/PRISE_Manual.pdf";
    QDesktopServices::openUrl(QUrl::fromLocalFile(url));
}

void SeqDiffWindow::on_actionPRISE_Tutorial_triggered()
{
    QString url = "instructions/PRISE_Tutorial.pdf";
    QDesktopServices::openUrl(QUrl::fromLocalFile(url));
}

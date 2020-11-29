#include "SeqReportWindow.h"
#include "ui_SeqReportWindow.h"
#include "prise.h"

using namespace std;
//Used to update the QTableView.
static SeqReportWindow* tmp;

void OnInsertData(struct SequenceList* )
{
    emit tmp->report_model->layoutAboutToBeChanged();
    emit tmp->report_model->layoutChanged();
}

void OnCreateModel()
{

}

SeqReportWindow::SeqReportWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SeqReportWindow)
{
    ui->setupUi(this);
    setCentralWidget(this->ui->widget);
    //ui->
    report_model = new SeqReportModel(this);
    ui->tableView->setModel(report_model);
    //tableModel =  new QStandardItemModel();

    //Used to update the QTreeView. By function pointer
    tmp = this;
    DataTools::registerOnInsertData(OnInsertData);
    DataTools::registerOnCreateModel(OnCreateModel);
    QHeaderView * HV = ui->tableView->horizontalHeader();
    connect(HV, SIGNAL(sectionClicked(int)), this->report_model, SLOT(sortData(int)));
}

SeqReportWindow::~SeqReportWindow()
{
    delete ui;
    //delete tableModel;
}

//Set TableView headers and fields.
bool SeqReportWindow::createModel()
{
    return true;
}

void SeqReportWindow::changeEvent(QEvent *e)
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

void SeqReportWindow::closeEvent(QCloseEvent *event)
{
    PRISE *p = static_cast<PRISE*>(parentWidget());
    p->removeFlat();
    DataTools::cleanMemorySeqs();
    event->accept();
}

void SeqReportWindow::emptyModel()
{
    emit report_model->layoutAboutToBeChanged();
    emit report_model->layoutChanged();
}

void SeqReportWindow::cleanModel()
{
    emit report_model->layoutAboutToBeChanged();
    emit report_model->layoutChanged();
}

void SeqReportWindow::on_actionLoad_Seed_Sequence_and_Hit_Table_triggered()
{
    LoadSeedHitTableDlg* shtDlg = new LoadSeedHitTableDlg(this);
    if(!shtDlg->exec())
        return;

    QString SeedFile,HitTableFile,FASTAFile, SeedAccs;
    accTooLongSeqToAlign.clear();
    accTooLongSeqToDownload.clear();
    DataTools::cleanMemorySeqs();
    queryName.clear();
    shtDlg->getFilePath(SeedFile,HitTableFile,FASTAFile);
    shtDlg->getAccessions(SeedAccs);
    if (DataTools::loadSeed(SeedAccs, SeedFile)==false)
        return;

    this->setWindowTitle(HitTableFile + "- Select Target and Non-target Sequences");
    DataTools::SaveQueryList();

    //Get alignment user settings
    SeqAlignSettingDlg saDlg(this);

    if(!saDlg.exec()){
        return;
    }

    //Parse Hit Table, add to sList
    if( HitTableFile != "" ){
        if( DataTools::ParseHitTable(HitTableFile) == false ){
            return;
        }
    }

    //Download information from NCBI
    int re = DataTools::SearchNCBI();
    if(re == 1){
        //user abort
        DataTools::cleanMemorySeqs();
        cleanModel();
        return;
    }else if (re == -1) {
        //internet error or data error
        emit report_model->layoutAboutToBeChanged();
        for(int i = 0; i<sList.size(); i++){
            for(int j = 0; j<qList.size(); j++){
                sList[i]->identity[j] =TINY_IDENTITY_VALUE;
            }
        }
        emit report_model->layoutChanged();
        QMessageBox::about(this, "Abort", "Sequence extraction was not finished.\n"
                           "The displayed information might be incomplete and inaccurate.\n");
        return;
    }

    // get fasta sequences if fasta file is provided
    if( FASTAFile != "" )
        DataTools::getFastaSequences(FASTAFile);

    emptyModel();
    if (! DataTools::UpdateIdentity()){
        QMessageBox::about(this, "Alignment Error", "Updating % identity was not finished.\n"
                           "The displayed information might be incomplete and inaccurate.\n");
        return;
    }

    delete shtDlg;
    //delete saDlg;
    QString message = QString("%1 sequence(s) were downloaded\n\n").arg(sList.size());
    if (missingList.size()>0){
        message += QString("%1 sequence(s) were failed to be found in NCBI:\n").arg(missingList.size());
        //for(int i = 0; i<missingList.size(); i++){
        //    message += QString("Accession# %1\n").arg(missingList[i]->gi);
        //}
    }

    if (notDownloadList.size()>0){
        message += QString("%1 sequence(s) were failed to be downloaded from  NCBI:\n").arg(notDownloadList.size());
        message += QString("( Note: Connection errors are more likely to occur for long sequences.\n Editing the sequences to reduce their length may prevent these errors from occurring.)\n");
    }

    if (accTooLongSeqToDownload.size()>0){
        message += QString("\n%1 sequence(s) were skipped (exceed the maximal size for downloading):\n").arg(accTooLongSeqToDownload.size());
        for(int i = 0; i<accTooLongSeqToDownload.size(); i++){
            message += QString("Accession# %1\n").arg(accTooLongSeqToDownload[i]);
        }
    }
    if (accTooLongSeqToAlign.size()>0){
        message += QString("\n%1 sequence(s) were not aligned (exceed the maximal size for pairwise alignment):\n").arg(accTooLongSeqToAlign.size());
        for(int i = 0; i<accTooLongSeqToAlign.size(); i++){
            message += QString("Accession# %1\n").arg(accTooLongSeqToAlign[i]);
        }
    }
    MessageDlg MDlg("Sequence Downloading Report", message);
    MDlg.hideRetry();
    MDlg.exec();

    if(!MDlg.cont){
        emptyModel();
        DataTools::cleanMemorySeqs();
        queryName.clear();
    }
}

void SeqReportWindow::on_actionSave_Seed_Sequence_List_sequence_File_triggered()
{
    if(sList.size() == 0){
        QMessageBox::about(this, "Empty List", "Nothing to be saved");
        return;
    }

    //TODO: i don't know how to change the directory to the PRISE directory...
    QFileDialog file_dialog( this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getSaveFileName( this, tr( "Save Sequence List File" ), "", tr( "Sequence List File (*.sequence);;All File (*.*)"));
    if(filename.isEmpty()){
        return;
    }
    DataTools::saveSeqList(filename,sList);
}

void SeqReportWindow::on_actionLoad_Seed_Sequence_List_sequence_File_triggered()
{
    //TODO: hard coded, i don't know how to change the directory to the PRISE directory....
    QFileDialog file_dialog(this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getOpenFileName( this, tr( "Load Sequence List File" ), "", tr( "Sequence List File (*.sequence);;All File (*.*)" )) ;
    if(filename.isEmpty()){
        return;
    }
    DataTools::cleanMemorySeqs();
    queryName.clear();
    DataTools::loadSeqList(filename,sList);
}

void SeqReportWindow::on_actionSave_Seed_Sequence_List_as_Tab_Delimited_File_Excel_File_triggered()
{
    //TODO: FIXME
    /*
    if(sList.size() == 0){
        QMessageBox::about(this, "Empty List", "Nothing to be saved");
        return;
    }

    QFileDialog file_dialog( this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getSaveFileName( this, tr( "Save Sequence List File to TXT" ), "", tr( "Text File (*.txt);;All File (*.*)"));
    if(filename.isEmpty()){
        return;
    }
    DataTools::saveSeqList2TXT(filename,sList, ui->tableView->headers());*/
}

void SeqReportWindow::on_actionSave_FASTA_Sequences_as_triggered()
{
    QFileDialog file_dialog( this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getSaveFileName( this, tr( "Save Sequence List File to TXT" ), pwd, tr( "Text File (*.txt);;All File (*.*)"));
    if(filename.isEmpty()){
        return;
    }
    QString FASTAContent = ui->textEdit->toPlainText();
    DataTools::saveFASTASeqListAs(filename, FASTAContent);
}

void SeqReportWindow::on_actionAdd_FASTA_Sequences_to_An_Existing_File_triggered()
{
    QFileDialog file_dialog( this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getSaveFileName( this, tr( "Save Sequence List File to TXT" ), pwd, tr( "Text File (*.txt);;All File (*.*)"));
    if(filename.isEmpty()){
        return;
    }
    QString FASTAContent = ui->textEdit->toPlainText();
    DataTools::appendFASTASeqListTo(filename, FASTAContent);
}

void SeqReportWindow::withinAllData(bool selectedOnly, QVector<int> selectedSeqList, bool setMarked)
{
    emit this->report_model->layoutAboutToBeChanged();
    int size = selectedOnly==true? selectedSeqList.size() : sList.size();
    for(int i = 0; i < size; i++) {
        sList[i]->marked = setMarked;
    }
    emit this->report_model->layoutChanged();
}

void SeqReportWindow::withinConstraintedData(bool selectedOnly, QVector<int> selectedSeqList, bool setMarked)
{
    //collect all sequences that need to be checked
    QVector<struct SequenceList*> processList;
    if(selectedOnly == false) {
        for(int i = 0; i< sList.size(); i++){
            sList[i]->marked = setMarked;
            processList.push_back(sList[i]);
        }
    } else {
        for(int i = 0; i< selectedSeqList.size(); i++){
            int idx = selectedSeqList[i];
            sList[idx]->marked = setMarked;
            processList.push_back(sList[idx]);
        }
    }

    //Check against each constraint one by one
    if(markConditionalCheck[0]){
        for(int i = 0; i < processList.size(); i++){
            if (processList[i]->length < markLenMin || processList[i]->length > markLenMax){
                processList[i]-> marked = !setMarked;
            }
        }
    }
    if(markConditionalCheck[1]){
        for(int i = 0; i < processList.size(); i++) {
            //TODO: THIS CODE IS COMPLETELY BIZARRE --Daniel
            int queryId = queryId;
            if (processList[i]->identity[queryId] < markIdMin || processList[i]->identity[queryId] > markIdMax){
                processList[i]-> marked = !setMarked;
            }
        }
    }
    if(markConditionalCheck[2]){
        for(int i = 0; i < processList.size(); i++){
            QString str  = processList[i]->definition;
            str = str.toLower();
            if(contains(str, markDefConstraint) != markContain[0] )
                processList[i]-> marked = !setMarked;
        }
    }
    if(markConditionalCheck[3]){
        for(int i = 0; i < processList.size(); i++){
            QString str  = processList[i]->organism;
            str = str.toLower();
            if(contains(str, markOrgConstraint) != markContain[1] )
                processList[i]-> marked = !setMarked;
        }
    }
    QString str;
    if(markConditionalCheck[4]){

        for(int i = 0; i < processList.size(); i++){
            switch(markConditionalCheck[4]){
            case 1:
               str  = processList[i]->source; break;
            case 2:
                str  = processList[i]->features; break;
            case 3:
                str  = processList[i]->authors; break;
            case 4:
                str  = processList[i]->title; break;
            default: break;
            }
            str = str.toLower();
            if(contains(str, markExtnStr1) != markContain[2] )
                processList[i]-> marked = !setMarked;
        }
    }

    if(markConditionalCheck[5]){
        for(int i = 0; i < processList.size(); i++){
            switch(markConditionalCheck[5]){
            case 1:
               str  = processList[i]->source; break;
            case 2:
                str  = processList[i]->features; break;
            case 3:
                str  = processList[i]->authors; break;
            case 4:
                str  = processList[i]->title; break;
            default: break;
            }
            str = str.toLower();
            if(contains(str, markExtnStr2) != markContain[3] )
                processList[i]-> marked = !setMarked;
        }
    }
}

bool SeqReportWindow::contains(QString text, QString pattern)
{
    if( pattern.indexOf("\"") < 0 ){ // pattern is a single term
        if( text.indexOf(pattern) >= 0 )
            return true;
        else
            return false;
    }
    // parse the compound string "pattern"
    int pos;
    bool appear = false;
    QString p, andOr;

    // get the 1st word term
    pos = pattern.indexOf("\"");
    pattern.remove(0,pos+1);
    pos = pattern.indexOf("\"");
    p = pattern.left(pos);
    pattern.remove(0,pos+1); // delete word term
    pattern = pattern.trimmed(); // trim space
    if( text.indexOf(p) >= 0 ){
        appear = true;
    }

    while( pattern.length() > 0 ){
            // get a "and/or" term
            pos = pattern.indexOf("\"");
            andOr = pattern.left(pos);
            andOr.remove(' ');
            pattern.remove(0,pos+1);

            // get a word term
            pos = pattern.indexOf("\"");
            p = pattern.left(pos);
            pattern.remove(0,pos+1); // delete word term
            //pattern.trimmed(); // trim space

            if( andOr == "and" ){
                if( appear == 1 && text.indexOf(p) >= 0 )appear = true;
                else appear = false;
            }else if( andOr == "or" ){
                if( appear == 1 || text.indexOf(p) >= 0 ) appear = true;
                else appear = false;
            }
    }
    return appear;
}

void SeqReportWindow::on_actionMark_Sequences_triggered()
{
    if(sList.size() == 0){
        return;
    }
    int nMarkedPrev=0;
    QString str;
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    //selected sequence indices:
    QVector<int> selectedSeqList;

    //track selected sequences
    for(int i = 0; i<selectedRows.length(); i++){
        int row = selectedRows.at(i).row();
        selectedSeqList.push_back(row);
    }

    SeqMarkUnmarkDlg smDlg(1,isSimple,queryName,this);
    if (smDlg.exec())
    {
        for(int i = 0; i < sList.size(); i++){
            if(sList[i]->marked) nMarkedPrev ++;
        }
        if(markAllData){//process in complete data.
            withinAllData(markWithinSelected, selectedSeqList, true);
        }else if (markConstrainted){ //process in constrainted data.
            withinConstraintedData(markWithinSelected, selectedSeqList, true);
        }

        int nMarkedNow =0;
        //count how many sequence are marked after the action;
        for(int i= 0; i < sList.size(); i++){
            if(sList[i]->marked) nMarkedNow++;
        }

        str = QString("%1 Sequences were previously marked.\r\n"
                      "%2 Sequences were newly marked.\r\n"
                      "%3 Sequences were currently marked.").arg(nMarkedPrev).arg(nMarkedNow-nMarkedPrev).arg(nMarkedNow);
        QMessageBox::about(this, "Report", str);
    }
}

void SeqReportWindow::on_actionUnmark_Sequences_triggered()
{
    if(sList.size() == 0){
        return;
    }
    int nMarkedPrev=0;
    QString str;
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    //selected sequence indices:
    QVector<int> selectedSeqList;

    //track selected sequences
    for(int i = 0; i<selectedRows.length(); i++){
        int row = selectedRows.at(i).row();
        selectedSeqList.push_back(row);
    }
    SeqMarkUnmarkDlg suDlg(0,isSimple,queryName,this);
    if (suDlg.exec())  //user pressed OK
    {
        for(int i = 0; i < sList.size(); i++){
            if(sList[i]->marked) nMarkedPrev ++;
        }
        if(markAllData){//process in complete data.
            withinAllData(markWithinSelected, selectedSeqList, false);
        }else if (markConstrainted){ //process in constrainted data.
            withinConstraintedData(markWithinSelected, selectedSeqList, false);
        }

        int nMarkedNow =0;
        //count how many sequence are marked after the action;
        for(int i= 0; i < sList.size(); i++){
            if(sList[i]->marked) nMarkedNow++;
        }

        str = QString("%1 Sequences were previously marked.\r\n"
                      "%2 Sequences were unmarked.\r\n"
                      "%3 Sequences were currently marked.").arg(nMarkedPrev).arg(nMarkedPrev-nMarkedNow).arg(nMarkedNow);
        QMessageBox::about(this, "Report", str);
    }
}

void SeqReportWindow::on_actionReverse_Marked_and_Unmark_triggered()
{
    if (sList.size() == 0){
        return;
    }
    emit report_model->layoutAboutToBeChanged();
    for(int i = 0; i < sList.size(); i++) {
        sList[i]->marked = !sList[i]->marked;
    }
    emit report_model->layoutChanged();
}

void SeqReportWindow::rightClick (const QPoint & point)
{
    const QModelIndex idx = ui->tableView->indexAt(point);
    clickedRow = idx.row();
    showRightClickMenu();
}

void SeqReportWindow::showRightClickMenu ()
{
  QMenu popupMenu(ui->tableView);
  QAction *actionDisplayPairwiseAlignment = new QAction(ui->tableView);
  QAction *actionInstantBlast;
  actionDisplayPairwiseAlignment->setObjectName(QString::fromUtf8("Display Pairwise Alignment"));
  actionDisplayPairwiseAlignment->setText(QString::fromUtf8("Display Pairwise Alignment"));
  actionInstantBlast = new QAction(ui->tableView);
  actionInstantBlast->setObjectName(QString::fromUtf8("Instant Blast"));
  actionInstantBlast->setText(QString::fromUtf8("Instant Blast"));
  popupMenu.addAction(actionDisplayPairwiseAlignment);
  popupMenu.addAction(actionInstantBlast);

  connect(actionDisplayPairwiseAlignment, SIGNAL(triggered()), this, SLOT(diaplayAlignment()));
  connect(actionInstantBlast, SIGNAL(triggered()), this, SLOT(instantBlast()));
  popupMenu.exec(QCursor::pos());
}

void SeqReportWindow::diaplayAlignment()
{
    struct SequenceList* seqPtr = sList[clickedRow];
    clickedRow = -1;
    if( seqPtr->length > MAX_ALIGN_SIZE ){
        QMessageBox::about(this, "Too long sequence","Sequence with accession " + seqPtr->accession + " exceeds the maximal size for pairwise alignment.");
        return;
    }
    QVector<short*> querySeqs;
    QVector<int> queryLens;
    QVector<QString> queryGIs;
    if( querySeqs.size() == 0 ) return;

    SeqAlignShowWindow* sasWindow = new SeqAlignShowWindow(seqPtr->seqInt, seqPtr->accession, seqPtr->length, querySeqs, queryGIs, queryLens, this);
    sasWindow->show();
}

void SeqReportWindow::instantBlast()
{
    //struct SequenceList* seqPtr = sList[clickedRow];
    clickedRow = -1;

    QString url = "http://www.ncbi.nlm.nih.gov/blast/Blast.cgi?"
                              "ALIGNMENTS=50&ALIGNMENT_VIEW=Pairwise"
                              "&AUTO_FORMAT=Semiauto&CLIENT=web&DATABASE=nr"
                              "&DESCRIPTIONS=100&END_OF_HTTPGET=Yes"
                              "&ENTREZ_QUERY=(none)&EXPECT=10&FILTER=L"
                              "&FORMAT_OBJECT=Alignment&FORMAT_TYPE=HTML"
                              "&GET_SEQUENCE=yes&HITLIST_SIZE=100"
                              "&LAYOUT=TwoWindows&NCBI_GI=on&PAGE=Nucleotides"
                              "&PROGRAM=blastn"
                              "&QUERY=";
    //url += seqPtr->gi +
    //                          "&SERVICE=plain&SET_DEFAULTS.x=34&SET_DEFAULTS.y=8"
    //                          "&SHOW_LINKOUT=yes&SHOW_OVERVIEW=on&END_OF_HTTPGET=Yes";

    QDesktopServices::openUrl(QUrl(QString(url)));
}

void SeqReportWindow::on_actionMove_Marked_Sequences_to_FASTA_Sequence_Box_triggered()
{
    if(sList.size() == 0)   return;

    int nMarks = 0;
    for( int i = 0; i < sList.size(); i++ ){
        if(sList[i]->marked) nMarks++;
    }

    if( nMarks == 0 ){
        QMessageBox::about(this, "Empty selection", "No sequence is marked!");
        return;
    }

    MyConfirmDlg CDlg("Are you sure you want to move marked sequence(s)?",this);
    if(!CDlg.exec()){
        return;
    }

    QString str = QString("%1 marked sequences were moved to FASTA sequence box.").arg(nMarks);
    // start moving to fasta file edit box
    int range = sList.size();
    MyProgressDlg pDlg("Moving marked sequences:",1, range);
    pDlg.setWindowModality(Qt::ApplicationModal);
/////////////////////////////////////////////////////////////////////////
//    bool res = SeqReportWindow::needsComplement();
//    LoadSeedHitTableDlg* shtDlg = new LoadSeedHitTableDlg(this);
//    if(!shtDlg->exec())
//        return;

//    QString SeedFile,HitTableFile,FASTAFile, SeedAccs;
//    accTooLongSeqToAlign.clear();
//    accTooLongSeqToDownload.clear();
//    DataTools::cleanMemorySeqs();
//    queryName.clear();
//    shtDlg->getFilePath(SeedFile,HitTableFile,FASTAFile);
//    shtDlg->getAccessions(SeedAccs);
//    if (DataTools::loadSeed(SeedAccs, SeedFile)==false)
//        return;

//    this->setWindowTitle(HitTableFile + "- Select Target and Non-target Sequences");
//    DataTools::SaveQueryList();

//    //Get alignment user settings
//    SeqAlignSettingDlg saDlg(this);

//    if(!saDlg.exec()){
//        return;
//    }

//    //Parse Hit Table, add to sList
//    if( HitTableFile != "" ){
//        if( DataTools::ParseHitTable(HitTableFile) == false ){
//            return;
//        }
//    }

//    bool res = SeqReportWindow::needsComplement(HitTableFile);



    /////////////////////////////////////////////////////////////////////

    nMarks = 1;
    QString FASTAContent;
    QString qs = QString::null;
    emit report_model->layoutAboutToBeChanged();
    for(int i = 0; i < sList.size(); i++ ){
        if( sList[i]->marked ){
            pDlg.setValue(nMarks);
            QString seqStr;
            tools::seqInt2Str(sList[i]->seqInt, sList[i]->length, seqStr);
//             if (DataTools::isrev) {
//                std::string DNAseq = seqStr.toLocal8Bit().constData();

//                reverse(DNAseq.begin(), DNAseq.end());
//                for (std::size_t i = 0; i < DNAseq.length(); ++i){
//                    switch (DNAseq[i]){
//                    case 'A':
//                        DNAseq[i] = 'T';
//                        break;
//                    case 'C':
//                        DNAseq[i] = 'G';
//                        break;
//                    case 'G':
//                        DNAseq[i] = 'C';
//                        break;
//                    case 'T':
//                        DNAseq[i] = 'A';
//                        break;
//                    }

//                }
//                qs = QString::fromLocal8Bit(DNAseq.c_str());
//           }
            FASTAContent += "original sequence";
            FASTAContent += seqStr;
            FASTAContent += '\n';
            std::string DNAseq = seqStr.toStdString().c_str();
                reverse(DNAseq.begin(), DNAseq.end());

                for (std::size_t i = 0; i < DNAseq.length(); ++i){
                    switch (DNAseq[i]){
                    case 'A':
                        DNAseq[i] = 'T';
                        break;
                    case 'C':
                        DNAseq[i] = 'G';
                        break;
                    case 'G':
                        DNAseq[i] = 'C';
                        break;
                    case 'T':
                        DNAseq[i] = 'A';
                        break;
                    }
                }
            qs = QString::fromStdString(DNAseq.c_str());
            FASTAContent += '>' + sList[i]->accession + ' ' + this->removeDuplicateSpaces(sList[i]->definition) + '\n';
            FASTAContent += qs +='\n';
            struct SequenceList* seqPtr = sList[i];
            delete seqPtr->seqInt;
            delete seqPtr;
            sList.erase(sList.begin() + i );
            i--;
        }
        nMarks++;
    }
    emit report_model->layoutChanged();

    //FASTAContent = FASTAContent.left(FASTAContent.length()-1);
    ui->textEdit->insertPlainText(FASTAContent);
    QMessageBox::about(this, "Info", str);
}

//bool SeqReportWindow::needsComplement(const QString &filename)
//{
//    bool res = false;
//    //open Hit Table File.
//    QFile file(filename);
//    if (!file.open(QFile::ReadOnly | QFile::Text))
//    {
//        QMessageBox::about(Q_NULLPTR, "File Error", "File doesn't exist or File corrupted");
//        return false;
//    }

//    // check file type
//    QTextStream stream ( &file );
//    QString line, sName;
//    line = stream.readLine();
//    if( stream.atEnd()  || ((line.indexOf("# BLASTN") < 0) && (line.indexOf("# blastn") < 0)) ){
//        printf("not hit table. First line: %s\n", line.toStdString().c_str());
//        QMessageBox::about(Q_NULLPTR, "File Error", filename + " is not a \"hit table\" file!");
//        return false;
//    }

//    //int index;
//    //Start to read file
//     while( !stream.atEnd() ) {
//         line = stream.readLine();
//         if( line == ""){
//             continue;
//         }
//         // skip hit table header lines
//         if( line.at(0)== '#' ){
//             continue;
//         }

//         struct SequenceList* seqPtr =  new SequenceList();
//         for( int i = 0; i < qList.size(); i++ ){
//             seqPtr->identity.push_back(0);
//         }

//         //TODO: Daniel, This is not as well optimized, come back to this later....
//         QStringList tokens = line.split(QRegExp("\\s+"));

//         //seqPtr->gi		= GetGIFromHitTable(line);
//         //seqPtr->accession	= GetAccessionFromHitTable(line);//tokens[1].left(tokens[1].indexOf("."));
//         qDebug() << "Accession " << seqPtr->accession.toStdString().c_str() << "\n";
//         //seqPtr->seqName	= GetSeqNameFromHitTable(line);//tokens[0];
//         //qDebug() << seqPtr->accession << seqPtr->seqName;
//         // initialize other stuff
//         seqPtr->marked	= 0;
//         seqPtr->length		= 0;
//         seqPtr->definition	= "";
//         seqPtr->organism	= "";
//         seqPtr->authors	= "";
//         seqPtr->title		= "";
//         seqPtr->seqInt		= Q_NULLPTR;
//         seqPtr->reverseComplement = false;


//         //alignment length
//         //int length  = tokens[3].toInt();

//         // query start/end, subject start/end
//         int q_start = tokens[6].toInt();
//         //int q_end   = tokens[7].toInt();
//         int s_start = tokens[8].toInt();
//         int s_end   = tokens[9].toInt();

//         if (s_start > s_end) {
//             qDebug() << "Sequence " << seqPtr->accession << " is reversed" << "\n";
//             seqPtr->reverseComplement = true;
//             swap(s_start, s_end);
//         }

//         res = seqPtr->reverseComplement;

//         // the final start and end values are calculated based on qstart/end
//         // and sstart/end
//         //according to Chrobak.
//         seqPtr->start = s_start - q_start + 1;
//         //seqPtr->end = s_end + qAbs(q_end - q_start) - q_end;
//         // We need to set the end only when we know the seed lengths
//         // seqPtr->end   = seqPtr->start + length - 1;

//         sList.push_back(seqPtr);
//     }
////     file.close();
//     return res;
//}

QString SeqReportWindow::removeDuplicateSpaces(QString description) {
    QString ret = "";
    bool wasSpace = false;
    for (QChar c: description) {
        if (c == ' ') {
            if (!wasSpace) {
                ret.append(c);
            }
            wasSpace = true;
        } else {
            wasSpace = false;
            ret.append(c);
        }
    }
    return ret;
}

void SeqReportWindow::on_actionChange_Sequence_Alignment_Setting_triggered()
{
    //Get alignment user settings
    SeqAlignSettingDlg saDlg(this);
    saDlg.exec();
}

void SeqReportWindow::on_actionUpdate_Identity_for_all_Sequences_triggered()
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
        QMessageBox::about(this, "Alignment Error", "Updating % identity was not finished.\n"
                           "The displayed information might be incomplete and inaccurate.\n");
        return;
    }

    if (accTooLongSeqToAlign.size()>0){
        QString message =  "%1 sequence(s) with the following accession exceed the maximal size for pairwise alignment:\n\n";
        for(int i = 0; i<accTooLongSeqToAlign.size(); i++){
            message += QString("%1\n").arg(accTooLongSeqToAlign[i]);
        }
        MessageDlg MDlg("Sequence Downloading Report",message);
        MDlg.exec();
    }
}

void SeqReportWindow::on_actionDelete_Marked_Sequences_triggered()
{
    if(sList.size() == 0)   return;

    int nMarks = 0;
    for( int i = 0; i < sList.size(); i++ ){
        if(sList[i]->marked) nMarks++;
    }

    if( nMarks == 0 ){
        QMessageBox::about(this, "Empty selection", "No sequence is marked!");
        return;
    }

    MyConfirmDlg CDlg("Are you sure you want to delete sequence(s)?",this);
    if(!CDlg.exec()){
        return;
    }

    emit report_model->layoutAboutToBeChanged();
    QString str = QString ("%1 marked sequences were deleted.").arg(nMarks);
    // delete from sequence table
    for( int i = sList.size()-1; i >= 0; i -- ){
        if( sList[i]->marked ){
            struct SequenceList* seqPtr = sList[i];
            delete seqPtr->seqInt;
            delete seqPtr;
            sList.erase(sList.begin() + i );
        }
    }
    emit report_model->layoutChanged();
    QMessageBox::about(this, "Info", str);
}

void SeqReportWindow::on_actionDelete_Selected_Sequences_triggered()
{
    if(sList.size() == 0)
        return;

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    //selected sequence indices:
    QVector<int> selectedSeqList;
    int nSelCnt = selectedRows.size();

    if(nSelCnt == 0){
        QMessageBox::about(this, "No selection", "No sequence selected!");
        return;
    }

    //track selected sequences
    for(int i = 0; i<selectedRows.length(); i++){
        int row = selectedRows.at(i).row();
        selectedSeqList.push_back(row);
    }

    MyConfirmDlg CDlg("re you sure you want to delete sequence(s)?",this);
    if(!CDlg.exec()){
        return;
    }
    emit report_model->layoutAboutToBeChanged();
    for( int i = selectedSeqList.size()-1; i >= 0; i -- ){
        int idx = selectedSeqList[i];
        struct SequenceList* seqPtr = sList[idx];
        delete seqPtr->seqInt;
        delete seqPtr;
        sList.erase(sList.begin() + idx);
    }
    emit report_model->layoutChanged();
    QMessageBox::about(this, "Info", QString ("%1 selected sequences were deleted.").arg(nSelCnt));
}

void SeqReportWindow::on_actionClear_FASTA_Sequence_Box_triggered()
{
    MyConfirmDlg cDlg("Are you sure you want to clear FASTA sequences?",this);
    if(!cDlg.exec()){
        return;
    }

    ui->textEdit->clear();
}

void SeqReportWindow::on_actionFind_Sequence_triggered()
{
    if(sList.size() == 0)
        return;

    SeqFindDlg sfDlg;
    if(sfDlg.exec()){
        on_actionFind_Next_triggered();
    }
}

void SeqReportWindow::on_actionFind_Next_triggered()
{
    if( sList.size() == 0 )
            return;

    // Find the first selected sequence
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    int CurSel;
    if( selectedRows.size() <= 0 )
            CurSel = -1;
    else
            CurSel = selectedRows.at(0).row();
    // start from the next sequence of the selected sequence
    CurSel++;

    // deselect all items
    ui->tableView->selectionModel()->clearSelection();

    // prepare for selected item list
    bool *selList = new bool[static_cast<unsigned long>(sList.size())];
    for( int i = 0; i < CurSel; i++ )
            selList[i] = false;
    for( int i = CurSel; i < sList.size(); i++ )
            selList[i] = true;

    if(findConditionalCheck[0]){
        for(int i = 0; i < sList.size(); i++){
            if (sList[i]->length < findLenMin || sList[i]->length > findLenMax){
                selList[i] = false;
            }
        }
    }
    if(findConditionalCheck[1]){
        for(int i = 0; i <sList.size(); i++){
            if (sList[i]->identity[queryId] < findIdMin || sList[i]->identity[queryId] > findIdMax){
                 selList[i] = false;
            }
        }
    }
    if(findConditionalCheck[2]){
        for(int i = 0; i < sList.size(); i++){
            QString str  = sList[i]->definition;
            str = str.toLower();
            if(contains(str, findDefConstraint) != findContain[0] )
                selList[i] = false;
        }
    }
    if(findConditionalCheck[3]){
        for(int i = 0; i < sList.size(); i++){
            QString str  = sList[i]->organism;
            str = str.toLower();
            if(contains(str, findOrgConstraint) != findContain[1] )
                selList[i] = false;
        }
    }
    QString str;
    if(findConditionalCheck[4]){
        for(int i = 0; i < sList.size(); i++){
            switch(findConditionalCheck[4]){
            case 1:
               str  = sList[i]->source; break;
            case 2:
                str = sList[i]->features; break;
            case 3:
                str = sList[i]->authors; break;
            case 4:
                str = sList[i]->title; break;
            default: break;
            }
            str = str.toLower();
            if(contains(str, findExtnStr1) != findContain[2] )
                selList[i] = false;
        }
    }

    if(findConditionalCheck[5]){
        for(int i = 0; i < sList.size(); i++){
            switch(findConditionalCheck[5]){
            case 1:
               str  = sList[i]->source; break;
            case 2:
                str = sList[i]->features; break;
            case 3:
                str = sList[i]->authors; break;
            case 4:
                str = sList[i]->title; break;
            default: break;
            }
            str = str.toLower();
            if(contains(str, findExtnStr2) != findContain[3] )
                selList[i] = false;
        }
    }

    int next = 0;
    while( next < sList.size() ){
        if( selList[next] ) break;
        next++;
    }
    if( next < sList.size() ){
        // highlight the selected item
        ui->tableView->selectRow(next);
    }
    else{
        QMessageBox::about(this, "Last one", "No more sequence found with the specified conditions!");
        if( CurSel > 0 )
            ui->tableView->selectRow(CurSel);
    }
    delete[] selList;

}

void SeqReportWindow::on_actionLoad_GenBank_gb_File_to_Be_compared_triggered()
{
    QFileDialog file_dialog(this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );

    QString filename = file_dialog.getOpenFileName( this, tr( "Open Seed File" ), pwd, tr( "Sequence file (*.gb);;All File (*.*)" )) ;
    if(filename.isEmpty()) return;

    for( int i = 0; i < compList.size(); i++ ){
            delete[] compList[i]->seqInt;
            delete compList[i];
    }
    compList.clear();
    if( !DataTools::readSeqFile(filename)) return;
    DataTools::compareSeq();
    // clear useless memory
    for( int k = 0; k < compList.size(); k++ ){
            delete[] compList[k]->seqInt;
            delete compList[k];
    }
    compList.clear();
}

void SeqReportWindow::on_actionDisplay_Sequences_not_in_Sequence_List_triggered()
{
    //TODO: (Daniel) is this a memory leak????
    SeqDiffWindow* sdWin = new SeqDiffWindow(notInSeqList, this);
    sdWin->setWindowTitle("Sequences not in Sequence List");
    sdWin->show();
    sdWin->initialize();
}

void SeqReportWindow::on_actionDisplay_Sequences_not_in_GenBank_File_triggered()
{
    //TODO: (Daniel) is this a memory leak????
    SeqDiffWindow* sdWin = new SeqDiffWindow(notInFileList, this);
    sdWin->setWindowTitle("Sequences not in GenBank File");
    sdWin->show();
    sdWin->initialize();
}

void SeqReportWindow::on_actionExit_triggered()
{
    close();
}

void SeqReportWindow::insertShownSeqList()
{
    MyProgressDlg pDlg("Adding primers to primer table",0, sList.size(), this);
    pDlg.setWindowModality(Qt::ApplicationModal);
    pDlg.show();

    // add all primer pairs in result list one by one
    emptyModel();

    // add all primer pairs in result list one by one
    for(int i = 0; i < sList.size(); i++ ){
        pDlg.setValue(static_cast<int>(i+1));
        OnInsertData(nullptr);//displays the inserted data on the screen
    }
}

bool SeqReportWindow::checkOrdeOK()
{
    return true; //(Daniel) because of the way I designed it, this always is true, and this function is unnessary
}


void SeqReportWindow::on_actionPRISE_Manual_triggered()
{
    QString url = "";
#ifdef MAC
    QString appPath = QDir::currentPath();
    //url += "file:/";
    url += appPath;
    url += "/";
#endif
#ifdef WIN
    url += "instructions\\PRISE2_Manual.pdf";
#else
    url += ":/instructions/PRISE2_Manual.pdf";
#endif


    QFile helpFile(url);
    if(!helpFile.exists()){
        QMessageBox::warning(this, "Help file is missing", QString("Cannot find the help file below:\n%1").arg(url));
    }else{
#ifdef MAC
        url = "file:/" + url;
#endif
        QDesktopServices::openUrl(QUrl::fromLocalFile(url));
    }
}

void SeqReportWindow::on_actionPRISE_Tuturial_triggered()
{
//    QString url = "";
//#ifdef MAC
//    QString appPath = QDir::currentPath();
//    url += "file:/";
//    url += appPath;
//    url += "/";
//#endif

//#ifdef WIN
//    url += "instructions\\PRISE_Tutorial.pdf";
//#else
//    url += "instructions/PRISE_Tutorial.pdf";
//#endif
//    QDesktopServices::openUrl(QUrl(url));

    QString url = "";
#ifdef MAC
    QString appPath = QDir::currentPath();
    //url += "file:/";
    url += appPath;
    url += "/";
#endif

#ifdef WIN
    url += "instructions\\PRISE2_Tutorial.pdf";
#else
    url += "instructions/PRISE2_Tutorial.pdf";
#endif


    QFile helpFile(url);
    if(!helpFile.exists()){
        QMessageBox::warning(this, "Help file is missing", QString("Cannot find the help file below:\n%1").arg(url));
    }else{

#ifdef MAC
        url = "file:/" + url;
#endif
        QDesktopServices::openUrl(QUrl::fromLocalFile(url));
    }

}

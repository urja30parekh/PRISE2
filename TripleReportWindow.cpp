#include <algorithm>
#include "TripleReportWindow.h"
#include "ui_TripleReportWindow.h"
#include "sorting.h" //sorting function for data.
#include "QCloseEvent"

//TripleReportWindow* tmp3;
/*void onDisplayAllProbeColumn()
{
    tmp3->displayAllColumn();
}

void onLoadProbeData()
{
    tmp3->displayData();
}

void onInsertProbe(int idx)
{
    tmp3->insertData(idx);
}*/

TripleReportWindow::TripleReportWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TripleReportWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Primer-Probe Set Report Window");
}

TripleReportWindow::~TripleReportWindow()
{
    delete ui;

    /*Clean Memory: (for Error)
      4. probe Candidates
      5. probes
      6. Triples (in Primer Pair)
      8. tripleShowList
      9. T&P
      10. Score
    */
    //this->tripleShowList.clear();
    //DataTools::cleanMemoryTriple();
    //DataTools::cleanMemoryProbeData();
}

//Set TableView headers and fields.


void TripleReportWindow::closeEvent(QCloseEvent *event)
{
    QTabWidget* tmp = ui->tabWidget;
    int numTabs = tmp->count();
    for(int i = 0; i < numTabs; i++){
        closeTab(0);
    }
    //DataTools::cleanMemoryTriple();
    DataTools::cleanMemoryProbeData();
    event->accept();


    /*Clean Memory: (for Error)
      4. probe Candidates
      5. probes
      6. Triples (in Primer Pair)
      8. tripleShowList
      9. T&P
      10. Score
    */
    /*this->tripleShowList.clear();
    DataTools::cleanMemoryTriple(pr);
    QMainWindow::closeEvent(event);
    this->~TripleReportWindow();*/
}


/*bool TripleReportWindow::close()
{
    QTabWidget* tmp = ui->tabWidget;
    int numTabs = tmp->count();
    for(int i = 0; i < numTabs; i++){
        closeTab(0);
    }
    DataTools::cleanMemoryTriple();
    DataTools::cleanMemoryProbeData();

    QMainWindow::close();
}*/

void TripleReportWindow::addTab(QWidget *widget, QString tabLabel)
{
    ui->tabWidget->addTab(widget, tabLabel);
}

void TripleReportWindow::closeTab(int tabIdx)
{
    TripleReportFrame *tpfm = (TripleReportFrame*)ui->tabWidget->widget(tabIdx);
    this->ui->tabWidget->removeTab(tabIdx);
    delete tpfm;
}

/*void TripleReportWindow::on_actionLoad_Triple_List_Triple_File_triggered()
{
    QFileDialog file_dialog(this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getOpenFileName( this, tr( "Load triple list file" ), "./", tr( "Triple List File (*.ppset);;All File (*.*)" )) ;
    if(filename.isEmpty()){
        return;
    }

    TripleReportFrame *tpfm = new TripleReportFrame(0, this);
    tpfm->loadTripleList(filename);
    addTab(tpfm, tpfm->getPrimerSeq());
}*/

void TripleReportWindow::on_actionSave_Triple_List_Triple_File_triggered()
{
    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());

    if(tpfm->getTripleListSize() == 0 || tpfm->getTripleShowListSize() == 0){
        QMessageBox::about(this, "Empty List", "Nothing to be saved");
        return;
    }

    TripleSaveListDlg* saveDlg = new TripleSaveListDlg(tpfm);
    if(!saveDlg->exec())
        return;

    QFileDialog file_dialog(this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getSaveFileName(this, tr( "Save Probe List to .Triple File" ), pwd, tr( "Probe List File (*.ppset);;All File (*.*)"));
    if(filename.isEmpty()){
        return;
    }

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Save triple list.\n";
    DataTools::writeErrorLog(errorlog);

    tpfm->saveTripleList(filename);
}

void TripleReportWindow::on_actionSave_Triple_List_as_Tab_Delimited_File_Excel_triggered()
{
    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());

    if(tpfm->getTripleListSize() == 0 || tpfm->getTripleShowListSize() == 0){
        QMessageBox::about(this, "Empty List", "Nothing to be saved");
        return;
    }

    TripleSaveExcelDlg* saveDlg = new TripleSaveExcelDlg(tpfm);
    if(!saveDlg->exec())
        return;

    QFileDialog file_dialog(this);
    file_dialog.setFileMode(QFileDialog::ExistingFile );
    QString filename = file_dialog.getSaveFileName(this, tr( "Save Probe List to Excel File" ), pwd, tr( "Text File (*.txt);;All File (*.*)"));
    if(filename.isEmpty()){
        return;
    }

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Save triple list plan text\n";
    DataTools::writeErrorLog(errorlog);

    tpfm->saveTripleList2TXT(filename);
}

void TripleReportWindow::on_actionSave_Triple_Information_Window_Content_triggered()
{
    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());

    QString tripleInfo = tpfm->getInfoText();
    if(tripleInfo.length() <= 0){
        QMessageBox::about(this, "Empty List", "Triple Information Window is empty.\nNothing to be saved!");
        return;
    }

    QFileDialog file_dialog(this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getSaveFileName(this, tr( "Save Information Window content" ), pwd, tr( "Text File (*.txt);;All File (*.*)"));
    if(filename.isEmpty()){
        return;
    }

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Save triple info window\n";
    DataTools::writeErrorLog(errorlog);

    DataTools::saveText2File(filename, tripleInfo);
}


void TripleReportWindow::on_actionSave_Triples_Only_triggered()
{
    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());

    if( tpfm->getTripleListSize() == 0){
        QMessageBox::about(this, "Empty List", "Nothing to be saved");
        return;
    }

    TripleSaveListDlg* saveDlg = new TripleSaveListDlg(this);
    if(!saveDlg->exec())
        return;

    QFileDialog file_dialog(this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getSaveFileName( this, tr( "Save Triple to Text" ), pwd, tr( "Text File (*.txt);;All File (*)"));
    if(filename.isEmpty()){
        return;
    }

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Save triple only.\n";
    DataTools::writeErrorLog(errorlog);

    tpfm->saveTriples(filename);
}

void TripleReportWindow::on_actionExit_triggered()
{

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Close\n";
    DataTools::writeErrorLog(errorlog);

    this->close();
}

void TripleReportWindow::on_actionDisplay_all_columns_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Triple show all columns.\n";
    DataTools::writeErrorLog(errorlog);

    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->displayAllColumn();
}

void TripleReportWindow::on_actionHide_Display_columns_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Triple hide\\display columns.\n";
    DataTools::writeErrorLog(errorlog);

    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->hideDispColumns();
}

void TripleReportWindow::on_actionHide_Display_triples_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Triple hide\\display triples.\n";
    DataTools::writeErrorLog(errorlog);

    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->hideDispTriples();
}

void TripleReportWindow::on_actionSort_Triple_List_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Triple sort triple list.\n";
    DataTools::writeErrorLog(errorlog);

    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->sortTripleList();

}

void TripleReportWindow::on_actionAdd_Triple_Manually_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Triple add probe manually.\n";
    DataTools::writeErrorLog(errorlog);

    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->addTripleManually();
}

void TripleReportWindow::on_actionDelete_Triples_Conditionally_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Triple delete triple conditionally.\n";
    DataTools::writeErrorLog(errorlog);

    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->deleteTriplesConditionally();
/*    if( tripleShowList.size() == 0 )
            return;

    TripleDeleteCondDlg delDlg(this);
    if(!delDlg.exec())
        return;

    MyConfirmDlg cDlg("Are you sure you want to delete sequence(s)?\n\n\n\n",this);
    if(!cDlg.exec())
        return;


    int range = pr->tripleList.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Clean triple list memory...",0, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);


    emptyModel();
    ui->textEdit->clear();

    Triple *tp;
    bool passChecking;
    int numDel = 0;
    // check for each probe
    for( unsigned int i = 0, j = 0; i < pr->tripleList.size(); i++, j++ ){
        tp = pr->tripleList[i];

        passChecking = checkTripleWithCondition( tp, deleteTripleCondTitle, deleteTripleCondGtLt, deleteTripleCondValue );

        if( passChecking){	// pass all the checks, remove it from probeList memory
            tp->pctgInTargetSeqs = -1; // mark it for deletion
            tp->pctgInNonTarSeqs = -1;
            pr->tripleList.erase( pr->tripleList.begin()+i );
            i--;
            numDel++;
        }
        pDlg->setValue(j+1);
    }
    delete pDlg;

    range = tripleShowList.size();
    pDlg = new MyProgressDlg("Adding triples to table...",0, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);

    // check for each probe in displaying probe list
    for( unsigned int i = 0, j = 0; i < tripleShowList.size(); i++, j++ ){
        tp = tripleShowList[i];
        // previously marked for deletion
        if( tp->pctgInTargetSeqs == -1 &&  tp->pctgInNonTarSeqs == -1 ){
            delete tp;
            tripleShowList.erase(tripleShowList.begin()+i );
            i--;
        }
        else
            this->insertData(i);

        pDlg->setValue(j+1);
    }
    delete pDlg;

    QString str = QString("%1 triples were deleted.").arg(numDel);
    QMessageBox::about(this,"",str);*/
}

void TripleReportWindow::on_actionDelete_Marked_Triples_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Triple delete marked triples.\n";
    DataTools::writeErrorLog(errorlog);

    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->deleteMarkedTriples();
/*    if( tripleShowList.size() == 0 )
        return;

    int numDel = 0;

    this->getMarkedList();
    numDel = this->markedList.size();

    if( numDel == 0 ){
        QMessageBox::about(this, "Empty delete list", "No triple is marked.");
        return;
    }

    // to confirm delete
    MyConfirmDlg cDlg("Are you sure you want to delete sequence(s)?\n\n\n\n",this);
    if(!cDlg.exec())
        return;

    ui->textEdit->clear();

    int range;
    MyProgressDlg *pDlg = new MyProgressDlg("Clean probe list memory...",0, 1, this);
    pDlg->setWindowModality(Qt::ApplicationModal);

    // if marked triples are too much, delete them all, then add one by one
    if( numDel > 100 ){
        this->emptyModel();
        range = markedList.size()+ pr->tripleList.size();
        pDlg->setRange(0, range);

        // mark those triples for deletion and remove them in displayed list first
        int i = markedList.size() - 1, j, fin = 1;
        while( i >= 0 ){
            j = markedList[i--];
            tripleShowList[j]->pctgInTargetSeqs = -1;
            tripleShowList[j]->pctgInNonTarSeqs = -1;
            tripleShowList.erase(tripleShowList.begin() + j );
            pDlg->setValue(fin++);
        }

        // destroy the memory and consolidate in memory probe list
        for( i = pr->tripleList.size() - 1; i >= 0; i-- ){
            if( pr->tripleList[i]->pctgInTargetSeqs == -1 && pr->tripleList[i]->pctgInNonTarSeqs == -1 ){
                delete pr->tripleList[i];
                pr->tripleList.erase( pr->tripleList.begin() + i );
            }
            pDlg->setValue(fin++);
        }
        insertShownTripleList();
    }
    else{
        QString tripleMarked;
        int i = numDel-1;
        while( i >= 0 ){
            // retrieve the probe sequences
            //tripleMarked = tableModel->item(markedList[i], B_PRIMER_PAIR)->text();
            tripleMarked = tableModel->item(markedList[i], B_PROBE)->text();

            // delete the selected probe from probe table
            delete tableModel->item(markedList[i],0);
            tableModel->removeRow(markedList[i]);
            i--;

            // delete the selected probe from current triple memory
            if( DataTools::deleteTripleFromTripleMem( tripleMarked, tripleShowList, FALSE ) < 0 )
                return;

            // delete the probe from theApp memory and destroy the memory
            if( DataTools::deleteTripleFromTripleMem( tripleMarked, pr->tripleList, TRUE ) < 0 )
                return;
        }
    }
    delete pDlg;

    QString str = QString("%1 marked triples were deleted.").arg(numDel);
    QMessageBox::about(this,"",str);*/
}

void TripleReportWindow::on_actionDelete_Seleted_Triples_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Triple delete selected triples.\n";
    DataTools::writeErrorLog(errorlog);

    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->deleteSeletedTriples();
 /*   if( tripleShowList.size() == 0 )
            return;

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();

    if( selectedRows.size() <= 0 ){
        QMessageBox::about(this, "Empty delete list", "No probe is selected.");
        return;
    }

    // to confirm delete
    MyConfirmDlg cDlg("Are you sure you want to delete sequence(s)?\n\n\n\n",this);
    if(!cDlg.exec())
        return;

    ui->textEdit->clear();

    int range;
    MyProgressDlg *pDlg = new MyProgressDlg("Clean probe list memory...",0, 1, this);
    pDlg->setWindowModality(Qt::ApplicationModal);


    // track selected triples
    int nSelCount = selectedRows.size();

    // if selected triples are too much, delete them all, then add one by one
    if( nSelCount > 100 ){
        this->emptyModel();
        range = nSelCount+ pr->tripleList.size();
        pDlg->setRange(0, range);

        // mark those triples for deletion and remove them in displayed list first
        int i = nSelCount - 1, j, fin = 1;
        while( i >= 0 ){
            j = selectedRows.at(i--).row();
            tripleShowList[j]->pctgInTargetSeqs = -1;
            tripleShowList[j]->pctgInNonTarSeqs = -1;
            tripleShowList.erase( tripleShowList.begin() + j );

            pDlg->setValue(fin++);
        }

        // destroy the memory and consolidate in memory triple list
        for( i = pr->tripleList.size() - 1; i >= 0; i-- ){
            if( pr->tripleList[i]->pctgInTargetSeqs == -1 && pr->tripleList[i]->pctgInNonTarSeqs == -1 ){
                delete pr->tripleList[i];
                pr->tripleList.erase( pr->tripleList.begin() + i );
            }
            pDlg->setValue(fin++);
        }
        this->insertShownTripleList();
    }
    else{

        QString tripleSelected;
        int i = selectedRows.size() - 1;
        while( i >= 0 ){

            int rowIdx = selectedRows.at(i).row();
            // retrieve the triple sequences
            tripleSelected = tableModel->item( rowIdx, B_PROBE)->text();

            // delete the selected triple from triple table
            delete tableModel->item(rowIdx,0);
            tableModel->removeRow(rowIdx);
            i--;

            // delete the selected triple from current triple memory
            if( DataTools::deleteTripleFromTripleMem( tripleSelected, tripleShowList, FALSE ) < 0 )
                    return;

            // delete the triple from theApp memory and destroy the memory
            if( DataTools::deleteTripleFromTripleMem( tripleSelected, pr->tripleList, TRUE ) < 0 )
                    return;
        }
    }
    delete pDlg;

    QString str = QString("%1 selected triples were deleted.").arg(nSelCount);
    QMessageBox::about(this,"",str);*/
}

void TripleReportWindow::on_actionMark_Selected_Triples_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Triple mark selected triples.\n";
    DataTools::writeErrorLog(errorlog);

    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->markSelectedTriples();
}

void TripleReportWindow::on_actionUnmark_Selected_Triples_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Triple unmark selected triples.\n";
    DataTools::writeErrorLog(errorlog);

    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->unmarkSelectedTriples();
}

void TripleReportWindow::on_actionTriple_Annealing_Position_Information_triggered()
{

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Triple show annealing position info.\n";
    DataTools::writeErrorLog(errorlog);

    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->dispCompleteInfo();
}

void TripleReportWindow::on_actionPercentage_of_Each_Nucleotide_in_Target_and_Non_Target_Sequences_in_Relation_to_Triple_Sequences_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Triple percentage in tar/non of each nucleotide.\n";
    DataTools::writeErrorLog(errorlog);

    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->reportBindingContent();
}

void TripleReportWindow::on_actionTarget_Sequences_Annealing_with_Triple_triggered()
{

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Triple target sequences covered by triple.\n";
    DataTools::writeErrorLog(errorlog);

    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->targetWTriple();
}

void TripleReportWindow::on_actionTarget_Sequences_Not_Annealing_with_Triple_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Triple target sequences not covered by triple.\n";
    DataTools::writeErrorLog(errorlog);

    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->targetWoTriple();
}

void TripleReportWindow::on_actionNon_target_Sequences_Annealing_with_Triple_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Triple nontarget sequences covered by triple.\n";
    DataTools::writeErrorLog(errorlog);

    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->nonTargetWTriple();
}

void TripleReportWindow::on_actionNon_target_Sequences_Not_Annealing_with_Triple_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Triple nontarget sequences not covered by triple.\n";
    DataTools::writeErrorLog(errorlog);

    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->nonTargetWoTriple();
}

/*void TripleReportWindow::on_actionUpdate_Estimated_Information_to_Exact_Information_triggered()
{

}*/

void TripleReportWindow::on_actionProbe_Intra_Complementarity_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Probe Intra Comp.\n";
    DataTools::writeErrorLog(errorlog);

    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->probeIntraComp();
}

/*void TripleReportWindow::on_actionProbe_3_Intra_Complementarity_triggered()
{

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Probe 3' Intra Comp.\n";
    DataTools::writeErrorLog(errorlog);


    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->probe3IntraComp();
}*/

void TripleReportWindow::on_actionTriple_Inter_Complementarity_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Triple Inter Comp.\n";
    DataTools::writeErrorLog(errorlog);

    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->tripleInterComp();
}

/*
void TripleReportWindow::on_actionTriple_3_Inter_Complementarity_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Triple 3' Inter Comp.\n";
    DataTools::writeErrorLog(errorlog);

    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->triple3InterComp();
}*/

void TripleReportWindow::on_actionShow_PrimerPair_Info_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Primer pair info.\n";
    DataTools::writeErrorLog(errorlog);

    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->showPrimerPairInfo();
}

void TripleReportWindow::on_actionView_Probe_Design_Setting_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Probe design settings.\n";
    DataTools::writeErrorLog(errorlog);

    ProbeSettingReportDlg *psReportDlg = new ProbeSettingReportDlg(false, this);
    psReportDlg->exec();
    delete psReportDlg;
}

void TripleReportWindow::on_actionBlast_Probe_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Blast.\n";
    DataTools::writeErrorLog(errorlog);

    TripleReportFrame *tpfm = static_cast<TripleReportFrame*>(ui->tabWidget->currentWidget());
    tpfm->blastProbe();
}

void TripleReportWindow::on_actionPRISE_Manual_triggered()
{
//    QString url = "";
//#ifdef MAC
//    QString appPath = QDir::currentPath();
//    url += "file:/";
//    url += appPath;
//    url += "/";
//#endif

//#ifdef WIN
//    url += "instructions\\PRISE_Manual.pdf";
//#else
//    url += "instructions/PRISE_Manual.pdf";
//#endif


//    QDesktopServices::openUrl(QUrl(url));

    //InstructionDlg iDlg;
    //iDlg.exec();

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
    url += "instructions/PRISE2_Manual.pdf";
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

void TripleReportWindow::on_actionPRISE_Tutorial_triggered()
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
/*
void TripleReportWindow::click(const QModelIndex & idx)
{
    int row = idx.row();
    int column = idx.column();

    if(column == 0) {
        switch(tableModel->item(row, column)->checkState()){
        case Qt::Checked:
            tripleShowList[row]->marked = true;
            tableModel->setData(tableModel->index(row, 0), " ");
            break;
        case Qt::Unchecked:
            tripleShowList[row]->marked = false;
            tableModel->setData(tableModel->index(row, 0), "");
            break;
        default:
            QMessageBox::about(this, "Error", "Partially Checked is not valid."
                               "Please contact the developer");
            break;
        }
    }
}
void TripleReportWindow::rightClick (const QPoint & point)
{
    const QModelIndex idx = ui->tableView->indexAt(point);
    dispTripleSel = idx.row();
    showRightClickMenu();
}

void TripleReportWindow::showRightClickMenu ()
{
  QMenu *popupMenu;
  QAction *actionTripleAnnealPosInfo;
  QAction *actionPercentage;
  QAction *actionTargetWTriple;
  QAction *actionTargetWoTriple;
  QAction *actionNonTargetWTriple;
  QAction *actionNonTargetWoTriple;
  QAction *actionGetExactPCRinfo;
  QAction *actionTripleInterComp;
  QAction *actionTriple3InterComp;
  QAction *actionProbeIntraComp;
  QAction *actionProbe3IntraComp;
  //QAction *actionProbeIntraComp;
  //QAction *actionProbe3IntraComp;
  //QAction *actionBlastForwardPrimer;
  //QAction *actionBlastReversePrimer;
  QAction *actionBlastProbe;
  QAction *actionDeleteTriple;

  popupMenu = new QMenu(ui->tableView);
  actionTripleAnnealPosInfo = new QAction(ui->tableView);
  actionTripleAnnealPosInfo->setObjectName(QString::fromUtf8("Triple Annealing Position Information"));
  actionTripleAnnealPosInfo->setText(QString::fromUtf8("Triple Annealing Position Information"));

  actionPercentage = new QAction(ui->tableView);
  actionPercentage->setObjectName(QString::fromUtf8("Percentage of Each Nucleotide in Target and Non-Target Sequences in Relation to Triple Sequences"));
  actionPercentage->setText(QString::fromUtf8("Percentage of Each Nucleotide in Target and Non-Target Sequences in Relation to Triple Sequences"));

  actionTargetWTriple = new QAction(ui->tableView);
  actionTargetWTriple->setObjectName(QString::fromUtf8("Target Sequences Annealing with Triple"));
  actionTargetWTriple->setText(QString::fromUtf8("Target Sequences Annealing with Triple"));

  actionTargetWoTriple = new QAction(ui->tableView);
  actionTargetWoTriple->setObjectName(QString::fromUtf8("Target Sequences Not Annealing with Triple"));
  actionTargetWoTriple->setText(QString::fromUtf8("Target Sequences Not Annealing with Triple"));

  actionNonTargetWTriple = new QAction(ui->tableView);
  actionNonTargetWTriple->setObjectName(QString::fromUtf8("Non-Target Sequences Annealing with Triple"));
  actionNonTargetWTriple->setText(QString::fromUtf8("Non-Target Sequences Annealing with Triple"));

  actionNonTargetWoTriple = new QAction(ui->tableView);
  actionNonTargetWoTriple->setObjectName(QString::fromUtf8("Non-Target Sequences Not Annealing with Triple"));
  actionNonTargetWoTriple->setText(QString::fromUtf8("Non-Target Sequences Not Annealing with Triple"));

  actionGetExactPCRinfo = new QAction(ui->tableView);
  actionGetExactPCRinfo->setObjectName(QString::fromUtf8("Update Estimated Information to Exact Information"));
  actionGetExactPCRinfo->setText(QString::fromUtf8("Update Estimated Information to Exact Information"));

  actionTripleInterComp = new QAction(ui->tableView);
  actionTripleInterComp->setObjectName(QString::fromUtf8("Triple Inter-Complementarity"));
  actionTripleInterComp->setText(QString::fromUtf8("Triple Inter-Complementarity"));

  actionTriple3InterComp = new QAction(ui->tableView);
  actionTriple3InterComp->setObjectName(QString::fromUtf8("Triple 3' Inter-Complementarity"));
  actionTriple3InterComp->setText(QString::fromUtf8("Triple 3' Inter-Complementarity"));

  actionProbeIntraComp = new QAction(ui->tableView);
  actionProbeIntraComp->setObjectName(QString::fromUtf8("Probe Intra-Complementarity"));
  actionProbeIntraComp->setText(QString::fromUtf8("Probe Intra-Complementarity"));

  actionProbe3IntraComp = new QAction(ui->tableView);
  actionProbe3IntraComp->setObjectName(QString::fromUtf8("Probe 3' Intra-Complementarity"));
  actionProbe3IntraComp->setText(QString::fromUtf8("Probe 3' Intra-Complementarity"));



  //actionBlastForwardPrimer = new QAction(ui->tableView);
  actionBlastForwardPrimer->setObjectName(QString::fromUtf8("BLAST Forward Primer"));
  actionBlastForwardPrimer->setText(QString::fromUtf8("BLAST Forward Primer"));

  actionBlastReversePrimer = new QAction(ui->tableView);
  actionBlastReversePrimer->setObjectName(QString::fromUtf8("BLAST Reverse Primer"));
  actionBlastReversePrimer->setText(QString::fromUtf8("BLAST Reverse Primer"));//

  actionBlastProbe = new QAction(ui->tableView);
  actionBlastProbe->setObjectName(QString::fromUtf8("BLAST Probe"));
  actionBlastProbe->setText(QString::fromUtf8("BLAST Probe"));

  actionDeleteTriple = new QAction(ui->tableView);
  actionDeleteTriple->setObjectName(QString::fromUtf8("Delete"));
  actionDeleteTriple->setText(QString::fromUtf8("Delete"));

  popupMenu->addAction(actionTripleAnnealPosInfo);
  popupMenu->addAction(actionPercentage);
  popupMenu->addAction(actionTargetWTriple);
  popupMenu->addAction(actionTargetWoTriple);
  popupMenu->addAction(actionNonTargetWTriple);
  popupMenu->addAction(actionNonTargetWoTriple);
  popupMenu->addAction(actionGetExactPCRinfo);
  popupMenu->addAction(actionTripleInterComp);
  popupMenu->addAction(actionTriple3InterComp);
  popupMenu->addAction(actionProbeIntraComp);
  popupMenu->addAction(actionProbe3IntraComp);

  //popupMenu->addAction(actionBlastForwardPrimer);
  //popupMenu->addAction(actionBlastReversePrimer);
  popupMenu->addAction(actionBlastProbe);
  popupMenu->addAction(actionDeleteTriple);

  connect(actionTripleAnnealPosInfo, SIGNAL(triggered()), this, SLOT(dispCompleteInfo()));
  connect(actionPercentage, SIGNAL(triggered()), this, SLOT(reportBindingContent()));
  connect(actionTargetWTriple, SIGNAL(triggered()), this, SLOT(targetWTriple()));
  connect(actionTargetWoTriple, SIGNAL(triggered()), this, SLOT(targetWoTriple()));
  connect(actionNonTargetWTriple, SIGNAL(triggered()), this, SLOT(nonTargetWTriple()));
  connect(actionNonTargetWoTriple, SIGNAL(triggered()), this, SLOT(nonTargetWoTriple()));
  connect(actionGetExactPCRinfo, SIGNAL(triggered()), this, SLOT(getExactPCRinfo()));
  connect(actionTripleInterComp, SIGNAL(triggered()), this, SLOT(tripleInterComp()));
  connect(actionTriple3InterComp, SIGNAL(triggered()), this, SLOT(triple3InterComp()));
  connect(actionProbeIntraComp, SIGNAL(triggered()), this, SLOT(probeIntraComp()));
  connect(actionProbe3IntraComp, SIGNAL(triggered()), this, SLOT(probe3IntraComp()));
  //connect(actionProbeIntraComp, SIGNAL(triggered()), this, SLOT(probeIntraComp()));
  //connect(actionBlastForwardPrimer, SIGNAL(triggered()), this, SLOT(blastForwardPrimer()));
  //connect(actionBlastReversePrimer, SIGNAL(triggered()), this, SLOT(blastReversePrimer()));
  connect(actionBlastProbe, SIGNAL(triggered()), this, SLOT(blastProbe()));
  connect(actionDeleteTriple, SIGNAL(triggered()), this, SLOT(deleteTriple()));

  popupMenu->exec(QCursor::pos());
  delete popupMenu;
  dispTripleSel = -1;
}*/

/*
void TripleReportWindow::getExactPCRinfo()
{
    int range = tarSeqs.size() + nontarSeqs.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Updating primer pair annealing information...",1, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);

    ui->textEdit->clear();

    Triple* tp = tripleShowList[dispTripleSel];
    PrimerPair* pair = tp->Pr;

    // get primer sequences
        int pLen = pair->left.length();
        short *lprimer = new short[pLen+1];
        for( int i = 0; i < pLen; i++ )
            lprimer[i] = tools::base2int(pair->left[i]);
        lprimer[pLen] = -1;

        pLen = pair->right.length();
        short *rprimer = new short[pLen+1];
        for( int i = 0; i < pLen; i++ )
            rprimer[i] = tools::base2int(pair->right[i]);
        rprimer[pLen] = -1;


        PrimerSearch* lft;
        PrimerSearch* rht;
        try{
            lft = new PrimerSearch(lprimer, LEFT_PRIMER);
            rht = new PrimerSearch(rprimer, RIGHT_PRIMER);
            delete[] lprimer;	delete[] rprimer;
        }
        catch( std::bad_alloc& e ){
            QMessageBox::about(this,"Fatal error","Fatal error: out of memory for primer searching.");

            dispTripleSel = -1;
            delete pDlg;
            return;
        }


    int numTarSeqs = tarSeqs.size();
    int numNonTarSeqs = nontarSeqs.size();

    int num;
    int maxProductSize;
    int minProductSize;
    int sumProductSize;
    int productSize;
    int bdPos[4];


    // compute frequencies for this primer pair in target sequences
    num = 0;
    maxProductSize = 0;	minProductSize = INT_MAX;	sumProductSize = 0;
    for( int i = 0; i < numTarSeqs; i++ ){

        // search left and right primers in target sequence
        //lft->appxSearchTarSeqs(tarSeqs[i], LEFT_PRIMER, bdPos[0], bdPos[1]);
        //rht->appxSearchTarSeqs(tarSeqs[i], RIGHT_PRIMER, bdPos[2], bdPos[3]);

        bdPos[0] = pair->posBindingTar[0][i]-pair->left.length()+1;
        bdPos[1] = pair->posBindingTar[0][i];
        bdPos[2] = pair->posBindingTar[1][i];
        bdPos[3] = pair->posBindingTar[1][i]+ pair->right.length()-1;
        // left and right primer both appear and positions are correct
        if( bdPos[0] >= 0 && bdPos[2] >=0 && bdPos[1] < bdPos[2] ){
            num++;
            productSize = bdPos[3] - bdPos[0] + 1;
            if( minProductSize > productSize )	minProductSize = productSize;
            if( maxProductSize < productSize )	maxProductSize = productSize;
            sumProductSize += productSize;
        }
        pDlg->setValue(i);
    }
    if( numTarSeqs != 0 )
        pair->pctgInTargetSeqs = (float)num / numTarSeqs * 100;
    else
        pair->pctgInTargetSeqs = 0;
    if( num == 0 ){
        pair->pcrSizeAvgTarSeqs = 0;
        pair->pcrSizeMinTarSeqs = 0;
        pair->pcrSizeMaxTarSeqs = 0;
    }
    else{
        pair->pcrSizeAvgTarSeqs = (float)sumProductSize / num;
        pair->pcrSizeMinTarSeqs = minProductSize;
        pair->pcrSizeMaxTarSeqs = maxProductSize;
    }

    // compute frequencies for this primer pair in non-target sequences
    num = 0;
    maxProductSize = 0;	minProductSize = INT_MAX;	sumProductSize = 0;
    for( int i = 0; i < numNonTarSeqs; i++ ){
        // search left and right primers in non-target sequence
        //lft->appxSearchNonTarSeqs(nontarSeqs[i], LEFT_PRIMER, bdPos[0], bdPos[1]);
        //rht->appxSearchNonTarSeqs(nontarSeqs[i], RIGHT_PRIMER, bdPos[2], bdPos[3]);

        bdPos[0] = pair->posBindingTar[0][i]-pair->left.length()+1;
        bdPos[1] = pair->posBindingTar[0][i];
        bdPos[2] = pair->posBindingTar[1][i];
        bdPos[3] = pair->posBindingTar[1][i]+ pair->right.length()-1;
        // left and right primer both appear and positions are correct
        if( bdPos[0] >= 0 && bdPos[2] >=0 && bdPos[1] < bdPos[2] ){
            num++;
            productSize = bdPos[3] - bdPos[0] + 1;
            if( minProductSize > productSize )	minProductSize = productSize;
            if( maxProductSize < productSize )	maxProductSize = productSize;
            sumProductSize += productSize;
        }
        pDlg->setValue(numTarSeqs+i);
    }
    if( numNonTarSeqs != 0 )
        pair->pctgInNonTarSeqs = (float)num / numNonTarSeqs * 100;
    else
        pair->pctgInNonTarSeqs = 0;
    if( num == 0 ){
        pair->pcrSizeAvgNonTarSeqs = 0;
        pair->pcrSizeMinNonTarSeqs = 0;
        pair->pcrSizeMaxNonTarSeqs = 0;
    }
    else{
        pair->pcrSizeAvgNonTarSeqs = (float)sumProductSize / num;
        pair->pcrSizeMinNonTarSeqs = minProductSize;
        pair->pcrSizeMaxNonTarSeqs = maxProductSize;
    }

    pair->isEstimateTar = 0;
    pair->isEstimateNonTar = 0;


    //delete lft;
    //delete rht;

    // update frequency, average, minimum and maximum product size to exact value
    int nItem = dispTripleSel;
    //QString str;
    //str = QString("%1").arg(pair->pctgInTargetSeqs);
    tableModel->setData(tableModel->index(dispTripleSel,B_TARGET_PCTG), pair->pctgInTargetSeqs);
    //str = QString("%1").arg(pair->pcrSizeAvgTarSeqs);
    tableModel->setData(tableModel->index(dispTripleSel,B_TAR_PCR_AVG), pair->pcrSizeAvgTarSeqs);
    //str = QString("%1").arg(pair->pcrSizeMinTarSeqs);
    tableModel->setData(tableModel->index(dispTripleSel,B_TAR_PCR_MIN), pair->pcrSizeMinTarSeqs);
    //str = QString("%1").arg(pair->pcrSizeMaxTarSeqs);
    tableModel->setData(tableModel->index(dispTripleSel,B_TAR_PCR_MAX), pair->pcrSizeMaxTarSeqs);

    //str = QString("%1").arg(pair->pctgInNonTarSeqs);
    tableModel->setData(tableModel->index(dispTripleSel,B_NONTAR_PCTG), pair->pctgInNonTarSeqs);
    //str = QString("%1").arg(pair->pcrSizeAvgNonTarSeqs);
    tableModel->setData(tableModel->index(dispTripleSel,B_NONTAR_PCR_AVG), pair->pcrSizeAvgNonTarSeqs);
    //str = QString("%1").arg(pair->pcrSizeMinNonTarSeqs);
    tableModel->setData(tableModel->index(dispTripleSel,B_NONTAR_PCR_MIN), pair->pcrSizeMinNonTarSeqs);
    //str = QString("%1").arg(pair->pcrSizeMaxNonTarSeqs);
    tableModel->setData(tableModel->index(dispTripleSel,B_NONTAR_PCR_MAX), pair->pcrSizeMaxNonTarSeqs);

    dispTripleSel = -1;
    delete pDlg;
}*/
/*
void TripleReportWindow::primerInterComp()
{

    Triple *tp = tripleShowList[dispTripleSel];
    PrimerPair *pr = tp->Pr;

    QString left = pr->left;
    char* tmpLeft = left.toAscii().data();
    std::reverse(tmpLeft, tmpLeft+left.length());
    QString leftRevs = QString(tmpLeft);

    QString right = pr->rightRevsComp;
    char* tmpRight = right.toAscii().data();
    std::reverse(tmpRight, tmpRight+right.length());
    QString rightRevs = QString(tmpRight);

    dispTripleSel = -1;
    ui->textEdit->clear();
    ui->textEdit->append("Primer inter-complementarity\n\n");

    int lenLeft = left.length();
    int lenRight= right.length();

    int maxComp, numComp, pos, shift, nSpace;
    QString str;

    // 1. left-right inter-complement
    maxComp = 0;
    for( int start = 0; start < lenLeft; start++ ){
        numComp = 0;
        for( int i = start, j = lenRight-1; i < lenLeft && j >= 0; i++, j-- ){
            if(  tools::base2int(left[i]) + tools::base2int(right[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = start;
            shift = 1;
        }
    }
    for( int start = lenLeft-1; start >= 0; start-- ){
        numComp = 0;
        for( int i = start, j = 0; i >= 0 && j < lenRight; i--, j++ ){
            if( tools::base2int(left[i]) + tools::base2int(right[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = start;
            shift = 2;
        }
    }
    str = QString("%1").arg(maxComp);
    ui->textEdit->append("\nForward-reverse primer inter-complementarity: ");
    if( maxComp > 1 ){
        ui->textEdit->append(str + ".\n");

        if( shift == 1 ){
            ui->textEdit->append("5'-" + left +"-3'\n");

            str = "   ";
            for( int i = 0; i < pos; i++ )
                str.append(" ");
            for( int i = pos, j = lenRight-1; i < lenLeft && j >= 0; i++, j-- ){
                if( tools::base2int(left[i]) + tools::base2int(right[j]) == 3 )
                    str.append("|");
                else
                    str.append(" ");
            }
            ui->textEdit->append(str+"\n");

            str.clear();
            for( int i = 0; i < pos; i++ )
                str.append(" ");
            str.append("3'-" + rightRevs);
            ui->textEdit->append(str+"-5'\n");
        }
        else{
            nSpace = lenRight - (pos+1);
            str.clear();
            for( int i = 0; i < nSpace; i++ )
                str.append(" ");
            str.append("5'-" + left);
            ui->textEdit->append(str + "-3'\n");

            str = "   ";
            for( int i = 0; i < nSpace; i++ )
                str.append(" ");
            for( int i = 0, j = pos; i <= pos && j >= 0; i++, j-- ){
                if( tools::base2int(left[i]) + tools::base2int(right[j]) == 3 )
                    str.append("|");
                else
                    str.append(" ");
            }
            ui->textEdit->append(str + "\n");

            ui->textEdit->append("3'-" + rightRevs + "-5'\n");
        }
    }
    else
        ui->textEdit->append("0.\n");

    // 2. left-left inter-complement
     maxComp = 0;
    for( int start = 0; start < lenLeft; start++ ){
        numComp = 0;
        for( int i = start, j = lenLeft-1; i < lenLeft && j >= 0; i++, j-- ){
            if(  tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = start;
            shift = 1;
        }
    }
    for( int start = lenLeft-1; start >= 0; start-- ){
        numComp = 0;
        for( int i = start, j = 0; i >= 0 && j < lenLeft; i--, j++ ){
            if( tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = start;
            shift = 2;
        }
    }
    str = QString("%1").arg(maxComp);
    ui->textEdit->append("\nForward-forward primer inter-complementarity: ");
    if( maxComp > 1 ){
        ui->textEdit->append(str + ".\n");

        if( shift == 1 ){
            ui->textEdit->append("5'-" + left +"-3'\n");

            str = "   ";
            for( int i = 0; i < pos; i++ )
                str.append(" ");
            for( int i = pos, j = lenLeft-1; i < lenLeft && j >= 0; i++, j-- ){
                if( tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                    str.append("|");
                else
                    str.append(" ");
            }
            ui->textEdit->append(str+"\n");

            str.clear();;
            for( int i = 0; i < pos; i++ )
                str.append(" ");
            str.append("3'-" + leftRevs);
            ui->textEdit->append(str+"-5'\n");
        }
        else{
            nSpace = lenLeft - (pos+1);
            str.clear();
            for( int i = 0; i < nSpace; i++ )
                str.append(" ");
            str.append("5'-" + left);
            ui->textEdit->append(str + "-3'\n");

            str = "   ";
            for( int i = 0; i < nSpace; i++ )
                str.append(" ");
            for( int i = 0, j = pos; i <= pos && j >= 0; i++, j-- ){
                if( tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                    str.append("|");
                else
                    str.append(" ");
            }
            ui->textEdit->append(str + "\n");

            ui->textEdit->append("3'-" + leftRevs + "-5'\n");
        }
    }
    else
        ui->textEdit->append("0.\n");

    // 3. right-right inter-complement
    maxComp = 0;
    for( int start = 0; start < lenRight; start++ ){
        numComp = 0;
        for( int i = start, j = lenRight-1; i < lenRight && j >= 0; i++, j-- ){
            if(  tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = start;
            shift = 1;
        }
    }
    for( int start = lenRight-1; start >= 0; start-- ){
        numComp = 0;
        for( int i = start, j = 0; i >= 0 && j < lenRight; i--, j++ ){
            if( tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = start;
            shift = 2;
        }
    }
    str = QString("%1").arg(maxComp);
    ui->textEdit->append("\nReverse-reverse primer inter-complementarity: ");
    if( maxComp > 1 ){
        ui->textEdit->append(str + ".\n");

        if( shift == 1 ){
            ui->textEdit->append("5'-" + right +"-3'\n");

            str = "   ";
            for( int i = 0; i < pos; i++ )
                str.append(" ");
            for( int i = pos, j = lenRight-1; i < lenRight && j >= 0; i++, j-- ){
                if( tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                    str.append("|");
                else
                    str.append(" ");
            }
            ui->textEdit->append(str+"\n");

            str.clear();
            for( int i = 0; i < pos; i++ )
                str.append(" ");
            str.append("3'-" + rightRevs);
            ui->textEdit->append(str+"-5'\n");
        }
        else{
            nSpace = lenRight - (pos+1);
            str.clear();
            for( int i = 0; i < nSpace; i++ )
                str.append(" ");
            str.append("5'-" + right);
            ui->textEdit->append(str + "-3'\n");

            str = "   ";
            for( int i = 0; i < nSpace; i++ )
                    str.append(" ");
            for( int i = 0, j = pos; i <= pos && j >= 0; i++, j-- ){
                if( tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                    str.append("|");
                else
                    str.append(" ");
            }
            ui->textEdit->append(str + "\n");

            ui->textEdit->append("3'-" + rightRevs + "-5'\n");
        }
    }
    else
        ui->textEdit->append("0.\n");
}

void TripleReportWindow::primer3InterComp()
{
    Triple *tp = tripleShowList[dispTripleSel];
    PrimerPair *pr = tp->Pr;

    QString left = pr->left;
    char* tmpLeft = left.toAscii().data();
    std::reverse(tmpLeft, tmpLeft+left.length());
    QString leftRevs = QString(tmpLeft);

    QString right = pr->rightRevsComp;
    char* tmpRight = right.toAscii().data();
    std::reverse(tmpRight, tmpRight+right.length());
    QString rightRevs = QString(tmpRight);

    dispTripleSel = -1;
    ui->textEdit->clear();
    ui->textEdit->append("Primer 3' inter-complementarity\n\n");


    int lenLeft = left.length();
    int lenRight= right.length();

    int start, maxComp, numComp, pos;
    QString str;

    // 1. left-right inter-complement
    maxComp = 0;
    if( primerInterBase3end < lenLeft )
        start = lenLeft - primerInterBase3end;
    else
        start = 0;
    for( ; start < lenLeft; start++ ){
        numComp = 0;
        for( int i = start, j = lenRight-1; i < lenLeft && j >= 0; i++, j-- ){
            if(  tools::base2int(left[i]) + tools::base2int(right[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = start;
        }
    }
    str = QString("%1").arg(maxComp);
    ui->textEdit->append("\nForward-reverse primer 3' inter-complementarity: ");
    if( maxComp > 1 ){
        ui->textEdit->append(str + ".\n");

        ui->textEdit->append("5'-" + left +"-3'\n");

        str = "   ";
        for( int i = 0; i < pos; i++ )
            str.append(" ");
        for( int i = pos, j = lenRight-1; i < lenLeft && j >= 0; i++, j-- ){
            if( tools::base2int(left[i]) + tools::base2int(right[j]) == 3 )
                str.append("|");
            else
                str.append(" ");
        }
        ui->textEdit->append(str+"\n");

        str.clear();;
        for( int i = 0; i < pos; i++ )
            str.append(" ");
        str.append("3'-" + rightRevs);
        ui->textEdit->append(str+"-5'\n");
    }
    else
        ui->textEdit->append("0.\n");

    // 2. left-left inter-complement
    maxComp = 0;
    if( primerInterBase3end < lenLeft )
        start = lenLeft - primerInterBase3end;
    else
        start = 0;
    for( ; start < lenLeft; start++ ){
        numComp = 0;
        for( int i = start, j = lenLeft-1; i < lenLeft && j >= 0; i++, j-- ){
            if(  tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = start;
        }
    }
    str = QString("%1").arg(maxComp);
    ui->textEdit->append("\nForward-forward primer 3' inter-complementarity: ");
    if( maxComp > 1 ){
        ui->textEdit->append(str + ".\n");

        ui->textEdit->append("5'-" + left +"-3'\n");

        str = "   ";
        for( int i = 0; i < pos; i++ )
            str.append(" ");
        for( int i = pos, j = lenLeft-1; i < lenLeft && j >= 0; i++, j-- ){
            if( tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                str.append("|");
            else
                str.append(" ");
        }
        ui->textEdit->append(str+"\n");

        str.clear();;
        for( int i = 0; i < pos; i++ )
            str.append(" ");
        str.append("3'-" + leftRevs);
        ui->textEdit->append(str+"-5'\n");
    }
    else
        ui->textEdit->append("0.\n");

    // 3. right-right inter-complement
    maxComp = 0;
    if( primerInterBase3end < lenRight )
        start = lenRight - primerInterBase3end;
    else
        start = 0;
    for( ; start < lenRight; start++ ){
        numComp = 0;
        for( int i = start, j = lenRight-1; i < lenRight && j >= 0; i++, j-- ){
            if(  tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = start;
        }
    }
    str = QString("%1").arg(maxComp);
    ui->textEdit->append("\nReverse-reverse primer 3' inter-complementarity: ");
    if( maxComp > 1 ){
        ui->textEdit->append(str + ".\n");

        ui->textEdit->append("5'-" + right +"-3'\n");

        str = "   ";
        for( int i = 0; i < pos; i++ )
            str.append(" ");
        for( int i = pos, j = lenRight-1; i < lenRight && j >= 0; i++, j-- ){
            if( tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                str.append("|");
            else
                str.append(" ");
        }
        ui->textEdit->append(str+"\n");

        str.clear();
        for( int i = 0; i < pos; i++ )
            str.append(" ");
        str.append("3'-" + rightRevs);
        ui->textEdit->append(str+"-5'\n");
    }
    else
        ui->textEdit->append("0.\n");
}

void TripleReportWindow::primerIntraComp()
{
    Triple *tp = tripleShowList[dispTripleSel];
    PrimerPair *pr = tp->Pr;

    QString left = pr->left;
    QString right = pr->rightRevsComp;

    dispTripleSel = -1;
    ui->textEdit->clear();
    ui->textEdit->append("Primer intra-complementarity\n\n");

    int lenLeft = left.length();
    int lenRight= right.length();

    int maxComp, numComp, pos, shift, nSpace;
    QString str;

    // left primer intra-complement
    maxComp = 0;
    for( int lEnd = 0; lEnd < lenLeft-1; lEnd++ ){
        numComp = 0;
        for( int i = lEnd, j = lenLeft-1; i < j-1; i++, j-- ){
            if( tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = lEnd;
            shift = 1;
        }
    }
    for( int rEnd = lenLeft-1; rEnd > 0; rEnd-- ){
        numComp = 0;
        for( int i = 0, j = rEnd; i < j-1; i++, j-- ){
            if( tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = rEnd;
            shift = 2;
        }
    }
    str = QString("%1").arg(maxComp);
    ui->textEdit->append("\nForward primer intra-complementarity: ");
    if( maxComp > 1 ){
        ui->textEdit->append(str + ".\n");

        if( shift == 1 ){
            str = "5'-";
            for( int i = 0; i < pos + ceil( (float)(lenLeft-pos)/2.0 ); i++ )
                str.append(left[i]);
            ui->textEdit->append(str + "\n");

            str = "   ";
            for( int i = 0; i < pos; i++ )
                    str.append(" ");
            for( int i = pos, j =  lenLeft-1; i < j-1; i++, j-- ){
                if( tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                    str.append("|");
                else
                    str.append(" ");
            }
            ui->textEdit->append(str + "\n");

            str.clear();
            for( int i = 0; i < pos; i++ )
                str.append(" ");
            str.append("3'-");
            for( int i = lenLeft-1; i >= lenLeft - (lenLeft-pos)/2.0; i-- )
                str.append(left[i]);
            ui->textEdit->append(str + "\n");
        }
        else{
            nSpace = lenLeft - (pos+1);
            str.clear();
            for( int i = 0; i < nSpace; i++ )
                str.append(" ");
            str.append("5'-");
            for( int i = 0; i < ceil((float)pos/2.0); i++ )
                str.append(left[i]);
            ui->textEdit->append(str + "\n");

            str = "   ";
            for( int i = 0; i < nSpace; i++ )
                    str.append(" ");
            for( int i = 0, j = pos; i < j-1; i++, j-- ){
                if( tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                    str.append("|");
                else
                    str.append(" ");
            }
            ui->textEdit->append(str + "\n");

            str = "3'-";
            for( int i = lenLeft-1; i >= ceil((float)pos/2.0); i-- )
                str.append(left[i]);
            ui->textEdit->append(str + "\n");
        }
    }
    else
        ui->textEdit->append("0.\n");


    // right primer intra-complement
    maxComp = 0;
    for( int lEnd = 0; lEnd < lenRight-1; lEnd++ ){
        numComp = 0;
        for( int i = lEnd, j = lenRight-1; i < j-1; i++, j-- ){
            if( tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = lEnd;
            shift = 1;
        }
    }
    for( int rEnd = lenRight-1; rEnd > 0; rEnd-- ){
        numComp = 0;
        for( int i = 0, j = rEnd; i < j-1; i++, j-- ){
            if( tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = rEnd;
            shift = 2;
        }
    }
    str = QString("%1").arg(maxComp);
    ui->textEdit->append("\nReverse primer intra-complementarity: ");
    if( maxComp > 1 ){
        ui->textEdit->append(str + ".\n");

        if( shift == 1 ){
            str = "5'-";
            for( int i = 0; i < pos + ceil( (float)(lenRight-pos)/2.0 ); i++ )
                str.append(right[i]);
            ui->textEdit->append(str + "\n");

            str = "   ";
            for( int i = 0; i < pos; i++ )
                    str.append(" ");
            for( int i = pos, j =  lenRight-1; i < j-1; i++, j-- ){
                if( tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                    str.append("|");
                else
                    str.append(" ");
            }
            ui->textEdit->append(str + "\n");

            str.clear();
            for( int i = 0; i < pos; i++ )
                str.append(" ");
            str.append("3'-");
            for( int i = lenRight-1; i >= lenRight - (lenRight-pos)/2.0; i-- )
                str.append(right[i]);
            ui->textEdit->append(str + "\n");
        }
        else{
            nSpace = lenRight - (pos+1);
            str.clear();
            for( int i = 0; i < nSpace; i++ )
                str.append(" ");
            str.append("5'-");
            for( int i = 0; i < ceil((float)pos/2.0); i++ )
                str.append(right[i]);
            ui->textEdit->append(str + "\n");

            str = "   ";
            for( int i = 0; i < nSpace; i++ )
                str.append(" ");
            for( int i = 0, j = pos; i < j-1; i++, j-- ){
                if( tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                        str.append("|");
                else
                        str.append(" ");
            }
            ui->textEdit->append(str + "\n");

            str = "3'-";
            for( int i = lenRight-1; i >= ceil((float)pos/2.0); i-- )
                str.append(right[i]);
            ui->textEdit->append(str + "\n");
        }
    }
    else
        ui->textEdit->append("0.\n");
}

void TripleReportWindow::primer3IntraComp()
{
    Triple *tp = tripleShowList[dispTripleSel];
    //PrimerPair *pr = tp->Pr;

    QString left = pr->left;
    QString right = pr->rightRevsComp;

    dispTripleSel = -1;
    ui->textEdit->clear();
    ui->textEdit->append(" 3' intra-complementarity\n\n");

    int lenLeft = left.length();
    int lenRight= right.length();

    int start, maxComp, numComp, pos;
    QString str;

    // left primer 3' intra-complement
    maxComp = 0;
    start = lenLeft - primerIntraBase3end*2;
    if( start < 0 )
            start = 0;
    for( ; start < lenLeft-1; start++ ){
        numComp = 0;
        for( int i = start, j = lenLeft-1; i < j-1; i++, j-- ){
            if( tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = start;
        }
    }
    str = QString("%1").arg(maxComp);
    ui->textEdit->append("\nForward primer 3' intra-complementarity: ");
    if( maxComp > 1 ){
        ui->textEdit->append(str + ".\n");

        str = "5'-";
        for( int i = 0; i < pos + ceil( (float)(lenLeft-pos)/2.0 ); i++ )
            str.append(left[i]);
        ui->textEdit->append(str + "\n");

        str = "   ";
        for( int i = 0; i < pos; i++ )
            str.append(" ");
        for( int i = pos, j =  lenLeft-1; i < j-1; i++, j-- ){
            if( tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                str.append("|");
            else
                str.append(" ");
        }
        ui->textEdit->append(str + "\n");

        str.clear();
        for( int i = 0; i < pos; i++ )
            str.append(" ");
        str.append("3'-");
        for( int i = lenLeft-1; i >= lenLeft - (lenLeft-pos)/2.0; i-- )
            str.append(left[i]);
        ui->textEdit->append(str + "\n");
    }
    else
        ui->textEdit->append("0.\n");


    // right primer intra-complement
    maxComp = 0;
    start = lenRight - primerIntraBase3end*2;
    if( start < 0 )
            start = 0;
    for( ; start < lenRight-1; start++ ){
        numComp = 0;
        for( int i = start, j = lenRight-1; i < j-1; i++, j-- ){
            if( tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = start;
        }
    }
    str = QString("%1").arg(maxComp);
    ui->textEdit->append("\nReverse primer 3' intra-complementarity: ");
    if( maxComp > 1 ){
        ui->textEdit->append(str + ".\n");

        str = "5'-";
        for( int i = 0; i < pos + ceil( (float)(lenRight-pos)/2.0 ); i++ )
            str.append(right[i]);
        ui->textEdit->append(str + "\n");

        str = "   ";
        for( int i = 0; i < pos; i++ )
            str.append(" ");
        for( int i = pos, j =  lenRight-1; i < j-1; i++, j-- ){
            if( tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                str.append("|");
            else
                str.append(" ");
        }
        ui->textEdit->append(str + "\n");

        str.clear();
        for( int i = 0; i < pos; i++ )
            str.append(" ");
        str.append("3'-");
        for( int i = lenRight-1; i >= lenRight - (lenRight-pos)/2.0; i-- )
            str.append(right[i]);
        ui->textEdit->append(str + "\n");
    }
    else
        ui->textEdit->append("0.\n");
}*/

/*
void TripleReportWindow::primerIntraComp()
{
    QString left = primerPairShowList[dispTripleSel]->left;
    QString right = primerPairShowList[dispTripleSel]->rightRevsComp;

    dispTripleSel = -1;
    ui->textEdit->clear();
    ui->textEdit->append("Primer intra-complementarity\n\n");

    int lenLeft = left.length();
    int lenRight= right.length();

    int maxComp, numComp, pos, shift, nSpace;
    QString str;

    // left primer intra-complement
    maxComp = 0;
    for( int lEnd = 0; lEnd < lenLeft-1; lEnd++ ){
        numComp = 0;
        for( int i = lEnd, j = lenLeft-1; i < j-1; i++, j-- ){
            if( tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = lEnd;
            shift = 1;
        }
    }
    for( int rEnd = lenLeft-1; rEnd > 0; rEnd-- ){
        numComp = 0;
        for( int i = 0, j = rEnd; i < j-1; i++, j-- ){
            if( tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = rEnd;
            shift = 2;
        }
    }

    str = "\nForward primer intra-complementarity: ";
    str += QString("%1.\n").arg(maxComp);
    if( maxComp > 1 ){
        if( shift == 1 ){
            str += "5'-";
            for( int i = 0; i < pos + ceil( (float)(lenLeft-pos)/2.0 ); i++ )
                str.append(left[i]);
            str += "\n";

            str += "   ";
            for( int i = 0; i < pos; i++ )
                str += " ";
            for( int i = pos, j =  lenLeft-1; i < j-1; i++, j-- ){
                if( tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                    str += "|";
                else
                    str += " ";
            }
            str += "\n";

            for( int i = 0; i < pos; i++ )
                str.append(" ");
            str += "3'-";
            for( int i = lenLeft-1; i >= lenLeft - (lenLeft-pos)/2.0; i-- )
                str.append(left[i]);
            str += "\n";
        }
        else{
            nSpace = lenLeft - (pos+1);
            for( int i = 0; i < nSpace; i++ )
                str += " ";
            str += "5'-";
            for( int i = 0; i < ceil((float)pos/2.0); i++ )
                str += left[i];
            str += "\n";

            str = "   ";
            for( int i = 0; i < nSpace; i++ )
                    str += " "  ;
            for( int i = 0, j = pos; i < j-1; i++, j-- ){
                if( tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                    str += "|";
                else
                    str += " ";
            }
            str += "\n";

            str += "3'-";
            for( int i = lenLeft-1; i >= ceil((float)pos/2.0); i-- )
                str += left[i];
            str += "\n";
        }
    }
    ui->textEdit->append(str);


    // right primer intra-complement
    maxComp = 0;
    for( int lEnd = 0; lEnd < lenRight-1; lEnd++ ){
        numComp = 0;
        for( int i = lEnd, j = lenRight-1; i < j-1; i++, j-- ){
            if( tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = lEnd;
            shift = 1;
        }
    }
    for( int rEnd = lenRight-1; rEnd > 0; rEnd-- ){
        numComp = 0;
        for( int i = 0, j = rEnd; i < j-1; i++, j-- ){
            if( tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = rEnd;
            shift = 2;
        }
    }

    str = "\nReverse primer intra-complementarity: ";
    str += QString("%1.\n").arg(maxComp);
    if( maxComp > 1 ){
        if( shift == 1 ){
            str += "5'-";
            for( int i = 0; i < pos + ceil( (float)(lenRight-pos)/2.0 ); i++ )
                str += right[i];
            str += "\n";

            str += "   ";
            for( int i = 0; i < pos; i++ )
                str += " ";
            for( int i = pos, j =  lenRight-1; i < j-1; i++, j-- ){
                if( tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                    str += "|";
                else
                    str += " ";
            }
            str += "\n";

            for( int i = 0; i < pos; i++ )
                str += " ";
            str += "3'-";
            for( int i = lenRight-1; i >= lenRight - (lenRight-pos)/2.0; i-- )
                str += right[i];
            str += "\n";
        }
        else{
            nSpace = lenRight - (pos+1);
            for( int i = 0; i < nSpace; i++ )
                str += " ";
            str += "5'-";
            for( int i = 0; i < ceil((float)pos/2.0); i++ )
                str += right[i];
            str += "\n";

            str += "   ";
            for( int i = 0; i < nSpace; i++ )
                str += " ";
            for( int i = 0, j = pos; i < j-1; i++, j-- ){
                if( tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                    str += "|";
                else
                    str += " ";
            }
            str += "\n";

            str += "3'-";
            for( int i = lenRight-1; i >= ceil((float)pos/2.0); i-- )
                str += right[i];
            str += "\n";
        }
    }

    ui->textEdit->append(str);
}
*/



/*void TripleReportWindow::blastForwardPrimer()
{
    QString url = "http://www.ncbi.nih.gov/BLAST/Blast.cgi?QUERY=";
    url +=	tripleShowList[dispTripleSel]->Pr->left +
                    "&CMD=Web&LAYOUT=TwoWindows&AUTO_FORMAT=Semiauto"
                    "&ALIGNMENTS=50&ALIGNMENT_VIEW=Pairwise&CLIENT=web"
                    "&DATABASE=nr&DESCRIPTIONS=100&ENTREZ_QUERY=(none)"
                    "&EXPECT=1000&FORMAT_OBJECT=Alignment&FORMAT_TYPE=HTML"
                    "&NCBI_GI=on&PAGE=Nucleotides&PROGRAM=blastn"
                    "&SERVICE=plain&SET_DEFAULTS.x=29&SET_DEFAULTS.y=6"
                    "&SHOW_OVERVIEW=on&WORD_SIZE=7&END_OF_HTTPGET=Yes"
                    "&SHOW_LINKOUT=yes&GET_SEQUENCE=yes";

    dispTripleSel = -1;

#ifdef LINUX
    QString commend = "xdg-open "+url;
    system(commend.toAscii().data());
#elif defined(WINDOWS)
       QDesktopServices::openUrl(QUrl(QString(url)));
#endif
}


void TripleReportWindow::blastReversePrimer()
{
    QString url = "http://www.ncbi.nih.gov/BLAST/Blast.cgi?QUERY=";
    url +=	tripleShowList[dispTripleSel]->Pr->rightRevsComp +
                    "&CMD=Web&LAYOUT=TwoWindows&AUTO_FORMAT=Semiauto"
                    "&ALIGNMENTS=50&ALIGNMENT_VIEW=Pairwise&CLIENT=web"
                    "&DATABASE=nr&DESCRIPTIONS=100&ENTREZ_QUERY=(none)"
                    "&EXPECT=1000&FORMAT_OBJECT=Alignment&FORMAT_TYPE=HTML"
                    "&NCBI_GI=on&PAGE=Nucleotides&PROGRAM=blastn"
                    "&SERVICE=plain&SET_DEFAULTS.x=29&SET_DEFAULTS.y=6"
                    "&SHOW_OVERVIEW=on&WORD_SIZE=7&END_OF_HTTPGET=Yes"
                    "&SHOW_LINKOUT=yes&GET_SEQUENCE=yes";

    dispTripleSel = -1;

#ifdef LINUX
    QString commend = "xdg-open "+url;
    system(commend.toAscii().data());
#elif defined(WINDOWS)
       QDesktopServices::openUrl(QUrl(QString(url)));
#endif
}*/







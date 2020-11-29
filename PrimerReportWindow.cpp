#include "PrimerReportWindow.h"
#include "ui_PrimerReportWindow.h"
#include "DataTools.h"
//#include <prise.h>
//#include "PrimerDispListDlg.h"
//#include "PrimerSaveListDlg.h"
//#include "PrimerSaveExcelDlg.h"

/*PrimerReportWindow* tmp2;
void onDisplayAllColumn()
{
    tmp2->displayAllColumn();
}

void onLoadPrimerData()
{
    tmp2->displayData();
}

void onInsertPrimer(int idx)
{
    tmp2->insertData(idx);
}*/

bool less_formula(const PrimerPair* m1, const PrimerPair* m2)
{
    return pow(100 - m1->pctgInTargetSeqs, 2 ) + 0.5f * pow( m1->pctgInNonTarSeqsLeft, 2 ) + 0.5f * pow( m1->pctgInNonTarSeqsRight, 2 )
                    <
            pow( 100 - m2->pctgInTargetSeqs, 2 ) + 0.5f * pow( m2->pctgInNonTarSeqsLeft, 2 ) + 0.5f * pow( m2->pctgInNonTarSeqsRight, 2 );
}

bool lt_by_seq(const PrimerPair* m1, const PrimerPair* m2)
{
    return (m1->left < m2->left) || (m1->left == m2->left && m1->rightRevsComp <= m2->rightRevsComp );
}
bool gt_by_seq(const PrimerPair* m1, const PrimerPair* m2)
{
    return (m1->left > m2->left) || (m1->left == m2->left && m1->rightRevsComp >= m2->rightRevsComp);
}


PrimerReportWindow::PrimerReportWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PrimerReportWindow)
{
    showWindow = true;

    ui->setupUi(this);
    setCentralWidget(ui->widget);
    this->setWindowTitle(targetSeqFilename+"- Primer Report");
    ui->actionUpdate_Estimated_Information_to_Exact_Information->setEnabled(false);
    //tmp2 = this;
    //DataTools::registerOnDisplayAllColumn(onDisplayAllColumn);
    //DataTools::registerOnLoadPrimerData(onLoadPrimerData);
    //DataTools::registerOnInsertPrimer(onInsertPrimer);

    tableModel =  new QStandardItemModel(0, A_NUM_COLUMNS, this);
    ui->tableView->setEditTriggers(0);
    int numTar = tarSeqs.size();

    primerColumns.clear();
    primerColumnDisp.clear();
    primerColumnHide.clear();
    primerColumns.push_back(" ");                                  primerColWidth[A_PRIMER_NO] = 30;
    primerColumns.push_back("Primer pair");                               primerColWidth[A_PRIMER_PAIR] = 200;
    primerColumns.push_back("Target\npair\n%");                           primerColWidth[A_TARGET_PCTG] = 55;
    primerColumns.push_back("Target\nforward\n%");                        primerColWidth[A_LFT_TARGET_PCTG] = 55;
    primerColumns.push_back("Target\nreverse\n%");                        primerColWidth[A_RHT_TARGET_PCTG] = 55;
    primerColumns.push_back("Non-target\npair\n%");                       primerColWidth[A_NONTAR_PCTG] = 65;
    primerColumns.push_back("Non-target\nforward\n%");                    primerColWidth[A_LFT_NONTAR_PCTG] = 65;
    primerColumns.push_back("Non-target\nreverse\n%");                    primerColWidth[A_RHT_NONTAR_PCTG] = 65;
    primerColumns.push_back("Target\nforward\nannealing avg pos");        primerColWidth[A_LFT_TAR_BIND_POS] = 105;
    primerColumns.push_back("Target\nreverse\nannealing avg pos");        primerColWidth[A_RHT_TAR_BIND_POS] = 105;
    primerColumns.push_back("Target\nPCR avg\nsize");                     primerColWidth[A_TAR_PCR_AVG] = 55;
    primerColumns.push_back("Target\nPCR min\nsize");                     primerColWidth[A_TAR_PCR_MIN] = 55;
    primerColumns.push_back("Target\nPCR max\nsize");                     primerColWidth[A_TAR_PCR_MAX] = 55;
    primerColumns.push_back("Non-target\nPCR avg\nsize");                 primerColWidth[A_NONTAR_PCR_AVG] = 65;
    primerColumns.push_back("Non-target\nPCR min\nsize");                 primerColWidth[A_NONTAR_PCR_MIN] = 65;
    primerColumns.push_back("Non-target\nPCR max\nsize");                 primerColWidth[A_NONTAR_PCR_MAX] = 65;
    primerColumns.push_back("\n(Fw)\nGC%");                               primerColWidth[A_LFT_GC] = 35;
    primerColumns.push_back("\n(Rv)\nGC% ");                              primerColWidth[A_RHT_GC] = 35;
    primerColumns.push_back("\n(Fw)\nTm(C)");                             primerColWidth[A_LFT_TM] = 40;
    primerColumns.push_back("\n(Rv)\nTm(C)");                             primerColWidth[A_RHT_TM] = 40;
    primerColumns.push_back("Inter\ncomple-\nmentarity");                primerColWidth[A_INTER_COMP] = 70;
    primerColumns.push_back("3' Inter\ncomple-\nmentarity");             primerColWidth[A_INTER3_COMP] = 70;
    primerColumns.push_back("(Fw)Primer intra\ncomple-\nmentarity");     primerColWidth[A_LFT_INTRA_COMP] = 70;
    primerColumns.push_back("(Rv)Primer intra\ncomple-\nmentarity");     primerColWidth[A_RHT_INTRA_COMP] = 70;
    primerColumns.push_back("(Fw)Primer 3' intra\ncomple-\nmentarity");  primerColWidth[A_LFT_INTRA3_COMP] = 80;
    primerColumns.push_back("(Rv)Primer 3' intra\ncomple-\nmentarity");  primerColWidth[A_RHT_INTRA3_COMP] = 80;
    //primerColumns.push_back("# of target sequences\n binding with ambiguous bases\n(Fw Primer)");              primerColWidth[A_NUM_AMBI_TARGETS_LEFT] = 115;
    //primerColumns.push_back("# of target sequences\n binding with ambiguous bases\n(Rv Primer)");              primerColWidth[A_NUM_AMBI_TARGETS_RIGHT] = 115;
    primerColumns.push_back(QString("# of target sequences\n(out of %1)\nwith ambiguous bases\ncovered by Fw primer").arg(numTar));              primerColWidth[A_NUM_AMBI_TARGETS_LEFT] = 115;
    primerColumns.push_back(QString("# of target sequences\n(out of %1)\nwith ambiguous bases\ncovered by Rv primer").arg(numTar));              primerColWidth[A_NUM_AMBI_TARGETS_RIGHT] = 115;
    primerColumns.push_back("# of ambiguous bases\nin target sequences that\n(Fw) primer binds to");          primerColWidth[A_NUM_AMBI_BASES_LEFT] = 120;
    primerColumns.push_back("# of ambiguous bases\nin target sequences that\n(Rv) primer binds to");          primerColWidth[A_NUM_AMBI_BASES_RIGHT] = 120;
    //primerColumns.push_back("Total # of\ntarget\nsequences");             primerColWidth[A_NUM_TAR] = 60;
    //primerColumnDisp = QStringList(primerColumns);

    // for sort order of each column and save or notcreateModel for each column in save excel dialog
    for( int i = 0; i < A_NUM_COLUMNS; i++ ){
        primerColSortOrder[i] = true;
        primerExcelSaveColumns[i] = true;
        primerColumnDisp.push_back(i);
    }
    this->createModel();

    dispPrimerSel = -1;
    addPrimerMauallyReady = false;

    // for display primer list conditionally, there are 4 conditions
    for( int i = 0; i < 4; i++ ){
        dispPrimerCondTitle[i] = 0;
        dispPrimerCondGtLt[i] = -1;
        dispPrimerCondValue[i] = 0.0;
        dispPrimerCondAllowCancel = false;
    }
    // for primer list sorting by multiple keys
    primerSortBy[0] = SORT_BY_FORMULA;        primerColSortOrder[0] = ASCENDING;
    primerSortBy[1] = -1;                     primerColSortOrder[1] = ASCENDING;
    primerSortBy[2] = -1;                     primerColSortOrder[2] = ASCENDING;
    primerSortBy[3] = -1;                     primerColSortOrder[3] = ASCENDING;

    // for primer pair list save: 1 for save all displayed primer pairs
    primerListSaveAllDispMark = 1;
    primerExcelSaveAllDispMark = 1;

    // for primer annealing content user constraints
    bindingContentDivider3end = 8;
    bindingContentMatchPctg5end = 50;
    bindingContentMatchPctg3end = 62.5;


    displayData();
    /*if( design == 0 ){ // load previusly designed primer list
    }
    else{ // display newly designed primer list
        displayData();
    }*/
}

PrimerReportWindow::~PrimerReportWindow()
{
    delete ui;
}

//Set TableView headers and fields.
bool PrimerReportWindow::createModel()
{
    ui->tableView->setModel(tableModel);
    ui->tableView->setSortingEnabled(true);
    for( int i = 0; i < A_NUM_COLUMNS; i++ ) {
        tableModel->setHeaderData(i, Qt::Horizontal, primerColumns[i]);
        ui->tableView->setColumnWidth(i,primerColWidth[i]);
    }

    QHeaderView * HV = ui->tableView->horizontalHeader();
    HV->setSortIndicatorShown(false);
    connect(HV, SIGNAL(sectionClicked(int)), this, SLOT(sortData(int)));

    return true;
}


void PrimerReportWindow::displayData()
{
    // sort on ascending order of primer quality formula
    std::stable_sort( primerPairList.begin(), primerPairList.end(), less_formula );

    // primer list is large, suggest for partial displaying
    if( primerPairList.size() > 1000 ){
        dispPrimerCondAllowCancel = 0;
        PrimerDispListDlg dispDlg(this);
        if (dispDlg.exec()){
            copyDispPrimerList();
        }else{
            showWindow = false;
            close();
        }
    }
    else{ // primer list is small, copy them all
        primerPairShowList.clear();
        for( unsigned int i = 0; i < primerPairList.size(); i++ )
            primerPairShowList.push_back( primerPairList[i] );
    }

    insertShownPrimerList();
    dispPrimerCondAllowCancel = false;
}

void PrimerReportWindow::emptyModel()
{
    for(int i = 0; i < tableModel->rowCount(); i++){
        delete tableModel->item(i,0);
    }
    tableModel->removeRows(0,tableModel->rowCount());
}


void PrimerReportWindow::click(const QModelIndex & idx)
{
    int row = idx.row();
    int column = idx.column();

    if(column == 0) {
        switch(tableModel->item(row, column)->checkState()){
        case Qt::Checked:
            primerPairShowList[row]->marked = true;
            tableModel->setData(tableModel->index(row, 0), " ");
            break;
        case Qt::Unchecked:
            primerPairShowList[row]->marked = false;
            tableModel->setData(tableModel->index(row, 0), "");
            break;
        default:
            QMessageBox::about(this, "Error", "Partially Checked is not valid."
                               "Please contact the developer");
            break;
        }
    }
}

void PrimerReportWindow::rightClick (const QPoint & point)
{
    const QModelIndex idx = ui->tableView->indexAt(point);
    dispPrimerSel = idx.row();
    showRightClickMenu();
}

void PrimerReportWindow::showRightClickMenu ()
{
  QMenu *popupMenu;
  QAction *actionPrimerAnnealPosInfo;
  QAction *actionPercentage;
  QAction *actionTargetWPrimer;
  QAction *actionTargetWoPrimer;
  QAction *actionNonTargetWPrimer;
  QAction *actionNonTargetWoPrimer;
  //QAction *actionGetExactPCRinfo;
  QAction *actionPrimerInterComp;
  QAction *actionPrimer3InterComp;
  QAction *actionPrimerIntraComp;
  QAction *actionPrimer3IntraComp;
  QAction *actionBlastForwardPrimer;
  QAction *actionBlastReversePrimer;
  QAction *actionDeletePrimerPair;
  QAction *actionDesignProbes;

  popupMenu = new QMenu(ui->tableView);
  actionPrimerAnnealPosInfo = new QAction(ui->tableView);
  actionPrimerAnnealPosInfo->setObjectName(QString::fromUtf8("Primer Annealing Position Information"));
  actionPrimerAnnealPosInfo->setText(QString::fromUtf8("Primer Annealing Position Information"));

  actionPercentage = new QAction(ui->tableView);
  actionPercentage->setObjectName(QString::fromUtf8("Percentage of Each Nucleotide in Target and Non-target Sequences in Relation to Primer Sequences"));
  actionPercentage->setText(QString::fromUtf8("Percentage of Each Nucleotide in Target and Non-target Sequences in Relation to Primer Sequences"));

  actionTargetWPrimer = new QAction(ui->tableView);
  actionTargetWPrimer->setObjectName(QString::fromUtf8("Target Sequences Annealing with Primer"));
  actionTargetWPrimer->setText(QString::fromUtf8("Target Sequences Annealing with Primer"));

  actionTargetWoPrimer = new QAction(ui->tableView);
  actionTargetWoPrimer->setObjectName(QString::fromUtf8("Target Sequences Not Annealing with Primer"));
  actionTargetWoPrimer->setText(QString::fromUtf8("Target Sequences Not Annealing with Primer"));

  actionNonTargetWPrimer = new QAction(ui->tableView);
  actionNonTargetWPrimer->setObjectName(QString::fromUtf8("Non-target Sequences Annealing with Primer"));
  actionNonTargetWPrimer->setText(QString::fromUtf8("Non-target Sequences Annealing with Primer"));

  actionNonTargetWoPrimer = new QAction(ui->tableView);
  actionNonTargetWoPrimer->setObjectName(QString::fromUtf8("Non-target Sequences Not Annealing with Primer"));
  actionNonTargetWoPrimer->setText(QString::fromUtf8("Non-target Sequences Not Annealing with Primer"));

  //actionGetExactPCRinfo = new QAction(ui->tableView);
  //actionGetExactPCRinfo->setObjectName(QString::fromUtf8("Update Estimated Information to Exact Information"));
  //actionGetExactPCRinfo->setText(QString::fromUtf8("Update Estimated Information to Exact Information"));

  actionPrimerInterComp = new QAction(ui->tableView);
  actionPrimerInterComp->setObjectName(QString::fromUtf8("Primer Inter-Complementarity"));
  actionPrimerInterComp->setText(QString::fromUtf8("Primer Inter-Complementarity"));

  actionPrimer3InterComp = new QAction(ui->tableView);
  actionPrimer3InterComp->setObjectName(QString::fromUtf8("Primer 3' Inter-Complementarity"));
  actionPrimer3InterComp->setText(QString::fromUtf8("Primer 3' Inter-Complementarity"));

  actionPrimerIntraComp = new QAction(ui->tableView);
  actionPrimerIntraComp->setObjectName(QString::fromUtf8("Primer Intra-Complementarity"));
  actionPrimerIntraComp->setText(QString::fromUtf8("Primer Intra-Complementarity"));

  actionPrimer3IntraComp = new QAction(ui->tableView);
  actionPrimer3IntraComp->setObjectName(QString::fromUtf8("Primer 3' Intra-Complementarity"));
  actionPrimer3IntraComp->setText(QString::fromUtf8("Primer 3' Intra-Complementarity"));

  actionBlastForwardPrimer = new QAction(ui->tableView);
  actionBlastForwardPrimer->setObjectName(QString::fromUtf8("BLAST Forward Primer"));
  actionBlastForwardPrimer->setText(QString::fromUtf8("BLAST Forward Primer"));

  actionBlastReversePrimer = new QAction(ui->tableView);
  actionBlastReversePrimer->setObjectName(QString::fromUtf8("BLAST Reverse Primer"));
  actionBlastReversePrimer->setText(QString::fromUtf8("BLAST Reverse Primer"));

  actionDeletePrimerPair = new QAction(ui->tableView);
  actionDeletePrimerPair->setObjectName(QString::fromUtf8("Delete"));
  actionDeletePrimerPair->setText(QString::fromUtf8("Delete"));

  actionDesignProbes = new QAction(ui->tableView);
  actionDesignProbes->setObjectName(QString::fromUtf8("Design Probes"));
  actionDesignProbes->setText(QString::fromUtf8("Design Probes"));

  popupMenu->addAction(actionPrimerAnnealPosInfo);
  popupMenu->addAction(actionPercentage);
  popupMenu->addAction(actionTargetWPrimer);
  popupMenu->addAction(actionTargetWoPrimer);
  popupMenu->addAction(actionNonTargetWPrimer);
  popupMenu->addAction(actionNonTargetWoPrimer);
  //popupMenu->addAction(actionGetExactPCRinfo);
  popupMenu->addAction(actionPrimerInterComp);
  popupMenu->addAction(actionPrimer3InterComp);
  popupMenu->addAction(actionPrimerIntraComp);
  popupMenu->addAction(actionPrimer3IntraComp);
  popupMenu->addAction(actionBlastForwardPrimer);
  popupMenu->addAction(actionBlastReversePrimer);
  popupMenu->addAction(actionDeletePrimerPair);
  popupMenu->addAction(actionDesignProbes);

  connect(actionPrimerAnnealPosInfo, SIGNAL(triggered()), this, SLOT(dispCompleteInfo()));
  connect(actionPercentage, SIGNAL(triggered()), this, SLOT(reportBindingContent()));
  connect(actionTargetWPrimer, SIGNAL(triggered()), this, SLOT(targetWPrimer()));
  connect(actionTargetWoPrimer, SIGNAL(triggered()), this, SLOT(targetWoPrimer()));
  connect(actionNonTargetWPrimer, SIGNAL(triggered()), this, SLOT(nonTargetWPrimer()));
  connect(actionNonTargetWoPrimer, SIGNAL(triggered()), this, SLOT(nonTargetWoPrimer()));
  //connect(actionGetExactPCRinfo, SIGNAL(triggered()), this, SLOT(getExactPCRinfo()));
  connect(actionPrimerInterComp, SIGNAL(triggered()), this, SLOT(primerInterComp()));
  connect(actionPrimer3InterComp, SIGNAL(triggered()), this, SLOT(primer3InterComp()));
  connect(actionPrimerIntraComp, SIGNAL(triggered()), this, SLOT(primerIntraComp()));
  connect(actionPrimer3IntraComp, SIGNAL(triggered()), this, SLOT(primer3IntraComp()));
  connect(actionBlastForwardPrimer, SIGNAL(triggered()), this, SLOT(blastForwardPrimer()));
  connect(actionBlastReversePrimer, SIGNAL(triggered()), this, SLOT(blastReversePrimer()));
  connect(actionDeletePrimerPair, SIGNAL(triggered()), this, SLOT(deletePrimerPair()));
  //connect(actionDesignProbes, SIGNAL(triggered()), this, SLOT(testSlot(2)));
  //connect(actionDesignProbes, SIGNAL(triggered()), this, SLOT(testSlot(2;)));
  connect(actionDesignProbes, SIGNAL(triggered()), this, SLOT(designProbes4markedPrimerPairs()));


  popupMenu->exec(QCursor::pos());
  delete popupMenu;
  dispPrimerSel = -1;
}

/*
void PrimerReportWindow::testSlot(int x)
{
    int tmpTest = x;
}*/

// return true if "pair" passes all 4 conditions, and false if not
bool PrimerReportWindow::checkPrimerWithCondition(PrimerPair *pair, int *condTitle, int *condGtLt, float *condValue)
{
    bool passChecking = true;
    // check for all 4 conditions
    for( int j = 0; j < 4; j++ ){
        if( condTitle[j] < A_TARGET_PCTG )	// skip checking this condition if it's "primer No" or "primer pair"
            continue;
        if( condGtLt[j] == LESSTHAN && DataTools::getPrimerInfo( pair, condTitle[j] ) >= condValue[j] )
            passChecking = false;
        if( condGtLt[j] == GREATERTHAN && DataTools::getPrimerInfo( pair, condTitle[j] ) <= condValue[j] )
            passChecking = false;
    }

    return passChecking;
}

void PrimerReportWindow::copyDispPrimerList()
{
    int range = primerPairList.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Filtering primer pair list",0, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);

    emptyModel();
    ui->textEdit->clear();
    primerPairShowList.clear();


    // user chooses to display primer pairs by conditions
    if( dispPrimerAllorTopX == 0 ){
        PrimerPair *pair;
        bool passChecking;

        for(int i = 0; i < primerPairList.size(); i++ )
            primerPairShowList.push_back( primerPairList[i] );

        // check for each primer pair
        for(int i = 0, k = 0; i < primerPairShowList.size(); i++, k++ ){
            pair = primerPairShowList[i];

            passChecking = checkPrimerWithCondition( pair, dispPrimerCondTitle, dispPrimerCondGtLt, dispPrimerCondValue );

            if( !passChecking){	// didn't pass all the checks, remove it from this->primerPairList
                primerPairShowList.erase( primerPairShowList.begin()+i );
                i--;
            }
            pDlg->setValue(k+1);
        }
    }

    else if( dispPrimerAllorTopX == 1 ){ // user chooses to display all primer pairs
        // copy all primer pairs
        for(int i = 0; i < primerPairList.size(); i++ )
            primerPairShowList.push_back( primerPairList[i] );
    }
    else if( dispPrimerAllorTopX == 2 ){ // user chooses to display top X primer pairs
        // copy top X primer pairs
        for( int i = 0; ( i < dispPrimerTopNum && i < primerPairList.size() ); i++ )
            primerPairShowList.push_back( primerPairList[i] );
    }
    delete pDlg;
}

void PrimerReportWindow::getMarkedList()
{
    markedList.clear();
    for(int i = 0; i < tableModel->rowCount(); i++){
        if(tableModel->item(i,0)->checkState() == Qt::Checked){
            markedList.push_back(i);
        }
    }
}

void PrimerReportWindow::insertData(int dataIdx, int rowIdx)
{
    if (rowIdx == -1){
        rowIdx = tableModel->rowCount();
        tableModel->insertRow(rowIdx);
    }

    PrimerPair *pair = primerPairShowList[dataIdx];


    QStandardItem* __tmpItem = new QStandardItem();
    __tmpItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
    tableModel->setItem(rowIdx,0,__tmpItem);
    if(pair->marked) {
        __tmpItem->setCheckState(Qt::Checked);
        tableModel->setData(tableModel->index(rowIdx, 0), " ");
    } else {
        __tmpItem->setCheckState(Qt::Unchecked);
        tableModel->setData(tableModel->index(rowIdx, 0), "");
    }

    // primer pair
    tableModel->setData(tableModel->index(rowIdx, A_PRIMER_PAIR), pair->left + ".." + pair->rightRevsComp);

    // frequency for single primers
    QString val;
    val.sprintf("%.2f", pair->pctgInTarSeqsLeft);
    tableModel->setData(tableModel->index(rowIdx, A_LFT_TARGET_PCTG),  val);
    tableModel->item(rowIdx, A_LFT_TARGET_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    val.sprintf("%.2f", pair->pctgInTarSeqsRight);
    tableModel->setData(tableModel->index(rowIdx, A_RHT_TARGET_PCTG),  val);
    tableModel->item(rowIdx, A_RHT_TARGET_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    val.sprintf("%.2f", pair->pctgInNonTarSeqsLeft);
    tableModel->setData(tableModel->index(rowIdx, A_LFT_NONTAR_PCTG),  val);
    tableModel->item(rowIdx, A_LFT_NONTAR_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    val.sprintf("%.2f", pair->pctgInNonTarSeqsRight);
    tableModel->setData(tableModel->index(rowIdx, A_RHT_NONTAR_PCTG),  val);
    tableModel->item(rowIdx, A_RHT_NONTAR_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);

    // average annealing position in target sequences: left and right primer
    if( pair->bindPosTarSeqsAvgLeft < 0 )
        tableModel->setData(tableModel->index(rowIdx, A_LFT_TAR_BIND_POS),  "-");
    else{
        val.sprintf("%.2f", pair->bindPosTarSeqsAvgLeft);
        tableModel->setData(tableModel->index(rowIdx, A_LFT_TAR_BIND_POS),  val);
        tableModel->item(rowIdx, A_LFT_TAR_BIND_POS)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    }
    if( pair->bindPosTarSeqsAvgRight < 0 )
        tableModel->setData(tableModel->index(rowIdx, A_RHT_TAR_BIND_POS),  "-");
    else{
        val.sprintf("%.2f", pair->bindPosTarSeqsAvgRight);
        tableModel->setData(tableModel->index(rowIdx, A_RHT_TAR_BIND_POS),  val);
        tableModel->item(rowIdx, A_RHT_TAR_BIND_POS)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    }

    //if( pair->isEstimateTar ){
    //    tableModel->setData(tableModel->index(rowIdx, A_TARGET_PCTG),  pair->pctgInTargetSeqs);
    //    tableModel->setData(tableModel->index(rowIdx, A_TAR_PCR_AVG),  pair->pcrSizeAvgTarSeqs);
    //    tableModel->setData(tableModel->index(rowIdx, A_TAR_PCR_MIN),  pair->pcrSizeMinTarSeqs);
    //    tableModel->setData(tableModel->index(rowIdx, A_TAR_PCR_MAX),  pair->pcrSizeMaxTarSeqs);
    //}
    //else{
    val.sprintf("%.2f", pair->pctgInTargetSeqs);
    tableModel->setData(tableModel->index(rowIdx, A_TARGET_PCTG),  val);
    tableModel->item(rowIdx, A_TARGET_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    val.sprintf("%.2f", pair->pcrSizeAvgTarSeqs);
    tableModel->setData(tableModel->index(rowIdx, A_TAR_PCR_AVG),  val);
    tableModel->item(rowIdx, A_TAR_PCR_AVG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    tableModel->setData(tableModel->index(rowIdx, A_TAR_PCR_MIN),  pair->pcrSizeMinTarSeqs);
    tableModel->item(rowIdx, A_TAR_PCR_MIN)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    tableModel->setData(tableModel->index(rowIdx, A_TAR_PCR_MAX),  pair->pcrSizeMaxTarSeqs);
    tableModel->item(rowIdx, A_TAR_PCR_MAX)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    //}
    // frequency, average, minimum and maximum product size for non-target sequences
    //if( pair->isEstimateNonTar ){
    //    tableModel->setData(tableModel->index(rowIdx, A_NONTAR_PCTG),  pair->pctgInNonTarSeqs);
    //    tableModel->setData(tableModel->index(rowIdx, A_NONTAR_PCR_AVG),  pair->pcrSizeAvgNonTarSeqs);
    //    tableModel->setData(tableModel->index(rowIdx, A_NONTAR_PCR_MIN),  pair->pcrSizeMinNonTarSeqs);
    //    tableModel->setData(tableModel->index(rowIdx, A_NONTAR_PCR_MAX),  pair->pcrSizeMaxNonTarSeqs);
    //}else{
    val.sprintf("%.2f", pair->pctgInNonTarSeqs);
    tableModel->setData(tableModel->index(rowIdx, A_NONTAR_PCTG),  val);
    tableModel->item(rowIdx, A_NONTAR_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    val.sprintf("%.2f", pair->pcrSizeAvgNonTarSeqs);
    tableModel->setData(tableModel->index(rowIdx, A_NONTAR_PCR_AVG),  val);
    tableModel->item(rowIdx, A_NONTAR_PCR_AVG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    tableModel->setData(tableModel->index(rowIdx, A_NONTAR_PCR_MIN),  pair->pcrSizeMinNonTarSeqs);
    tableModel->item(rowIdx, A_NONTAR_PCR_MIN)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    tableModel->setData(tableModel->index(rowIdx, A_NONTAR_PCR_MAX),  pair->pcrSizeMaxNonTarSeqs);
    tableModel->item(rowIdx, A_NONTAR_PCR_MAX)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    //}

    // left primer GC%
    val.sprintf("%.2f", pair->GCLeft);
    tableModel->setData(tableModel->index(rowIdx, A_LFT_GC),  val);
    tableModel->item(rowIdx, A_LFT_GC)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    // right primer GC%
    val.sprintf("%.2f", pair->GCRight);
    tableModel->setData(tableModel->index(rowIdx, A_RHT_GC),  val);
    tableModel->item(rowIdx, A_RHT_GC)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    // left primer Tm
    val.sprintf("%.2f", pair->tmLeft);
    tableModel->setData(tableModel->index(rowIdx, A_LFT_TM),  val);
    tableModel->item(rowIdx, A_LFT_TM)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    // right primer Tm
    val.sprintf("%.2f", pair->tmRight);
    tableModel->setData(tableModel->index(rowIdx, A_RHT_TM),  val);
    tableModel->item(rowIdx, A_RHT_TM)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    // inter-complementarity
    tableModel->setData(tableModel->index(rowIdx, A_INTER_COMP),  pair->prPrComp);
    tableModel->item(rowIdx, A_INTER_COMP)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    // 3' inter-complementarity
    tableModel->setData(tableModel->index(rowIdx, A_INTER3_COMP),  pair->prPr3Comp);
    tableModel->item(rowIdx, A_INTER3_COMP)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    // self-complementarity
    tableModel->setData(tableModel->index(rowIdx, A_LFT_INTRA_COMP),  pair->selfCompLeft);
    tableModel->item(rowIdx, A_LFT_INTRA_COMP)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    tableModel->setData(tableModel->index(rowIdx, A_RHT_INTRA_COMP),  pair->selfCompRight);
    tableModel->item(rowIdx, A_RHT_INTRA_COMP)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    // 3'self-complementarity
    tableModel->setData(tableModel->index(rowIdx, A_LFT_INTRA3_COMP),  pair->self3CompLeft);
    tableModel->item(rowIdx, A_LFT_INTRA3_COMP)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    tableModel->setData(tableModel->index(rowIdx, A_RHT_INTRA3_COMP),  pair->self3CompRight);
    tableModel->item(rowIdx, A_RHT_INTRA3_COMP)->setData(Qt::AlignRight, Qt::TextAlignmentRole);

    tableModel->setData(tableModel->index(rowIdx, A_NUM_AMBI_TARGETS_LEFT),  pair->numAmbiguousTargetsLeft);
    tableModel->item(rowIdx, A_NUM_AMBI_TARGETS_LEFT)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    tableModel->setData(tableModel->index(rowIdx, A_NUM_AMBI_TARGETS_RIGHT),  pair->numAmbiguousTargetsRight);
    tableModel->item(rowIdx, A_NUM_AMBI_TARGETS_RIGHT)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    tableModel->setData(tableModel->index(rowIdx, A_NUM_AMBI_BASES_LEFT),  pair->numAmbiguousBasesLeft);
    tableModel->item(rowIdx, A_NUM_AMBI_BASES_LEFT)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    tableModel->setData(tableModel->index(rowIdx, A_NUM_AMBI_BASES_RIGHT),  pair->numAmbiguousBasesRight);
    tableModel->item(rowIdx, A_NUM_AMBI_BASES_RIGHT)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    //tableModel->setData(tableModel->index(rowIdx, A_NUM_TAR), tarSeqs.size());
}

void PrimerReportWindow::displayAllColumn()
{
        primerColumnDisp.clear();
        primerColumnHide.clear();

        for(int i = 0; i < primerColumns.size(); i++){
            primerColumnDisp.push_back(i);
            ui->tableView->setColumnHidden(i, false);
        }
        ui->textEdit->clear();
}

void PrimerReportWindow::markselectedprimerpairs()
{
    if( primerPairShowList.size() == 0 )
        return;

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();

    if( selectedRows.size() <= 0 ){
        QMessageBox::about(this, "Empty selected list", "No primer pair is selected.");
        return;
    }


    // track selected primer pairs and set it as marked
    int nSelCount = selectedRows.size();

    if( nSelCount >= 5000 ){
        MyProgressDlg *pDlg = new MyProgressDlg("Marking selected primer pairs",1, nSelCount, this);
        pDlg->setWindowModality(Qt::ApplicationModal);


        for( int i = 0; i < nSelCount; i++ ){
            int rowIdx = selectedRows.at(i).row();
            QStandardItem* __tmpItem =  tableModel->item(rowIdx,0);
            __tmpItem->setCheckState(Qt::Checked);;
            primerPairShowList[rowIdx]->marked = true;
            pDlg->setValue(i + 1);
        }
        delete pDlg;
    }
    else{
        for( int i = 0; i < nSelCount; i++ ){
            int rowIdx = selectedRows.at(i).row();
            QStandardItem* __tmpItem =  tableModel->item(rowIdx,0);
            primerPairShowList[rowIdx]->marked = true;
            __tmpItem->setCheckState(Qt::Checked);
        }
    }
}

void PrimerReportWindow::unmarkselectedprimerpairs()
{
    if( primerPairShowList.size() == 0 )
        return;

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();

    if( selectedRows.size() <= 0 ){
        QMessageBox::about(this, "Empty selected list", "No primer pair is selected.");
        return;
    }

    // track selected primer pairs and set it as unmarked
    int nSelCount = selectedRows.size();

    if( nSelCount >= 5000 ){
        MyProgressDlg *pDlg = new MyProgressDlg("Unmarking selected primer pairs",1, nSelCount, this);
        pDlg->setWindowModality(Qt::ApplicationModal);


        for( int i = 0; i < nSelCount; i++ ){
            int rowIdx = selectedRows.at(i).row();
            QStandardItem* __tmpItem =  tableModel->item(rowIdx,0);
            __tmpItem->setCheckState(Qt::Unchecked);
            primerPairShowList[rowIdx]->marked = false;
            pDlg->setValue(i + 1);
        }
        delete pDlg;
    }
    else{
        for( int i = 0; i < nSelCount; i++ ){
            int rowIdx = selectedRows.at(i).row();
            QStandardItem* __tmpItem =  tableModel->item(rowIdx,0);
            __tmpItem->setCheckState(Qt::Unchecked);
            primerPairShowList[rowIdx]->marked = false;
        }
    }
}

void PrimerReportWindow::dispCompleteInfo()
{
    int range = tarSeqs.size() + nontarSeqs.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Collecting primer pair annealing information",1, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);

    ui->textEdit->clear();

    PrimerPair* pair =primerPairShowList[dispPrimerSel];
    QString currPrimer = pair->left + ".." + pair->rightRevsComp;

    //int pLen = pair->left.length();

    /*short *lprimer = new short[pLen+1];

    for( int i = 0; i < pLen; i++ ){
        lprimer[i] = tools::base2int(pair->left[i]);
    }
    lprimer[pLen] = -1;

    pLen = pair->right.length();
    short *rprimer = new short[pLen+1];
    for( int i = 0; i < pLen; i++ ){
        rprimer[i] = tools::base2int(pair->right[i]);
    }
    rprimer[pLen] = -1;*/

    /*PrimerSearch* lft;
    PrimerSearch* rht;
    try{
        lft = new PrimerSearch(lprimer, LEFT_PRIMER);
        rht = new PrimerSearch(rprimer, RIGHT_PRIMER);
        delete[] lprimer;	delete[] rprimer;
    }
    catch( std::bad_alloc& e ){
        QMessageBox::about(this,"Fatal Error","Fatal error: out of memory for primer searching.");
        dispPrimerSel = -1;
        delete pDlg;
        return;
    }*/

    int numTarSeqs = tarSeqs.size();
    int numNontarSeqs = nontarSeqs.size();

    int num;
    int maxProductSize;
    int minProductSize;
    int sumProductSize;
    int productSize;
    int bdPos[4];
    QString str, posS, posE;

    // copy target and non-target sequence names
    QStringList tmpTarSeqNames, tmpNontarSeqNames;
    for( int i = 0; i < numTarSeqs; i++ ){
        str = tarSeqNames[i].left(70);
        while( str.length()< 70 )
            str.insert(70, ' ');
        tmpTarSeqNames.push_back(str);
    }
    for( int i = 0; i < numNontarSeqs; i++ ){
        str = nontarSeqNames[i].left(70);
        while( str.length()< 70 )
            str.insert(70, ' ');
        tmpNontarSeqNames.push_back(str);
    }

    ui->textEdit->insertPlainText("Primer annealing position information for primer pair -- " + currPrimer + "\r\n\r\n");
    ui->textEdit->insertPlainText("Target sequences:\r\n");
    str = "";
    while( str.length() < 70 )
            str.insert(70, ' ');
    ui->textEdit->insertPlainText(str+"\t\tFw primer\t\tRv primer\t\tPCR size\r\n");


    // compute frequencies for this primer pair in target sequences
    num = 0;
    maxProductSize = 0;	minProductSize = INT_MAX;	sumProductSize = 0;
    for( int i = 0; i < numTarSeqs; i++ ){

        // search left and right primers in target sequence
        /*lft->appxSearchTarSeqs(tarSeqs[i], LEFT_PRIMER, bdPos[0], bdPos[1]);
        rht->appxSearchTarSeqs(tarSeqs[i], RIGHT_PRIMER, bdPos[2], bdPos[3]);*/

        // left and right primer both appear and positions are correct
        bdPos[1] = pair->posBindingTar[0][i];
        bdPos[2] = pair->posBindingTar[1][i];

        bdPos[0] = bdPos[1] > -1 ? bdPos[1]- pair->left.length()+1:-1;
        bdPos[3] = bdPos[2] > -1 ? bdPos[2]+ pair->right.length()-1:-1;

        if( bdPos[0] >= 0 && bdPos[2] >=0 && bdPos[1] < bdPos[2] ){
            num++;
            productSize = bdPos[3] - bdPos[0] + 1;
            if( minProductSize > productSize )	minProductSize = productSize;
            if( maxProductSize < productSize )	maxProductSize = productSize;
            sumProductSize += productSize;
        }
        ui->textEdit->insertPlainText(tmpTarSeqNames[i]+"\t\t");
        if( bdPos[0] < 0 )
             str = "   -  \t\t";
        else{
            posS = QString("%1").arg(bdPos[0]+1);
            posE = QString("%1").arg(bdPos[1]+1);
            str = posS + "-" + posE + "\t\t";
            //if( str.length() < 7 )
            //    str += "\t";
        }
        ui->textEdit->insertPlainText(str);
        if( bdPos[2] < 0 )
            str = "   -  \t\t";
        else{
            posS = QString("%1").arg(bdPos[2]+1);
            posE = QString("%1").arg(bdPos[3]+1);
            str = posS + "-" + posE + "\t\t";
            //if( str.length() < 7 )
            //    str += "\t";
        }
        ui->textEdit->insertPlainText(str);
        if( bdPos[0] < 0 || bdPos[2] < 0 || bdPos[1] >= bdPos[2] )
             str = "-";
        else
            str = QString("%1").arg( bdPos[3] - bdPos[0] + 1);
        ui->textEdit->insertPlainText(str+"\n");

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



    ui->textEdit->insertPlainText("\r\n\r\nNon-target sequences:\r\n");
    str = "";
    while( str.length() < 70 )
        str.insert(70, ' ');
    ui->textEdit->insertPlainText(str+"\t\tFw primer\t\tRv primer\t\tPCR size\r\n");

    // compute frequencies for this primer pair in non-target sequences
    num = 0;
    maxProductSize = 0;	minProductSize = INT_MAX;	sumProductSize = 0;
    for( int i = 0; i < numNontarSeqs; i++ ){

        // search left and right primers in target sequence
        /*lft->appxSearchNonTarSeqs(nontarSeqs[i], LEFT_PRIMER, bdPos[0], bdPos[1]);
        rht->appxSearchNonTarSeqs(nontarSeqs[i], RIGHT_PRIMER, bdPos[2], bdPos[3]);*/

        // left and right primer both appear and positions are correct
        bdPos[1] = pair->posBindingNontar[0][i];
        bdPos[2] = pair->posBindingNontar[1][i];

        bdPos[0] = bdPos[1] > -1 ? bdPos[1]- pair->left.length()+1:-1;
        bdPos[3] = bdPos[2] > -1 ? bdPos[2]+ pair->right.length()-1:-1;

        if( bdPos[0] >= 0 && bdPos[2] >=0 && bdPos[1] < bdPos[2] ){
            num++;
            productSize = bdPos[3] - bdPos[0] + 1;
            if( minProductSize > productSize )	minProductSize = productSize;
            if( maxProductSize < productSize )	maxProductSize = productSize;
            sumProductSize += productSize;
        }
        ui->textEdit->insertPlainText(tmpNontarSeqNames[i]+"\t\t");
        if( bdPos[0] < 0 )
            str = "   -  \t\t";
        else{
            posS = QString("%1").arg(bdPos[0]+1);
            posE = QString("%1").arg(bdPos[1]+1);
            str = posS + "-" + posE + "\t\t";
            //if( str.length() < 7 )
            //    str += "\t";
        }
        ui->textEdit->insertPlainText(str);
        if( bdPos[2] < 0 )
            str = "   -  \t\t";
        else{
            posS = QString("%1").arg(bdPos[2]+1);
            posE = QString("%1").arg(bdPos[3]+1);
            str = posS + "-" + posE + "\t\t";
            //if( str.length() < 7 )
            //    str += "\t";
        }
        ui->textEdit->insertPlainText(str);
        if( bdPos[0] < 0 || bdPos[2] < 0 || bdPos[1] >= bdPos[2] )
            str = "-";
        else
            str = QString("%1").arg(bdPos[3] - bdPos[0] + 1);
        ui->textEdit->insertPlainText(str+"\n");

        pDlg->setValue(numTarSeqs+i);
    }
    if( numNontarSeqs != 0 )
        pair->pctgInNonTarSeqs = (float)num / numNontarSeqs * 100;
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

    //pair->isEstimateTar = 0;
    //pair->isEstimateNonTar = 0;


    ui->textEdit->insertPlainText("\r\n\r\nFinish Listing of primer-sequence annealing information.");


    //delete lft;
    //delete rht;

    // update frequency, average, minimum and maximum product size to exact value
    str = QString("%1").arg(pair->pctgInTargetSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel,A_TARGET_PCTG),str + "%");
    str = QString("%1").arg(pair->pcrSizeAvgTarSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel,A_TAR_PCR_AVG), str);
    str = QString("%1").arg(pair->pcrSizeMinTarSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel,A_TAR_PCR_MIN), str);
    str = QString("%1").arg(pair->pcrSizeMaxTarSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel, A_TAR_PCR_MAX), str);

    str = QString("%1").arg(pair->pctgInNonTarSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel, A_NONTAR_PCTG), str + "%");
    str = QString("%1").arg(pair->pcrSizeAvgNonTarSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel, A_NONTAR_PCR_AVG), str);
    str = QString("%1").arg(pair->pcrSizeMinNonTarSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel, A_NONTAR_PCR_MIN), str);
    str = QString("%1").arg(pair->pcrSizeMaxNonTarSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel, A_NONTAR_PCR_MAX), str);

    dispPrimerSel = -1;
    delete pDlg;
}

// return starting annealing position of primer in sequence
// if not anneal, return -1;
/*int PrimerReportWindow::searchPrimerInNonbindingSeq(short *primer, short *sequence, int lft_rht)
{
    int score;
    int bestScore = 10000, bestPos = 0;
    int pLen = 0, sLen = 0;
    int nBaseFrom5end, nBaseFrom3end, minMatches5end, minMatches3end, nMatch;
    int i, j;


    // get primer length and sequence length
    while( primer[pLen] >= 0 )      pLen++;
    while( sequence[sLen] >= 0 )    sLen++;
    // get max mismatch number of 5' end and 3' end, respectively
    nBaseFrom3end = bindingContentDivider3end;
    nBaseFrom5end = pLen > nBaseFrom3end ? ( pLen - nBaseFrom3end ) : 0;
    minMatches3end = (int) ( bindingContentMatchPctg3end / 100 * nBaseFrom3end );
    minMatches5end = (int) ( bindingContentMatchPctg5end / 100 * nBaseFrom5end );


    // start searching

    if( lft_rht == LEFT_PRIMER ){	// left primer: get left most annealing position
        for( i = 0; i <= sLen - pLen; i++ ){
            score = 0;

            for( j = 0; j < pLen; j++ )
                if( sequence[i+j] != primer[j] )    score++;

            // record best score even seen
            if( bestScore > score ){
                bestScore = score;
                bestPos = i;
            }
        }


        // check if user's constraints of annealing content for non-annealing sequences are satisfied
        // left primer: left part (base 0) is 5' end, check 5' end first
        nMatch = 0;
        for( i = 0, j = bestPos; i < nBaseFrom5end && j < sLen; i++, j++ )
            if( primer[i] == sequence[j] )	nMatch++;
        if( nMatch < minMatches5end )	return -1;
        // check 3' end next
        nMatch = 0;
        while( i < pLen && j < sLen )
            if( primer[i++] == sequence[j++] )	nMatch++;
        if( nMatch < minMatches3end )	return -1;
    }
    else{	// right primer: get right most annealing position

        for( i = 0; i <= sLen - pLen; i++ ){
            score = 0;

            for( j = 0; j < pLen; j++ )
                if( sequence[i+j] != primer[j] )	score++;

            // record best score even seen
            if( bestScore >= score ){
                bestScore = score;
                bestPos = i;
            }
        }


        // check if user's constraints of annealing content for non-annealing sequences are satisfied
        // right primer: left part (base 0) is 3' end, check 3' end first
        nMatch = 0;
        for( i = 0, j = bestPos; i < nBaseFrom3end && j < sLen; i++, j++ )
            if( primer[i] == sequence[j] )	nMatch++;
        if( nMatch < minMatches3end )	return -1;
        // check 5' end next
        nMatch = 0;
        while( i < pLen && j < sLen )
            if( primer[i++] == sequence[j++] )	nMatch++;
        if( nMatch < minMatches5end )	return -1;
    }
    return bestPos;	// return the most possible annealing position
}*/

void PrimerReportWindow::reportBindingContent()
{
    AnnealInfoDlg annInfoDlg(&bindingContentDivider3end, &bindingContentMatchPctg5end, &bindingContentMatchPctg3end);
    if( !annInfoDlg.exec() )
        return;

    ui->textEdit->clear();

    int range = tarSeqs.size() + nontarSeqs.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Collecting primer pair annealing information",1, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);



    // get primer sequence
    PrimerPair* pair = primerPairShowList[dispPrimerSel];

    QString lprimerStr = pair->left,
            rprimerStr = pair->rightRevsComp;

    int pLen = lprimerStr.length();
    short *lprimer = new short[pLen+1];
    for( int i = 0; i < pLen; i++ )
        lprimer[i] = tools::base2int(lprimerStr[i]);
    lprimer[pLen] = -1;

    pLen = pair->right.length();
    short *rprimer = new short[pLen+1];
    for( int i = 0; i < pLen; i++ )
        rprimer[i] = tools::base2int(pair->right[i]);
    rprimer[pLen] = -1;

    /*PrimerSearch* lft;
    PrimerSearch* rht;
    try{
        lft = new PrimerSearch(lprimer, LEFT_PRIMER);
        rht = new PrimerSearch(rprimer, RIGHT_PRIMER);
    }
    catch(std::bad_alloc& e){
        QMessageBox::about(this,"Fatal error","Fatal error: out of memory for primer searching.");
        delete lprimer;	delete rprimer;
        delete pDlg;
        return;
    }*/


    // compute and display final result
    ui->textEdit->insertPlainText("Percentage of each nucleotide in target and non-target sequences in relation to primer sequences information for primer pair -- " +
                                            pair->left + ".." + pair->rightRevsComp + "\r\n");
    int numTarSeqs = tarSeqs.size();
    int numNontarSeqs = nontarSeqs.size();
    QString str;
    str = QString("%1").arg(numTarSeqs);
    ui->textEdit->insertPlainText(str + " target sequences and ");
    str = QString("%1").arg(numNontarSeqs);
    ui->textEdit->insertPlainText(str + " non-target sequences.\r\n\r\n\r\n");

    QVector<short*> seqPtrBindLeft;
    QVector<short*> seqPtrNonBindLeft;
    QVector<short*> seqPtrBindRight;
    QVector<short*> seqPtrNonBindRight;
    short *seqPtr;
    float percent;
    int num;
    int maxProductSize;
    int minProductSize;
    int sumProductSize;
    int productSize;
    int pos[4];



// -----------------------target sequences--------------------------------------------------
    num = 0;
    maxProductSize = 0;	minProductSize = INT_MAX;	sumProductSize = 0;

    for( int i = 0; i < numTarSeqs; i++ ){

        seqPtr = tarSeqs[i];

        // search left and right primers in target sequence
        /*lft->appxSearchTarSeqs(seqPtr, LEFT_PRIMER, pos[0], pos[1]);
        rht->appxSearchTarSeqs(seqPtr, RIGHT_PRIMER, pos[2], pos[3]);*/

        // primer pair anneal with this sequence
        pos[1] = pair->posBindingTar[0][i];
        pos[2] = pair->posBindingTar[1][i];

        pos[0] = pos[1] > -1 ? pos[1]- pair->left.length()+1:-1;
        pos[3] = pos[2] > -1 ? pos[2]+ pair->right.length()-1:-1;

        if( pos[0] >= 0 && pos[1] < pos[2] ){

            short *bindPtrLeft = seqPtr;
            bindPtrLeft += pos[0];
            seqPtrBindLeft.push_back(bindPtrLeft);

            short *bindPtrRight = seqPtr;
            bindPtrRight += pos[2];
            seqPtrBindRight.push_back(bindPtrRight);

            // compute pcr info
            num++;
            productSize = pos[3] - pos[0] + 1;
            if( minProductSize > productSize )	minProductSize = productSize;
            if( maxProductSize < productSize )	maxProductSize = productSize;
            sumProductSize += productSize;
        }
        else{ // primer pair doesn't anneal with this sequence

            // search for best match
            pos[0] = DataTools::searchPrPbInNonbindingSeq(lprimer, seqPtr, LEFT_PRIMER);
            pos[2] = DataTools::searchPrPbInNonbindingSeq(rprimer, seqPtr, RIGHT_PRIMER);

            // if the annealing order is right: record annealing info for non_binding sequence
            if( pos[0] >= 0 && pos[2] > 0 && pos[0] + lprimerStr.length() < pos[2] ){
                short *bindPtrLeft = seqPtr;
                bindPtrLeft += pos[0];
                seqPtrNonBindLeft.push_back(bindPtrLeft);

                short *bindPtrRight = seqPtr;
                bindPtrRight += pos[2];
                seqPtrNonBindRight.push_back(bindPtrRight);
            }
        }
        pDlg->setValue(i);
    }

    // update annealing information for those estimated info
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


    // output the primer sequences : annealing sequences
    ui->textEdit->insertPlainText("Information for target sequences annealing with primer pair: (");
    str = QString("%1").arg(seqPtrBindLeft.size() );
    ui->textEdit->insertPlainText(str + " sequences)\r\n\r\n");

    // cout primer sequences
    for( int i = 0; i < lprimerStr.length(); i++ ){
        str = lprimerStr.mid(i, 1);
        ui->textEdit->insertPlainText("\t"+str);
    }
    ui->textEdit->insertPlainText("\t..");
    for( int i = 0; i < rprimerStr.length(); i++ ){
        str = rprimerStr.mid(i, 1);
        ui->textEdit->insertPlainText("\t"+str);
    }
    ui->textEdit->insertPlainText("\r\n");


    for( int base = BASE_A; base <= BASE_T; base++ ){
        str = "";
        str += tools::int2base(base);
        ui->textEdit->insertPlainText(str);

        // left primer annealing region
        for( int i = 0; i < lprimerStr.length(); i++ ){
            percent = 0;
            for( int j = 0; j < seqPtrBindLeft.size(); j++ ){
                if( seqPtrBindLeft[j][i] == base )
                    percent++;
            }
            if( seqPtrBindLeft.size() > 0 )
                percent =  percent / (float)seqPtrBindLeft.size() * 100;
            else
                percent = 0;
            str = QString("%1").arg(percent);
            ui->textEdit->insertPlainText("\t"+str+"%");
        }

        ui->textEdit->insertPlainText("\t..");

        // right primer annealing region
        for( int i = rprimerStr.length() - 1; i >= 0; i-- ){
            percent = 0;
            for( int j = 0; j < seqPtrBindRight.size(); j++ ){
                if( seqPtrBindRight[j][i] == 3-base ) // 3 - base: make it reverse complement for reverse primer
                    percent++;
            }
            if( seqPtrBindRight.size() > 0 )
                percent =  percent / (float)seqPtrBindRight.size() * 100;
            else
                percent = 0;
            str = QString("%1").arg(percent);
            ui->textEdit->insertPlainText("\t"+str+"%");
        }

        ui->textEdit->insertPlainText("\r\n");
    }


    // non-annealing sequences
    ui->textEdit->insertPlainText("\r\n\r\nInformation for target sequences not annealing with primer pair: (");
    str = QString("%1").arg(seqPtrNonBindLeft.size() );
    ui->textEdit->insertPlainText(str + " sequences)\r\n\r\n");

    // cout primer sequences
    for( int i = 0; i < lprimerStr.length(); i++ ){
        str = lprimerStr.mid(i, 1);
        ui->textEdit->insertPlainText("\t"+str);
    }
    ui->textEdit->insertPlainText("\t..");
    for( int i = 0; i < rprimerStr.length(); i++ ){
        str = rprimerStr.mid(i, 1);
        ui->textEdit->insertPlainText("\t"+str);
    }
    ui->textEdit->insertPlainText("\r\n");


    for( int base = BASE_A; base <= BASE_T; base++ ){
        str = "";
        str += tools::int2base(base);
        ui->textEdit->insertPlainText(str);

        // left primer annealing region
        for( int i = 0; i < lprimerStr.length(); i++ ){
            percent = 0;
            for( int j = 0; j < seqPtrNonBindLeft.size(); j++ ){
                if( seqPtrNonBindLeft[j][i] == base )
                    percent++;
            }
            if( seqPtrNonBindLeft.size() > 0 )
                percent =  percent / (float)seqPtrNonBindLeft.size() * 100;
            else
                percent = 0;
            str = QString("%1").arg(percent);
            ui->textEdit->insertPlainText("\t"+str+"%");
        }

        ui->textEdit->insertPlainText("\t..");

        // right primer annealing region
        for( int i = rprimerStr.length() - 1; i >= 0; i-- ){
            percent = 0;
            for( int j = 0; j < seqPtrNonBindRight.size(); j++ ){
                if( seqPtrNonBindRight[j][i] == 3-base ) // 3 - base: make it reverse complement for reverse primer
                    percent++;
            }
            if( seqPtrNonBindRight.size() > 0 )
                percent =  percent / (float)seqPtrNonBindRight.size() * 100;
            else
                percent = 0;
            str = QString("%1").arg(percent);
            ui->textEdit->insertPlainText("\t"+str+"%");
        }

        ui->textEdit->insertPlainText("\r\n");
    }



// -------------non target sequences--------------------------------------------------
    seqPtrBindLeft.clear();
    seqPtrNonBindLeft.clear();
    seqPtrBindRight.clear();
    seqPtrNonBindRight.clear();

    num = 0;
    maxProductSize = 0;	minProductSize = INT_MAX;	sumProductSize = 0;

    for( int i = 0; i < numNontarSeqs; i++ ){

        seqPtr = nontarSeqs[i];

        // search left and right primers in non-target sequence
        //lft->appxSearchNonTarSeqs(seqPtr, LEFT_PRIMER, pos[0], pos[1]);
        //rht->appxSearchNonTarSeqs(seqPtr, RIGHT_PRIMER, pos[2], pos[3]);
        pos[1] = pair->posBindingNontar[0][i];
        pos[2] = pair->posBindingNontar[1][i];

        pos[0] = pos[1] > -1 ? pos[1]- pair->left.length()+1:-1;
        pos[3] = pos[2] > -1 ? pos[2]+ pair->right.length()-1:-1;
        // primer pair anneal with this sequence
        if( pos[0] >= 0 && pos[1] < pos[2] ){

            short *bindPtrLeft = seqPtr;
            bindPtrLeft += pos[0];
            seqPtrBindLeft.push_back(bindPtrLeft);

            short *bindPtrRight = seqPtr;
            bindPtrRight += pos[2];
            seqPtrBindRight.push_back(bindPtrRight);

            // compute pcr info
            num++;
            productSize = pos[3] - pos[0] + 1;
            if( minProductSize > productSize )	minProductSize = productSize;
            if( maxProductSize < productSize )	maxProductSize = productSize;
            sumProductSize += productSize;
        }
        else{ // primer pair doesn't anneal with this sequence

            // search for best match
            pos[0] = DataTools::searchPrPbInNonbindingSeq(lprimer, seqPtr, LEFT_PRIMER);
            pos[2] = DataTools::searchPrPbInNonbindingSeq(rprimer, seqPtr, RIGHT_PRIMER);

            // if the annealing order is right: record annealing info for non_binding sequence
            if( pos[0] >= 0 && pos[2] > 0 && pos[0] + lprimerStr.length() < pos[2] ){
                short *bindPtrLeft = seqPtr;
                bindPtrLeft += pos[0];
                seqPtrNonBindLeft.push_back(bindPtrLeft);

                short *bindPtrRight = seqPtr;
                bindPtrRight += pos[2];
                seqPtrNonBindRight.push_back(bindPtrRight);
            }
        }
        pDlg->setValue(numTarSeqs+i);
    }

    // update annealing infomation for estimated info
    if( numNontarSeqs != 0 )
        pair->pctgInNonTarSeqs = (float)num / numNontarSeqs * 100;
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

    //pair->isEstimateTar = 0;
    //pair->isEstimateNonTar = 0;

    // output the primer sequences : annealing sequences
    ui->textEdit->insertPlainText("\r\n\r\nInformation for non-target sequences annealing with primer pair: (");
    str = QString("%1").arg(seqPtrBindLeft.size() );
    ui->textEdit->insertPlainText(str + " sequences)\r\n\r\n");

    // cout primer sequences
    for( int i = 0; i < lprimerStr.length(); i++ ){
        str = lprimerStr.mid(i, 1);
        ui->textEdit->insertPlainText("\t"+str);
    }
    ui->textEdit->insertPlainText("\t..");
    for( int i = 0; i < rprimerStr.length(); i++ ){
        str = rprimerStr.mid(i, 1);
        ui->textEdit->insertPlainText("\t"+str);
    }
    ui->textEdit->insertPlainText("\r\n");


    for( int base = BASE_A; base <= BASE_T; base++ ){
        str = "";
        str += tools::int2base(base);
        ui->textEdit->insertPlainText(str);

        // left primer annealing region
        for(int i = 0; i < lprimerStr.length(); i++ ){
            percent = 0;
            for(int j = 0; j < seqPtrBindLeft.size(); j++ ){
                if( seqPtrBindLeft[j][i] == base )
                    percent++;
            }
            if( seqPtrBindLeft.size() > 0 )
                percent =  percent / static_cast<float>(seqPtrBindLeft.size()) * 100.0;
            else
                percent = 0;
            str = QString("%1").arg(percent);
            ui->textEdit->insertPlainText("\t"+str+"%");
        }

        ui->textEdit->insertPlainText("\t..");

        // right primer annealing region
        for( int i = rprimerStr.length() - 1; i >= 0; i-- ){
            percent = 0;
            for( int j = 0; j < seqPtrBindRight.size(); j++ ){
                if( seqPtrBindRight[j][i] == 3-base ) // 3 - base: make it reverse complement for reverse primer
                    percent++;
            }
            if( seqPtrBindRight.size() > 0 )
                percent =  percent / static_cast<float>(seqPtrBindRight.size()) * 100.0;
            else
                percent = 0;
            str = QString("%1").arg(percent);
            ui->textEdit->insertPlainText("\t"+str+"%");
        }

        ui->textEdit->insertPlainText("\r\n");
    }


    // non-annealing sequences
    ui->textEdit->insertPlainText("\r\n\r\nInformation for non-target sequences not annealing with primer pair: (");
    str = QString("%1").arg(seqPtrNonBindLeft.size() );
    ui->textEdit->insertPlainText(str + " sequences)\r\n\r\n");

    // cout primer sequences
    for( int i = 0; i < lprimerStr.length(); i++ ){
        str = lprimerStr.mid(i, 1);
        ui->textEdit->insertPlainText("\t"+str);
    }
    ui->textEdit->insertPlainText("\t..");
    for( int i = 0; i < rprimerStr.length(); i++ ){
        str = rprimerStr.mid(i, 1);
        ui->textEdit->insertPlainText("\t"+str);
    }
    ui->textEdit->insertPlainText("\r\n");


    for( int base = BASE_A; base <= BASE_T; base++ ){
        str = "";
        str += tools::int2base(base);
        ui->textEdit->insertPlainText(str);

        // left primer annealing region
        for( int i = 0; i < lprimerStr.length(); i++ ){
            percent = 0;
            for( int j = 0; j < seqPtrNonBindLeft.size(); j++ ){
                if( seqPtrNonBindLeft[j][i] == base )
                    percent++;
            }
            if( seqPtrNonBindLeft.size() > 0 )
                percent =  percent / (float)seqPtrNonBindLeft.size() * 100;
            else
                percent = 0;
            str = QString("%1").arg(percent);
            ui->textEdit->insertPlainText("\t"+str+"%");
        }

        ui->textEdit->insertPlainText("\t..");

        // right primer annealing region
        for( int i = rprimerStr.length() - 1; i >= 0; i-- ){
            percent = 0;
            for( int j = 0; j < seqPtrNonBindRight.size(); j++ ){
                if( seqPtrNonBindRight[j][i] == 3-base ) // 3 - base: make it reverse complement for reverse primer
                    percent++;
            }
            if( seqPtrNonBindRight.size() > 0 )
                percent =  percent / (float)seqPtrNonBindRight.size() * 100;
            else
                percent = 0;
            str = QString("%1").arg(percent);
            ui->textEdit->insertPlainText("\t"+str+"%");
        }

        ui->textEdit->insertPlainText("\r\n");
    }



// -------------update frequency, average, minimum and maximum product size to exact value------------
    //int nItem = dispPrimerSel;
    str = QString("%1").arg(pair->pctgInTargetSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel, A_TARGET_PCTG), str + "%");
    str = QString("%1").arg(pair->pcrSizeAvgTarSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel, A_TAR_PCR_AVG), str);
    str = QString("%1").arg(pair->pcrSizeMinTarSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel, A_TAR_PCR_MIN), str);
    str = QString("%1").arg(pair->pcrSizeMaxTarSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel, A_TAR_PCR_MAX), str);

    str = QString("%1").arg(pair->pctgInNonTarSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel, A_NONTAR_PCTG), str + "%");
    str = QString("%1").arg(pair->pcrSizeAvgNonTarSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel, A_NONTAR_PCR_AVG), str);
    str = QString("%1").arg(pair->pcrSizeMinNonTarSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel, A_NONTAR_PCR_MIN), str);
    str = QString("%1").arg(pair->pcrSizeMaxNonTarSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel, A_NONTAR_PCR_MAX), str);

// ------------house keeping clean---------------------------------------------
    delete[] lprimer;	delete[] rprimer;
    //delete lft;			delete rht;
    dispPrimerSel = -1;
    delete pDlg;
}

void PrimerReportWindow::appendSeq2Rich(QString seqName, short *seqInt, int *pos)
{
    // change seqInt to CString
    QString sequence;
    tools::seqInt2Str(seqInt,sequence);

    ui->textEdit->append(seqName);
    //int len = ui->textEdit->toPlainText().length();
    // primer appear in sequence, highlight primer pair
    if(pos[2] >= 0)
        sequence.replace(pos[2], pos[3]-pos[2]+1, "<span style=\" color:#ff0000;\">"+sequence.mid(pos[2], pos[3]-pos[2]+1)+"</span>");
    if(pos[0] >= 0)
        sequence.replace(pos[0], pos[1]-pos[0]+1, "<span style=\" color:#ff0000;\">"+sequence.mid(pos[0], pos[1]-pos[0]+1)+"</span>");

    sequence = "<span style=\" color:#000000;\">"+sequence +"</span>";

    ui->textEdit->append(sequence);
    ui->textEdit->append("");
}

void PrimerReportWindow::targetWPrimer()
{
    int pos[4];
    ui->textEdit->clear();
    int range = tarSeqs.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Collecting primer pair annealing information",1, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);
    PrimerPair *pr = primerPairShowList[dispPrimerSel];

    // get primer sequence
    /*int pLen = primerPairShowList[dispPrimerSel]->left.length();
    short *lprimer = new short[pLen+1];
    for( int i = 0; i < pLen; i++ )
        lprimer[i] = tools::base2int(primerPairShowList[dispPrimerSel]->left[i]);
    lprimer[pLen] = -1;

    pLen = primerPairShowList[dispPrimerSel]->right.length();
    short *rprimer = new short[pLen+1];
    for( int i = 0; i < pLen; i++ )
        rprimer[i] = tools::base2int(primerPairShowList[dispPrimerSel]->right[i]);
    rprimer[pLen] = -1;

    PrimerSearch* lft;
    PrimerSearch* rht;
    try{
        lft = new PrimerSearch(lprimer, LEFT_PRIMER);
        rht = new PrimerSearch(rprimer, RIGHT_PRIMER);
        delete lprimer;	delete rprimer;
    }
    catch(std::bad_alloc& e){
        QMessageBox::about(this,"Fatal error","Fatal error: out of memory for primer searching.");
        delete pDlg;
        return;
    }*/



    for(int i = 0; i < tarSeqs.size(); i++ ){
        pos[1] = pr->posBindingTar[0][i];
        pos[2] = pr->posBindingTar[1][i];

        pos[0] = pos[1] > -1 ? pos[1]- pr->left.length()+1:-1;
        pos[3] = pos[2] > -1 ? pos[2]+ pr->right.length()-1:-1;

        // search left and right primers in target sequence
        //lft->appxSearchTarSeqs(tarSeqs[i], LEFT_PRIMER, pos[0], pos[1]);
        //rht->appxSearchTarSeqs(tarSeqs[i], RIGHT_PRIMER, pos[2], pos[3]);

        // only add those sequence having primer pairs
        if( pos[0] >= 0 && pos[1] < pos[2] )
            appendSeq2Rich(tarSeqNames[i], tarSeqs[i], pos);
        pDlg->setValue(i+1);
    }

    //delete lft;
    //delete rht;
    dispPrimerSel = -1;
    delete pDlg;
}

void PrimerReportWindow::targetWoPrimer()
{
    int pos[4];

    ui->textEdit->clear();
    int range = tarSeqs.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Collecting primer pair annealing information",1, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);
    PrimerPair *pr = primerPairShowList[dispPrimerSel];


    // get primer sequence
    /*int pLen = primerPairShowList[dispPrimerSel]->left.length();
    short *lprimer = new short[pLen+1];
    for( int i = 0; i < pLen; i++ )
        lprimer[i] = tools::base2int(primerPairShowList[dispPrimerSel]->left[i]);
    lprimer[pLen] = -1;

    pLen = primerPairShowList[dispPrimerSel]->right.length();
    short *rprimer = new short[pLen+1];
    for( int i = 0; i < pLen; i++ )
        rprimer[i] = tools::base2int(primerPairShowList[dispPrimerSel]->right[i]);
    rprimer[pLen] = -1;

    PrimerSearch* lft;
    PrimerSearch* rht;
    try{
        lft = new PrimerSearch(lprimer, LEFT_PRIMER);
        rht = new PrimerSearch(rprimer, RIGHT_PRIMER);
        delete lprimer;	delete rprimer;
    }
    catch( std::bad_alloc& e){
        QMessageBox::about(this,"Fatal error","Fatal error: out of memory for primer searching.");
        delete pDlg;
        return;
    }*/


    for(int i = 0; i < tarSeqs.size(); i++ ){
        pos[1] = pr->posBindingTar[0][i];
        pos[2] = pr->posBindingTar[1][i];

        pos[0] = pos[1] > -1 ? pos[1]- pr->left.length()+1:-1;
        pos[3] = pos[2] > -1 ? pos[2]+ pr->right.length()-1:-1;

        // search left and right primers in target sequence
        //lft->appxSearchTarSeqs(tarSeqs[i], LEFT_PRIMER, pos[0], pos[1]);
        //rht->appxSearchTarSeqs(tarSeqs[i], RIGHT_PRIMER, pos[2], pos[3]);

        // only add those sequences not having primer pairs
        if( pos[0] < 0 || pos[2] < 0 || pos[1] >= pos[2] ){
            pos[0] = -1;
            appendSeq2Rich(tarSeqNames[i], tarSeqs[i], pos);
        }
        pDlg->setValue(i+1);
    }

    //delete lft;
    //delete rht;
    dispPrimerSel = -1;
    delete pDlg;
}

void PrimerReportWindow::nonTargetWPrimer()
{
    int pos[4];
    ui->textEdit->clear();
    int range = nontarSeqs.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Collecting primer pair annealing information",1, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);
    PrimerPair *pr = primerPairShowList[dispPrimerSel];

    // get primer sequence
    /*int pLen = primerPairShowList[dispPrimerSel]->left.length();
    short *lprimer = new short[pLen+1];
    for( int i = 0; i < pLen; i++ )
        lprimer[i] = tools::base2int(primerPairShowList[dispPrimerSel]->left[i]);
    lprimer[pLen] = -1;

    pLen = primerPairShowList[dispPrimerSel]->right.length();
    short *rprimer = new short[pLen+1];
    for( int i = 0; i < pLen; i++ )
        rprimer[i] = tools::base2int(primerPairShowList[dispPrimerSel]->right[i]);
    rprimer[pLen] = -1;

    PrimerSearch* lft;
    PrimerSearch* rht;
    try{
        lft = new PrimerSearch(lprimer, LEFT_PRIMER);
        rht = new PrimerSearch(rprimer, RIGHT_PRIMER);
        delete lprimer;	delete rprimer;
    }
    catch( std::bad_alloc& e ){
            QMessageBox::about(this,"Fatal error","Fatal error: out of memory for primer searching.");
            delete pDlg;
            return;
    }*/

    // start searching
    for(int i = 0; i < nontarSeqs.size(); i++ ){
        pos[1] = pr->posBindingNontar[0][i];
        pos[2] = pr->posBindingNontar[1][i];

        pos[0] = pos[1] > -1 ? pos[1]- pr->left.length()+1:-1;
        pos[3] = pos[2] > -1 ? pos[2]+ pr->right.length()-1:-1;

        // search left and right primers in non-target sequence
        //lft->appxSearchNonTarSeqs(nontarSeqs[i], LEFT_PRIMER, pos[0], pos[1]);
        //rht->appxSearchNonTarSeqs(nontarSeqs[i], RIGHT_PRIMER, pos[2], pos[3]);

        // only add those sequence having primer pairs
        if( pos[0] >= 0 && pos[1] < pos[2] )
            appendSeq2Rich(nontarSeqNames[i], nontarSeqs[i], pos);

        pDlg->setValue(i+1);
    }

    //delete lft;
    //delete rht;
    dispPrimerSel = -1;
    delete pDlg;
}

void PrimerReportWindow::nonTargetWoPrimer()
{
    int pos[4];
    ui->textEdit->clear();
    int range = nontarSeqs.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Collecting primer pair annealing information",1, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);
    PrimerPair *pr = primerPairShowList[dispPrimerSel];

    // get primer sequence
    /*int pLen = primerPairShowList[dispPrimerSel]->left.length();
    short *lprimer = new short[pLen+1];
    for( int i = 0; i < pLen; i++ )
        lprimer[i] = tools::base2int(primerPairShowList[dispPrimerSel]->left[i]);
    lprimer[pLen] = -1;

    pLen = primerPairShowList[dispPrimerSel]->right.length();
    short *rprimer = new short[pLen+1];
    for( int i = 0; i < pLen; i++ )
        rprimer[i] = tools::base2int(primerPairShowList[dispPrimerSel]->right[i]);
    rprimer[pLen] = -1;

    PrimerSearch* lft;
    PrimerSearch* rht;
    try{
        lft = new PrimerSearch(lprimer, LEFT_PRIMER);
        rht = new PrimerSearch(rprimer, RIGHT_PRIMER);
        delete lprimer;	delete rprimer;
    }
    catch( std::bad_alloc& e ){
        QMessageBox::about(this,"Fatal error","Fatal error: out of memory for primer searching.");
        delete pDlg;
        return;
    }*/


    // start searching
    for(int i = 0; i < nontarSeqs.size(); i++ ){
        pos[1] = pr->posBindingNontar[0][i];
        pos[2] = pr->posBindingNontar[1][i];

        pos[0] = pos[1] > -1 ? pos[1]- pr->left.length()+1:-1;
        pos[3] = pos[2] > -1 ? pos[2]+ pr->right.length()-1:-1;

        // search left and right primers in non-target sequence
        //lft->appxSearchNonTarSeqs(nontarSeqs[i], LEFT_PRIMER, pos[0], pos[1]);
        //rht->appxSearchNonTarSeqs(nontarSeqs[i], RIGHT_PRIMER, pos[2], pos[3]);

        // only add those sequence not having primer pairs
        if( pos[0] < 0 || pos[2] < 0 || pos[1] >= pos[2] ){
            pos[0] = -1;
            appendSeq2Rich(nontarSeqNames[i], nontarSeqs[i], pos);
        }

        pDlg->setValue(i+1);
    }

    //delete lft;
    //delete rht;
    dispPrimerSel = -1;
    delete pDlg;
}

/*void PrimerReportWindow::getExactPCRinfo()
{
    int range = tarSeqs.size() + nontarSeqs.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Updating primer pair annealing information...",1, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);

    ui->textEdit->clear();

    PrimerPair* pair = primerPairShowList[dispPrimerSel];
    if( !pair->isEstimateTar && !pair->isEstimateNonTar ){  // no need this menu if already exact info
        QMessageBox::about(this,"Exact Value Already","The annealing percentages of this primer pair are exact already.");
        return;
    }

    // get primer sequences
    *int pLen = pair->left.length();
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

        dispPrimerSel = -1;
        delete pDlg;
        return;
    }*


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

        // left and right primer both appear and positions are correct
        bdPos[0] = pair->posBindingTar[0][i]-pair->left.length()+1;
        bdPos[1] = pair->posBindingTar[0][i];
        bdPos[2] = pair->posBindingTar[1][i];
        bdPos[3] = pair->posBindingTar[1][i]+ pair->right.length()-1;
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

        // left and right primer both appear and positions are correct
        bdPos[0] = pair->posBindingNontar[0][i]-pair->left.length()+1;
        bdPos[1] = pair->posBindingNontar[0][i];
        bdPos[2] = pair->posBindingNontar[1][i];
        bdPos[3] = pair->posBindingNontar[1][i]+ pair->right.length()-1;
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
    int nItem = dispPrimerSel;
    QString str;
    //str = QString("%1").arg(pair->pctgInTargetSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel,A_TARGET_PCTG), pair->pctgInTargetSeqs);
    //str = QString("%1").arg(pair->pcrSizeAvgTarSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel,A_TAR_PCR_AVG), pair->pcrSizeAvgTarSeqs);
    //str = QString("%1").arg(pair->pcrSizeMinTarSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel,A_TAR_PCR_MIN), pair->pcrSizeMinTarSeqs);
    //str = QString("%1").arg(pair->pcrSizeMaxTarSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel,A_TAR_PCR_MAX), pair->pcrSizeMaxTarSeqs);

    //str = QString("%1").arg(pair->pctgInNonTarSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel,A_NONTAR_PCTG), pair->pctgInNonTarSeqs);
    //str = QString("%1").arg(pair->pcrSizeAvgNonTarSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel,A_NONTAR_PCR_AVG), pair->pcrSizeAvgNonTarSeqs);
    //str = QString("%1").arg(pair->pcrSizeMinNonTarSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel,A_NONTAR_PCR_MIN), pair->pcrSizeMinNonTarSeqs);
    //str = QString("%1").arg(pair->pcrSizeMaxNonTarSeqs);
    tableModel->setData(tableModel->index(dispPrimerSel,A_NONTAR_PCR_MAX), pair->pcrSizeMaxNonTarSeqs);

    dispPrimerSel = -1;
    delete pDlg;
}*/

void PrimerReportWindow::primerInterComp()
{

    QString left = primerPairShowList[dispPrimerSel]->left;
    //char* tmpLeft = left.toAscii().data();
    QString leftRevs(left);
    std::reverse(leftRevs.begin(), leftRevs.end());
    //QString leftRevs = QString(tmpLeft);

    QString right = primerPairShowList[dispPrimerSel]->rightRevsComp;
    //char* tmpRight = right.toAscii().data();
    QString rightRevs(right);
    std::reverse(rightRevs.begin(), rightRevs.end());
    //QString rightRevs = QString(tmpRight);

    dispPrimerSel = -1;
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

    str = "\nForward-reverse primer inter-complementarity ";
    str += QString("%1.\n").arg(maxComp);
    if( maxComp > 1 ){

        if( shift == 1 ){
            str += "5'-" + left +"-3'\n";

            str += "   ";
            for( int i = 0; i < pos; i++ )
                str += " ";
            for( int i = pos, j = lenRight-1; i < lenLeft && j >= 0; i++, j-- ){
                if( tools::base2int(left[i]) + tools::base2int(right[j]) == 3 )
                    str += "|";
                else
                    str += " ";
            }
            str += "\n";

            for( int i = 0; i < pos; i++ )
                str += " " ;
            str += "3'-" + rightRevs + "-5'\n";
        }
        else{
            nSpace = lenRight - (pos+1);
            //str.clear();
            for( int i = 0; i < nSpace; i++ )
                str += " ";
            str += "5'-" + left + "-3'\n";

            str += "   ";
            for( int i = 0; i < nSpace; i++ )
                str += " ";
            for( int i = 0, j = pos; i <= pos && j >= 0; i++, j-- ){
                if( tools::base2int(left[i]) + tools::base2int(right[j]) == 3 )
                    str += "|";
                else
                    str += " ";
            }
            str += "\n";

            str += "3'-" + rightRevs + "-5'\n";
        }
    }
    ui->textEdit->append(str);
    str.clear();
    //else
    //    ui->textEdit->append("0.\n");


}

void PrimerReportWindow::primer3InterComp()
{
    QString left = primerPairShowList[dispPrimerSel]->left;
    //char* tmpLeft = left.toAscii().data();
    QString leftRevs(left);
    std::reverse(leftRevs.begin(), leftRevs.end());
    //QString leftRevs = QString(tmpLeft);

    QString right = primerPairShowList[dispPrimerSel]->rightRevsComp;
    //char* tmpRight = right.toAscii().data();
    QString rightRevs(right);
    std::reverse(rightRevs.begin(), rightRevs.end());
    //QString rightRevs = QString(tmpRight);

    dispPrimerSel = -1;
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

    str = "\nForward-reverse primer 3' inter-complementarity: ";
    str += QString("%1.\n").arg(maxComp);
    if( maxComp > 1 ){
        str += "5'-" + left +"-3'\n";

        str += "   ";
        for( int i = 0; i < pos; i++ )
            str += " ";
        for( int i = pos, j = lenRight-1; i < lenLeft && j >= 0; i++, j-- ){
            if( tools::base2int(left[i]) + tools::base2int(right[j]) == 3 )
                str += "|";
            else
                str += " ";
        }
        str += "\n";

        for( int i = 0; i < pos; i++ )
            str.append(" ");
        str += "3'-" + rightRevs + "-5'\n";
    }
    ui->textEdit->append(str);
}

void PrimerReportWindow::primerIntraComp()
{
    QString left = primerPairShowList[dispPrimerSel]->left;
    //char* tmpLeft = left.toAscii().data();
    QString leftRevs(left);
    std::reverse(leftRevs.begin(), leftRevs.end());
    //QString leftRevs = QString(tmpLeft);

    QString right = primerPairShowList[dispPrimerSel]->rightRevsComp;
    char* tmpRight = right.toLatin1().data();
    std::reverse(tmpRight, tmpRight+right.length());
    QString rightRevs = QString(tmpRight);

    dispPrimerSel = -1;
    ui->textEdit->clear();
    ui->textEdit->append("Primer intra-complementarity\n\n");

    int lenLeft = left.length();
    int lenRight= right.length();

    int maxComp, numComp, pos, shift, nSpace;
    QString str;


    // 2. left-left intra-complement
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

    str = "\nLeft-left primer intra-complementarity ";
    str += QString("%1.\n").arg(maxComp);
    if( maxComp > 1 ){
        if( shift == 1 ){
            str += "5'-" + left +"-3'\n";

            str += "   ";
            for( int i = 0; i < pos; i++ )
                str += " ";
            for( int i = pos, j = lenLeft-1; i < lenLeft && j >= 0; i++, j-- ){
                if( tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                    str += "|";
                else
                    str += " ";
            }
            str += "\n";

            for( int i = 0; i < pos; i++ )
                str += " ";
            str += "3'-" + leftRevs + "-5'\n";
        }
        else{
            nSpace = lenLeft - (pos+1);

            for( int i = 0; i < nSpace; i++ )
                str += " ";
            str += "5'-" + left + "-3'\n";

            str += "   ";
            for( int i = 0; i < nSpace; i++ )
                str += " ";
            for( int i = 0, j = pos; i <= pos && j >= 0; i++, j-- ){
                if( tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                    str += "|";
                else
                    str += " ";
            }
            str += "\n";

            str += "3'-" + leftRevs + "-5'\n";
        }
    }
    ui->textEdit->append(str);
    str.clear();

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

    str = "\nRight-Right primer intra-complementarity ";
    str += QString("%1.\n").arg(maxComp);
    if( maxComp > 1 ){

        if( shift == 1 ){
            str += "5'-" + right +"-3'\n";
            str += "   ";
            for( int i = 0; i < pos; i++ )
                str += " ";
            for( int i = pos, j = lenRight-1; i < lenRight && j >= 0; i++, j-- ){
                if( tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                    str += "|";
                else
                    str += " ";
            }
            str += "\n";

            for( int i = 0; i < pos; i++ )
                str += " ";
            str += "3'-" + rightRevs + "-5'\n";
        }
        else{
            nSpace = lenRight - (pos+1);
            for( int i = 0; i < nSpace; i++ )
                str += " ";
            str += "5'-" + right + "-3'\n";

            str += "   ";
            for( int i = 0; i < nSpace; i++ )
                    str += " ";
            for( int i = 0, j = pos; i <= pos && j >= 0; i++, j-- ){
                if( tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                    str += "|";
                else
                    str += " ";
            }
            str += "\n";

            str += "3'-" + rightRevs + "-5'\n";
        }
    }
    ui->textEdit->append(str);
/*    QString left = primerPairShowList[dispPrimerSel]->left;
    QString right = primerPairShowList[dispPrimerSel]->rightRevsComp;

    dispPrimerSel = -1;
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

    ui->textEdit->append(str);*/
}

void PrimerReportWindow::primer3IntraComp()
{
    QString left = primerPairShowList[dispPrimerSel]->left;
    char* tmpLeft = left.toLatin1().data();
    std::reverse(tmpLeft, tmpLeft+left.length());
    QString leftRevs = QString(tmpLeft);

    QString right = primerPairShowList[dispPrimerSel]->rightRevsComp;
    char* tmpRight = right.toLatin1().data();
    std::reverse(tmpRight, tmpRight+right.length());
    QString rightRevs = QString(tmpRight);

    dispPrimerSel = -1;
    ui->textEdit->clear();
    ui->textEdit->append("Primer 3' intra-complementarity\n\n");


    int lenLeft = left.length();
    int lenRight= right.length();

    int start, maxComp, numComp, pos;
    QString str;

    // 1. left-left intra-complement
    maxComp = 0;
    if( primerIntraBase3end < lenLeft )
        start = lenLeft - primerIntraBase3end;
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

    str = "\nLeft-left primer 3' intra-complementarity ";
    str += QString("%1.\n").arg(maxComp);
    if( maxComp > 1 ){
        str += "5'-" + left +"-3'\n";

        str += "   ";
        for( int i = 0; i < pos; i++ )
            str.append(" ");
        for( int i = pos, j = lenLeft-1; i < lenLeft && j >= 0; i++, j-- ){
            if( tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                str.append("|");
            else
                str.append(" ");
        }
        str += "\n";

        for( int i = 0; i < pos; i++ )
            str.append(" ");
        str.append("3'-" + leftRevs + "-5'\n");
    }
    ui->textEdit->append(str);

    // 2. right-right intra-complement
    maxComp = 0;
    if( primerIntraBase3end < lenRight )
        start = lenRight - primerIntraBase3end;
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

    str = "\nRight-right primer 3' intra-complementarity ";
    str += QString("%1.\n").arg(maxComp);
    if( maxComp > 1 ){
        str.append("5'-" + right +"-3'\n");

        str += "   ";
        for( int i = 0; i < pos; i++ )
            str.append(" ");
        for( int i = pos, j = lenRight-1; i < lenRight && j >= 0; i++, j-- ){
            if( tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                str.append("|");
            else
                str.append(" ");
        }
        str += "\n";

        for( int i = 0; i < pos; i++ )
            str.append(" ");
        str += "3'-" + rightRevs + "-5'\n";
    }
    ui->textEdit->append(str);

/*    QString left = primerPairShowList[dispPrimerSel]->left;
    QString right = primerPairShowList[dispPrimerSel]->rightRevsComp;

    dispPrimerSel = -1;
    ui->textEdit->clear();
    ui->textEdit->append("Primer 3' intra-complementarity\n\n");

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

    str = "\nForward primer 3' intra-complementarity: ";
    str += QString("%1.\n").arg(maxComp);
    if( maxComp > 1 ){

        str += "5'-";
        for( int i = 0; i < pos + ceil( (float)(lenLeft-pos)/2.0 ); i++ )
            str += left[i];
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
            str += " ";
        str += "3'-";
        for( int i = lenLeft-1; i >= lenLeft - (lenLeft-pos)/2.0; i-- )
            str += left[i];
        str += "\n";
    }
    ui->textEdit->append(str);


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

    str = "\nReverse primer 3' intra-complementarity: ";
    str += QString("%1.\n").arg(maxComp);
    if( maxComp > 1 ){
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
    ui->textEdit->append(str);*/
}

void PrimerReportWindow::on_actionLoad_Primer_List_primer_triggered()
{
    QFileDialog file_dialog(this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getOpenFileName( this, tr( "Load Primer List File" ), pwd, tr( "Primer List File (*.primer);;All File (*.*)" )) ;
    if(filename.isEmpty()){
        return;
    }

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Load primer list\n";
    DataTools::writeErrorLog(errorlog);

    emptyModel();
    DataTools::loadPrimerList(filename, this);
}

void PrimerReportWindow::on_actionSave_Primer_List_primer_File_triggered()
{
    if(primerPairList.size() == 0){
        QMessageBox::about(this, "Empty List", "Nothing to be saved");
        return;
    }

    getMarkedList();
    PrimerSaveListDlg* saveDlg = new PrimerSaveListDlg(/*false,*/this);

    if(!saveDlg->exec())
        return;

    QFileDialog file_dialog( this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getSaveFileName( this, tr( "Save Primer List File" ), pwd, tr( "Primer List File (*.primer);;All File (*.*)"));
    if(filename.isEmpty()){
        return;
    }

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Save primer list\n";
    DataTools::writeErrorLog(errorlog);

    DataTools::savePrimerList(filename, markedList);
}

void PrimerReportWindow::on_actionSave_Primer_List_as_Tab_Delimited_File_Excel_triggered()
{
    if( primerPairList.size() == 0 ){
        QMessageBox::about(this, "Empty List", "Nothing to be saved");
        return;
    }

    getMarkedList();
    PrimerSaveExcelDlg* saveDlg = new PrimerSaveExcelDlg(this);
    if(!saveDlg->exec())
        return;

    QFileDialog file_dialog( this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getSaveFileName( this, tr( "Save Primer to Text" ), pwd, tr( "Text File (*.txt);;All File (*.*)"));
    if(filename.isEmpty()){
        return;
    }

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Save primer list plain text\n";
    DataTools::writeErrorLog(errorlog);

    DataTools::savePrimerList2TXT(filename, markedList);
}

void PrimerReportWindow::on_actionSave_Primer_Information_Window_Content_triggered()
{
    QString primerInfo = ui->textEdit->toPlainText();
    if(primerInfo.length() <= 0){
        QMessageBox::about(this, "Empty List", "Primer Information Window is empty.\nNothing to be saved!");
        return;
    }

    QFileDialog file_dialog( this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getSaveFileName( this, tr( "Save Information Window content" ), pwd, tr( "Text File (*.txt);;All File (*.*)"));
    if(filename.isEmpty()){
        return;
    }
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Save primer Info Window\n";
    DataTools::writeErrorLog(errorlog);

    DataTools::saveText2File(filename, primerInfo);
}

void PrimerReportWindow::on_actionSave_Primer_Pairs_Only_triggered()
{
    if (primerPairList.size() == 0)
        return;

    getMarkedList();
    PrimerSaveListDlg* saveDlg = new PrimerSaveListDlg(/*false,*/ this);

    if(!saveDlg->exec())
        return;

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Save primers only\n";
    DataTools::writeErrorLog(errorlog);

    QFileDialog file_dialog( this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getSaveFileName( this, tr( "Save Information Window content" ), pwd, tr( "Text File (*.txt);;All File (*.*)"));
    if(filename.isEmpty()){
        return;
    }
    DataTools::savePrimerPairs(filename,markedList);
}

void PrimerReportWindow::closeEvent(QCloseEvent *event)
{
    /*Clean Memory: (for Error)
      1. Target&Nontar Seqs
      2. Primer Candidates
      3. Primer Pairs
      4. probe Candidates
      5. probes
      6. Triples (in Primer Pair)
      7. PrimerShowList
      8. tripleShowList
      9. T&P
      10. Score
    */
    ((PRISE*)parentWidget())->removeFlat();
    primerPairShowList.clear();
    DataTools::cleanMemoryAllData();
    QMainWindow::close();
    event->accept();

}

//bool PrimerReportWindow::close()
//{
//
//}

void PrimerReportWindow::on_actionExit_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Clsoe PRISE\n";
    DataTools::writeErrorLog(errorlog);

    close();
}

void PrimerReportWindow::on_actionDisplay_All_Columns_triggered()
{

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Display all columns\n";
    DataTools::writeErrorLog(errorlog);

    /*for(int i = 0; i < primerColumns.size(); i++){
        ui->tableView->setColumnWidth(i, primerColWidth[i]);
    }

    primerColumnDisp.clear();
    primerColumnHide.clear();
    primerColumnDisp = QStringList(primerColumns);

    for(int i = 0; i < primerColumns.size(); i++){
         ui->tableView->setColumnHidden(i, false);
    }
    ui->textEdit->clear();*/
    displayAllColumn();



}

void PrimerReportWindow::on_actionHide_Display_Columns_triggered()
{
    if (primerPairShowList.size() == 0)
        return;

    PrimerColumnDlg *cDlg = new PrimerColumnDlg(this);
    if(!cDlg->exec())
        return;

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Hide\\display columns\n";
    DataTools::writeErrorLog(errorlog);

    for(int i = 0; i < primerColumns.size(); i++){
        ui->tableView->setColumnWidth(i, primerColWidth[i]);
        ui->tableView->setColumnHidden(i, false);
    }

    for(int i = 0; i < primerColumnHide.size(); i++){
        ui->tableView->setColumnHidden(primerColumnHide[i], true);
        //ui->tableView->setColumnHidden(i, false);
        //for(int j = 0; j < primerColumnHide.size(); j++){
        //    if(primerColumns[i].toAscii().replace('\n', ' ') == primerColumnHide[j]){
        //        ui->tableView->setColumnHidden(i, true);
        //        break;
        //    }
        //}
    }
    ui->textEdit->clear();
}

void PrimerReportWindow::on_actionHide_Display_Primer_Pairs_triggered()
{
    if( primerPairList.size() == 0 )
        return;

    dispPrimerCondAllowCancel = 1;
    PrimerDispListDlg* dispDlg = new PrimerDispListDlg(this);

    // process only if the user hits OK
    if( !dispDlg->exec() )
        return;

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Hide\\display primers\n";
    DataTools::writeErrorLog(errorlog);

    copyDispPrimerList();
    insertShownPrimerList();
    /*int range = primerPairShowList.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Adding primers to primer table...", 1,range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);
    pDlg->show();

    // add all primer pairs in result list one by one
    emptyModel();
    tableModel->insertRows(0,primerPairShowList.size());

    // add all primer pairs in result list one by one
    for( unsigned int i = 0; i < primerPairShowList.size(); i++ ){
        insertData(i,i);
        pDlg->setValue(i+1);
    }
    delete pDlg;*/
    QMessageBox::about(this, "Display Info", QString("%1 primer pairs out of %2 total primer pairs are displayed.").arg(primerPairShowList.size()).arg(primerPairList.size()));

}

void PrimerReportWindow::on_actionSort_Primer_List_triggered()
{
    if( primerPairShowList.size() == 0 )
        return;

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Sort primer list\n";
    DataTools::writeErrorLog(errorlog);

    PrimerSortDlg* sDlg = new PrimerSortDlg(this);
    // do nothing if user hit "cancel"
    if( !sDlg->exec() )
        return;
    // execute only if user hit "ok"

    this->emptyModel();
    ui->textEdit->clear();

    DataTools::sortPrimerList();
    this->insertShownPrimerList();
}

void PrimerReportWindow::on_actionAdd_Primer_Pair_Manually_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Add primer manually\n";
    DataTools::writeErrorLog(errorlog);

    tableModel->insertRow(0);
    QStandardItem* __tmpItem = new QStandardItem();
    __tmpItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
    tableModel->setItem(0,0,__tmpItem);
    __tmpItem->setCheckState(Qt::Unchecked);
    tableModel->setData(tableModel->index(0,0), "");

    //tableModel->setData(tableModel->index(0,1),"..");
    //connect(tableModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(checkInputPrimer(QStandardItem*)));
    ui->tableView->setItemDelegate(new PrimerItemDelegate);
    ui->tableView->edit(tableModel->index(0,1));
}

/*
void PrimerReportWindow::checkInputPrimer(QStandardItem *item)
{

    disconnect(tableModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(checkInputPrimer(QStandardItem*)));

    QModelIndex idx = item->index();
    int colIdx = idx.column();
    int rowIdx = idx.row();

    assert(colIdx == 1);
    assert(rowIdx == 0);

    QString primerpair = tableModel->data(idx).toString();
    primerpair = primerpair.toUpper();
    int sep = primerpair.indexOf("..");

    if(sep >= 0 ){
        QString primer_left = primerpair.left(sep);
        QString primer_right = primerpair.mid(sep+2);

        QRegExp rx("^[ACGTUacgtu]+$");

        //int tmpLeft = primer_left.indexOf(rx);
        //int tmpRight = primer_right.indexOf(rx);

        if(!primer_left.isEmpty() && !primer_right.isEmpty() &&
                primer_left.indexOf(rx)>=0 && primer_right.indexOf(rx)>=0){

            QVector<PrimerPair*> pairList;

            PrimerPair *pair = new PrimerPair();
            pair->left = primer_left;
            pair->rightRevsComp = primer_right;
            pairList.push_back(pair);

            DataTools::buildSimCost(primerCostType);
            Designer::updataPrimerInfo(pairList);
            //DataTools::cleanMemoryScoringTable();

            tableModel->setData(tableModel->index(rowIdx,1),primerpair.toUpper());

            primerPairList = pairList + primerPairList;
            primerPairShowList = pairList + primerPairShowList;


            // frequency for single primers
            tableModel->setData(tableModel->index(0, A_LFT_TARGET_PCTG),  pair->pctgInTarSeqsLeft);
            tableModel->setData(tableModel->index(0, A_RHT_TARGET_PCTG),  pair->pctgInTarSeqsRight);
            tableModel->setData(tableModel->index(rowIdx, A_LFT_NONTAR_PCTG),  pair->pctgInNonTarSeqsLeft);
            tableModel->setData(tableModel->index(rowIdx, A_RHT_NONTAR_PCTG),  pair->pctgInNonTarSeqsRight);

            // average annealing position in target sequences: left and right primer
            if( pair->bindPosTarSeqsAvgLeft < 0 )
                tableModel->setData(tableModel->index(rowIdx, A_LFT_TAR_BIND_POS),  "-");
            else{
                tableModel->setData(tableModel->index(rowIdx, A_LFT_TAR_BIND_POS),  pair->bindPosTarSeqsAvgLeft);
            }
            if( pair->bindPosTarSeqsAvgRight < 0 )
                tableModel->setData(tableModel->index(rowIdx, A_RHT_TAR_BIND_POS),  "-");
            else{
                tableModel->setData(tableModel->index(rowIdx, A_RHT_TAR_BIND_POS),  pair->bindPosTarSeqsAvgRight);
            }

            //if( pair->isEstimateTar ){
            //    tableModel->setData(tableModel->index(rowIdx, A_TARGET_PCTG),  pair->pctgInTargetSeqs);
            //    tableModel->setData(tableModel->index(rowIdx, A_TAR_PCR_AVG),  pair->pcrSizeAvgTarSeqs);
            //    tableModel->setData(tableModel->index(rowIdx, A_TAR_PCR_MIN),  pair->pcrSizeMinTarSeqs);
            //    tableModel->setData(tableModel->index(rowIdx, A_TAR_PCR_MAX),  pair->pcrSizeMaxTarSeqs);
            //}
            //else{
                tableModel->setData(tableModel->index(rowIdx, A_TARGET_PCTG),  pair->pctgInTargetSeqs);
                tableModel->setData(tableModel->index(rowIdx, A_TAR_PCR_AVG),  pair->pcrSizeAvgTarSeqs);
                tableModel->setData(tableModel->index(rowIdx, A_TAR_PCR_MIN),  pair->pcrSizeMinTarSeqs);
                tableModel->setData(tableModel->index(rowIdx, A_TAR_PCR_MAX),  pair->pcrSizeMaxTarSeqs);
            //}
            // frequency, average, minimum and maximum product size for non-target sequences
            //if( pair->isEstimateNonTar ){
            //    tableModel->setData(tableModel->index(rowIdx, A_NONTAR_PCTG),  pair->pctgInNonTarSeqs);
            //    tableModel->setData(tableModel->index(rowIdx, A_NONTAR_PCR_AVG),  pair->pcrSizeAvgNonTarSeqs);
            //    tableModel->setData(tableModel->index(rowIdx, A_NONTAR_PCR_MIN),  pair->pcrSizeMinNonTarSeqs);
            //    tableModel->setData(tableModel->index(rowIdx, A_NONTAR_PCR_MAX),  pair->pcrSizeMaxNonTarSeqs);
            //}else{
                tableModel->setData(tableModel->index(rowIdx, A_NONTAR_PCTG),  pair->pctgInNonTarSeqs);
                tableModel->setData(tableModel->index(rowIdx, A_NONTAR_PCR_AVG),  pair->pcrSizeAvgNonTarSeqs);
                tableModel->setData(tableModel->index(rowIdx, A_NONTAR_PCR_MIN),  pair->pcrSizeMinNonTarSeqs);
                tableModel->setData(tableModel->index(rowIdx, A_NONTAR_PCR_MAX),  pair->pcrSizeMaxNonTarSeqs);
            //}

            // left primer GC%
            tableModel->setData(tableModel->index(rowIdx, A_LFT_GC),  pair->GCLeft);
            // right primer GC%
            tableModel->setData(tableModel->index(rowIdx, A_RHT_GC),  pair->GCRight);
            // left primer Tm
            tableModel->setData(tableModel->index(rowIdx, A_LFT_TM),  pair->tmLeft);
            // right primer Tm
            tableModel->setData(tableModel->index(rowIdx, A_RHT_TM),  pair->tmRight);
            // inter-complementarity
            tableModel->setData(tableModel->index(rowIdx, A_INTER_COMP),  pair->prPrComp);
            // 3' inter-complementarity
            tableModel->setData(tableModel->index(rowIdx, A_INTER3_COMP),  pair->prPr3Comp);
            // self-complementarity
            tableModel->setData(tableModel->index(rowIdx, A_LFT_INTRA_COMP),  pair->selfCompLeft);
            tableModel->setData(tableModel->index(rowIdx, A_RHT_INTRA_COMP),  pair->selfCompRight);
            // 3'self-complementarity
            tableModel->setData(tableModel->index(rowIdx, A_LFT_INTRA3_COMP),  pair->self3CompLeft);
            tableModel->setData(tableModel->index(rowIdx, A_RHT_INTRA3_COMP),  pair->self3CompRight);

            tableModel->setData(tableModel->index(rowIdx, A_NUM_AMBI_TARGETS_LEFT),  pair->numAmbiguousTargetsLeft);
            tableModel->setData(tableModel->index(rowIdx, A_NUM_AMBI_TARGETS_RIGHT),  pair->numAmbiguousTargetsRight);
            tableModel->setData(tableModel->index(rowIdx, A_NUM_AMBI_BASES_LEFT),  pair->numAmbiguousBasesLeft);
            tableModel->setData(tableModel->index(rowIdx, A_NUM_AMBI_BASES_RIGHT),  pair->numAmbiguousBasesRight);
            tableModel->setData(tableModel->index(rowIdx, A_NUM_TAR), tarSeqs.size());


            return;
        }
    }

    QString str = QString(" The input contains wrong bases or in wrong format.\n Please check and input a correct primer pair again.");
    QMessageBox::about(this,"Warning",str);

    // delete the selected primer pair from primer pair table
    delete tableModel->item(rowIdx,0);
    tableModel->removeRow(rowIdx);

    return;
}*/





void PrimerReportWindow::on_actionDelete_Primer_Pairs_Conditionally_triggered()
{
    if( primerPairShowList.size() == 0 )
            return;

    PrimerDeleteCondDlg delDlg(this);
    if(!delDlg.exec())
        return;

    MyConfirmDlg cDlg("Are you sure you want to delete sequence(s)?\n\n\n\n",this);
    if(!cDlg.exec())
        return;


    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Delete primer conditionally\n";
    DataTools::writeErrorLog(errorlog);

    int range = primerPairList.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Clean primer pair list memory",0, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);

    emptyModel();
    ui->textEdit->clear();

    PrimerPair *pair;
    bool passChecking;
    int numDel = 0;
    // check for each primer pair
    int totalNum = primerPairList.size();
    for( int i = 0, j = 0; j < totalNum; i++, j++ ){
        pair = primerPairList[i];

        passChecking = checkPrimerWithCondition( pair, deletePrimerCondTitle, deletePrimerCondGtLt, deletePrimerCondValue );

        if( passChecking){	// pass all the checks, remove it from primerPairList memory
            pair->left = "DELETE"; // mark it for deletion
        }
        pDlg->setValue(j+1);
    }
    delete pDlg;

    //range = primerPairShowList.size();
    //pDlg = new MyProgressDlg("Adding primers to primer table...",0, range, this);
    //pDlg->setWindowModality(Qt::ApplicationModal);


    // check for each primer pair in displaying primer list
    totalNum = primerPairShowList.size();
    for( int i = 0, j = 0; j < totalNum; i++, j++ ){
        pair = primerPairShowList[i];
        // previously marked for deletion
        // remove from shown list
        if( pair->left == "DELETE" ){
            primerPairShowList.erase(primerPairShowList.begin()+i );
            i--;
        }
    }



    // delete from the memory
    totalNum = primerPairList.size();
    for( int i = 0, j = 0; j < totalNum; i++, j++ ){
        pair = primerPairList[i];

        if( pair->left == "DELETE"){	// pass all the checks, remove it from primerPairList memory
            primerPairList.erase( primerPairList.begin()+i );
            delete pair;
            i--;
            numDel++;
        }
    }


    range = primerPairShowList.size();
    pDlg = new MyProgressDlg("Adding primers to primer table",0, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);

    tableModel->insertRows(0,primerPairShowList.size());
    for(int i = 0; i < primerPairShowList.size(); i++){
        this->insertData(i,i);
        pDlg->setValue(range+i);
    }

    delete pDlg;

    QString str = QString("%1 primer pairs were deleted.").arg(numDel);
    QMessageBox::about(this,"",str);
}

void PrimerReportWindow::insertShownPrimerList()
{
    int range = primerPairShowList.size();

    //QString currCnt = QString("%1").arg(range);
    //QMessageBox::about(0,"",currCnt);
    MyProgressDlg *pDlg = new MyProgressDlg("Adding primers to primer table",0, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);
    pDlg->show();

    // add all primer pairs in result list one by one
    emptyModel();
    tableModel->insertRows(0,primerPairShowList.size());

    // add all primer pairs in result list one by one
    for(int i = 0; i < primerPairShowList.size(); i++ ){
        insertData(i,i);
        pDlg->setValue(i+1);
    }
    delete pDlg;
}

void PrimerReportWindow::on_actionDelete_Marked_Primer_Pairs_triggered()
{
    if( primerPairShowList.size() == 0 )
        return;

    int numDel = 0;

    this->getMarkedList();
    numDel = this->markedList.size();

    if( numDel == 0 ){
        QMessageBox::about(this, "Empty delete list", "No primer pair is marked.");
        return;
    }

    // to confirm delete
    MyConfirmDlg cDlg("Are you sure you want to delete sequence(s)?\n\n\n\n",this);
    if(!cDlg.exec())
        return;

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Delete Marked Primer Pairs\n";
    DataTools::writeErrorLog(errorlog);

    ui->textEdit->clear();

    int range;
    MyProgressDlg *pDlg = new MyProgressDlg("Clean primer pair list memory",0, 1, this);
    pDlg->setWindowModality(Qt::ApplicationModal);

    // if marked primer pairs are too much, delete them all, then add one by one
    if( numDel > 100 ){
        this->emptyModel();
        range = markedList.size()+ primerPairList.size();
        pDlg->setRange(0, range);

        // mark those primers for deletion and remove them in displayed list first
        int i = markedList.size() - 1, j, fin = 1;
        while( i >= 0 ){
            j = markedList[i--];
            primerPairShowList[j]->left = "DELETE";
            primerPairShowList.erase(primerPairShowList.begin() + j );

            pDlg->setValue(fin++);
        }

        // destroy the memory and consolidate in memory primer list
        for( i = primerPairList.size() - 1; i >= 0; i-- ){
            if( primerPairList[i]->left == "DELETE" ){
                delete primerPairList[i];
                primerPairList.erase( primerPairList.begin() + i );
            }

            pDlg->setValue(fin++);
        }
        insertShownPrimerList();
    }
    else{
        QString primerMarked;
        int i = numDel-1;
        while( i >= 0 ){
            // retrieve the primer pair sequences
            primerMarked = tableModel->item(markedList[i], A_PRIMER_PAIR)->text();

            // delete the selected primer pair from primer pair table
            delete tableModel->item(markedList[i],0);
            tableModel->removeRow(markedList[i]);
            i--;

            // delete the selected primer pair from current primer pair memory
            if( DataTools::deletePrimerFromPrimerMem( primerMarked, primerPairShowList, 0 ) < 0 )
                return;

            // delete the primer pair from theApp memory and destroy the memory
            if( DataTools::deletePrimerFromPrimerMem( primerMarked, primerPairList, 1 ) < 0 )
                return;
        }
    }
    delete pDlg;

    QString str = QString("%1 marked primer pairs were deleted.").arg(numDel);
    QMessageBox::about(this,"",str);
}

void PrimerReportWindow::on_actionDelete_Seleted_Primer_Pairs_triggered()
{
    if( primerPairShowList.size() == 0 )
            return;

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();

    if( selectedRows.size() <= 0 ){
        QMessageBox::about(this, "Empty delete list", "No primer pair is selected.");
        return;
    }

    // to confirm delete
    MyConfirmDlg cDlg("Are you sure you want to delete sequence(s)?\n\n\n\n",this);
    if(!cDlg.exec())
        return;


    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Delete selected primer manually\n";
    DataTools::writeErrorLog(errorlog);

    ui->textEdit->clear();

    int range;
    MyProgressDlg *pDlg = new MyProgressDlg("Clean primer pair list memory",0, 1, this);
    pDlg->setWindowModality(Qt::ApplicationModal);


    // track selected primer pairs
    int nSelCount = selectedRows.size();

    // if selected primer pairs are too much, delete them all, then add one by one
    if( nSelCount > 100 ){
        this->emptyModel();
        range = nSelCount+ primerPairList.size();
        pDlg->setRange(0, range);

        // mark those primers for deletion and remove them in displayed list first
        int i = nSelCount - 1, j, fin = 1;
        while( i >= 0 ){
            j = selectedRows.at(i--).row();
            primerPairShowList[j]->left = "DELETE";
            primerPairShowList.erase( primerPairShowList.begin() + j );

            pDlg->setValue(fin++);
        }

        // destroy the memory and consolidate in memory primer list
        for( i = primerPairList.size() - 1; i >= 0; i-- ){
            if( primerPairList[i]->left == "DELETE" ){
                delete primerPairList[i];
                primerPairList.erase( primerPairList.begin() + i );
            }
            pDlg->setValue(fin++);
        }
        this->insertShownPrimerList();
    }
    else{

        QString primerSelected;
        int i = selectedRows.size() - 1;
        while( i >= 0 ){

            int rowIdx = selectedRows.at(i).row();
            // retrieve the primer pair sequences
            primerSelected = tableModel->item( rowIdx, A_PRIMER_PAIR)->text();

            // delete the selected primer pair from primer pair table
            delete tableModel->item(rowIdx,0);
            tableModel->removeRow(rowIdx);
            i--;

            // delete the selected primer pair from current primer pair memory
            if( DataTools::deletePrimerFromPrimerMem( primerSelected, primerPairShowList, 0 ) < 0 )
                    return;

            // delete the primer pair from theApp memory and destroy the memory
            if( DataTools::deletePrimerFromPrimerMem( primerSelected, primerPairList, 1 ) < 0 )
                    return;
        }
    }
    delete pDlg;

    QString str = QString("%1 selected primer pairs were deleted.").arg(nSelCount);
    QMessageBox::about(this,"",str);
}

void PrimerReportWindow::deletePrimerPair()
{
    // to confirm delete
    MyConfirmDlg cDlg("Are you sure you want to delete sequence(s)?\n\n\n\n",this);
    if(!cDlg.exec())
        return;

    ui->textEdit->clear();
    // retrieve the selected primer pair sequences
    QString primerSelected = tableModel->item( dispPrimerSel, A_PRIMER_PAIR)->text();

    // delete the selected primer pair from primer pair table
    delete tableModel->item(dispPrimerSel,0);
    tableModel->removeRow(dispPrimerSel);

    // delete the selected primer pair from current primer pair memory
    if( DataTools::deletePrimerFromPrimerMem( primerSelected, primerPairShowList, 0 ) < 0 )
            return;
    // delete the primer pair from theApp memory and destroy the memory
    if( DataTools::deletePrimerFromPrimerMem( primerSelected, primerPairList, 1 ) < 0 )
            return;

}

void PrimerReportWindow::on_actionMark_Selected_Primer_Pairs_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Mark selected primer pairs\n";
    DataTools::writeErrorLog(errorlog);

    markselectedprimerpairs();
}

void PrimerReportWindow::on_actionUnmark_Selected_Primer_Pairs_triggered()
{
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Unmark selected primer pairs\n";
    DataTools::writeErrorLog(errorlog);

    unmarkselectedprimerpairs();
}

void PrimerReportWindow::on_actionAnnealing_Info_triggered()
{
    if(primerPairShowList.size() == 0 )
        return;

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if( selectedRows.size() <= 0 ||selectedRows.size() > 1 ){
        QMessageBox::about(this,"selection error","Please select one primer pair to analyze.\r\n\r\n"
                                "(Primer pairs are selected by clicking on any part "
                                "of the row except the Primer Pair No.\r\n"
                                "Selected primer pairs are designated by blue shading.)" );
        return;
    }

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Show annealing info\n";
    DataTools::writeErrorLog(errorlog);

    dispPrimerSel = selectedRows.at(0).row();
    dispCompleteInfo();
}

void PrimerReportWindow::on_actionPercentage_of_Each_Nucleotide_in_Target_and_Non_Target_Sequences_in_Relation_to_Primer_Sequences_triggered()
{
    if( primerPairShowList.size() == 0 )
        return;

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if( selectedRows.size() <= 0 ||selectedRows.size() > 1 ){
        QMessageBox::about(this,"selection error","Please select one primer pair to analyze.\r\n\r\n"
                                "(Primer pairs are selected by clicking on any part "
                                "of the row except the Primer Pair No.\r\n"
                                "Selected primer pairs are designated by blue shading.)" );
        return;
    }

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Show percentage of each nucleotide\n";
    DataTools::writeErrorLog(errorlog);

    dispPrimerSel = selectedRows.at(0).row();
    reportBindingContent();
}

void PrimerReportWindow::on_actionTarget_Sequences_Annealing_with_Primer_triggered()
{

    if( primerPairShowList.size() == 0 )
        return;

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if( selectedRows.size() <= 0 ||selectedRows.size() > 1 ){
        QMessageBox::about(this,"selection error","Please select one primer pair to analyze.\r\n\r\n"
                                "(Primer pairs are selected by clicking on any part "
                                "of the row except the Primer Pair No.\r\n"
                                "Selected primer pairs are designated by blue shading.)" );
        return;
    }
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Target sequences covered by primer pair.\n";
    DataTools::writeErrorLog(errorlog);

    dispPrimerSel = selectedRows.at(0).row();
    targetWPrimer();
}

void PrimerReportWindow::on_actionTarget_Sequences_Not_Annealing_with_Primer_triggered()
{
    if( primerPairShowList.size() == 0 )
        return;

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if( selectedRows.size() <= 0 ||selectedRows.size() > 1 ){
        QMessageBox::about(this,"selection error","Please select one primer pair to analyze.\r\n\r\n"
                            "(Primer pairs are selected by clicking on any part "
                            "of the row except the Primer Pair No.\r\n"
                            "Selected primer pairs are designated by blue shading.)" );
        return;
    }
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Target sequences not covered by primer pair.\n";
    DataTools::writeErrorLog(errorlog);

    dispPrimerSel = selectedRows.at(0).row();
    targetWoPrimer();
}

void PrimerReportWindow::on_actionNon_Target_Sequences_Not_Annealing_with_Primer_triggered()
{
    if( primerPairShowList.size() == 0 )
        return;

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if( selectedRows.size() <= 0 ||selectedRows.size() > 1 ){
        QMessageBox::about(this,"selection error","Please select one primer pair to analyze.\r\n\r\n"
                            "(Primer pairs are selected by clicking on any part "
                            "of the row except the Primer Pair No.\r\n"
                            "Selected primer pairs are designated by blue shading.)" );
        return;
    }
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Nontarget sequences not covered by primer pair.\n";
    DataTools::writeErrorLog(errorlog);

    dispPrimerSel = selectedRows.at(0).row();
    nonTargetWoPrimer();
}

void PrimerReportWindow::on_actionNon_Target_Sequences_Annealing_with_Primer_triggered()
{
    if( primerPairShowList.size() == 0 )
        return;

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if( selectedRows.size() <= 0 ||selectedRows.size() > 1 ){
        QMessageBox::about(this,"selection error","Please select one primer pair to analyze.\r\n\r\n"
                            "(Primer pairs are selected by clicking on any part "
                            "of the row except the Primer Pair No.\r\n"
                            "Selected primer pairs are designated by blue shading.)" );
        return;
    }
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Nontarget sequences covered by primer pair.\n";
    DataTools::writeErrorLog(errorlog);

    dispPrimerSel = selectedRows.at(0).row();
    nonTargetWPrimer();
}


/*void PrimerReportWindow::on_actionUpdate_Estimated_Information_to_Exact_Information_triggered()
{
    if( primerPairShowList.size() == 0 )
        return;

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if( selectedRows.size() <= 0 ||selectedRows.size() > 1 ){
        QMessageBox::about(this,"selection error","Please select one primer pair to analyze.\r\n\r\n"
                            "(Primer pairs are selected by clicking on any part "
                            "of the row except the Primer Pair No.\r\n"
                            "Selected primer pairs are designated by blue shading.)" );
        return;
    }

    dispPrimerSel = selectedRows.at(0).row();
    getExactPCRinfo();
}*/

void PrimerReportWindow::on_actionPrimer_Inter_Complementarity_triggered()
{
    if( primerPairShowList.size() == 0 )
        return;

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if( selectedRows.size() <= 0 ||selectedRows.size() > 1 ){
        QMessageBox::about(this,"selection error","Please select one primer pair to analyze.\r\n\r\n"
                            "(Primer pairs are selected by clicking on any part "
                            "of the row except the Primer Pair No.\r\n"
                            "Selected primer pairs are designated by blue shading.)" );
        return;
    }
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Primer Inter Comp.\n";
    DataTools::writeErrorLog(errorlog);

    dispPrimerSel = selectedRows.at(0).row();
    primerInterComp();
}

void PrimerReportWindow::on_actionPrimer_3_Inter_Complementarity_triggered()
{
    if( primerPairShowList.size() == 0 )
        return;

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if( selectedRows.size() <= 0 ||selectedRows.size() > 1 ){
        QMessageBox::about(this,"selection error","Please select one primer pair to analyze.\r\n\r\n"
                            "(Primer pairs are selected by clicking on any part "
                            "of the row except the Primer Pair No.\r\n"
                            "Selected primer pairs are designated by blue shading.)" );
        return;
    }
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Primer 3' Inter Comp.\n";
    DataTools::writeErrorLog(errorlog);

    dispPrimerSel = selectedRows.at(0).row();
    primer3InterComp();
}

void PrimerReportWindow::on_actionPrimer_Intra_Complementarity_triggered()
{
    if( primerPairShowList.size() == 0 )
        return;

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if( selectedRows.size() <= 0 ||selectedRows.size() > 1 ){
        QMessageBox::about(this,"selection error","Please select one primer pair to analyze.\r\n\r\n"
                            "(Primer pairs are selected by clicking on any part "
                            "of the row except the Primer Pair No.\r\n"
                            "Selected primer pairs are designated by blue shading.)" );
        return;
    }
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Primer Intra Comp.\n";
    DataTools::writeErrorLog(errorlog);

    dispPrimerSel = selectedRows.at(0).row();
    primerIntraComp();
}

void PrimerReportWindow::on_actionPrimer_3_Intra_Complementarity_triggered()
{
    if( primerPairShowList.size() == 0 )
        return;

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if( selectedRows.size() <= 0 ||selectedRows.size() > 1 ){
        QMessageBox::about(this,"selection error","Please select one primer pair to analyze.\r\n\r\n"
                            "(Primer pairs are selected by clicking on any part "
                            "of the row except the Primer Pair No.\r\n"
                            "Selected primer pairs are designated by blue shading.)" );
        return;
    }
    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Primer 3' Intra Comp.\n";
    DataTools::writeErrorLog(errorlog);

    dispPrimerSel = selectedRows.at(0).row();
    primer3IntraComp();
}

void PrimerReportWindow::sortData(int idx)
{
    primerColSortOrder[idx] = 1-primerColSortOrder[idx];
    int adescending =  primerColSortOrder[idx];

    //MyProgressDlg *pDlg = new MyProgressDlg("Sorting primer list",0, 1, this);
    //pDlg->setWindowModality(Qt::ApplicationModal);

    //emptyModel();

    //int range = primerPairShowList.size();
    //int numPrimerPairs = range;
    //pDlg->setRange(0,range);

    //PrimerPair *pair;

    // sort primer list base on primer pair No.
    /*if( idx == A_PRIMER_NO ){ // sort marked / unmarked primer pair
        if( adescending == 0 ){ // marked first, followed by unmarked
            for( int i = 1; i < numPrimerPairs; i++ ){
                for( int j = 0; j < i; j++ ){
                    if( primerPairShowList[j]->marked && !primerPairShowList[i]->marked ){
                        pair = primerPairShowList[i];
                        primerPairShowList[i] = primerPairShowList[j];
                        primerPairShowList[j] = pair;
                    }
                }
                pDlg->setValue(i);
            }
        }else{ // unmarked first, followed by marked
            for( int i = 1; i < numPrimerPairs; i++ ){
                for( int j = 0; j < i; j++ ){
                    if( !primerPairShowList[j]->marked && primerPairShowList[i]->marked ){
                        pair = primerPairShowList[i];
                        primerPairShowList[i] = primerPairShowList[j];
                        primerPairShowList[j] = pair;
                    }
                }
                pDlg->setValue(i);
            }
        }
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::marked)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::marked)) ; break;
        }

        delete pDlg;
        insertShownPrimerList();
        return;
    }*/

    // sort primer list base on primer pair
    /*if( idx == A_PRIMER_PAIR ){
        QStringList primerList;
        for( int i = 0; i < numPrimerPairs; i++ )
            primerList.push_back(primerPairShowList[i]->left + primerPairShowList[i]->rightRevsComp );
        QString tPrimer;
        // ascending order
        if( adescending == 0 ){
            // insertion sort according to baseArray
            for( int i = 1; i < numPrimerPairs; i++ ){
                for( int j = 0; j < i; j++){
                    if( primerList[j] > primerList[i] ){
                        tPrimer = primerList[i];
                        primerList[i] = primerList[j];
                        primerList[j] = tPrimer;
                        pair = primerPairShowList[i];
                        primerPairShowList[i] = primerPairShowList[j];
                        primerPairShowList[j] = pair;
                    }
                }
                pDlg->setValue(i);
            }
        }
        else{ // descending order
            // insertion sort according to baseArray
            for( int i = 1; i < numPrimerPairs; i++ ){
                for( int j = 0; j < i; j++){
                    if( primerList[j] < primerList[i] ){
                        tPrimer = primerList[i];
                        primerList[i] = primerList[j];
                        primerList[j] = tPrimer;
                        pair = primerPairShowList[i];
                        primerPairShowList[i] = primerPairShowList[j];
                        primerPairShowList[j] = pair;
                    }
                }
                pDlg->setValue(i);
            }
        }
        delete pDlg;

        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), lt_by_seq) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), gt_by_seq) ; break;
        }

        delete pDlg;
        insertShownPrimerList();
        return;
    }*/

    /*float *baseArray;
    try{
            baseArray = new float[numPrimerPairs];
    }
    catch( std::bad_alloc& e ){
        QMessageBox::about(this,"Fatal error","Fatal error: out of memory for sorting.\nProgram is forced to terminate.");
        DataTools::cleanMemoryPrimerReport();
        exit(-1);
    }

    for( int i = 0; i < numPrimerPairs; i++ )
        baseArray[i] = DataTools::getPrimerInfo( primerPairShowList[i], idx );


    float temp;
    // ascending order
    if( adescending == 0 ){
        // insertion sort according to baseArray
        for( int i = 1; i < numPrimerPairs; i++ ){
            for( int j = 0; j < i; j++){
                if( baseArray[j] > baseArray[i] ){
                    temp = baseArray[i];
                    baseArray[i] = baseArray[j];
                    baseArray[j] = temp;
                    pair = primerPairShowList[i];
                    primerPairShowList[i] = primerPairShowList[j];
                    primerPairShowList[j] = pair;
                }
            }
            pDlg->setValue(i);
        }
    }else{ // descending order
        // insertion sort according to baseArray
        for( int i = 1; i < numPrimerPairs; i++ ){
            for( int j = 0; j < i; j++){
                if( baseArray[j] < baseArray[i] ){
                    temp = baseArray[i];
                    baseArray[i] = baseArray[j];
                    baseArray[j] = temp;
                    pair = primerPairShowList[i];
                    primerPairShowList[i] = primerPairShowList[j];
                    primerPairShowList[j] = pair;
                }
            }
            pDlg->setValue(i);
        }
    }*/


    switch(idx){
    case 0:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::marked)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::marked)) ; break;
        }
        break;
    }
    case 1:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), lt_by_seq) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), gt_by_seq) ; break;
        }
        break;
    }
    case 2:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::pctgInTargetSeqs)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::pctgInTargetSeqs)) ; break;
        }
        break;
    }
    case 3:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::pctgInTarSeqsLeft)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::pctgInTarSeqsLeft)) ; break;
        }

        break;
    }
    case 4:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::pctgInTarSeqsRight)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::pctgInTarSeqsRight)) ; break;
        }
        break;
    }
    case 5:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::pctgInNonTarSeqs)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::pctgInNonTarSeqs)) ; break;
        }
        break;
    }
    case 6:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::pctgInNonTarSeqsLeft)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::pctgInNonTarSeqsLeft)) ; break;
        }
        break;
    }
    case 7:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::pctgInNonTarSeqsRight)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::pctgInNonTarSeqsRight)) ; break;
        }
        break;
    }
    case 8:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::bindPosTarSeqsAvgLeft)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::bindPosTarSeqsAvgLeft)) ; break;
        }
        break;

    }
    case 9:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::bindPosTarSeqsAvgRight)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::bindPosTarSeqsAvgRight)) ; break;
        }
        break;
    }
    case 10:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::pcrSizeAvgTarSeqs)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::pcrSizeAvgTarSeqs)) ; break;
        }
        break;

    }
    case 11:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::pcrSizeMinTarSeqs)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::pcrSizeMinTarSeqs)) ; break;
        }
        break;
    }
    case 12:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::pcrSizeMaxTarSeqs)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::pcrSizeMaxTarSeqs)) ; break;
        }
        break;
    }
    case 13:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::pcrSizeAvgNonTarSeqs)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::pcrSizeAvgNonTarSeqs)) ; break;
        }

        break;
    }
    case 14:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::pcrSizeMinNonTarSeqs)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::pcrSizeMinNonTarSeqs)) ; break;
        }
        break;
    }
    case 15:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::pcrSizeMaxNonTarSeqs)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::pcrSizeMaxNonTarSeqs)) ; break;
        }
        break;
    }
    case 16:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::GCLeft)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::GCLeft)) ; break;
        }
        break;
    }
    case 17:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::GCRight)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::GCRight)) ; break;
        }
        break;
    }
    case 18:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::tmLeft)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::tmLeft)) ; break;
        }
        break;

    }
    case 19:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::tmRight)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::tmRight)) ; break;
        }
        break;
    }
    case 20:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::prPrComp)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::prPrComp)) ; break;
        }
        break;

    }
    case 21:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::prPr3Comp)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::prPr3Comp)) ; break;
        }
        break;
    }
    case 22:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::selfCompLeft)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::selfCompLeft)) ; break;
        }
        break;
    }
    case 23:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::selfCompRight)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::selfCompRight)) ; break;
        }

        break;
    }
    case 24:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::self3CompLeft)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::self3CompLeft)) ; break;
        }
        break;
    }
    case 25:
    {
        switch(adescending){
        case true:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::self3CompRight)) ; break;
        case false:
            stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::self3CompRight)) ; break;
        }
        break;
    }
    }


    /*switch(adescending){
    case true:
        stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberAscend(&PrimerPair::marked)) ; break;
    case false:
        stable_sort(primerPairShowList.begin(), primerPairShowList.end(), byMemberDescend(&PrimerPair::marked)) ; break;
    }*/
    //delete[] baseArray;
    //delete pDlg;
    insertShownPrimerList();
    return;
}

void PrimerReportWindow::on_actionBLAST_Forward_Primer_triggered()
{
    if( primerPairShowList.size() == 0 )
        return;

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if( selectedRows.size() <= 0 ||selectedRows.size() > 1 ){
        QMessageBox::about(this,"selection error","Please select one primer pair to analyze.\r\n\r\n"
                            "(Primer pairs are selected by clicking on any part "
                            "of the row except the Primer Pair No.\r\n"
                            "Selected primer pairs are designated by blue shading.)" );
        return;
    }
    dispPrimerSel = selectedRows.at(0).row();

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + QString("\t Blast forward.\n").arg(primerPairShowList[dispPrimerSel]->left);
    DataTools::writeErrorLog(errorlog);

    blastForwardPrimer();
}

void PrimerReportWindow::on_actionBLAST_Reverse_Primer_triggered()
{

    if( primerPairShowList.size() == 0 )
        return;

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if( selectedRows.size() <= 0 ||selectedRows.size() > 1 ){
        QMessageBox::about(this,"selection error","Please select one primer pair to analyze.\r\n\r\n"
                            "(Primer pairs are selected by clicking on any part "
                            "of the row except the Primer Pair No.\r\n"
                            "Selected primer pairs are designated by blue shading.)" );
        return;
    }

    dispPrimerSel = selectedRows.at(0).row();

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + QString("\t Blast reverse.\n").arg(primerPairShowList[dispPrimerSel]->rightRevsComp);
    DataTools::writeErrorLog(errorlog);

    blastReversePrimer();
}

void PrimerReportWindow::blastForwardPrimer()
{
    QString url = "http://www.ncbi.nlm.nih.gov/BLAST/Blast.cgi?QUERY=";
    url +=	primerPairShowList[dispPrimerSel]->left +
                    "&CMD=Web&LAYOUT=TwoWindows&AUTO_FORMAT=Semiauto"
                    "&ALIGNMENTS=50&ALIGNMENT_VIEW=Pairwise&CLIENT=web"
                    "&DATABASE=nr&DESCRIPTIONS=100&ENTREZ_QUERY=(none)"
                    "&EXPECT=1000&FORMAT_OBJECT=Alignment&FORMAT_TYPE=HTML"
                    "&NCBI_GI=on&PAGE=Nucleotides&PROGRAM=blastn"
                    "&SERVICE=plain&SET_DEFAULTS.x=29&SET_DEFAULTS.y=6"
                    "&SHOW_OVERVIEW=on&WORD_SIZE=7&END_OF_HTTPGET=Yes"
                    "&SHOW_LINKOUT=yes&GET_SEQUENCE=yes";

    dispPrimerSel = -1;
    QDesktopServices::openUrl(QUrl(QString(url)));
}

void PrimerReportWindow::blastReversePrimer()
{
    QString url = "http://www.ncbi.nlm.nih.gov/BLAST/Blast.cgi?QUERY=";
    url +=	primerPairShowList[dispPrimerSel]->rightRevsComp +
                    "&CMD=Web&LAYOUT=TwoWindows&AUTO_FORMAT=Semiauto"
                    "&ALIGNMENTS=50&ALIGNMENT_VIEW=Pairwise&CLIENT=web"
                    "&DATABASE=nr&DESCRIPTIONS=100&ENTREZ_QUERY=(none)"
                    "&EXPECT=1000&FORMAT_OBJECT=Alignment&FORMAT_TYPE=HTML"
                    "&NCBI_GI=on&PAGE=Nucleotides&PROGRAM=blastn"
                    "&SERVICE=plain&SET_DEFAULTS.x=29&SET_DEFAULTS.y=6"
                    "&SHOW_OVERVIEW=on&WORD_SIZE=7&END_OF_HTTPGET=Yes"
                    "&SHOW_LINKOUT=yes&GET_SEQUENCE=yes";

    dispPrimerSel = -1;
    QDesktopServices::openUrl(QUrl(QString(url)));
}

void PrimerReportWindow::on_actionPRISE_Manual_triggered()
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

void PrimerReportWindow::on_actionPRISE_Tutorial_triggered()
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

void PrimerReportWindow::designProbes4markedPrimerPairs()
{
    QVector<int> selected;
    selected.push_back(dispPrimerSel);
    designProbes4markedPrimerPairs(selected);
    selected.clear();
}

void PrimerReportWindow::designProbes4markedPrimerPairs(QVector<int> primers4ProbeDesign)
{
    //Probe Design Wizard
    task = 1; //design probes

    //Start probe design
    DesignWizard* pWprobe = new DesignWizard(this);

    pWprobe->forProbes();
    pWprobe->setStartId(Page_pCand);

    while(pWprobe->exec()){
        firstProbe = false;
        pWprobe->restart();
        /*QString logFilePath = "";
        #ifdef MAC
            QString appPath = QDir::currentPath();
            logFilePath += "file:/";
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

        file.open(QIODevice::Append| QIODevice::Text);
        QTextStream out(&file);
        QTime time = QTime::currentTime();
        out << time.toString()<<"\t Start Probe Design Process\n";
        out << "Settings:\n";
        out << "Primer Length: " << probeLenMin << " " << probeLenMax << "\n";
        out << "Primer Freq: " << probeCandidFrequency << " " << probeCandidTopPctg << "\n";
        out << "Minimum gap: " << probeMinGap << "\n";
        out << "Avoid: " << probeAvoidACT << " " <<probeAvoidG << " " << probeAvoidGat5end << "\n";
        out << "avoid #: " <<probeNumACT << " " << probeNumG << " " <<probeNumGat5end << "\n";
        out << "Primer Tm: " << probeTmMin << " " << probeTmMax << " " << probeTmGreaterMin << " " << probeTmGreaterMax;
        out << "Na Consent: " << probeNaConsent << "\n";
        out << "GC: " << probeGCMin << " " << probeGCMax << "\n";
        out << "Comp: " <<  maxPbPrComp << " " << max3PbPrComp << " " << tripleInterBase3end << " " << probeMaxSelfComp << " " <<probeMax3SelfComp << " " << probeIntraBase3end << "\n";
        out << "Indel: "<< maxIndel << " " << indelCost << "\n";
        file.close();*/

        QString errorlog = "";

        QTime time = QTime::currentTime();
        errorlog += time.toString() + "\t Start Probe Design Process\n";
        errorlog += "Settings:\n";
        errorlog += "Primer Length: " + QString::number(probeLenMin) + " " + QString::number(probeLenMax) + "\n";
        errorlog += "Primer Freq: " + QString::number(probeCandidFrequency) + " " + QString::number(probeCandidTopPctg) + "\n";
        errorlog += "Minimum gap: " + QString::number(probeMinGap) + "\n";
        errorlog += "Avoid: " + QString::number(probeAvoidACT) + " " + QString::number(probeAvoidG) + " " + QString::number(probeAvoidGat5end) + "\n";
        errorlog += "avoid #: " + QString::number(probeNumACT) + " " + QString::number(probeNumG) + "\n";
        errorlog += "Primer Tm: " + QString::number(probeTmMin) + " " + QString::number(probeTmMax) + " " + QString::number(probeTmGreaterMin) + " " + QString::number(probeTmGreaterMax);
        errorlog += "Na Consent: " + QString::number(probeNaConsent) + "\n";
        errorlog += "GC: " + QString::number(probeGCMin) + " " + QString::number(probeGCMax) + "\n";
        errorlog += "Comp: " +  QString::number(maxPbPrComp) + " " + QString::number(probeMaxSelfComp) + "\n";
        errorlog += "Indel: " + QString::number(maxIndel) + " " + QString::number(indelCost) + "\n";

        DataTools::writeErrorLog(errorlog);


        DataTools::buildSimCost(probeCostType);
        Designer::startProbeDesign();

        //    }
//    else return;
//    pWprobe->close();

        for(int i = 0; i < primers4ProbeDesign.size(); i++){
            PrimerPair* currPr = primerPairShowList[primers4ProbeDesign[i]];
            Designer::checkTriple(currPr);
        }

        logDesign += "\r\n";
        time = QTime::currentTime();
        QString timeStr = time.toString();
        logDesign += timeStr;
        logDesign += ":	Terminate primer-probe sets design process.";

        tpWindow = new TripleReportWindow(this);

        // call result dialog and add primer pair list
        int tabCnt = 0;
        QStringList noProbe;

        for(int i = 0; i < primers4ProbeDesign.size(); i++){
            PrimerPair* currPr = primerPairShowList[primers4ProbeDesign[i]];

            if(currPr->tripleList.size() == 0){
                noProbe.push_back(currPr->left+".."+currPr->rightRevsComp);
                continue;
            }

            TripleReportFrame  *prWin = new TripleReportFrame(currPr);
            tpWindow->addTab(prWin,currPr->left+".."+currPr->rightRevsComp);
            tabCnt++;
        }

        logDesign =
            QString(
                "The report below shows the number of candidate probes meeting "
                "each selectivity criterion.\r\n"
                "You can increase the number of probes by repeating the process "
                "and relaxing these criteria in the Primer-Probe Set Design Settings window.\r\n\r\n"

                "To repeat design process using different criteria, click \"Change Criteria\".\r\n"
                "To go back to main window, click OK.\r\n\r\n"
                "==================================================================\r\n")
        + logDesign;

        MessageDlg* MDlg = new MessageDlg("Primer-Probe Set Design Report", logDesign);


        assert(tabCnt + noProbe.size() == primers4ProbeDesign.size());


        QTextCharFormat emFormat = QTextCharFormat();

        if(tabCnt == 0) {            
            MDlg->enableOK(false);

            emFormat.setForeground(QColor("#ff0000"));
            emFormat.setFontPointSize(12);

            QString tmpStr = "No probe is found for any marked primer pairs under current critera.\r\n\r\n";
            MDlg->insertRichText(tmpStr, emFormat);

        }else{
            if(!noProbe.isEmpty()){
                QString tmpStr;

                emFormat.setForeground(QColor("#ff0000"));

                tmpStr +=  "\r\n";
                for(int i = 0; i < noProbe.size(); i++){
                    tmpStr += "\t"+ noProbe[i] + "\r\n";
                }
                tmpStr +=  "\r\n\r\n";
                MDlg->insertRichText(tmpStr,emFormat );

                emFormat.setFontPointSize(12);

                tmpStr.clear();
                tmpStr = "No probe is found for following primer pairs under current criteria:\r\n";
                MDlg->insertRichText(tmpStr, emFormat);
            }
        }


        MDlg->exec();
        if(!MDlg->retry && MDlg->cont){ //Continue, do nothing.
            delete MDlg;
            if(tabCnt != 0)
                tpWindow->show();
            else{
                delete tpWindow;
            }
            break;
        }
        else{ // retry
            DataTools::cleanMemoryProbeData();
            delete tpWindow;
            if(!MDlg->retry){
                break;
            }
        }
        pWprobe->setStartId(Page_pCand);
    }
    pWprobe->close();
    return;
}

void PrimerReportWindow::on_actionDesign_Probes_for_marked_primer_pairs_triggered()
{
    if( primerPairShowList.size() == 0 )
        return;

    int numPrimerMarked = 0;
    getMarkedList();
    numPrimerMarked = markedList.size();

    //QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if( numPrimerMarked <= 0 ){
        QMessageBox::about(this,"marked error","Please mark one primer pair to analyze.\r\n\r\n"
                            "(Primer pairs are selected by clicking on any part "
                            "of the row except the Primer Pair No.\r\n"
                            "Selected primer pairs are designated by blue shading.)" );
        return;
    }

    QString errorlog = "";
    QTime time = QTime::currentTime();
    errorlog += time.toString() + "\t Design Probes for:\n";
    for(int i = 0; i < markedList.size(); i++){
        PrimerPair* currPr = primerPairShowList[markedList[i]];
        errorlog += "\t\t "+ currPr->left + ".." + currPr->rightRevsComp + "\n";
    }
    DataTools::writeErrorLog(errorlog);

    designProbes4markedPrimerPairs(markedList);
}

void PrimerReportWindow::on_actionView_Primer_Design_Setting_triggered()
{
    PrimerSettingReportDlg *psReportDlg = new PrimerSettingReportDlg(false, this);
    psReportDlg->exec();
    delete psReportDlg;
}

//void PrimerReportWindow::on_actionProbe_3_Intra_Complementarity_triggered()
//{
//
//}

void PrimerReportWindow::on_actionCompare_with_PP_data_triggered()
{
    QFileDialog file_dialog(this);
    file_dialog.setFileMode( QFileDialog::ExistingFile );
    QString filename = file_dialog.getOpenFileName( this, tr( "Load Primer List from PP" ), pwd, tr( "Primer List File (*.primer);;All File (*.*)" )) ;
    if(filename.isEmpty()){
        return;
    }
    DataTools::buildSimCost(primerCostType);
    DataTools::loadPpResult(filename, this);
    //DataTools::cleanMemoryScoringTable();

    this->emptyModel();
    ui->textEdit->clear();

    std::stable_sort( primerPairList.begin(), primerPairList.end(), less_formula );
    std::stable_sort( primerPairShowList.begin(), primerPairShowList.end(), less_formula );
    this->insertShownPrimerList();
}


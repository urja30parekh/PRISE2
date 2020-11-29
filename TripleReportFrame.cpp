#include "TripleReportFrame.h"
#include "ui_TripleReportFrame.h"

TripleReportFrame::TripleReportFrame(PrimerPair* pair,QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TripleReportFrame)
{
    ui->setupUi(this);
    //setCentralWidget(this->ui->widget);
    tableModel =  new QStandardItemModel(0, B_NUM_COLUMNS, this);
    ui->tableView->setEditTriggers(Q_NULLPTR);

    int numTar = tarSeqs.size();

    pr = pair;

    tripleColumns.clear();
    tripleColumnDisp.clear();
    tripleColumnHide.clear();
    tripleColumns.push_back("");                                          tripleColWidth[B_TRIPLE_NO] = 30;
    //tripleColumns.push_back("Primer Pair");                               tripleColWidth[B_PRIMER_PAIR] = 200;
    tripleColumns.push_back("Probe");                                     tripleColWidth[B_PROBE] = 200;
    tripleColumns.push_back("Target\nprimer-probe set\n%");                           tripleColWidth[B_TARGET_PCTG] = 55;
    //tripleColumns.push_back("Target\nforward\n%");                        tripleColWidth[B_LFT_TARGET_PCTG] = 55;
    //tripleColumns.push_back("Target\nreverse\n%");                        tripleColWidth[B_RHT_TARGET_PCTG] = 55;
    tripleColumns.push_back("Target\nprobe\n%");                          tripleColWidth[B_PROBE_TARGET_PCTG] = 55;
    tripleColumns.push_back("Non-target\nprimer-probe set\n%");                       tripleColWidth[B_NONTAR_PCTG] = 65;
    //tripleColumns.push_back("Non-Target\nforward\n%");                    tripleColWidth[B_LFT_NONTAR_PCTG] = 65;
    //tripleColumns.push_back("Non-Target\nreverse\n%");                    tripleColWidth[B_RHT_NONTAR_PCTG] = 65;
    tripleColumns.push_back("Non-target\nprobe\n%");                      tripleColWidth[B_PROBE_NONTAR_PCTG] = 65;
    //tripleColumns.push_back("Target\nforward\nAnnealing Avg Pos");        tripleColWidth[B_LFT_TAR_BIND_POS] = 105;
    //tripleColumns.push_back("Target\nreverse\nAnnealing Avg Pos");        tripleColWidth[B_RHT_TAR_BIND_POS] = 105;
    tripleColumns.push_back("Target\nprobe\nannealing avg pos");          tripleColWidth[B_PROBE_TAR_BIND_POS] = 105;
    //tripleColumns.push_back("Target\nPCR Avg\nSize");                     tripleColWidth[B_TAR_PCR_AVG] = 55;
    //tripleColumns.push_back("Target\nPCR Min\nSize");                     tripleColWidth[B_TAR_PCR_MIN] = 55;
    //tripleColumns.push_back("Target\nPCR Max\nSize");                     tripleColWidth[B_TAR_PCR_MAX] = 55;
    //tripleColumns.push_back("Non-Target\nPCR Avg\nSize");                 tripleColWidth[B_NONTAR_PCR_AVG] = 65;
    //tripleColumns.push_back("Non-Target\nPCR Min\nSize");                 tripleColWidth[B_NONTAR_PCR_MIN] = 65;
    //tripleColumns.push_back("Non-Target\nPCR Max\nSize");                 tripleColWidth[B_NONTAR_PCR_MAX] = 65;
    //tripleColumns.push_back("\n(Fw)\nGC%");                               tripleColWidth[B_LFT_GC] = 35;
    //tripleColumns.push_back("\n(Rv)\nGC% ");                              tripleColWidth[B_RHT_GC] = 35;
    tripleColumns.push_back("\nProbe\nGC% ");                           tripleColWidth[B_PROBE_GC] = 35;
    //tripleColumns.push_back("\n(Fw)\nTm(C)");                             tripleColWidth[B_LFT_TM] = 40;
    //tripleColumns.push_back("\n(Rv)\nTm(C)");                             tripleColWidth[B_RHT_TM] = 40;
    tripleColumns.push_back("\nProbe\nTm(C)");                          tripleColWidth[B_PROBE_TM] = 40;
    tripleColumns.push_back("Inter \ncomple-\nmentarity");                tripleColWidth[B_INTER_COMP] = 70;
    //tripleColumns.push_back("3' inter \ncomple-\nmentarity");             tripleColWidth[B_INTER3_COMP] = 70;
    //tripleColumns.push_back("(Fw)Primer Intra-\nComple-\nmentarity");     tripleColWidth[B_LFT_INTRA_COMP] = 70;
    //tripleColumns.push_back("(Rv)Primer Intra-\nComple-\nmentarity");     tripleColWidth[B_RHT_INTRA_COMP] = 70;
    tripleColumns.push_back("Probe intra \ncomple-\nmentarity");          tripleColWidth[B_PROBE_INTRA_COMP] = 70;
    //tripleColumns.push_back("(Fw)Primer 3' Intra-\nComple-\nmentarity");  tripleColWidth[B_LFT_INTRA3_COMP] = 80;
    //tripleColumns.push_back("(Rv)Primer 3' Intra-\nComple-\nmentarity");  tripleColWidth[B_RHT_INTRA3_COMP] = 80;
    //tripleColumns.push_back("Probe 3' intra \ncomple-\nmentarity");       tripleColWidth[B_PROBE_INTRA3_COMP] = 80;
    //tripleColumns.push_back("# of target sequences\nthat probe binds to\nambiguous bases");        tripleColWidth[B_NUM_AMBI_TARGETS] = 115;
    tripleColumns.push_back(QString("# of target sequences\n(out of %1)\nwith ambiguous bases\ncovered by probe").arg(numTar));        tripleColWidth[B_NUM_AMBI_TARGETS] = 115;
    tripleColumns.push_back("# of ambiguous bases\nin target sequences\nthat probe binds to");    tripleColWidth[B_NUM_AMBI_BASES] = 120;
    //tripleColumns.push_back("Total # of\ntarget\nsequences");             tripleColWidth[B_NUM_TAR] = 60;
    //tripleColumnDisp = QStringList(tripleColumns);


    // for sort order of each column and save or notcreateModel for each column in save excel dialog
    for( int i = 0; i < B_NUM_COLUMNS; i++ ){
        tripleColSortOrder[i] = true;
        tripleExcelSaveColumns[i] = true;
        tripleColumnDisp.push_back(i);
    }
    this->createModel();

    dispTripleSel = -1;
    addTripleMauallyReady = false;
    this->ui->textEdit->setPlainText("");

    // for display triple list conditionally, there are 4 conditions
    for( int i = 0; i < 4; i++ ){
        dispTripleCondTitle[i] = 0;
        dispTripleCondGtLt[i] = -1;
        dispTripleCondValue[i] = 0.0;
        dispTripleCondAllowCancel = false;
    }
    // for probe list sorting by multiple keys
    tripleSortBy[0] = SORT_BY_FORMULA;           tripleColSortOrder[0] = ASCENDING;
    tripleSortBy[1] = -1;					tripleColSortOrder[1] = ASCENDING;
    tripleSortBy[2] = -1;					tripleColSortOrder[2] = ASCENDING;
    tripleSortBy[3] = -1;					tripleColSortOrder[3] = ASCENDING;

    // for probe list save: 1 for save all displayed probes
    tripleListSaveAllDispMark = 1;
    tripleExcelSaveAllDispMark = 1;

    // for probe annealing content user constraints
    //bindingContentDivider3end = 8;
    //bindingContentMatchPctg5end = 50;
    //bindingContentMatchPctg3end = 62.5;


    if (pair != Q_NULLPTR)
        displayData();
}

TripleReportFrame::~TripleReportFrame()
{
    delete ui;
}

bool less_formula(const Triple* m1, const Triple* m2)
{
    //PrimerPair *pr1 = m1->Pr;
    //PrimerPair *pr2 = m2->Pr;
    double a = pow(100 - m1->pctgInTargetSeqs, 2 )
             + pow( m1->pctgInNonTarSeqs, 2 )
             + 0.5*(100 - static_cast<double>(m1->Pb->pctgInTargetSeqs))
             + 0.25*(static_cast<double>(m1->Pb->pctgInNonTarSeqs));
    double b = pow(100 - m2->pctgInTargetSeqs, 2 )
             + pow( m2->pctgInNonTarSeqs, 2 )
             + 0.5*(100 - static_cast<double>(m2->Pb->pctgInTargetSeqs))
             + 0.25*(static_cast<double>(m2->Pb->pctgInNonTarSeqs));
    return a < b;
}

int TripleReportFrame::getTripleListSize()
{
    return pr->tripleList.size();
}

int TripleReportFrame::getTripleShowListSize()
{
    return tripleShowList.size();
}

/*int TripleReportFrame::setPrimerPair(PrimerPair* pair)
{
    this->pr = pair;
}*/


int TripleReportFrame::getMarkedNum()
{
    getMarkedList();
    return markedList.size();
}

void TripleReportFrame::getMarkedList()
{
    markedList.clear();
    for(int i = 0; i < tableModel->rowCount(); i++){
        if(tableModel->item(i,0)->checkState() == Qt::Checked)
            markedList.push_back(i);
    }
}

bool TripleReportFrame::createModel()
{
    ui->tableView->setModel(tableModel);
    ui->tableView->setSortingEnabled(true);
    for( int i = 0; i < B_NUM_COLUMNS; i++ ) {
        tableModel->setHeaderData(i, Qt::Horizontal, tripleColumns[i]);
        ui->tableView->setColumnWidth(i,tripleColWidth[i]);
    }

    QHeaderView * HV = ui->tableView->horizontalHeader();
    connect(HV, SIGNAL(sectionClicked(int)), this, SLOT(sortData(int)));

    return true;
}

void TripleReportFrame::emptyModel()
{
    for(int i = 0; i < tableModel->rowCount(); i++){
        delete tableModel->item(i,0);
    }
    tableModel->removeRows(0,tableModel->rowCount());
}

bool TripleReportFrame::close()
{
    tripleShowList.clear();
    DataTools::cleanMemoryTriple(pr);
    QWidget::close();
    return 0;
}

void TripleReportFrame::sortData(int idx)
{
    tripleColSortOrder[idx] = 1-tripleColSortOrder[idx];
    int adescending =  tripleColSortOrder[idx];

    //MyProgressDlg *pDlg = new MyProgressDlg("Sorting probe list",0, 1, this);
    //pDlg->setWindowModality(Qt::ApplicationModal);

    //emptyModel();
    //int range = tripleShowList.size();
    //int numTriples = range;
    //pDlg->setRange(0,range);
    int numTriples = tripleShowList.size();

    Triple *tp;

    // sort probe list base on probe pair No.
    if( idx == B_TRIPLE_NO ){ // sort marked / unmarked probe pair
        if( adescending == 0 ){ // marked first, followed by unmarked
            for( int i = 1; i < numTriples; i++ ){
                for( int j = 0; j < i; j++ ){
                    if( tripleShowList[j]->marked && !tripleShowList[i]->marked ){
                        tp = tripleShowList[i];
                        tripleShowList[i] = tripleShowList[j];
                        tripleShowList[j] = tp;
                    }
                }
                //pDlg->setValue(i);
            }
        }else{ // unmarked first, followed by marked
            for( int i = 1; i < numTriples; i++ ){
                for( int j = 0; j < i; j++ ){
                    if( !tripleShowList[j]->marked && tripleShowList[i]->marked ){
                        tp = tripleShowList[i];
                        tripleShowList[i] = tripleShowList[j];
                        tripleShowList[j] = tp;
                    }
                }
                //pDlg->setValue(i);
            }
        }

        //delete pDlg;
        insertShownTripleList();
        return;
    }

    //QStringList prSeqListLeft;
    //QStringList prSeqListRight;
    QStringList pbSeqList;
    for( int i = 0; i < numTriples; i++ ){
        //PrimerPair *pr = tripleShowList[i]->Pr;
        //prSeqListLeft.push_back(pr->left);
        //prSeqListRight.push_back(pr->rightRevsComp );
        pbSeqList.push_back(tripleShowList[i]->Pb->probeSeq);
    }

    QString tPrSeq;
    QString tPbSeq;

    /*if( idx == B_PRIMER_PAIR ){ // sort triple list base on primer pair seq
        QString tPrSeq;
        // ascending order
        if( adescending == 0 ){
            // insertion sort according to baseArray
            for( int i = 1; i < numTriples; i++ ){
                for( int j = 0; j < i; j++){
                    if( prSeqListLeft[j] > prSeqListLeft[i] || (prSeqListLeft[j] == prSeqListLeft[i] && prSeqListRight[j] > prSeqListRight[i])){
                        tPrSeq = prSeqListLeft[i];
                        prSeqListLeft[i] = prSeqListLeft[j];
                        prSeqListLeft[j] = tPrSeq;

                        tPrSeq = prSeqListRight[i];
                        prSeqListRight[i] = prSeqListRight[j];
                        prSeqListRight[j] = tPrSeq;

                        tPbSeq = pbSeqList[i];
                        pbSeqList[i] = pbSeqList[j];
                        pbSeqList[j] = tPrSeq;

                        tp = tripleShowList[i];
                        tripleShowList[i] = tripleShowList[j];
                        tripleShowList[j] = tp;
                    }
                }
                pDlg->setValue(i);
            }
        }else{ // descending order
            // insertion sort according to baseArray
            for( int i = 1; i < numTriples; i++ ){
                for( int j = 0; j < i; j++){
                    if( prSeqListLeft[j] < prSeqListLeft[i] || (prSeqListLeft[j] == prSeqListLeft[i] && prSeqListRight[j] < prSeqListRight[i])){
                        tPrSeq = prSeqListLeft[i];
                        prSeqListLeft[i] = prSeqListLeft[j];
                        prSeqListLeft[j] = tPrSeq;

                        tPrSeq = prSeqListRight[i];
                        prSeqListRight[i] = prSeqListRight[j];
                        prSeqListRight[j] = tPrSeq;

                        tPbSeq = pbSeqList[i];
                        pbSeqList[i] = pbSeqList[j];
                        pbSeqList[j] = tPrSeq;

                        tp = tripleShowList[i];
                        tripleShowList[i] = tripleShowList[j];
                        tripleShowList[j] = tp;
                    }
                }
                pDlg->setValue(i);
            }
        }
        delete pDlg;

        insertShownTripleList();
        return;
    }*/

    if( idx == B_PROBE){// sort triple list base on probe seq
        // ascending order

        if( adescending == 0 ){
            // insertion sort according to baseArray
            for( int i = 1; i < numTriples; i++ ){
                for( int j = 0; j < i; j++){
                    if(pbSeqList[j] > pbSeqList[i]){
                        /*tPrSeq = prSeqListLeft[i];
                        prSeqListLeft[i] = prSeqListLeft[j];
                        prSeqListLeft[j] = tPrSeq;

                        tPrSeq = prSeqListRight[i];
                        prSeqListRight[i] = prSeqListRight[j];
                        prSeqListRight[j] = tPrSeq;*/

                        tPbSeq = pbSeqList[i];
                        pbSeqList[i] = pbSeqList[j];
                        pbSeqList[j] = tPbSeq;

                        tp = tripleShowList[i];
                        tripleShowList[i] = tripleShowList[j];
                        tripleShowList[j] = tp;
                    }
                }
                //pDlg->setValue(i);
            }
        }else{ // descending order
            // insertion sort according to baseArray
            for( int i = 1; i < numTriples; i++ ){
                for( int j = 0; j < i; j++){
                    if( pbSeqList[j] < pbSeqList[i]){
                        /*tPrSeq = prSeqListLeft[i];
                        prSeqListLeft[i] = prSeqListLeft[j];
                        prSeqListLeft[j] = tPrSeq;

                        tPrSeq = prSeqListRight[i];
                        prSeqListRight[i] = prSeqListRight[j];
                        prSeqListRight[j] = tPrSeq;*/

                        tPbSeq = pbSeqList[i];
                        pbSeqList[i] = pbSeqList[j];
                        pbSeqList[j] = tPbSeq;

                        tp = tripleShowList[i];
                        tripleShowList[i] = tripleShowList[j];
                        tripleShowList[j] = tp;
                    }
                }
                //pDlg->setValue(i);
            }
        }
        //delete pDlg;

        insertShownTripleList();
        return;
    }

    float *baseArray;
    try{
        baseArray = new float[static_cast<unsigned long>(numTriples)];
    }
    catch( std::bad_alloc& ){
        QMessageBox::about(this,"Fatal error","Fatal error: out of memory for sorting.\nProgram is forced to terminate.");
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
        //DataTools::cleanMemoryProbeData();
        this->tripleShowList.clear();
        DataTools::cleanMemoryTriple(pr);
        this->close();
    }


    for( int i = 0; i < numTriples; i++ )
        baseArray[i] = DataTools::getTripleInfo( tripleShowList[i], idx );


    float temp;
    // ascending order
    if( adescending == 0 ){
        // insertion sort according to baseArray
        for( int i = 1; i < numTriples; i++ ){
            for( int j = 0; j < i; j++){

                if( baseArray[j] > baseArray[i]){

                    /*tPrSeq = prSeqListLeft[i];
                    prSeqListLeft[i] = prSeqListLeft[j];
                    prSeqListLeft[j] = tPrSeq;

                    tPrSeq = prSeqListRight[i];
                    prSeqListRight[i] = prSeqListRight[j];
                    prSeqListRight[j] = tPrSeq;*/

                    tPbSeq = pbSeqList[i];
                    pbSeqList[i] = pbSeqList[j];
                    pbSeqList[j] = tPbSeq;

                    temp = baseArray[i];
                    baseArray[i] = baseArray[j];
                    baseArray[j] = temp;

                    tp = tripleShowList[i];
                    tripleShowList[i] = tripleShowList[j];
                    tripleShowList[j] = tp;
                }
            }
            //pDlg->setValue(i);
        }
    }else{ // descending order
        // insertion sort according to baseArray
        for( int i = 1; i < numTriples; i++ ){
            for( int j = 0; j < i; j++){
                if(baseArray[j] < baseArray[i]){

                    /*tPrSeq = prSeqListLeft[i];
                    prSeqListLeft[i] = prSeqListLeft[j];
                    prSeqListLeft[j] = tPrSeq;

                    tPrSeq = prSeqListRight[i];
                    prSeqListRight[i] = prSeqListRight[j];
                    prSeqListRight[j] = tPrSeq;*/

                    tPbSeq = pbSeqList[i];
                    pbSeqList[i] = pbSeqList[j];
                    pbSeqList[j] = tPrSeq;

                    temp = baseArray[i];
                    baseArray[i] = baseArray[j];
                    baseArray[j] = temp;

                    tp = tripleShowList[i];
                    tripleShowList[i] = tripleShowList[j];
                    tripleShowList[j] = tp;
                }
            }
            //pDlg->setValue(i);
        }
    }

    delete[] baseArray;
    //delete pDlg;
    insertShownTripleList();
    return;
}


void TripleReportFrame::insertData(int dataIdx, int rowIdx)
{
    if (rowIdx == -1){
        rowIdx = tableModel->rowCount();
        tableModel->insertRow(rowIdx);
    }

    Triple *tp = tripleShowList[dataIdx];
    //PrimerPair *pr=tp->Pr;
    Probe *pb = tp->Pb;



    QStandardItem* __tmpItem = new QStandardItem();
    __tmpItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
    tableModel->setItem(rowIdx,0,__tmpItem);
    if(tp->marked == true) {
        __tmpItem->setCheckState(Qt::Unchecked);
        tableModel->setData(tableModel->index(rowIdx, 0), "");
    } else if(tp->marked == false) {
        __tmpItem->setCheckState(Qt::Checked);
        tableModel->setData(tableModel->index(rowIdx, 0), " ");
    } else {
        QMessageBox::about(this, "Abort", "Sequecen mark error\n"
                           "please contact the developer\n");
        return;
    }

    // primer pair
    //tableModel->setData(tableModel->index(rowIdx, B_PRIMER_PAIR), pr->left + ".." + pr->rightRevsComp);
    // probe
    tableModel->setData(tableModel->index(rowIdx, B_PROBE), pb->probeSeq);


    // frequency for single primers
    QString val;
    val.sprintf("%.2f", static_cast<double>(tp->pctgInTargetSeqs));
    tableModel->setData(tableModel->index(rowIdx, B_TARGET_PCTG),  val);
    tableModel->item(rowIdx, B_TARGET_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    //tableModel->setData(tableModel->index(rowIdx, B_LFT_TARGET_PCTG),  pr->pctgInTarSeqsLeft);
    //tableModel->setData(tableModel->index(rowIdx, B_RHT_TARGET_PCTG),  pr->pctgInTarSeqsRight);
    val.sprintf("%.2f", static_cast<double>(pb->pctgInTargetSeqs));
    tableModel->setData(tableModel->index(rowIdx, B_PROBE_TARGET_PCTG),  val);
    tableModel->item(rowIdx, B_PROBE_TARGET_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);

    val.sprintf("%.2f", static_cast<double>(tp->pctgInNonTarSeqs));
    tableModel->setData(tableModel->index(rowIdx, B_NONTAR_PCTG),  val);
    tableModel->item(rowIdx, B_NONTAR_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    //tableModel->setData(tableModel->index(rowIdx, B_LFT_NONTAR_PCTG),  pr->pctgInNonTarSeqsLeft);
    //tableModel->setData(tableModel->index(rowIdx, B_RHT_NONTAR_PCTG),  pr->pctgInNonTarSeqsRight);
    val.sprintf("%.2f", static_cast<double>(pb->pctgInNonTarSeqs));
    tableModel->setData(tableModel->index(rowIdx, B_PROBE_NONTAR_PCTG),  val);
    tableModel->item(rowIdx, B_PROBE_NONTAR_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);

    // average annealing position in target sequences:
    /*if( pr->bindPosTarSeqsAvgLeft < 0 )
        tableModel->setData(tableModel->index(rowIdx, B_LFT_TAR_BIND_POS),  "-");
    else{
        tableModel->setData(tableModel->index(rowIdx, B_LFT_TAR_BIND_POS),  pr->bindPosTarSeqsAvgLeft);
    }
    if( pr->bindPosTarSeqsAvgRight < 0 )
        tableModel->setData(tableModel->index(rowIdx, B_RHT_TAR_BIND_POS),  "-");
    else{
        tableModel->setData(tableModel->index(rowIdx, B_RHT_TAR_BIND_POS),  pr->bindPosTarSeqsAvgRight);
    }*/
    if( pb->bindPosTarSeqsAvg[0]< 0 )
        tableModel->setData(tableModel->index(rowIdx, B_PROBE_TAR_BIND_POS),  "-");
    else{
        val.sprintf("%.2f", static_cast<double>(pb->bindPosTarSeqsAvg[0]));
        tableModel->setData(tableModel->index(rowIdx, B_PROBE_TAR_BIND_POS),  val);
        tableModel->item(rowIdx, B_PROBE_TAR_BIND_POS)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    }
/*
    tableModel->setData(tableModel->index(rowIdx, B_TAR_PCR_AVG),  pr->pcrSizeAvgTarSeqs);
    tableModel->setData(tableModel->index(rowIdx, B_TAR_PCR_MIN),  pr->pcrSizeMinTarSeqs);
    tableModel->setData(tableModel->index(rowIdx, B_TAR_PCR_MAX),  pr->pcrSizeMaxTarSeqs);

    tableModel->setData(tableModel->index(rowIdx, B_NONTAR_PCR_AVG),  pr->pcrSizeAvgNonTarSeqs);
    tableModel->setData(tableModel->index(rowIdx, B_NONTAR_PCR_MIN),  pr->pcrSizeMinNonTarSeqs);
    tableModel->setData(tableModel->index(rowIdx, B_NONTAR_PCR_MAX),  pr->pcrSizeMaxNonTarSeqs);
*/

    // left primer GC%
    //tableModel->setData(tableModel->index(rowIdx, B_LFT_GC),  pr->GCLeft);
    // right primer GC%
    //tableModel->setData(tableModel->index(rowIdx, B_RHT_GC),  pr->GCRight);
    // probe GC%
    val.sprintf("%.2f", static_cast<double>(pb->GCNum));
    tableModel->setData(tableModel->index(rowIdx, B_PROBE_GC),  val);
    tableModel->item(rowIdx, B_PROBE_GC)->setData(Qt::AlignRight, Qt::TextAlignmentRole);


    // left primer Tm
    //tableModel->setData(tableModel->index(rowIdx, B_LFT_TM),  pr->tmLeft);
    // right primer Tm
    //tableModel->setData(tableModel->index(rowIdx, B_RHT_TM),  pr->tmRight);
    // probe Tm
    val.sprintf("%.2f", static_cast<double>(pb->tmProbe));
    tableModel->setData(tableModel->index(rowIdx, B_PROBE_TM),  val);
    tableModel->item(rowIdx, B_PROBE_TM)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    // inter-complementarity
    tableModel->setData(tableModel->index(rowIdx, B_INTER_COMP),  tp->pbPrComp);
    tableModel->item(rowIdx, B_INTER_COMP)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    // 3' inter-complementarity
    //tableModel->setData(tableModel->index(rowIdx, B_INTER3_COMP),  tp->pbPr3Comp);
    //tableModel->item(rowIdx, B_INTER3_COMP)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    // self-complementarity
    //tableModel->setData(tableModel->index(rowIdx, B_LFT_INTRA_COMP),  pr->selfCompLeft);
    //tableModel->setData(tableModel->index(rowIdx, B_RHT_INTRA_COMP),  pr->selfCompRight);
    tableModel->setData(tableModel->index(rowIdx, B_PROBE_INTRA_COMP),  pb->selfComp);
    tableModel->item(rowIdx, B_PROBE_INTRA_COMP)->setData(Qt::AlignRight, Qt::TextAlignmentRole);

    // 3'self-complementarity
    //tableModel->setData(tableModel->index(rowIdx, B_LFT_INTRA3_COMP),  pr->self3CompLeft);
    //tableModel->setData(tableModel->index(rowIdx, B_RHT_INTRA3_COMP),  pr->self3CompRight);
    //tableModel->setData(tableModel->index(rowIdx, B_PROBE_INTRA3_COMP),  pb->self3Comp);
    //tableModel->item(rowIdx, B_PROBE_INTRA3_COMP)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    tableModel->setData(tableModel->index(rowIdx, B_NUM_AMBI_TARGETS),  pb->numAmbiguousTargets);
    tableModel->item(rowIdx, B_NUM_AMBI_TARGETS)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    tableModel->setData(tableModel->index(rowIdx, B_NUM_AMBI_BASES),  pb->numAmbiguousBases);
    tableModel->item(rowIdx, B_NUM_AMBI_BASES)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    //tableModel->setData(tableModel->index(rowIdx, B_NUM_TAR),  tarSeqs.size());
}


void TripleReportFrame::displayData()
{
    // sort on ascending order of probe quality formula
    std::stable_sort( pr->tripleList.begin(), pr->tripleList.end(), less_formula );

    // probe list is large, suggest for partial displaying
    /*if( pr->tripleList.size() > 1000 ){
        dispTripleCondAllowCancel = FALSE;
        TripleDispListDlg dispDlg(this);
        if (dispDlg.exec()){
            copyDispTripleList();
        }else{
            showWindow = false;
            close();
        }
    }*/
    //else{ // probe list is small, copy them all

    tripleShowList.clear();
    for(int i = 0; i < pr->tripleList.size(); i++ )
        tripleShowList.push_back( pr->tripleList[i] );

    //}

    this->insertShownTripleList();
    dispTripleCondAllowCancel = false;
}
/*
void TripleReportFrame::setTableColumnWidth(int column, int width)
{
    ui->tableView->setColumnWidth(column, width);
}

void TripleReportFrame::setTableColumnHidden(int column, bool hidden)
{
    ui->tableView->setColumnHidden(column, hidden);
}
*/
QString TripleReportFrame::getInfoText()
{
    return ui->textEdit->toPlainText();
}

QString TripleReportFrame::getPrimerSeq()
{
    return pr->left+".."+pr->rightRevsComp;
}

PrimerPair* TripleReportFrame::getPrimerPair()
{
    return this->pr;
}

void TripleReportFrame::addTripleShowList( Triple* tr, int pos)
{
    switch(pos){
    case 0:
        this->tripleShowList.push_front(tr);
        break;
    case 1:
        this->tripleShowList.push_back(tr);
        break;
    default:
        return;
    }
}

void TripleReportFrame::copyDispTripleList()
{
    int range = pr->tripleList.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Filtering triple list",0, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);

    emptyModel();
    ui->textEdit->clear();
    tripleShowList.clear();


    if( dispTripleAllorTopX == 0 ){// user chooses to display triple by conditions
        Triple *tp;
        bool passChecking;

        for(int i = 0; i < pr->tripleList.size(); i++ )
            tripleShowList.push_back( pr->tripleList[i] );

        // check for each triple
        for(int i = 0, k = 0; i < tripleShowList.size(); i++, k++ ){
            tp = tripleShowList[i];

            passChecking = checkTripleWithCondition( tp, dispTripleCondTitle, dispTripleCondGtLt, dispTripleCondValue );

            if( !passChecking){	// didn't pass all the checks, remove it from this->tripleShowList
                tripleShowList.erase( tripleShowList.begin()+i );
                i--;
            }
            pDlg->setValue(k+1);
        }
    }

    else if( dispTripleAllorTopX == 1 ){ // user chooses to display all triple
        // copy all probes
        for(int i = 0; i < pr->tripleList.size(); i++ )
            tripleShowList.push_back( pr->tripleList[i] );
    }
    else if( dispTripleAllorTopX == 2 ){ // user chooses to display top X probes
        // copy top X probe
        for(int i = 0; ( i < dispTripleTopNum && i < pr->tripleList.size() ); i++ )
            tripleShowList.push_back( pr->tripleList[i] );
    }
    delete pDlg;
}


bool TripleReportFrame::checkTripleWithCondition(Triple* tp, int *condTitle, int *condGtLt, float *condValue)
{
    bool passChecking = true;
    // check for all 4 conditions
    for( int i = 0; i < 4; i++ ){
        //int v = static_cast<int>(DataTools::getTripleInfo(tp, condTitle[i]));
        float v = DataTools::getTripleInfo( tp, condTitle[i] );
        if( condTitle[i] <= B_PROBE )	// skip checking this condition if it's "triple No" or "triple Seqs"
            continue;
        if( condGtLt[i] == LESSTHAN && v >= condValue[i] )
            passChecking = false;
        if( condGtLt[i] == GREATERTHAN && v <= condValue[i] )
            passChecking = false;
    }

    return passChecking;
}

void TripleReportFrame::insertShownTripleList()
{
    int range = tripleShowList.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Adding triples to the table",0, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);

    emptyModel();
    tableModel->insertRows(0,tripleShowList.size());
    // add all probes in result list one by one
    for( int i = 0; i < tripleShowList.size(); i++ ){
        insertData(i,i);
        pDlg->setValue(i+1);
    }
    delete pDlg;
}

void TripleReportFrame::appendSeq2Rich(QString seqName, short *seqInt, int *pos)
{
    // change seqInt to CString
    QString sequence;
    tools::seqInt2Str(seqInt,sequence);

    ui->textEdit->append(seqName);
    //int len = ui->textEdit->toPlainText().length();


    // primer appear in sequence, highlight primer pair
    if(pos[4] >= 0)//{
        sequence.replace(pos[4], pos[5]-pos[4]+1, "<span style=\" color:#ff0000;\">"+sequence.mid(pos[4], pos[5]-pos[4]+1)+"</span>");
    if(pos[2] >= 0)
        sequence.replace(pos[2], pos[3]-pos[2]+1, "<span style=\" color:#00c800;\">"+sequence.mid(pos[2], pos[3]-pos[2]+1)+"</span>");
    if(pos[0] >= 0)
        sequence.replace(pos[0], pos[1]-pos[0]+1, "<span style=\" color:#ff0000;\">"+sequence.mid(pos[0], pos[1]-pos[0]+1)+"</span>");
    //}

    sequence = "<span style=\" color:#000000;\">"+sequence +"</span>";

    ui->textEdit->append(sequence);
    ui->textEdit->append("");
}

int TripleReportFrame::getCurrSelectedItem()
{
    if( tripleShowList.size() == 0 )
           return 0;

   QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
   if( selectedRows.size() <= 0 ||selectedRows.size() > 1 ){
       QMessageBox::about(this,"selection error","Please select one probe to analyze.\r\n\r\n"
                               "(Probes are selected by clicking on any part "
                               "of the row except the Probe No.\r\n"
                               "Selected probes are designated by blue shading.)" );
       return 0;
   }

   dispTripleSel = selectedRows.at(0).row();
   return 1;
}

void TripleReportFrame::loadTripleList(QString filename){
    short * seqptr;
    int numTarSeqs;
    int numNontarSeqs;
    int numTriples;
    //PrimerPair *pr;
    Probe *pb;
    Triple* tp;
    //Open Probe File.
    QFile checkFile(filename);
    if (!checkFile.open(QFile::ReadOnly)){

        QMessageBox QMBox;
        QMBox.setWindowTitle("File Error");
        QMBox.setText("File doesn't exist or File corrupted");
        QMBox.exec();
        //QMessageBox::about(0, "File Error", "File doesn't exist or File corrupted");
        return;
    }
    QFile file(filename);
    if (!file.open(QFile::ReadOnly)){
        QMessageBox QMBox;
        QMBox.setWindowTitle("File Error");
        QMBox.setText("File doesn't exist or File corrupted");
        QMBox.exec();
        //QMessageBox::about(0, "File Error", "File doesn't exist or File corrupted");
        return;
    }
    QTextStream streamTmp ( &checkFile );
    QString line  = streamTmp.readLine();
    checkFile.close();

    // check file type
    QByteArray compressedData, origData;

    if(line.startsWith("Triple list file")){ //uncompressed file
        origData = file.readAll();
    }else{ //compressed file
        compressedData = file.readAll();
        origData = qUncompress(compressedData);
    }

    QTextStream stream(&origData);
    line = stream.readLine();
    if(line.compare("Triple list file 1.0. Please do not modify.")){
        QMessageBox QMBox;
        QMBox.setWindowTitle("File Error");
        QMBox.setText(QString("%1 is not a triple list file!").arg(filename));
        QMBox.exec();
        //QMessageBox::about(0, "File Error", QString("%1 is not a triple list file!").arg(filename));
        file.close();
        return;
    }

    //int iEstimate;
    line = stream.readLine();

    line = stream.readLine();
    stream >> probeLenMin >> probeLenMax;
    line = stream.readLine();

    line = stream.readLine();
    stream >> probeCandidFrequency;
    line = stream.readLine();

    line = stream.readLine();
    stream >> probeMinGap;
    line = stream.readLine();


    int iAvoid;
    line = stream.readLine();
    stream >> iAvoid;
    probeAvoidACT = static_cast<bool>(iAvoid);

    stream >> iAvoid;
    probeAvoidG = static_cast<bool>(iAvoid);

    stream >> iAvoid;
    probeAvoidGat5end = static_cast<bool>(iAvoid);
    line = stream.readLine();

    line = stream.readLine();
    stream >> probeNumACT >> probeNumG;
    line = stream.readLine();

    //line = stream.readLine();
    //stream >> iEstimate;
    //probeEstimate = (bool)iEstimate;
    //line = stream.readLine();

    line = stream.readLine();
    stream >> probeTmMin >> probeTmMax;
    line = stream.readLine();

    line = stream.readLine();
    stream >> probeTmGreaterMin >> probeTmGreaterMax;
    line = stream.readLine();

    line = stream.readLine();
    stream >> probeNaConsent;
    line = stream.readLine();

    line = stream.readLine();
    stream >> probeGCMin >> probeGCMax;
    line = stream.readLine();

    line = stream.readLine();
    stream >> maxPbPrComp;
    line = stream.readLine();

    //line = stream.readLine();
    //stream >> max3PbPrComp;
    //line = stream.readLine();

    //line = stream.readLine();
    //stream >> tripleInterBase3end;
    //line = stream.readLine();

    line = stream.readLine();
    stream >> probeMaxSelfComp;
    line = stream.readLine();

    //line = stream.readLine();
    //stream >> probeMax3SelfComp;
    //line = stream.readLine();

    //line = stream.readLine();
    //stream >> probeIntraBase3end;
    //line = stream.readLine();

    line = stream.readLine();
    stream >> maxIndel;
    line = stream.readLine();

    line = stream.readLine();
    stream >> indelCost;
    line = stream.readLine();

    int iProbeCostType;
    line = stream.readLine();
    stream >> iProbeCostType;
    line = stream.readLine();
    probeCostType = static_cast<bool>(iProbeCostType);

    line = stream.readLine();
    for( int i = 0; i < 15; i++ ){
        for( int j = 0; j < 15; j++ )
            stream >> simCost[i][j];
    }
    /*line = stream.readLine();
    line = stream.readLine();
    for( int i = 0; i < MAX_PRIMER_LEN; i++ )
        stream >> primerPosWtTar[i];
    line = stream.readLine();
    line = stream.readLine();
    for( int i = 0; i < MAX_PRIMER_LEN; i++ )
        stream >> primerPosWtTar[i];*/
    line = stream.readLine();
    line = stream.readLine();
    for( int i = 0; i < MAX_PROBE_LEN/2+1; i++ )
        stream >> probePosWtTar[i];
    line = stream.readLine();
    line = stream.readLine();
    for( int i = 0; i < MAX_PROBE_LEN/2+1; i++ )
        stream >> probePosWtNonTar[i];

    line = stream.readLine();
    line = stream.readLine();
    line = stream.readLine();


    /*Clean Memory: (for loading triple)
      1. Target&Nontar Seqs
      2. Primer Candidates
      3. Primer Pairs
      4. probe Candidates
      5. probes
      6. Triples (in Primer Pair)
    */
    DataTools::cleanMemoryAllData();
    // 3. read target sequences
    stream >> numTarSeqs;
    line = stream.readLine();
    for( int i = 0; i < numTarSeqs; i++ ){
        line = stream.readLine();
        tarSeqNames.push_back(line);

        line = stream.readLine();
        // add target sequences to theApp
        seqptr = new short[static_cast<unsigned long>(line.length()+1)];
        int j = 0;
        while( j < line.length() ){
            seqptr[j] = static_cast<short>(tools::base2int(line[j]));
            j++;
        }
        seqptr[j] = -1;
        tarSeqs.push_back(seqptr);
    }

    // 4. read non-target sequences
    stream >> numNontarSeqs;
    line = stream.readLine();
    for( int i = 0; i < numNontarSeqs; i++ ){
        line = stream.readLine();
        nontarSeqNames.push_back(line);

        line = stream.readLine();
        // add target sequences to theApp
        seqptr = new short[static_cast<unsigned long>(line.length()+1)];
        int j = 0;
        while( j < line.length()){
            seqptr[j] = static_cast<short>(tools::base2int(line[j]));
            j++;
        }
        seqptr[j] = -1;
        nontarSeqs.push_back(seqptr);
    }


    //int iIsEstimateTar;
    //int iIsEstimateNonTar;
    // 5. read corresponding primer pair
    pr = new PrimerPair();
    line = stream.readLine();
    pr->left = line;
    line = stream.readLine();
    pr->right = line;
    line = stream.readLine();
    pr->rightRevsComp = line;

    stream
        //>> iIsEstimateTar >> iIsEstimateNonTar
        >> pr->pctgInTargetSeqs >> pr->pctgInTarSeqsLeft >> pr->pctgInTarSeqsRight
        >> pr->pctgInNonTarSeqs >> pr->pctgInNonTarSeqsLeft >> pr->pctgInNonTarSeqsRight
        >> pr->bindPosTarSeqsAvgLeft >> pr->bindPosTarSeqsAvgRight
        >> pr->pcrSizeAvgTarSeqs >> pr->pcrSizeMinTarSeqs >> pr->pcrSizeMaxTarSeqs
        >> pr->pcrSizeAvgNonTarSeqs >> pr->pcrSizeMinNonTarSeqs >> pr->pcrSizeMaxNonTarSeqs
        >> pr->tmLeft >> pr->tmRight
        >> pr->GCLeft >> pr->GCRight
        >> pr->prPrComp >> pr->prPr3Comp
        >> pr->selfCompLeft >> pr->self3CompLeft >> pr->selfCompRight >> pr->self3CompRight
        >> pr->numAmbiguousTargetsLeft >> pr->numAmbiguousTargetsRight
        >> pr->numAmbiguousBasesLeft >> pr->numAmbiguousBasesRight;
    stream.readLine();
    //pr->isEstimateTar = (bool) iIsEstimateTar;
    //pr->isEstimateNonTar = (bool) iIsEstimateNonTar;


    pr->posBindingTar = new short*[2];
    for( int i = 0; i < 2; i++ ){
        pr->posBindingTar[i] = new short[static_cast<unsigned long>(numTarSeqs)];
        for( int j = 0; j < numTarSeqs; j++ )
            stream >> pr->posBindingTar[i][j];
        stream.readLine();
    }

    pr->posBindingNontar = new short*[2];
    for( int i = 0; i < 2; i++ ){
        pr->posBindingNontar[i] = new short[static_cast<unsigned long>(numNontarSeqs)];
        for( int j = 0; j < numNontarSeqs; j++ )
            stream >> pr->posBindingNontar[i][j];
        stream.readLine();
    }

    //parent->setPrimerPair(pr);

    //???????TODO: why are we assigning a variable to itself??? what good does THAT do?!?!?!!?
    //this->pr = pr;


    // 6. read triples
    stream >> numTriples;
    line = stream.readLine();
    int range = numTriples;
    MyProgressDlg *pDlg = new MyProgressDlg("Loading probe list..",0, range);
    pDlg->setWindowModality(Qt::ApplicationModal);

    for( int i = 0; i < numTriples; i++ ){
        tp = new Triple();
        pb = new Probe();
        stream >>tp->pctgInTargetSeqs >> tp->pctgInNonTarSeqs >> tp->pbPrComp;
        line = stream.readLine();
        line = stream.readLine();
        pb->probeSeq = line;

        stream
            //>>iIsEstimateTar
            //>> iIsEstimateNonTar
            >> pb->pctgInTargetSeqs
            //>> p->pctgInTarSeqsLeft
            //>> p->pctgInTarSeqsRight
            >> pb->pctgInNonTarSeqs
            //>> p->pctgInNonTarSeqsLeft
            //>> p->pctgInNonTarSeqsRight
            >> pb->bindPosTarSeqsAvg[0]
            >> pb->bindPosTarSeqsAvg[1]
            //>> p->bindPosTarSeqsAvgLeft
            //>> p->bindPosTarSeqsAvgRight
            //>> p->pcrSizeAvgTarSeqs
            //>> p->pcrSizeMinTarSeqs
            //>> p->pcrSizeMaxTarSeqs
            //>> p->pcrSizeAvgNonTarSeqs
            //>> p->pcrSizeMinNonTarSeqs
            //>> p->pcrSizeMaxNonTarSeqs
            >> pb->tmProbe
            >> pb->GCNum
            //>> p->GCLeft
            //>> p->GCRight
            //>> p->tmLeft
            //>> p->tmRight
            //>> p->prPrComp
            //>> p->prPr3Comp
            //>> p->selfCompLeft
            >> pb->selfComp
            //>> p->self3CompLeft
            //>> p->self3CompRight;
            //>> pb->self3Comp
            >> pb->numAmbiguousTargets
            >> pb->numAmbiguousBases;
        line = stream.readLine();

        pb->posBindingTar = new short*[2];
        for( int i = 0; i < 2; i++ ){
            pb->posBindingTar[i] = new short[static_cast<unsigned long>(numTarSeqs)];
            for( int j = 0; j < numTarSeqs; j++ )
                stream >> pb->posBindingTar[i][j];
            line = stream.readLine();
        }
        pb->posBindingNontar = new short*[2];
        for( int i = 0; i < 2; i++ ){
            pb->posBindingNontar[i] = new short[static_cast<unsigned long>(numNontarSeqs)];
            for( int j = 0; j < numNontarSeqs; j++ )
                stream >> pb->posBindingNontar[i][j];
            line = stream.readLine();
        }


        line = stream.readLine();
        //pb->isEstimateTar = (bool) iIsEstimateTar;
        //pb->isEstimateNonTar = (bool) iIsEstimateNonTar;
        pb->marked = 0;

        tp->Pb = pb;

        probeList.push_back(pb);
        pr->tripleList.push_back(tp);
        pDlg->setValue(i+1);
    }
    file.close();
    delete pDlg;
    displayData();
    QString str;
    str = QString("%1 probes are loaded in memory.\r\n"
                  "%2 probes are displayed in probe table.").arg(probeList.size()).arg( getTripleShowListSize() );

    QMessageBox QMBox;
    QMBox.setWindowTitle("Probe Info");
    QMBox.setText(str);
    QMBox.exec();
    //QMessageBox::about(0, "Probe Info",str);
}

void TripleReportFrame::saveTripleList(QString filename)
{
    QFile file(filename);
    if (!file.open(QFile::WriteOnly)){
        QMessageBox QMBox;
        QMBox.setWindowTitle("File Error");
        QMBox.setText("Error saving probe list file! \n Please report the problem to the developer");
        QMBox.exec();
        //QMessageBox::about(0, "File Error", "Error saving probe list file! \n Please report the problem to the developer");
        return;
    }

    QByteArray origData;
    QTextStream in(&origData);

    in << "Triple list file 1.0. Please do not modify.\n\n"
        << "Probe length range:\n" << probeLenMin << " " << probeLenMax << endl
        << "% target contain a candidate probe at least:\n" << probeCandidFrequency << "%\n"
        << "Gaps to primer:\n" << probeMinGap << endl

        << "Base avoidance constraints:\n" << probeAvoidACT << " " << probeAvoidG << " " << probeAvoidGat5end << endl
        << "Num Avoidance:\n" << probeNumACT << " " << probeNumG <<  endl


        << "Melting temperature between:\n" << probeTmMin << " " << probeTmMax << endl
        << "Melting temperature higher than primer between:\n" << probeTmGreaterMin << " " << probeTmGreaterMax << endl
        << "Salt concentration:\n" << probeNaConsent << endl
        << "Probe GC% between:\n" << probeGCMin << " " << probeGCMax << endl
        << "Max probe inter-complementarity:\n" << maxPbPrComp << endl
//        << "Max primer 3' inter-complementarity:\n" << max3PbPrComp << endl
//        << "Number of bases to count at 3' end(inter):\n" << tripleInterBase3end << endl
        << "Max probe intra-complementarity:\n" << probeMaxSelfComp << endl
//        << "Max probe 3' intra-complementarity:\n" << probeMax3SelfComp << endl
//        << "Number of bases to count at 3' end(intra):\n" << probeIntraBase3end << endl
        << "Max number of insertion and deletion allowed:\n" << maxIndel << endl
        << "Insertion and deletion cost:\n" << indelCost << endl
        << "Probe cost function type:\n" << probeCostType << endl;

    in << "Mismatch cost:\n";
    for( int i = 0; i < 15; i++ ){
        for( int j = 0; j < 15; j++ )
            in << simCost[i][j] << " ";
    }

    /*in << "\nPrimerPair Positional weight cost: target sequences:\n";
    for( int i = 0; i < MAX_PRIMER_LEN; i++ )
        in << primerPosWtTar[i] << " ";
    in << "\nPrimerPair non-target sequences:\n";
    for( int i = 0; i < MAX_PRIMER_LEN; i++ )
        in << primerPosWtNonTar[i] << " ";*/
    in << "\nTriple Positional weight cost: target sequences:\n";
    for( int i = 0; i < MAX_PROBE_LEN; i++ )
        in << probePosWtTar[i] << " ";
    in << "\nTriple non-target sequences:\n";
    for( int i = 0; i < MAX_PROBE_LEN; i++ )
        in << probePosWtNonTar[i] << " ";
    in << "\n---------------End of settings.--------------\n\n";


    int numTarSeqs = tarSeqNames.size();
    int numNontarSeqs = nontarSeqNames.size();
    //int numProbeSeqs = probeList.size();
    in << numTarSeqs << endl;
    QString tmpSeq;
    for( int i = 0; i < numTarSeqs; i++ ){
        tools::seqInt2Str(tarSeqs[i],tmpSeq);
        in << tarSeqNames[i] << endl << tmpSeq << endl;
    }
    in << numNontarSeqs << endl;
    for( int i = 0; i < numNontarSeqs; i++ ){
        tools::seqInt2Str(nontarSeqs[i],tmpSeq);
        in << nontarSeqNames[i] << endl << tmpSeq << endl;
    }
    /*for( int i = 0; i < numProbeSeqs; i++ ){
        Probe* pb = probeList[i];
        in << pb->probeSeq << endl
           << pb->pctgInTargetSeqs << " " << pb->pctgInNonTarSeqs <<" "
           << pb->bindPosTarSeqsAvg[0] << " " << pb->bindPosTarSeqsAvg[1] << " "
           << pb->isEstimateTar << " " << pb->isEstimateNonTar << " "
           << pb->tmProbe << " " << pb->GCNum << " " << pb->selfComp << endl;
        short** posTar = pb->posBindingTar;
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < numTarSeqs; j++)
                in << posTar[i][j];
            in << endl;
        }
    }*/

    // store corresponding primer pair.
    in << pr->left << endl << pr->right << endl << pr->rightRevsComp << endl
       //<< pr->isEstimateTar << " " << pr->isEstimateNonTar << " "
       << pr->pctgInTargetSeqs << " " << pr->pctgInTarSeqsLeft << " " << pr->pctgInTarSeqsRight << " "
       << pr->pctgInNonTarSeqs << " " << pr->pctgInNonTarSeqsLeft << " " << pr->pctgInNonTarSeqsRight << " "
       << pr->bindPosTarSeqsAvgLeft << " " << pr->bindPosTarSeqsAvgRight << " "
       << pr->pcrSizeAvgTarSeqs << " " << pr->pcrSizeMinTarSeqs << " " << pr->pcrSizeMaxTarSeqs << " "
       << pr->pcrSizeAvgNonTarSeqs << " " << pr->pcrSizeMinNonTarSeqs << " " << pr->pcrSizeMaxNonTarSeqs << " "
       << pr->tmLeft << " " << pr->tmRight << " "
       << pr->GCLeft << " " << pr->GCRight << " "
       << pr->prPrComp << " " << pr->prPr3Comp << " "
       << pr->selfCompLeft << " " << pr->self3CompLeft << " " << pr->selfCompRight << " " << pr->self3CompRight << " "
       << pr->numAmbiguousTargetsLeft << " " << pr->numAmbiguousTargetsRight << " "
       << pr->numAmbiguousBasesLeft << " " << pr->numAmbiguousBasesRight << " " << endl;

    for(int i = 0; i < 2; i++){
        for(int j = 0; j < numTarSeqs; j++)
            in << pr->posBindingTar[i][j] << " ";
        in << endl;
    }
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < numNontarSeqs; j++)
            in << pr->posBindingNontar[i][j] << " ";
        in << endl;
    }


    MyProgressDlg *pDlg;
    int numTriples;
    Triple* tp;

    // user choose to save all triples
    if( tripleListSaveAllDispMark == 0 ){
        numTriples = pr->tripleList.size();
        in << numTriples << endl;

        int range = numTriples;
        pDlg = new MyProgressDlg("Saving triple list",0, range);
        pDlg->setWindowModality(Qt::ApplicationModal);

        for( int i = 0; i < numTriples; i++ ){
            tp = pr->tripleList[i];
            /*PrimerPair *pr = tp->Pr;
            in << pr->left <<endl<< pr->right <<endl<< pr->rightRevsComp<<endl
               << tp->Pb->probeSeq << endl
               << pr->isEstimateTar << " "
               << pr->isEstimateNonTar;*/
            in << tp->pctgInTargetSeqs << " " << tp->pctgInNonTarSeqs << " "
               << tp->pbPrComp << endl;
            Probe *pb = tp->Pb;
            in << pb->probeSeq << endl
               //<< pb->isEstimateTar << " "
               //<< pb->isEstimateNonTar << " "
               << pb->pctgInTargetSeqs << " " << pb->pctgInNonTarSeqs <<" "
               << pb->bindPosTarSeqsAvg[0] << " " << pb->bindPosTarSeqsAvg[1] << " "
               << pb->tmProbe << " " << pb->GCNum << " "
               << pb->selfComp << " "
               << pb->numAmbiguousTargets << " " << pb->numAmbiguousBases << endl;
            short** pos = pb->posBindingTar;
            for(int i = 0; i < 2; i++){
                for(int j = 0; j < numTarSeqs; j++)
                    in << pos[i][j] << " ";
                in << endl;
            }
            pos = pb->posBindingNontar;
            for(int i = 0; i < 2; i++){
                for(int j = 0; j < numNontarSeqs; j++)
                    in << pos[i][j] << " ";
                in << endl;
            }

            in << endl;
            in.flush();

            pDlg->setValue(i+1);
        }
    }
    else if( tripleListSaveAllDispMark == 1 ){	// user wants to save all displayed probe pairs
        numTriples = tripleShowList.size();
        in << numTriples << endl;

        int range = numTriples;
        pDlg = new MyProgressDlg("Saving triple list",0, range);
        pDlg->setWindowModality(Qt::ApplicationModal);

        for( int i = 0; i < numTriples; i++ ){
            tp = tripleShowList[i];
            /*PrimerPair *pr = tp->Pr;
            in << pr->left <<endl<< pr->right <<endl<< pr->rightRevsComp<<endl
               << tp->Pb->probeSeq <<  endl
               << pr->isEstimateTar << " "
               << pr->isEstimateNonTar;*/
            in << tp->pctgInTargetSeqs << " " << tp->pctgInNonTarSeqs << " "
               << tp->pbPrComp << endl;
            Probe *pb = tp->Pb;
            in << pb->probeSeq << endl
               //<< pb->isEstimateTar << " "
               //<< pb->isEstimateNonTar << " "
               << pb->pctgInTargetSeqs << " " << pb->pctgInNonTarSeqs <<" "
               << pb->bindPosTarSeqsAvg[0] << " " << pb->bindPosTarSeqsAvg[1] << " "
               << pb->tmProbe << " " << pb->GCNum << " "
               << pb->selfComp << " "
               << pb->numAmbiguousTargets << " " << pb->numAmbiguousBases << endl;
            short** pos = pb->posBindingTar;
            for(int i = 0; i < 2; i++){
                for(int j = 0; j < numTarSeqs; j++)
                    in << pos[i][j] << " ";
                in << endl;
            }
            pos = pb->posBindingNontar;
            for(int i = 0; i < 2; i++){
                for(int j = 0; j < numNontarSeqs; j++)
                    in << pos[i][j] << " ";
                in << endl;
            }

            in << endl;
            in.flush();

            pDlg->setValue(i+1);
        }
    }
    else if(tripleListSaveAllDispMark == 2 ){	// user wants to save marked probes only
        getMarkedList();
        // write marked probes to outfile
        numTriples = markedList.size();
        in << numTriples << endl;

        int range = numTriples;
        pDlg = new MyProgressDlg("Saving triple list",0, range);
        pDlg->setWindowModality(Qt::ApplicationModal);

        for( int i = 0; i < numTriples; i++ ){
            tp = tripleShowList[markedList[i]];
            /*PrimerPair *pr = tp->Pr;
            in << pr->left <<endl<< pr->right <<endl<< pr->rightRevsComp<<endl
               << tp->Pb->probeSeq << endl
               << pr->isEstimateTar << " "
               << pr->isEstimateNonTar;*/
            in << tp->pctgInTargetSeqs << " " << tp->pctgInNonTarSeqs << " "
               << tp->pbPrComp << endl;
            Probe *pb = tp->Pb;
            in << pb->probeSeq << endl
               //<< pb->isEstimateTar << " "
               //<< pb->isEstimateNonTar << " "
               << pb->pctgInTargetSeqs << " " << pb->pctgInNonTarSeqs <<" "
               << pb->bindPosTarSeqsAvg[0] << " " << pb->bindPosTarSeqsAvg[1] << " "
               << pb->tmProbe << " " << pb->GCNum << " "
               << pb->selfComp << " "
               << pb->numAmbiguousTargets << " " << pb->numAmbiguousBases << endl;
            short** pos = pb->posBindingTar;
            for(int i = 0; i < 2; i++){
                for(int j = 0; j < numTarSeqs; j++)
                    in << pos[i][j] << " ";
                in << endl;
            }
            pos = pb->posBindingNontar;
            for(int i = 0; i < 2; i++){
                for(int j = 0; j < numNontarSeqs; j++)
                    in << pos[i][j] << " ";
                in << endl;
            }

            for( int col = B_TARGET_PCTG; col < B_NUM_COLUMNS; col++ )
                in << " " << DataTools::getTripleInfo( tp, col );
            in << endl;
            in.flush();

            pDlg->setValue(i+1);
        }
    }

    // zip the probe list temp file, then remove the temp file
    QByteArray compressedData = qCompress(origData);
    file.write(compressedData);
    file.close();

    delete pDlg;


    QMessageBox QMBox;
    QMBox.setWindowTitle("File saved");
    QMBox.setText(QString("%1 probes were saved to probe list file: %2").arg(numTriples).arg(filename));
    QMBox.exec();
    //QMessageBox::about(0, "File saved", QString("%1 probes were saved to probe list file: %2").arg(numTriples).arg(filename));
}

void TripleReportFrame::saveTripleList2TXT(QString filename)
{
    QFile file(filename);
    if (!file.open(QFile::WriteOnly)){
        QMessageBox QMBox;
        QMBox.setWindowTitle("File Error");
        QMBox.setText("Error saving probe to TXT file! \n Please report the problem to the developer");
        QMBox.exec();
        //QMessageBox::about(0, "File Error", "Error saving probe to TXT file! \n Please report the problem to the developer");
        return;
    }

    QTextStream out(&file);

    // output corresponding primer

    out << "Forward Primer\tReverse Primer";
    for( int i = A_TARGET_PCTG; i < A_NUM_COLUMNS; i++ ){
        QString colName = primerColumns[i];
        colName.replace('\n',' ');
        out << "\t" << colName;
    }
    out << endl;

    out << pr->left << "\t" << pr->rightRevsComp;
    for( int col = A_TARGET_PCTG; col < A_NUM_COLUMNS; col++ ){
        if((static_cast<QString>(primerColumns[col]).indexOf('%')) >= 0 )
            out << "\t" << DataTools::getPrimerInfo( pr, col ) << "%";
        else
            out << "\t" << DataTools::getPrimerInfo( pr, col );
    }
    out << endl << endl << endl;

    // output column headers
    out << "Triple No.\tProbe";
    for( int i = B_TARGET_PCTG; i < B_NUM_COLUMNS; i++ ){
        if( tripleExcelSaveColumns[i] ){
            QString colName = tripleColumns[i];
            colName.replace('\n',' ');
            out << "\t" << colName;
        }
    }
    out << endl;

    int numTriples;
    Triple *tp;
    MyProgressDlg* pDlg;

    // user choose to save all primer pairs
    if( tripleExcelSaveAllDispMark == 0 ){
        numTriples = getTripleListSize();
        pDlg = new MyProgressDlg("Saving triple list",1,numTriples, Q_NULLPTR);
        pDlg->setWindowModality(Qt::ApplicationModal);

        for( int i = 0; i < numTriples; i++ ){
            tp = pr->tripleList[i];
            //out << i+1 << "\t" << tp->Pr->left <<"\t" << tp->Pr->rightRevsComp << "\t" << tp->Pb->probeSeq;
            out << i+1 << "\t" << tp->Pb->probeSeq;
            for( int col = B_TARGET_PCTG; col < B_NUM_COLUMNS; col++ ){
                if( tripleExcelSaveColumns[col] && (static_cast<QString>(tripleColumns[col])).indexOf('%') >= 0 )
                    out << "\t" << DataTools::getTripleInfo( tp, col ) << "%";
                else if( tripleExcelSaveColumns[col] )
                    out << "\t" << DataTools::getTripleInfo( tp, col );
            }
            out << endl;
            out.flush();

            pDlg->setValue(i+1);
        }
    }
    else if( tripleExcelSaveAllDispMark == 1 ){	// user wants to save all displayed primer pairs
        numTriples = getTripleShowListSize();
        pDlg = new MyProgressDlg("Saving probe list",1,numTriples, Q_NULLPTR);
        pDlg->setWindowModality(Qt::ApplicationModal);

        for( int i = 0; i < numTriples; i++ ){
            tp = tripleShowList[i];
            //out << i+1 << "\t" << tp->Pr->left <<"\t" << tp->Pr->rightRevsComp << "\t" << tp->Pb->probeSeq;
            out << i+1 << "\t" << tp->Pb->probeSeq;
            for( int col = B_TARGET_PCTG; col < B_NUM_COLUMNS; col++ ){
                if( tripleExcelSaveColumns[col] && (static_cast<QString>(tripleColumns[col])).indexOf('%') >= 0 )
                    out << "\t" << DataTools::getTripleInfo( tp, col ) << "%";
                else if( tripleExcelSaveColumns[col] )
                    out << "\t" << DataTools::getTripleInfo( tp, col );
            }
            out << endl;
            out.flush();

            pDlg->setValue(i+1);
        }
    }
    else if( tripleExcelSaveAllDispMark == 2 ){	// user wants to save marked primer pairs only
        // write marked primer pairs to outfile
        getMarkedList();
        numTriples = markedList.size();
        pDlg = new MyProgressDlg("Saving probe list",1,numTriples, Q_NULLPTR);
        pDlg->setWindowModality(Qt::ApplicationModal);

        for( int i = 0; i < numTriples; i++ ){
            tp = tripleShowList[markedList[i]];
            //out << i+1 << "\t" << tp->Pr->left <<"\t" << tp->Pr->rightRevsComp << "\t" << tp->Pb->probeSeq;
            out << i+1 << "\t" << tp->Pb->probeSeq;
            for( int col = B_TARGET_PCTG; col < B_NUM_COLUMNS; col++ ){
                if( tripleExcelSaveColumns[col] && (static_cast<QString>(tripleColumns[col])).indexOf('%') >= 0 )
                    out << "\t" << DataTools::getTripleInfo( tp, col ) << "%";
                else if( tripleExcelSaveColumns[col] )
                    out << "\t" << DataTools::getTripleInfo( tp, col );
            }
            out << endl;
            out.flush();

            pDlg->setValue(i+1);
        }
    }

    file.close();

    delete pDlg;

    QMessageBox QMBox;
    QMBox.setWindowTitle("File saved");
    QMBox.setText(QString("%1 triples were saved to file: %2").arg(numTriples).arg(filename));
    QMBox.exec();
    //QMessageBox::about(0, "File saved", QString("%1 triples were saved to file: %2").arg(numTriples).arg(filename));
}

void TripleReportFrame::saveTriples(QString filename)
{
    QFile file(filename);
    if (!file.open(QFile::WriteOnly)){
        QMessageBox QMBox;
        QMBox.setWindowTitle("File Error");
        QMBox.setText("Error saving Information to TXT file! \n Please report the problem to the developer");
        QMBox.exec();
        //QMessageBox::about(0, "File Error", "Error saving Information to TXT file! \n Please report the problem to the developer");
        return;
    }
    QTextStream out(&file);

    int numTriples;
    Triple *tp;
    MyProgressDlg* pDlg;

    // user choose to save all primer pairs
    if( tripleExcelSaveAllDispMark == 0 ){
        numTriples = pr->tripleList.size();

        pDlg = new MyProgressDlg("Saving triples",1,numTriples, Q_NULLPTR);
        pDlg->setWindowModality(Qt::ApplicationModal);

        for( int i = 0; i < numTriples; i++ ){
            tp = pr->tripleList[i];
            out << pr->left << "..[" << tp->Pb->probeSeq << "].." << pr->rightRevsComp << endl;
            out.flush();

            pDlg->setValue(i+1);
        }
    }
    else if( tripleExcelSaveAllDispMark == 1 ){	// user wants to save all displayed primer pairs
        numTriples = tripleShowList.size();
        pDlg = new MyProgressDlg("Saving triples",1,numTriples, Q_NULLPTR);
        pDlg->setWindowModality(Qt::ApplicationModal);

        for( int i = 0; i < numTriples; i++ ){
            tp = tripleShowList[i];
            //out << pr->left << ".." << pr->rightRevsComp << " | " << tp->Pb->probeSeq << endl;
            out << pr->left << "..[" << tp->Pb->probeSeq << "].." << pr->rightRevsComp << endl;
            out.flush();

            pDlg->setValue(i+1);
        }
    }
    else if( tripleExcelSaveAllDispMark == 2 ){	// user wants to save marked primer pairs only
        getMarkedList();
        // write marked primer pairs to outfile
        numTriples = markedList.size();
        pDlg = new MyProgressDlg("Saving triples",1,numTriples, Q_NULLPTR);
        pDlg->setWindowModality(Qt::ApplicationModal);

        for( int i = 0; i < numTriples; i++ ){
            tp = tripleShowList[markedList[i]];
            //out << pr->left << ".." << pr->rightRevsComp << " | " << tp->Pb->probeSeq << endl;
            out << pr->left << "..[" << tp->Pb->probeSeq << "].." << pr->rightRevsComp << endl;
            out.flush();

            pDlg->setValue(i+1);
        }
    }
    file.close();

    delete pDlg;

    QMessageBox QMBox;
    QMBox.setWindowTitle("File saved");
    QMBox.setText(QString("%1 triples were saved to file: %2").arg(numTriples).arg(filename));
    QMBox.exec();
    //QMessageBox::about(0, "File saved", QString("%1 triples were saved to file: %2").arg(numTriples).arg(filename));
}

void TripleReportFrame::displayAllColumn()
{
    tripleColumnDisp.clear();
    tripleColumnHide.clear();

    for( int i = 0; i < tripleColumns.size(); i++ ){
        tripleColumnDisp.push_back(i);
        ui->tableView->setColumnHidden(i, false);
    }
    ui->textEdit->clear();
}

void TripleReportFrame::hideDispColumns()
{

    TripleColumnDlg *cDlg = new TripleColumnDlg(this);
    if(!cDlg->exec())
        return;

    for(int i = 0; i < tripleColumns.size(); i++){
        ui->tableView->setColumnWidth(i, tripleColWidth[i]);
        ui->tableView->setColumnHidden(i, false);
    }

    for(int i = 0; i < tripleColumnHide.size(); i++){
        ui->tableView->setColumnHidden(tripleColumnHide[i], true);
    }
}

void TripleReportFrame::hideDispTriples()
{
    if( getTripleListSize() == 0 )
        return;

    dispTripleCondAllowCancel = 1;
    TripleDispListDlg* dispDlg = new TripleDispListDlg(this);

    // process only if the user hits OK
    if( !dispDlg->exec() )
        return;

    copyDispTripleList();

    int range = tripleShowList.size();
    MyProgressDlg* pDlg = new MyProgressDlg("Adding triples to triple table", 1,range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);


    // add all triples in result list one by one
    for(int i = 0; i < tripleShowList.size(); i++ ){
        insertData(i);
        pDlg->setValue(i+1);
    }
    delete pDlg;
    QMessageBox::about(this, "Display Info", QString("%1 triples out of %1 total probes are displayed.").arg(tripleShowList.size()).arg(pr->tripleList.size()));
}

void TripleReportFrame::sortTripleList()
{
    if( tripleShowList.size() == 0 )
        return;

    TripleSortDlg* sDlg = new TripleSortDlg(this);
    // do nothing if user hit "cancel"
    if( !sDlg->exec() )
        return;
    // execute only if user hit "ok"

    this->emptyModel();
    ui->textEdit->clear();

    DataTools::sortTripleList(tripleShowList);
    insertShownTripleList();
}

void TripleReportFrame::addTripleManually()
{
    tableModel->insertRow(0);
    QStandardItem* __tmpItem = new QStandardItem();
    __tmpItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
    tableModel->setItem(0,0,__tmpItem);
    __tmpItem->setCheckState(Qt::Unchecked);
    tableModel->setData(tableModel->index(0,0), "");

    //tableModel->setData(tableModel->index(0,1),"..");
    //connect(tableModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(checkInputPrimer(QStandardItem*)));
    ui->tableView->setItemDelegate(new ProbeItemDelegate);
    ui->tableView->edit(tableModel->index(0,1));



}

void TripleReportFrame::deleteTriplesConditionally()
{
    if( tripleShowList.size() == 0 )
            return;

    TripleDeleteCondDlg delDlg(this);
    if(!delDlg.exec())
        return;

    MyConfirmDlg cDlg("Are you sure you want to delete sequence(s)?\n\n\n\n",this);
    if(!cDlg.exec())
        return;


    int range = pr->tripleList.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Clean triple list memory",0, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);

    emptyModel();
    ui->textEdit->clear();

    Triple *tp;
    bool passChecking;
    int numDel = 0;
    // check for each probe

    int numTotal = pr->tripleList.size();
    for(int i = 0, j = 0; j < numTotal; i++, j++ ){
        tp = pr->tripleList[i];

        passChecking = checkTripleWithCondition(tp, deleteTripleCondTitle, deleteTripleCondGtLt, deleteTripleCondValue );

        if( passChecking){	// pass all the checks, remove it from probeList memory
            tp->pctgInTargetSeqs = -1; // mark it for deletion
            tp->pctgInNonTarSeqs = -1;
            //pr->tripleList.erase( pr->tripleList.begin()+i );
            //i--;
            //numDel++;
        }
        pDlg->setValue(j+1);
    }
    delete pDlg;

    range = tripleShowList.size();
    pDlg = new MyProgressDlg("Adding triples to table",0, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);

    // check for each probe in displaying probe list
    numTotal = tripleShowList.size();
    for(int i = 0, j = 0; j < numTotal; i++, j++ ){
        tp = tripleShowList[i];
        // previously marked for deletion
        if( tp->pctgInTargetSeqs == -1 &&  tp->pctgInNonTarSeqs == -1 ){
            //delete tp;
            tripleShowList.erase(tripleShowList.begin()+i );
            i--;
        }
        else
            insertData(i);

        pDlg->setValue(j+1);
    }
    delete pDlg;

    numTotal = pr->tripleList.size();
    for( int i = 0, j = 0; j < numTotal; i++, j++ ){
        tp = pr->tripleList[i];

        if( tp->pctgInTargetSeqs == -1 && tp->pctgInNonTarSeqs == -1){	// pass all the checks, remove it from probeList memory
            pr->tripleList.erase( pr->tripleList.begin()+i );
            delete tp;
            i--;
            numDel++;
        }
    }

    QString str = QString("%1 triples were deleted.").arg(numDel);
    QMessageBox::about(this,"",str);
}

void TripleReportFrame::deleteMarkedTriples()
{
    if( tripleShowList.size() == 0 )
        return;

    int numDel = 0;

    getMarkedList();
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
    MyProgressDlg *pDlg = new MyProgressDlg("Clean probe list memory",0, 1, this);
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
            if( DataTools::deleteTripleFromTripleMem( tripleMarked, tripleShowList, 0 ) < 0 )
                return;

            // delete the probe from theApp memory and destroy the memory
            if( DataTools::deleteTripleFromTripleMem( tripleMarked, pr->tripleList, 1 ) < 0 )
                return;
        }
    }
    delete pDlg;

    QString str = QString("%1 marked triples were deleted.").arg(numDel);
    QMessageBox::about(this,"",str);
}

void TripleReportFrame::deleteSeletedTriples()
{
    if( tripleShowList.size() == 0 )
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
    MyProgressDlg *pDlg = new MyProgressDlg("Clean probe list memory",0, 1, this);
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
            if( DataTools::deleteTripleFromTripleMem( tripleSelected, tripleShowList, 0 ) < 0 )
                    return;

            // delete the triple from theApp memory and destroy the memory
            if( DataTools::deleteTripleFromTripleMem( tripleSelected, pr->tripleList, 1 ) < 0 )
                    return;
        }
    }
    delete pDlg;

    QString str = QString("%1 selected triples were deleted.").arg(nSelCount);
    QMessageBox::about(this,"",str);
}

void TripleReportFrame::markSelectedTriples()
{
    if( tripleShowList.size() == 0 )
        return;

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();

    if( selectedRows.size() <= 0 ){
        QMessageBox::about(this, "Empty delete list", "No probe is selected.");
        return;
    }

    ui->textEdit->clear();

    // track selected probes and set it as marked
    int nSelCount = selectedRows.size();

    if( nSelCount >= 5000 ){
        MyProgressDlg *pDlg = new MyProgressDlg("Marking selected probes",1, nSelCount, this);
        pDlg->setWindowModality(Qt::ApplicationModal);


        for( int i = 0; i < nSelCount; i++ ){
            int rowIdx = selectedRows.at(i).row();
            QStandardItem* __tmpItem =  this->tableModel->item(rowIdx,0);
            __tmpItem->setCheckState(Qt::Checked);
            tripleShowList[rowIdx]->marked = true;
            pDlg->setValue(i + 1);
        }
        delete pDlg;
    }
    else{
        for( int i = 0; i < nSelCount; i++ ){
            int rowIdx = selectedRows.at(i).row();
            QStandardItem* __tmpItem =  this->tableModel->item(rowIdx,0);
            __tmpItem->setCheckState(Qt::Checked);
            tripleShowList[rowIdx]->marked = true;
        }
    }
}

void TripleReportFrame::unmarkSelectedTriples()
{
    if( tripleShowList.size() == 0 )
        return;

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();

    if( selectedRows.size() <= 0 ){
        QMessageBox::about(this, "Empty delete list", "No probe is selected.");
        return;
    }

    ui->textEdit->clear();

    // track selected probes and set it as unmarked
    int nSelCount = selectedRows.size();

    if( nSelCount >= 5000 ){
        MyProgressDlg *pDlg = new MyProgressDlg("Unmarking selected probes",1, nSelCount, this);
        pDlg->setWindowModality(Qt::ApplicationModal);


        for( int i = 0; i < nSelCount; i++ ){
            int rowIdx = selectedRows.at(i).row();
            QStandardItem* __tmpItem =  this->tableModel->item(rowIdx,0);
            __tmpItem->setCheckState(Qt::Unchecked);
            tripleShowList[rowIdx]->marked = false;
            pDlg->setValue(i + 1);
        }
        delete pDlg;
    }
    else{
        for( int i = 0; i < nSelCount; i++ ){
            int rowIdx = selectedRows.at(i).row();
            QStandardItem* __tmpItem =  this->tableModel->item(rowIdx,0);
            tripleShowList[rowIdx]->marked = false;
            __tmpItem->setCheckState(Qt::Unchecked);
        }
    }
}

void TripleReportFrame::dispCompleteInfo()
{
    if(!getCurrSelectedItem())
        return ;

    int range = tarSeqs.size() + nontarSeqs.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Collecting primer-probe set annealing information",1, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);

    ui->textEdit->clear();

    Triple* tp =tripleShowList[dispTripleSel];
    //PrimerPair *pr = tp->Pr;
    Probe *pb = tp->Pb;
    QString currTriple = pr->left + "..[" + tp->Pb->probeSeq + "].." + pr->rightRevsComp;

    /*int pLen = p->probeSeq.length();

    short *probe = new short[pLen+1];
    for( int i = 0; i < pLen; i++ ){
        probe[i] = tools::base2int(p->probeSeq[i]);
    }
    probe[pLen] = -1;*/


    /*ProbeSearch* ps;
    try{
        ps = new ProbeSearch(probe);
        delete[] probe;
    }
    catch( std::bad_alloc& e ){
        QMessageBox::about(this,"Fatal Error","Fatal error: out of memory for probe searching.");
        dispProbeSel = -1;
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
    int bdPos[6];
    QString str, posS, posE;

    //tarSeqNames;
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

    ui->textEdit->insertPlainText("Primer-probe set annealing position information -- " + currTriple + "\r\n\r\n");
    ui->textEdit->insertPlainText("Target sequences:\r\n");
    str = "";
    while( str.length() < 70 )
            str.insert(70, ' ');
    ui->textEdit->insertPlainText(str+"\tFw primer\tProbe\tRv primer\tPCR size\r\n");


    // compute frequencies for this triple in target sequences
    num = 0;
    for( int i = 0; i < numTarSeqs; i++ ){

        // search probes in target sequence
        //ps->appxSearchTarSeqs(tarSeqs[i], bdPos[0], bdPos[1]);

        // triple appears
        bdPos[1] = pr->posBindingTar[0][i];
        bdPos[2] = pb->posBindingTar[0][i];
        bdPos[3] = pb->posBindingTar[1][i];
        bdPos[4] = pr->posBindingTar[1][i];

        bdPos[0] = bdPos[1] > -1 ? bdPos[1]- pr->left.length()+1:-1;
        bdPos[5] = bdPos[4] > -1 ? bdPos[4]+ pr->right.length()-1:-1;
        if( bdPos[0] >= 0 && bdPos[2] >= 0 && bdPos[4] >= 0 && bdPos[1]<bdPos[2] && bdPos[3] <bdPos[4]){
            num++;
            productSize = bdPos[5] - bdPos[0] + 1;
            if( minProductSize > productSize )	minProductSize = productSize;
            if( maxProductSize < productSize )	maxProductSize = productSize;
            sumProductSize += productSize;
        }



        ui->textEdit->insertPlainText(tmpTarSeqNames[i]+"\t");
        if( bdPos[0] < 0 )
             str = "   -  \t";
        else{
            posS = QString("%1").arg(bdPos[0]+1);
            posE = QString("%1").arg(bdPos[1]+1);
            str = posS + "-" + posE + "\t";
            //if( str.length() < 7 )
            //    str += "\t";
        }
        ui->textEdit->insertPlainText(str);
        if( bdPos[2] < 0 )
            str = "   -  \t";
        else{
            posS = QString("%1").arg(bdPos[2]+1);
            posE = QString("%1").arg(bdPos[3]+1);
            str = posS + "-" + posE + "\t";
            //if( str.length() < 7 )
            //    str += "\t";
        }
        ui->textEdit->insertPlainText(str);
        if( bdPos[4] < 0 )
            str = "   -  \t";
        else{
            posS = QString("%1").arg(bdPos[4]+1);
            posE = QString("%1").arg(bdPos[5]+1);
            str = posS + "-" + posE + "\t";
            //if( str.length() < 7 )
            //    str += "\t";
        }
        ui->textEdit->insertPlainText(str);
        if( bdPos[0] < 0 || bdPos[2] < 0 || bdPos[1] >= bdPos[2] )
             str = "-";
        else
            str = QString("%1").arg( bdPos[5] - bdPos[0] + 1);
        ui->textEdit->insertPlainText(str+"\n");



        /*ui->textEdit->insertPlainText(tmpTarSeqNames[i]+"\t");
        if( bdPos[0] < 0 )
             str = "   -  \t";
        else{
            posS = QString("%1").arg(bdPos[0]+1);
            posE = QString("%1").arg(bdPos[1]+1);
            str = posS + "-" + posE + "\t";
            //if( str.length() < 7 )
            //    str += "\t";
        }
        ui->textEdit->insertPlainText(str);
        ui->textEdit->insertPlainText("\n");*/

        pDlg->setValue(i);
    }
    if( numTarSeqs != 0 )
        tp->pctgInTargetSeqs = static_cast<float>(num) / numTarSeqs * 100;
    else
        tp->pctgInTargetSeqs = 0;
    if( num == 0 ){
        pr->pcrSizeAvgTarSeqs = 0;
        pr->pcrSizeMinTarSeqs = 0;
        pr->pcrSizeMaxTarSeqs = 0;
    }
    else{
        pr->pcrSizeAvgTarSeqs = static_cast<float>(sumProductSize) / num;
        pr->pcrSizeMinTarSeqs = minProductSize;
        pr->pcrSizeMaxTarSeqs = maxProductSize;
    }



    ui->textEdit->insertPlainText("\r\n\r\nNon-target sequences:\r\n");
    str = "";
    while( str.length() < 70 )
        str.insert(70, ' ');
    ui->textEdit->insertPlainText(str+"\tFw primer\tProbe\tRv primer\tPCR size\r\n");

    // compute frequencies for this primer pair in non-target sequences
    num = 0;
    for( int i = 0; i < numNontarSeqs; i++ ){

        // search probes in target sequence
        //ps->appxSearchNonTarSeqs(nontarSeqs[i], bdPos[0], bdPos[1]);

        // Probe appear and positions are correct
        bdPos[1] = pr->posBindingNontar[0][i];
        bdPos[2] = pb->posBindingNontar[0][i];
        bdPos[3] = pb->posBindingNontar[1][i];
        bdPos[4] = pr->posBindingNontar[1][i];

        bdPos[0] = bdPos[1] > -1 ? bdPos[1]- pr->left.length()+1:-1;
        bdPos[5] = bdPos[4] > -1 ? bdPos[4]+ pr->right.length()-1:-1;

        if( bdPos[0] >= 0 && bdPos[2] >= 0 && bdPos[4] >= 0 && bdPos[1]<bdPos[2] && bdPos[3] <bdPos[4] ){
            num++;
            productSize = bdPos[5] - bdPos[0] + 1;
            if( minProductSize > productSize )	minProductSize = productSize;
            if( maxProductSize < productSize )	maxProductSize = productSize;
            sumProductSize += productSize;
        }
        ui->textEdit->insertPlainText(tmpNontarSeqNames[i]+"\t");

        if( bdPos[0] < 0 )
            str = "   -  \t";
        else{
            posS = QString("%1").arg(bdPos[0]+1);
            posE = QString("%1").arg(bdPos[1]+1);
            str = posS + "-" + posE + "\t";
            //if( str.length() < 7 )
            //        str += "\t";
        }
        ui->textEdit->insertPlainText(str);
        if( bdPos[2] < 0 )
            str = "   -  \t";
        else{
            posS = QString("%1").arg(bdPos[2]+1);
            posE = QString("%1").arg(bdPos[3]+1);
            str = posS + "-" + posE + "\t";
            //if( str.length() < 7 )
            //    str += "\t";
        }
        ui->textEdit->insertPlainText(str);
        if( bdPos[4] < 0 )
            str = "   -  \t";
        else{
            posS = QString("%1").arg(bdPos[4]+1);
            posE = QString("%1").arg(bdPos[5]+1);
            str = posS + "-" + posE + "\t";
            //if( str.length() < 7 )
            //    str += "\t";
        }
        ui->textEdit->insertPlainText(str);
        if( bdPos[0] < 0 || bdPos[4] < 0 || bdPos[1] >= bdPos[4] )
            str = "-";
        else
            str = QString("%1").arg(bdPos[5] - bdPos[0] + 1);
        ui->textEdit->insertPlainText(str+"\n");
        pDlg->setValue(numTarSeqs+i);
    }

    if( numNontarSeqs != 0 )
        tp->pctgInNonTarSeqs = static_cast<float>(num) / numNontarSeqs * 100;
    else
        tp->pctgInNonTarSeqs = 0;
    if( num == 0 ){
        pr->pcrSizeAvgNonTarSeqs = 0;
        pr->pcrSizeMinNonTarSeqs = 0;
        pr->pcrSizeMaxNonTarSeqs = 0;
    }
    else{
        pr->pcrSizeAvgNonTarSeqs = static_cast<float>(sumProductSize) / num;
        pr->pcrSizeMinNonTarSeqs = minProductSize;
        pr->pcrSizeMaxNonTarSeqs = maxProductSize;
    }



    //pr->isEstimateTar = 0;
    //pr->isEstimateNonTar = 0;

    ui->textEdit->insertPlainText("\r\n\r\nFinish Listing of primer and probe sequence annealing information.");

    //delete ps;

    // update frequency, average, minimum and maximum product size to exact value
    QString val;
    val.sprintf("%.2f", static_cast<double>(tp->pctgInTargetSeqs));
    tableModel->setData(tableModel->index(dispTripleSel, B_TARGET_PCTG), val);
    tableModel->item(dispTripleSel, B_TARGET_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);


    val.sprintf("%.2f", static_cast<double>(tp->pctgInNonTarSeqs));
    tableModel->setData(tableModel->index(dispTripleSel, B_NONTAR_PCTG), val);
    tableModel->item(dispTripleSel, B_NONTAR_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);


    dispTripleSel = -1;
    delete pDlg;
}

void TripleReportFrame::reportBindingContent()
{
    if(!getCurrSelectedItem())
        return ;

    AnnealInfoDlg annInfoDlg(&bindingContentDivider3end, &bindingContentMatchPctg5end, &bindingContentMatchPctg3end);
    if( !annInfoDlg.exec() )
        return;

    ui->textEdit->clear();

    int range = tarSeqs.size() + nontarSeqs.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Collecting primer-probe set annealing information",1, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);



    // get triple sequence
    Triple* tp = tripleShowList[dispTripleSel];
    Probe* pb = tp->Pb;
    QString lprimerStr = pr->left,
            rprimerStr = pr->rightRevsComp,
            probeStr = pb->probeSeq;


    int pLen = lprimerStr.length();
    short *lprimer = new short[static_cast<unsigned int>(pLen+1)];
    for( int i = 0; i < pLen; i++ )
        lprimer[i] = static_cast<short>(tools::base2int(lprimerStr[i]));
    lprimer[pLen] = -1;

    pLen = pr->right.length();
    short *rprimer = new short[static_cast<unsigned long>(pLen+1)];
    for( int i = 0; i < pLen; i++ )
        rprimer[i] = static_cast<short>(tools::base2int(pr->right[i]));
    rprimer[pLen] = -1;

    pLen = pb->probeSeq.length();
    short *pbSeq = new short[static_cast<unsigned long>(pLen+1)];
    for( int i = 0; i < pLen; i++ )
        pbSeq[i] = static_cast<short>(tools::base2int(pb->probeSeq[i]));
    pbSeq[pLen] = -1;

    //PrimerSearch* lft;
    //PrimerSearch* rht;
    //try{
    //    lft = new PrimerSearch(lprimer, LEFT_PRIMER);
    //    rht = new PrimerSearch(rprimer, RIGHT_PRIMER);
    //}
    //catch(std::bad_alloc& e){
    //    QMessageBox::about(this,"Fatal error","Fatal error: out of memory for primer searching.");
    //    delete lprimer;	delete rprimer;
    //    delete pDlg;
    //    return;
    //}


    // compute and display final result
    ui->textEdit->insertPlainText("Percentage of each nucleotide in target and non-target sequences in relation to primer and probe sequences information for primer-probe sets -- " +
                                            pr->left +"..["+ pb->probeSeq+ "].." + pr->rightRevsComp + "\r\n");
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
    QVector<short*> seqPtrBindProbe;
    QVector<short*> seqPtrNonBindProbe;
    short *seqPtr;
    float percent;
    int num;
    int maxProductSize;
    int minProductSize;
    int sumProductSize;
    int productSize;
    int pos[6];



// -----------------------target sequences--------------------------------------------------
    num = 0;
    maxProductSize = 0;	minProductSize = INT_MAX;	sumProductSize = 0;

    for( int i = 0; i < numTarSeqs; i++ ){
        seqPtr = tarSeqs[i];

        // search left and right primers in target sequence
        //lft->appxSearchTarSeqs(seqPtr, LEFT_PRIMER, pos[0], pos[1]);
        //rht->appxSearchTarSeqs(seqPtr, RIGHT_PRIMER, pos[2], pos[3]);
        pos[1] = pr->posBindingTar[0][i];
        pos[2] = pb->posBindingTar[0][i];
        pos[3] = pb->posBindingTar[1][i];
        pos[4] = pr->posBindingTar[1][i];

        pos[0] = pos[1] > -1 ? pos[1]- pr->left.length()+1:-1;
        pos[5] = pos[4] > -1 ? pos[4]+ pr->right.length()-1:-1;
        // triple anneal with this sequence
        if( pos[0] >= 0 && pos[1] < pos[2] && pos[3] < pos[4] ){

            short *bindPtrLeft = seqPtr;
            bindPtrLeft += pos[0];
            seqPtrBindLeft.push_back(bindPtrLeft);

            short *bindPtrRight = seqPtr;
            bindPtrRight += pos[4];
            seqPtrBindRight.push_back(bindPtrRight);

            short *bindPtrProbe = seqPtr;
            bindPtrProbe += pos[2];
            seqPtrBindProbe.push_back(bindPtrProbe);

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
            pos[2] = DataTools::searchPrPbInNonbindingSeq(pbSeq,   seqPtr, LEFT_PRIMER);
            pos[4] = DataTools::searchPrPbInNonbindingSeq(rprimer, seqPtr, RIGHT_PRIMER);

            // if the annealing order is right: record annealing info for non_binding sequence
            if( pos[0] >= 0 && pos[2] > 0 && pos[4] > 0
                    && (pos[0] + lprimerStr.length() < pos[2]) && (pos[2] + probeStr.length() < pos[4]) ){
                short *bindPtrLeft = seqPtr;
                bindPtrLeft += pos[0];
                seqPtrNonBindLeft.push_back(bindPtrLeft);

                short *bindPtrRight = seqPtr;
                bindPtrRight += pos[4];
                seqPtrNonBindRight.push_back(bindPtrRight);

                short *bindPtrProbe = seqPtr;
                bindPtrProbe += pos[2];
                seqPtrNonBindProbe.push_back(bindPtrProbe);
            }
        }
        pDlg->setValue(i);
    }

    // update annealing information for those estimated info
    if( numTarSeqs != 0 )
        pr->pctgInTargetSeqs = (float)num / numTarSeqs * 100;
    else
        pr->pctgInTargetSeqs = 0;
    if( num == 0 ){
        pr->pcrSizeAvgTarSeqs = 0;
        pr->pcrSizeMinTarSeqs = 0;
        pr->pcrSizeMaxTarSeqs = 0;
    }
    else{
        pr->pcrSizeAvgTarSeqs = (float)sumProductSize / num;
        pr->pcrSizeMinTarSeqs = minProductSize;
        pr->pcrSizeMaxTarSeqs = maxProductSize;
    }
    //pr->isEstimateTar = 0;
    //pr->isEstimateNonTar = 0;

    // output the primer sequences : annealing sequences
    ui->textEdit->insertPlainText("Information for target sequences annealing with primer-probe set: (");
    str = QString("%1").arg(seqPtrBindLeft.size() );
    ui->textEdit->insertPlainText(str + " sequences)\r\n\r\n");

    // cout primer sequences
    for( int i = 0; i < lprimerStr.length(); i++ ){
        str = lprimerStr.mid(i, 1);
        ui->textEdit->insertPlainText("\t"+str);
    }
    ui->textEdit->insertPlainText("\t..[");
    for( int i = 0; i < probeStr.length(); i++ ){
        str = probeStr.mid(i, 1);
        ui->textEdit->insertPlainText("\t"+str);
    }
    ui->textEdit->insertPlainText("\t]..\t");
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
            for(int j = 0; j < seqPtrBindLeft.size(); j++ ){
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

        // probe annealing region
        ui->textEdit->insertPlainText("\t..[");
        for( int i = 0; i < probeStr.length(); i++ ){
            percent = 0;
            for(int j = 0; j < seqPtrBindProbe.size(); j++ ){
                if( seqPtrBindProbe[j][i] == base )
                    percent++;
            }
            if( seqPtrBindProbe.size() > 0 )
                percent =  percent / (float)seqPtrBindProbe.size() * 100;
            else
                percent = 0;
            str = QString("%1").arg(static_cast<double>(percent));
            ui->textEdit->insertPlainText("\t"+str+"%");
        }
        ui->textEdit->insertPlainText("\t]..\t");

        // right primer annealing region
        for( int i = rprimerStr.length() - 1; i >= 0; i-- ){
            percent = 0;
            for(int j = 0; j < seqPtrBindRight.size(); j++ ){
                if( seqPtrBindRight[j][i] == 3-base ) // 3 - base: make it reverse complement for reverse primer
                    percent++;
            }
            if( seqPtrBindRight.size() > 0 )
                percent =  percent / static_cast<float>(seqPtrBindRight.size()) * 100;
            else
                percent = 0;
            str = QString("%1").arg(static_cast<double>(percent));
            ui->textEdit->insertPlainText("\t"+str+"%");
        }

        ui->textEdit->insertPlainText("\r\n");
    }


    // non-annealing sequences
    ui->textEdit->insertPlainText("\r\n\r\nInformation for target sequences not annealing with primer-probe set: (");
    str = QString("%1").arg(seqPtrNonBindLeft.size() );
    ui->textEdit->insertPlainText(str + " sequences)\r\n\r\n");

    // cout primer sequences
    for( int i = 0; i < lprimerStr.length(); i++ ){
        str = lprimerStr.mid(i, 1);
        ui->textEdit->insertPlainText("\t"+str);
    }
    ui->textEdit->insertPlainText("\t..[");
    for( int i = 0; i < probeStr.length(); i++ ){
        str = probeStr.mid(i, 1);
        ui->textEdit->insertPlainText("\t"+str);
    }
    ui->textEdit->insertPlainText("\t]..\t");
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
            for(int j = 0; j < seqPtrNonBindLeft.size(); j++ ){
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

        // probe annealing region
        ui->textEdit->insertPlainText("\t..[");
        for( int i = 0; i < probeStr.length(); i++ ){
            percent = 0;
            for(int j = 0; j < seqPtrNonBindProbe.size(); j++ ){
                if( seqPtrNonBindProbe[j][i] == base )
                    percent++;
            }
            if( seqPtrNonBindProbe.size() > 0 )
                percent =  percent / (float)seqPtrNonBindProbe.size() * 100;
            else
                percent = 0;
            str = QString("%1").arg(percent);
            ui->textEdit->insertPlainText("\t"+str+"%");
        }
        ui->textEdit->insertPlainText("\t]..\t");

        // right primer annealing region
        for( int i = rprimerStr.length() - 1; i >= 0; i-- ){
            percent = 0;
            for(int j = 0; j < seqPtrNonBindRight.size(); j++ ){
                if( seqPtrNonBindRight[j][i] == 3-base ) // 3 - base: make it reverse complement for reverse primer
                    percent++;
            }
            if( seqPtrNonBindRight.size() > 0 )
                percent =  percent / static_cast<float>(seqPtrNonBindRight.size()) * 100;
            else
                percent = 0;
            str = QString("%1").arg(static_cast<double>(percent));
            ui->textEdit->insertPlainText("\t"+str+"%");
        }

        ui->textEdit->insertPlainText("\r\n");
    }



// -------------non target sequences--------------------------------------------------
    seqPtrBindLeft.clear();
    seqPtrNonBindLeft.clear();
    seqPtrBindRight.clear();
    seqPtrNonBindRight.clear();
    seqPtrBindProbe.clear();
    seqPtrNonBindProbe.clear();

    num = 0;
    maxProductSize = 0;	minProductSize = INT_MAX;	sumProductSize = 0;

    for( int i = 0; i < numNontarSeqs; i++ ){
        seqPtr = nontarSeqs[i];

        // search left and right primers in non-target sequence
        //lft->appxSearchNonTarSeqs(seqPtr, LEFT_PRIMER, pos[0], pos[1]);
        //rht->appxSearchNonTarSeqs(seqPtr, RIGHT_PRIMER, pos[2], pos[3]);
        pos[1] = pr->posBindingNontar[0][i];
        pos[2] = pb->posBindingNontar[0][i];
        pos[3] = pb->posBindingNontar[1][i];
        pos[4] = pr->posBindingNontar[1][i];

        pos[0] = pos[1] > -1 ? pos[1]- pr->left.length()+1:-1;
        pos[5] = pos[4] > -1 ? pos[4]+ pr->right.length()-1:-1;
        // triple anneal with this sequence
        if( pos[0] >= 0 && pos[1] < pos[2] && pos[3] < pos[4] ){

            short *bindPtrLeft = seqPtr;
            bindPtrLeft += pos[0];
            seqPtrBindLeft.push_back(bindPtrLeft);

            short *bindPtrRight = seqPtr;
            bindPtrRight += pos[4];
            seqPtrBindRight.push_back(bindPtrRight);

            short *bindPtrProbe = seqPtr;
            bindPtrProbe += pos[2];
            seqPtrBindProbe.push_back(bindPtrProbe);

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
            pos[2] = DataTools::searchPrPbInNonbindingSeq(pbSeq,   seqPtr, LEFT_PRIMER);
            pos[4] = DataTools::searchPrPbInNonbindingSeq(rprimer, seqPtr, RIGHT_PRIMER);

            // if the annealing order is right: record annealing info for non_binding sequence
            if( pos[0] >= 0 && pos[2] > 0 && pos[4] > 0
                    && (pos[0] + lprimerStr.length() < pos[2]) && (pos[2] + probeStr.length() < pos[4]) ){
                short *bindPtrLeft = seqPtr;
                bindPtrLeft += pos[0];
                seqPtrNonBindLeft.push_back(bindPtrLeft);

                short *bindPtrRight = seqPtr;
                bindPtrRight += pos[4];
                seqPtrNonBindRight.push_back(bindPtrRight);

                short *bindPtrProbe = seqPtr;
                bindPtrProbe += pos[2];
                seqPtrNonBindProbe.push_back(bindPtrProbe);
            }
        }
        pDlg->setValue(numTarSeqs+i);
    }

    // update annealing infomation for estimated info
    if( numNontarSeqs != 0 )
        pr->pctgInNonTarSeqs = static_cast<float>(num) / numNontarSeqs * 100;
    else
        pr->pctgInNonTarSeqs = 0;
    if( num == 0 ){
        pr->pcrSizeAvgNonTarSeqs = 0;
        pr->pcrSizeMinNonTarSeqs = 0;
        pr->pcrSizeMaxNonTarSeqs = 0;
    }
    else{
        pr->pcrSizeAvgNonTarSeqs = static_cast<float>(sumProductSize) / num;
        pr->pcrSizeMinNonTarSeqs = minProductSize;
        pr->pcrSizeMaxNonTarSeqs = maxProductSize;
    }
    //pr->isEstimateTar = 0;
    //pr->isEstimateNonTar = 0;

    // output the primer sequences : annealing sequences
    ui->textEdit->insertPlainText("\r\n\r\nInformation for non-target sequences annealing with primer-probe set: (");
    str = QString("%1").arg(seqPtrBindLeft.size() );
    ui->textEdit->insertPlainText(str + " sequences)\r\n\r\n");

    // cout primer sequences
    for( int i = 0; i < lprimerStr.length(); i++ ){
        str = lprimerStr.mid(i, 1);
        ui->textEdit->insertPlainText("\t"+str);
    }
    ui->textEdit->insertPlainText("\t..[");
    for( int i = 0; i < probeStr.length(); i++ ){
        str = probeStr.mid(i, 1);
        ui->textEdit->insertPlainText("\t"+str);
    }
    ui->textEdit->insertPlainText("\t]..\t");
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
            for(int j = 0; j < seqPtrBindLeft.size(); j++ ){
                if( seqPtrBindLeft[j][i] == base )
                    percent++;
            }
            if( seqPtrBindLeft.size() > 0 )
                percent =  percent / static_cast<float>(seqPtrBindLeft.size()) * 100;
            else
                percent = 0;
            str = QString("%1").arg(static_cast<double>(percent));
            ui->textEdit->insertPlainText("\t"+str+"%");
        }

        // probe annealing region
        ui->textEdit->insertPlainText("\t..[");
        for( int i = 0; i < probeStr.length(); i++ ){
            percent = 0;
            for(int j = 0; j < seqPtrBindProbe.size(); j++ ){
                if( seqPtrBindProbe[j][i] == base )
                    percent++;
            }
            if( seqPtrBindProbe.size() > 0 )
                percent =  percent / static_cast<float>(seqPtrBindProbe.size()) * 100;
            else
                percent = 0;
            str = QString("%1").arg(static_cast<double>(percent));
            ui->textEdit->insertPlainText("\t"+str+"%");
        }
        ui->textEdit->insertPlainText("\t]..\t");

        // right primer annealing region
        for( int i = rprimerStr.length() - 1; i >= 0; i-- ){
            percent = 0;
            for(int j = 0; j < seqPtrBindRight.size(); j++ ){
                if( seqPtrBindRight[j][i] == 3-base ) // 3 - base: make it reverse complement for reverse primer
                    percent++;
            }
            if( seqPtrBindRight.size() > 0 )
                percent =  percent / static_cast<float>(seqPtrBindRight.size()) * 100;
            else
                percent = 0;
            str = QString("%1").arg(static_cast<double>(percent));
            ui->textEdit->insertPlainText("\t"+str+"%");
        }

        ui->textEdit->insertPlainText("\r\n");
    }


    // non-annealing sequences
    ui->textEdit->insertPlainText("\r\n\r\nInformation for non-target sequences not annealing with primer-probe set: (");
    str = QString("%1").arg(seqPtrNonBindLeft.size() );
    ui->textEdit->insertPlainText(str + " sequences)\r\n\r\n");

    // cout primer sequences
    for( int i = 0; i < lprimerStr.length(); i++ ){
        str = lprimerStr.mid(i, 1);
        ui->textEdit->insertPlainText("\t"+str);
    }
    ui->textEdit->insertPlainText("\t..[");
    for( int i = 0; i < probeStr.length(); i++ ){
        str = probeStr.mid(i, 1);
        ui->textEdit->insertPlainText("\t"+str);
    }
    ui->textEdit->insertPlainText("\t]..\t");
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
            for(int j = 0; j < seqPtrNonBindLeft.size(); j++ ){
                if( seqPtrNonBindLeft[j][i] == base )
                    percent++;
            }
            if( seqPtrNonBindLeft.size() > 0 )
                percent =  percent / static_cast<float>(seqPtrNonBindLeft.size()) * 100;
            else
                percent = 0;
            str = QString("%1").arg(static_cast<double>(percent));
            ui->textEdit->insertPlainText("\t"+str+"%");
        }

        // probe annealing region
        ui->textEdit->insertPlainText("\t..[");
        for( int i = 0; i < probeStr.length(); i++ ){
            percent = 0;
            for(int j = 0; j < seqPtrNonBindProbe.size(); j++ ){
                if( seqPtrNonBindProbe[j][i] == base )
                    percent++;
            }
            if( seqPtrNonBindProbe.size() > 0 )
                percent =  percent / static_cast<float>(seqPtrNonBindProbe.size()) * 100;
            else
                percent = 0;
            str = QString("%1").arg(static_cast<double>(percent));
            ui->textEdit->insertPlainText("\t"+str+"%");
        }
        ui->textEdit->insertPlainText("\t]..\t");

        // right primer annealing region
        for( int i = rprimerStr.length() - 1; i >= 0; i-- ){
            percent = 0;
            for(int j = 0; j < seqPtrNonBindRight.size(); j++ ){
                if( seqPtrNonBindRight[j][i] == 3-base ) // 3 - base: make it reverse complement for reverse primer
                    percent++;
            }
            if( seqPtrNonBindRight.size() > 0 )
                percent =  percent / static_cast<float>(seqPtrNonBindRight.size()) * 100;
            else
                percent = 0;
            str = QString("%1").arg(static_cast<double>(percent));
            ui->textEdit->insertPlainText("\t"+str+"%");
        }

        ui->textEdit->insertPlainText("\r\n");
    }



// -------------update frequency, average, minimum and maximum product size to exact value------------
    /*int nItem = dispTripleSel;
    str = QString("%1").arg(pair->pctgInTargetSeqs);
    tableModel->setData(tableModel->index(dispTripleSel, A_TARGET_PCTG), str + "%");
    str = QString("%1").arg(pair->pcrSizeAvgTarSeqs);
    tableModel->setData(tableModel->index(dispTripleSel, A_TAR_PCR_AVG), str);
    str = QString("%1").arg(pair->pcrSizeMinTarSeqs);
    tableModel->setData(tableModel->index(dispTripleSel, A_TAR_PCR_MIN), str);
    str = QString("%1").arg(pair->pcrSizeMaxTarSeqs);
    tableModel->setData(tableModel->index(dispTripleSel, A_TAR_PCR_MAX), str);

    str = QString("%1").arg(pair->pctgInNonTarSeqs);
    tableModel->setData(tableModel->index(dispTripleSel, A_NONTAR_PCTG), str + "%");
    str = QString("%1").arg(pair->pcrSizeAvgNonTarSeqs);
    tableModel->setData(tableModel->index(dispTripleSel, A_NONTAR_PCR_AVG), str);
    str = QString("%1").arg(pair->pcrSizeMinNonTarSeqs);
    tableModel->setData(tableModel->index(dispTripleSel, A_NONTAR_PCR_MIN), str);
    str = QString("%1").arg(pair->pcrSizeMaxNonTarSeqs);
    tableModel->setData(tableModel->index(dispTripleSel, A_NONTAR_PCR_MAX), str);*/

// ------------house keeping clean---------------------------------------------
    delete[] lprimer;	delete[] rprimer;
    //delete lft;			delete rht;
    dispTripleSel = -1;
    delete pDlg;
}

void TripleReportFrame::targetWTriple()
{
    if(!getCurrSelectedItem())
        return ;

    int pos[6];
    ui->textEdit->clear();
    int range = tarSeqs.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Collecting triple annealing information",1, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);
    Triple *tp = tripleShowList[dispTripleSel];
    //PrimerPair *pr = tp->Pr;
    Probe *pb = tp->Pb;

    // get primer sequence
/*    int pLen = tripleShowList[dispTripleSel]->probeSeq.length();
    short *probe = new short[pLen+1];
    for( int i = 0; i < pLen; i++ )
        probe[i] = tools::base2int(tripleShowList[dispProbeSel]->probeSeq[i]);
    probe[pLen] = -1;

    ProbeSearch* ps;
    try{
        ps = new ProbeSearch(probe);
        delete probe;
    }
    catch(std::bad_alloc& e){
        QMessageBox::about(this,"Fatal error","Fatal error: out of memory for primer searching.");
        delete pDlg;
        return;
    }*/


    for(int i = 0; i < tarSeqs.size(); i++ ){
        pos[1] = pr->posBindingTar[0][i];
        pos[2] = pb->posBindingTar[0][i];
        pos[3] = pb->posBindingTar[1][i];
        pos[4] = pr->posBindingTar[1][i];

        pos[0] = pos[1] > -1 ? pos[1]- pr->left.length()+1:-1;
        pos[5] = pos[4] > -1 ? pos[4]+ pr->right.length()-1:-1;

        // search left and right primers in target sequence
        //ps->appxSearchTarSeqs(tarSeqs[i], pos[0], pos[1]);

        // only add those sequence having probes
        if(  pos[0] >= 0 && pos[2] >= 0 && pos[4] >= 0 && pos[1]<pos[2] && pos[3] <pos[4])
            appendSeq2Rich(tarSeqNames[i], tarSeqs[i], pos);
        pDlg->setValue(i+1);
    }

    //delete ps;
    dispTripleSel = -1;
    delete pDlg;
}

void TripleReportFrame::targetWoTriple()
{
    if(!getCurrSelectedItem())
        return ;

    int pos[6];

    ui->textEdit->clear();
    int range = tarSeqs.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Collecting probe annealing information",0, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);
    pDlg->setValue(0);
    Triple *tp = tripleShowList[dispTripleSel];
    //PrimerPair *pr = tp->Pr;
    Probe *pb = tp->Pb;



    // get primer sequence
    /*int pLen = tripleShowList[dispProbeSel]->probeSeq.length();
    short *probe = new short[pLen+1];
    for( int i = 0; i < pLen; i++ )
        probe[i] = tools::base2int(tripleShowList[dispProbeSel]->probeSeq[i]);
    probe[pLen] = -1;


    ProbeSearch* ps;
    try{
        ps = new ProbeSearch(probe);
        delete probe;
    }
    catch( std::bad_alloc& e){
        QMessageBox::about(this,"Fatal error","Fatal error: out of memory for primer searching.");
        delete pDlg;
        return;
    }*/


    for(int i = 0; i < tarSeqs.size(); i++ ){
        pos[1] = pr->posBindingTar[0][i];
        pos[2] = pb->posBindingTar[0][i];
        pos[3] = pb->posBindingTar[1][i];
        pos[4] = pr->posBindingTar[1][i];

        pos[0] = pos[1] > -1 ? pos[1]- pr->left.length()+1:-1;
        pos[5] = pos[4] > -1 ? pos[4]+ pr->right.length()-1:-1;
        // search left and right primers in target sequence
        //ps->appxSearchTarSeqs(tarSeqs[i],  pos[0], pos[1]);

        // only add those sequences not having probes
        if(pos[0] < 0 || pos[2] < 0 || pos[4] < 0 || pos[1] > pos[2] || pos[3] > pos[4]){
            appendSeq2Rich(tarSeqNames[i], tarSeqs[i], pos);
        }
        pDlg->setValue(i+1);
    }

    //delete ps;
    dispTripleSel = -1;
    delete pDlg;
}

void TripleReportFrame::nonTargetWTriple()
{
    if(!getCurrSelectedItem())
        return ;

    int pos[6];
    ui->textEdit->clear();
    int range = nontarSeqs.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Collecting probe annealing information",1, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);
    Triple *tp = tripleShowList[dispTripleSel];
    //PrimerPair *pr = tp->Pr;
    Probe *pb = tp->Pb;


    // get primer sequence
    /*int pLen = tripleShowList[dispProbeSel]->probeSeq.length();
    short *probe = new short[pLen+1];
    for( int i = 0; i < pLen; i++ )
        probe[i] = tools::base2int(tripleShowList[dispProbeSel]->probeSeq[i]);
    probe[pLen] = -1;

    ProbeSearch* ps;
    try{
        ps = new ProbeSearch(probe);
        delete probe;
    }
    catch( std::bad_alloc& e ){
            QMessageBox::about(this,"Fatal error","Fatal error: out of memory for primer searching.");
            delete pDlg;
            return;
    }*/

    // start searching
    for(int i = 0; i < nontarSeqs.size(); i++ ){
        pos[1] = pr->posBindingNontar[0][i];
        pos[2] = pb->posBindingNontar[0][i];
        pos[3] = pb->posBindingNontar[1][i];
        pos[4] = pr->posBindingNontar[1][i];

        pos[0] = pos[1] > -1 ? pos[1]- pr->left.length()+1:-1;
        pos[5] = pos[4] > -1 ? pos[4]+ pr->right.length()-1:-1;
        // search left and right primers in non-target sequence
        //ps->appxSearchNonTarSeqs(nontarSeqs[i], pos[0], pos[1]);

        // only add those sequence having probes
        if(   pos[0] >= 0 && pos[2] >= 0 && pos[4] >= 0 && pos[1]<pos[2] && pos[3] <pos[4] )
            this->appendSeq2Rich(nontarSeqNames[i], nontarSeqs[i], pos);

        pDlg->setValue(i+1);
    }

    //delete ps;
    dispTripleSel = -1;
    delete pDlg;
}

void TripleReportFrame::nonTargetWoTriple()
{
    if(!getCurrSelectedItem())
        return ;

    int pos[6];
    ui->textEdit->clear();
    int range = nontarSeqs.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Collecting probe annealing information",0, range, this);
    pDlg->setWindowModality(Qt::ApplicationModal);
    pDlg->setValue(0);
    Triple *tp = tripleShowList[dispTripleSel];
    //PrimerPair *pr = tp->Pr;
    Probe *pb = tp->Pb;


    // get primer sequence
    /*int pLen = tripleShowList[dispProbeSel]->probeSeq.length();
    short *probe = new short[pLen+1];
    for( int i = 0; i < pLen; i++ )
        probe[i] = tools::base2int(tripleShowList[dispProbeSel]->probeSeq[i]);
    probe[pLen] = -1;

    ProbeSearch* ps;
    try{
        ps = new ProbeSearch(probe);
        delete probe;
    }
    catch( std::bad_alloc& e ){
        QMessageBox::about(this,"Fatal error","Fatal error: out of memory for primer searching.");
        delete pDlg;
        return;
    }*/


    // start searching
    for(int i = 0; i < nontarSeqs.size(); i++ ){
        pos[1] = pr->posBindingNontar[0][i];
        pos[2] = pb->posBindingNontar[0][i];
        pos[3] = pb->posBindingNontar[1][i];
        pos[4] = pr->posBindingNontar[1][i];

        pos[0] = pos[1] > -1 ? pos[1]- pr->left.length()+1:-1;
        pos[5] = pos[4] > -1 ? pos[4]+ pr->right.length()-1:-1;
        // search left and right primers in non-target sequence
        //ps->appxSearchNonTarSeqs(nontarSeqs[i], pos[0], pos[1]);

        // only add those sequence not having primer pairs
        if( pos[0] < 0 || pos[2] < 0 || pos[4] < 0 || pos[1] > pos[2] || pos[3] > pos[4]){
            appendSeq2Rich(nontarSeqNames[i], nontarSeqs[i], pos);
        }

        pDlg->setValue(i+1);
    }

    //delete ps;
    dispTripleSel = -1;
    delete pDlg;
}

void TripleReportFrame::probeIntraComp()
{
    if(!getCurrSelectedItem())
        return ;

    QString probe = tripleShowList[dispTripleSel]->Pb->probeSeq;
    char* tmpProbe = probe.toLatin1().data();
    std::reverse(tmpProbe, tmpProbe+probe.length());
    QString probeRevs = QString(tmpProbe);

    dispTripleSel = -1;
    ui->textEdit->clear();
    ui->textEdit->append("Probe intra-complementarity\n\n");

    int lenProbe = probe.length();

    int maxComp, numComp, pos, shift, nSpace;
    QString str;

    // probe-probe inter-complement
    maxComp = 0;
    for( int start = 0; start < lenProbe; start++ ){
        numComp = 0;
        for( int i = start, j = lenProbe-1; i < lenProbe && j >= 0; i++, j-- ){
            if(  tools::base2int(probe[i]) + tools::base2int(probe[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = start;
            shift = 1;
        }
    }
    for( int start = lenProbe-1; start >= 0; start-- ){
        numComp = 0;
        for( int i = start, j = 0; i >= 0 && j < lenProbe; i--, j++ ){
            if( tools::base2int(probe[i]) + tools::base2int(probe[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = start;
            shift = 2;
        }
    }

    str = "\nProbe-probe intra-complementarity ";
    str += QString("%1.\n").arg(maxComp);
    if( maxComp > 1 ){

        if( shift == 1 ){
            str += "5'-" + probe +"-3'\n";
            str += "   ";
            for( int i = 0; i < pos; i++ )
                str += " ";
            for( int i = pos, j = lenProbe-1; i < lenProbe && j >= 0; i++, j-- ){
                if( tools::base2int(probe[i]) + tools::base2int(probe[j]) == 3 )
                    str += "|";
                else
                    str += " ";
            }
            str += "\n";

            for( int i = 0; i < pos; i++ )
                str += " ";
            str += "3'-" + probeRevs + "-5'\n";
        }
        else{
            nSpace = lenProbe - (pos+1);
            for( int i = 0; i < nSpace; i++ )
                str += " ";
            str += "5'-" + probe + "-3'\n";

            str += "   ";
            for( int i = 0; i < nSpace; i++ )
                    str += " ";
            for( int i = 0, j = pos; i <= pos && j >= 0; i++, j-- ){
                if( tools::base2int(probe[i]) + tools::base2int(probe[j]) == 3 )
                    str += "|";
                else
                    str += " ";
            }
            str += "\n";

            str += "3'-" + probeRevs + "-5'\n";
        }
    }
    ui->textEdit->append(str);
    /*QString probe = tripleShowList[dispTripleSel]->Pb->probeSeq;

    dispTripleSel = -1;
    ui->textEdit->clear();
    QString str;
    str = "probe intra-complementarity\n\n";

    int lenProbe =probe.length();

    int maxComp, numComp, pos, shift, nSpace;


    // probe intra-complement
    maxComp = 0;
    for( int lEnd = 0; lEnd < lenProbe-1; lEnd++ ){
        numComp = 0;
        for( int i = lEnd, j = lenProbe-1; i < j-1; i++, j-- ){
            if( tools::base2int(probe[i]) + tools::base2int(probe[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = lEnd;
            shift = 1;
        }
    }
    for( int rEnd = lenProbe-1; rEnd > 0; rEnd-- ){
        numComp = 0;
        for( int i = 0, j = rEnd; i < j-1; i++, j-- ){
            if( tools::base2int(probe[i]) + tools::base2int(probe[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = rEnd;
            shift = 2;
        }
    }
    str += "\nProbe intra-complementarity: ";
    str = QString("%1.\n").arg(maxComp);
    if( maxComp > 1 ){
        if( shift == 1 ){
            str = "5'-";
            for( int i = 0; i < pos + ceil( (float)(lenProbe-pos)/2.0 ); i++ )
                str += probe[i];
            str += "\n";

            str += "   ";
            for( int i = 0; i < pos; i++ )
                    str += " ";
            for( int i = pos, j =  lenProbe-1; i < j-1; i++, j-- ){
                if( tools::base2int(probe[i]) + tools::base2int(probe[j]) == 3 )
                    str += "|";
                else
                    str += " ";
            }
            str += "\n";

            for( int i = 0; i < pos; i++ )
                str += " ";
            str += "3'-";
            for( int i = lenProbe-1; i >= lenProbe - (lenProbe-pos)/2.0; i-- )
                str += probe[i];
            str += "\n";
        }
        else{
            nSpace = lenProbe - (pos+1);
            for( int i = 0; i < nSpace; i++ )
                str += " ";
            str += "5'-";
            for( int i = 0; i < ceil((float)pos/2.0); i++ )
                str += probe[i];
            str += "\n";

            str += "   ";
            for( int i = 0; i < nSpace; i++ )
                str += " ";
            for( int i = 0, j = pos; i < j-1; i++, j-- ){
                if( tools::base2int(probe[i]) + tools::base2int(probe[j]) == 3 )
                    str += "|";
                else
                    str += " ";
            }
            str += "\n";

            str += "3'-";
            for( int i = lenProbe-1; i >= ceil((float)pos/2.0); i-- )
                str += probe[i];
            str += "\n";
        }
    }
//    str = "<span style=\" color:#000000;\">"+str +"</span>";
    ui->textEdit->append(str);*/
}

//void TripleReportFrame::probe3IntraComp()
//{
    /*if(!getCurrSelectedItem())
        return ;

    QString probe = tripleShowList[dispTripleSel]->Pb->probeSeq;
    char* tmpProbe = probe.toAscii().data();
    std::reverse(tmpProbe, tmpProbe+probe.length());
    QString probeRevs = QString(tmpProbe);

    dispTripleSel = -1;
    ui->textEdit->clear();
    ui->textEdit->append("Probe 3' intra-complementarity\n\n");

    int lenProbe = probe.length();
    int start, maxComp, numComp, pos;
    QString str;

    // probe-probe intra-complement
    maxComp = 0;
    if( probeIntraBase3end < lenProbe )
        start = lenProbe - probeIntraBase3end;
    else
        start = 0;
    for( ; start < lenProbe; start++ ){
        numComp = 0;
        for( int i = start, j = lenProbe-1; i < lenProbe && j >= 0; i++, j-- ){
            if(  tools::base2int(probe[i]) + tools::base2int(probe[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = start;
        }
    }

    str = "\nProbe-Probe 3' intra-complementarity ";
    str += QString("%1.\n").arg(maxComp);
    if( maxComp > 1 ){
        str += "5'-" + probe +"-3'\n";

        str += "   ";
        for( int i = 0; i < pos; i++ )
            str += " ";
        for( int i = pos, j = lenProbe-1; i < lenProbe && j >= 0; i++, j-- ){
            if( tools::base2int(probe[i]) + tools::base2int(probe[j]) == 3 )
                str += "|";
            else
                str += " ";
        }
        str += "\n";

        for( int i = 0; i < pos; i++ )
            str.append(" ");
        str += "3'-" + probeRevs + "-5'\n";
    }
    ui->textEdit->append(str);*/
//}

void TripleReportFrame::tripleInterComp()
{
    if(!getCurrSelectedItem())
        return ;

    QString probe = tripleShowList[dispTripleSel]->Pb->probeSeq;
    //char* tmpProbe = probe.toStdString().c_str();
    QString probeRevs(probe);
    std::reverse(probeRevs.begin(), probeRevs.end());
    //QString probeRevs = QString(tmpProbe);

    QString left = pr->left;
    //char* tmpLeft = left.toStdString().c_str();
    QString leftRevs(left);
    std::reverse(leftRevs.begin(), leftRevs.end());
    //QString leftRevs = QString(tmpLeft);

    QString right = pr->rightRevsComp;
    //char* tmpRight = right.toStdString().c_str();
    QString rightRevs(right);
    std::reverse(rightRevs.begin(), rightRevs.end());
    //QString rightRevs = QString(tmpRight);

    dispTripleSel = -1;
    ui->textEdit->clear();
    ui->textEdit->append("Primer-Probe set inter-complementarity\n\n");

    int lenProbe = probe.length();
    int lenLeft = left.length();
    int lenRight= right.length();

    int maxComp, numComp, pos, shift, nSpace;
    QString str;

    // 1. probe-left inter-complement
    maxComp = 0;
    for( int start = 0; start < lenProbe; start++ ){
        numComp = 0;
        for( int i = start, j = lenLeft-1; i < lenProbe && j >= 0; i++, j-- ){
            if(  tools::base2int(probe[i]) + tools::base2int(left[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = start;
            shift = 1;
        }
    }
    for( int start = lenProbe-1; start >= 0; start-- ){
        numComp = 0;
        for( int i = start, j = 0; i >= 0 && j < lenLeft; i--, j++ ){
            if( tools::base2int(probe[i]) + tools::base2int(left[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = start;
            shift = 2;
        }
    }

    str = "\nProbe-Left primer inter-complementarity ";
    str += QString("%1.\n").arg(maxComp);
    if( maxComp > 1 ){

        if( shift == 1 ){
            str += "5'-" + probe +"-3'\n";

            str += "   ";
            for( int i = 0; i < pos; i++ )
                str += " ";
            for( int i = pos, j = lenLeft-1; i < lenProbe && j >= 0; i++, j-- ){
                if( tools::base2int(probe[i]) + tools::base2int(left[j]) == 3 )
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
            //str.clear();
            for( int i = 0; i < nSpace; i++ )
                str += " ";
            str += "5'-" + probe + "-3'\n";

            str += "   ";
            for( int i = 0; i < nSpace; i++ )
                str += " ";
            for( int i = 0, j = pos; i <= pos && j >= 0; i++, j-- ){
                if( tools::base2int(probe[i]) + tools::base2int(left[j]) == 3 )
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
    //else
    //    ui->textEdit->append("0.\n");

    // 2. probe-right inter-complement
    maxComp = 0;
    for( int start = 0; start < lenProbe; start++ ){
        numComp = 0;
        for( int i = start, j = lenRight-1; i < lenProbe && j >= 0; i++, j-- ){
            if(  tools::base2int(probe[i]) + tools::base2int(right[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = start;
            shift = 1;
        }
    }
    for( int start = lenProbe-1; start >= 0; start-- ){
        numComp = 0;
        for( int i = start, j = 0; i >= 0 && j < lenRight; i--, j++ ){
            if( tools::base2int(probe[i]) + tools::base2int(right[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = start;
            shift = 2;
        }
    }

    str = "\nProbe-Right primer inter-complementarity ";
    str += QString("%1.\n").arg(maxComp);
    if( maxComp > 1 ){

        if( shift == 1 ){
            str += "5'-" + probe +"-3'\n";

            str += "   ";
            for( int i = 0; i < pos; i++ )
                str += " ";
            for( int i = pos, j = lenRight-1; i < lenProbe && j >= 0; i++, j-- ){
                if( tools::base2int(probe[i]) + tools::base2int(right[j]) == 3 )
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
            str += "5'-" + probe + "-3'\n";

            str += "   ";
            for( int i = 0; i < nSpace; i++ )
                str += " ";
            for( int i = 0, j = pos; i <= pos && j >= 0; i++, j-- ){
                if( tools::base2int(probe[i]) + tools::base2int(right[j]) == 3 )
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

    // 3. left-right inter-complement
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

    str = "\nLeft-Right primer inter-complementarity ";
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
                str += " ";
            str += "3'-" + rightRevs + "-5'\n";
        }
        else{
            nSpace = lenRight - (pos+1);

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


}
/*
void TripleReportFrame::triple3InterComp()
{
    if(!getCurrSelectedItem())
        return ;

    QString probe = tripleShowList[dispTripleSel]->Pb->probeSeq;
    char* tmpProbe = probe.toAscii().data();
    std::reverse(tmpProbe, tmpProbe+probe.length());
    QString probeRevs = QString(tmpProbe);

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
    ui->textEdit->append("Primer-Probe set 3' inter-complementarity\n\n");

    int lenProbe = probe.length();
    int lenLeft = left.length();
    int lenRight= right.length();

    int start, maxComp, numComp, pos;
    QString str;

    // 1. probe-left inter-complement
    maxComp = 0;
    if( tripleInterBase3end < lenProbe )
        start = lenProbe - tripleInterBase3end;
    else
        start = 0;
    for( ; start < lenProbe; start++ ){
        numComp = 0;
        for( int i = start, j = lenLeft-1; i < lenProbe && j >= 0; i++, j-- ){
            if(  tools::base2int(probe[i]) + tools::base2int(left[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = start;
        }
    }

    str = "\nProbe-Left primer 3' inter-complementarity ";
    str += QString("%1.\n").arg(maxComp);
    if( maxComp > 1 ){
        str += "5'-" + probe +"-3'\n";

        str += "   ";
        for( int i = 0; i < pos; i++ )
            str += " ";
        for( int i = pos, j = lenLeft-1; i < lenProbe && j >= 0; i++, j-- ){
            if( tools::base2int(probe[i]) + tools::base2int(left[j]) == 3 )
                str += "|";
            else
                str += " ";
        }
        str += "\n";

        for( int i = 0; i < pos; i++ )
            str += " ";
        str += "3'-" + leftRevs + "-5'\n";
    }
    ui->textEdit->append(str);

    // 2. probe-right inter-complement
    maxComp = 0;
    if( tripleInterBase3end < lenProbe )
        start = lenProbe - tripleInterBase3end;
    else
        start = 0;
    for( ; start < lenProbe; start++ ){
        numComp = 0;
        for( int i = start, j = lenRight-1; i < lenProbe && j >= 0; i++, j-- ){
            if(  tools::base2int(probe[i]) + tools::base2int(right[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp ){
            maxComp = numComp;
            pos = start;
        }
    }

    str = "\nProbe-Right primer 3' inter-complementarity ";
    str += QString("%1.\n").arg(maxComp);
    if( maxComp > 1 ){
        str += "5'-" + probe +"-3'\n";

        str += "   ";
        for( int i = 0; i < pos; i++ )
            str += " ";
        for( int i = pos, j = lenRight-1; i < lenProbe && j >= 0; i++, j-- ){
            if( tools::base2int(probe[i]) + tools::base2int(right[j]) == 3 )
                str += "|";
            else
                str += " ";
        }
        str += "\n";

        for( int i = 0; i < pos; i++ )
            str += " ";
        str += "3'-" + rightRevs + "-5'\n";
    }
    ui->textEdit->append(str);

    // 3. left-right inter-complement
    maxComp = 0;
    if( tripleInterBase3end < lenLeft )
        start = lenLeft - tripleInterBase3end;
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

    str = "\nLeft-Right primer 3' inter-complementarity ";
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
            str += " ";
        str += "3'-" + rightRevs + "-5'\n";
    }
    ui->textEdit->append(str);
}*/

void TripleReportFrame::showPrimerPairInfo()
{
    ui->textEdit->clear();
    QString str;
    str = "Primer Pair Information: \n\n";

    str += "Primer Pair\t " +pr->left+ ".." + pr->rightRevsComp + "\n";
    str += "Target Sequences Binding Percentage\n";
    str += "   Pair  " + QString("%1").arg(static_cast<double>(pr->pctgInTargetSeqs)) + "\n";
    str += "   Left  " + QString("%1").arg(static_cast<double>(pr->pctgInTarSeqsLeft)) + "\n";
    str += "   Right " + QString("%1").arg(static_cast<double>(pr->pctgInTarSeqsRight)) + "\n";
    str += "Non-target Sequences Binding Percentage\n";
    str += "   Pair  " + QString("%1").arg(static_cast<double>(pr->pctgInNonTarSeqs)) + "\n";
    str += "   Left  " + QString("%1").arg(static_cast<double>(pr->pctgInNonTarSeqsLeft)) + "\n";
    str += "   Right " + QString("%1").arg(static_cast<double>(pr->pctgInNonTarSeqsRight)) + "\n";
    str += "Target Sequences Average Binding Position\n";
    str += "   Left  " + QString("%1").arg(static_cast<double>(pr->bindPosTarSeqsAvgLeft)) + "\n";
    str += "   Right " + QString("%1").arg(static_cast<double>(pr->bindPosTarSeqsAvgRight)) + "\n";
    str += "PCR Information on Target Sequences\n";
    str += "   Average  " + QString("%1").arg(static_cast<double>(pr->pcrSizeAvgTarSeqs)) + "\n";
    str += "   Max      " + QString("%1").arg(pr->pcrSizeMaxTarSeqs) + "\n";
    str += "   Min      " + QString("%1").arg(pr->pcrSizeMinTarSeqs) + "\n";
    str += "PCR Information on Non-target Sequences\n";
    str += "   Average " + QString("%1").arg(static_cast<double>(pr->pcrSizeAvgNonTarSeqs)) + "\n";
    str += "   Max     " + QString("%1").arg(pr->pcrSizeMaxNonTarSeqs) + "\n";
    str += "   Min     " + QString("%1").arg(pr->pcrSizeMinNonTarSeqs) + "\n";
    str += "Primer TM\n";
    str += "   Left  " + QString("%1").arg(static_cast<double>(pr->tmLeft)) + "\n";
    str += "   Right " + QString("%1").arg(static_cast<double>(pr->tmRight)) + "\n";
    str += "Primer GC\n";
    str += "   Left  " + QString("%1").arg(static_cast<double>(pr->GCLeft)) + "\n";
    str += "   Right " + QString("%1").arg(static_cast<double>(pr->GCRight)) + "\n";
    str += "Complementary\n";
    str += "   Left Intra         " + QString("%1").arg(pr->selfCompLeft) + "\n";
    str += "   Left Intra 3' end  " + QString("%1").arg(pr->selfCompRight) + "\n";
    str += "   Right Intra        " + QString("%1").arg(pr->self3CompLeft) + "\n";
    str += "   Right Intra 3' end " + QString("%1").arg(pr->self3CompRight) + "\n";

    str += "   Inter:        " + QString("%1").arg(pr->prPrComp) + "\n";
    str += "   Inter 3' end: " + QString("%1").arg(pr->prPr3Comp) + "\n";

    ui->textEdit->append(str);
}

void TripleReportFrame::deleteTriple()
{
    // to confirm delete
    MyConfirmDlg cDlg("Are you sure you want to delete triple(s)?\n\n\n\n",this);
    if(!cDlg.exec())
        return;

    ui->textEdit->clear();
    // retrieve the selected triple sequences
    QString tripleSelected = tableModel->item( dispTripleSel, B_PROBE)->text();


    // delete the selected triple from primer pair table
    delete tableModel->item(dispTripleSel,0);
    tableModel->removeRow(dispTripleSel);

    // delete the selected primer pair from current primer pair memory
    if( DataTools::deleteTripleFromTripleMem( tripleSelected, tripleShowList, 0 ) < 0 )
            return;
    // delete the triple from theApp memory and destroy the memory
    if( DataTools::deleteTripleFromTripleMem( tripleSelected, pr->tripleList, 1 ) < 0 )
            return;
}

void TripleReportFrame::click(const QModelIndex & idx)
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

void TripleReportFrame::rightClick (const QPoint & point)
{
    const QModelIndex idx = ui->tableView->indexAt(point);
    dispTripleSel = idx.row();
    showRightClickMenu();
}

void TripleReportFrame::blastProbe()
{
    if(!getCurrSelectedItem())
        return ;

    QString url = "http://www.ncbi.nlm.nih.gov/BLAST/Blast.cgi?QUERY=";
    url +=	tripleShowList[dispTripleSel]->Pb->probeSeq +
                    "&CMD=Web&LAYOUT=TwoWindows&AUTO_FORMAT=Semiauto"
                    "&ALIGNMENTS=50&ALIGNMENT_VIEW=Pairwise&CLIENT=web"
                    "&DATABASE=nr&DESCRIPTIONS=100&ENTREZ_QUERY=(none)"
                    "&EXPECT=1000&FORMAT_OBJECT=Alignment&FORMAT_TYPE=HTML"
                    "&NCBI_GI=on&PAGE=Nucleotides&PROGRAM=blastn"
                    "&SERVICE=plain&SET_DEFAULTS.x=29&SET_DEFAULTS.y=6"
                    "&SHOW_OVERVIEW=on&WORD_SIZE=7&END_OF_HTTPGET=Yes"
                    "&SHOW_LINKOUT=yes&GET_SEQUENCE=yes";

    dispTripleSel = -1;

//#ifdef LINUX
//    QString commend = "xdg-open "+url;
//    system(commend.toAscii().data());
//#elif defined(WINDOWS)
    QDesktopServices::openUrl(QUrl(QString(url)));
//#endif
}


void TripleReportFrame::showRightClickMenu ()
{
  QMenu *popupMenu;
  QAction *actionTripleAnnealPosInfo;
  QAction *actionPercentage;
  QAction *actionTargetWTriple;
  QAction *actionTargetWoTriple;
  QAction *actionNonTargetWTriple;
  QAction *actionNonTargetWoTriple;
  //QAction *actionGetExactPCRinfo;
  QAction *actionTripleInterComp;
  //QAction *actionTriple3InterComp;
  QAction *actionProbeIntraComp;
  //QAction *actionProbe3IntraComp;
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
  actionPercentage->setObjectName(QString::fromUtf8("Percentage of Each Nucleotide in Target and Non-target Sequences in Relation to Triple Sequences"));
  actionPercentage->setText(QString::fromUtf8("Percentage of Each Nucleotide in Target and Non-target Sequences in Relation to Triple Sequences"));

  actionTargetWTriple = new QAction(ui->tableView);
  actionTargetWTriple->setObjectName(QString::fromUtf8("Target Sequences Annealing with Triple"));
  actionTargetWTriple->setText(QString::fromUtf8("Target Sequences Annealing with Triple"));

  actionTargetWoTriple = new QAction(ui->tableView);
  actionTargetWoTriple->setObjectName(QString::fromUtf8("Target Sequences Not Annealing with Triple"));
  actionTargetWoTriple->setText(QString::fromUtf8("Target Sequences Not Annealing with Triple"));

  actionNonTargetWTriple = new QAction(ui->tableView);
  actionNonTargetWTriple->setObjectName(QString::fromUtf8("Non-target Sequences Annealing with Triple"));
  actionNonTargetWTriple->setText(QString::fromUtf8("Non-target Sequences Annealing with Triple"));

  actionNonTargetWoTriple = new QAction(ui->tableView);
  actionNonTargetWoTriple->setObjectName(QString::fromUtf8("Non-target Sequences Not Annealing with Triple"));
  actionNonTargetWoTriple->setText(QString::fromUtf8("Non-target Sequences Not Annealing with Triple"));

  //actionGetExactPCRinfo = new QAction(ui->tableView);
  //actionGetExactPCRinfo->setObjectName(QString::fromUtf8("Update Estimated Information to Exact Information"));
  //actionGetExactPCRinfo->setText(QString::fromUtf8("Update Estimated Information to Exact Information"));

  actionTripleInterComp = new QAction(ui->tableView);
  actionTripleInterComp->setObjectName(QString::fromUtf8("Triple Inter-Complementarity"));
  actionTripleInterComp->setText(QString::fromUtf8("Triple Inter-Complementarity"));

  //actionTriple3InterComp = new QAction(ui->tableView);
  //actionTriple3InterComp->setObjectName(QString::fromUtf8("Triple 3' Inter-Complementarity"));
  //actionTriple3InterComp->setText(QString::fromUtf8("Triple 3' Inter-Complementarity"));

  actionProbeIntraComp = new QAction(ui->tableView);
  actionProbeIntraComp->setObjectName(QString::fromUtf8("Probe Intra-Complementarity"));
  actionProbeIntraComp->setText(QString::fromUtf8("Probe Intra-Complementarity"));

  //actionProbe3IntraComp = new QAction(ui->tableView);
  //actionProbe3IntraComp->setObjectName(QString::fromUtf8("Probe 3' Intra-Complementarity"));
  //actionProbe3IntraComp->setText(QString::fromUtf8("Probe 3' Intra-Complementarity"));



  /*actionBlastForwardPrimer = new QAction(ui->tableView);
  actionBlastForwardPrimer->setObjectName(QString::fromUtf8("BLAST Forward Primer"));
  actionBlastForwardPrimer->setText(QString::fromUtf8("BLAST Forward Primer"));

  actionBlastReversePrimer = new QAction(ui->tableView);
  actionBlastReversePrimer->setObjectName(QString::fromUtf8("BLAST Reverse Primer"));
  actionBlastReversePrimer->setText(QString::fromUtf8("BLAST Reverse Primer"));*/

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
  //popupMenu->addAction(actionGetExactPCRinfo);
  popupMenu->addAction(actionTripleInterComp);
  //popupMenu->addAction(actionTriple3InterComp);
  popupMenu->addAction(actionProbeIntraComp);
  //popupMenu->addAction(actionProbe3IntraComp);

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
  //connect(actionGetExactPCRinfo, SIGNAL(triggered()), this, SLOT(getExactPCRinfo()));
  connect(actionTripleInterComp, SIGNAL(triggered()), this, SLOT(tripleInterComp()));
  //connect(actionTriple3InterComp, SIGNAL(triggered()), this, SLOT(triple3InterComp()));
  connect(actionProbeIntraComp, SIGNAL(triggered()), this, SLOT(probeIntraComp()));
  //connect(actionProbe3IntraComp, SIGNAL(triggered()), this, SLOT(probe3IntraComp()));
  //connect(actionProbeIntraComp, SIGNAL(triggered()), this, SLOT(probeIntraComp()));
  //connect(actionBlastForwardPrimer, SIGNAL(triggered()), this, SLOT(blastForwardPrimer()));
  //connect(actionBlastReversePrimer, SIGNAL(triggered()), this, SLOT(blastReversePrimer()));
  connect(actionBlastProbe, SIGNAL(triggered()), this, SLOT(blastProbe()));
  connect(actionDeleteTriple, SIGNAL(triggered()), this, SLOT(deleteTriple()));

  popupMenu->exec(QCursor::pos());
  delete popupMenu;
  dispTripleSel = -1;
}

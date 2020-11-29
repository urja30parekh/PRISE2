#include "ProbeItemDelegate.h"
//#include "QStyledItemDelegate"

void ProbeItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &idx ) const
{
    int colIdx = idx.column();
    int rowIdx = idx.row();

    assert(colIdx == 1);
    assert(rowIdx == 0);

    QItemDelegate::setModelData(editor, model, idx);

    QString probeStr = model->data(idx).toString();
    probeStr = probeStr.toUpper();

    QRegExp rx("^[ACGTUacgtu]+$");

    if(!probeStr.isEmpty() && probeStr.indexOf(rx)>=0){
        int numTriple; //Occurrence of Triple
        int numTarSeqs = (int)tarSeqs.size();
        int numNonTarSeqs = (int)nontarSeqs.size();
        int bdPos[2];

        QVector<Probe*> pbList;

        TripleReportFrame* parentFrame = ((TripleReportFrame*)(model->parent()));
        PrimerPair* pr = parentFrame->getPrimerPair();

        Probe *pb = new Probe();
        Triple *tp = new Triple();
        tp->Pb = pb;

        pb->probeSeq = probeStr;
        pbList.push_back(pb);

        DataTools::buildSimCost(probeCostType);
        Designer::updataProbeInfo(pbList);

        model->setData(model->index(rowIdx,1),probeStr.toUpper());

        probeList = pbList + probeList;
        pr->tripleList.push_front(tp);
        parentFrame->addTripleShowList(tp,0);


        numTriple = 0;
        for( int k = 0; k < numTarSeqs; k++ ){
            bdPos[0] = pb->posBindingTar[0][k];
            bdPos[1] = pb->posBindingTar[1][k];

            int leftEnd = pr->posBindingTar[0][k];
            int rightEnd = pr->posBindingTar[1][k];
            if( bdPos[0]>=0 && leftEnd >=0 && rightEnd >=0  //both primer and probe exist
                 && leftEnd + probeMinGap < bdPos[0] && rightEnd > bdPos[1]+ probeMinGap){  //and has correct positions
                assert(bdPos[1]>0);
                numTriple++;
            }
        }
        //tp->pctgInTargetSeqs =numTarSeqs>0 ? ((float)numTriple / numTarSeqs * 100):0;
        if( numTarSeqs != 0 )
            tp->pctgInTargetSeqs = (float)numTriple / numTarSeqs * 100;
        else
            tp->pctgInTargetSeqs = 0;

        numTriple = 0;
        for( int k = 0; k < numNonTarSeqs; k++ ){
            bdPos[0] = pb->posBindingNontar[0][k];
            bdPos[1] = pb->posBindingNontar[1][k];

            int leftEnd = pr->posBindingNontar[0][k];
            int rightEnd = pr->posBindingNontar[1][k];
            if( bdPos[0]>=0 && leftEnd >=0 && rightEnd >=0  //both primer and probe exist
                 && leftEnd + probeMinGap < bdPos[0] && rightEnd > bdPos[1] + probeMinGap){  //and has correct positions
                assert(bdPos[1]>0);
                numTriple++;
            }
        }
        //tp->pctgInNonTarSeqs =numNonTarSeqs>0 ? ((float)numTriple / numNonTarSeqs * 100):0;
        if( numNonTarSeqs != 0 )
            tp->pctgInNonTarSeqs = (float)numTriple / numNonTarSeqs * 100;
        else
            tp->pctgInNonTarSeqs = 0;


        tp->pbPrComp = Designer::tripleInterComplementarity(pb->probeSeq, pr->left, pr->rightRevsComp);
        //tp->pbPr3Comp = Designer::triple3InterComplementarity(pb->probeSeq, pr->left, pr->rightRevsComp);



        // frequency for single primers
        /*model->setData(model->index(0, A_LFT_TARGET_PCTG),  pb->pctgInTarSeqsLeft);
        model->setData(model->index(0, A_RHT_TARGET_PCTG),  pb->pctgInTarSeqsRight);
        model->setData(model->index(rowIdx, A_LFT_NONTAR_PCTG),  pb->pctgInNonTarSeqsLeft);
        model->setData(model->index(rowIdx, A_RHT_NONTAR_PCTG),  pb->pctgInNonTarSeqsRight);

        // average annealing position in target sequences: left and right primer
        if( pair->bindPosTarSeqsAvgLeft < 0 )
            model->setData(model->index(rowIdx, A_LFT_TAR_BIND_POS),  "-");
        else{
            model->setData(model->index(rowIdx, A_LFT_TAR_BIND_POS),  pair->bindPosTarSeqsAvgLeft);
        }
        if( pair->bindPosTarSeqsAvgRight < 0 )
            model->setData(model->index(rowIdx, A_RHT_TAR_BIND_POS),  "-");
        else{
            model->setData(model->index(rowIdx, A_RHT_TAR_BIND_POS),  pair->bindPosTarSeqsAvgRight);
        }


        model->setData(model->index(rowIdx, A_TARGET_PCTG),  pair->pctgInTargetSeqs);
        model->setData(model->index(rowIdx, A_TAR_PCR_AVG),  pair->pcrSizeAvgTarSeqs);
        model->setData(model->index(rowIdx, A_TAR_PCR_MIN),  pair->pcrSizeMinTarSeqs);
        model->setData(model->index(rowIdx, A_TAR_PCR_MAX),  pair->pcrSizeMaxTarSeqs);

        model->setData(model->index(rowIdx, A_NONTAR_PCTG),  pair->pctgInNonTarSeqs);
        model->setData(model->index(rowIdx, A_NONTAR_PCR_AVG),  pair->pcrSizeAvgNonTarSeqs);
        model->setData(model->index(rowIdx, A_NONTAR_PCR_MIN),  pair->pcrSizeMinNonTarSeqs);
        model->setData(model->index(rowIdx, A_NONTAR_PCR_MAX),  pair->pcrSizeMaxNonTarSeqs);
        // left primer GC%
        model->setData(model->index(rowIdx, A_LFT_GC),  pair->GCLeft);
        // right primer GC%
        model->setData(model->index(rowIdx, A_RHT_GC),  pair->GCRight);
        // left primer Tm
        model->setData(model->index(rowIdx, A_LFT_TM),  pair->tmLeft);
        // right primer Tm
        model->setData(model->index(rowIdx, A_RHT_TM),  pair->tmRight);
        // inter-complementarity
        model->setData(model->index(rowIdx, A_INTER_COMP),  pair->prPrComp);
        // 3' inter-complementarity
        model->setData(model->index(rowIdx, A_INTER3_COMP),  pair->prPr3Comp);
        // self-complementarity
        model->setData(model->index(rowIdx, A_LFT_INTRA_COMP),  pair->selfCompLeft);
        model->setData(model->index(rowIdx, A_RHT_INTRA_COMP),  pair->selfCompRight);
        // 3'self-complementarity
        model->setData(model->index(rowIdx, A_LFT_INTRA3_COMP),  pair->self3CompLeft);
        model->setData(model->index(rowIdx, A_RHT_INTRA3_COMP),  pair->self3CompRight);

        model->setData(model->index(rowIdx, A_NUM_AMBI_TARGETS_LEFT),  pair->numAmbiguousTargetsLeft);
        model->setData(model->index(rowIdx, A_NUM_AMBI_TARGETS_RIGHT),  pair->numAmbiguousTargetsRight);
        model->setData(model->index(rowIdx, A_NUM_AMBI_BASES_LEFT),  pair->numAmbiguousBasesLeft);
        model->setData(model->index(rowIdx, A_NUM_AMBI_BASES_RIGHT),  pair->numAmbiguousBasesRight);
        model->setData(model->index(rowIdx, A_NUM_TAR), tarSeqs.size());
        */

        // frequency for single primers
        QString val;
        val.sprintf("%.2f", tp->pctgInTargetSeqs);
        model->setData(model->index(rowIdx, B_TARGET_PCTG),  val);
        ((QStandardItemModel*)model)->item(rowIdx, B_TARGET_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
        val.sprintf("%.2f", pb->pctgInTargetSeqs);
        model->setData(model->index(rowIdx, B_PROBE_TARGET_PCTG),  val);
        ((QStandardItemModel*)model)->item(rowIdx, B_PROBE_TARGET_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);

        val.sprintf("%.2f", tp->pctgInNonTarSeqs);
        model->setData(model->index(rowIdx, B_NONTAR_PCTG),  val);
        ((QStandardItemModel*)model)->item(rowIdx, B_NONTAR_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
        val.sprintf("%.2f", pb->pctgInNonTarSeqs);
        model->setData(model->index(rowIdx, B_PROBE_NONTAR_PCTG),  val);
        ((QStandardItemModel*)model)->item(rowIdx, B_PROBE_NONTAR_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);

        // average annealing position in target sequences:
        if( pb->bindPosTarSeqsAvg[0]< 0 )
            model->setData(model->index(rowIdx, B_PROBE_TAR_BIND_POS),  "-");
        else{
            val.sprintf("%.2f", pb->bindPosTarSeqsAvg[0]);
            model->setData(model->index(rowIdx, B_PROBE_TAR_BIND_POS),  val);
            ((QStandardItemModel*)model)->item(rowIdx, B_PROBE_TAR_BIND_POS)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
        }

        // probe GC%
        val.sprintf("%.2f", pb->GCNum);
        model->setData(model->index(rowIdx, B_PROBE_GC),  val);
        ((QStandardItemModel*)model)->item(rowIdx, B_PROBE_GC)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
        // probe Tm
        val.sprintf("%.2f", pb->tmProbe);
        model->setData(model->index(rowIdx, B_PROBE_TM),  val);
        ((QStandardItemModel*)model)->item(rowIdx, B_PROBE_TM)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
        // inter-complementarity
        model->setData(model->index(rowIdx, B_INTER_COMP),  tp->pbPrComp);
        ((QStandardItemModel*)model)->item(rowIdx, B_INTER_COMP)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
        // 3' inter-complementarity
        //model->setData(model->index(rowIdx, B_INTER3_COMP),  tp->pbPr3Comp);
        //((QStandardItemModel*)model)->item(rowIdx, B_INTER3_COMP)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
        // self-complementarity
        model->setData(model->index(rowIdx, B_PROBE_INTRA_COMP),  pb->selfComp);
        ((QStandardItemModel*)model)->item(rowIdx, B_PROBE_INTRA_COMP)->setData(Qt::AlignRight, Qt::TextAlignmentRole);

        // 3'self-complementarity
        //model->setData(model->index(rowIdx, B_PROBE_INTRA3_COMP),  pb->self3Comp);
        //((QStandardItemModel*)model)->item(rowIdx, B_PROBE_INTRA3_COMP)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
        model->setData(model->index(rowIdx, B_NUM_AMBI_TARGETS),  pb->numAmbiguousTargets);
        ((QStandardItemModel*)model)->item(rowIdx, B_NUM_AMBI_TARGETS)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
        model->setData(model->index(rowIdx, B_NUM_AMBI_BASES),  pb->numAmbiguousBases);
        ((QStandardItemModel*)model)->item(rowIdx, B_NUM_AMBI_BASES)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
        //model->setData(model->index(rowIdx, B_NUM_TAR),  tarSeqs.size());


        return;
    }

    QString str = QString("Incorrect input, input must be a strings of\r\n"
                          "letters A,G,C,T.\r\n\r\n"
                          "For example AGCCTTAGCGAAC");
    QMessageBox::about(0,"Error",str);

    // delete the selected primer pair from primer pair table
    delete ((QStandardItemModel*)model)->item(rowIdx,0);
    model->removeRow(rowIdx);
}



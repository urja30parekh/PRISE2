#include "PrimerItemDelegate.h"
#include "QStyledItemDelegate"

void PrimerItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &idx ) const
{
    int colIdx = idx.column();
    int rowIdx = idx.row();

    assert(colIdx == 1);
    assert(rowIdx == 0);

    QItemDelegate::setModelData(editor, model, idx);

    QString primerpair = model->data(idx).toString();
    primerpair = primerpair.toUpper();
    int sep = primerpair.indexOf("..");

    if(sep >= 0 ){
        QString primer_left = primerpair.left(sep);
        QString primer_right = primerpair.mid(sep+2);

        QRegExp rx("^[ACGTUacgtu]+$");

        if(!primer_left.isEmpty() && !primer_right.isEmpty() &&
                primer_left.indexOf(rx)>=0 && primer_right.indexOf(rx)>=0){

            QVector<PrimerPair*> pairList;

            PrimerPair *pair = new PrimerPair();
            pair->left = primer_left;
            pair->rightRevsComp = primer_right;
            pairList.push_back(pair);

            DataTools::buildSimCost(primerCostType);
            Designer::updataPrimerInfo(pairList);

            model->setData(model->index(rowIdx,1),primerpair.toUpper());

            primerPairList = pairList + primerPairList;
            primerPairShowList = pairList + primerPairShowList;


            // frequency for single primers
            QString val;
            val.sprintf("%.2f", pair->pctgInTarSeqsLeft);
            model->setData(model->index(0, A_LFT_TARGET_PCTG),  val);
            ((QStandardItemModel*)model)->item(rowIdx, A_LFT_TARGET_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            val.sprintf("%.2f", pair->pctgInTarSeqsRight);
            model->setData(model->index(0, A_RHT_TARGET_PCTG),  val);
            ((QStandardItemModel*)model)->item(rowIdx, A_RHT_TARGET_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            val.sprintf("%.2f", pair->pctgInNonTarSeqsLeft);
            model->setData(model->index(rowIdx, A_LFT_NONTAR_PCTG),  val);
            ((QStandardItemModel*)model)->item(rowIdx, A_LFT_NONTAR_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            val.sprintf("%.2f", pair->pctgInNonTarSeqsRight);
            model->setData(model->index(rowIdx, A_RHT_NONTAR_PCTG),  val);
            ((QStandardItemModel*)model)->item(rowIdx, A_RHT_NONTAR_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);

            // average annealing position in target sequences: left and right primer
            if( pair->bindPosTarSeqsAvgLeft < 0 )
                model->setData(model->index(rowIdx, A_LFT_TAR_BIND_POS),  "-");
            else{
                val.sprintf("%.2f", pair->bindPosTarSeqsAvgLeft);
                model->setData(model->index(rowIdx, A_LFT_TAR_BIND_POS),  val);
                ((QStandardItemModel*)model)->item(rowIdx, A_LFT_TAR_BIND_POS)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            }
            if( pair->bindPosTarSeqsAvgRight < 0 )
                model->setData(model->index(rowIdx, A_RHT_TAR_BIND_POS),  "-");
            else{
                val.sprintf("%.2f", pair->bindPosTarSeqsAvgRight);
                model->setData(model->index(rowIdx, A_RHT_TAR_BIND_POS),  val);
                ((QStandardItemModel*)model)->item(rowIdx, A_RHT_TAR_BIND_POS)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            }


            val.sprintf("%.2f", pair->pctgInTargetSeqs);
            model->setData(model->index(rowIdx, A_TARGET_PCTG),  val);
            ((QStandardItemModel*)model)->item(rowIdx, A_TARGET_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            val.sprintf("%.2f", pair->pcrSizeAvgTarSeqs);
            model->setData(model->index(rowIdx, A_TAR_PCR_AVG),  val);
            ((QStandardItemModel*)model)->item(rowIdx, A_TAR_PCR_AVG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            model->setData(model->index(rowIdx, A_TAR_PCR_MIN),  pair->pcrSizeMinTarSeqs);
            ((QStandardItemModel*)model)->item(rowIdx, A_TAR_PCR_MIN)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            model->setData(model->index(rowIdx, A_TAR_PCR_MAX),  pair->pcrSizeMaxTarSeqs);
            ((QStandardItemModel*)model)->item(rowIdx, A_TAR_PCR_MAX)->setData(Qt::AlignRight, Qt::TextAlignmentRole);

            val.sprintf("%.2f", pair->pctgInNonTarSeqs);
            model->setData(model->index(rowIdx, A_NONTAR_PCTG),  val);
            ((QStandardItemModel*)model)->item(rowIdx, A_NONTAR_PCTG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            val.sprintf("%.2f", pair->pcrSizeAvgNonTarSeqs);
            model->setData(model->index(rowIdx, A_NONTAR_PCR_AVG),  val);
            ((QStandardItemModel*)model)->item(rowIdx, A_NONTAR_PCR_AVG)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            model->setData(model->index(rowIdx, A_NONTAR_PCR_MIN),  pair->pcrSizeMinNonTarSeqs);
            ((QStandardItemModel*)model)->item(rowIdx, A_NONTAR_PCR_MIN)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            model->setData(model->index(rowIdx, A_NONTAR_PCR_MAX),  pair->pcrSizeMaxNonTarSeqs);
            ((QStandardItemModel*)model)->item(rowIdx, A_NONTAR_PCR_MAX)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            // left primer GC%
            val.sprintf("%.2f", pair->GCLeft);
            model->setData(model->index(rowIdx, A_LFT_GC),  val);
            ((QStandardItemModel*)model)->item(rowIdx, A_LFT_GC)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            // right primer GC%
            val.sprintf("%.2f", pair->GCRight);
            model->setData(model->index(rowIdx, A_RHT_GC),  val);
            ((QStandardItemModel*)model)->item(rowIdx, A_RHT_GC)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            // left primer Tm
            val.sprintf("%.2f", pair->tmLeft);
            model->setData(model->index(rowIdx, A_LFT_TM),  val);
            ((QStandardItemModel*)model)->item(rowIdx, A_LFT_TM)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            // right primer Tm
            val.sprintf("%.2f", pair->tmRight);
            model->setData(model->index(rowIdx, A_RHT_TM),  val);
            ((QStandardItemModel*)model)->item(rowIdx, A_RHT_TM)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            // inter-complementarity
            model->setData(model->index(rowIdx, A_INTER_COMP),  pair->prPrComp);
            ((QStandardItemModel*)model)->item(rowIdx, A_INTER_COMP)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            // 3' inter-complementarity
            model->setData(model->index(rowIdx, A_INTER3_COMP),  pair->prPr3Comp);
            ((QStandardItemModel*)model)->item(rowIdx, A_INTER3_COMP)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            // self-complementarity
            model->setData(model->index(rowIdx, A_LFT_INTRA_COMP),  pair->selfCompLeft);
            ((QStandardItemModel*)model)->item(rowIdx, A_LFT_INTRA_COMP)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            model->setData(model->index(rowIdx, A_RHT_INTRA_COMP),  pair->selfCompRight);
            ((QStandardItemModel*)model)->item(rowIdx, A_RHT_INTRA_COMP)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            // 3'self-complementarity
            model->setData(model->index(rowIdx, A_LFT_INTRA3_COMP),  pair->self3CompLeft);
            ((QStandardItemModel*)model)->item(rowIdx, A_LFT_INTRA3_COMP)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            model->setData(model->index(rowIdx, A_RHT_INTRA3_COMP),  pair->self3CompRight);
            ((QStandardItemModel*)model)->item(rowIdx, A_RHT_INTRA3_COMP)->setData(Qt::AlignRight, Qt::TextAlignmentRole);

            model->setData(model->index(rowIdx, A_NUM_AMBI_TARGETS_LEFT),  pair->numAmbiguousTargetsLeft);
            ((QStandardItemModel*)model)->item(rowIdx, A_NUM_AMBI_TARGETS_LEFT)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            model->setData(model->index(rowIdx, A_NUM_AMBI_TARGETS_RIGHT),  pair->numAmbiguousTargetsRight);
            ((QStandardItemModel*)model)->item(rowIdx, A_NUM_AMBI_TARGETS_RIGHT)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            model->setData(model->index(rowIdx, A_NUM_AMBI_BASES_LEFT),  pair->numAmbiguousBasesLeft);
            ((QStandardItemModel*)model)->item(rowIdx, A_NUM_AMBI_BASES_LEFT)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            model->setData(model->index(rowIdx, A_NUM_AMBI_BASES_RIGHT),  pair->numAmbiguousBasesRight);
            ((QStandardItemModel*)model)->item(rowIdx, A_NUM_AMBI_BASES_RIGHT)->setData(Qt::AlignRight, Qt::TextAlignmentRole);
            //  model->setData(model->index(rowIdx, A_NUM_TAR), tarSeqs.size());


            return;
        }
    }

    QString str = QString("Incorrect input, input must be two strings of\r\n"
                          "letters A,G,C,T separated by two periods.\r\n\r\n"
                          "For example AGCCTTAGCGAAC..CGGTAAGTTCCAGG");
    QMessageBox::about(0,"Error",str);

    // delete the selected primer pair from primer pair table
    delete ((QStandardItemModel*)model)->item(rowIdx,0);
    model->removeRow(rowIdx);
}



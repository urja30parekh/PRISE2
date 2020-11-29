#include "seqreportmodel.h"

SeqReportModel::SeqReportModel(QObject *parent) : QAbstractTableModel(parent)
{

}

QVariant SeqReportModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch(role) {

    case Qt::DisplayRole:
        //vertical
        if(orientation == Qt::Vertical)
            return section + 1;

        //horizontal
        if(section == 0) {
            return tr("Selected");
        } else if(section == 1) {
            return tr("Accession Number");
        }

        if(qList.size() == 1) {
            if(section == 2) {
                return tr("% Identity");
            } else if(section == 3) {
                return tr("Length");
            } else if(section == 4) {
                return tr("Definition");
            } else if(section == 5) {
                return tr("Organism");
            } else if(section == 6) {
                return tr("Source");
            } else if(section == 7) {
                return tr("Features");
            } else if(section == 8) {
                return tr("Authors");
            } else if(section == 9) {
                return tr("Title");
            }
        } else {
            section -= 2;
            if(section >= 0 && section < qList.size()) {
                return tr("%id: ")+qList.at(section)->definition;
            }

            section  -= qList.size();
            if(section == 0) {
                return tr("Length");
            } else if(section == 1) {
                return tr("Definition");
            } else if(section == 2) {
                return tr("Organism");
            } else if(isSimple == false) {
                return QVariant();
            } else if(section == 3) {
                return tr("Source");
            } else if(section == 4) {
                return tr("Features");
            } else if(section == 5) {
                return tr("Authors");
            } else if(section == 6) {
                return tr("Title");
            }
        }
        return QVariant();


    default:
        return QVariant();

    }
}

int SeqReportModel::rowCount(const QModelIndex &) const
{
    return sList.size();
}

int SeqReportModel::columnCount(const QModelIndex &) const
{
    //fix an edge case
    if(sList.isEmpty())
        return 5;
    //2 col at beginning, qList.size, then followed by 3 then follow by an optional 4 cols
    return 2 + qList.size() + 3 + (isSimple? 0 : 4);
}

bool SeqReportModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role != Qt::CheckStateRole) //items can only be checked/unchecked
        return false;
    if(data(index, role) != value) {
        int row = index.row();
        struct SequenceList *seqPt = sList[row];
        seqPt->marked = static_cast<int>(value.toBool());
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

QVariant SeqReportModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    int row = index.row();
    int col = index.column();
    struct SequenceList *seqPt = sList[row];
    switch(role) {
    case Qt::CheckStateRole:
        if(col==0)
            return seqPt->marked? Qt::Checked : Qt::Unchecked;
        break;

    case Qt::ToolTipRole:
    case Qt::DisplayRole:
        if(col==1) {
            return seqPt->accession;
        } else if(qList.size() == 1) {
            switch(col) {
            case 2:
                return seqPt->identity[0];
            case 3:
                return seqPt->length;
            case 4:
                return seqPt->definition;
            case 5:
                return seqPt->organism;
            case 6:
                return isSimple? QVariant() : seqPt->source;
            case 7:
                return isSimple? QVariant() : seqPt->features;
            case 8:
                return isSimple? QVariant() : seqPt->authors;
            case 9:
                return isSimple? QVariant() : seqPt->title;
            }
        } else {
            col -= 2;
            if(col >= 0 && col < qList.size()) {
                return seqPt->identity[col];
            }
            col -= qList.size();
            switch(col) {
            case 0:
                return seqPt->length;
            case 1:
                return seqPt->definition;
            case 2:
                return seqPt->organism;
            case 3:
                return isSimple? QVariant() : seqPt->source;
            case 4:
                return isSimple? QVariant() : seqPt->features;
            case 5:
                return isSimple? QVariant() : seqPt->authors;
            case 6:
                return isSimple? QVariant() : seqPt->title;
            }
        }
        break;
    }

    return QVariant();
}

Qt::ItemFlags SeqReportModel::flags(const QModelIndex &index) const {
    if(!index.isValid())
        return Qt::NoItemFlags;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
}

void SeqReportModel::sortData(int col)
{   
    //obtain the order to sort the data in, and sort ascending/descending order...
    const QHeaderView *const view = static_cast<const QHeaderView *const>(sender());
    Qt::SortOrder order = view->sortIndicatorOrder();
    QVector<struct SequenceList *>::iterator begin = sList.begin();
    QVector<struct SequenceList *>::iterator end   = sList.end();
    emit this->layoutAboutToBeChanged();

    if(col == 0) {
        if(order == Qt::AscendingOrder) {
            std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->marked < b->marked;});
        } else {
            std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->marked > b->marked;});
        }
    } else if(col == 1) {
        if(order == Qt::AscendingOrder) {
            std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->accession < b->accession;});
        } else {
            std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->accession > b->accession;});
        }
    } else if(qList.size() == 1) {
        if(col == 2) {
            if(order == Qt::AscendingOrder) {
                std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->identity[0] < b->identity[0];});
            } else {
                std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->identity[0] > b->identity[0];});
            }
        } else if(col == 3) {
            if(order == Qt::AscendingOrder) {
                std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->length < b->length;});
            } else {
                std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->length > b->length;});
            }
        } else if(col == 4) {
            if(order == Qt::AscendingOrder) {
                std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->definition < b->definition;});
            } else {
                std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->definition > b->definition;});
            }
        } else if(col == 5) {
            if(order == Qt::AscendingOrder) {
                std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->organism < b->organism;});
            } else {
                std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->organism > b->organism;});
            }
        } else if(col == 6) {
            if(order == Qt::AscendingOrder) {
                std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->source < b->source;});
            } else {
                std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->source > b->source;});
            }
        } else if(col == 7) {
            if(order == Qt::AscendingOrder ) {
                std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->features < b->features;});
            } else {
                std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->features > b->features;});
            }
        } else if(col == 8) {
            if(order == Qt::AscendingOrder) {
                std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->authors < b->authors;});
            } else {
                std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->authors > b->authors;});
            }
        } else if(col == 9) {
            if(order == Qt::AscendingOrder) {
                std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->title < b->title;});
            } else {
                std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->title > b->title;});
            }
        }
    } else {
        col -= 2;
        if(col >= 0 && col < qList.size()) {
            if(order == Qt::AscendingOrder) {
                std::sort(begin, end, [col](SequenceList *a, SequenceList *b){return a->identity[col] < b->identity[col];});
            } else {
                std::sort(begin, end, [col](SequenceList *a, SequenceList *b){return a->identity[col] > b->identity[col];});
            }
        } else {
            col -= qList.size();
            switch(col) {
            case 0:
                if(order == Qt::AscendingOrder) {
                    std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->length < b->length;});
                } else {
                    std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->length > b->length;});
                } break;
            case 1:
                if(order == Qt::AscendingOrder) {
                    std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->definition < b->definition;});
                } else {
                    std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->definition > b->definition;});
                } break;
            case 2:
                if(order == Qt::AscendingOrder) {
                    std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->organism < b->organism;});
                } else {
                    std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->organism > b->organism;});
                } break;
            case 3:
                if(order == Qt::AscendingOrder) {
                    std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->source < b->source;});
                } else {
                    std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->source > b->source;});
                } break;
            case 4:
                if(order == Qt::AscendingOrder) {
                    std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->features < b->features;});
                } else {
                    std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->features > b->features;});
                } break;
            case 5:
                if(order == Qt::AscendingOrder) {
                    std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->authors < b->authors;});
                } else {
                    std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->authors > b->authors;});
                } break;
            case 6:
                if(order == Qt::AscendingOrder) {
                    std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->title < b->title;});
                } else {
                    std::sort(begin, end, [](SequenceList *a, SequenceList *b){return a->title > b->title;});
                } break;
            }
        }
    }
    emit this->layoutChanged();
}







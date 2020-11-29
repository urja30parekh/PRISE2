#ifndef PRIMERITEMDELEGATE_H
#define PRIMERITEMDELEGATE_H

#include <QItemDelegate>
#include <QStyleOptionViewItem>
#include "Data.h"
#include "DataTools.h"

class PrimerItemDelegate : public QItemDelegate
{
     Q_OBJECT
public:
    PrimerItemDelegate(QWidget *parent = 0) : QItemDelegate(parent) {}

    void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
};

#endif // PRIMERITEMDELEGATE_H

#ifndef PROBEITEMDELEGATE_H
#define PROBEITEMDELEGATE_H

#include <QItemDelegate>
#include "Data.h"
#include "DataTools.h"

class ProbeItemDelegate : public QItemDelegate
{
     Q_OBJECT
public:
    ProbeItemDelegate(QWidget *parent = 0) : QItemDelegate(parent) {}

    void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
};

#endif // PROBEITEMDELEGATE_H

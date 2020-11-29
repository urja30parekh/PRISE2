#ifndef SEQREPORTMODEL_H
#define SEQREPORTMODEL_H
#include "Data.h"
#include "PRISESetting.h"
#include <QAbstractTableModel>
#include <QHeaderView>
#include <algorithm>

extern bool isSimple; //(DANIEL TAN)... not so simple...
extern QVector<struct QueryList*> qList;

class SeqReportModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit SeqReportModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

public slots:
    void sortData(int);
private:
};

#endif // SEQREPORTMODEL_H

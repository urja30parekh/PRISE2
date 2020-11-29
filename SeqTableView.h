#ifndef SEQTABLEVIEW_H
#define SEQTABLEVIEW_H

#include <QTableView>

class SeqTableView : public QTableView
{
    Q_OBJECT
public:
    explicit SeqTableView(QWidget *parent = 0);
    
signals:
    
public slots:

    //void update ( const QModelIndex & index );
    //void commitData ( QWidget * editor);
    //void activated ( const QModelIndex & index );
    
};

#endif // SEQTABLEVIEW_H

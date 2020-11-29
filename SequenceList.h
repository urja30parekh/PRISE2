#ifndef SEQUENCELIST_H
#define SEQUENCELIST_H
#include <QString>
#include <QVector>

struct SequenceList{
        int marked;
        //QString gi;
        QString accession;
        QVector<float> identity;
        int length;
        QString definition;
        QString organism;
        QString source;
        QString features;
        QString authors;
        QString title;
        QString seqName;
        short *seqInt;

        //Daniel:
        int start;
        int end;

        bool reverseComplement;
};

struct QueryList{
        //QString gi;
        QString definition;
        int length;
        short *seqInt;
        short *seqIntRev;
};
#endif // SEQUENCELIST_H

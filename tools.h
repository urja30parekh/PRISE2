#ifndef TOOLS_H
#define TOOLS_H
#include <QString>
#include <QMessageBox>

class tools
{
public:
    tools();
    static unsigned int base2int(QChar);
    static QChar int2base(int);
    static int int2complement(int);
    static short* seqStr2Int(QString);
    static void seqInt2Str(short* intSeq, QString& seqStr);
    static void seqInt2Str(short* intSeq, int len, QString& seq);
    static void seqInt2StrReverseComplement(short* seqInt,  QString& seqStr);

    static int parseConstraint(QString pattern);
};

#endif // TOOLS_H

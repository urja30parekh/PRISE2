#include "tools.h"

tools::tools()
{
}

QChar tools::int2base(int baseID)
{
    switch(baseID)
    {
    case 0:   return 'A';
    case 1:   return 'C';
    case 2:   return 'G';
    case 3:   return 'T';
    case 4:   return 'B';
    case 5:   return 'D';
    case 6:   return 'H';
    case 7:   return 'K';
    case 8:   return 'M';
    case 9:   return 'N';
    case 10:   return 'R';
    case 11:   return 'S';
    case 12:   return 'V';
    case 13:   return 'W';
    case 14:   return 'Y';
    case 15:   return '-';
    }
    QMessageBox::about(Q_NULLPTR,"base2int Error ", QString("Fatal error! Unkown base found\nPlease terminate program and report the error to developer!").arg(baseID));
    exit(1);
}

int tools::int2complement(int baseID)
{
   switch(baseID)
   {
    case 0: return 3;
    case 1: return 2;
    case 2: return 3;
    case 3: return 0;
   }
   return baseID;
}

unsigned int tools::base2int(QChar base)
{
    switch(base.toLatin1())
    {
    case 'A':   return 0;
    case 'C':   return 1;
    case 'G':   return 2;
    case 'T':   return 3;
    case 'B':   return 4;
    case 'D':   return 5;
    case 'H':   return 6;
    case 'K':   return 7;
    case 'M':   return 8;
    case 'N':   return 9;
    case 'R':   return 10;
    case 'S':   return 11;
    case 'V':   return 12;
    case 'W':   return 13;
    case 'Y':   return 14;
    case '-':   return 15;
    }
    QString sBase(base);
    QMessageBox::about(Q_NULLPTR,"base2int Error ", QString("Fatal error! Unkown base found: %1\nPlease terminate program and report the error to developer!").arg(base));
    return -1;
}

//start from pos 0 and end with "-1"
short* tools::seqStr2Int(QString str)
{
    int len = str.length();
    short* tmpSeq = new short[len+1];
    for(int i = 0; i < len; i++ ){
        tmpSeq[i] = base2int(str.at(i));
    }
    tmpSeq[len] = -1;

    return tmpSeq;
}

//start from position 0, end at '-1'
void tools::seqInt2Str(short* intSeq, QString& seqStr)
{
    QChar base;
    seqStr = "";

    int i = 0;
    while(intSeq[i] >= 0){
        base = int2base(intSeq[i]);
        i++;
        seqStr += base;
    }
}

//Start from position 1
void tools::seqInt2Str(short* intSeq, int len, QString& seqStr)
{
    QChar base;
    seqStr = "";

    for(int i = 1; i <= len; i++){
        base = int2base(intSeq[i]);
        seqStr += base;
    }
}



void tools::seqInt2StrReverseComplement(short* seqInt,  QString& seqStr)
{
    QChar base;
    seqStr = "";

    int i = 0;
    while( seqInt[i] >= 0 ){
        if( seqInt[i] < 4 )
            base = int2base(3-seqInt[i]);
        else
            base = int2base(seqInt[i]);
        seqStr.insert(0,base);
        i++;
    }
}


int tools::parseConstraint(QString pattern)
{
        // empty string not allowed
        if( pattern == "" )
                return -1;
        // pattern is a single term string
        if( pattern.indexOf("\"") < 0 )
                return 0;

        // parse the compound string "pattern"
        int pos;
        QString andOr;

        // get the 1st word term
        pos = pattern.indexOf("\"");
        pattern.remove(0,pos+1);
        pos = pattern.indexOf("\"");
        if( pos <= 0 ) // empty string or single quote not allowed
                return -1;
        pattern.remove(0,pos+1); // delete word term
        //pattern.trimmed(); // trim space

        while( pattern.length() > 0 ){
                // get a "and/or" term
                pos = pattern.indexOf("\"");
                // missing "and/or" term, no term followed after "and/or" term
                if( pos <= 0 )
                        return -1;
                andOr = pattern.left(pos);
                andOr.remove(' ');
                pattern.remove(0,pos+1);
                // wrong "and/or" term
                if( andOr != "and" && andOr != "or" )
                        return -1;

                // get a word term
                pos = pattern.indexOf("\"");
                if( pos <= 0 ) // empty string, single quote
                        return -1;
                pattern.remove(0,pos+1); // delete word term
                pattern = pattern.trimmed(); // trim space
        }

        return 0;
}

#include <QDateTime>
#include "Designer.h"
#include <queue>

struct PPOcc{
    int occ;
    int cloneNo;
    short* ppSeq;
    int pLen;
    float bindPosSum;
    struct PPOcc *next;

    PPOcc(short *p, int nClone, int maxLen)
    {
        ppSeq = new short[maxLen];
        occ = 1;
        cloneNo = nClone;
        next = NULL;
        int i = 0;
        while( p[i] >= 0 && i < maxLen-1 ){
            ppSeq[i] = p[i];
            i++;
        }
        ppSeq[i] = -1;
        pLen = i;
        bindPosSum = 0;
    }
    ~PPOcc() {}
    bool isSame(short *p2)
    {
        int p2Len = 0;
        while( p2[p2Len] >= 0 )	p2Len++;
        if( pLen != p2Len )	return false;
        for( int i = 0; i < pLen; i++ ){
            if( ppSeq[i] != p2[i] )
                return false;
        }
        return true;
    }
};


class ComparePairs {
public:
    bool operator()(const PrimerPair* p1, const PrimerPair* p2)
    {
        return pow(100 - p1->pctgInTargetSeqs, 2 ) + 0.5f * pow( p1->pctgInNonTarSeqsLeft, 2 ) + 0.5f * pow( p1->pctgInNonTarSeqsRight, 2 )
                        >
                pow( 100 - p2->pctgInTargetSeqs, 2 ) + 0.5f * pow( p2->pctgInNonTarSeqsLeft, 2 ) + 0.5f * pow( p2->pctgInNonTarSeqsRight, 2 );
    }
};



PrimerDesignProgressDlg* Designer::pdProgDlg;
MyBig Designer::constA;

Designer::Designer()
{
}


void Designer::getLogInfo()
{
    // prepare log file
    QTime time = QTime::currentTime();
    QString timeStr = time.toString();
    logDesign = timeStr;


    switch(task){
    case 0:{ //desing primer pairs
        logDesign += ":	Primer settings defined.\r\n\r\n";
        //logDesign += "		---------------------------List of settings---------------------------\r\n";

        logDesign += "		Primer length range:	";
        logDesign += QString("%1").arg(primerLenMin);
        logDesign += " to ";
        logDesign += QString("%1").arg(primerLenMax);

        logDesign += "\r\n		% target contain a candidate primer at least:	";
        logDesign += QString("%1").arg(primerCandidFrequency);

        logDesign += "%\r\n		Only consider top ";
        logDesign += QString("%1").arg(primerCandidTopPctg) + "% candidate primers";

        logDesign += "\r\n		PCR product size range:	";
        logDesign += QString("%1").arg(primerPcrSizeMin);
        logDesign += " to ";
        logDesign += QString("%1").arg(primerPcrSizeMax);

        /*logDesign += "\r\n		Get estimated or exact PCR product information:	";
        if( primerEstimate )
            logDesign += "estimate";
        else
            logDesign += "exact";*/

        logDesign += "\r\n		Melting temperature between:	";
        logDesign += QString("%1").arg(primerTmMin);
        logDesign += " to ";
        logDesign += QString("%1").arg(primerTmMax);

        logDesign += "\r\n		Salt concentration:	";
        logDesign += QString("%1").arg(primerNaConsent);

        logDesign += "\r\n		Primer melting temperature of primers differ no more than: ";
        logDesign += QString("%1").arg(primerTmDiff);
        logDesign += "¢J";

        logDesign += "\r\n		Primer GC% between:	";
        logDesign += QString("%1").arg(primerGCMin);
        logDesign += " to ";
        logDesign += QString("%1").arg(primerGCMax);

        logDesign += "\r\n		Max primer inter-complementarity:	";
        logDesign += QString("%1").arg(primerMaxPrPrComp);

        logDesign += "\r\n		Max primer 3' inter-complementarity:	";
        logDesign += QString("%1").arg(primerMax3PrPrComp);

        logDesign += "\r\n		Number of bases to count at 3' end(inter):	";
        logDesign += QString("%1").arg(primerInterBase3end);

        logDesign += "\r\n		Max primer intra-complementarity:	";
        logDesign += QString("%1").arg(primerMaxSelfComp);

        logDesign += "\r\n		Max primer 3' intra-complementarity:	";
        logDesign += QString("%1").arg(primerMax3SelfComp);

        logDesign += "\r\n		Number of bases to count at 3' end(intra):	";
        logDesign += QString("%1").arg(primerIntraBase3end);

        logDesign += "\r\n		Max number of insertion and deletion allowed:	";
        logDesign += QString("%1").arg(maxIndel);

        logDesign += "\r\n		Insertion and deletion cost:	";
        logDesign += QString("%1").arg(indelCost);

        logDesign += "\r\n		Positional weight cost: target sequences:\r\n			";
        for( int i = 29; i >= 0; i-- ){
                logDesign += QString("%1").arg(primerPosWtTar[i]);
                logDesign += " ";
        }
        logDesign += "\r\n		Positional weight cost: non-target sequences:\r\n			";
        for( int i = 29; i >= 0; i-- ){
                logDesign += QString("%1").arg(primerPosWtNonTar[i]);
                logDesign += " ";
        }
        logDesign += "\r\n";
        break;
    }
    case 1:{ //desige triple
        logDesign += ":	Primer-probe set settings defined.\r\n\r\n";
        //logDesign += "		---------------------------List of settings---------------------------\r\n";

        logDesign += "		Probe length range:	";
        logDesign += QString("%1").arg(probeLenMin);
        logDesign += " to ";
        logDesign += QString("%1").arg(probeLenMax);

        logDesign += "\r\n		% target contain a probe at least:	";
        logDesign += QString("%1").arg(probeCandidFrequency);

        logDesign += "%\r\n		Only consider top ";
        logDesign += QString("%1").arg(probeCandidTopPctg) + "% candidate probe;";

        /*logDesign += "\r\n		Get estimated or exact PCR product information:	";
        if( probeEstimate )
            logDesign += "estimate";
        else
            logDesign += "exact";*/

        logDesign += "\r\n		Probe Melting temperature between:	";
        logDesign += QString("%1").arg(probeTmMin);
        logDesign += " to ";
        logDesign += QString("%1").arg(probeTmMax);

        logDesign += "\r\n		Salt concentration:	";
        logDesign += QString("%1").arg(probeNaConsent);


        logDesign += "\r\n		Probe GC% between:	";
        logDesign += QString("%1").arg(probeGCMin);
        logDesign += " to ";
        logDesign += QString("%1").arg(probeGCMax);

        /*logDesign += "\r\n		Max primer inter-complementarity:	";
        logDesign += QString("%1").arg(primerMaxPrPrComp);

        logDesign += "\r\n		Max primer 3' inter-complementarity:	";
        logDesign += QString("%1").arg(primerMax3PrPrComp);

        logDesign += "\r\n		Number of bases to count at 3' end(inter):	";
        logDesign += QString("%1").arg(primerInterBase3end);*/

        logDesign += "\r\n		Max probe intra-complementarity:	";
        logDesign += QString("%1").arg(probeMaxSelfComp);

        //logDesign += "\r\n		Max primer 3' intra-complementarity:	";
        //logDesign += QString("%1").arg(primerMax3SelfComp);

        //logDesign += "\r\n		Number of bases to count at 3' end(intra):	";
        //logDesign += QString("%1").arg(probeIntraBase3end);

        logDesign += "\r\n		Max number of insertion and deletion allowed:	";
        logDesign += QString("%1").arg(maxIndel);

        logDesign += "\r\n		Insertion and deletion cost:	";
        logDesign += QString("%1").arg(indelCost);

        logDesign += "\r\n		Positional weight cost: target sequences:\r\n			";
        for( int i = 0; i < 15; i++ ){
                logDesign += QString("%1").arg(probePosWtTar[i]);
                logDesign += " ";
        }
        logDesign += "\r\n		Positional weight cost: non-target sequences:\r\n			";
        for( int i = 0; i < 15; i++ ){
                logDesign += QString("%1").arg(probePosWtNonTar[i]);
                logDesign += " ";
        }
        logDesign += "\r\n";
        break;
    }
    default:
        QMessageBox QMBox;
        QMBox.setWindowTitle("Task Error");
        QMBox.setText("Task Error. Please contact the developer");
        QMBox.exec();
        //QMessageBox::about(0, "Task Error","Task Error. Please contact the developer" );
        break;
    }
    //logDesign += "\r\n		---------------------------End list of settings---------------------------\r\n\r\n";
    logDesign += "\r\n";
}

int Designer::readSequence(QString filename,QStringList& seqNames, QVector<short*>& seqs)
{
    QStringList tmpSeqs;
    QString seq;
    short * seqInt;

    // check input file
    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox QMBox;
        QMBox.setWindowTitle("File Error");
        QMBox.setText("target/non-target sequence file doesn't exist or file corrupted");
        QMBox.exec();
        //QMessageBox::about(0, "File Error", "target/non-target sequence file doesn't exist or File corrupted");
        return -1;
    }

    seqs.clear();
    QTextStream fileStream ( &file );
    QString newStr= fileStream.readAll();
    newStr = newStr.replace(QChar(13),'\n');
    QTextStream stream (&newStr);
    QString line;

    //Check file format
    line = stream.readLine();
    if( line.at(0) != '>' ){
        QMessageBox QMBox;
        QMBox.setWindowTitle("File Error");
        QMBox.setText(filename +" is not a FASTA file.");
        QMBox.exec();
        //QMessageBox::about(0, "File Error",filename +" is not a FASTA file.");
        return -1;
    }
    if( stream.atEnd()){
        QMessageBox QMBox;
        QMBox.setWindowTitle("File Error");
        QMBox.setText("Error reading tar/nontar sequence " + filename + ".\r\nPlease double check your sequence file.");
        QMBox.exec();
        //QMessageBox::about(0, "File Error","Error reading tar/nontar sequence " + filename + ".\r\nPlease double check your sequence file." );
        seqNames.clear();
        tmpSeqs.clear();
        return false;
    }

    //Start reading sequences
    while( !stream.atEnd() ) {
        if( line == "" || line.at(0) != '>' )
            continue;
        // get a sequence name
        seqNames.append(line);
        seq.clear();

        while( !stream.atEnd() && (line = stream.readLine()) == "" ){
        }
        while(!line.startsWith('>')){
            seq += line.toUpper();
            if(stream.atEnd())
                break;
            line = stream.readLine();
        } 
        // get a sequence
        tmpSeqs.append(seq);
        if( seq == "" ){
            QMessageBox QMBox;
            QMBox.setWindowTitle("File Error");
            QMBox.setText("Error reading tar/nontar sequence " + filename + ".\r\nPlease double check your sequence file.");
            QMBox.exec();
            //QMessageBox::about(0, "File Error","Error reading tar/nontar sequence " + filename + ".\r\nPlease double check your sequence file." );
            seqNames.clear();
            tmpSeqs.clear();
            return -1;
        }
    }
    file.close();
    // finish reading

    // clean memory
    for(int i = 0; i < seqs.size(); i++ )
        delete[] seqs[i];
    seqs.clear();

    // Add target/non-target sequences to "seqs"
    for( int i = 0; i < tmpSeqs.size(); i++ ){
        seqInt = new short[((QString)tmpSeqs[i]).length()+1];

        int j = 0;
        while( j < ((QString)tmpSeqs.at(i)).size()){
            seqInt[j] = (short)tools::base2int(((QString)tmpSeqs.at(i)).at(j));
            j++;
        }
        seqInt[j] = -1;
        seqs.push_back(seqInt);
    }
    return 0;
}

int Designer::readSequence(void)
{

    QTime time = QTime::currentTime();
    QString timeStr = time.toString();
    logDesign += timeStr;
    logDesign += ":	Start reading target and non-target sequences.\r\n";
    //----------------------------read target sequence------------------


    if (readSequence(targetSeqFilename, tarSeqNames, tarSeqs) < 0)
        return -1;
    if (readSequence(nontargetSeqFilename, nontarSeqNames, nontarSeqs) < 0)
        return -1;

    //-------------------------finish reading-------------------
    QString str;
    str = QString("%1").arg(tarSeqs.size());
    logDesign += "\t\tRead " + str + " target sequences.\r\n";
    str = QString("%1").arg(nontarSeqs.size());
    logDesign += "\t\tRead " + str + " non-target sequences.\r\n\r\n";
    return 0;
}


// return 1 is s1 and s2 are same sequences
bool Designer::areSameSeqs( short *s1, short *s2 )
{
    int i = 0;
    while( s1[i] >= 0 && s2[i] >= 0 ){
        if( s1[i] != s2[i] )
            return false;
        i++;
    }
    if( s1[i] != s2[i] )
        return false;
    return true;
}

int Designer::filterSeqsOutRange(QStringList& dumpList, QStringList& seqNames, QVector<short*>& seqs, QString tarOrNontar)
{
    int len, nFilter, nSeqs;
    nSeqs = (int)seqs.size();
    nFilter = 0;

    for( int i = 0; i < nSeqs; i++ ){
        len = 0;
        while( len < seqLenMax+2 && seqs[i][len] >= 0 )
            len++;
        if( len < seqLenMin || len > seqLenMax ){ // not within range
            dumpList.append(seqNames[i]);
            QString seq;
            tools::seqInt2Str(seqs[i],seq);
            dumpList.append(seq);

            nFilter++;
            seqNames.removeAt(i);
            delete[] seqs[i];
            seqs.erase(seqs.begin()+i);
            i--; nSeqs--;
        }
    }

    QString str = "----------- ";
    str += QString("%1").arg(nFilter) + " " + tarOrNontar + " sequences were filtered out with length < ";
    str += QString("%1").arg(seqLenMin) + " or > ";
    str += QString("%1").arg(seqLenMax) + "------------\n";
    dumpList.append(str);

    if( nFilter > 0 ){
        str = QString("%1").arg(nFilter);
        logDesign += "\t\t" + str + " " + tarOrNontar + " sequences with length < ";
        str = QString("%1").arg(seqLenMin);
        logDesign += str + " or > ";
        str = QString("%1").arg(seqLenMax);
        logDesign += str + " nucleotides were filtered out";

        if( !dpLenOutRangeSeqs ) // simply delete those sequences
            logDesign += ".\r\n";
        else // dump sequences to file
            logDesign += " and dumped to file " + lenOutRangeSeqFilename + ".\r\n";
    }
    return nFilter;
}

int Designer::filterSeqSDup(QStringList& dumpList, QStringList& seqNames, QVector<short*>& seqs, QString tarOrNontar)
{
    int nFilter = 0;
    int nSeqs = (int)seqs.size();

    for( int i = 0; i < nSeqs; i++ ){
        for( int j = i+1; j < nSeqs; j++ ){
            if( areSameSeqs(seqs[j], seqs[i]) ){
                dumpList.append(seqNames[j]);
                QString seq;
                tools::seqInt2Str(seqs[j],seq);
                dumpList.append(seq);
                nFilter++;
                seqNames.removeAt(j);
                delete[] seqs[j];
                seqs.erase(seqs.begin()+j);
                j--; nSeqs--;
            }
        }
    }

    QString str = "----------- ";
    str += QString("%1").arg(nFilter) + " duplicated " + tarOrNontar + "sequences were filtered out------------\n";
    dumpList.append(str);

    if( nFilter > 0 ){
        str = QString("%1").arg(nFilter);
        logDesign += "\t\t" + str + " duplicated " + tarOrNontar +" target sequences were filtered out";
        if( !dpDupSeqs ) // simply delete those sequences
            logDesign += ".\r\n";
        else // dump sequences to file
            logDesign += " and dumped to file " + dupSeqFilename + ".\r\n";
    }
    return nFilter;
}

int Designer::filterSequence(void)
{
    int nTotal = 0;
    QStringList dumpList;

    QTime time = QTime::currentTime();
    QString timeStr = time.toString();
    logDesign += timeStr;
    logDesign += ":	Start filtering target and non-target sequences.\r\n";

    //---------------1. filter sequences not within user-specified range---------

    if( rmLenOutRangeSeqs ){
        nTotal += filterSeqsOutRange(dumpList, tarSeqNames, tarSeqs, "target");
        nTotal += filterSeqsOutRange(dumpList, nontarSeqNames, nontarSeqs, "non-target");

        if( dpLenOutRangeSeqs ){ // user choose to dump to a file
            QFile file(lenOutRangeSeqFilename);
            if (!file.open(QFile::WriteOnly)){
                QMessageBox::about(0, "File Error", "Error saving sequence list file! \n Please report the problem to the developer");
                return -1;
            }
            QTextStream out(&file);
            for( int i = 0; i < dumpList.size(); i++ )
                out << dumpList[i] + "\n";
            file.close();
        }
    }


    //---------------2. filter duplicated sequences------------------------------

    dumpList.clear();

    if( rmDupSeqs ){
        nTotal += filterSeqSDup(dumpList, tarSeqNames, tarSeqs, "target");
        nTotal += filterSeqSDup(dumpList, nontarSeqNames, nontarSeqs, "non-target");

        if( dpDupSeqs ){ // user choose to dump to a file
            QFile file(dupSeqFilename);
            if (!file.open(QFile::WriteOnly)){
                QMessageBox::about(0, "File Error", "Cannot open file "+dupSeqFilename+" to dump sequences");
                return -1;
            }
            QTextStream out(&file);
            for( int i = 0; i < dumpList.size(); i++ )
                out << dumpList[i] + "\n";
            file.close();
        }
    }

    if( nTotal == 0 )
        logDesign += "\t\tNo sequence was filtered out.\r\n";

    //-------------------------finish filtering----------------------------------
    return 0;
}

int Designer::readPrimer(QString primer, QVector <short*>& primerCandid, QVector <float>& primerBindPosAvg, int leftOrRight) //1: left, 2:right
{
    short *candid;
    // check primer
    if( primer.length() <= 0 ){
        QMessageBox::about(0, "File Error", "File " + primerFilename + " is not a primer candidate file.\r\n"
                           + "Error format of primer pair candidates.\r\n"
                           + "Correct format:\"(5'-)forward primer(-3')..(5'-)reverse primer(-3\')" );
        return -1;
    }
    if( primer.length() >= MAX_PRIMER_LEN ){
        QMessageBox::about(0, "Length Error", "Primer cannot exceed 40 nucleotide bases.");
        return -1;
    }
    // check if primer is already in the primer list
    bool isInList = false;
    for( int i = 0; i <primerCandid.size(); i++ ){
        QString tmpStr;
        tools::seqInt2Str( primerCandid[i],tmpStr ) ;
        if( primer == tmpStr ){
            isInList = true;
            break;
        }
    }

    if( !isInList ){
        candid = new short[primer.length()+1];

        if(leftOrRight == true){ //left primer
            for( int i = 0; i < primer.length(); i++ ){
                int j = tools::base2int(primer[i]);
                if (j == -1){
                    QMessageBox::about(0, "File Error", "Primer candidate file " + primerFilename + " contains unknow base in " + primer + ".\r\n");
                    return -1;
                }
                candid[i] = j;
            }
            primerBindPosAvg.push_back(0.0);
        }
        else{ //right primer
            for( int i = 0, k = primer.length() - 1; i < primer.length(); i++, k-- ){
                int j = tools::base2int(primer[k]);
                if (j == -1){
                    QMessageBox::about(0, "File Error", "Primer candidate file " + primerFilename + " contains unknow base in " + primer + ".\r\n");
                    return -1;
                }
                if( j < 4)
                    candid[i] = 3-j;
                else
                    candid[i] = j;
            }
            primerBindPosAvg.push_back(100000.0);
        }
        candid[primer.length()] = -1;
        primerCandid.push_back(candid);
    }
}

int Designer::readPrimer(void)
{
    QTime time = QTime::currentTime();
    QString timeStr = time.toString();
    logDesign += timeStr;
    logDesign += ":	Start reading primer candidates.\r\n";

    DataTools::cleanMemoryPrimerCands();
    QFile file(primerFilename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::about(0, "File Error", "Cannot open primer candidate file " + primerFilename + "!");
        return -1;
    }
    QTextStream stream ( &file );
    QString line, left, right;
    int nPos;

    while( !stream.atEnd() ) {
        line  = stream.readLine();
        if(line.length() == 0)
            continue;

        nPos = line.indexOf("..");
        if( nPos < 0 ){
            QMessageBox::about(0, "File Error", "File " + primerFilename + " is not a primer candidate file.\r\n"
                               + "Error format of primer pair candidates.\r\n"
                               + "Correct format:\"(5'-)forward primer(-3')..(5'-)reverse primer(-3\')" );
            return -1;
        }
        // get  primer.
        line.toUpper();
        left = line.left(nPos);
        readPrimer(left, lPrimerCandid, lPrimerBindPosAvg, true);
        right = line.remove(0,nPos+2);
        readPrimer(right, rPrimerCandid, rPrimerBindPosAvg, false);
    }
    file.close();
    //-------------------------finish reading-------------------
    QString str = QString("%1").arg(lPrimerCandid.size());
    logDesign += "\t\tFinish reading primer pair candidates.\r\n";
    return 0;
}

int Designer::readProbe(void)
{/*
    QTime time = QTime::currentTime();
    QString timeStr = time.toString();
    logDesign += timeStr;
    logDesign += ":	Start reading primer candidates.\r\n";

    DataTools::cleanPrimer();
    QFile file(primerFilename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::about(0, "File Error", "Cannot open primer candidate file " + primerFilename + "!");
        return -1;
    }
    QTextStream stream ( &file );
    QString line, left, right;
    int nPos;

    while( !stream.atEnd() ) {
        line  = stream.readLine();
        if(line.length() == 0)
            continue;

        nPos = line.indexOf("..");
        if( nPos < 0 ){
            QMessageBox::about(0, "File Error", "File " + primerFilename + " is not a primer candidate file.\r\n"
                               + "Error format of primer pair candidates.\r\n"
                               + "Correct format:\"(5'-)forward primer(-3')..(5'-)reverse primer(-3\')" );
            return -1;
        }
        // get  primer.
        line.toUpper();
        left = line.left(nPos);
        readPrimer(left, lPrimerCandid, lPrimerBindPosAvg, true);
        right = line.remove(0,nPos+2);
        readPrimer(right, rPrimerCandid, rPrimerBindPosAvg, false);
    }
    file.close();
    //-------------------------finish reading-------------------
    QString str = QString("%1").arg(lPrimerCandid.size());
    logDesign += "\t\tFinish reading primer pair candidates.\r\n";
    return 0;*/
    return 0;
}


// copy target sequences into sequences
// and delete all ambiguous bases in target sequences
int Designer::copySeqs(QVector <short*>& copyTo ,QVector<short*> from )
{
    copyTo.clear();
    int len;
    short* seqPtr;
    for( int i = 0; i < from.size(); i++ ){
        len = 0;
        while( from[i][len] >= 0 )
            len++;
        seqPtr = new short[len+1];
        int k = 0;
        for( int j = 0; j < len; j++ ){
            if( from[i][j] < 4 )
                seqPtr[k++] = from[i][j];
        }
        seqPtr[k] = -1;
        copyTo.push_back(seqPtr);
    }
    return 0;
}

int Designer::pp2int(MyBig& pCode, short *myPP)
{
    pCode = 0;
    int i = 0;
    while( myPP[i] >= 0 ){
        pCode.Mul(MAX_ALPHABET_SIZE);// pCode *= MAX_ALPHABET_SIZE;
        pCode.Add(myPP[i]);// pCode += myPrimer[i];
        i++;
    }
    return 0;
}

int Designer::ppHash(MyBig key)
{
    int pIndex;

    MyBig kA = 0, kA_floor = 0, kAmod1 = 0, mXkAmod1 = 0;

    kA = key;
    kA.Mul(constA);

    kA_floor =  ttmath::Floor(kA);

    kAmod1 =kA;
    kAmod1.Sub(kA_floor);

    mXkAmod1 = kAmod1;
    mXkAmod1.Mul(HASH_OCC_SIZE);

    double tmp;
    mXkAmod1.ToDouble(tmp);
    pIndex = (int) tmp;

    if( pIndex > HASH_OCC_SIZE ){
        QMessageBox::about(0, "Fatal Error", "Fatal error: primer candidate searching error.\nProgram is forced to terminate.\n"
                           "Please report it to developer.");
        return FATAL_ERROR;
    }
    else    return pIndex;
}

int Designer::pp2int_quick(MyBig& pCode, int head, int tail, int pLength)
{
    MyBig n = 0;
    MyBig four = MAX_ALPHABET_SIZE;

    //mpf_pow_ui(n, four, pLength-1);
    n = four;
    n.Pow(pLength-1);

    MyBig temp = 0;

    //pCode = (pCode - head*pow_array[pLength-1]) * MAX_ALPHABET_SIZE + tail;
    //mpf_mul_ui(temp, n, head);
    temp = n;
    temp.Mul(head);
    //mpf_sub(temp, pCode, temp);
    temp.Sub(pCode);
    temp = -temp;
    //mpf_mul_ui(pCode, temp, MAX_ALPHABET_SIZE);
    pCode = temp;
    pCode.Mul(MAX_ALPHABET_SIZE);
    pCode.Add(tail);

    return 0;
}

int Designer::pp2int_quick(MyBig& pCode, int tail)
{
    pCode.Mul(MAX_ALPHABET_SIZE);
    pCode.Add(tail);

    return 0;
}

//Old structure. need to be refine
/*int PrimerDesigner::choosePrimerCandid(void)
{
    logPrimerDesign += "\t\tThere are " + QString("%d").arg(tarSeqs.size()) + " target and ";
    logPrimerDesign += QString("%d").arg(nontarSeqs.size()) + " non-target sequences.\r\n\r\n";
    DataTools::cleanPrimer();

    QVector <short*> seqs;
    this->copySeqs(seqs, tarSeqs);

    struct PrimerOcc	**hash_occ_array;
    try{
         hash_occ_array = new PrimerOcc*[HASH_OCC_SIZE];
    }    
    catch ( std::bad_alloc& e ) {
        QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for hashing.\nProgram is forced to terminate.");
        return FATAL_ERROR;
    }

    short *clone;			// pointer to clone string
    short *primer;		// pointer to primer string
    int clone_ith_base; // starting position to chop the primer
    short pattern[MAX_PRIMER_LEN];   	// primer

    MyBig pCode;

    int pHead, pTail;
    int pIndex;
    struct PrimerOcc *primerPtr;
    struct PrimerOcc *pLeftPtr;
    float occRate;
    short *lCandid;
    short *rCandid;
    QVector<int> lCandidOcc;
    QVector<int> rCandidOcc;
    int candidTopNum;

    for( int i = 0; i < HASH_OCC_SIZE; i++)		hash_occ_array[i] = NULL;
    int range = seqs.size() * (primerLenMax-primerLenMin+1);
    pdProgDlg->setRange(1, range);

    // pick primers for all string with length between min length and max length
    for( int primerLen = primerLenMin; primerLen <= primerLenMax; primerLen++ ){
        // hash_occ_array->occ: a primer occurs in how many clones
        for( unsigned int i = 0; i < seqs.size(); i++){//repeat in each clone
            clone = seqs[i];

            // the first primer in clone i
            primer = pattern;
            // copy primer
            int m = 0;
            while( m < primerLen ){
                if( clone[m] < 0 )	break;
                primer[m] = clone[m];
                m++;
            }
            if( m < primerLen )	break;
            primer[primerLen] = -1;

            primer2int(pCode, primer);
            pHead = primer[0];
            pIndex = hashPrimer(pCode);

            primerPtr = hash_occ_array[pIndex];
            // search for the corresponding primer on the list
            while( primerPtr != NULL ){
                // find the corresponding primer
                if( primerPtr->isSamePrimer(primer) ){
                    primerPtr->occ++;
                    // mark this primer is visited by clone i
                    primerPtr->cloneNo = i;
                    break;
                }
                pLeftPtr = primerPtr;
                primerPtr = primerPtr->next;
            }
            // reach the end of list
            if( primerPtr == NULL ){
                primerPtr = new PrimerOcc(primer, i);
                if( hash_occ_array[pIndex] == NULL )
                    hash_occ_array[pIndex] = primerPtr;
                else
                    pLeftPtr->next = primerPtr;
            }

            clone_ith_base = 1; // record current position in clone to chop primer
            clone++;			// move window to right by one
            while( clone[primerLen-1] != -1 ){	//get all candidate primers in one clone
                pTail = clone[primerLen-1];
                primer2int_quick(pCode, pHead, pTail, primerLen);
                for( m = 0; m < primerLen; m++ )	primer[m] = clone[m];
                primer[primerLen] = -1;
                pIndex = hashPrimer(pCode);

                primerPtr = hash_occ_array[pIndex];
                // search for the corresponding primer on the list
                while( primerPtr != NULL ){
                    // find the corresponding primer
                    if( primerPtr->isSamePrimer(primer) ){
                        // first time for this primer is visited by clone i
                        if( primerPtr->cloneNo != i ){
                            primerPtr->occ++;
                            // mark this primer is visited by clone i
                            primerPtr->cloneNo = i;
                            // update primer annealing position information
                            primerPtr->bindPosSum += clone_ith_base;
                        }
                        break;
                    }
                    pLeftPtr = primerPtr;
                    primerPtr = primerPtr->next;
                }
                // reach the end of list
                if( primerPtr == NULL ){
                    primerPtr = new PrimerOcc(primer, i);
                    if( hash_occ_array[pIndex] == NULL )
                        hash_occ_array[pIndex] = primerPtr;
                    else
                        pLeftPtr->next = primerPtr;
                }
                pHead = clone[0];
                clone_ith_base++; // record current position in clone to chop primer
                clone++;			// move window to right by one
            }
            pdProgDlg->setValue(i + seqs.size() * (primerLen-primerLenMin));


            // user abort

            if (pdProgDlg->wasCanceled()){
                // clean memory
                for( unsigned int ii = 0; ii < seqs.size(); ii++ )
                    delete[] seqs[ii];
                seqs.clear();
                for( int ii = 0; ii < HASH_OCC_SIZE; ii++ ){
                    primerPtr = hash_occ_array[ii];
                    while( primerPtr != NULL ){
                        pLeftPtr = primerPtr;
                        primerPtr = primerPtr->next;
                        delete pLeftPtr;
                    }
                    hash_occ_array[ii] = NULL;
                }
                delete[] hash_occ_array;
                return -1;
            }
        }


        // record primer candidates and clean hash memory
        for( int i = 0; i < HASH_OCC_SIZE; i++ ){
            primerPtr = hash_occ_array[i];
            while( primerPtr != NULL ){
                occRate = primerPtr->occ * 100 / (float)seqs.size();
                if( occRate >= candidFrequency ){
                    // add primer candidates to theApp
                    try{
                        lCandid = new short[primerPtr->pLen+1];
                    }
                    catch( std::bad_alloc& e ){
                        QMessageBox::about(0, "Memory Error", "Fatal error: out of memory for primer candidates.\nProgram is forced to terminate.");
                        return FATAL_ERROR;
                    }
                    try{
                            rCandid = new short[primerPtr->pLen+1];
                    }
                    catch( std::bad_alloc& e ){
                        QMessageBox::about(0, "Memory Error", "Fatal error: out of memory for primer candidates.\nProgram is forced to terminate.");
                        return FATAL_ERROR;
                    }
                    for( int j = 0; j < primerPtr->pLen; j++ ){
                        lCandid[j] = primerPtr->primer[j];
                        rCandid[j] = primerPtr->primer[j];
                    }
                    lCandid[primerPtr->pLen] = -1;
                    rCandid[primerPtr->pLen] = -1;
                    lPrimerCandid.push_back(lCandid);
                    rPrimerCandid.push_back(rCandid);
                    lCandidOcc.push_back(primerPtr->occ);
                    rCandidOcc.push_back(primerPtr->occ);
                    lPrimerBindPosAvg.push_back( primerPtr->bindPosSum/primerPtr->occ );
                    rPrimerBindPosAvg.push_back( primerPtr->bindPosSum/primerPtr->occ );
                }
                pLeftPtr = primerPtr;
                primerPtr = primerPtr->next;
                delete pLeftPtr;
            }
            hash_occ_array[i] = NULL;

            //user abort
            if (pdProgDlg->wasCanceled()){
                // clean memory
                for( unsigned int ii = 0; ii < seqs.size(); ii++ )
                    delete[] seqs[ii];
                seqs.clear();
                delete[] hash_occ_array;
                lPrimerBindPosAvg.clear();
                rPrimerBindPosAvg.clear();
                return -1;
            }
        }
    }

    QTime time = QTime::currentTime();
    QString timeStr = time.toString();
    logPrimerDesign += timeStr;
    logPrimerDesign += ":	Found " + QString("%d").arg(lPrimerCandid.size()) + " (single) primer candidates with frequency ";
    logPrimerDesign += QString("%/1f").arg(candidFrequency) + "% in target sequences.\r\n";

    // user wants to consider less primer candidates
    if( candidTopPctg < 100.0f ){
        // sort left primer candidates with descending order of primer occ
        short* tPrimer;
        int tOcc;
        float tPos;
        for( unsigned i = 1; i < lPrimerCandid.size(); i++ ){
            for( unsigned j = 0; j < i; j++){
                if( lCandidOcc[i] > lCandidOcc[j] ){
                    tPrimer = lPrimerCandid[i];
                    lPrimerCandid[i] = lPrimerCandid[j];
                    lPrimerCandid[j] = tPrimer;
                    tPos = lPrimerBindPosAvg[i];
                    lPrimerBindPosAvg[i] = lPrimerBindPosAvg[j];
                    lPrimerBindPosAvg[j] = tPos;
                    tOcc = lCandidOcc[i];
                    lCandidOcc[i] = lCandidOcc[j];
                    lCandidOcc[j] = tOcc;
                }
            }
        }
        // delete bottom list candidate primers
        candidTopNum = (int) (candidTopPctg / 100 * lPrimerCandid.size());
        while( (int)lPrimerCandid.size() > candidTopNum ){
            delete[] lPrimerCandid[lPrimerCandid.size()-1];
            lPrimerCandid.erase( lPrimerCandid.end()-1 );
            lPrimerBindPosAvg.erase( lPrimerBindPosAvg.end()-1 );
        }

        QTime time = QTime::currentTime();
        QString timeStr = time.toString();
        logPrimerDesign += timeStr;
        logPrimerDesign += ":	Only consider top " + QString(lPrimerCandid.size()) + " (single) primer candidates.\r\n";
    }
    if( candidTopPctg < 100.0f ){
        // sort right primer candidates with descending order of primer occ
        short* tPrimer;
        int tOcc;
        float tPos;
        for( unsigned i = 1; i < rPrimerCandid.size(); i++ ){
            for( unsigned j = 0; j < i; j++){
                if( rCandidOcc[i] > rCandidOcc[j] ){
                    tPrimer = rPrimerCandid[i];
                    rPrimerCandid[i] = rPrimerCandid[j];
                    rPrimerCandid[j] = tPrimer;
                    tPos = rPrimerBindPosAvg[i];
                    rPrimerBindPosAvg[i] = rPrimerBindPosAvg[j];
                    rPrimerBindPosAvg[j] = tPos;
                    tOcc = rCandidOcc[i];
                    rCandidOcc[i] = rCandidOcc[j];
                    rCandidOcc[j] = tOcc;
                }
            }
        }
        // delete bottom list candidate primers
        candidTopNum = (int) (candidTopPctg / 100 * rPrimerCandid.size());
        while( (int)rPrimerCandid.size() > candidTopNum ){
            delete[] rPrimerCandid[rPrimerCandid.size()-1];
            rPrimerCandid.erase( rPrimerCandid.end()-1 );
            rPrimerBindPosAvg.erase( rPrimerBindPosAvg.end()-1 );
        }
    }
    pdProgDlg->setValue(0);

    // clean memory
    for( int i = 0; i < seqs.size(); i++ )
        delete[] seqs[i];
    seqs.clear();

    delete[] hash_occ_array;  // the content of this occ array has been cleaned previously in "record primer candidates" part
    return 0;
}
*/

int Designer::choosePrimerCandid(void)
{
    QTime time;
    QString timeStr;

    logDesign += "\t\tThere are " + QString("%1").arg(tarSeqs.size()) + " target and ";
    logDesign += QString("%1").arg(nontarSeqs.size()) + " non-target sequences.\r\n\r\n";

    // clean memory
    for(int i = 0; i < lPrimerCandid.size(); i++ )
        delete[] lPrimerCandid[i];
    lPrimerCandid.clear();
    for(int i = 0; i < rPrimerCandid.size(); i++ )
        delete[] rPrimerCandid[i];
    rPrimerCandid.clear();
    lPrimerBindPosAvg.clear();
    rPrimerBindPosAvg.clear();


    // copy target sequences into sequences
    // and delete all ambiguous bases in target sequences
    QVector <short*> sequences;
    short * seqptr;
    int len;
    for(int i = 0; i < tarSeqs.size(); i++ ){
        len = 0;
        while( tarSeqs[i][len] >= 0 )	len++;
        seqptr = new short[len+1];
        int k = 0;
        for( int j = 0; j < len; j++ ){
            //if( tarSeqs[i][j] < 4 )
            seqptr[k++] = tarSeqs[i][j];
        }
        seqptr[k] = -1;
        sequences.push_back(seqptr);
    }


    struct PPOcc	**hash_occ_array;
    try{
         hash_occ_array = new PPOcc*[HASH_OCC_SIZE];
    }
    catch( std::bad_alloc& e  ){
        QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for hashing.\nProgram is forced to terminate.");
        return FATAL_ERROR;
    }
    short *clone;			// pointer to clone string
    short *primer;		// pointer to primer string
    int clone_ith_base; // starting position to chop the primer
    short 	pattern[MAX_PRIMER_LEN];   	// primer
    MyBig pCode = 0;
    int pHead, pTail;
    int pIndex;
    struct PPOcc *primerPtr;
    struct PPOcc *pLeftPtr;
    float occRate;
    short *lCandid;
    short *rCandid;
    QVector<int> lCandidOcc;
    QVector<int> rCandidOcc;
    int candidTopNum;

    for( int i = 0; i < HASH_OCC_SIZE; i++)		hash_occ_array[i] = NULL;
    int range = sequences.size() * (primerLenMax-primerLenMin+1);
    Designer::pdProgDlg->setRange(0, range);

    // pick primers for all string with length between min length and max length
    for( int primerLen = primerLenMin; primerLen <= primerLenMax; primerLen++ ){
        // hash_occ_array->occ: a primer occurs in how many clones
        for( int i = 0; i < sequences.size(); i++){		//repeat in each clone
            clone = sequences[i];

            // the first primer in clone i
            primer = pattern;
            // copy primer
            int m = 0;
            while( m < primerLen ){
                if( clone[m] < 0 || clone[m] > 3 )	break;
                primer[m] = clone[m];
                m++;
            }
            if( m < primerLen )	break;
            primer[primerLen] = -1;

            pp2int(pCode, primer);
            pHead = primer[0];
            pIndex = ppHash(pCode);

            primerPtr = hash_occ_array[pIndex];
            // search for the corresponding primer on the list
            while( primerPtr != NULL ){
                // find the corresponding primer
                if( primerPtr->isSame(primer) ){
                    primerPtr->occ++;
                    // mark this primer is visited by clone i
                    primerPtr->cloneNo = i;
                    break;
                }
                pLeftPtr = primerPtr; //move to next
                primerPtr = primerPtr->next;
            }

            // reach the end of list - create new primer in the list.
            if( primerPtr == NULL ){
                primerPtr = new PPOcc(primer, i, MAX_PRIMER_LEN);
                if( hash_occ_array[pIndex] == NULL )//originally this index is empty - create the first node
                    hash_occ_array[pIndex] = primerPtr;
                else  // else add to the end of list
                     pLeftPtr->next = primerPtr;
            }

            clone_ith_base = 1; // record current position in clone to chop primer
            clone++;			// move window to right by one


            while( clone[primerLen-1] != -1 ){	//get all candidate primers in one clone
                pTail = clone[primerLen-1];
                pp2int_quick(pCode, pHead, pTail, primerLen);

                bool valid = true;
                for( m = 0; m < primerLen; m++ )
                {
                    if(clone[m] > 3){
                        valid = false;
                        break;
                    }
                    primer[m] = clone[m];
                }


                if( ! valid){
                    pHead = clone[0];
                    clone_ith_base++; // record current position in clone to chop primer
                    clone++;
                }

                primer[primerLen] = -1;

                pIndex = ppHash(pCode);

                primerPtr = hash_occ_array[pIndex];
                // search for the corresponding primer on the list
                while( primerPtr != NULL ){
                    // find the corresponding primer
                    if( primerPtr->isSame(primer) ){
                        // first time for this primer is visited by clone i
                        if( primerPtr->cloneNo != i ){
                            primerPtr->occ++;
                            // mark this primer is visited by clone i
                            primerPtr->cloneNo = i;
                            // update primer annealing position information
                            primerPtr->bindPosSum += clone_ith_base;
                        }
                        break;
                    }
                    pLeftPtr = primerPtr;
                    primerPtr = primerPtr->next;
                }
                // reach the end of list
                if( primerPtr == NULL ){
                    primerPtr = new PPOcc(primer, i, MAX_PRIMER_LEN);
                    if( hash_occ_array[pIndex] == NULL )
                         hash_occ_array[pIndex] = primerPtr;
                    else
                         pLeftPtr->next = primerPtr;
                }

                pHead = clone[0];
                clone_ith_base++; // record current position in clone to chop primer
                clone++;			// move window to right by one
            }
            pdProgDlg->setValue(1+ i + sequences.size() * (primerLen-primerLenMin));
            // user abort
            if(pdProgDlg->wasCanceled()){
                // clean memory
                for( int ii = 0; ii < sequences.size(); ii++ )
                        delete[] sequences[ii];
                sequences.clear();
                for( int ii = 0; ii < HASH_OCC_SIZE; ii++ ){
                        primerPtr = hash_occ_array[ii];
                        while( primerPtr != NULL ){
                                pLeftPtr = primerPtr;
                                primerPtr = primerPtr->next;
                                delete pLeftPtr;
                        }
                        hash_occ_array[ii] = NULL;
                }
                delete[] hash_occ_array;
                return -1;
            }
        }


        // record primer candidates and clean hash memory
        for( int i = 0; i < HASH_OCC_SIZE; i++ ){
            primerPtr = hash_occ_array[i];
            while( primerPtr != NULL ){
                occRate = primerPtr->occ * 100 / (float)sequences.size();
                if( occRate >= primerCandidFrequency ){
                    // add primer candidates to theApp
                    try{
                        lCandid = new short[primerPtr->pLen+1];
                    }catch( std::bad_alloc& e ){
                        QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer candidates.\nProgram is forced to terminate.");
                        return FATAL_ERROR;
                    }
                    try{
                            rCandid = new short[primerPtr->pLen+1];
                    }catch( std::bad_alloc& e ){
                        QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer candidates.\nProgram is forced to terminate.");
                        return FATAL_ERROR;
                    }
                    for( int j = 0; j < primerPtr->pLen; j++ ){
                        lCandid[j] = primerPtr->ppSeq[j];
                        rCandid[j] = primerPtr->ppSeq[j];
                    }
                    lCandid[primerPtr->pLen] = -1;
                    rCandid[primerPtr->pLen] = -1;
                    lPrimerCandid.push_back(lCandid);
                    rPrimerCandid.push_back(rCandid);
                    lCandidOcc.push_back(primerPtr->occ);
                    rCandidOcc.push_back(primerPtr->occ);
                    lPrimerBindPosAvg.push_back( primerPtr->bindPosSum/primerPtr->occ );
                    rPrimerBindPosAvg.push_back( primerPtr->bindPosSum/primerPtr->occ );
                }
                pLeftPtr = primerPtr;
                primerPtr = primerPtr->next;
                delete pLeftPtr;
            }
            hash_occ_array[i] = NULL;

            //user abort
            if(pdProgDlg->wasCanceled()){
                // clean memory
                for( int ii = 0; ii < sequences.size(); ii++ )
                    delete[] sequences[ii];
                sequences.clear();
                delete[] hash_occ_array;
                lPrimerBindPosAvg.clear();
                rPrimerBindPosAvg.clear();
                return -1;
            }
        }
    }
    time = QTime::currentTime();
    timeStr = time.toString();
    logDesign += timeStr;
    logDesign += ":	Found " + QString("%1").arg(lPrimerCandid.size()) + " (single) primer candidates with frequency";
    logDesign += QString("%1").arg(primerCandidFrequency) + "% in target sequences.\r\n\r\n";

    // user wants to consider less primer candidates
    if( primerCandidTopPctg < 100.0f ){
        // sort left primer candidates with descending order of primer occ
        short* tPrimer;
        int tOcc;
        float tPos;
        for( int i = 1; i < lPrimerCandid.size(); i++ ){
            for( int j = 0; j < i; j++){
                if( lCandidOcc[i] > lCandidOcc[j] ){
                    tPrimer = lPrimerCandid[i];
                    lPrimerCandid[i] = lPrimerCandid[j];
                    lPrimerCandid[j] = tPrimer;
                    tPos = lPrimerBindPosAvg[i];
                    lPrimerBindPosAvg[i] = lPrimerBindPosAvg[j];
                    lPrimerBindPosAvg[j] = tPos;
                    tOcc = lCandidOcc[i];
                    lCandidOcc[i] = lCandidOcc[j];
                    lCandidOcc[j] = tOcc;
                }
            }
        }
        // delete bottom list candidate primers
        candidTopNum = (int) (primerCandidTopPctg / 100 * lPrimerCandid.size());
        while( (int)lPrimerCandid.size() > candidTopNum ){
            delete[] lPrimerCandid[lPrimerCandid.size()-1];
            lPrimerCandid.erase( lPrimerCandid.end()-1 );
            lPrimerBindPosAvg.erase( lPrimerBindPosAvg.end()-1 );
        }

        // sort right primer candidates with descending order of primer occ
        for( int i = 1; i < rPrimerCandid.size(); i++ ){
            for( int j = 0; j < i; j++){
                if( rCandidOcc[i] > rCandidOcc[j] ){
                    tPrimer = rPrimerCandid[i];
                    rPrimerCandid[i] = rPrimerCandid[j];
                    rPrimerCandid[j] = tPrimer;
                    tPos = rPrimerBindPosAvg[i];
                    rPrimerBindPosAvg[i] = rPrimerBindPosAvg[j];
                    rPrimerBindPosAvg[j] = tPos;
                    tOcc = rCandidOcc[i];
                    rCandidOcc[i] = rCandidOcc[j];
                    rCandidOcc[j] = tOcc;
                }
            }
        }
        // delete bottom list candidate primers
        candidTopNum = (int) (primerCandidTopPctg / 100 * rPrimerCandid.size());
        while( (int)rPrimerCandid.size() > candidTopNum ){
            delete[] rPrimerCandid[rPrimerCandid.size()-1];
            rPrimerCandid.erase( rPrimerCandid.end()-1 );
            rPrimerBindPosAvg.erase( rPrimerBindPosAvg.end()-1 );
        }

        time = QTime::currentTime();
        timeStr = time.toString();
        logDesign += timeStr;
        logDesign += ":	Only consider top " + QString("%1").arg(lPrimerCandid.size()) + " (single) primer candidates.\r\n";
    }

    pdProgDlg->setValue(0);
    //pdProgDlg->update();
    // clean memory
    for( int i = 0; i < sequences.size(); i++ )
        delete[] sequences[i];
    sequences.clear();
    delete[] hash_occ_array;  // the content of this occ array has been cleaned previously in "record primer candidates" part
    return 0;
}

int Designer::chooseProbeCandid()
{
    QTime time;
    QString timeStr;

    logDesign += "\t\tThere are " + QString("%1").arg(tarSeqs.size()) + " target and ";
    logDesign += QString("%1").arg(nontarSeqs.size()) + " non-target sequences.\r\n\r\n";

    // clean memory
    for( int i = 0; i < probeCandid.size(); i++ )
        delete[] probeCandid[i];

    probeCandid.clear();
    probeBindPosAvg.clear();

    // copy target sequences into sequences
    // and delete all ambiguous bases in target sequences
    QVector <short*> sequences;
    short * seqptr;
    int len;
    for( int i = 0; i < tarSeqs.size(); i++ ){
        len = 0;
        while( tarSeqs[i][len] >= 0 )	len++;
        seqptr = new short[len+1];
        int k = 0;
        for( int j = 0; j < len; j++ ){
            if( tarSeqs[i][j] < 4 )
                seqptr[k++] = tarSeqs[i][j];
        }
        seqptr[k] = -1;
        sequences.push_back(seqptr);
    }


    struct PPOcc	**hash_occ_array;
    try{
         hash_occ_array = new PPOcc*[HASH_OCC_SIZE];
    }
    catch( std::bad_alloc& e  ){
        QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for hashing.\nProgram is forced to terminate.");
        return FATAL_ERROR;
    }
    short *clone;   // pointer to clone string
    short *probe;   // pointer to probe string
    int clone_ith_base; // starting position to chop the probe
    short pattern[MAX_PROBE_LEN];   	// probe
    MyBig pCode = 0;
    int pHead, pTail;
    int pIndex;
    struct PPOcc *probePtr;
    struct PPOcc *pLeftPtr;
    float occRate;
    short *candid;
    QVector<int> candidOcc;
    int candidTopNum;

    for( int i = 0; i < HASH_OCC_SIZE; i++)
        hash_occ_array[i] = NULL;
    int range = sequences.size() * (probeLenMax-probeLenMin+1);
    Designer::pdProgDlg->setRange(0, range);

    // pick probes for all string with length between min length and max length
    for( int probeLen = probeLenMin; probeLen <= probeLenMax; probeLen++ ){
        // hash_occ_array->occ: a probe occurs in how many clones
        for( int i = 0; i < sequences.size(); i++){ //repeat in each clone
            clone = sequences[i];

            // the first probe in clone i
            probe = pattern;
            // copy probe
            int m = 0;
            while( m < probeLen ){
                if( clone[m] < 0 )	break;
                probe[m] = clone[m];
                m++;
            }
            if( m < probeLen )	break;
            probe[probeLen] = -1;

            pp2int(pCode, probe);
            pHead = probe[0];
            pIndex = ppHash(pCode);

            probePtr = hash_occ_array[pIndex];
            // search for the corresponding probe on the list
            while( probePtr != NULL ){// find the corresponding probe

                if( probePtr->isSame(probe) ){
                    probePtr->occ++;  // This probe is visited by current sequence
                    probePtr->cloneNo = i;;
                    break;
                }
                pLeftPtr = probePtr;
                probePtr = probePtr->next;
            }

            if( probePtr == NULL ){  // First occurs at current sequence
                probePtr = new PPOcc(probe, i, MAX_PROBE_LEN);

                if( hash_occ_array[pIndex] == NULL )
                    hash_occ_array[pIndex] = probePtr;
                else
                     pLeftPtr->next = probePtr;
            }

            clone_ith_base = 1; // record current position in clone to chop probe
            clone++;		// move window to right by one
            while( clone[probeLen-1] != -1 ){	//get all candidate probes in one clone
                pTail = clone[probeLen-1];
                pp2int_quick(pCode, pHead, pTail, probeLen);
                for( m = 0; m < probeLen; m++ )	probe[m] = clone[m];
                probe[probeLen] = -1;

                pIndex = ppHash(pCode);

                probePtr = hash_occ_array[pIndex];
                // search for the corresponding probe on the list
                while( probePtr != NULL ){
                    // find the corresponding probe
                    if( probePtr->isSame(probe) ){
                        // first time for this probe is visited by clone i
                        if( probePtr->cloneNo != i ){
                            probePtr->occ++;
                            // mark this probe is visited by clone i
                            probePtr->cloneNo = i;
                            // update probe annealing position information
                            probePtr->bindPosSum += clone_ith_base;
                        }
                        break;
                    }
                    pLeftPtr = probePtr;
                    probePtr = probePtr->next;
                }
                // reach the end of list
                if( probePtr == NULL ){
                    probePtr = new PPOcc(probe, i, MAX_PROBE_LEN);
                    if( hash_occ_array[pIndex] == NULL )
                         hash_occ_array[pIndex] = probePtr;
                    else
                         pLeftPtr->next = probePtr;
                }

                pHead = clone[0];
                clone_ith_base++; // record current position in clone to chop probe
                clone++;			// move window to right by one
            }
            pdProgDlg->setValue(1+ i + sequences.size() * (probeLen-probeLenMin));
            // user abort
            if(pdProgDlg->wasCanceled()){
                // clean memory
                for( int ii = 0; ii < sequences.size(); ii++ )
                        delete[] sequences[ii];
                sequences.clear();
                for( int ii = 0; ii < HASH_OCC_SIZE; ii++ ){
                        probePtr = hash_occ_array[ii];
                        while( probePtr != NULL ){
                                pLeftPtr = probePtr;
                                probePtr = probePtr->next;
                                delete pLeftPtr;
                        }
                        hash_occ_array[ii] = NULL;
                }
                delete[] hash_occ_array;
                return -1;
            }
        }
        // record probe candidates and clean hash memory
        for( int i = 0; i < HASH_OCC_SIZE; i++ ){
            probePtr = hash_occ_array[i];
            while( probePtr != NULL ){

                occRate = probePtr->occ * 100 / (float)tarSeqs.size();
                if(occRate >= probeCandidFrequency ){
                    // add probe candidates to theApp
                    try{
                        candid = new short[probePtr->pLen+1];
                    }
                    catch( std::bad_alloc& e ){
                        QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for probe candidates.\nProgram is forced to terminate.");
                        return FATAL_ERROR;
                    }

                    for( int j = 0; j < probePtr->pLen; j++ ){
                        candid[j] = probePtr->ppSeq[j];
                    }
                    candid[probePtr->pLen] = -1;
                    probeCandid.push_back(candid);
                    candidOcc.push_back(probePtr->occ);
                    probeBindPosAvg.push_back( probePtr->bindPosSum/probePtr->occ );
                }
                pLeftPtr = probePtr;
                probePtr = probePtr->next;
                delete pLeftPtr;
            }
            hash_occ_array[i] = NULL;

            //user abort
            if(pdProgDlg->wasCanceled()){
                // clean memory
                for( int ii = 0; ii < sequences.size(); ii++ )
                    delete[] sequences[ii];
                sequences.clear();
                delete[] hash_occ_array;
                probeBindPosAvg.clear();
                return -1;
            }
        }
    }
    time = QTime::currentTime();
    timeStr = time.toString();
    logDesign += timeStr;
    logDesign += ":	Found " + QString("%1").arg(probeCandid.size()) + " probe candidates with frequency ";
    logDesign += QString("%1").arg(probeCandidFrequency) + "% in target sequences.\r\n\r\n";

    // user wants to consider less probe candidates
    if( probeCandidTopPctg < 100.0f ){
        // sort probe candidates with descending order of probe occ
        short* tProbe;
        int tOcc;
        float tPos;
        for( int i = 1; i < probeCandid.size(); i++ ){
            for( int j = 0; j < i; j++){
                if( candidOcc[i] > candidOcc[j] ){
                    tProbe = probeCandid[i];
                    probeCandid[i] = probeCandid[j];
                    probeCandid[j] = tProbe;
                    tPos = probeBindPosAvg[i];
                    probeBindPosAvg[i] = probeBindPosAvg[j];
                    probeBindPosAvg[j] = tPos;
                    tOcc = candidOcc[i];
                    candidOcc[i] = candidOcc[j];
                    candidOcc[j] = tOcc;
                }
            }
        }
        // delete bottom list candidate probes
        candidTopNum = (int) (probeCandidTopPctg / 100 * probeCandid.size());
        while( (int)probeCandid.size() > candidTopNum ){
            delete[] probeCandid[probeCandid.size()-1];
            probeCandid.erase( probeCandid.end()-1 );
            probeBindPosAvg.erase( probeBindPosAvg.end()-1 );
        }

        time = QTime::currentTime();
        timeStr = time.toString();
        logDesign += timeStr;
        logDesign += ":	Only consider top " + QString("%1").arg(probeCandid.size()) + " probe candidates.\r\n";
    }

    pdProgDlg->setValue(0);
    // clean memory
    for( int i = 0; i < sequences.size(); i++ )
        delete[] sequences[i];
    sequences.clear();
    delete[] hash_occ_array;  // the content of this occ array has been cleaned previously in "record probe candidates" part
    return 0;
}

bool Designer::continuity(QString primer)
{
    int len = primer.length();
    int con = 0;
    QChar prev;

    for( int i = 0; i < len; i++ ){
        if( i == 0 || primer[i] == prev){
            con++;
            if(primerAvoidACT && prev != 'G' && con >= primerNumACT )
                return true;
            else if(primerAvoidG && prev == 'G' && con >= primerNumG)
                return true;
        }
        else
            con = 1;
        prev = primer[i];
    }
    return false;
}

float Designer::GCcontent(QString primer)
{
    int len = primer.length();
    int GC = 0;
    for( int i = 0; i < len; i++ ){
        if( primer[i] == 'C' || primer[i] == 'G' )
                GC++;
    }
    return ( (float)GC / len * 100 );
}

// Tm = 81.5 + 16.6 log [Na+] + 41(G + C)/length - 500/length
float Designer::meltingTemp(QString primer)
{
    int len = primer.length();
    float GC = 0;
    for( int i = 0; i < len; i++ ){
        if( primer[i] == 'C' || primer[i] == 'G' )
            GC++;
    }
    float Tm = 81.5f + 16.6f * log10(primerNaConsent) + 41.0f * GC / (float)len - 500.0f / (float)len;
    return Tm;
}

int Designer::ppIntraComplementarity(QString pp)
{
    int len = pp.length();

    int maxComp = 0;
    int numComp;
    for( int rEnd = len-1; rEnd >= 0; rEnd-- ){
        numComp = 0;
        for( int i = 0, j = rEnd; i < len && j >= 0; i++, j-- ){
            if( tools::base2int(pp[i]) + tools::base2int(pp[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp )
            maxComp = numComp;
    }
    for( int lEnd = 0; lEnd < len; lEnd++ ){
        numComp = 0;
        for( int i = lEnd, j = len-1; i < len && j >= 0; i++, j-- ){
            if( tools::base2int(pp[i]) + tools::base2int(pp[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp )
            maxComp = numComp;
    }

    if( maxComp > 1 )
        return maxComp;
    else
        return 0;
}

int Designer::pp3IntraComplementarity(QString pp, int intraBase3end)
{
    int len = pp.length();
    int numComp;
    int maxComp = 0;
    int start = len - intraBase3end;
    if( start < 0 )
        start = 0;
    for( ; start < len-1; start++ ){
        numComp = 0;
        for( int i = start, j = len-1; i < len && j >= 0; i++, j-- ){
            if( tools::base2int(pp[i]) + tools::base2int(pp[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp )
            maxComp = numComp;
    }

    if( maxComp > 1 )
        return maxComp;
    else
        return 0;
}

int Designer::ppInterComplementarity(QString p1, QString p2)
{
    int lenP1 = p1.length();
    int lenP2 = p2.length();

    int maxComp = 0;
    int numComp;

    for( int start = 0; start < lenP1; start++ ){
        numComp = 0;
        for( int i = start, j = lenP2-1; i < lenP1 && j >= 0; i++, j-- ){
            if(  tools::base2int(p1[i]) + tools::base2int(p2[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp )
            maxComp = numComp;
    }
    for( int start = lenP1-1; start >= 0; start-- ){
        numComp = 0;
        for( int i = start, j = 0; i >= 0 && j < lenP2; i--, j++ ){
            if( tools::base2int(p1[i]) + tools::base2int(p2[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp )
            maxComp = numComp;
    }
    return maxComp;
}

int Designer::pp3InterComplementarity(QString p1, QString p2, int interBase3end)
{
    int lenP1 = p1.length();
    int lenP2 = p2.length();

    int maxComp = 0;
    int numComp;
    int start;

    if( interBase3end < lenP1 )
        start = lenP1 - interBase3end;
    else
        start = 0;
    for( ; start < lenP1; start++ ){
        numComp = 0;
        for( int i = start, j = lenP2-1; i < lenP1 && j >= 0; i++, j-- ){
            if( tools::base2int(p1[i]) + tools::base2int(p2[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp )
            maxComp = numComp;
    }
    return maxComp;
}

int Designer::primerInterComplementarity(QString left, QString right)
{

    return ppInterComplementarity(left, right);
/*    int lenLeft = left.length();
    int lenRight= right.length();

    int maxComp = 0;
    int numComp;

    // 1. left-right inter-complement
    for( int start = 0; start < lenLeft; start++ ){
        numComp = 0;
        for( int i = start, j = lenRight-1; i < lenLeft && j >= 0; i++, j-- ){
            if(  tools::base2int(left[i]) + tools::base2int(right[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp )
            maxComp = numComp;
    }
    for( int start = lenLeft-1; start >= 0; start-- ){
        numComp = 0;
        for( int i = start, j = 0; i >= 0 && j < lenRight; i--, j++ ){
            if( tools::base2int(left[i]) + tools::base2int(right[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp )
            maxComp = numComp;
    }

    // 2. left-left inter-complement
    for( int start = 0; start < lenLeft; start++ ){
        numComp = 0;
        for( int i = start, j = lenLeft-1; i < lenLeft && j >= 0; i++, j-- ){
            if(  tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp )
                maxComp = numComp;
    }
    for( int start = lenLeft-1; start >= 0; start-- ){
        numComp = 0;
        for( int i = start, j = 0; i >= 0 && j < lenLeft; i--, j++ ){
            if( tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp )
            maxComp = numComp;
    }

    // 3. right-right inter-complement
    for( int start = 0; start < lenRight; start++ ){
        numComp = 0;
        for( int i = start, j = lenRight-1; i < lenRight && j >= 0; i++, j-- ){
            if(  tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp )
            maxComp = numComp;
    }
    for( int start = lenRight-1; start >= 0; start-- ){
        numComp = 0;
        for( int i = start, j = 0; i >= 0 && j < lenRight; i--, j++ ){
            if( tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp )
            maxComp = numComp;
    }
    if( maxComp > 1 )
        return maxComp;
    else
        return 0;*/
}

int Designer::tripleInterComplementarity(QString probe, QString left, QString right)
{

    int maxComp = 0;
    int numComp;

    numComp = ppInterComplementarity(probe, left);
    if (maxComp <  numComp)
        maxComp = numComp;
    numComp = ppInterComplementarity(probe, right);
    if (maxComp <  numComp)
        maxComp = numComp;
    numComp = ppInterComplementarity(left, right);
    if (maxComp <  numComp)
        maxComp = numComp;
    return maxComp;




/*    int lenProbe = probe.length();
    int lenLeft = left.length();
    int lenRight= right.length();


    int maxComp = 0;
    int numComp;

    // 1. probe-left inter-complement
    for( int start = 0; start < lenProbe; start++ ){
        numComp = 0;
        for( int i = start, j = lenLeft-1; i < lenProbe && j >= 0; i++, j-- ){
            if(  tools::base2int(probe[i]) + tools::base2int(left[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp )
            maxComp = numComp;
    }
    for( int start = lenProbe-1; start >= 0; start-- ){
        numComp = 0;
        for( int i = start, j = 0; i >= 0 && j < lenLeft; i--, j++ ){
            if( tools::base2int(probe[i]) + tools::base2int(left[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp )
            maxComp = numComp;
    }

    // 2. probe-right inter-complement
    for( int start = 0; start < lenProbe; start++ ){
        numComp = 0;
        for( int i = start, j = lenRight-1; i < lenProbe && j >= 0; i++, j-- ){
            if(  tools::base2int(probe[i]) + tools::base2int(right[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp )
                maxComp = numComp;
    }
    for( int start = lenProbe-1; start >= 0; start-- ){
        numComp = 0;
        for( int i = start, j = 0; i >= 0 && j < lenRight; i--, j++ ){
            if( tools::base2int(probe[i]) + tools::base2int(right[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp )
            maxComp = numComp;
    }

    // 3. probe-probe inter-complement
    for( int start = 0; start < lenProbe; start++ ){
        numComp = 0;
        for( int i = start, j = lenProbe-1; i < lenProbe && j >= 0; i++, j-- ){
            if(  tools::base2int(probe[i]) + tools::base2int(probe[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp )
                maxComp = numComp;
    }
    for( int start = lenProbe-1; start >= 0; start-- ){
        numComp = 0;
        for( int i = start, j = 0; i >= 0 && j < lenProbe; i--, j++ ){
            if( tools::base2int(probe[i]) + tools::base2int(probe[j]) == 3 )
                numComp++;
        }
        if( maxComp < numComp )
            maxComp = numComp;
    }


    if( maxComp > 1 )
        return maxComp;
    else
        return 0;*/
}

int Designer::primer3InterComplementarity(QString left, QString right)
{
    return pp3InterComplementarity(left, right, primerInterBase3end);
/*        int lenLeft = left.length();
        int lenRight= right.length();

        int maxComp = 0;
        int numComp;
        int start;
        // 1. left-right inter-complement
        if( primerInterBase3end < lenLeft )
            start = lenLeft - primerInterBase3end;
        else
            start = 0;
        for( ; start < lenLeft; start++ ){
            numComp = 0;
            for( int i = start, j = lenRight-1; i < lenLeft && j >= 0; i++, j-- ){
                if( tools::base2int(left[i]) + tools::base2int(right[j]) == 3 )
                    numComp++;
            }
            if( maxComp < numComp )
                maxComp = numComp;
        }

        // 2. left-left inter-complement
        if( primerInterBase3end < lenLeft )
            start = lenLeft - primerInterBase3end;
        else
            start = 0;
        for( ; start < lenLeft; start++ ){
            numComp = 0;
            for( int i = start, j = lenLeft-1; i < lenLeft && j >= 0; i++, j-- ){
                if( tools::base2int(left[i]) + tools::base2int(left[j]) == 3 )
                    numComp++;
            }
            if( maxComp < numComp )
                maxComp = numComp;
        }

        // 3. right-right inter-complement
        if( primerInterBase3end < lenRight )
            start = lenRight - primerInterBase3end;
        else
            start = 0;
        for( ; start < lenRight; start++ ){
            numComp = 0;
            for( int i = start, j = lenRight-1; i < lenRight && j >= 0; i++, j-- ){
                if( tools::base2int(right[i]) + tools::base2int(right[j]) == 3 )
                        numComp++;
            }
            if( maxComp < numComp )
                maxComp = numComp;
        }


        if( maxComp > 1 )
            return maxComp;
        else
            return 0;*/
}

//int Designer::triple3InterComplementarity(QString probe,QString left, QString right)
//{
 //   if (probe == "AACGCAGCGAAACGCGAT")
 //       int breakhere = 0;

    /*int maxComp = 0;
    int numComp;

    numComp = pp3InterComplementarity(probe, left, tripleInterBase3end);
    if (maxComp <  numComp)
        maxComp = numComp;
    numComp = pp3InterComplementarity(probe, right, tripleInterBase3end);
    if (maxComp <  numComp)
        maxComp = numComp;
    numComp = pp3InterComplementarity(left, right, tripleInterBase3end);
    if (maxComp <  numComp)
        maxComp = numComp;
    return maxComp;*/
 /*       int lenProbe = probe.length();
        int lenLeft = left.length();
        int lenRight= right.length();

        int maxComp = 0;
        int numComp;
        int start;
        // 1. probe-left inter-complement
        if( tripleInterBase3end < lenProbe )
            start = lenProbe - tripleInterBase3end;
        else
            start = 0;
        for( ; start < lenProbe; start++ ){
            numComp = 0;
            for( int i = start, j = lenLeft-1; i < lenProbe && j >= 0; i++, j-- ){
                if( tools::base2int(probe[i]) + tools::base2int(left[j]) == 3 )
                    numComp++;
            }
            if( maxComp < numComp )
                maxComp = numComp;
        }

        // 2. probe-right inter-complement
        if( tripleInterBase3end < lenProbe )
            start = lenProbe - tripleInterBase3end;
        else
            start = 0;
        for( ; start < lenProbe; start++ ){
            numComp = 0;
            for( int i = start, j = lenRight-1; i < lenProbe && j >= 0; i++, j-- ){
                if( tools::base2int(probe[i]) + tools::base2int(right[j]) == 3 )
                    numComp++;
            }
            if( maxComp < numComp )
                maxComp = numComp;
        }

        // 3. probe-probe inter-complement
        if( tripleInterBase3end < lenProbe )
            start = lenProbe - tripleInterBase3end;
        else
            start = 0;
        for( ; start < lenProbe; start++ ){
            numComp = 0;
            for( int i = start, j = lenProbe-1; i < lenProbe && j >= 0; i++, j-- ){
                if( tools::base2int(probe[i]) + tools::base2int(probe[j]) == 3 )
                        numComp++;
            }
            if( maxComp < numComp )
                maxComp = numComp;
        }


        if( maxComp > 1 )
            return maxComp;
        else
            return 0;*/
//}

// compare two left primers: from 3' to 5'
// return 0 if equal; 1 if p1 > p2; -1 if p1 < p2
int Designer::lprimerComp(short* p1, short* p2)
{
    int i = 0, j = 0;
    while( p1[i] != -1 )	i++;
    while( p2[j] != -1 )	j++;

    while( i >= 0 && j >= 0 ){
        if( p1[i] < p2[j] ) return -1;
        else if( p1[i] > p2[j] ) return 1;
        i--;	j--;
    }

    if( i < 0 && j >= 0 )	return -1;
    if( i >= 0 && j < 0 )	return 1;
    return 0;
}

int Designer::probeComp(short* p1, short* p2)
{
    int i = 0;
    while( p1[i] != -1 && p2[i] != -1 ){
        if( p1[i] < p2[i] ) return -1;
        else if( p1[i] > p2[i] ) return 1;
        i++;
    }

    if( p1[i] == -1 && p2[i] != -1 )	return -1;
    if( p1[i] != -1 && p2[i] == -1 )	return 1;
    return 0;
}


// compare two right primers: from 5' to 3'
// return 0 if equal; 1 if p1 > p2; -1 if p1 < p2
int Designer::rprimerComp(short* p1, short* p2)
{
    int i = 0;
    while( p1[i] != -1 && p2[i] != -1 ){
        if( p1[i] < p2[i] ) return -1;
        else if( p1[i] > p2[i] ) return 1;
        i++;
    }

    if( p1[i] == -1 && p2[i] != -1 )	return -1;
    if( p1[i] != -1 && p2[i] == -1 )	return 1;
    return 0;
}


// Now keep original structure.
// Need to do structure refine;
/*int PrimerDesigner::filterPrimerCandid(void)
{
    short nUpper = 4+(short)lPrimerCandid.size();
   pdProgDlg->setRange(0,nUpper);

    QString str;
    QString primer;

    // filter out primer candidates with too low or too high GC%
    float GC;
    for( int i = 0; i < lPrimerCandid.size(); i++ ){
        tools::seqInt2Str(lPrimerCandid[i],primer);
        GC = GCcontent(primer);
        if( GC < GCMin || GC > GCMax ){
                QVector<short*>::iterator p = lPrimerCandid.begin() + i;
                delete *p;
                lPrimerCandid.erase(p);
                lPrimerBindPosAvg.erase( lPrimerBindPosAvg.begin()+i );
                i--;
        }
    }
    for( int i = 0; i < rPrimerCandid.size(); i++ ){
        tools::seqInt2Str(rPrimerCandid[i],primer);
        GC = GCcontent(primer);
        if( GC < GCMin || GC > GCMax ){
            QVector<short*>::iterator p = rPrimerCandid.begin() + i;
            delete *p;
            rPrimerCandid.erase(p);
            rPrimerBindPosAvg.erase( rPrimerBindPosAvg.begin()+i );
            i--;
        }
    }
    pdProgDlg->setValue(1);
    //user abort
    if (pdProgDlg->wasCanceled()){
        lPrimerBindPosAvg.clear();
        rPrimerBindPosAvg.clear();
        return -1;
    }


    QTime time = QTime::currentTime();
    QString timeStr = time.toString();
    logPrimerDesign += timeStr;
    logPrimerDesign += ":	Start filtering (single) primer candidates.\r\n";
    logPrimerDesign += "\t\tCheck GC content, " + QString("%1").arg(lPrimerCandid.size()) + " (single) primer candidates left.\r\n";

    // filter out primer candidates with too low or too high Tm
    float Tm;
    for( int i = 0; i < lPrimerCandid.size(); i++ ){
        tools::seqInt2Str(lPrimerCandid[i],primer);
        Tm = meltingTemp(primer);
        if( Tm < tmMin || Tm > tmMax ){
            QVector<short*>::iterator p = lPrimerCandid.begin() + i;
            delete *p;
            lPrimerCandid.erase(p);
            lPrimerBindPosAvg.erase( lPrimerBindPosAvg.begin()+i );
            i--;
        }
    }
    for( int i = 0; i < rPrimerCandid.size(); i++ ){
        tools::seqInt2Str(rPrimerCandid[i],primer);
        Tm = meltingTemp(primer);
        if( Tm < tmMin || Tm > tmMax ){
            QVector<short*>::iterator p = rPrimerCandid.begin() + i;
            delete *p;
            rPrimerCandid.erase(p);
            rPrimerBindPosAvg.erase( rPrimerBindPosAvg.begin()+i );
            i--;
        }
    }
    pdProgDlg->setValue(2);
    if (pdProgDlg->wasCanceled()){
        lPrimerBindPosAvg.clear();
        rPrimerBindPosAvg.clear();
        return -1;
    }
    logPrimerDesign += "\t\tCheck Tm, " + QString("%1").arg(lPrimerCandid.size() ) + " (single) primer candidates left.\r\n";

    // filter out primer candidates with large intra-complementarity
    int numComp;
    for( int i = 0; i < lPrimerCandid.size(); i++ ){
        tools::seqInt2Str(lPrimerCandid[i],primer);
        numComp = primerIntraComplementarity(primer);
        if( numComp > maxSelfComp ){
            QVector<short*>::iterator p = lPrimerCandid.begin() + i;
            delete *p;
            lPrimerCandid.erase(p);
            lPrimerBindPosAvg.erase( lPrimerBindPosAvg.begin()+i );
            i--;
        }
    }
    for( int i = 0; i < rPrimerCandid.size(); i++ ){
        tools::seqInt2Str(rPrimerCandid[i],primer);
        numComp = primerIntraComplementarity(primer);
        if( numComp > maxSelfComp ){
            QVector<short*>::iterator p = rPrimerCandid.begin() + i;
            delete *p;
            rPrimerCandid.erase(p);
            rPrimerBindPosAvg.erase( rPrimerBindPosAvg.begin()+i );
            i--;
        }
    }
    pdProgDlg->setValue(3);
    if (pdProgDlg->wasCanceled()){
        lPrimerBindPosAvg.clear();
        rPrimerBindPosAvg.clear();
        return -1;
    }
    logPrimerDesign += "\t\tCheck intra-complementarity, " + QString("%1").arg(lPrimerCandid.size()) + " (single) primer candidates left.\r\n";



    // filter out primer candidates with large 3' intra-complementarity
    for( int i = 0; i < lPrimerCandid.size(); i++ ){
        tools::seqInt2Str(lPrimerCandid[i],primer);
        numComp = primer3IntraComplementarity(primer);
        if( numComp > max3SelfComp ){
            QVector<short*>::iterator p = lPrimerCandid.begin() + i;
            delete *p;
            lPrimerCandid.erase(p);
            lPrimerBindPosAvg.erase( lPrimerBindPosAvg.begin()+i );
            i--;
        }
    }
    for( int i = 0; i < rPrimerCandid.size(); i++ ){
        tools::seqInt2Str(rPrimerCandid[i],primer);
        numComp = primer3IntraComplementarity(primer);
        if( numComp > max3SelfComp ){
            QVector<short*>::iterator p = rPrimerCandid.begin() + i;
            delete *p;
            rPrimerCandid.erase(p);
            rPrimerBindPosAvg.erase( rPrimerBindPosAvg.begin()+i );
            i--;
        }
    }

    pdProgDlg->setValue(4);
    if (pdProgDlg->wasCanceled()){
        lPrimerBindPosAvg.clear();
        rPrimerBindPosAvg.clear();
        return -1;
    }
    logPrimerDesign += "\t\tCheck 3' intra-complementarity, " + QString("%1").arg(lPrimerCandid.size()) + " (single) primer candidates left.\r\n";


    // house keeping stuff =========================================================================================

    int numLprimers = lPrimerCandid.size();
    int numRprimers = rPrimerCandid.size();

    // only for primer design report window
    QStringList lCandid, rCandid;
    for( int i = 0; i < numLprimers; i++ )
        tools::seqInt2Str(lPrimerCandid[i],primer);
        lCandid.push_back(primer);
    for( int j = 0; j < numRprimers; j++)
        tools::seqInt2StrReverseComplement(rPrimerCandid[j],primer);
        rCandid.push_back(primer );

    // check to see if memory is large enough
    int nPairs = 0;
    for( int i = 0; i < numLprimers; i++ ){
        for( int j = 0; j < numRprimers; j++ ){
            if( i == j )
                continue;
            if( lPrimerBindPosAvg[i] + 20 < rPrimerBindPosAvg[j]
                && abs( meltingTemp(lCandid[i]) - meltingTemp(rCandid[j]) ) <= tmDiff
                && primerInterComplementarity( lCandid[i], rCandid[j] ) <= maxPrPrComp
                && primer3InterComplementarity( lCandid[i], rCandid[j] ) <= max3PrPrComp	)
                        nPairs++;
        }
        pdProgDlg->setValue(4+i);
        //user abort
        if (pdProgDlg->wasCanceled()){
            lPrimerBindPosAvg.clear();
            rPrimerBindPosAvg.clear();
            return -1;
        }
    }
    lPrimerBindPosAvg.clear();
    rPrimerBindPosAvg.clear();

    // get system memory information
   // QString AllMem , AvailableMem;


    try{
        PrimerPair* test = new PrimerPair[nPairs*2];
        delete[] test;
    }catch(std::bad_alloc& e){
        QMessageBox::about(0,"May not have enough Memory",
                "Required memory may exceed the available physical memory.\n"
                "Suggest to abort computation and redefine your primer settings.");


        MyConfirmDlg* CDlg = new MyConfirmDlg("Memory Warning?\n\n\n\n"
                                              "Click OK to terminate computing\n"
                                              "Click Cancel to Proceed computing"
                                              "May be forced to leave when out of memory",0);
        if(CDlg->exec()){
            this->pdProgDlg->close();
            delete this->pdProgDlg;
            //DataTools::cleanMemoryPrimer();
            lPrimerBindPosAvg.clear();
            rPrimerBindPosAvg.clear();
            return -1;
        }
    }

    // sort left and right primer candidates with insertion sort.
    // this is for later the primers with common prefix can be grouped
    // together and do approximation search in groups to save searching time.
    short* tmpPrimer;
    for( int i = 1; i < numLprimers; i++ ){
        for( int j = 0; j < i; j++){
            if( lprimerComp( lPrimerCandid[j], lPrimerCandid[i] ) > 0 ){
                tmpPrimer = lPrimerCandid[i];
                lPrimerCandid[i] = lPrimerCandid[j];
                lPrimerCandid[j] = tmpPrimer;
            }
        }
        if( 4+numLprimers+i < nUpper )
            pdProgDlg->setValue(4+numLprimers+i);
        //user abort
        if (pdProgDlg->wasCanceled()){
            lPrimerBindPosAvg.clear();
            rPrimerBindPosAvg.clear();
            return -1;
        }
    }
    for( int i = 1; i < numRprimers; i++ ){
        for( int j = 0; j < i; j++){
            if( rprimerComp( rPrimerCandid[j], rPrimerCandid[i] ) > 0 ){
                tmpPrimer = rPrimerCandid[i];
                rPrimerCandid[i] = rPrimerCandid[j];
                rPrimerCandid[j] = tmpPrimer;
            }
        }
        if( 4+numLprimers+i < nUpper )
            pdProgDlg->setValue(4+numLprimers+i);
        //user abort
        if (pdProgDlg->wasCanceled()){
            lPrimerBindPosAvg.clear();
            rPrimerBindPosAvg.clear();
            return -1;
        }
    }
    pdProgDlg->setValue(0);
    return 0;
}*/
int Designer::filterPrimerCandid(void)
{
        short nUpper = 6+(short)lPrimerCandid.size();
        pdProgDlg->setRange(0,nUpper);

        QTime time;
        QString timeStr;

        bool b;
        QString tmpSeq;
        time = QTime::currentTime();
        timeStr = time.toString();
        logDesign += timeStr;
        logDesign += ":	Start filtering (single) primer candidates.\r\n";

        // filter out probe candidates with continuous same nucleotide
        if(primerAvoidACT || primerAvoidG){
            for( int i = 0; i < lPrimerCandid.size(); i++ ){
                tools::seqInt2Str(lPrimerCandid[i],tmpSeq);
                b = continuity(tmpSeq);
                if( b ){
                    QVector<short*>::iterator p = lPrimerCandid.begin() + i;
                    delete *p;
                    lPrimerCandid.erase(p);
                    lPrimerBindPosAvg.erase( lPrimerBindPosAvg.begin()+i );
                    i--;
                }
            }
            for( int i = 0; i < rPrimerCandid.size(); i++ ){
                tools::seqInt2Str(rPrimerCandid[i],tmpSeq);
                b = continuity(tmpSeq);
                if( b){
                    QVector<short*>::iterator p = rPrimerCandid.begin() + i;
                    delete *p;
                    rPrimerCandid.erase(p);
                    rPrimerBindPosAvg.erase( rPrimerBindPosAvg.begin()+i );
                    i--;
                }
            }

            //user abort
            if(pdProgDlg->wasCanceled()){
                lPrimerBindPosAvg.clear();
                rPrimerBindPosAvg.clear();
                return -1;
            }
            /*time = QTime::currentTime();
            timeStr = time.toString();
            logDesign += timeStr;*/
            logDesign += "\t\tCheck continuous symbol, " + QString("%1").arg(lPrimerCandid.size() ) + " (single) primer candidates left.\r\n";
        }



        // filter out primer candidates with too low or too high GC%
        float GC;
        for( int i = 0; i < lPrimerCandid.size(); i++ ){
            tools::seqInt2Str(lPrimerCandid[i],tmpSeq);
            GC = GCcontent(tmpSeq);
            if( GC < primerGCMin || GC > primerGCMax ){
                QVector<short*>::iterator p = lPrimerCandid.begin() + i;
                delete *p;
                lPrimerCandid.erase(p);
                lPrimerBindPosAvg.erase( lPrimerBindPosAvg.begin()+i );
                i--;
            }
        }
        for( int i = 0; i < rPrimerCandid.size(); i++ ){
            tools::seqInt2Str(rPrimerCandid[i],tmpSeq);
            GC = GCcontent(tmpSeq);
            if( GC < primerGCMin || GC > primerGCMax ){
                QVector<short*>::iterator p = rPrimerCandid.begin() + i;
                delete *p;
                rPrimerCandid.erase(p);
                rPrimerBindPosAvg.erase( rPrimerBindPosAvg.begin()+i );
                i--;
            }
        }
        pdProgDlg->setValue(2);
        //user abort
        if(pdProgDlg->wasCanceled()){
            lPrimerBindPosAvg.clear();
            rPrimerBindPosAvg.clear();
            return -1;
        }

        /*time = QTime::currentTime();
        timeStr = time.toString();
        logDesign += timeStr;*/
        logDesign += "\t\tCheck GC content, " + QString("%1").arg(lPrimerCandid.size() ) + " (single) primer candidates left.\r\n";


        // filter out primer candidates who has too much GC in the last 5 bases at 3' end
        if(primerAvoidCGat3end){
            for( int i = 0; i < lPrimerCandid.size(); i++ ){
                tools::seqInt2Str(lPrimerCandid[i],tmpSeq);
                tmpSeq = tmpSeq.right(5);
                GC = GCcontent(tmpSeq);
                if( GC >= (int)((float)primerNumCGat3end/5*100)){
                    QVector<short*>::iterator p = lPrimerCandid.begin() + i;
                    delete *p;
                    lPrimerCandid.erase(p);
                    lPrimerBindPosAvg.erase( lPrimerBindPosAvg.begin()+i );
                    i--;
                }
            }
            for( int i = 0; i < rPrimerCandid.size(); i++ ){
                tools::seqInt2Str(rPrimerCandid[i],tmpSeq);
                tmpSeq = tmpSeq.right(5);
                GC = GCcontent(tmpSeq);
                if( GC >= (int)((float)primerNumCGat3end/5*100 )){
                    QVector<short*>::iterator p = rPrimerCandid.begin() + i;
                    delete *p;
                    rPrimerCandid.erase(p);
                    rPrimerBindPosAvg.erase( rPrimerBindPosAvg.begin()+i );
                    i--;
                }
            }
            pdProgDlg->setValue(3);
            //user abort
            if(pdProgDlg->wasCanceled()){
                lPrimerBindPosAvg.clear();
                rPrimerBindPosAvg.clear();
                return -1;
            }

            /*time = QTime::currentTime();
            timeStr = time.toString();
            logDesign += timeStr;*/
            logDesign += "\t\tCheck GC at 3\' end, " + QString("%1").arg(lPrimerCandid.size() ) + " (single) primer candidates left.\r\n";
        }

        // filter out primer candidates with too low or too high Tm
        float Tm;
        for( int i = 0; i < lPrimerCandid.size(); i++ ){
            tools::seqInt2Str(lPrimerCandid[i],tmpSeq);
            Tm = meltingTemp(tmpSeq);
            if( Tm < primerTmMin || Tm > primerTmMax ){
                QVector<short*>::iterator p = lPrimerCandid.begin() + i;
                delete *p;
                lPrimerCandid.erase(p);
                lPrimerBindPosAvg.erase( lPrimerBindPosAvg.begin()+i );
                i--;
            }
        }
        for( int i = 0; i < rPrimerCandid.size(); i++ ){
            tools::seqInt2Str(rPrimerCandid[i],tmpSeq);
            Tm = meltingTemp(tmpSeq);
            if( Tm < primerTmMin || Tm > primerTmMax ){
                QVector<short*>::iterator p = rPrimerCandid.begin() + i;
                delete *p;
                rPrimerCandid.erase(p);
                rPrimerBindPosAvg.erase( rPrimerBindPosAvg.begin()+i );
                i--;
            }
        }
        pdProgDlg->setValue(4);
        //user abort
        if(pdProgDlg->wasCanceled()){
            lPrimerBindPosAvg.clear();
            rPrimerBindPosAvg.clear();
            return -1;
        }
        /*time = QTime::currentTime();
        timeStr = time.toString();
        logDesign += timeStr;*/
        logDesign += "\t\tCheck Tm, " + QString("%1").arg(lPrimerCandid.size()) + " (single) primer candidates left.\r\n";

        // filter out primer candidates with large intra-complementarity
        int numComp;
        for( int i = 0; i < lPrimerCandid.size(); i++ ){
            tools::seqInt2Str(lPrimerCandid[i],tmpSeq);
            numComp = ppIntraComplementarity(tmpSeq);
            if( numComp > primerMaxSelfComp ){
                QVector<short*>::iterator p = lPrimerCandid.begin() + i;
                delete *p;
                lPrimerCandid.erase(p);
                lPrimerBindPosAvg.erase( lPrimerBindPosAvg.begin()+i );
                i--;
            }
        }
        for( int i = 0; i < rPrimerCandid.size(); i++ ){
            tools::seqInt2Str(rPrimerCandid[i],tmpSeq);
            numComp =ppIntraComplementarity(tmpSeq);
            if( numComp > primerMaxSelfComp ){
                QVector<short*>::iterator p = rPrimerCandid.begin() + i;
                delete *p;
                rPrimerCandid.erase(p);
                rPrimerBindPosAvg.erase( rPrimerBindPosAvg.begin()+i );
                i--;
            }
        }
        pdProgDlg->setValue(5);
        //user abort
        if(pdProgDlg->wasCanceled()){
            lPrimerBindPosAvg.clear();
            rPrimerBindPosAvg.clear();
            return -1;
        }

        /*time = QTime::currentTime();
        timeStr = time.toString();
        logDesign += timeStr;*/
        logDesign += "\t\tCheck intra-complementarity, " + QString("%1").arg(lPrimerCandid.size()) + " (single) primer candidates left.\r\n";

        // filter out primer candidates with large 3' intra-complementarity
        for( int i = 0; i < lPrimerCandid.size(); i++ ){
            tools::seqInt2Str(lPrimerCandid[i],tmpSeq);
            numComp = pp3IntraComplementarity(tmpSeq, primerIntraBase3end);
            if( numComp > primerMax3SelfComp ){
                QVector<short*>::iterator p = lPrimerCandid.begin() + i;
                delete *p;
                lPrimerCandid.erase(p);
                lPrimerBindPosAvg.erase( lPrimerBindPosAvg.begin()+i );
                i--;
            }
        }
        for( int i = 0; i < rPrimerCandid.size(); i++ ){
            tools::seqInt2StrReverseComplement(rPrimerCandid[i],tmpSeq);
            numComp = pp3IntraComplementarity(tmpSeq, primerIntraBase3end);
            if( numComp > primerMax3SelfComp ){
                QVector<short*>::iterator p = rPrimerCandid.begin() + i;
                delete *p;
                rPrimerCandid.erase(p);
                rPrimerBindPosAvg.erase( rPrimerBindPosAvg.begin()+i );
                i--;
            }
        }
        pdProgDlg->setValue(6);
        //user abort
        if(pdProgDlg->wasCanceled()){
            lPrimerBindPosAvg.clear();
            rPrimerBindPosAvg.clear();
            return -1;
        }
        /*time = QTime::currentTime();
        timeStr = time.toString();
        logDesign += timeStr;*/
        logDesign += "\t\tCheck 3' intra-complementarity, " + QString("%1").arg(lPrimerCandid.size()) + " (single) primer candidates left.\r\n\r\n";


        // house keeping stuff =========================================================================================

        int numLprimers = (int)lPrimerCandid.size();
        int numRprimers = (int)rPrimerCandid.size();

        // only for primer design report window
        QStringList lCandid, rCandid;
        for( int i = 0; i < numLprimers; i++ ){
            tools::seqInt2Str(lPrimerCandid[i],tmpSeq);
            lCandid.push_back(tmpSeq);
        }
        for( int j = 0; j < numRprimers; j++ ) {
            tools::seqInt2StrReverseComplement(rPrimerCandid[j],tmpSeq);
            rCandid.push_back(tmpSeq);
        }

        // check to see if memory is large enough
        int nPairs = 0;
        for( int i = 0; i < numLprimers; i++ ){
            for( int j = 0; j < numRprimers; j++ ){
                if( i == j )
                        continue;
                if( lPrimerBindPosAvg[i] + 20 < rPrimerBindPosAvg[j]
                    && fabs( meltingTemp(lCandid[i]) - meltingTemp(rCandid[j]) ) <= primerTmDiff
                    && primerInterComplementarity( lCandid[i], rCandid[j] ) <= primerMaxPrPrComp
                    && primer3InterComplementarity( lCandid[i], rCandid[j] ) <= primerMax3PrPrComp	)
                        nPairs++;
            }

            pdProgDlg->setValue(6+i);
            //user abort
            if(pdProgDlg->wasCanceled()){
                lPrimerBindPosAvg.clear();
                rPrimerBindPosAvg.clear();
                return -1;
            }
        }
        lPrimerBindPosAvg.clear();	rPrimerBindPosAvg.clear();


        // get system memory information
        /*CString AllMemo , AvailableMemo;
        MEMORYSTATUS memory;
        memory.dwLength = sizeof(memory);
        GlobalMemoryStatus(&memory);
        AllMemo.Format("%d",memory.dwTotalPhys/1024/1024);
        AvailableMemo.Format("%d",memory.dwAvailPhys/1024/1024);

        int memEachPrimerPair = sizeof(PrimerPair) * 2;

        if( (int)memory.dwAvailPhys <= nPairs * memEachPrimerPair ){
                CString total;
                total.Format( "%d", nPairs );
                MessageBox(NULL, "Total physical memory in your system : " + AllMemo + "MB.\n"
                        "Available physical memory in your system : " + AvailableMemo + "MB.\n\n"
                        "Estimated number of primer pairs : " + total + ".\n"
                        "Required memory may exceed the available physical memory.\n"
                        "Suggest to abort computation and redefine your primer settings.", "", MB_OK);

                CGeneralConfirmDlg dlg(6);
                if( dlg.DoModal() ){
                        progrsDlg.DestroyWindow();
                        parentDlg->EnableWindow(TRUE);
                        parentDlg->SetForegroundWindow();
                        //this->cleanMemory();
                        lPrimerBindPosAvg.clear();
                        rPrimerBindPosAvg.clear();
                        return -1;
                }
        }*/


        // sort left and right primer candidates with insertion sort.
        // this is for later the primers with common prefix can be grouped
        // together and do approximation search in groups to save searching time.
        short* tPrimer;
        for( int i = 1; i < numLprimers; i++ ){
            for( int j = 0; j < i; j++){
                if( lprimerComp( lPrimerCandid[j], lPrimerCandid[i] ) > 0 ){
                    tPrimer = lPrimerCandid[i];
                    lPrimerCandid[i] = lPrimerCandid[j];
                    lPrimerCandid[j] = tPrimer;
                }
            }
            if( 5+numLprimers+i < nUpper ){
                pdProgDlg->setValue(6+numLprimers+i);
                //pdProgDlg->update();
            }
            //user abort
            if(pdProgDlg->wasCanceled()){
                lPrimerBindPosAvg.clear();
                rPrimerBindPosAvg.clear();
                return -1;
            }
        }
        for( int i = 1; i < numRprimers; i++ ){
            for( int j = 0; j < i; j++){
                if( rprimerComp( rPrimerCandid[j], rPrimerCandid[i] ) > 0 ){
                    tPrimer = rPrimerCandid[i];
                    rPrimerCandid[i] = rPrimerCandid[j];
                    rPrimerCandid[j] = tPrimer;
                }
            }
            if( 5+numLprimers+i < nUpper ){
                 pdProgDlg->setValue(6+numLprimers+i);
                 //pdProgDlg->update();
             }
            //user abort
            if(pdProgDlg->wasCanceled()){
                lPrimerBindPosAvg.clear();
                rPrimerBindPosAvg.clear();
                return -1;
            }
        }

        pdProgDlg->setValue(0);
        //pdProgDlg->update();
        return 0;
}

int Designer::filterProbeCandid(void)
{
    short nUpper = 6+(short)probeCandid.size();
    pdProgDlg->setRange(0,nUpper);

    QTime time;
    QString timeStr;

    bool b;
    QString tmpSeq;
    time = QTime::currentTime();
    timeStr = time.toString();
    logDesign += timeStr;
    logDesign += ":	Start filtering probe candidates.\r\n";

    // filter out probe candidates with continuous same nucleotide
    if(probeAvoidACT || probeAvoidG){
        for( int i = 0; i < probeCandid.size(); i++ ){
            tools::seqInt2Str(probeCandid[i],tmpSeq);
            b = continuity(tmpSeq);
            if( b ){
                QVector<short*>::iterator p = probeCandid.begin() + i;
                delete *p;
                probeCandid.erase(p);
                probeBindPosAvg.erase( probeBindPosAvg.begin()+i );
                i--;
            }
        }
        //user abort
        if(pdProgDlg->wasCanceled()){
            probeBindPosAvg.clear();
            return -1;
        }
        /*time = QTime::currentTime();
        timeStr = time.toString();
        logDesign += timeStr;*/
        logDesign += "\t\tCheck continuous symbol, " + QString("%1").arg(probeCandid.size() ) + " probe candidates left.\r\n";
    }

    // filter out probe candidates with too low or too high GC%
    float GC;
    for( int i = 0; i < probeCandid.size(); i++ ){
        tools::seqInt2Str(probeCandid[i],tmpSeq);
        GC = GCcontent(tmpSeq);
        if( GC < probeGCMin || GC > probeGCMax ){
            QVector<short*>::iterator p = probeCandid.begin() + i;
            delete *p;
            probeCandid.erase(p);
            probeBindPosAvg.erase( probeBindPosAvg.begin()+i );
            i--;
        }
    }

    pdProgDlg->setValue(2);
    //user abort
    if(pdProgDlg->wasCanceled()){
        probeBindPosAvg.clear();
        return -1;
    }

    /*time = QTime::currentTime();
    timeStr = time.toString();
    logDesign += timeStr;*/
    logDesign += "\t\tCheck GC content, " + QString("%1").arg(probeCandid.size() ) + " probe candidates left.\r\n";


    // filter out probe candidates who has too much GC in the last bases at 5' end
    if(probeAvoidGat5end){
        for( int i = 0; i < probeCandid.size(); i++ ){
            if(probeCandid[i][0] == 2){
                QVector<short*>::iterator p = probeCandid.begin() + i;
                delete *p;
                probeCandid.erase(p);
                probeBindPosAvg.erase( probeBindPosAvg.begin()+i );
                i--;
            }


            /*for(int j = 0; j < probeNumGat5end; j++){
                if(probeCandid[i][j] == 2){
                    QVector<short*>::iterator p = probeCandid.begin() + i;
                    delete *p;
                    probeCandid.erase(p);
                    probeBindPosAvg.erase( probeBindPosAvg.begin()+i );
                    i--;
                    break;
                }
            }*/
        }
        pdProgDlg->setValue(3);
        //user abort
        if(pdProgDlg->wasCanceled()){
            probeBindPosAvg.clear();
            return -1;
        }

        /*time = QTime::currentTime();
        timeStr = time.toString();
        logDesign += timeStr;*/
        logDesign += "\t\tCheck G at 3\' end, " + QString("%1").arg(probeCandid.size() ) + " probe candidates left.\r\n";
    }

    // filter out probe candidates with too low or too high Tm
    float Tm;
    for( int i = 0; i < probeCandid.size(); i++ ){
        tools::seqInt2Str(probeCandid[i],tmpSeq);
        Tm = meltingTemp(tmpSeq);
        if( Tm < probeTmMin || Tm > probeTmMax ){
            QVector<short*>::iterator p = probeCandid.begin() + i;
            delete *p;
            probeCandid.erase(p);
            probeBindPosAvg.erase( probeBindPosAvg.begin()+i );
            i--;
        }
    }
    pdProgDlg->setValue(4);
    //user abort
    if(pdProgDlg->wasCanceled()){
        probeBindPosAvg.clear();
        return -1;
    }
    /*time = QTime::currentTime();
    timeStr = time.toString();
    logDesign += timeStr;*/
    logDesign += "\t\tCheck Tm, " + QString("%1").arg(probeCandid.size()) + " probe candidates left.\r\n";

    // filter out probe candidates with large intra-complementarity
    int numComp;
    for( int i = 0; i < probeCandid.size(); i++ ){
        tools::seqInt2Str(probeCandid[i],tmpSeq);

        numComp = ppIntraComplementarity(tmpSeq);
        if( numComp > probeMaxSelfComp ){
            QVector<short*>::iterator p = probeCandid.begin() + i;
            delete *p;
            probeCandid.erase(p);
            probeBindPosAvg.erase( probeBindPosAvg.begin()+i );
            i--;
        }
        /*numComp = pp3IntraComplementarity(tmpSeq, probeIntraBase3end);
        if (numComp > probeMax3SelfComp){
            QVector<short*>::iterator p = probeCandid.begin() + i;
            delete *p;
            probeCandid.erase(p);
            probeBindPosAvg.erase( probeBindPosAvg.begin()+i );
            i--;
        }*/
    }
    pdProgDlg->setValue(5);
    //user abort
    if(pdProgDlg->wasCanceled()){
        probeBindPosAvg.clear();
        return -1;
    }
    time = QTime::currentTime();
    timeStr = time.toString();
    logDesign += timeStr;
    logDesign += ":\t\tCheck intra-complementarity, " + QString("%1").arg(probeCandid.size()) + " probe candidates left.\r\n";


    // filter out probe candidates with large 3' intra-complementarity
    /*for( int i = 0; i < probeCandid.size(); i++ ){
        tools::seqInt2Str(probeCandid[i],tmpSeq);
        numComp = pp3IntraComplementarity(tmpSeq, probeIntraBase3end);
        if( numComp > probeMax3SelfComp ){
            QVector<short*>::iterator p = probeCandid.begin() + i;
            delete *p;
            probeCandid.erase(p);
            probeBindPosAvg.erase( probeBindPosAvg.begin()+i );
            i--;
        }
    }*/
    pdProgDlg->setValue(6);
    //user abort
    if(pdProgDlg->wasCanceled()){
        probeBindPosAvg.clear();
        return -1;
    }
    /*time = QTime::currentTime();
    timeStr = time.toString();
    logDesign += timeStr;*/
    logDesign += "\t\tCheck 3' intra-complementarity, " + QString("%1").arg(probeCandid.size()) + " probe candidates left.\r\n\r\n";


    // house keeping stuff =========================================================================================

    int numProbes = (int)probeCandid.size();

    // only for probe design report window
    QStringList candid;
    for( int i = 0; i < numProbes; i++ ){
        tools::seqInt2Str(probeCandid[i],tmpSeq);
        candid.push_back(tmpSeq);
    }

    // sort left and right probe candidates with insertion sort.
    // this is for later the probes with common prefix can be grouped
    // together and do approximation search in groups to save searching time.
    short* tProbe;
    for( int i = 1; i < numProbes; i++ ){
        for( int j = 0; j < i; j++){
            if( probeComp( probeCandid[j], probeCandid[i] ) > 0 ){
                tProbe = probeCandid[i];
                probeCandid[i] = probeCandid[j];
                probeCandid[j] = tProbe;
            }
        }
        if( 5+numProbes+i < nUpper ){
            pdProgDlg->setValue(5+numProbes+i);
            //pdProgDlg->update();
        }
        //user abort
        if(pdProgDlg->wasCanceled()){
            probeBindPosAvg.clear();
            return -1;
        }
    }

    pdProgDlg->setValue(0);
    return 0;
}

int Designer::allocAnnealingPosVar( short ***&bindPosTarSeqs, short ***&bindPosNonTarSeqs, bool isLeft)
{
    int numTarSeqs = (int)tarSeqs.size();
    int numNontarSeqs = (int)nontarSeqs.size();
    int numPrimers;
    if (isLeft){
        numPrimers = (int)lPrimerCandid.size();
    }else
        numPrimers = (int)rPrimerCandid.size();

    bindPosTarSeqs = new short**[numPrimers];
    bindPosNonTarSeqs = new short**[numPrimers];
    for( int i = 0; i < numPrimers; i++ ){
        // for target sequences
        try{
             bindPosTarSeqs[i] = new short*[numTarSeqs];
        }
        catch( std::bad_alloc& e ){
            QMessageBox::about(0, "Memory Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            return FATAL_ERROR;
        }
        for( int j = 0; j < numTarSeqs; j++ )
            bindPosTarSeqs[i][j] = new short[2];
        // for non-target sequences
        try{
            bindPosNonTarSeqs[i] = new short*[numNontarSeqs];
        }
        catch( std::bad_alloc& e ){
            QMessageBox::about(0, "Memory Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            return FATAL_ERROR;
        }
        for( int j = 0; j < numNontarSeqs; j++ )
             bindPosNonTarSeqs[i][j] = new short[2];
    }
    return 0;
}

void Designer::freeAnnealingPosVar( short ***&bindPosTarSeqs, short ***&bindPosNonTarSeqs, bool isLeft )
{
    int numTarSeqs = (int)tarSeqs.size();
    int numNontarSeqs = (int)nontarSeqs.size();
    int numPrimers;
    if (isLeft){
        numPrimers = (int)lPrimerCandid.size();
    }else
        numPrimers = (int)rPrimerCandid.size();

    for( int i = 0; i < numPrimers; i++ ){
        for( int j = 0; j < numTarSeqs; j++ )
            delete[] bindPosTarSeqs[i][j];
        for( int j = 0; j < numNontarSeqs; j++ )
            delete[] bindPosNonTarSeqs[i][j];
        delete[] bindPosTarSeqs[i];
        delete[] bindPosNonTarSeqs[i];
    }
    delete[] bindPosTarSeqs;
    delete[] bindPosNonTarSeqs;
}

void Designer::freeSeqs(QVector<short*>& tar,QVector<short*>& nontar)
{
    int numTarSeqs = (int)tarSeqs.size();
    int numNontarSeqs = (int)nontarSeqs.size();
    for( int i = 0; i < numTarSeqs; i++ )
        delete[] tar[i];
    for( int i = 0; i < numNontarSeqs; i++ )
        delete[] nontar[i];
    tar.clear();
    nontar.clear();
}

void Designer::prepareReversedSeqs(QVector<short*>& reversedTarSeqs, QVector<short*>& reversedNonTarSeqs  )
{
    int seqLen;
    short* seq;
    int numTarSeqs = (int)tarSeqs.size();
    int numNontarSeqs = (int)nontarSeqs.size();
    for( int i = 0; i < numTarSeqs; i++ ){
        seqLen = 0;
        while( tarSeqs[i][seqLen] >= 0 )
            seqLen++;
        seq = new short[seqLen+1];
        for(int j = 0, k = seqLen-1; j < seqLen; j++, k--)
             seq[j] = tarSeqs[i][k];
        seq[seqLen] = -1;
        reversedTarSeqs.push_back(seq);
    }
    for( int i = 0; i < numNontarSeqs; i++ ){
        seqLen = 0;
        while( nontarSeqs[i][seqLen] >= 0 )
            seqLen++;
        seq = new short[seqLen+1];
        for(int j = 0, k = seqLen-1; j < seqLen; j++, k--)
            seq[j] = nontarSeqs[i][k];
        seq[seqLen] = -1;
        reversedNonTarSeqs.push_back(seq);
    }
}

// check if primer p1 is a prefix of p2
// note that for left primer start from 3' to 5'
int Designer::isPrefixLeftPrimer(short* p1, short* p2)
{
    int i = 0, j = 0;
    while( p1[i] != -1 )	i++;
    while( p2[j] != -1 )	j++;

    while( i >= 0 && j >= 0 ){
        if( p1[i] != p2[j] )	return 0;
        i--;	j--;
    }
    if( i > 0 && j <= 0 )	return 0;
    return 1;
}

// check if primer p1 is a prefix of p2 from 5 to 3'
int Designer::isPrefixRightPrimer(short* p1, short* p2)
{
    int i = 0;
    while( p1[i] != -1 && p2[i] != -1 ){
            if( p1[i] != p2[i] )	return 0;
            i++;
    }
    if( p1[i] != -1 && p2[i] == -1 )	return 0;
    return 1;
}

// check if primer p1 is a prefix of p2
// note that for left primer start from 3' to 5'
int Designer::isPrefixProbe(short* p1, short* p2)
{
    int len1 = 0, len2 = 0;
    while( p1[len1] != -1 )	len1++;
    while( p2[len2] != -1 )	len2++;

    int mid1 = len1/2;
    int mid2 = len2/2;

    int i = mid1;
    int j = mid2;
    while( i < len1 && j < len2 ){
        if( p1[i] != p2[j] )	return 0;
        i++;	j++;
    }
    if( p1[i] != -1 && p2[i] == -1 )	return 0;

    i = mid1;
    j = mid2;
    while( i > 0 && j > 0 ){
        if( p1[i] != p2[j] )	return 0;
        i--;	j--;
    }
    if( i > 0 && j <= 0 )	return 0; //that way p2 is a prefix of p1
    return 1;

    /*int i = 0;
    while( p1[i] != -1 && p2[i] != -1 ){
            if( p1[i] != p2[i] )	return 0;
            i++;
    }
    if( p1[i] != -1 && p2[i] == -1 )	return 0;
    return 1;*/
}

int Designer::quickSearchPrimerInSeqs( PrimerQuickSearch* ptr,
                                             QVector<short*> primerCandid,QVector<short*> seqs,
                                             int lft_rht, int tar_nontar,
                                             short ***&lPrBindPosTarSeqs, short ***&lPrBindPosNonTarSeqs,
                                             short ***&rPrBindPosTarSeqs, short ***&rPrBindPosNonTarSeqs,
                                             QVector<short*> &reversedTarSeqs, QVector<short*> &reversedNonTarSeqs)
{
    short ***bindPosTarSeqs;
    int numSeqs = seqs.size();
    int numPrimers = primerCandid.size();
    short primerGroupLens[40];
    int prLen, prNum;
    int pr1st, prlast;
    int val;
    pr1st = 0;	prlast = 1;

    if(lft_rht == 0 && tar_nontar == 0){
        bindPosTarSeqs = lPrBindPosTarSeqs;
    }else if(lft_rht == 1 && tar_nontar == 0){
        bindPosTarSeqs = rPrBindPosTarSeqs;
    }else if(lft_rht == 0 && tar_nontar == 1){
        bindPosTarSeqs = lPrBindPosNonTarSeqs;
    }else{
        bindPosTarSeqs = rPrBindPosNonTarSeqs;
    }

    // 1. primer candidates in target sequences
    while( prlast < numPrimers ){
        if( isPrefixLeftPrimer( primerCandid[prlast-1], primerCandid[prlast] ) )
            prlast++;
        else{
            prNum = prlast - pr1st;
            // get a group. //j: the j-th primer
            for( int i = pr1st, j = 0; i < prlast; i++, j++ ){
                prLen = 0;
                while( primerCandid[i][prLen] >= 0 )
                    prLen++;
                primerGroupLens[j] = prLen;
            }
            ptr->initMember( primerCandid[prlast-1], prNum, primerGroupLens, lft_rht );
            // start searching
            for( int i = 0; i < numSeqs; i++ ){
                if(tar_nontar == 0)
                    ptr->searchPrimersInTarSeq( seqs[i] );
                else
                    ptr->searchPrimersInNonTarSeq(seqs[i]);
                for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
                    bindPosTarSeqs[j][i][0] = ptr->bindingPosStart[k];
                    bindPosTarSeqs[j][i][1] = ptr->bindingPosEnd[k];
                }
            }
            pr1st = prlast++;

            if(lft_rht == 0){
                val = prlast;
            }else{
                val = prlast+lPrimerCandid.size();
            }

            pdProgDlg->setValue(val);
            //pdProgDlg->update();
            //user abort
            if (pdProgDlg->wasCanceled()){
                freeAnnealingPosVar(lPrBindPosTarSeqs,lPrBindPosNonTarSeqs,true);
                freeAnnealingPosVar(rPrBindPosTarSeqs,rPrBindPosNonTarSeqs, false);
                freeSeqs(reversedTarSeqs, reversedNonTarSeqs);
                return -1;
            }
        }
    }

    prNum = prlast - pr1st;
    for( int i = pr1st, j = 0; i < prlast; i++, j++ ){
        prLen = 0;
        while( primerCandid[i][prLen] >= 0 )
            prLen++;
        primerGroupLens[j] = prLen;
    }
    ptr->initMember( primerCandid[prlast-1], prNum, primerGroupLens, lft_rht );
    // start searching
    for( int i = 0; i < numSeqs; i++ ){
        if(tar_nontar == 0)
            ptr->searchPrimersInTarSeq( seqs[i] );
        else
            ptr->searchPrimersInNonTarSeq(seqs[i]);
        for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
            bindPosTarSeqs[j][i][0] = ptr->bindingPosStart[k];
            bindPosTarSeqs[j][i][1] = ptr->bindingPosEnd[k];
        }
    }

    if(lft_rht == 0){
        val = prlast;
    }else{
        val = prlast+lPrimerCandid.size();
    }
    pdProgDlg->setValue(val);
    //pdProgDlg->update();
    //user abort
    if (pdProgDlg->wasCanceled()){
        // free memory
        freeAnnealingPosVar(lPrBindPosTarSeqs,lPrBindPosNonTarSeqs,true);
        freeAnnealingPosVar(rPrBindPosTarSeqs,rPrBindPosNonTarSeqs, false);
        freeSeqs(reversedTarSeqs, reversedNonTarSeqs);
        return -1;
    }
}

// randomly select 100 position in "total"
// if total < 100, return "total" positions.
// this member is for estimate the frequency of a primer pair appering
// in target and nontaget sequences.
void Designer::rand100position(int total, QVector<short> &sel)
{
    sel.clear();
    for( int i = 0; i < total; i++ )
         sel.push_back(i);

    int del;
    //srand( (unsigned)time(NULL) );
    while( sel.size() > 100 ){
        del = rand() % (int)sel.size();
        sel.erase( sel.begin()+del );
    }
}
/*int PrimerDesigner::quickSearchPrimerInSeqs(void)
{
    if( lPrimerCandid.size() == 0 || rPrimerCandid.size() == 0 )
         return 0;


    short ***lPrBindPosTarSeqs, ***lPrBindPosNonTarSeqs;
    short ***rPrBindPosTarSeqs, ***rPrBindPosNonTarSeqs;
    QVector<float> lPrPctgTarSeqs, lPrPctgNonTarSeqs;
    QVector<float> rPrPctgTarSeqs, rPrPctgNonTarSeqs;
    int numLprimers = (int)lPrimerCandid.size();
    int numRprimers = (int)rPrimerCandid.size();
    int numTarSeqs = (int)tarSeqs.size();
    int numNontarSeqs = (int)nontarSeqs.size();
    QVector<short*> reversedTarSeqs;
    QVector<short*> reversedNonTarSeqs;
    short* sequence;
    short primerGroupLens[40];
    int prLen, prNum;
    int pr1st, prlast;
    int seqLen;
    QString timeStr;
    QString str;

    // first allocate memory for primer annealing position variables
    lPrBindPosTarSeqs = new short**[numLprimers];
    lPrBindPosNonTarSeqs = new short**[numLprimers];
    for( int i = 0; i < numLprimers; i++ ){
            // for target sequences
            try{
                    lPrBindPosTarSeqs[i] = new short*[numTarSeqs];
            }
            catch(std::bad_alloc& e ){
                QMessageBox::about(0, "Memory Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
                return FATAL_ERROR;
            }
            for( int j = 0; j < numTarSeqs; j++ )
                    lPrBindPosTarSeqs[i][j] = new short[2];

            // for non-target sequences
            try{
                    lPrBindPosNonTarSeqs[i] = new short*[numNontarSeqs];
            }
            catch( std::bad_alloc& e ){
                QMessageBox::about(0, "Memory Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
                return FATAL_ERROR;
            }
            for( int j = 0; j < numNontarSeqs; j++ )
                    lPrBindPosNonTarSeqs[i][j] = new short[2];
    }
    rPrBindPosTarSeqs = new short**[numRprimers];
    rPrBindPosNonTarSeqs = new short**[numRprimers];
    for( int i = 0; i < numRprimers; i++ ){
            // for target sequences
            try{
                    rPrBindPosTarSeqs[i] = new short*[numTarSeqs];
            }
            catch( std::bad_alloc& e ){
                QMessageBox::about(0, "Memory Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
                return FATAL_ERROR;
            }
            for( int j = 0; j < numTarSeqs; j++ )
                    rPrBindPosTarSeqs[i][j] = new short[2];

            // for non-target sequences
            try{
                    rPrBindPosNonTarSeqs[i] = new short*[numNontarSeqs];
            }
            catch( std::bad_alloc& e ){
                QMessageBox::about(0, "Memory Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
                return FATAL_ERROR;
            }
            for( int j = 0; j < numNontarSeqs; j++ )
                    rPrBindPosNonTarSeqs[i][j] = new short[2];
    }
    //user abort
    if (pdProgDlg->wasCanceled())
    {
        // free memory
        for( int i = 0; i < numLprimers; i++ ){
                for( int j = 0; j < numTarSeqs; j++ )
                        delete[] lPrBindPosTarSeqs[i][j];
                for( int j = 0; j < numNontarSeqs; j++ )
                        delete[] lPrBindPosNonTarSeqs[i][j];
                delete[] lPrBindPosTarSeqs[i];
                delete[] lPrBindPosNonTarSeqs[i];
        }
        for( int i = 0; i < numRprimers; i++ ){
                for( int j = 0; j < numTarSeqs; j++ )
                        delete[] rPrBindPosTarSeqs[i][j];
                for( int j = 0; j < numNontarSeqs; j++ )
                        delete[] rPrBindPosNonTarSeqs[i][j];
                delete[] rPrBindPosTarSeqs[i];
                delete[] rPrBindPosNonTarSeqs[i];
        }
        delete[] lPrBindPosTarSeqs;
        delete[] lPrBindPosNonTarSeqs;
        delete[] rPrBindPosTarSeqs;
        delete[] rPrBindPosNonTarSeqs;

        return -1;
    }

    // next prepare reversed target and non-target sequences
    // for searching left primers in sequences from 3' to 5'
    for( int i = 0; i < numTarSeqs; i++ ){
            seqLen = 0;
            while( tarSeqs[i][seqLen] >= 0 )
                    seqLen++;
            sequence = new short[seqLen+1];
            for(int j = 0, k = seqLen-1; j < seqLen; j++, k--)
                    sequence[j] = tarSeqs[i][k];
            sequence[seqLen] = -1;
            reversedTarSeqs.push_back(sequence);
    }
    for( int i = 0; i < numNontarSeqs; i++ ){
            seqLen = 0;
            while( nontarSeqs[i][seqLen] >= 0 )
                    seqLen++;
            sequence = new short[seqLen+1];
            for(int j = 0, k = seqLen-1; j < seqLen; j++, k--)
                    sequence[j] = nontarSeqs[i][k];
            sequence[seqLen] = -1;
            reversedNonTarSeqs.push_back(sequence);
    }
    //user abort
    if (pdProgDlg->wasCanceled()){
        // free memory
        for( int i = 0; i < numLprimers; i++ ){
                for( int j = 0; j < numTarSeqs; j++ )
                        delete[] lPrBindPosTarSeqs[i][j];
                for( int j = 0; j < numNontarSeqs; j++ )
                        delete[] lPrBindPosNonTarSeqs[i][j];
                delete[] lPrBindPosTarSeqs[i];
                delete[] lPrBindPosNonTarSeqs[i];
        }
        for( int i = 0; i < numRprimers; i++ ){
                for( int j = 0; j < numTarSeqs; j++ )
                        delete[] rPrBindPosTarSeqs[i][j];
                for( int j = 0; j < numNontarSeqs; j++ )
                        delete[] rPrBindPosNonTarSeqs[i][j];
                delete[] rPrBindPosTarSeqs[i];
                delete[] rPrBindPosNonTarSeqs[i];
        }
        delete[] lPrBindPosTarSeqs;
        delete[] lPrBindPosNonTarSeqs;
        delete[] rPrBindPosTarSeqs;
        delete[] rPrBindPosNonTarSeqs;
        for( int i = 0; i < numTarSeqs; i++ )
                delete[] reversedTarSeqs[i];
        for( int i = 0; i < numNontarSeqs; i++ )
                delete[] reversedNonTarSeqs[i];
        reversedTarSeqs.clear();
        reversedNonTarSeqs.clear();

        return -1;
    }


    // finally start approxiation search: group primers with common prefix
    // do the search for a group

    PrimerQuickSearch* ptr = new PrimerQuickSearch();

    QTime time = QTime::currentTime();
    timeStr = time.toString();
    logPrimerDesign += timeStr;
    logPrimerDesign += ":	Start searching for (single) primers in target sequences.\r\n";

    // 1. left primer candidates in target sequences==========================================================
    pdProgDlg->setRange(1, numLprimers + numRprimers);
    pdProgDlg->computation(3);
    pr1st = 0;	prlast = 1;
    while( prlast < numLprimers ){
            if( isPrefixLeftPrimer( lPrimerCandid[prlast-1], lPrimerCandid[prlast] ) )
                    prlast++;
            else{
                    prNum = prlast - pr1st;
                    // get a group
                    for( int i = pr1st, j = 0; i < prlast; i++, j++ ){
                            prLen = 0;
                            while( lPrimerCandid[i][prLen] >= 0 )
                                    prLen++;
                            primerGroupLens[j] = prLen;
                    }
                    ptr->initMember( lPrimerCandid[prlast-1], prNum, primerGroupLens, LEFT_PRIMER );
                    // start searching
                    for( int i = 0; i < numTarSeqs; i++ ){
                            ptr->searchPrimersInTarSeq( reversedTarSeqs[i] );
                            for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
                                    lPrBindPosTarSeqs[j][i][0] = ptr->bindingPosStart[k];
                                    lPrBindPosTarSeqs[j][i][1] = ptr->bindingPosEnd[k];
                            }
                    }
                    pr1st = prlast++;

                    pdProgDlg->setValue(prlast);
                    //user abort


                    if (pdProgDlg->wasCanceled()){
                        // free memory
                        for( int i = 0; i < numLprimers; i++ ){
                                for( int j = 0; j < numTarSeqs; j++ )
                                        delete[] lPrBindPosTarSeqs[i][j];
                                for( int j = 0; j < numNontarSeqs; j++ )
                                        delete[] lPrBindPosNonTarSeqs[i][j];
                                delete[] lPrBindPosTarSeqs[i];
                                delete[] lPrBindPosNonTarSeqs[i];
                        }
                        for( int i = 0; i < numRprimers; i++ ){
                                for( int j = 0; j < numTarSeqs; j++ )
                                        delete[] rPrBindPosTarSeqs[i][j];
                                for( int j = 0; j < numNontarSeqs; j++ )
                                        delete[] rPrBindPosNonTarSeqs[i][j];
                                delete[] rPrBindPosTarSeqs[i];
                                delete[] rPrBindPosNonTarSeqs[i];
                        }
                        delete[] lPrBindPosTarSeqs;
                        delete[] lPrBindPosNonTarSeqs;
                        delete[] rPrBindPosTarSeqs;
                        delete[] rPrBindPosNonTarSeqs;
                        for( int i = 0; i < numTarSeqs; i++ )
                                delete[] reversedTarSeqs[i];
                        for( int i = 0; i < numNontarSeqs; i++ )
                                delete[] reversedNonTarSeqs[i];
                        reversedTarSeqs.clear();
                        reversedNonTarSeqs.clear();

                        return -1;
                    }
            }
    }
    prNum = prlast - pr1st;
    for( int i = pr1st, j = 0; i < prlast; i++, j++ ){
            prLen = 0;
            while( lPrimerCandid[i][prLen] >= 0 )
                    prLen++;
            primerGroupLens[j] = prLen;
    }
    ptr->initMember( lPrimerCandid[prlast-1], prNum, primerGroupLens, LEFT_PRIMER );
    // start searching
    for( int i = 0; i < numTarSeqs; i++ ){
            ptr->searchPrimersInTarSeq( reversedTarSeqs[i] );
            for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
                    lPrBindPosTarSeqs[j][i][0] = ptr->bindingPosStart[k];
                    lPrBindPosTarSeqs[j][i][1] = ptr->bindingPosEnd[k];
            }
    }
    pdProgDlg->setValue(prlast);
    //user abort

    if (pdProgDlg->wasCanceled()){
        // free memory
        for( int i = 0; i < numLprimers; i++ ){
                for( int j = 0; j < numTarSeqs; j++ )
                        delete[] lPrBindPosTarSeqs[i][j];
                for( int j = 0; j < numNontarSeqs; j++ )
                        delete[] lPrBindPosNonTarSeqs[i][j];
                delete[] lPrBindPosTarSeqs[i];
                delete[] lPrBindPosNonTarSeqs[i];
        }
        for( int i = 0; i < numRprimers; i++ ){
                for( int j = 0; j < numTarSeqs; j++ )
                        delete[] rPrBindPosTarSeqs[i][j];
                for( int j = 0; j < numNontarSeqs; j++ )
                        delete[] rPrBindPosNonTarSeqs[i][j];
                delete[] rPrBindPosTarSeqs[i];
                delete[] rPrBindPosNonTarSeqs[i];
        }
        delete[] lPrBindPosTarSeqs;
        delete[] lPrBindPosNonTarSeqs;
        delete[] rPrBindPosTarSeqs;
        delete[] rPrBindPosNonTarSeqs;
        for( int i = 0; i < numTarSeqs; i++ )
                delete[] reversedTarSeqs[i];
        for( int i = 0; i < numNontarSeqs; i++ )
                delete[] reversedNonTarSeqs[i];
        reversedTarSeqs.clear();
        reversedNonTarSeqs.clear();

        return -1;
    }

    // 2. right primer candidates in target sequences==========================================================
    pr1st = 0;	prlast = 1;
    while( prlast < numRprimers ){
            if( isPrefixRightPrimer( rPrimerCandid[prlast-1], rPrimerCandid[prlast] ) )
                    prlast++;
            else{
                    prNum = prlast - pr1st;
                    // get a group
                    for( int i = pr1st, j = 0; i < prlast; i++, j++ ){
                            prLen = 0;
                            while( rPrimerCandid[i][prLen] >= 0 )
                                    prLen++;
                            primerGroupLens[j] = prLen;
                    }
                    ptr->initMember( rPrimerCandid[prlast-1], prNum, primerGroupLens, RIGHT_PRIMER );
                    // start searching
                    for( int i = 0; i < numTarSeqs; i++ ){
                            ptr->searchPrimersInTarSeq( tarSeqs[i] );
                            for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
                                    rPrBindPosTarSeqs[j][i][0] = ptr->bindingPosStart[k];
                                    rPrBindPosTarSeqs[j][i][1] = ptr->bindingPosEnd[k];
                            }
                    }
                    pr1st = prlast++;

                    pdProgDlg->setValue(prlast+numLprimers);
                    //user abort
                    if (pdProgDlg->wasCanceled()){
                        // free memory
                        for( int i = 0; i < numLprimers; i++ ){
                                for( int j = 0; j < numTarSeqs; j++ )
                                        delete[] lPrBindPosTarSeqs[i][j];
                                for( int j = 0; j < numNontarSeqs; j++ )
                                        delete[] lPrBindPosNonTarSeqs[i][j];
                                delete[] lPrBindPosTarSeqs[i];
                                delete[] lPrBindPosNonTarSeqs[i];
                        }
                        for( int i = 0; i < numRprimers; i++ ){
                                for( int j = 0; j < numTarSeqs; j++ )
                                        delete[] rPrBindPosTarSeqs[i][j];
                                for( int j = 0; j < numNontarSeqs; j++ )
                                        delete[] rPrBindPosNonTarSeqs[i][j];
                                delete[] rPrBindPosTarSeqs[i];
                                delete[] rPrBindPosNonTarSeqs[i];
                        }
                        delete[] lPrBindPosTarSeqs;
                        delete[] lPrBindPosNonTarSeqs;
                        delete[] rPrBindPosTarSeqs;
                        delete[] rPrBindPosNonTarSeqs;
                        for( int i = 0; i < numTarSeqs; i++ )
                                delete[] reversedTarSeqs[i];
                        for( int i = 0; i < numNontarSeqs; i++ )
                                delete[] reversedNonTarSeqs[i];
                        reversedTarSeqs.clear();
                        reversedNonTarSeqs.clear();

                        return -1;
                    }
            }
    }
    prNum = prlast - pr1st;
    for( int i = pr1st, j = 0; i < prlast; i++, j++ ){
            prLen = 0;
            while( rPrimerCandid[i][prLen] >= 0 )
                    prLen++;
            primerGroupLens[j] = prLen;
    }
    ptr->initMember( rPrimerCandid[prlast-1], prNum, primerGroupLens, RIGHT_PRIMER );
    // start searching
    for( int i = 0; i < numTarSeqs; i++ ){
            ptr->searchPrimersInTarSeq( tarSeqs[i] );
            for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
                    rPrBindPosTarSeqs[j][i][0] = ptr->bindingPosStart[k];
                    rPrBindPosTarSeqs[j][i][1] = ptr->bindingPosEnd[k];
            }
    }
    pdProgDlg->setValue(prlast+numLprimers);
    //user abort

    if (pdProgDlg->wasCanceled()){
        // free memory
        for( int i = 0; i < numLprimers; i++ ){
                for( int j = 0; j < numTarSeqs; j++ )
                        delete[] lPrBindPosTarSeqs[i][j];
                for( int j = 0; j < numNontarSeqs; j++ )
                        delete[] lPrBindPosNonTarSeqs[i][j];
                delete[] lPrBindPosTarSeqs[i];
                delete[] lPrBindPosNonTarSeqs[i];
        }
        for( int i = 0; i < numRprimers; i++ ){
                for( int j = 0; j < numTarSeqs; j++ )
                        delete[] rPrBindPosTarSeqs[i][j];
                for( int j = 0; j < numNontarSeqs; j++ )
                        delete[] rPrBindPosNonTarSeqs[i][j];
                delete[] rPrBindPosTarSeqs[i];
                delete[] rPrBindPosNonTarSeqs[i];
        }
        delete[] lPrBindPosTarSeqs;
        delete[] lPrBindPosNonTarSeqs;
        delete[] rPrBindPosTarSeqs;
        delete[] rPrBindPosNonTarSeqs;
        for( int i = 0; i < numTarSeqs; i++ )
                delete[] reversedTarSeqs[i];
        for( int i = 0; i < numNontarSeqs; i++ )
                delete[] reversedNonTarSeqs[i];
        reversedTarSeqs.clear();
        reversedNonTarSeqs.clear();

        return -1;
    }



    time = QTime::currentTime();
    timeStr = time.toString();
    logPrimerDesign += timeStr;
    logPrimerDesign += ":	Start searching for (single) primers in non-target sequences.\r\n";

    // 3. left primer candidates in non-target sequences=======================================================
    pdProgDlg->setRange(1, numLprimers + numRprimers);
    pdProgDlg->computation(4);

    pr1st = 0;	prlast = 1;
    while( prlast < numLprimers ){
            if( isPrefixLeftPrimer( lPrimerCandid[prlast-1], lPrimerCandid[prlast] ) )
                    prlast++;
            else{
                    prNum = prlast - pr1st;
                    // get a group
                    for( int i = pr1st, j = 0; i < prlast; i++, j++ ){
                            prLen = 0;
                            while( lPrimerCandid[i][prLen] >= 0 )
                                    prLen++;
                            primerGroupLens[j] = prLen;
                    }
                    ptr->initMember( lPrimerCandid[prlast-1], prNum, primerGroupLens, LEFT_PRIMER );
                    // start searching
                    for( int i = 0; i < numNontarSeqs; i++ ){
                            ptr->searchPrimersInNonTarSeq( reversedNonTarSeqs[i] );
                            for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
                                    lPrBindPosNonTarSeqs[j][i][0] = ptr->bindingPosStart[k];
                                    lPrBindPosNonTarSeqs[j][i][1] = ptr->bindingPosEnd[k];
                            }
                    }
                    pr1st = prlast++;

                    pdProgDlg->setValue(prlast);
                    //user abort
                    if (pdProgDlg->wasCanceled()){
                        // free memory
                        for( int i = 0; i < numLprimers; i++ ){
                                for( int j = 0; j < numTarSeqs; j++ )
                                        delete[] lPrBindPosTarSeqs[i][j];
                                for( int j = 0; j < numNontarSeqs; j++ )
                                        delete[] lPrBindPosNonTarSeqs[i][j];
                                delete[] lPrBindPosTarSeqs[i];
                                delete[] lPrBindPosNonTarSeqs[i];
                        }
                        for( int i = 0; i < numRprimers; i++ ){
                                for( int j = 0; j < numTarSeqs; j++ )
                                        delete[] rPrBindPosTarSeqs[i][j];
                                for( int j = 0; j < numNontarSeqs; j++ )
                                        delete[] rPrBindPosNonTarSeqs[i][j];
                                delete[] rPrBindPosTarSeqs[i];
                                delete[] rPrBindPosNonTarSeqs[i];
                        }
                        delete[] lPrBindPosTarSeqs;
                        delete[] lPrBindPosNonTarSeqs;
                        delete[] rPrBindPosTarSeqs;
                        delete[] rPrBindPosNonTarSeqs;
                        for( int i = 0; i < numTarSeqs; i++ )
                                delete[] reversedTarSeqs[i];
                        for( int i = 0; i < numNontarSeqs; i++ )
                                delete[] reversedNonTarSeqs[i];
                        reversedTarSeqs.clear();
                        reversedNonTarSeqs.clear();

                        return -1;
                    }
            }
    }
    prNum = prlast - pr1st;
    for( int i = pr1st, j = 0; i < prlast; i++, j++ ){
            prLen = 0;
            while( lPrimerCandid[i][prLen] >= 0 )
                    prLen++;
            primerGroupLens[j] = prLen;
    }
    ptr->initMember( lPrimerCandid[prlast-1], prNum, primerGroupLens, LEFT_PRIMER );
    // start searching
    for( int i = 0; i < numNontarSeqs; i++ ){
            ptr->searchPrimersInNonTarSeq( reversedNonTarSeqs[i] );
            for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
                    lPrBindPosNonTarSeqs[j][i][0] = ptr->bindingPosStart[k];
                    lPrBindPosNonTarSeqs[j][i][1] = ptr->bindingPosEnd[k];
            }
    }
    pdProgDlg->setValue(prlast);
    //user abort
    if (pdProgDlg->wasCanceled()){
        // free memory
        for( int i = 0; i < numLprimers; i++ ){
                for( int j = 0; j < numTarSeqs; j++ )
                        delete[] lPrBindPosTarSeqs[i][j];
                for( int j = 0; j < numNontarSeqs; j++ )
                        delete[] lPrBindPosNonTarSeqs[i][j];
                delete[] lPrBindPosTarSeqs[i];
                delete[] lPrBindPosNonTarSeqs[i];
        }
        for( int i = 0; i < numRprimers; i++ ){
                for( int j = 0; j < numTarSeqs; j++ )
                        delete[] rPrBindPosTarSeqs[i][j];
                for( int j = 0; j < numNontarSeqs; j++ )
                        delete[] rPrBindPosNonTarSeqs[i][j];
                delete[] rPrBindPosTarSeqs[i];
                delete[] rPrBindPosNonTarSeqs[i];
        }
        delete[] lPrBindPosTarSeqs;
        delete[] lPrBindPosNonTarSeqs;
        delete[] rPrBindPosTarSeqs;
        delete[] rPrBindPosNonTarSeqs;
        for( int i = 0; i < numTarSeqs; i++ )
                delete[] reversedTarSeqs[i];
        for( int i = 0; i < numNontarSeqs; i++ )
                delete[] reversedNonTarSeqs[i];
        reversedTarSeqs.clear();
        reversedNonTarSeqs.clear();

        return -1;
    }

    // 4. right primer candidates in non-target sequences======================================================
    pr1st = 0;	prlast = 1;
    while( prlast < numRprimers ){
            if( isPrefixRightPrimer( rPrimerCandid[prlast-1], rPrimerCandid[prlast] ) )
                    prlast++;
            else{
                    prNum = prlast - pr1st;
                    // get a group
                    for( int i = pr1st, j = 0; i < prlast; i++, j++ ){
                            prLen = 0;
                            while( rPrimerCandid[i][prLen] >= 0 )
                                    prLen++;
                            primerGroupLens[j] = prLen;
                    }
                    ptr->initMember( rPrimerCandid[prlast-1], prNum, primerGroupLens, RIGHT_PRIMER );
                    // start searching
                    for( int i = 0; i < numNontarSeqs; i++ ){
                            ptr->searchPrimersInNonTarSeq( nontarSeqs[i] );
                            for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
                                    rPrBindPosNonTarSeqs[j][i][0] = ptr->bindingPosStart[k];
                                    rPrBindPosNonTarSeqs[j][i][1] = ptr->bindingPosEnd[k];
                            }
                    }
                    pr1st = prlast++;;
                    pdProgDlg->setValue(prlast+numLprimers);
                    //user abort
                    if (pdProgDlg->wasCanceled()){
                        // free memory
                        for( int i = 0; i < numLprimers; i++ ){
                                for( int j = 0; j < numTarSeqs; j++ )
                                        delete[] lPrBindPosTarSeqs[i][j];
                                for( int j = 0; j < numNontarSeqs; j++ )
                                        delete[] lPrBindPosNonTarSeqs[i][j];
                                delete[] lPrBindPosTarSeqs[i];
                                delete[] lPrBindPosNonTarSeqs[i];
                        }
                        for( int i = 0; i < numRprimers; i++ ){
                                for( int j = 0; j < numTarSeqs; j++ )
                                        delete[] rPrBindPosTarSeqs[i][j];
                                for( int j = 0; j < numNontarSeqs; j++ )
                                        delete[] rPrBindPosNonTarSeqs[i][j];
                                delete[] rPrBindPosTarSeqs[i];
                                delete[] rPrBindPosNonTarSeqs[i];
                        }
                        delete[] lPrBindPosTarSeqs;
                        delete[] lPrBindPosNonTarSeqs;
                        delete[] rPrBindPosTarSeqs;
                        delete[] rPrBindPosNonTarSeqs;
                        for( int i = 0; i < numTarSeqs; i++ )
                                delete[] reversedTarSeqs[i];
                        for( int i = 0; i < numNontarSeqs; i++ )
                                delete[] reversedNonTarSeqs[i];
                        reversedTarSeqs.clear();
                        reversedNonTarSeqs.clear();

                        return -1;
                    }
            }
    }
    prNum = prlast - pr1st;
    for( int i = pr1st, j = 0; i < prlast; i++, j++ ){
            prLen = 0;
            while( rPrimerCandid[i][prLen] >= 0 )
                    prLen++;
            primerGroupLens[j] = prLen;
    }
    ptr->initMember( rPrimerCandid[prlast-1], prNum, primerGroupLens, RIGHT_PRIMER );
    // start searching
    for( int i = 0; i < numNontarSeqs; i++ ){
            ptr->searchPrimersInNonTarSeq( nontarSeqs[i] );
            for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
                    rPrBindPosNonTarSeqs[j][i][0] = ptr->bindingPosStart[k];
                    rPrBindPosNonTarSeqs[j][i][1] = ptr->bindingPosEnd[k];
            }
    }
    pdProgDlg->setValue(prlast+numLprimers);
    //user abort
    if (pdProgDlg->wasCanceled()){
        for( int i = 0; i < numLprimers; i++ ){
                for( int j = 0; j < numTarSeqs; j++ )
                        delete[] lPrBindPosTarSeqs[i][j];
                for( int j = 0; j < numNontarSeqs; j++ )
                        delete[] lPrBindPosNonTarSeqs[i][j];
                delete[] lPrBindPosTarSeqs[i];
                delete[] lPrBindPosNonTarSeqs[i];
        }
        for( int i = 0; i < numRprimers; i++ ){
                for( int j = 0; j < numTarSeqs; j++ )
                        delete[] rPrBindPosTarSeqs[i][j];
                for( int j = 0; j < numNontarSeqs; j++ )
                        delete[] rPrBindPosNonTarSeqs[i][j];
                delete[] rPrBindPosTarSeqs[i];
                delete[] rPrBindPosNonTarSeqs[i];
        }
        delete[] lPrBindPosTarSeqs;
        delete[] lPrBindPosNonTarSeqs;
        delete[] rPrBindPosTarSeqs;
        delete[] rPrBindPosNonTarSeqs;
        for( int i = 0; i < numTarSeqs; i++ )
                delete[] reversedTarSeqs[i];
        for( int i = 0; i < numNontarSeqs; i++ )
                delete[] reversedNonTarSeqs[i];
        reversedTarSeqs.clear();
        reversedNonTarSeqs.clear();

        return -1;
    }

    delete ptr;


    //short ***lPrBindPosTarSeqs, ***lPrBindPosNonTarSeqs;
    short ***rPrBindPosTarSeqs, ***rPrBindPosNonTarSeqs;
    QVector<float> lPrPctgTarSeqs, lPrPctgNonTarSeqs;
    QVector<float> rPrPctgTarSeqs, rPrPctgNonTarSeqs;
    QVector<short*> reversedTarSeqs;
    QVector<short*> reversedNonTarSeqs;
    int numLprimers = (int)lPrimerCandid.size();
    int numRprimers = (int)rPrimerCandid.size();
    int numTarSeqs = (int)tarSeqs.size();
    int numNontarSeqs = (int)nontarSeqs.size();

    QString timeStr;
    QString str;

    // 1.allocate memory for primer annealing position variables
    if(allocAnnealingPosVar(lPrBindPosTarSeqs, lPrBindPosNonTarSeqs,true) != 0)
        return FATAL_ERROR;
    if(allocAnnealingPosVar(rPrBindPosTarSeqs,rPrBindPosNonTarSeqs,false) != 0)
        return FATAL_ERROR;

    //user abort
    if (pdProgDlg->wasCanceled()){
        // free memory
        freeAnnealingPosVar(lPrBindPosTarSeqs, lPrBindPosNonTarSeqs,true);
        freeAnnealingPosVar(rPrBindPosTarSeqs,rPrBindPosNonTarSeqs, false);
        return -1;
    }

    // 2.prepare reversed target and non-target sequences
    // for searching left primers in sequences from 3' to 5'
    this->prepareReversedSeqs(reversedTarSeqs,reversedNonTarSeqs);
    //user abort
    if (pdProgDlg->wasCanceled()){
        // free memory
        freeAnnealingPosVar(lPrBindPosTarSeqs,lPrBindPosNonTarSeqs,true);
        freeAnnealingPosVar(rPrBindPosTarSeqs,rPrBindPosNonTarSeqs, false);
        freeSeqs(reversedTarSeqs, reversedNonTarSeqs);
        return -1;
    }

    // 3.start approxiation search: group primers with common prefix
    // do the search for a group
    PrimerQuickSearch* ptr = new PrimerQuickSearch();

    QTime time = QTime::currentTime();
    timeStr = time.toString();
    logPrimerDesign += timeStr;
    logPrimerDesign += ":	Start searching for (single) primers in target sequences.\r\n";
    pdProgDlg->setRange(1, numLprimers + numRprimers);
    pdProgDlg->computation(3);
    this->quickSearchPrimerInSeqs(ptr,
                                  lPrimerCandid,reversedTarSeqs, LEFT_PRIMER, 0,
                                  lPrBindPosTarSeqs, lPrBindPosNonTarSeqs,
                                  rPrBindPosTarSeqs, rPrBindPosNonTarSeqs,
                                  reversedTarSeqs,reversedNonTarSeqs);
    this->quickSearchPrimerInSeqs(ptr,
                                  rPrimerCandid,tarSeqs, RIGHT_PRIMER, 0,
                                  lPrBindPosTarSeqs, lPrBindPosNonTarSeqs,
                                  rPrBindPosTarSeqs, rPrBindPosNonTarSeqs,
                                  reversedTarSeqs,reversedNonTarSeqs);

    time = QTime::currentTime();
    timeStr = time.toString();
    logPrimerDesign += timeStr;
    logPrimerDesign += ":	Start searching for (single) primers in non-target sequences.\r\n";
    pdProgDlg->setRange(1, numLprimers + numRprimers);
    pdProgDlg->computation(4);

    this->quickSearchPrimerInSeqs(ptr,
                                  lPrimerCandid,reversedNonTarSeqs, LEFT_PRIMER, 1,
                                  lPrBindPosTarSeqs, lPrBindPosNonTarSeqs,
                                  rPrBindPosTarSeqs, rPrBindPosNonTarSeqs,
                                  reversedTarSeqs,reversedNonTarSeqs);
    this->quickSearchPrimerInSeqs(ptr,
                                  rPrimerCandid,nontarSeqs, RIGHT_PRIMER, 1,
                                  lPrBindPosTarSeqs, lPrBindPosNonTarSeqs,
                                  rPrBindPosTarSeqs, rPrBindPosNonTarSeqs,
                                  reversedTarSeqs,reversedNonTarSeqs);
    delete ptr;*/

    // finish searching========================================================================================
    // prepare frequency for single primer in target and non-target sequences
    /*float occ;
    for( int i = 0; i < numLprimers; i++ ){
        occ = 0;
        for( int j = 0; j < numTarSeqs; j++ ){
            if( lPrBindPosTarSeqs[i][j][0] >= 0 )
                occ++;
        }
        lPrPctgTarSeqs.push_back( occ/numTarSeqs * 100 );
        occ = 0;
        for( int j = 0; j < numNontarSeqs; j++ ){
            if( lPrBindPosNonTarSeqs[i][j][0] >= 0 )
                 occ++;
        }
        lPrPctgNonTarSeqs.push_back( occ/numNontarSeqs * 100 );
    }


    for( int i = 0; i < numRprimers; i++ ){
        occ = 0;
        for( int j = 0; j < numTarSeqs; j++ ){
            if( rPrBindPosTarSeqs[i][j][0] >= 0 )
                occ++;
        }
        rPrPctgTarSeqs.push_back( occ/numTarSeqs * 100 );
        occ = 0;
        for( int j = 0; j < numNontarSeqs; j++ ){
            if( rPrBindPosNonTarSeqs[i][j][0] >= 0 )
                 occ++;
        }
        rPrPctgNonTarSeqs.push_back( occ/numNontarSeqs * 100 );
    }
    pdProgDlg->setValue(0);

    //user abort
    if (pdProgDlg->wasCanceled()){
        for( int i = 0; i < numLprimers; i++ ){
        for( int j = 0; j < numTarSeqs; j++ )
            delete[] lPrBindPosTarSeqs[i][j];
        for( int j = 0; j < numNontarSeqs; j++ )
            delete[] lPrBindPosNonTarSeqs[i][j];
        delete[] lPrBindPosTarSeqs[i];
        delete[] lPrBindPosNonTarSeqs[i];
        }
        for( int i = 0; i < numRprimers; i++ ){
            for( int j = 0; j < numTarSeqs; j++ )
                delete[] rPrBindPosTarSeqs[i][j];
            for( int j = 0; j < numNontarSeqs; j++ )
                delete[] rPrBindPosNonTarSeqs[i][j];
            delete[] rPrBindPosTarSeqs[i];
            delete[] rPrBindPosNonTarSeqs[i];
        }
        delete[] lPrBindPosTarSeqs;
        delete[] lPrBindPosNonTarSeqs;
        delete[] rPrBindPosTarSeqs;
        delete[] rPrBindPosNonTarSeqs;
        for( int i = 0; i < numTarSeqs; i++ )
            delete[] reversedTarSeqs[i];
        for( int i = 0; i < numNontarSeqs; i++ )
             delete[] reversedNonTarSeqs[i];
        reversedTarSeqs.clear();
        reversedNonTarSeqs.clear();
        lPrPctgTarSeqs.clear();
        rPrPctgTarSeqs.clear();
        lPrPctgNonTarSeqs.clear();
        rPrPctgNonTarSeqs.clear();

        return -1;
    }


    // free memory for reversed sequences
    for( int i = 0; i < numTarSeqs; i++ )
            delete[] reversedTarSeqs[i];
    for( int i = 0; i < numNontarSeqs; i++ )
            delete[] reversedNonTarSeqs[i];
    reversedTarSeqs.clear();
    reversedNonTarSeqs.clear();


    // pair off primer candidates=============================================================================
    int num;
    float numDouble;
    int maxProductSize;
    int minProductSize;
    int sumProductSize;
    int productSize;
    int bdPos[4];
    int nPairsLeft[5];
    // fill searching result in primer list
    pdProgDlg->computation(5);
    pdProgDlg->setRange(1, numLprimers);
    int numPrimerPairList = primerPairList.size();
    PrimerPair *pair;
    QVector<int> randSelSeqs;

    // first prepare average annealing position for all candidate in target and non-target sequences.
    // this is for later, pair each left with right primer, if left average annealing position appear
    // to more right to the right primer, don't consider this pair
    float* lPrBindPosTarSeqsAvg;
    float* rPrBindPosTarSeqsAvg;
    try{
        lPrBindPosTarSeqsAvg = new float[numLprimers];
        rPrBindPosTarSeqsAvg = new float[numRprimers];
    }
    catch(  std::bad_alloc& e ){
        QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");


        DataTools::cleanMemorySeqs();
        // free memory
        for( int i = 0; i < numLprimers; i++ ){
            for( int j = 0; j < numTarSeqs; j++ )
                delete[] lPrBindPosTarSeqs[i][j];
            for( int j = 0; j < numNontarSeqs; j++ )
                delete[] lPrBindPosNonTarSeqs[i][j];
            delete[] lPrBindPosTarSeqs[i];
            delete[] lPrBindPosNonTarSeqs[i];
        }
        for( int i = 0; i < numRprimers; i++ ){
            for( int j = 0; j < numTarSeqs; j++ )
                delete[] rPrBindPosTarSeqs[i][j];
            for( int j = 0; j < numNontarSeqs; j++ )
                delete[] rPrBindPosNonTarSeqs[i][j];
            delete[] rPrBindPosTarSeqs[i];
            delete[] rPrBindPosNonTarSeqs[i];
        }
        delete[] lPrBindPosTarSeqs;
        delete[] lPrBindPosNonTarSeqs;
        delete[] rPrBindPosTarSeqs;
        delete[] rPrBindPosNonTarSeqs;
        lPrPctgTarSeqs.clear();
        rPrPctgTarSeqs.clear();
        lPrPctgNonTarSeqs.clear();
        rPrPctgNonTarSeqs.clear();

        return -1;
    }

    for( int i = 0; i < numLprimers; i++ ){
        numDouble = 0; num = 0;
        for( int j = 0; j < numTarSeqs; j++ ){
            if( lPrBindPosTarSeqs[i][j][0] >= 0 ){
                num++;
                numDouble += lPrBindPosTarSeqs[i][j][1];  // left primer check ending position
            }
        }
        lPrBindPosTarSeqsAvg[i] = numDouble / num;
    }
    for( int i = 0; i < numRprimers; i++ ){
        numDouble = 0;	num = 0;
        for( int j = 0; j < numTarSeqs; j++ ){
            if( rPrBindPosTarSeqs[i][j][0] >= 0 ){
                num++;
                numDouble += rPrBindPosTarSeqs[i][j][0];  // right primer check starting position
            }
        }
        rPrBindPosTarSeqsAvg[i] = numDouble / num;
    }



    // for information of primer design report window
    for( int i = 0; i < 5; i++ )
        nPairsLeft[i] = 0;

    time = QTime::currentTime();
    timeStr = time.toString();
    logPrimerDesign += timeStr;

    // pair each left primer with each right primer in all possible ways and check if this pair
    // is acceptable

    if( estimate ){	// compute estimated PCR information
        for( int i = 0; i < numLprimers; i++ ){
            for( int j = 0; j < numRprimers; j++ ){

                // first check if this primer pair is possible to be a pair
                // skip if left primer ending position is on the right of right primer starting postion
                if( lPrBindPosTarSeqsAvg[i] >= rPrBindPosTarSeqsAvg[j] )
                    continue;
                try{
                    pair = new PrimerPair();
                }
                catch( std::bad_alloc& e ){
                    QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory.\nThe possible solutions:\n"
                                       "1. Install more physical memory.\n2. Close all other applications.\n"
                                       "3. Assign stricter primer settings.\n"
                                       "4. Compute primers for a single length at a time.\n"
                                       "Program is forced to terminate.");
                    // free memory
                    for( int i = 0; i < numLprimers; i++ ){
                            for( int j = 0; j < numTarSeqs; j++ )
                                delete[] lPrBindPosTarSeqs[i][j];
                            for( int j = 0; j < numNontarSeqs; j++ )
                                delete[] lPrBindPosNonTarSeqs[i][j];
                            delete[] lPrBindPosTarSeqs[i];
                            delete[] lPrBindPosNonTarSeqs[i];
                    }
                    for( int i = 0; i < numRprimers; i++ ){
                            for( int j = 0; j < numTarSeqs; j++ )
                                delete[] rPrBindPosTarSeqs[i][j];
                            for( int j = 0; j < numNontarSeqs; j++ )
                                delete[] rPrBindPosNonTarSeqs[i][j];
                            delete[] rPrBindPosTarSeqs[i];
                            delete[] rPrBindPosNonTarSeqs[i];
                    }
                    delete[] lPrBindPosTarSeqs;
                    delete[] lPrBindPosNonTarSeqs;
                    delete[] rPrBindPosTarSeqs;
                    delete[] rPrBindPosNonTarSeqs;
                    delete[] lPrBindPosTarSeqsAvg;
                    delete[] rPrBindPosTarSeqsAvg;
                    lPrPctgTarSeqs.clear();
                    rPrPctgTarSeqs.clear();
                    lPrPctgNonTarSeqs.clear();
                    rPrPctgNonTarSeqs.clear();

                    return FATAL_ERROR;
                }

                tools::seqInt2Str(lPrimerCandid[i], pair->left);
                tools::seqInt2Str(rPrimerCandid[j], pair->right);
                if( pair->left == pair->right ){
                    delete pair;
                    continue;
                }
                tools::seqInt2StrReverseComplement(rPrimerCandid[j],pair->rightRevsComp);

                // compute frequencies for primer pair in target sequences
                // only pick 100 random target sequences to consider
                rand100position( numTarSeqs, randSelSeqs );
                if( (int)randSelSeqs.size() < numTarSeqs )
                    pair->isEstimateTar = true;
                else
                    pair->isEstimateTar = false;
                num = 0;
                maxProductSize = 0;	minProductSize = INT_MAX;	sumProductSize = 0;
                for( unsigned k = 0; k < randSelSeqs.size(); k++ ){

                    bdPos[0] = lPrBindPosTarSeqs[i][randSelSeqs[k]][0];
                    bdPos[1] = lPrBindPosTarSeqs[i][randSelSeqs[k]][1];
                    bdPos[2] = rPrBindPosTarSeqs[j][randSelSeqs[k]][0];
                    bdPos[3] = rPrBindPosTarSeqs[j][randSelSeqs[k]][1];

                    // left and right primer both appear and positions are correct
                    if( bdPos[0] >= 0 && bdPos[2] >=0 && bdPos[1] < bdPos[2] ){
                        num++;
                        productSize = bdPos[3] - bdPos[0] + 1;
                        if( minProductSize > productSize )	minProductSize = productSize;
                        if( maxProductSize < productSize )	maxProductSize = productSize;
                        sumProductSize += productSize;
                    }
                }
                if( randSelSeqs.size() != 0 )
                    pair->pctgInTargetSeqs = (float)num / randSelSeqs.size() * 100;
                else
                    pair->pctgInTargetSeqs = 0;
                if( num == 0 ){
                    pair->pcrSizeAvgTarSeqs = 0;
                    pair->pcrSizeMinTarSeqs = 0;
                    pair->pcrSizeMaxTarSeqs = 0;
                }
                else{
                    pair->pcrSizeAvgTarSeqs = (float)sumProductSize / num;
                    pair->pcrSizeMinTarSeqs = minProductSize;
                    pair->pcrSizeMaxTarSeqs = maxProductSize;
                }

                // compute frequencies for primer pair in non-target sequences
                // only pick 100 random non-target sequences to consider
                rand100position( numNontarSeqs, randSelSeqs );
                if( (int)randSelSeqs.size() < numNontarSeqs )
                    pair->isEstimateNonTar = 1;
                else
                    pair->isEstimateNonTar = 0;
                num = 0;
                maxProductSize = 0;	minProductSize = INT_MAX;	sumProductSize = 0;
                for( unsigned k = 0; k < randSelSeqs.size(); k++ ){

                    bdPos[0] = lPrBindPosNonTarSeqs[i][randSelSeqs[k]][0];
                    bdPos[1] = lPrBindPosNonTarSeqs[i][randSelSeqs[k]][1];
                    bdPos[2] = rPrBindPosNonTarSeqs[j][randSelSeqs[k]][0];
                    bdPos[3] = rPrBindPosNonTarSeqs[j][randSelSeqs[k]][1];

                    // left and right primer both appear and positions are correct
                    if( bdPos[0] >= 0 && bdPos[2] >=0 && bdPos[1] < bdPos[2] ){
                        num++;
                        productSize = bdPos[3] - bdPos[0] + 1;
                        if( minProductSize > productSize )	minProductSize = productSize;
                        if( maxProductSize < productSize )	maxProductSize = productSize;
                        sumProductSize += productSize;
                    }
                }
                if( randSelSeqs.size() != 0 )
                    pair->pctgInNonTarSeqs = (float)num / randSelSeqs.size() * 100;
                else
                    pair->pctgInNonTarSeqs = 0;

                if( num == 0 ){
                    pair->pcrSizeAvgNonTarSeqs = 0;
                    pair->pcrSizeMinNonTarSeqs = 0;
                    pair->pcrSizeMaxNonTarSeqs = 0;
                }
                else{
                    pair->pcrSizeAvgNonTarSeqs = (float)sumProductSize / num;
                    pair->pcrSizeMinNonTarSeqs = minProductSize;
                    pair->pcrSizeMaxNonTarSeqs = maxProductSize;
                }


                if( pair->pcrSizeAvgTarSeqs > 30 )
                    nPairsLeft[0]++;

                // filtering:
                // 1. pcr product size
                if( ( pair->pcrSizeAvgTarSeqs <  pcrSizeMin || pair->pcrSizeAvgTarSeqs > pcrSizeMax ) ){
                        //&& ( pair->pcrSizeAvgNonTarSeqs < pcrSizeMin || pair->pcrSizeAvgNonTarSeqs > pcrSizeMax )  ) {

                    delete pair;
                    continue;
                }
                nPairsLeft[1]++;

                // 2. Tm difference
                pair->tmLeft = meltingTemp(pair->left);
                pair->tmRight = meltingTemp(pair->right);
                numDouble = pair->tmLeft - pair->tmRight;
                numDouble = abs(numDouble);
                if( numDouble > tmDiff ){
                    delete pair;
                    continue;
                }
                nPairsLeft[2]++;

                //3. primer inter-complementarity
                pair->prPrComp = primerInterComplementarity(pair->left, pair->rightRevsComp);
                if( pair->prPrComp > maxPrPrComp ){
                    delete pair;
                    continue;
                }
                nPairsLeft[3]++;

                //4. primer 3' inter-complementarity
                pair->prPr3Comp = primer3InterComplementarity(pair->left, pair->rightRevsComp);
                if( pair->prPr3Comp > max3PrPrComp ){
                    delete pair;
                    continue;
                }
                nPairsLeft[4]++;

                // GC content
                pair->GCLeft = GCcontent(pair->left);
                pair->GCRight = GCcontent(pair->right);

                // primer self-complementarity
                pair->selfCompLeft = primerIntraComplementarity(pair->left);
                pair->selfCompRight = primerIntraComplementarity(pair->rightRevsComp);

                // primer 3' self-complementarity
                pair->self3CompLeft = primer3IntraComplementarity(pair->left);
                pair->self3CompRight = primer3IntraComplementarity(pair->rightRevsComp);

                // sigle primer average annealing positions in target
                // note lPrBindPosTarSeqsAvg[i] saves ending position for left primers,
                // need to change it to starting position for left primers. right primer is fine.
                pair->bindPosTarSeqsAvgLeft = lPrBindPosTarSeqsAvg[i] - pair->left.length() + 1;
                pair->bindPosTarSeqsAvgRight = rPrBindPosTarSeqsAvg[j];

                // single primer frequency in target and non-target sequences
                pair->pctgInTarSeqsLeft = lPrPctgTarSeqs[i];
                pair->pctgInNonTarSeqsLeft = lPrPctgNonTarSeqs[i];
                pair->pctgInTarSeqsRight = rPrPctgTarSeqs[j];
                pair->pctgInNonTarSeqsRight = rPrPctgNonTarSeqs[j];

                pair->marked = 0;

                primerPairList.push_back(pair);
            }
            pdProgDlg->setValue(i+1);
            //user abort
            if (pdProgDlg->wasCanceled()){
                for( int i = 0; i < numLprimers; i++ ){
                    for( int j = 0; j < numTarSeqs; j++ )
                        delete[] lPrBindPosTarSeqs[i][j];
                    for( int j = 0; j < numNontarSeqs; j++ )
                        delete[] lPrBindPosNonTarSeqs[i][j];
                    delete[] lPrBindPosTarSeqs[i];
                    delete[] lPrBindPosNonTarSeqs[i];
                }
                for( int i = 0; i < numRprimers; i++ ){
                    for( int j = 0; j < numTarSeqs; j++ )
                        delete[] rPrBindPosTarSeqs[i][j];
                    for( int j = 0; j < numNontarSeqs; j++ )
                        delete[] rPrBindPosNonTarSeqs[i][j];
                    delete[] rPrBindPosTarSeqs[i];
                    delete[] rPrBindPosNonTarSeqs[i];
                }
                delete[] lPrBindPosTarSeqs;
                delete[] lPrBindPosNonTarSeqs;
                delete[] rPrBindPosTarSeqs;
                delete[] rPrBindPosNonTarSeqs;
                delete[] lPrBindPosTarSeqsAvg;
                delete[] rPrBindPosTarSeqsAvg;
                lPrPctgTarSeqs.clear();
                rPrPctgTarSeqs.clear();
                lPrPctgNonTarSeqs.clear();
                rPrPctgNonTarSeqs.clear();

                return -1;
            }
        }
    }
    else{	// compute exact PCR information
        for( int i = 0; i < numLprimers; i++ ){
            for( int j = 0; j < numRprimers; j++ ){

                // first check if this primer pair is possible to be a pair
                // skip if left primer ending position is on the right of right primer starting postion
                if( lPrBindPosTarSeqsAvg[i] >= rPrBindPosTarSeqsAvg[j] )
                    continue;

                try{
                     pair = new PrimerPair();
                }
                catch( std::bad_alloc& e ){
                    QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory.\nThe possible solutions:\n"
                                       "1. Install more physical memory.\n2. Close all other applications.\n"
                                       "3. Assign stricter primer settings.\n"
                                       "4. Compute primers for a single length at a time.\n"
                                       "Program is forced to terminate.");
                    // free memory
                    for( int i = 0; i < numLprimers; i++ ){
                        for( int j = 0; j < numTarSeqs; j++ )
                            delete[] lPrBindPosTarSeqs[i][j];
                        for( int j = 0; j < numNontarSeqs; j++ )
                            delete[] lPrBindPosNonTarSeqs[i][j];
                        delete[] lPrBindPosTarSeqs[i];
                        delete[] lPrBindPosNonTarSeqs[i];
                    }
                    for( int i = 0; i < numRprimers; i++ ){
                        for( int j = 0; j < numTarSeqs; j++ )
                            delete[] rPrBindPosTarSeqs[i][j];
                        for( int j = 0; j < numNontarSeqs; j++ )
                            delete[] rPrBindPosNonTarSeqs[i][j];
                        delete[] rPrBindPosTarSeqs[i];
                        delete[] rPrBindPosNonTarSeqs[i];
                    }
                    delete[] lPrBindPosTarSeqs;
                    delete[] lPrBindPosNonTarSeqs;
                    delete[] rPrBindPosTarSeqs;
                    delete[] rPrBindPosNonTarSeqs;
                    delete[] lPrBindPosTarSeqsAvg;
                    delete[] rPrBindPosTarSeqsAvg;
                    lPrPctgTarSeqs.clear();
                    rPrPctgTarSeqs.clear();
                    lPrPctgNonTarSeqs.clear();
                    rPrPctgNonTarSeqs.clear();

                    return -1;
                }
                tools::seqInt2Str(lPrimerCandid[i], pair->left);
                tools::seqInt2Str(rPrimerCandid[j], pair->right);
                if( pair->left == pair->right ){
                    delete pair;
                    continue;
                }
                tools::seqInt2StrReverseComplement(rPrimerCandid[j],pair->rightRevsComp);

                // compute frequencies for primer pair in target sequences
                num = 0;
                maxProductSize = 0;	minProductSize = INT_MAX;	sumProductSize = 0;
                for( int k = 0; k < numTarSeqs; k++ ){
                    bdPos[0] = lPrBindPosTarSeqs[i][k][0];
                    bdPos[1] = lPrBindPosTarSeqs[i][k][1];
                    bdPos[2] = rPrBindPosTarSeqs[j][k][0];
                    bdPos[3] = rPrBindPosTarSeqs[j][k][1];

                    // left and right primer both appear and positions are correct
                    if( bdPos[0] >= 0 && bdPos[2] >=0 && bdPos[1] < bdPos[2] ){
                        num++;
                        productSize = bdPos[3] - bdPos[0] + 1;
                        if( minProductSize > productSize )	minProductSize = productSize;
                        if( maxProductSize < productSize )	maxProductSize = productSize;
                        sumProductSize += productSize;
                    }
                }
                if( numTarSeqs != 0 )
                    pair->pctgInTargetSeqs = (float)num / numTarSeqs * 100;
                else
                    pair->pctgInTargetSeqs = 0;
                if( num == 0 ){
                    pair->pcrSizeAvgTarSeqs = 0;
                    pair->pcrSizeMinTarSeqs = 0;
                    pair->pcrSizeMaxTarSeqs = 0;
                }
                else{
                    pair->pcrSizeAvgTarSeqs = (float)sumProductSize / num;
                    pair->pcrSizeMinTarSeqs = minProductSize;
                    pair->pcrSizeMaxTarSeqs = maxProductSize;
                }

                // compute frequencies for primer pair in non-target sequences
                num = 0;
                maxProductSize = 0;	minProductSize = INT_MAX;	sumProductSize = 0;
                for( int k = 0; k < numNontarSeqs; k++ ){
                    bdPos[0] = lPrBindPosNonTarSeqs[i][k][0];
                    bdPos[1] = lPrBindPosNonTarSeqs[i][k][1];
                    bdPos[2] = rPrBindPosNonTarSeqs[j][k][0];
                    bdPos[3] = rPrBindPosNonTarSeqs[j][k][1];

                    // left and right primer both appear and positions are correct
                    if( bdPos[0] >= 0 && bdPos[2] >=0 && bdPos[1] < bdPos[2] ){
                        num++;
                        productSize = bdPos[3] - bdPos[0] + 1;
                        if( minProductSize > productSize )	minProductSize = productSize;
                        if( maxProductSize < productSize )	maxProductSize = productSize;
                        sumProductSize += productSize;
                    }
                }
                if( numNontarSeqs != 0 )
                    pair->pctgInNonTarSeqs = (float)num / numNontarSeqs * 100;
                else
                    pair->pctgInNonTarSeqs = 0;

                if( num == 0 ){
                    pair->pcrSizeAvgNonTarSeqs = 0;
                    pair->pcrSizeMinNonTarSeqs = 0;
                    pair->pcrSizeMaxNonTarSeqs = 0;
                }
                else{
                    pair->pcrSizeAvgNonTarSeqs = (float)sumProductSize / num;
                    pair->pcrSizeMinNonTarSeqs = minProductSize;
                    pair->pcrSizeMaxNonTarSeqs = maxProductSize;
                }


                if( pair->pcrSizeAvgTarSeqs > 30 )
                     nPairsLeft[0]++;

                // filtering:
                // 1. pcr product size
                if( ( pair->pcrSizeAvgTarSeqs < pcrSizeMin || pair->pcrSizeAvgTarSeqs > pcrSizeMax ) ){
                    //&& ( pair->pcrSizeAvgNonTarSeqs < pcrSizeMin || pair->pcrSizeAvgNonTarSeqs > pcrSizeMax )  ) {

                    delete pair;
                    continue;
                }
                nPairsLeft[1]++;

                // 2. Tm difference
                pair->tmLeft = meltingTemp(pair->left);
                pair->tmRight = meltingTemp(pair->right);
                numDouble = pair->tmLeft - pair->tmRight;
                numDouble = abs(numDouble);
                if( numDouble > tmDiff ){
                    delete pair;
                    continue;
                }
                nPairsLeft[2]++;

                //3. primer inter-complementarity
                pair->prPrComp = primerInterComplementarity(pair->left, pair->rightRevsComp);
                if( pair->prPrComp > maxPrPrComp ){
                    delete pair;
                    continue;
                }
                nPairsLeft[3]++;

                //4. primer 3' inter-complementarity
                pair->prPr3Comp = primer3InterComplementarity(pair->left, pair->rightRevsComp);
                if( pair->prPr3Comp > max3PrPrComp ){
                    delete pair;
                    continue;
                }
                nPairsLeft[4]++;

                // GC content
                pair->GCLeft = GCcontent(pair->left);
                pair->GCRight = GCcontent(pair->right);

                // primer self-complementarity
                pair->selfCompLeft = primerIntraComplementarity(pair->left);
                pair->selfCompRight = primerIntraComplementarity(pair->rightRevsComp);

                // primer 3' self-complementarity
                pair->self3CompLeft = primer3IntraComplementarity(pair->left);
                pair->self3CompRight = primer3IntraComplementarity(pair->rightRevsComp);

                // sigle primer average annealing positions in target
                // note lPrBindPosTarSeqsAvg[i] saves ending position for left primers,
                // need to change it to starting position for left primers. right primer is fine.
                pair->bindPosTarSeqsAvgLeft = lPrBindPosTarSeqsAvg[i] - pair->left.length() + 1;
                pair->bindPosTarSeqsAvgRight = rPrBindPosTarSeqsAvg[j];

                // single primer frequency in target and non-target sequences
                pair->pctgInTarSeqsLeft = lPrPctgTarSeqs[i];
                pair->pctgInNonTarSeqsLeft = lPrPctgNonTarSeqs[i];
                pair->pctgInTarSeqsRight = rPrPctgTarSeqs[j];
                pair->pctgInNonTarSeqsRight = rPrPctgNonTarSeqs[j];

                // indicating exact information computed
                pair->isEstimateTar = 0;
                pair->isEstimateNonTar = 0;

                pair->marked = 0;

                primerPairList.push_back(pair);
            }
            pdProgDlg->setValue(i+1);
            //user abort


            if (pdProgDlg->wasCanceled()){
                for( int i = 0; i < numLprimers; i++ ){
                    for( int j = 0; j < numTarSeqs; j++ )
                        delete[] lPrBindPosTarSeqs[i][j];
                    for( int j = 0; j < numNontarSeqs; j++ )
                        delete[] lPrBindPosNonTarSeqs[i][j];
                    delete[] lPrBindPosTarSeqs[i];
                    delete[] lPrBindPosNonTarSeqs[i];
                }
                for( int i = 0; i < numRprimers; i++ ){
                    for( int j = 0; j < numTarSeqs; j++ )
                        delete[] rPrBindPosTarSeqs[i][j];
                    for( int j = 0; j < numNontarSeqs; j++ )
                        delete[] rPrBindPosNonTarSeqs[i][j];
                    delete[] rPrBindPosTarSeqs[i];
                    delete[] rPrBindPosNonTarSeqs[i];
                }
                delete[] lPrBindPosTarSeqs;
                delete[] lPrBindPosNonTarSeqs;
                delete[] rPrBindPosTarSeqs;
                delete[] rPrBindPosNonTarSeqs;
                delete[] lPrBindPosTarSeqsAvg;
                delete[] rPrBindPosTarSeqsAvg;
                lPrPctgTarSeqs.clear();
                rPrPctgTarSeqs.clear();
                lPrPctgNonTarSeqs.clear();
                rPrPctgNonTarSeqs.clear();

                return -1;
            }
        }
    }

    logPrimerDesign += ":	Start pairing off with " + QString("%1").arg(nPairsLeft[0]) + " possible primer pairs.\r\n";
    logPrimerDesign += "\t\tCheck PCR product size, " + QString("%1").arg(nPairsLeft[1]) + " primer pairs left.\r\n";
    logPrimerDesign += "\t\tCheck Tm difference, " + QString("%1").arg( nPairsLeft[2]) + " primer pairs left.\r\n";
    logPrimerDesign += "\t\tCheck inter-complementarity, " + QString("%1").arg(nPairsLeft[3]) + " primer pairs left.\r\n";
    logPrimerDesign += "\t\tCheck 3' inter-complementarity, " + QString("%1").arg( nPairsLeft[4]) + " primer pairs left.\r\n";


    time = QTime::currentTime();
    timeStr = time.toString();
    logPrimerDesign += timeStr;
    logPrimerDesign += ":	Generate primer pair report with " + QString("%1").arg( primerPairList.size()) + " primer pairs.\r\n";
    pdProgDlg->setValue(0);

    // free memory
    for( int i = 0; i < numLprimers; i++ ){
        for( int j = 0; j < numTarSeqs; j++ )
             delete[] lPrBindPosTarSeqs[i][j];
        for( int j = 0; j < numNontarSeqs; j++ )
            delete[] lPrBindPosNonTarSeqs[i][j];
        delete[] lPrBindPosTarSeqs[i];
        delete[] lPrBindPosNonTarSeqs[i];
    }
    for( int i = 0; i < numRprimers; i++ ){
        for( int j = 0; j < numTarSeqs; j++ )
            delete[] rPrBindPosTarSeqs[i][j];
        for( int j = 0; j < numNontarSeqs; j++ )
             delete[] rPrBindPosNonTarSeqs[i][j];
        delete[] rPrBindPosTarSeqs[i];
        delete[] rPrBindPosNonTarSeqs[i];
    }
    delete[] lPrBindPosTarSeqs;
    delete[] lPrBindPosNonTarSeqs;
    delete[] rPrBindPosTarSeqs;
    delete[] rPrBindPosNonTarSeqs;
    delete[] lPrBindPosTarSeqsAvg;
    delete[] rPrBindPosTarSeqsAvg;
    lPrPctgTarSeqs.clear();
    rPrPctgTarSeqs.clear();
    lPrPctgNonTarSeqs.clear();
    rPrPctgNonTarSeqs.clear();

    return 0;

}
*/
/*
int Designer::updataPrimerInfo(PrimerPair* pair)
{
    int numTarSeqs = (int)tarSeqs.size();
    int numNonTarSeqs = (int)nontarSeqs.size();
    QVector<short*> reversedTarSeqs;
    QVector<short*> reversedNonTarSeqs;
    short* primerLens = new short[1];
    short* sequence;

    short **lBindPosTarSeqs, **lBindPosNonTarSeqs;
    short **rBindPosTarSeqs, **rBindPosNonTarSeqs;

    float lPctgTarSeqs, lPctgNonTarSeqs;
    float rPctgTarSeqs, rPctgNonTarSeqs;

    try{
        pair->posBindingTar = new short*[2];
        pair->posBindingNontar = new short*[2];
        for(int k = 0; k < 2; k++){
            pair->posBindingTar[k] = new short[numTarSeqs];
            pair->posBindingNontar[k] = new short[numNonTarSeqs];
        }
    }
    catch(  std::bad_alloc& e ){

        //Clean Memory: (for Error)
        //  4. primer Candidates
        //  5. primers
        //  6. Triples (in Primer Pair)
        //  8. tripleShowList
        //  9. T&P
        //  10. Score

        // free memory for reversed sequences
        QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
        DataTools::cleanMemoryAllData();
        exit(-1);
    }



    lBindPosTarSeqs = new short*[2];
    lBindPosNonTarSeqs = new short*[2];
    for( int i = 0; i < 2; i++ ){
        // for target sequences
        try{
            lBindPosTarSeqs[i] = new short[numTarSeqs];
        }catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            DataTools::cleanMemoryAllData();
            exit(-1);
        }
        // for non-target sequences
        try{
            lBindPosNonTarSeqs[i] = new short[numNonTarSeqs];
        }catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            DataTools::cleanMemoryAllData();
            exit(-1);
        }

    }

    rBindPosTarSeqs = new short*[2];
    rBindPosNonTarSeqs = new short*[2];
    for( int i = 0; i < 2; i++ ){
        // for target sequences
        try{
            rBindPosTarSeqs[i] = new short[numTarSeqs];
        }catch(std::bad_alloc& e ){

            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            DataTools::cleanMemoryAllData();
            exit(-1);
        }

        // for non-target sequences
        try{
            rBindPosNonTarSeqs[i] = new short[numNonTarSeqs];
        } catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            return FATAL_ERROR;
        }
    }



    int seqLen;
    for( int i = 0; i < numTarSeqs; i++ ){
        seqLen = 0;
        while( tarSeqs[i][seqLen] >= 0 )
            seqLen++;
        sequence = new short[seqLen+1];
        for(int j = 0, k = seqLen-1; j < seqLen; j++, k--)
            sequence[j] = tarSeqs[i][k];
        sequence[seqLen] = -1;
        reversedTarSeqs.push_back(sequence);
    }
    for( int i = 0; i < numNonTarSeqs; i++ ){
        seqLen = 0;
        while( nontarSeqs[i][seqLen] >= 0 )
            seqLen++;
        sequence = new short[seqLen+1];
        for(int j = 0, k = seqLen-1; j < seqLen; j++, k--)
            sequence[j] = nontarSeqs[i][k];
        sequence[seqLen] = -1;
        reversedNonTarSeqs.push_back(sequence);
    }


    PrimerQuickSearch* ptr = new PrimerQuickSearch();

    if( DataTools::allocMemScoringTable() == FATAL_ERROR ){
        //Clean Memory: (for Error)
        //  4. probe Candidates
        //  5. probes
        //  6. Triples (in Primer Pair)
        //  8. tripleShowList
        //  9. T&P
        //  10. Score

        QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
        return FATAL_ERROR;
        // free memory for reversed sequences
        for( int i = 0; i < numTarSeqs; i++ )
            delete[] reversedTarSeqs[i];
        for( int i = 0; i < numNonTarSeqs; i++ )
            delete[] reversedNonTarSeqs[i];
        reversedTarSeqs.clear();
        reversedNonTarSeqs.clear();
        DataTools::cleanMemoryAllData();
        exit(-1);
    }
    short* l = tools::seqStr2Int(pair->left);
    short* rc = tools::seqStr2Int(pair->rightRevsComp);

    tools::seqInt2StrReverseComplement(rc,pair->right);
    short* r = tools::seqStr2Int(pair->right);


    primerLens[0] = (short)pair->right.length();
    ptr->initMember( r, 1, primerLens, RIGHT_PRIMER );
    // start searching
    for( int i = 0; i < numTarSeqs; i++ ){
        ptr->searchPrimersInTarSeq( tarSeqs[i] );
        rBindPosTarSeqs[0][i] = ptr->bindingPosStart[0];
        rBindPosTarSeqs[1][i] = ptr->bindingPosEnd[0];
    }

    ptr->initMember( r, 1, primerLens, RIGHT_PRIMER );
    // start searching
    for( int i = 0; i < numNonTarSeqs; i++ ){
        ptr->searchPrimersInNonTarSeq( nontarSeqs[i] );
        rBindPosNonTarSeqs[0][i] = ptr->bindingPosStart[0];
        rBindPosNonTarSeqs[1][i] = ptr->bindingPosEnd[0];
    }

    primerLens[0] = (short)pair->left.length();
    ptr->initMember( l, 1, primerLens, LEFT_PRIMER );
    // start searching
    for( int i = 0; i < numTarSeqs; i++ ){
        ptr->searchPrimersInTarSeq( reversedTarSeqs[i]);
        lBindPosTarSeqs[0][i] = ptr->bindingPosStart[0];
        lBindPosTarSeqs[1][i] = ptr->bindingPosEnd[0];
    }

    ptr->initMember( l, 1, primerLens, LEFT_PRIMER );
    // start searching
    for( int i = 0; i < numNonTarSeqs; i++ ){
        ptr->searchPrimersInNonTarSeq( reversedNonTarSeqs[i] );
        lBindPosNonTarSeqs[0][i] = ptr->bindingPosStart[0];
        lBindPosNonTarSeqs[1][i] = ptr->bindingPosEnd[0];
    }


    // free memory for reversed sequences
    for( int i = 0; i < numTarSeqs; i++ )
        delete[] reversedTarSeqs[i];
    for( int i = 0; i < numNonTarSeqs; i++ )
        delete[] reversedNonTarSeqs[i];
    reversedTarSeqs.clear();
    reversedNonTarSeqs.clear();



    float occ = 0;
    for( int j = 0; j < numTarSeqs; j++ ){
        if( lBindPosTarSeqs[0][j] >= 0 )
            occ++;
    }
    assert(numTarSeqs >= occ);
    lPctgTarSeqs =numTarSeqs>0?(occ/numTarSeqs * 100 ):0;

    occ = 0;
    for( int j = 0; j < numNonTarSeqs; j++ ){
        if( lBindPosNonTarSeqs[0][j] >= 0 )
            occ++;
    }
    assert(numNonTarSeqs >= occ);
    lPctgNonTarSeqs =numNonTarSeqs>0?( occ/numNonTarSeqs * 100 ):0;

    occ = 0;
    for( int j = 0; j < numTarSeqs; j++ ){
        if( rBindPosTarSeqs[0][j] >= 0 )
            occ++;
    }
    assert(numTarSeqs >= occ);
    rPctgTarSeqs =numTarSeqs>0?( occ/numTarSeqs * 100 ):0;

    occ = 0;
    for( int j = 0; j < numNonTarSeqs; j++ ){
        if( rBindPosNonTarSeqs[0][j] >= 0 )
            occ++;
    }
    assert(numNonTarSeqs >= occ);
    rPctgNonTarSeqs =numNonTarSeqs>0?( occ/numNonTarSeqs * 100 ):0;



    int bdPos[4];
    float lBindPosTarSeqsAvg, rBindPosTarSeqsAvg;
    float numDouble;
    int num;
    int maxProductSize;
    int minProductSize;
    int sumProductSize;
    int productSize;



    numDouble = 0; num = 0;
    for( int j = 0; j < numTarSeqs; j++ ){
        if( lBindPosTarSeqs[0][j] >= 0 ){
            num++;
            numDouble += lBindPosTarSeqs[1][j];  // left primer check ending position
        }
    }
    lBindPosTarSeqsAvg = num>0?(numDouble / num):0;

    numDouble = 0;	num = 0;
    for( int j = 0; j < numTarSeqs; j++ ){
        if( rBindPosTarSeqs[0][j] >= 0 ){
            num++;
            numDouble += rBindPosTarSeqs[0][j];  // right primer check starting position
        }
    }
    rBindPosTarSeqsAvg = num>0?(numDouble / num):0;



    num = 0;
    maxProductSize = 0;	minProductSize = INT_MAX;	sumProductSize = 0;
    int numAmbiTargetsLeft = 0;
    int numAmbiTargetsRight = 0;
    int numAmbiBasesLeft = 0;
    int numAmbiBasesRight = 0;
    bool flagLeft;
    bool flagRight;
    for( int k = 0; k < numTarSeqs; k++ ){
        bdPos[0] = lBindPosTarSeqs[0][k];
        bdPos[1] = lBindPosTarSeqs[1][k];
        bdPos[2] = rBindPosTarSeqs[0][k];
        bdPos[3] = rBindPosTarSeqs[1][k];

        // left and right primer both appear and positions are correct
        if( bdPos[0] >= 0 && bdPos[2] >=0 && bdPos[1] < bdPos[2] ){
            num++;

            productSize = bdPos[3] - bdPos[0] + 1;
            if( minProductSize > productSize )	minProductSize = productSize;
            if( maxProductSize < productSize )	maxProductSize = productSize;
            sumProductSize += productSize;
        }

        flagLeft = false;
        flagRight = false;
        for(int m = bdPos[0] ; m <= bdPos[1]; m++){
            if(tarSeqs[k][m] >3){
                numAmbiBasesLeft++;
                flagLeft = true;
            }
        }
        if(flagLeft){
            numAmbiTargetsLeft++;
        }
        for(int m = bdPos[2] ; m <= bdPos[3]; m++){
            if(tarSeqs[k][m] >3){
                numAmbiBasesRight++;
                flagRight = true;
            }
        }
        if(flagRight){
            numAmbiTargetsRight++;
        }
    }
    assert(numTarSeqs >= num);
    pair->pctgInTargetSeqs = numTarSeqs>0?((float)num / numTarSeqs * 100):0;
    pair->pcrSizeAvgTarSeqs = num>0?((float)sumProductSize / num):0;
    pair->pcrSizeMinTarSeqs = num>0?minProductSize:0;
    pair->pcrSizeMaxTarSeqs = num>0?maxProductSize:0;

    num = 0;
    maxProductSize = 0;	minProductSize = INT_MAX;	sumProductSize = 0;
    for( int k = 0; k < numNonTarSeqs; k++ ){
        bdPos[0] = lBindPosNonTarSeqs[0][k];
        bdPos[1] = lBindPosNonTarSeqs[1][k];
        bdPos[2] = rBindPosNonTarSeqs[0][k];
        bdPos[3] = rBindPosNonTarSeqs[1][k];

        // left and right primer both appear and positions are correct
        if( bdPos[0] >= 0 && bdPos[2] >=0 && bdPos[1] < bdPos[2] ){
            num++;
            //record binding target sequences and positions
            productSize = bdPos[3] - bdPos[0] + 1;
            if( minProductSize > productSize )	minProductSize = productSize;
            if( maxProductSize < productSize )	maxProductSize = productSize;
            sumProductSize += productSize;
        }
    }
    assert(numNonTarSeqs >= num);
    pair->pctgInNonTarSeqs = numNonTarSeqs>0?((float)num / numNonTarSeqs * 100):0;
    pair->pcrSizeAvgNonTarSeqs = num>0?((float)sumProductSize / num):0;
    pair->pcrSizeMinNonTarSeqs = num>0?minProductSize:0;
    pair->pcrSizeMaxNonTarSeqs = num>0?maxProductSize:0;

    pair->tmLeft = meltingTemp(pair->left);
    pair->tmRight = meltingTemp(pair->right);

    pair->prPrComp = primerInterComplementarity(pair->left, pair->rightRevsComp);
    pair->prPr3Comp = primer3InterComplementarity(pair->left, pair->rightRevsComp);
    pair->GCLeft = GCcontent(pair->left);
    pair->GCRight = GCcontent(pair->right);

    // primer self-complementarity
    pair->selfCompLeft = ppIntraComplementarity(pair->left);
    pair->selfCompRight = ppIntraComplementarity(pair->rightRevsComp);

    // primer 3' self-complementarity
    pair->self3CompLeft = pp3IntraComplementarity(pair->left, primerIntraBase3end);
    pair->self3CompRight = pp3IntraComplementarity(pair->rightRevsComp, primerIntraBase3end);

    // sigle primer average annealing positions in target
    // note lPrBindPosTarSeqsAvg[i] saves ending position for left primers,
    // need to change it to starting position for left primers. right primer is fine.
    pair->bindPosTarSeqsAvgLeft = lBindPosTarSeqsAvg - pair->left.length() + 1;
    pair->bindPosTarSeqsAvgRight = rBindPosTarSeqsAvg;

    // single primer frequency in target and non-target sequences
    pair->pctgInTarSeqsLeft = lPctgTarSeqs;
    pair->pctgInNonTarSeqsLeft = lPctgNonTarSeqs;
    pair->pctgInTarSeqsRight = rPctgTarSeqs;
    pair->pctgInNonTarSeqsRight = rPctgNonTarSeqs;

    // indicating exact information computed
    //pair->isEstimateTar = 0;
    //pair->isEstimateNonTar = 0;

    pair->numAmbiguousTargetsLeft = numAmbiTargetsLeft;
    pair->numAmbiguousTargetsRight = numAmbiTargetsRight ;
    pair->numAmbiguousBasesLeft = numAmbiBasesLeft;
    pair->numAmbiguousBasesRight = numAmbiBasesRight;
    pair->marked = 0;
    // record binding position for each seqs

    for(int k = 0; k < numTarSeqs; k++){
        pair->posBindingTar[0][k] = lBindPosTarSeqs[1][k];
        pair->posBindingTar[1][k] = rBindPosTarSeqs[0][k];
    }
    for(int k = 0; k < numNonTarSeqs; k++){
        pair->posBindingNontar[0][k] = lBindPosNonTarSeqs[1][k];
        pair->posBindingNontar[1][k] = rBindPosNonTarSeqs[0][k];
    }
    //pair->posBindingTar[0] =  lPrBindPosTarSeqs[1][i];
    //pair->posBindingTar[1] =  rPrBindPosTarSeqs[0][j];
    //pair->posBindingNontar[0] = lPrBindPosNonTarSeqs[1][i];
    //pair->posBindingNontar[1] = rPrBindPosNonTarSeqs[0][j];

    for( int i = 0; i < 2; i++ ){
        delete[] lBindPosTarSeqs[i];
        delete[] lBindPosNonTarSeqs[i];
    }
    for( int i = 0; i < 2; i++ ){
        delete[] rBindPosTarSeqs[i];
        delete[] rBindPosNonTarSeqs[i];
    }
    delete[] lBindPosTarSeqs;
    delete[] lBindPosNonTarSeqs;
    delete[] rBindPosTarSeqs;
    delete[] rBindPosNonTarSeqs;

    return 0;

}*/


int Designer::updataPrimerInfo(QVector<PrimerPair*> pairList)
{
    int numPrimerPairs = (int)pairList.size();
    int numTarSeqs = (int)tarSeqs.size();
    int numNonTarSeqs = (int)nontarSeqs.size();

    QVector<short*> reversedTarSeqs;
    QVector<short*> reversedNonTarSeqs;

    short* primerLens = new short[1];
    short* sequence;

    short ***lBindPosTarSeqs, ***lBindPosNonTarSeqs;
    short ***rBindPosTarSeqs, ***rBindPosNonTarSeqs;

    float lPctgTarSeqs, lPctgNonTarSeqs;
    float rPctgTarSeqs, rPctgNonTarSeqs;



    lBindPosTarSeqs = new short**[2];
    lBindPosNonTarSeqs = new short**[2];
    for( int i = 0; i < 2; i++ ){
        // for target sequences
        try{
            lBindPosTarSeqs[i] = new short*[numPrimerPairs];
            for( int j = 0; j < numPrimerPairs; j++ ){
                lBindPosTarSeqs[i][j] = new short[numTarSeqs];
            }
        }catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            return FATAL_ERROR;
        }

        // for non-target sequences
        try{
            lBindPosNonTarSeqs[i] = new short*[numPrimerPairs];
            for( int j = 0; j < numPrimerPairs; j++ ){
                lBindPosNonTarSeqs[i][j] = new short[numNonTarSeqs];
            }
        }catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            return FATAL_ERROR;
        }
    }

    rBindPosTarSeqs = new short**[2];
    rBindPosNonTarSeqs = new short**[2];
    for( int i = 0; i < 2; i++ ){
        // for target sequences
        try{
            rBindPosTarSeqs[i] = new short*[numPrimerPairs];
            for( int j = 0; j < numPrimerPairs; j++ )
                rBindPosTarSeqs[i][j] = new short[numTarSeqs];
        }catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            return FATAL_ERROR;
        }


        // for non-target sequences
        try{
            rBindPosNonTarSeqs[i] = new short*[numPrimerPairs];
            for( int j = 0; j < numPrimerPairs; j++ )
                rBindPosNonTarSeqs[i][j] = new short[numNonTarSeqs];
        } catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            return FATAL_ERROR;
        }

    }
    /*lBindPosTarSeqs = new short*[2];
    lBindPosNonTarSeqs = new short*[2];
    for( int i = 0; i < 2; i++ ){
        // for target sequences
        try{
            lBindPosTarSeqs[i] = new short[numTarSeqs];
        }catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            DataTools::cleanMemoryAllData();
            exit(-1);
        }
        // for non-target sequences
        try{
            lBindPosNonTarSeqs[i] = new short[numNonTarSeqs];
        }catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            DataTools::cleanMemoryAllData();
            exit(-1);
        }

    }

    rBindPosTarSeqs = new short*[2];
    rBindPosNonTarSeqs = new short*[2];
    for( int i = 0; i < 2; i++ ){
        // for target sequences
        try{
            rBindPosTarSeqs[i] = new short[numTarSeqs];
        }catch(std::bad_alloc& e ){

            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            DataTools::cleanMemoryAllData();
            exit(-1);
        }

        // for non-target sequences
        try{
            rBindPosNonTarSeqs[i] = new short[numNonTarSeqs];
        } catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            return FATAL_ERROR;
        }
    }*/


    //prepare reversed target sequences and non-target sequences
    int seqLen;
    for( int i = 0; i < numTarSeqs; i++ ){
        seqLen = 0;
        while( tarSeqs[i][seqLen] >= 0 )
            seqLen++;
        sequence = new short[seqLen+1];
        for(int j = 0, k = seqLen-1; j < seqLen; j++, k--)
            sequence[j] = tarSeqs[i][k];
        sequence[seqLen] = -1;
        reversedTarSeqs.push_back(sequence);
    }
    for( int i = 0; i < numNonTarSeqs; i++ ){
        seqLen = 0;
        while( nontarSeqs[i][seqLen] >= 0 )
            seqLen++;
        sequence = new short[seqLen+1];
        for(int j = 0, k = seqLen-1; j < seqLen; j++, k--)
            sequence[j] = nontarSeqs[i][k];
        sequence[seqLen] = -1;
        reversedNonTarSeqs.push_back(sequence);
    }


    //start to search
    PrimerQuickSearch* ptr = new PrimerQuickSearch();
    /*if( ptr->allocMemScoringTable() == FATAL_ERROR ){
        //Clean Memory: (for Error)
        //  4. probe Candidates
        //  5. probes
        //  6. Triples (in Primer Pair)
        //  8. tripleShowList
        //  9. T&P
        //  10. Score

        QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
        return FATAL_ERROR;
        // free memory for reversed sequences
        for( int i = 0; i < numTarSeqs; i++ )
            delete[] reversedTarSeqs[i];
        for( int i = 0; i < numNonTarSeqs; i++ )
            delete[] reversedNonTarSeqs[i];
        reversedTarSeqs.clear();
        reversedNonTarSeqs.clear();
        DataTools::cleanMemoryAllData();
        exit(-1);
    }*/


    MyProgressDlg *pDlg = new MyProgressDlg("Updating primer pair information",0, pairList.size());
    pDlg->setWindowModality(Qt::ApplicationModal);

    int p = 0;
    QVector<PrimerPair*>::iterator it;

    for(it = pairList.begin(); it < pairList.end(); it++){
        pDlg->setValue(p);
        PrimerPair* pr = *it;
        try{
            pr->posBindingTar = new short*[2];
            pr->posBindingNontar = new short*[2];
            for(int k = 0; k < 2; k++){
                pr->posBindingTar[k] = new short[numTarSeqs];
                pr->posBindingNontar[k] = new short[numNonTarSeqs];
            }
        }
        catch(  std::bad_alloc& e ){
            // free memory
            for( int i = 0; i < 2; i++ ){
                for( int j = 0; j < numPrimerPairs; j++ ){
                    delete[] lBindPosTarSeqs[i][j];
                    delete[] lBindPosNonTarSeqs[i][j];
                    delete[] rBindPosTarSeqs[i][j];
                    delete[] rBindPosNonTarSeqs[i][j];
                }
                delete[] lBindPosTarSeqs[i];
                delete[] lBindPosNonTarSeqs[i];

                delete[] rBindPosTarSeqs[i];
                delete[] rBindPosNonTarSeqs[i];
            }

            delete[] lBindPosTarSeqs;
            delete[] lBindPosNonTarSeqs;
            delete[] rBindPosTarSeqs;
            delete[] rBindPosNonTarSeqs;


            for(int i = 0; i < tarSeqs.size(); i++ ){
                delete[] reversedTarSeqs[i];
            }
            for(int i = 0; i < nontarSeqs.size(); i++){
                delete[] reversedNonTarSeqs[i];
            }

            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            DataTools::cleanMemoryAllData();
            exit(-1);
        }

        short* l = tools::seqStr2Int(pr->left);
        short* rc = tools::seqStr2Int(pr->rightRevsComp);

        tools::seqInt2StrReverseComplement(rc,pr->right);
        short* r = tools::seqStr2Int(pr->right);

        primerLens[0] = (short)pr->left.length();
        ptr->initMember( l, 1, primerLens, LEFT_PRIMER );
        // start searching
        for( int i = 0; i < numTarSeqs; i++ ){
            ptr->searchPrimersInTarSeq( reversedTarSeqs[i]);
            lBindPosTarSeqs[0][p][i] = ptr->bindingPosStart[0];
            lBindPosTarSeqs[1][p][i] = ptr->bindingPosEnd[0];
        }
        //ptr->initMember( l, 1, primerLens, LEFT_PRIMER );
        // start searching
        for( int i = 0; i < numNonTarSeqs; i++ ){
            ptr->searchPrimersInNonTarSeq( reversedNonTarSeqs[i] );
            lBindPosNonTarSeqs[0][p][i] = ptr->bindingPosStart[0];
            lBindPosNonTarSeqs[1][p][i] = ptr->bindingPosEnd[0];
        }
        primerLens[0] = (short)pr->right.length();
        ptr->initMember( r, 1, primerLens, RIGHT_PRIMER );
        // start searching
        for( int i = 0; i < numTarSeqs; i++ ){
            ptr->searchPrimersInTarSeq( tarSeqs[i] );
            rBindPosTarSeqs[0][p][i] = ptr->bindingPosStart[0];
            rBindPosTarSeqs[1][p][i] = ptr->bindingPosEnd[0];
        }
        //ptr->initMember( r, 1, primerLens, RIGHT_PRIMER );
        // start searching
        for( int i = 0; i < numNonTarSeqs; i++ ){
            ptr->searchPrimersInNonTarSeq( nontarSeqs[i] );
            rBindPosNonTarSeqs[0][p][i] = ptr->bindingPosStart[0];
            rBindPosNonTarSeqs[1][p][i] = ptr->bindingPosEnd[0];
        }

        float occ = 0;
        for( int j = 0; j < numTarSeqs; j++ ){
            if( lBindPosTarSeqs[0][p][j] >= 0 )
                occ++;
        }
        assert(numTarSeqs >= occ);
        lPctgTarSeqs =numTarSeqs>0?(occ/numTarSeqs * 100 ):0;

        occ = 0;
        for( int j = 0; j < numNonTarSeqs; j++ ){
            if( lBindPosNonTarSeqs[0][p][j] >= 0 )
                occ++;
        }
        assert(numNonTarSeqs >= occ);
        lPctgNonTarSeqs =numNonTarSeqs>0?( occ/numNonTarSeqs * 100 ):0;

        occ = 0;
        for( int j = 0; j < numTarSeqs; j++ ){
            if( rBindPosTarSeqs[0][p][j] >= 0 )
                occ++;
        }
        assert(numTarSeqs >= occ);
        rPctgTarSeqs =numTarSeqs>0?( occ/numTarSeqs * 100 ):0;

        occ = 0;
        for( int j = 0; j < numNonTarSeqs; j++ ){
            if( rBindPosNonTarSeqs[0][p][j] >= 0 )
                occ++;
        }
        assert(numNonTarSeqs >= occ);
        rPctgNonTarSeqs =numNonTarSeqs>0?( occ/numNonTarSeqs * 100 ):0;



        int bdPos[4];
        float lBindPosTarSeqsAvg, rBindPosTarSeqsAvg;
        float numDouble;
        int num;
        int maxProductSize;
        int minProductSize;
        int sumProductSize;
        int productSize;



        numDouble = 0; num = 0;
        for( int j = 0; j < numTarSeqs; j++ ){
            if( lBindPosTarSeqs[0][p][j] >= 0 ){
                num++;
                numDouble += lBindPosTarSeqs[1][p][j];  // left primer check ending position
            }
        }
        lBindPosTarSeqsAvg = num>0?(numDouble / num):0;

        numDouble = 0;	num = 0;
        for( int j = 0; j < numTarSeqs; j++ ){
            if( rBindPosTarSeqs[0][p][j] >= 0 ){
                num++;
                numDouble += rBindPosTarSeqs[0][p][j];  // right primer check starting position
            }
        }
        rBindPosTarSeqsAvg = num>0?(numDouble / num):0;



        num = 0;
        maxProductSize = 0;	minProductSize = INT_MAX;	sumProductSize = 0;
        int numAmbiTargetsLeft = 0;
        int numAmbiTargetsRight = 0;
        int numAmbiBasesLeft = 0;
        int numAmbiBasesRight = 0;
        bool flagLeft;
        bool flagRight;
        for( int k = 0; k < numTarSeqs; k++ ){
            bdPos[0] = lBindPosTarSeqs[0][p][k];
            bdPos[1] = lBindPosTarSeqs[1][p][k];
            bdPos[2] = rBindPosTarSeqs[0][p][k];
            bdPos[3] = rBindPosTarSeqs[1][p][k];

            // left and right primer both appear and positions are correct
            if( bdPos[0] >= 0 && bdPos[2] >=0 && bdPos[1] < bdPos[2] ){
                num++;

                productSize = bdPos[3] - bdPos[0] + 1;
                if( minProductSize > productSize )	minProductSize = productSize;
                if( maxProductSize < productSize )	maxProductSize = productSize;
                sumProductSize += productSize;
            }

            flagLeft = false;
            flagRight = false;
            for(int m = bdPos[0] ; m <= bdPos[1]; m++){
                if(tarSeqs[k][m] >3){
                    numAmbiBasesLeft++;
                    flagLeft = true;
                }
            }
            if(flagLeft){
                numAmbiTargetsLeft++;
            }
            for(int m = bdPos[2] ; m <= bdPos[3]; m++){
                if(tarSeqs[k][m] >3){
                    numAmbiBasesRight++;
                    flagRight = true;
                }
            }
            if(flagRight){
                numAmbiTargetsRight++;
            }
        }
        assert(numTarSeqs >= num);
        pr->pctgInTargetSeqs = numTarSeqs>0?((float)num / numTarSeqs * 100):0;
        pr->pcrSizeAvgTarSeqs = num>0?((float)sumProductSize / num):0;
        pr->pcrSizeMinTarSeqs = num>0?minProductSize:0;
        pr->pcrSizeMaxTarSeqs = num>0?maxProductSize:0;

        num = 0;
        maxProductSize = 0;	minProductSize = INT_MAX;	sumProductSize = 0;
        for( int k = 0; k < numNonTarSeqs; k++ ){
            bdPos[0] = lBindPosNonTarSeqs[0][p][k];
            bdPos[1] = lBindPosNonTarSeqs[1][p][k];
            bdPos[2] = rBindPosNonTarSeqs[0][p][k];
            bdPos[3] = rBindPosNonTarSeqs[1][p][k];

            // left and right primer both appear and positions are correct
            if( bdPos[0] >= 0 && bdPos[2] >=0 && bdPos[1] < bdPos[2] ){
                num++;
                //record binding target sequences and positions
                productSize = bdPos[3] - bdPos[0] + 1;
                if( minProductSize > productSize )	minProductSize = productSize;
                if( maxProductSize < productSize )	maxProductSize = productSize;
                sumProductSize += productSize;
            }
        }
        assert(numNonTarSeqs >= num);
        pr->pctgInNonTarSeqs = numNonTarSeqs>0?((float)num / numNonTarSeqs * 100):0;
        pr->pcrSizeAvgNonTarSeqs = num>0?((float)sumProductSize / num):0;
        pr->pcrSizeMinNonTarSeqs = num>0?minProductSize:0;
        pr->pcrSizeMaxNonTarSeqs = num>0?maxProductSize:0;
        /*if( numNonTarSeqs != 0 )
            pair->pctgInNonTarSeqs = (float)num / numNonTarSeqs * 100;
        else
            pair->pctgInNonTarSeqs = 0;

        if( num == 0 ){
            pair->pcrSizeAvgNonTarSeqs = 0;
            pair->pcrSizeMinNonTarSeqs = 0;
            pair->pcrSizeMaxNonTarSeqs = 0;
        }
        else{
            pair->pcrSizeAvgNonTarSeqs = (float)sumProductSize / num;
            pair->pcrSizeMinNonTarSeqs = minProductSize;
            pair->pcrSizeMaxNonTarSeqs = maxProductSize;
        }*/

        pr->tmLeft = meltingTemp(pr->left);
        pr->tmRight = meltingTemp(pr->right);

        pr->prPrComp = primerInterComplementarity(pr->left, pr->rightRevsComp);
        pr->prPr3Comp = primer3InterComplementarity(pr->left, pr->rightRevsComp);
        pr->GCLeft = GCcontent(pr->left);
        pr->GCRight = GCcontent(pr->right);

        // primer self-complementarity
        pr->selfCompLeft = ppIntraComplementarity(pr->left);
        pr->selfCompRight = ppIntraComplementarity(pr->rightRevsComp);

        // primer 3' self-complementarity
        pr->self3CompLeft = pp3IntraComplementarity(pr->left, primerIntraBase3end);
        pr->self3CompRight = pp3IntraComplementarity(pr->rightRevsComp, primerIntraBase3end);

        // sigle primer average annealing positions in target
        // note lPrBindPosTarSeqsAvg[i] saves ending position for left primers,
        // need to change it to starting position for left primers. right primer is fine.
        pr->bindPosTarSeqsAvgLeft = lBindPosTarSeqsAvg - pr->left.length() + 1;
        pr->bindPosTarSeqsAvgRight = rBindPosTarSeqsAvg;

        // single primer frequency in target and non-target sequences
        pr->pctgInTarSeqsLeft = lPctgTarSeqs;
        pr->pctgInNonTarSeqsLeft = lPctgNonTarSeqs;
        pr->pctgInTarSeqsRight = rPctgTarSeqs;
        pr->pctgInNonTarSeqsRight = rPctgNonTarSeqs;

        // indicating exact information computed
        //pair->isEstimateTar = 0;
        //pair->isEstimateNonTar = 0;

        pr->numAmbiguousTargetsLeft = numAmbiTargetsLeft;
        pr->numAmbiguousTargetsRight = numAmbiTargetsRight ;
        pr->numAmbiguousBasesLeft = numAmbiBasesLeft;
        pr->numAmbiguousBasesRight = numAmbiBasesRight;
        pr->marked = 0;
        // record binding position for each seqs

        for(int k = 0; k < numTarSeqs; k++){
            pr->posBindingTar[0][k] = lBindPosTarSeqs[1][p][k];
            pr->posBindingTar[1][k] = rBindPosTarSeqs[0][p][k];
        }
        for(int k = 0; k < numNonTarSeqs; k++){
            pr->posBindingNontar[0][k] = lBindPosNonTarSeqs[1][p][k];
            pr->posBindingNontar[1][k] = rBindPosNonTarSeqs[0][p][k];
        }
        //pair->posBindingTar[0] =  lPrBindPosTarSeqs[1][i];
        //pair->posBindingTar[1] =  rPrBindPosTarSeqs[0][j];
        //pair->posBindingNontar[0] = lPrBindPosNonTarSeqs[1][i];
        //pair->posBindingNontar[1] = rPrBindPosNonTarSeqs[0][j];
        p++;
    }

    delete pDlg;



    // free memory for reversed sequences
    for( int i = 0; i < numTarSeqs; i++ )
        delete[] reversedTarSeqs[i];
    for( int i = 0; i < numNonTarSeqs; i++ )
        delete[] reversedNonTarSeqs[i];
    reversedTarSeqs.clear();
    reversedNonTarSeqs.clear();

    for( int i = 0; i < 2; i++ ){
        delete[] lBindPosTarSeqs[i];
        delete[] lBindPosNonTarSeqs[i];
    }
    for( int i = 0; i < 2; i++ ){
        delete[] rBindPosTarSeqs[i];
        delete[] rBindPosNonTarSeqs[i];
    }
    delete[] lBindPosTarSeqs;
    delete[] lBindPosNonTarSeqs;
    delete[] rBindPosTarSeqs;
    delete[] rBindPosNonTarSeqs;

    return 0;

}

int Designer::updataProbeInfo(QVector<Probe*> probeList)
{
    int numProbes = (int)probeList.size();
    int numTarSeqs = (int)tarSeqs.size();
    int numNonTarSeqs = (int)nontarSeqs.size();

    QVector<short*> reversedTarSeqs;
    QVector<short*> reversedNonTarSeqs;

    short* probeGroupLens = new short[1];
    short* sequence;

    short ***probeBindPosTarSeqs, ***probeBindPosNonTarSeqs;
    float probePctgTarSeqs,probePctgNonTarSeqs;

    /***************************************************************************************************/
    // first allocate memory for probe annealing position variables
    probeBindPosTarSeqs = new short**[2];
    probeBindPosNonTarSeqs = new short**[2];
    for( int i = 0; i < 2; i++ ){
        // for target sequences
        try{
            probeBindPosTarSeqs[i] = new short*[numProbes];
            for( int j = 0; j < numProbes; j++ ){
                probeBindPosTarSeqs[i][j] = new short[numTarSeqs];
            }
        }catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            return FATAL_ERROR;
        }

        // for non-target sequences
        try{
            probeBindPosNonTarSeqs[i] = new short*[numProbes];
            for( int j = 0; j < numProbes; j++ ){
                probeBindPosNonTarSeqs[i][j] = new short[numNonTarSeqs];
            }
        }catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            return FATAL_ERROR;
        }
    }


    /***************************************************************************************************/
    // next prepare reversed target and non-target sequences
    // for searching left probes in sequences from 3' to 5'
    int seqLen;
    for( int i = 0; i < numTarSeqs; i++ ){
        seqLen = 0;
        while( tarSeqs[i][seqLen] >= 0 )
            seqLen++;
        sequence = new short[seqLen+1];
        for(int j = 0, k = seqLen-1; j < seqLen; j++, k--)
            sequence[j] = tarSeqs[i][k];
        sequence[seqLen] = -1;
        reversedTarSeqs.push_back(sequence);
    }
    for( int i = 0; i < numNonTarSeqs; i++ ){
        seqLen = 0;
        while( nontarSeqs[i][seqLen] >= 0 )
            seqLen++;
        sequence = new short[seqLen+1];
        for(int j = 0, k = seqLen-1; j < seqLen; j++, k--)
            sequence[j] = nontarSeqs[i][k];
        sequence[seqLen] = -1;
        reversedNonTarSeqs.push_back(sequence);
    }

    /***************************************************************************************************/
    // finally start approxiation search: group probes with common prefix
    // do the search for a group
    ProbeQuickSearch* ptr = new ProbeQuickSearch();

    MyProgressDlg *pDlg = new MyProgressDlg("Updating probe information",0, probeList.size());
    pDlg->setWindowModality(Qt::ApplicationModal);

    int p = 0;
    QVector<Probe*>::iterator it;

    for(it = probeList.begin(); it < probeList.end(); it++){
        pDlg->setValue(p);
        Probe* pb = *it;
        try{
            pb->posBindingTar = new short*[2];
            pb->posBindingNontar = new short*[2];
            for(int k = 0; k < 2; k++){
                pb->posBindingTar[k] = new short[numTarSeqs];
                pb->posBindingNontar[k] = new short[numNonTarSeqs];
            }

        }catch(  std::bad_alloc& e ){
            // free memory
            for( int i = 0; i < 2; i++ ){
                for( int j = 0; j < numProbes; j++ ){
                    delete[] probeBindPosTarSeqs[i][j];
                    delete[] probeBindPosNonTarSeqs[i][j];
                }
                delete[] probeBindPosTarSeqs[i];
                delete[] probeBindPosNonTarSeqs[i];
            }
            delete[] probeBindPosTarSeqs;
            delete[] probeBindPosNonTarSeqs;

            for(int i = 0; i < tarSeqs.size(); i++ ){
                delete[] reversedTarSeqs[i];
            }
            for(int i = 0; i < nontarSeqs.size(); i++){
                delete[] reversedNonTarSeqs[i];
            }
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for probe searching.\nProgram is forced to terminate.");
            DataTools::cleanMemoryAllData();
            exit(-1);
        }

        short* pbInt = tools::seqStr2Int(pb->probeSeq);

        probeGroupLens[0] = (short)pb->probeSeq.length();
        ptr->initMember( pbInt, 1, probeGroupLens);
        // start searching
        for( int i = 0; i < numTarSeqs; i++ ){
            ptr->searchProbesInTarSeq(tarSeqs[i], reversedTarSeqs[i] );
            probeBindPosTarSeqs[0][p][i] = ptr->bindingPosStart[0];
            probeBindPosTarSeqs[1][p][i] = ptr->bindingPosEnd[0];
        }
        ptr->initMember( pbInt, 1, probeGroupLens);
        // start searching
        for( int i = 0; i < numNonTarSeqs; i++ ){
            ptr->searchProbesInNonTarSeq( nontarSeqs[i], reversedNonTarSeqs[i] );
            probeBindPosNonTarSeqs[0][p][i] = ptr->bindingPosStart[0];
            probeBindPosNonTarSeqs[1][p][i] = ptr->bindingPosEnd[0];
        }


        float occ = 0;
        for( int j = 0; j < numTarSeqs; j++ ){
            if( probeBindPosTarSeqs[0][p][j] >= 0 )
                occ++;
        }
        assert(numTarSeqs >= occ);
        probePctgTarSeqs = numTarSeqs>0?(occ/numTarSeqs * 100 ):0;

        occ = 0;
        for( int j = 0; j < numNonTarSeqs; j++ ){
            if( probeBindPosNonTarSeqs[0][p][j] >= 0 )
                occ++;
        }
        assert(numNonTarSeqs >= occ);
        probePctgNonTarSeqs = numNonTarSeqs>0?(occ/numNonTarSeqs * 100 ):0;

        int bdPos[2];
        float* probeBindPosTarSeqsAvg = new float[2];
        float numPosStart, numPosEnd;
        int numProbe;


        numPosStart = 0; numPosEnd = 0; numProbe = 0;
        for( int j = 0; j < numTarSeqs; j++ ){
            if( probeBindPosTarSeqs[0][p][j] >= 0 ){
                assert(probeBindPosTarSeqs[1][p][j] > 0);
                numProbe++;
                numPosStart += probeBindPosTarSeqs[0][p][j];
                numPosEnd   += probeBindPosTarSeqs[1][p][j];  // probe check both start and ending positions
            }
        }
        probeBindPosTarSeqsAvg[0] = numProbe>0?(numPosStart / numProbe):0;
        probeBindPosTarSeqsAvg[1] = numProbe>0?(numPosEnd / numProbe):0;


        numProbe = 0;
        int numAmbiTargets = 0;
        int numAmbiBases = 0;
        bool flag;

        for( int j = 0; j < numTarSeqs; j++ ){
            bdPos[0] = probeBindPosTarSeqs[0][p][j];
            bdPos[1] = probeBindPosTarSeqs[1][p][j];

            if( bdPos[0] >= 0 ){//probe appears
                numProbe++;   //this primer exists at current tar Seqs
            }

            flag = false;
            for(int k = bdPos[0] ; k <= bdPos[1]; k++){
                if(tarSeqs[j][k] >3){
                    numAmbiBases++;
                    flag = true;
                }
            }
            if(flag){
                numAmbiTargets++;
            }
        }
        pb->pctgInTargetSeqs = numTarSeqs>0?((float)numProbe / numTarSeqs * 100):0;
        /*if( numTarSeqs != 0 )
            pb->pctgInTargetSeqs = (float)numProbe / numTarSeqs * 100;
        else
            pb->pctgInTargetSeqs = 0;*/

        // compute frequencies for probe pair in non-target
        numProbe = 0;
        for( int j = 0; j < numNonTarSeqs; j++ ){
            bdPos[0] = probeBindPosNonTarSeqs[0][p][j];
            bdPos[1] = probeBindPosNonTarSeqs[1][p][j];
            // probe both appear and positions are correct
            if( bdPos[0] >= 0){
                numProbe++;
            }
        }
        pb->pctgInNonTarSeqs = numNonTarSeqs>0?((float)numProbe / numNonTarSeqs * 100):0;
        /*if( numNonTarSeqs != 0 )
            pb->pctgInNonTarSeqs = (float)numProbe / numNonTarSeqs * 100;
        else
            pb->pctgInNonTarSeqs = 0;*/


        pb->tmProbe= meltingTemp(pb->probeSeq);
        // GC content
        pb->GCNum= GCcontent(pb->probeSeq);
        // probe self-complementarity
        pb->selfComp = ppIntraComplementarity(pb->probeSeq);
        //pb->self3Comp = pp3IntraComplementarity(pb->probeSeq, probeIntraBase3end);

        // sigle probe average annealing positions in target
        // note lPrBindPosTarSeqsAvg[i] saves ending position for probes,
        // need to change it to starting position for probes
        pb->bindPosTarSeqsAvg[0] = probeBindPosTarSeqsAvg[0];
        pb->bindPosTarSeqsAvg[1] = probeBindPosTarSeqsAvg[1];

        // probe frequency in target and non-target sequences
        pb->pctgInTargetSeqs = probePctgTarSeqs;
        pb->pctgInNonTarSeqs = probePctgNonTarSeqs;

        // indicating exact information computed
        //pb->isEstimateTar = 0;
        //pb->isEstimateNonTar = 0;
        pb->numAmbiguousTargets = numAmbiTargets ;
        pb->numAmbiguousBases = numAmbiBases;

        pb->marked = 0;


        for(int k = 0; k < numTarSeqs; k++){
            pb->posBindingTar[0][k] = probeBindPosTarSeqs[0][p][k];
            pb->posBindingTar[1][k] = probeBindPosTarSeqs[1][p][k];
        }
        for(int k = 0; k < numNonTarSeqs; k++){
            pb->posBindingNontar[0][k] = probeBindPosNonTarSeqs[0][p][k];
            pb->posBindingNontar[1][k] = probeBindPosNonTarSeqs[1][p][k];
        }
        p++;
    }
    delete pDlg;

    // free memory for reversed sequences
    for( int i = 0; i < numTarSeqs; i++ )
        delete[] reversedTarSeqs[i];
    for( int i = 0; i < numNonTarSeqs; i++ )
        delete[] reversedNonTarSeqs[i];
    reversedTarSeqs.clear();
    reversedNonTarSeqs.clear();

    for( int i = 0; i < 2; i++ ){
        delete[] probeBindPosTarSeqs[i];
        delete[] probeBindPosNonTarSeqs[i];
    }
    delete[] probeBindPosTarSeqs;
    delete[] probeBindPosNonTarSeqs;

    return 0;





}
//MX
int Designer::quickSearchPrimerInSeqs(void)
{
    if( lPrimerCandid.size() == 0 || rPrimerCandid.size() == 0 )
        return 0;

    QVector<float> lPrPctgTarSeqs, lPrPctgNonTarSeqs;
    QVector<float> rPrPctgTarSeqs, rPrPctgNonTarSeqs;
    int numLprimers = (int)lPrimerCandid.size();
    int numRprimers = (int)rPrimerCandid.size();
    int numTarSeqs = (int)tarSeqs.size();
    int numNonTarSeqs = (int)nontarSeqs.size();
    QVector<short*> reversedTarSeqs;
    QVector<short*> reversedNonTarSeqs;
    short* sequence;
    short primerGroupLens[40];
    int prLen, prNum;
    int pr1st, prlast;
    int seqLen;
    QTime time;
    QString timeStr;

    // first allocate memory for primer annealing position variables
    /*
    lPrBindPosTarSeqs = new short**[numLprimers];
    lPrBindPosNonTarSeqs = new short**[numLprimers];
    for( int i = 0; i < numLprimers; i++ ){
        // for target sequences
        try{
            lPrBindPosTarSeqs[i] = new short*[numTarSeqs];
        }catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            return FATAL_ERROR;
        }
        for( int j = 0; j < numTarSeqs; j++ )
            lPrBindPosTarSeqs[i][j] = new short[2];

        // for non-target sequences
        try{
            lPrBindPosNonTarSeqs[i] = new short*[numNonTarSeqs];
        }catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            return FATAL_ERROR;
        }
        for( int j = 0; j < numNonTarSeqs; j++ )
            lPrBindPosNonTarSeqs[i][j] = new short[2];
    }
    rPrBindPosTarSeqs = new short**[numRprimers];
    rPrBindPosNonTarSeqs = new short**[numRprimers];
    for( int i = 0; i < numRprimers; i++ ){
        // for target sequences
        try{
            rPrBindPosTarSeqs[i] = new short*[numTarSeqs];
        }catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            return FATAL_ERROR;
        }
        for( int j = 0; j < numTarSeqs; j++ )
            rPrBindPosTarSeqs[i][j] = new short[2];

        // for non-target sequences
        try{
            rPrBindPosNonTarSeqs[i] = new short*[numNonTarSeqs];
        } catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            return FATAL_ERROR;
        }
        for( int j = 0; j < numNonTarSeqs; j++ )
            rPrBindPosNonTarSeqs[i][j] = new short[2];
    }*/



    lPrBindPosTarSeqs = new short**[2];
    lPrBindPosNonTarSeqs = new short**[2];
    for( int i = 0; i < 2; i++ ){
        // for target sequences
        try{
            lPrBindPosTarSeqs[i] = new short*[numLprimers];
            for( int j = 0; j < numLprimers; j++ ){
                lPrBindPosTarSeqs[i][j] = new short[numTarSeqs];
            }
        }catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            return FATAL_ERROR;
        }

        // for non-target sequences
        try{
            lPrBindPosNonTarSeqs[i] = new short*[numLprimers];
            for( int j = 0; j < numLprimers; j++ ){
                lPrBindPosNonTarSeqs[i][j] = new short[numNonTarSeqs];
            }
        }catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            return FATAL_ERROR;
        }

    }


    rPrBindPosTarSeqs = new short**[2];
    rPrBindPosNonTarSeqs = new short**[2];
    for( int i = 0; i < 2; i++ ){
        // for target sequences
        try{
            rPrBindPosTarSeqs[i] = new short*[numRprimers];
            for( int j = 0; j < numRprimers; j++ )
                rPrBindPosTarSeqs[i][j] = new short[numTarSeqs];
        }catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            return FATAL_ERROR;
        }


        // for non-target sequences
        try{
            rPrBindPosNonTarSeqs[i] = new short*[numRprimers];
            for( int j = 0; j < numRprimers; j++ )
                rPrBindPosNonTarSeqs[i][j] = new short[numNonTarSeqs];
        } catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            return FATAL_ERROR;
        }

    }


    //user abort
    if(pdProgDlg->wasCanceled()){
        // free memory
        for( int i = 0; i < 2; i++ ){
            for( int j = 0; j < numLprimers; j++ )
                delete[] lPrBindPosTarSeqs[i][j];
            for( int j = 0; j < numLprimers; j++ )
                delete[] lPrBindPosNonTarSeqs[i][j];
            delete[] lPrBindPosTarSeqs[i];
            delete[] lPrBindPosNonTarSeqs[i];
        }
        for( int i = 0; i < 2; i++ ){
            for( int j = 0; j < numRprimers; j++ )
                delete[] rPrBindPosTarSeqs[i][j];
            for( int j = 0; j < numRprimers; j++ )
                delete[] rPrBindPosNonTarSeqs[i][j];
            delete[] rPrBindPosTarSeqs[i];
            delete[] rPrBindPosNonTarSeqs[i];
        }
        delete[] lPrBindPosTarSeqs;
        delete[] lPrBindPosNonTarSeqs;
        delete[] rPrBindPosTarSeqs;
        delete[] rPrBindPosNonTarSeqs;

        return -1;
    }

    // next prepare reversed target and non-target sequences
    // for searching left primers in sequences from 3' to 5'
    for( int i = 0; i < numTarSeqs; i++ ){
        seqLen = 0;
        while( tarSeqs[i][seqLen] >= 0 )
            seqLen++;
        sequence = new short[seqLen+1];
        for(int j = 0, k = seqLen-1; j < seqLen; j++, k--)
            sequence[j] = tarSeqs[i][k];
        sequence[seqLen] = -1;
        reversedTarSeqs.push_back(sequence);
    }
    for( int i = 0; i < numNonTarSeqs; i++ ){
        seqLen = 0;
        while( nontarSeqs[i][seqLen] >= 0 )
            seqLen++;
        sequence = new short[seqLen+1];
        for(int j = 0, k = seqLen-1; j < seqLen; j++, k--)
            sequence[j] = nontarSeqs[i][k];
        sequence[seqLen] = -1;
        reversedNonTarSeqs.push_back(sequence);
    }
    //user abort
     if(pdProgDlg->wasCanceled()){
         // free memory
         for( int i = 0; i < 2; i++ ){
             for( int j = 0; j < numLprimers; j++ )
                delete[] lPrBindPosTarSeqs[i][j];
             for( int j = 0; j < numLprimers; j++ )
                delete[] lPrBindPosNonTarSeqs[i][j];
             delete[] lPrBindPosTarSeqs[i];
             delete[] lPrBindPosNonTarSeqs[i];
         }
         for( int i = 0; i < 2; i++ ){
             for( int j = 0; j < numRprimers; j++ )
                delete[] rPrBindPosTarSeqs[i][j];
             for( int j = 0; j < numRprimers; j++ )
                delete[] rPrBindPosNonTarSeqs[i][j];
             delete[] rPrBindPosTarSeqs[i];
             delete[] rPrBindPosNonTarSeqs[i];
         }
         delete[] lPrBindPosTarSeqs;
         delete[] lPrBindPosNonTarSeqs;
         delete[] rPrBindPosTarSeqs;
         delete[] rPrBindPosNonTarSeqs;
         for( int i = 0; i < numTarSeqs; i++ )
            delete[] reversedTarSeqs[i];
         for( int i = 0; i < numNonTarSeqs; i++ )
            delete[] reversedNonTarSeqs[i];
         reversedTarSeqs.clear();
         reversedNonTarSeqs.clear();

         return -1;
     }


    // finally start approxiation search: group primers with common prefix
    // do the search for a group
    PrimerQuickSearch* ptr = new PrimerQuickSearch();

    time = QTime::currentTime();
    timeStr = time.toString();
    logDesign += timeStr;
    logDesign += ":	Start searching for (single) primers in target sequences.\r\n\r\n";

    // 1. left primer candidates in target sequences==========================================================
    pdProgDlg->setRange(0, numLprimers + numRprimers);
    pdProgDlg->computation(3);

    pr1st = 0;	prlast = 1;
    while( prlast < numLprimers ){
        if( isPrefixLeftPrimer( lPrimerCandid[prlast-1], lPrimerCandid[prlast] ) )
            prlast++;
        else{ //Find the end of the original group and start a new group
            QString tmpSeq;
            tools::seqInt2Str(lPrimerCandid[prlast-1],tmpSeq);
            prNum = prlast - pr1st;
            // get a group
            for( int i = pr1st, j = 0; i < prlast; i++, j++ ){
                prLen = 0;
                while( lPrimerCandid[i][prLen] >= 0 )
                    prLen++;
                primerGroupLens[j] = prLen;
            }
            ptr->initMember( lPrimerCandid[prlast-1], prNum, primerGroupLens, LEFT_PRIMER );
            // start searching
            for( int i = 0; i < numTarSeqs; i++ ){
                ptr->searchPrimersInTarSeq( reversedTarSeqs[i] );
                for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
                    lPrBindPosTarSeqs[0][j][i] = ptr->bindingPosStart[k];
                    lPrBindPosTarSeqs[1][j][i] = ptr->bindingPosEnd[k];
                }
            }
            pr1st = prlast++;
            pdProgDlg->setValue(prlast);

            //user abort
            if(pdProgDlg->wasCanceled()){
                // free memory
                for( int i = 0; i < 2; i++ ){
                    for( int j = 0; j < numLprimers; j++ )
                        delete[] lPrBindPosTarSeqs[i][j];
                    for( int j = 0; j < numLprimers; j++ )
                        delete[] lPrBindPosNonTarSeqs[i][j];
                    delete[] lPrBindPosTarSeqs[i];
                    delete[] lPrBindPosNonTarSeqs[i];
                }
                for( int i = 0; i < 2; i++ ){
                    for( int j = 0; j < numRprimers; j++ )
                        delete[] rPrBindPosTarSeqs[i][j];
                    for( int j = 0; j < numRprimers; j++ )
                        delete[] rPrBindPosNonTarSeqs[i][j];
                    delete[] rPrBindPosTarSeqs[i];
                    delete[] rPrBindPosNonTarSeqs[i];
                }
                delete[] lPrBindPosTarSeqs;
                delete[] lPrBindPosNonTarSeqs;
                delete[] rPrBindPosTarSeqs;
                delete[] rPrBindPosNonTarSeqs;
                for( int i = 0; i < numTarSeqs; i++ )
                    delete[] reversedTarSeqs[i];
                for( int i = 0; i < numNonTarSeqs; i++ )
                    delete[] reversedNonTarSeqs[i];
                reversedTarSeqs.clear();
                reversedNonTarSeqs.clear();

                return -1;
            }
        }
    }
    prNum = prlast - pr1st;
    for( int i = pr1st, j = 0; i < prlast; i++, j++ ){
        prLen = 0;
        while( lPrimerCandid[i][prLen] >= 0 )
            prLen++;
        primerGroupLens[j] = prLen;
    }
    ptr->initMember( lPrimerCandid[prlast-1], prNum, primerGroupLens, LEFT_PRIMER );
    // start searching
    for( int i = 0; i < numTarSeqs; i++ ){
        ptr->searchPrimersInTarSeq( reversedTarSeqs[i] );
        for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
            lPrBindPosTarSeqs[0][j][i] = ptr->bindingPosStart[k];
            lPrBindPosTarSeqs[1][j][i] = ptr->bindingPosEnd[k];
        }
    }
    pdProgDlg->setValue(prlast);
//MX
    //user abort
    if(pdProgDlg->wasCanceled()){
        // free memory
        for( int i = 0; i < 2; i++ ){
            for( int j = 0; j < numLprimers; j++ )
                delete[] lPrBindPosTarSeqs[i][j];
            for( int j = 0; j < numLprimers; j++ )
                delete[] lPrBindPosNonTarSeqs[i][j];
            delete[] lPrBindPosTarSeqs[i];
            delete[] lPrBindPosNonTarSeqs[i];
        }
        for( int i = 0; i < 2; i++ ){
            for( int j = 0; j < numRprimers; j++ )
                delete[] rPrBindPosTarSeqs[i][j];
            for( int j = 0; j < numRprimers; j++ )
                delete[] rPrBindPosNonTarSeqs[i][j];
            delete[] rPrBindPosTarSeqs[i];
            delete[] rPrBindPosNonTarSeqs[i];
        }
        delete[] lPrBindPosTarSeqs;
        delete[] lPrBindPosNonTarSeqs;
        delete[] rPrBindPosTarSeqs;
        delete[] rPrBindPosNonTarSeqs;
        for( int i = 0; i < numTarSeqs; i++ )
            delete[] reversedTarSeqs[i];
        for( int i = 0; i < numNonTarSeqs; i++ )
            delete[] reversedNonTarSeqs[i];
        reversedTarSeqs.clear();
        reversedNonTarSeqs.clear();

        return -1;
    }
    // 2. right primer candidates in target sequences==========================================================
    pr1st = 0;	prlast = 1;
    while( prlast < numRprimers ){
        if( isPrefixRightPrimer( rPrimerCandid[prlast-1], rPrimerCandid[prlast] ) )
            prlast++;
        else{
            prNum = prlast - pr1st;
            // get a group
            for( int i = pr1st, j = 0; i < prlast; i++, j++ ){
                prLen = 0;
                while( rPrimerCandid[i][prLen] >= 0 )
                    prLen++;
                primerGroupLens[j] = prLen;
            }
            ptr->initMember( rPrimerCandid[prlast-1], prNum, primerGroupLens, RIGHT_PRIMER );
            // start searching
            for( int i = 0; i < numTarSeqs; i++ ){
                ptr->searchPrimersInTarSeq( tarSeqs[i] );
                for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
                    rPrBindPosTarSeqs[0][j][i] = ptr->bindingPosStart[k];
                    rPrBindPosTarSeqs[1][j][i] = ptr->bindingPosEnd[k];
                }
            }
            pr1st = prlast++;
            pdProgDlg->setValue(prlast+numLprimers)
                    ;
            //user abort
            if(pdProgDlg->wasCanceled()){
                // free memory
                for( int i = 0; i < 2; i++ ){
                    for( int j = 0; j < numLprimers; j++ )
                        delete[] lPrBindPosTarSeqs[i][j];
                    for( int j = 0; j < numLprimers; j++ )
                        delete[] lPrBindPosNonTarSeqs[i][j];
                    delete[] lPrBindPosTarSeqs[i];
                    delete[] lPrBindPosNonTarSeqs[i];
                }
                for( int i = 0; i < 2; i++ ){
                    for( int j = 0; j < numRprimers; j++ )
                            delete[] rPrBindPosTarSeqs[i][j];
                    for( int j = 0; j < numRprimers; j++ )
                            delete[] rPrBindPosNonTarSeqs[i][j];
                    delete[] rPrBindPosTarSeqs[i];
                    delete[] rPrBindPosNonTarSeqs[i];
                }
                delete[] lPrBindPosTarSeqs;
                delete[] lPrBindPosNonTarSeqs;
                delete[] rPrBindPosTarSeqs;
                delete[] rPrBindPosNonTarSeqs;
                for( int i = 0; i < numTarSeqs; i++ )
                    delete[] reversedTarSeqs[i];
                for( int i = 0; i < numNonTarSeqs; i++ )
                    delete[] reversedNonTarSeqs[i];
                reversedTarSeqs.clear();
                reversedNonTarSeqs.clear();

                return -1;
            }
        }
    }
    prNum = prlast - pr1st;
    for( int i = pr1st, j = 0; i < prlast; i++, j++ ){
        prLen = 0;
        while( rPrimerCandid[i][prLen] >= 0 )
            prLen++;
        primerGroupLens[j] = prLen;
    }
    ptr->initMember( rPrimerCandid[prlast-1], prNum, primerGroupLens, RIGHT_PRIMER );
    // start searching
    for( int i = 0; i < numTarSeqs; i++ ){
        ptr->searchPrimersInTarSeq( tarSeqs[i] );
        for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
            rPrBindPosTarSeqs[0][j][i] = ptr->bindingPosStart[k];
            rPrBindPosTarSeqs[1][j][i] = ptr->bindingPosEnd[k];
        }
    }
    pdProgDlg->setValue(prlast+numLprimers);
    //user abort
    if(pdProgDlg->wasCanceled()){
        // free memory
        for( int i = 0; i < 2; i++ ){
            for( int j = 0; j < numLprimers; j++ )
                delete[] lPrBindPosTarSeqs[i][j];
            for( int j = 0; j < numLprimers; j++ )
                delete[] lPrBindPosNonTarSeqs[i][j];
            delete[] lPrBindPosTarSeqs[i];
            delete[] lPrBindPosNonTarSeqs[i];
        }
        for( int i = 0; i < 2; i++ ){
            for( int j = 0; j < numRprimers; j++ )
                delete[] rPrBindPosTarSeqs[i][j];
            for( int j = 0; j < numRprimers; j++ )
                delete[] rPrBindPosNonTarSeqs[i][j];
            delete[] rPrBindPosTarSeqs[i];
            delete[] rPrBindPosNonTarSeqs[i];
        }
        delete[] lPrBindPosTarSeqs;
        delete[] lPrBindPosNonTarSeqs;
        delete[] rPrBindPosTarSeqs;
        delete[] rPrBindPosNonTarSeqs;
        for( int i = 0; i < numTarSeqs; i++ )
            delete[] reversedTarSeqs[i];
        for( int i = 0; i < numNonTarSeqs; i++ )
            delete[] reversedNonTarSeqs[i];
        reversedTarSeqs.clear();
        reversedNonTarSeqs.clear();

        return -1;
    }

    time = QTime::currentTime();
    timeStr = time.toString();
    logDesign += timeStr;
    logDesign += ":	Start searching for (single) primers in non-target sequences.\r\n\r\n";

    // 3. left primer candidates in non-target sequences=======================================================
    pdProgDlg->setRange(0, numLprimers + numRprimers);
    pdProgDlg->computation(4);

    pr1st = 0;	prlast = 1;
    while( prlast < numLprimers ){
        if( isPrefixLeftPrimer( lPrimerCandid[prlast-1], lPrimerCandid[prlast] ) )
            prlast++;
        else{
            prNum = prlast - pr1st;
            // get a group
            for( int i = pr1st, j = 0; i < prlast; i++, j++ ){
                prLen = 0;
                while( lPrimerCandid[i][prLen] >= 0 )
                    prLen++;
                primerGroupLens[j] = prLen;
            }
            ptr->initMember( lPrimerCandid[prlast-1], prNum, primerGroupLens, LEFT_PRIMER );
            // start searching
            for( int i = 0; i < numNonTarSeqs; i++ ){
                ptr->searchPrimersInNonTarSeq( reversedNonTarSeqs[i] );
                for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
                    lPrBindPosNonTarSeqs[0][j][i] = ptr->bindingPosStart[k];
                    lPrBindPosNonTarSeqs[1][j][i] = ptr->bindingPosEnd[k];
                }
            }
            pr1st = prlast++;
            pdProgDlg->setValue(prlast);

            //user abort
            if(pdProgDlg->wasCanceled()){
                // free memory
                for( int i = 0; i < 2; i++ ){
                    for( int j = 0; j < numLprimers; j++ )
                        delete[] lPrBindPosTarSeqs[i][j];
                    for( int j = 0; j < numLprimers; j++ )
                        delete[] lPrBindPosNonTarSeqs[i][j];
                    delete[] lPrBindPosTarSeqs[i];
                    delete[] lPrBindPosNonTarSeqs[i];
                }
                for( int i = 0; i < 2; i++ ){
                    for( int j = 0; j < numRprimers; j++ )
                        delete[] rPrBindPosTarSeqs[i][j];
                    for( int j = 0; j < numRprimers; j++ )
                        delete[] rPrBindPosNonTarSeqs[i][j];
                    delete[] rPrBindPosTarSeqs[i];
                    delete[] rPrBindPosNonTarSeqs[i];
                }
                delete[] lPrBindPosTarSeqs;
                delete[] lPrBindPosNonTarSeqs;
                delete[] rPrBindPosTarSeqs;
                delete[] rPrBindPosNonTarSeqs;
                for( int i = 0; i < numTarSeqs; i++ )
                    delete[] reversedTarSeqs[i];
                for( int i = 0; i < numNonTarSeqs; i++ )
                    delete[] reversedNonTarSeqs[i];
                reversedTarSeqs.clear();
                reversedNonTarSeqs.clear();

                return -1;
            }
        }
    }
    prNum = prlast - pr1st;
    for( int i = pr1st, j = 0; i < prlast; i++, j++ ){
        prLen = 0;
        while( lPrimerCandid[i][prLen] >= 0 )
            prLen++;
        primerGroupLens[j] = prLen;
    }
    ptr->initMember( lPrimerCandid[prlast-1], prNum, primerGroupLens, LEFT_PRIMER );
    // start searching
    for( int i = 0; i < numNonTarSeqs; i++ ){
        ptr->searchPrimersInNonTarSeq( reversedNonTarSeqs[i] );
        for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
            lPrBindPosNonTarSeqs[0][j][i] = ptr->bindingPosStart[k];
            lPrBindPosNonTarSeqs[1][j][i] = ptr->bindingPosEnd[k];
        }
    }
    pdProgDlg->setValue(prlast);
    //user abort
    if(pdProgDlg->wasCanceled()){
        // free memory
        for( int i = 0; i < 2; i++ ){
            for( int j = 0; j < numLprimers; j++ )
                delete[] lPrBindPosTarSeqs[i][j];
            for( int j = 0; j < numLprimers; j++ )
                delete[] lPrBindPosNonTarSeqs[i][j];
            delete[] lPrBindPosTarSeqs[i];
            delete[] lPrBindPosNonTarSeqs[i];
        }
        for( int i = 0; i < 2; i++ ){
            for( int j = 0; j < numRprimers; j++ )
                delete[] rPrBindPosTarSeqs[i][j];
            for( int j = 0; j < numRprimers; j++ )
                delete[] rPrBindPosNonTarSeqs[i][j];
            delete[] rPrBindPosTarSeqs[i];
            delete[] rPrBindPosNonTarSeqs[i];
        }
        delete[] lPrBindPosTarSeqs;
        delete[] lPrBindPosNonTarSeqs;
        delete[] rPrBindPosTarSeqs;
        delete[] rPrBindPosNonTarSeqs;
        for( int i = 0; i < numTarSeqs; i++ )
            delete[] reversedTarSeqs[i];
        for( int i = 0; i < numNonTarSeqs; i++ )
            delete[] reversedNonTarSeqs[i];
        reversedTarSeqs.clear();
        reversedNonTarSeqs.clear();

        return -1;
    }
//MX
    // 4. right primer candidates in non-target sequences======================================================
    pr1st = 0;	prlast = 1;
    while( prlast < numRprimers ){
        if( isPrefixRightPrimer( rPrimerCandid[prlast-1], rPrimerCandid[prlast] ) )
                prlast++;
        else{
            prNum = prlast - pr1st;
            // get a group
            for( int i = pr1st, j = 0; i < prlast; i++, j++ ){
                prLen = 0;
                while( rPrimerCandid[i][prLen] >= 0 )
                    prLen++;
                primerGroupLens[j] = prLen;
            }
            ptr->initMember( rPrimerCandid[prlast-1], prNum, primerGroupLens, RIGHT_PRIMER );
            // start searching
            for( int i = 0; i < numNonTarSeqs; i++ ){
                ptr->searchPrimersInNonTarSeq( nontarSeqs[i] );
                for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
                    rPrBindPosNonTarSeqs[0][j][i] = ptr->bindingPosStart[k];
                    rPrBindPosNonTarSeqs[1][j][i] = ptr->bindingPosEnd[k];
                }
            }
            pr1st = prlast++;
            pdProgDlg->setValue(prlast+numLprimers);
            //user abort

            if(pdProgDlg->wasCanceled()){
                // free memory
                for( int i = 0; i < 2; i++ ){
                    for( int j = 0; j < numLprimers; j++ )
                        delete[] lPrBindPosTarSeqs[i][j];
                    for( int j = 0; j < numLprimers; j++ )
                        delete[] lPrBindPosNonTarSeqs[i][j];
                    delete[] lPrBindPosTarSeqs[i];
                    delete[] lPrBindPosNonTarSeqs[i];
                }
                for( int i = 0; i < 2; i++ ){
                    for( int j = 0; j < numRprimers; j++ )
                        delete[] rPrBindPosTarSeqs[i][j];
                    for( int j = 0; j < numRprimers; j++ )
                        delete[] rPrBindPosNonTarSeqs[i][j];
                    delete[] rPrBindPosTarSeqs[i];
                    delete[] rPrBindPosNonTarSeqs[i];
                }
                delete[] lPrBindPosTarSeqs;
                delete[] lPrBindPosNonTarSeqs;
                delete[] rPrBindPosTarSeqs;
                delete[] rPrBindPosNonTarSeqs;
                for( int i = 0; i < numTarSeqs; i++ )
                    delete[] reversedTarSeqs[i];
                for( int i = 0; i < numNonTarSeqs; i++ )
                    delete[] reversedNonTarSeqs[i];
                reversedTarSeqs.clear();
                reversedNonTarSeqs.clear();

                return -1;
            }
        }
    }
    prNum = prlast - pr1st;
    for( int i = pr1st, j = 0; i < prlast; i++, j++ ){
        prLen = 0;
        while( rPrimerCandid[i][prLen] >= 0 )
            prLen++;
        primerGroupLens[j] = prLen;
    }
    ptr->initMember( rPrimerCandid[prlast-1], prNum, primerGroupLens, RIGHT_PRIMER );
    // start searching
    for( int i = 0; i < numNonTarSeqs; i++ ){
        ptr->searchPrimersInNonTarSeq( nontarSeqs[i] );
        for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
            rPrBindPosNonTarSeqs[0][j][i] = ptr->bindingPosStart[k];
            rPrBindPosNonTarSeqs[1][j][i] = ptr->bindingPosEnd[k];
        }
    }

    pdProgDlg->setValue(prlast+numLprimers);
    //user abort
    if(pdProgDlg->wasCanceled()){
        // free memory
        for( int i = 0; i < 2; i++ ){
            for( int j = 0; j < numLprimers; j++ )
                delete[] lPrBindPosTarSeqs[i][j];
            for( int j = 0; j < numLprimers; j++ )
                delete[] lPrBindPosNonTarSeqs[i][j];
            delete[] lPrBindPosTarSeqs[i];
            delete[] lPrBindPosNonTarSeqs[i];
        }
        for( int i = 0; i < 2; i++ ){
            for( int j = 0; j < numRprimers; j++ )
                delete[] rPrBindPosTarSeqs[i][j];
            for( int j = 0; j < numRprimers; j++ )
                delete[] rPrBindPosNonTarSeqs[i][j];
            delete[] rPrBindPosTarSeqs[i];
            delete[] rPrBindPosNonTarSeqs[i];
        }
        delete[] lPrBindPosTarSeqs;
        delete[] lPrBindPosNonTarSeqs;
        delete[] rPrBindPosTarSeqs;
        delete[] rPrBindPosNonTarSeqs;
        for( int i = 0; i < numTarSeqs; i++ )
            delete[] reversedTarSeqs[i];
        for( int i = 0; i < numNonTarSeqs; i++ )
            delete[] reversedNonTarSeqs[i];
        reversedTarSeqs.clear();
        reversedNonTarSeqs.clear();

        return -1;
    }

    delete ptr;


    // finish searching========================================================================================
    // prepare frequency for single primer in target and non-target sequences
    // record how many times a single primer occurs in target sequences and non-target sequence
    float occ;
    for( int i = 0; i < numLprimers; i++ ){
        occ = 0;
        for( int j = 0; j < numTarSeqs; j++ ){
            if( lPrBindPosTarSeqs[0][i][j] >= 0 )
                occ++;
        }
        assert(numTarSeqs >= occ);
        lPrPctgTarSeqs.push_back( numTarSeqs > 0?(occ/numTarSeqs * 100):0 );
        occ = 0;
        for( int j = 0; j < numNonTarSeqs; j++ ){
            if( lPrBindPosNonTarSeqs[0][i][j] >= 0 )
                occ++;
        }
        assert(numNonTarSeqs >= occ);
        lPrPctgNonTarSeqs.push_back( numNonTarSeqs > 0?(occ/numNonTarSeqs * 100):0 );
    }

    for( int i = 0; i < numRprimers; i++ ){
        occ = 0;
        for( int j = 0; j < numTarSeqs; j++ ){
            if( rPrBindPosTarSeqs[0][i][j] >= 0 )
                occ++;
        }
        assert(numTarSeqs >= occ);
        rPrPctgTarSeqs.push_back( numTarSeqs > 0?(occ/numTarSeqs * 100):0 );
        occ = 0;
        for( int j = 0; j < numNonTarSeqs; j++ ){
            if( rPrBindPosNonTarSeqs[0][i][j] >= 0 )
                occ++;
        }
        assert(numNonTarSeqs >= occ);
        rPrPctgNonTarSeqs.push_back( numNonTarSeqs > 0?(occ/numNonTarSeqs * 100):0);
    }

    pdProgDlg->setValue(0);
    //user abort
    if(pdProgDlg->wasCanceled()){
        // free memory
        for( int i = 0; i < 2; i++ ){
            for( int j = 0; j < numLprimers; j++ )
                delete[] lPrBindPosTarSeqs[i][j];
            for( int j = 0; j < numLprimers; j++ )
                delete[] lPrBindPosNonTarSeqs[i][j];
            delete[] lPrBindPosTarSeqs[i];
            delete[] lPrBindPosNonTarSeqs[i];
        }
        for( int i = 0; i < 2; i++ ){
            for( int j = 0; j < numRprimers; j++ )
                delete[] rPrBindPosTarSeqs[i][j];
            for( int j = 0; j < numRprimers; j++ )
                delete[] rPrBindPosNonTarSeqs[i][j];
            delete[] rPrBindPosTarSeqs[i];
            delete[] rPrBindPosNonTarSeqs[i];
        }
        delete[] lPrBindPosTarSeqs;
        delete[] lPrBindPosNonTarSeqs;
        delete[] rPrBindPosTarSeqs;
        delete[] rPrBindPosNonTarSeqs;
        for( int i = 0; i < numTarSeqs; i++ )
            delete[] reversedTarSeqs[i];
        for( int i = 0; i < numNonTarSeqs; i++ )
            delete[] reversedNonTarSeqs[i];
        reversedTarSeqs.clear();
        reversedNonTarSeqs.clear();
        lPrPctgTarSeqs.clear();
        rPrPctgTarSeqs.clear();
        lPrPctgNonTarSeqs.clear();
        rPrPctgNonTarSeqs.clear();

        return -1;
    }

    // free memory for reversed sequences
    for( int i = 0; i < numTarSeqs; i++ )
        delete[] reversedTarSeqs[i];
    for( int i = 0; i < numNonTarSeqs; i++ )
        delete[] reversedNonTarSeqs[i];
    reversedTarSeqs.clear();
    reversedNonTarSeqs.clear();


    // pair off primer candidates=============================================================================
    int num;
    float numDouble;
    int maxProductSize;
    int minProductSize;
    int sumProductSize;
    int productSize;
    int bdPos[4];
    int nPairsLeft[5];
    // fill searching result in primer list
    pdProgDlg->computation(5);
    pdProgDlg->setRange(0,numLprimers);

    //int numPrimerPairList = primerPairList.size();
    PrimerPair *pair;
    QVector<short> randSelSeqs;

    // first prepare average annealing position for all candidate in target and non-target sequences.
    // this is for later, pair each left with right primer, if left average annealing position appear
    // to more right to the right primer, don't consider this pair
    float* lPrBindPosTarSeqsAvg;
    float* rPrBindPosTarSeqsAvg;
    try{
        lPrBindPosTarSeqsAvg = new float[numLprimers]; //average position of left primers
        rPrBindPosTarSeqsAvg = new float[numRprimers]; //average position of right primers
    }catch(  std::bad_alloc& e){
        QMessageBox::about(0, "Fatal error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate");
        /*Clean Memory: (for Error)
          1. Target&Nontar Seqs
          2. Primer Candidates
          3. Primer Pairs
          4. probe Candidates
          5. probes
          6. Triples (in Primer Pair)
          7. T & P
        */
        DataTools::cleanMemoryAllData();
        // free memory
        for( int i = 0; i < 2; i++ ){
            for( int j = 0; j < numLprimers; j++ )
                delete[] lPrBindPosTarSeqs[i][j];
            for( int j = 0; j < numLprimers; j++ )
                delete[] lPrBindPosNonTarSeqs[i][j];
            delete[] lPrBindPosTarSeqs[i];
            delete[] lPrBindPosNonTarSeqs[i];
        }
        for( int i = 0; i < 2; i++ ){
            for( int j = 0; j < numRprimers; j++ )
                delete[] rPrBindPosTarSeqs[i][j];
            for( int j = 0; j < numRprimers; j++ )
                delete[] rPrBindPosNonTarSeqs[i][j];
            delete[] rPrBindPosTarSeqs[i];
            delete[] rPrBindPosNonTarSeqs[i];
        }
        delete[] lPrBindPosTarSeqs;
        delete[] lPrBindPosNonTarSeqs;
        delete[] rPrBindPosTarSeqs;
        delete[] rPrBindPosNonTarSeqs;
        lPrPctgTarSeqs.clear();
        rPrPctgTarSeqs.clear();
        lPrPctgNonTarSeqs.clear();
        rPrPctgNonTarSeqs.clear();

        return -1;
    }

    for( int i = 0; i < numLprimers; i++ ){
        numDouble = 0; num = 0;
        for( int j = 0; j < numTarSeqs; j++ ){
            if( lPrBindPosTarSeqs[0][i][j] >= 0 ){
                num++;
                numDouble += lPrBindPosTarSeqs[1][i][j];  // left primer check ending position
            }
        }
        lPrBindPosTarSeqsAvg[i] = num>0?(numDouble / num):0;
    }
    for( int i = 0; i < numRprimers; i++ ){
        numDouble = 0;	num = 0;
        for( int j = 0; j < numTarSeqs; j++ ){
            if( rPrBindPosTarSeqs[0][i][j] >= 0 ){
                num++;
                numDouble += rPrBindPosTarSeqs[0][i][j];  // right primer check starting position
            }
        }
        rPrBindPosTarSeqsAvg[i] = num>0?(numDouble / num):0;
    }



    // for information of primer design report window
    for( int i = 0; i < 5; i++ )
        nPairsLeft[i] = 0;

    time = QTime::currentTime();
    timeStr = time.toString();
    logDesign += timeStr;

    // pair each left primer with each right primer in all possible ways and check if this pair
    // is acceptable

    /*if( primerEstimate){	// compute estimated PCR information
        for( int i = 0; i < numLprimers; i++ ){
            for( int j = 0; j < numRprimers; j++ ){

                // first check if this primer pair is possible to be a pair
                // skip if left primer ending position is on the right of right primer starting postion
                if( lPrBindPosTarSeqsAvg[i] >= rPrBindPosTarSeqsAvg[j] )
                    continue;
                try{
                    pair = new PrimerPair(); //create a primer pair for it.
                    pair->posBindingTar = new short*[2];
                    pair->posBindingNontar = new short*[2];
                }catch(  std::bad_alloc& e ){
                    QMessageBox::about(0,"Fatal error","Fatal error: out of memory.\nThe possible solutions:\n"
                                       "1. Install more physical memory.\n2. Close all other applications.\n"
                                       "3. Assign stricter primer settings.\n"
                                       "4. Compute primers for a single length at a time.\n"
                                       "Program is forced to terminate");
                    // free memory
                    for( int i = 0; i < 2; i++ ){
                        for( int j = 0; j < numLprimers; j++ )
                            delete[] lPrBindPosTarSeqs[i][j];
                        for( int j = 0; j < numLprimers; j++ )
                            delete[] lPrBindPosNonTarSeqs[i][j];
                        delete[] lPrBindPosTarSeqs[i];
                        delete[] lPrBindPosNonTarSeqs[i];
                    }
                    for( int i = 0; i < 2; i++ ){
                        for( int j = 0; j < numRprimers; j++ )
                            delete[] rPrBindPosTarSeqs[i][j];
                        for( int j = 0; j < numRprimers; j++ )
                            delete[] rPrBindPosNonTarSeqs[i][j];
                        delete[] rPrBindPosTarSeqs[i];
                        delete[] rPrBindPosNonTarSeqs[i];
                    }
                    delete[] lPrBindPosTarSeqs;
                    delete[] lPrBindPosNonTarSeqs;
                    delete[] rPrBindPosTarSeqs;
                    delete[] rPrBindPosNonTarSeqs;
                    delete[] lPrBindPosTarSeqsAvg;
                    delete[] rPrBindPosTarSeqsAvg;

                    lPrPctgTarSeqs.clear();
                    rPrPctgTarSeqs.clear();
                    lPrPctgNonTarSeqs.clear();
                    rPrPctgNonTarSeqs.clear();
                    return FATAL_ERROR;
                }

                tools::seqInt2Str(lPrimerCandid[i],pair->left);
                tools::seqInt2Str(rPrimerCandid[j],pair->right);

                //if(pair->left == "ATACCATTTACAACCGT" && pair->right == "CAAAATGAATCAAAACTTTC")
                //    int breakpt = 3;

                if( pair->left == pair->right ){
                    delete pair;
                    continue;
                }
                tools::seqInt2StrReverseComplement(rPrimerCandid[j],pair->rightRevsComp);

                // compute frequencies for primer pair in target sequences
                // only pick 100 random target sequences to consider
                rand100position( numTarSeqs, randSelSeqs );
                if( (int)randSelSeqs.size() < numTarSeqs )
                    pair->isEstimateTar = true;
                else
                    pair->isEstimateTar = false;   //# of target sequences is small: check in all target sequences
                num = 0;
                maxProductSize = 0;	minProductSize = INT_MAX;	sumProductSize = 0;
                for( int k = 0; k < randSelSeqs.size(); k++ ){
                    int idx = randSelSeqs[k];
                    bdPos[0] = lPrBindPosTarSeqs[0][i][idx];
                    bdPos[1] = lPrBindPosTarSeqs[1][i][idx];
                    bdPos[2] = rPrBindPosTarSeqs[0][j][idx];
                    bdPos[3] = rPrBindPosTarSeqs[1][j][idx];

                    // left and right primer both appear and positions are correct
                    // left primer comes before right primer, no overlap
                    if( bdPos[0] >= 0 && bdPos[2] >=0 && bdPos[1] < bdPos[2] ){
                        num++;
                        productSize = bdPos[3] - bdPos[0] + 1;
                        if( minProductSize > productSize )	minProductSize = productSize;
                        if( maxProductSize < productSize )	maxProductSize = productSize;
                        sumProductSize += productSize;
                    }
                }
                if( randSelSeqs.size() != 0 )
                    pair->pctgInTargetSeqs = (float)num / randSelSeqs.size() * 100;
                else
                    pair->pctgInTargetSeqs = 0;
                if( num == 0 ){
                    pair->pcrSizeAvgTarSeqs = 0;
                    pair->pcrSizeMinTarSeqs = 0;
                    pair->pcrSizeMaxTarSeqs = 0;
                }
                else{
                    pair->pcrSizeAvgTarSeqs = (float)sumProductSize / num;
                    pair->pcrSizeMinTarSeqs = minProductSize;
                    pair->pcrSizeMaxTarSeqs = maxProductSize;
                }

                // compute frequencies for primer pair in non-target sequences
                // only pick 100 random non-target sequences to consider
                rand100position( numNonTarSeqs, randSelSeqs );
                if( (int)randSelSeqs.size() < numNonTarSeqs )
                    pair->isEstimateNonTar = 1;
                else
                    pair->isEstimateNonTar = 0;
                num = 0;
                maxProductSize = 0;	minProductSize = INT_MAX;	sumProductSize = 0;
                for( int k = 0; k < randSelSeqs.size(); k++ ){
                    int idx = randSelSeqs[k];
                    bdPos[0] = lPrBindPosNonTarSeqs[0][i][randSelSeqs[k]];
                    bdPos[1] = lPrBindPosNonTarSeqs[1][i][randSelSeqs[k]];
                    bdPos[2] = rPrBindPosNonTarSeqs[0][j][randSelSeqs[k]];
                    bdPos[3] = rPrBindPosNonTarSeqs[1][j][randSelSeqs[k]];

                    // left and right primer both appear and positions are correct
                    if( bdPos[0] >= 0 && bdPos[2] >=0 && bdPos[1] < bdPos[2] ){
                        num++;

                        productSize = bdPos[3] - bdPos[0] + 1;
                        if( minProductSize > productSize )	minProductSize = productSize;
                        if( maxProductSize < productSize )	maxProductSize = productSize;
                        sumProductSize += productSize;
                    }
                }



                if( randSelSeqs.size() != 0 )
                        pair->pctgInNonTarSeqs = (float)num / randSelSeqs.size() * 100;
                else
                    pair->pctgInNonTarSeqs = 0;

                if( num == 0 ){
                    pair->pcrSizeAvgNonTarSeqs = 0;
                    pair->pcrSizeMinNonTarSeqs = 0;
                    pair->pcrSizeMaxNonTarSeqs = 0;
                }
                else{
                    pair->pcrSizeAvgNonTarSeqs = (float)sumProductSize / num;
                    pair->pcrSizeMinNonTarSeqs = minProductSize;
                    pair->pcrSizeMaxNonTarSeqs = maxProductSize;
                }


                if( pair->pcrSizeAvgTarSeqs > 30 )
                    nPairsLeft[0]++;

                // filtering:
                // 1. pcr product size
                if( ( pair->pcrSizeAvgTarSeqs < primerPcrSizeMin || pair->pcrSizeAvgTarSeqs > primerPcrSizeMax ) ){
                        //&& ( pair->pcrSizeAvgNonTarSeqs < pcrSizeMin || pair->pcrSizeAvgNonTarSeqs > pcrSizeMax )  ) {             
                    delete pair;
                    continue;
                }
                nPairsLeft[1]++;

                // 2. Tm difference
                pair->tmLeft = meltingTemp(pair->left);
                pair->tmRight = meltingTemp(pair->right);
                numDouble = pair->tmLeft - pair->tmRight;
                numDouble = fabs(numDouble);
                if( numDouble > primerTmDiff ){
                    delete pair;
                    continue;
                }
                nPairsLeft[2]++;

                //3. primer inter-complementarity
                pair->prPrComp = primerInterComplementarity(pair->left, pair->rightRevsComp);
                if( pair->prPrComp > primerMaxPrPrComp ){
                    delete pair;
                    continue;
                }
                nPairsLeft[3]++;

                //4. primer 3' inter-complementarity
                pair->prPr3Comp = primer3InterComplementarity(pair->left, pair->rightRevsComp);
                if( pair->prPr3Comp > primerMax3PrPrComp ){
                    delete pair;
                    continue;
                }
                nPairsLeft[4]++;

                // GC content
                pair->GCLeft = GCcontent(pair->left);
                pair->GCRight = GCcontent(pair->right);

                // primer self-complementarity
                pair->selfCompLeft = ppIntraComplementarity(pair->left);
                pair->selfCompRight = ppIntraComplementarity(pair->rightRevsComp);

                // primer 3' self-complementarity
                pair->self3CompLeft = pp3IntraComplementarity(pair->left, primerIntraBase3end);
                pair->self3CompRight = pp3IntraComplementarity(pair->rightRevsComp, primerIntraBase3end);

                // sigle primer average annealing positions in target
                // note lPrBindPosTarSeqsAvg[i] saves ending position for left primers,
                // need to change it to starting position for left primers. right primer is fine.
                pair->bindPosTarSeqsAvgLeft = lPrBindPosTarSeqsAvg[i] - pair->left.length() + 1;
                pair->bindPosTarSeqsAvgRight = rPrBindPosTarSeqsAvg[j];

                // single primer frequency in target and non-target sequences
                pair->pctgInTarSeqsLeft = lPrPctgTarSeqs[i];
                pair->pctgInNonTarSeqsLeft = lPrPctgNonTarSeqs[i];
                pair->pctgInTarSeqsRight = rPrPctgTarSeqs[j];
                pair->pctgInNonTarSeqsRight = rPrPctgNonTarSeqs[j];

                pair->marked = 0;

                // record binding position for each seqs
                pair->posBindingTar[0] =  lPrBindPosTarSeqs[1][i];
                pair->posBindingTar[1] =  rPrBindPosTarSeqs[0][j];
                pair->posBindingNontar[0] = lPrBindPosNonTarSeqs[1][i];
                pair->posBindingNontar[1] = rPrBindPosNonTarSeqs[0][j];

                primerPairList.push_back(pair);
            }
            pdProgDlg->setValue(i+1);
            //pdProgDlg->update();
            //user abort
            if(pdProgDlg->wasCanceled())
            {
                // free memory
                for( int i = 0; i < 2; i++ ){
                    for( int j = 0; j < numLprimers; j++ )
                        delete[] lPrBindPosTarSeqs[i][j];
                    for( int j = 0; j < numLprimers; j++ )
                        delete[] lPrBindPosNonTarSeqs[i][j];
                    delete[] lPrBindPosTarSeqs[i];
                    delete[] lPrBindPosNonTarSeqs[i];
                }
                for( int i = 0; i < 2; i++ ){
                    for( int j = 0; j < numRprimers; j++ )
                        delete[] rPrBindPosTarSeqs[i][j];
                    for( int j = 0; j < numRprimers; j++ )
                        delete[] rPrBindPosNonTarSeqs[i][j];
                    delete[] rPrBindPosTarSeqs[i];
                    delete[] rPrBindPosNonTarSeqs[i];
                }
                delete[] lPrBindPosTarSeqs;
                delete[] lPrBindPosNonTarSeqs;
                delete[] rPrBindPosTarSeqs;
                delete[] rPrBindPosNonTarSeqs;
                delete[] lPrBindPosTarSeqsAvg;
                delete[] rPrBindPosTarSeqsAvg;
                lPrPctgTarSeqs.clear();
                rPrPctgTarSeqs.clear();
                lPrPctgNonTarSeqs.clear();
                rPrPctgNonTarSeqs.clear();

                return -1;
            }
        }
    }
    else{*/	// compute exact PCR information

        priority_queue <PrimerPair*, vector<PrimerPair*>, ComparePairs > tmpPrimerPairList;
        // In order to control memory usage, we don't save all primer pairs
        // We save pairs into a priority queue and only keep the best 30000


        for( int i = 0; i < numLprimers; i++ ){
            for( int j = 0; j < numRprimers; j++ ){

                // first check if this primer pair is possible to be a pair
                // skip if left primer ending position is on the right of right primer starting postion
                if( lPrBindPosTarSeqsAvg[i] >= rPrBindPosTarSeqsAvg[j] )
                      continue;

                try{
                    pair = new PrimerPair();
                    pair->posBindingTar = new short*[2];
                    pair->posBindingNontar = new short*[2];
                    for(int k = 0; k < 2; k++){
                        pair->posBindingTar[k] = new short[numTarSeqs];
                        pair->posBindingNontar[k] = new short[numNonTarSeqs];
                    }
                }
                catch(  std::bad_alloc& e ){
                    QMessageBox::about(0,"Fatal error", "Fatal error: out of memory.\nThe possible solutions:\n"
                                "1. Install more physical memory.\n2. Close all other applications.\n"
                                "3. Assign stricter primer settings.\n"
                                "4. Compute primers for a single length at a time.\n"
                                "Program is forced to terminate.");
                    // free memory
                    for( int i = 0; i < 2; i++ ){
                        for( int j = 0; j < numLprimers; j++ )
                                delete[] lPrBindPosTarSeqs[i][j];
                        for( int j = 0; j < numLprimers; j++ )
                                delete[] lPrBindPosNonTarSeqs[i][j];
                        delete[] lPrBindPosTarSeqs[i];
                        delete[] lPrBindPosNonTarSeqs[i];
                    }
                    for( int i = 0; i < 2; i++ ){
                        for( int j = 0; j < numRprimers; j++ )
                                delete[] rPrBindPosTarSeqs[i][j];
                        for( int j = 0; j < numRprimers; j++ )
                                delete[] rPrBindPosNonTarSeqs[i][j];
                        delete[] rPrBindPosTarSeqs[i];
                        delete[] rPrBindPosNonTarSeqs[i];
                    }
                    delete[] lPrBindPosTarSeqs;
                    delete[] lPrBindPosNonTarSeqs;
                    delete[] rPrBindPosTarSeqs;
                    delete[] rPrBindPosNonTarSeqs;
                    delete[] lPrBindPosTarSeqsAvg;
                    delete[] rPrBindPosTarSeqsAvg;
                    lPrPctgTarSeqs.clear();
                    rPrPctgTarSeqs.clear();
                    lPrPctgNonTarSeqs.clear();
                    rPrPctgNonTarSeqs.clear();

                    return -1;
                }
                tools::seqInt2Str(lPrimerCandid[i],pair->left);
                tools::seqInt2Str(rPrimerCandid[j],pair->right);

                if( pair->left == pair->right ){
                        delete pair;
                        continue;
                }
                tools::seqInt2StrReverseComplement(rPrimerCandid[j],pair->rightRevsComp);

                // compute frequencies for primer pair in target sequences
                num = 0;
                maxProductSize = 0;	minProductSize = INT_MAX;	sumProductSize = 0;
                int numAmbiTargetsLeft = 0;
                int numAmbiTargetsRight = 0;
                int numAmbiBasesLeft = 0;
                int numAmbiBasesRight = 0;
                bool flagLeft;
                bool flagRight;

                for( int k = 0; k < numTarSeqs; k++ ){
                    bdPos[0] = lPrBindPosTarSeqs[0][i][k];
                    bdPos[1] = lPrBindPosTarSeqs[1][i][k];
                    bdPos[2] = rPrBindPosTarSeqs[0][j][k];
                    bdPos[3] = rPrBindPosTarSeqs[1][j][k];

                    // left and right primer both appear and positions are correct
                    if( bdPos[0] >= 0 && bdPos[2] >=0 && bdPos[1] < bdPos[2] ){
                        num++;

                        productSize = bdPos[3] - bdPos[0] + 1;
                        if( minProductSize > productSize )	minProductSize = productSize;
                        if( maxProductSize < productSize )	maxProductSize = productSize;
                        sumProductSize += productSize;
                    }

                    flagLeft = false;
                    flagRight = false;
                    for(int m = bdPos[0] ; m <= bdPos[1]; m++){
                        if(tarSeqs[k][m] >3){
                            numAmbiBasesLeft++;
                            flagLeft = true;
                        }    
                    }
                    if(flagLeft){
                        numAmbiTargetsLeft++;
                    }
                    for(int m = bdPos[2] ; m <= bdPos[3]; m++){
                        if(tarSeqs[k][m] >3){
                            numAmbiBasesRight++;
                            flagRight = true;
                        }
                    }
                    if(flagRight){
                        numAmbiTargetsRight++;
                    }
                }
                assert(numTarSeqs >= num);
                //if( numTarSeqs != 0 )
                pair->pctgInTargetSeqs = numTarSeqs>0?((float)num / numTarSeqs * 100):0;
                //else
                    //pair->pctgInTargetSeqs = 0;

                pair->pcrSizeAvgTarSeqs = num>0?((float)sumProductSize / num):0;
                pair->pcrSizeMinTarSeqs = num>0?minProductSize:0;
                pair->pcrSizeMaxTarSeqs = num>0?maxProductSize:0;

                /*if( num == 0 ){
                    pair->pcrSizeAvgTarSeqs = 0;
                    pair->pcrSizeMinTarSeqs = 0;
                    pair->pcrSizeMaxTarSeqs = 0;
                }
                else{
                    pair->pcrSizeAvgTarSeqs = (float)sumProductSize / num;
                    pair->pcrSizeMinTarSeqs = minProductSize;
                    pair->pcrSizeMaxTarSeqs = maxProductSize;
                }*/

                // compute frequencies for primer pair in non-target sequences
                num = 0;
                maxProductSize = 0;	minProductSize = INT_MAX;	sumProductSize = 0;
                for( int k = 0; k < numNonTarSeqs; k++ ){

                    bdPos[0] = lPrBindPosNonTarSeqs[0][i][k];
                    bdPos[1] = lPrBindPosNonTarSeqs[1][i][k];
                    bdPos[2] = rPrBindPosNonTarSeqs[0][j][k];
                    bdPos[3] = rPrBindPosNonTarSeqs[1][j][k];

                    // left and right primer both appear and positions are correct
                    if( bdPos[0] >= 0 && bdPos[2] >=0 && bdPos[1] < bdPos[2] ){
                        num++;
                        //record binding target sequences and positions
                        productSize = bdPos[3] - bdPos[0] + 1;
                        if( minProductSize > productSize )	minProductSize = productSize;
                        if( maxProductSize < productSize )	maxProductSize = productSize;
                        sumProductSize += productSize;
                    }
                }

                assert(numNonTarSeqs >= num);
                pair->pctgInNonTarSeqs = numNonTarSeqs>0?((float)num / numNonTarSeqs * 100):0;
                pair->pcrSizeAvgNonTarSeqs = num>0?((float)sumProductSize / num):0;
                pair->pcrSizeMinNonTarSeqs = num>0?minProductSize:0;
                pair->pcrSizeMaxNonTarSeqs = num>0?maxProductSize:0;

                /*if( numNonTarSeqs != 0 )
                    pair->pctgInNonTarSeqs = (float)num / numNonTarSeqs * 100;
                else
                    pair->pctgInNonTarSeqs = 0;

                if( num == 0 ){
                    pair->pcrSizeAvgNonTarSeqs = 0;
                    pair->pcrSizeMinNonTarSeqs = 0;
                    pair->pcrSizeMaxNonTarSeqs = 0;
                }
                else{
                    pair->pcrSizeAvgNonTarSeqs = (float)sumProductSize / num;
                    pair->pcrSizeMinNonTarSeqs = minProductSize;
                    pair->pcrSizeMaxNonTarSeqs = maxProductSize;
                }*/


                if( pair->pcrSizeAvgTarSeqs > 30 )
                    nPairsLeft[0]++;

                // filtering:
                // 1. pcr product size
                if( ( pair->pcrSizeAvgTarSeqs < primerPcrSizeMin || pair->pcrSizeAvgTarSeqs > primerPcrSizeMax ) ){
                    //&& ( pair->pcrSizeAvgNonTarSeqs < pcrSizeMin || pair->pcrSizeAvgNonTarSeqs > pcrSizeMax )  ) {
                    delete pair;
                    continue;
                }
                nPairsLeft[1]++;

                // 2. Tm difference
                pair->tmLeft = meltingTemp(pair->left);
                pair->tmRight = meltingTemp(pair->right);
                numDouble = pair->tmLeft - pair->tmRight;
                numDouble = fabs(numDouble);
                if( numDouble > primerTmDiff ){
                    delete pair;
                    continue;
                }
                nPairsLeft[2]++;

                //3. primer inter-complementarity
                pair->prPrComp = primerInterComplementarity(pair->left, pair->rightRevsComp);
                if( pair->prPrComp > primerMaxPrPrComp ){
                    delete pair;
                    continue;
                }
                nPairsLeft[3]++;

                //4. primer 3' inter-complementarity
                pair->prPr3Comp = primer3InterComplementarity(pair->left, pair->rightRevsComp);
                if( pair->prPr3Comp > primerMax3PrPrComp ){
                    delete pair;
                    continue;
                }
                nPairsLeft[4]++;

                // GC content
                pair->GCLeft = GCcontent(pair->left);
                pair->GCRight = GCcontent(pair->right);

                // primer self-complementarity
                pair->selfCompLeft = ppIntraComplementarity(pair->left);
                pair->selfCompRight = ppIntraComplementarity(pair->rightRevsComp);

                // primer 3' self-complementarity
                pair->self3CompLeft = pp3IntraComplementarity(pair->left, primerIntraBase3end);
                pair->self3CompRight = pp3IntraComplementarity(pair->rightRevsComp, primerIntraBase3end);

                // sigle primer average annealing positions in target
                // note lPrBindPosTarSeqsAvg[i] saves ending position for left primers,
                // need to change it to starting position for left primers. right primer is fine.
                pair->bindPosTarSeqsAvgLeft = lPrBindPosTarSeqsAvg[i] - pair->left.length() + 1;
                pair->bindPosTarSeqsAvgRight = rPrBindPosTarSeqsAvg[j];

                // single primer frequency in target and non-target sequences
                pair->pctgInTarSeqsLeft = lPrPctgTarSeqs[i];
                pair->pctgInNonTarSeqsLeft = lPrPctgNonTarSeqs[i];
                pair->pctgInTarSeqsRight = rPrPctgTarSeqs[j];
                pair->pctgInNonTarSeqsRight = rPrPctgNonTarSeqs[j];

                // indicating exact information computed
                //pair->isEstimateTar = 0;
                //pair->isEstimateNonTar = 0;

                pair->numAmbiguousTargetsLeft = numAmbiTargetsLeft;
                pair->numAmbiguousTargetsRight = numAmbiTargetsRight ;
                pair->numAmbiguousBasesLeft = numAmbiBasesLeft;
                pair->numAmbiguousBasesRight = numAmbiBasesRight;
                pair->marked = 0;
                // record binding position for each seqs

                for(int k = 0; k < numTarSeqs; k++){
                    pair->posBindingTar[0][k] = lPrBindPosTarSeqs[1][i][k];
                    pair->posBindingTar[1][k] = rPrBindPosTarSeqs[0][j][k];
                }
                for(int k = 0; k < numNonTarSeqs; k++){
                    pair->posBindingNontar[0][k] = lPrBindPosNonTarSeqs[1][i][k];
                    pair->posBindingNontar[1][k] = rPrBindPosNonTarSeqs[0][j][k];
                }
                //pair->posBindingTar[0] =  lPrBindPosTarSeqs[1][i];
                //pair->posBindingTar[1] =  rPrBindPosTarSeqs[0][j];
                //pair->posBindingNontar[0] = lPrBindPosNonTarSeqs[1][i];
                //pair->posBindingNontar[1] = rPrBindPosNonTarSeqs[0][j];

                //PrimerPair* x = tmpPrimerPairList.top();
                tmpPrimerPairList.push(pair);
                //PrimerPair* x1 = tmpPrimerPairList.top();

                //int scp = pow(100 - pair->pctgInTargetSeqs, 2 ) + 0.5f * pow( pair->pctgInNonTarSeqsLeft, 2 ) + 0.5f * pow( pair->pctgInNonTarSeqsRight, 2 );
                //int scx = pow(100 - x->pctgInTargetSeqs, 2 ) + 0.5f * pow( x->pctgInNonTarSeqsLeft, 2 ) + 0.5f * pow( x->pctgInNonTarSeqsRight, 2 );
                //int scx1 = pow(100 - x1->pctgInTargetSeqs, 2 ) + 0.5f * pow( x1->pctgInNonTarSeqsLeft, 2 ) + 0.5f * pow( x1->pctgInNonTarSeqsRight, 2 );
                while(tmpPrimerPairList.size() > keepPairNum ){
                    PrimerPair* x2 = tmpPrimerPairList.top();
                    //int scx2 = pow(100 - x2->pctgInTargetSeqs, 2 ) + 0.5f * pow( x2->pctgInNonTarSeqsLeft, 2 ) + 0.5f * pow( x2->pctgInNonTarSeqsRight, 2 );
                    tmpPrimerPairList.pop();
                    delete x2;
                }
                //primerPairList.push_back(pair);
            }
            pdProgDlg->setValue(i+1);
            //user abort
            if(pdProgDlg->wasCanceled()){
                // free memory
                for( int i = 0; i < 2; i++ ){
                    for( int j = 0; j < numLprimers; j++ )
                            delete[] lPrBindPosTarSeqs[i][j];
                    for( int j = 0; j < numLprimers; j++ )
                            delete[] lPrBindPosNonTarSeqs[i][j];
                    delete[] lPrBindPosTarSeqs[i];
                    delete[] lPrBindPosNonTarSeqs[i];
                }
                for( int i = 0; i < 2; i++ ){
                    for( int j = 0; j < numRprimers; j++ )
                            delete[] rPrBindPosTarSeqs[i][j];
                    for( int j = 0; j < numRprimers; j++ )
                            delete[] rPrBindPosNonTarSeqs[i][j];
                    delete[] rPrBindPosTarSeqs[i];
                    delete[] rPrBindPosNonTarSeqs[i];
                }
                delete[] lPrBindPosTarSeqs;
                delete[] lPrBindPosNonTarSeqs;
                delete[] rPrBindPosTarSeqs;
                delete[] rPrBindPosNonTarSeqs;
                delete[] lPrBindPosTarSeqsAvg;
                delete[] rPrBindPosTarSeqsAvg;
                lPrPctgTarSeqs.clear();
                rPrPctgTarSeqs.clear();
                lPrPctgNonTarSeqs.clear();
                rPrPctgNonTarSeqs.clear();

                return -1;
            }
        }

        while (! tmpPrimerPairList.empty()) {
            PrimerPair* topPrimer = tmpPrimerPairList.top();
            tmpPrimerPairList.pop();
            primerPairList.push_front(topPrimer);
        }

        //std::copy(&(tmpPrimerPairList.top()), &(tmpPrimerPairList.top()) + tmpPrimerPairList.size(), &primerPairList[0]);
    //}



    logDesign += ":	Start pairing off with " + QString("%1").arg(nPairsLeft[0] ) + " possible primer pairs.\r\n";
    logDesign += "\t\tCheck PCR product size, " + QString("%1").arg(nPairsLeft[1] ) + " primer pairs left.\r\n";
    logDesign += "\t\tCheck Tm difference, " + QString("%1").arg(nPairsLeft[2] ) + " primer pairs left.\r\n";
    logDesign += "\t\tCheck inter-complementarity, " + QString("%1").arg(nPairsLeft[3] ) + " primer pairs left.\r\n";
    logDesign += "\t\tCheck 3' inter-complementarity, " + QString("%1").arg(nPairsLeft[4] ) + " primer pairs left.\r\n\r\n";


    time = QTime::currentTime();
    timeStr = time.toString();
    logDesign += timeStr;
    logDesign += ":	Generate primer pair report with " + QString("%1").arg( primerPairList.size()) + " primer pairs.\r\n";

    pdProgDlg->setValue(0);

    // free memory
    for( int i = 0; i < 2; i++ ){
        for( int j = 0; j < numLprimers; j++ )
                delete[] lPrBindPosTarSeqs[i][j];
        for( int j = 0; j < numLprimers; j++ )
                delete[] lPrBindPosNonTarSeqs[i][j];
        delete[] lPrBindPosTarSeqs[i];
        delete[] lPrBindPosNonTarSeqs[i];
    }
    for( int i = 0; i < 2; i++ ){
        for( int j = 0; j < numRprimers; j++ )
                delete[] rPrBindPosTarSeqs[i][j];
        for( int j = 0; j < numRprimers; j++ )
                delete[] rPrBindPosNonTarSeqs[i][j];
        delete[] rPrBindPosTarSeqs[i];
        delete[] rPrBindPosNonTarSeqs[i];
    }
    delete[] lPrBindPosTarSeqs;
    delete[] lPrBindPosNonTarSeqs;
    delete[] rPrBindPosTarSeqs;
    delete[] rPrBindPosNonTarSeqs;
    delete[] lPrBindPosTarSeqsAvg;
    delete[] rPrBindPosTarSeqsAvg;
    lPrPctgTarSeqs.clear();
    rPrPctgTarSeqs.clear();
    lPrPctgNonTarSeqs.clear();
    rPrPctgNonTarSeqs.clear();

    return 0;
}


int Designer::quickSearchProbeInSeqs(void)
{
    if( probeCandid.size() == 0)
        return 0;

    short ***probeBindPosTarSeqs, ***probeBindPosNonTarSeqs;
    QVector<float> probePctgTarSeqs, probePctgNonTarSeqs;
    int numProbes = (int)probeCandid.size();
    int numTarSeqs = (int)tarSeqs.size();
    int numNonTarSeqs = (int)nontarSeqs.size();
    //int numPrimers4Probe =  (int)Primers4Probe.size();
    QVector<short*> reversedTarSeqs;
    QVector<short*> reversedNonTarSeqs;
    short* sequence;
    short probeGroupLens[40];
    int prLen, prNum;
    int pr1st, prlast;
    int seqLen;
    QTime time;
    QString timeStr;

    /***************************************************************************************************/
    // first allocate memory for probe annealing position variables
    probeBindPosTarSeqs = new short**[2];
    probeBindPosNonTarSeqs = new short**[2];
    for( int i = 0; i < 2; i++ ){
        // for target sequences
        try{
            probeBindPosTarSeqs[i] = new short*[numProbes];
            for( int j = 0; j < numProbes; j++ ){
                probeBindPosTarSeqs[i][j] = new short[numTarSeqs];
            }
        }catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            return FATAL_ERROR;
        }

        // for non-target sequences
        try{
            probeBindPosNonTarSeqs[i] = new short*[numProbes];
            for( int j = 0; j < numProbes; j++ ){
                probeBindPosNonTarSeqs[i][j] = new short[numNonTarSeqs];
            }
        }catch(std::bad_alloc& e ){
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for primer searching.\nProgram is forced to terminate.");
            return FATAL_ERROR;
        }

    }
    //user abort
    if(pdProgDlg->wasCanceled()){
        // free memory
        for( int i = 0; i < 2; i++ ){
            for( int j = 0; j < numProbes; j++ )
                delete[] probeBindPosTarSeqs[i][j];
            for( int j = 0; j < numProbes; j++ )
                delete[] probeBindPosNonTarSeqs[i][j];
            delete[] probeBindPosTarSeqs[i];
            delete[] probeBindPosNonTarSeqs[i];
        }
        delete[] probeBindPosTarSeqs;
        delete[] probeBindPosNonTarSeqs;
        return -1;
    }

    /***************************************************************************************************/
    // next prepare reversed target and non-target sequences
    // for searching left probes in sequences from 3' to 5'
    for( int i = 0; i < numTarSeqs; i++ ){
        seqLen = 0;
        while( tarSeqs[i][seqLen] >= 0 )
            seqLen++;
        sequence = new short[seqLen+1];
        for(int j = 0, k = seqLen-1; j < seqLen; j++, k--)
            sequence[j] = tarSeqs[i][k];
        sequence[seqLen] = -1;
        reversedTarSeqs.push_back(sequence);
    }
    for( int i = 0; i < numNonTarSeqs; i++ ){
        seqLen = 0;
        while( nontarSeqs[i][seqLen] >= 0 )
            seqLen++;
        sequence = new short[seqLen+1];
        for(int j = 0, k = seqLen-1; j < seqLen; j++, k--)
            sequence[j] = nontarSeqs[i][k];
        sequence[seqLen] = -1;
        reversedNonTarSeqs.push_back(sequence);
    }
    //user abort
     if(pdProgDlg->wasCanceled()){
         // free memory
         for( int i = 0; i < numProbes; i++ ){
             for( int j = 0; j < numTarSeqs; j++ )
                delete[] probeBindPosTarSeqs[i][j];
             for( int j = 0; j < numNonTarSeqs; j++ )
                delete[] probeBindPosNonTarSeqs[i][j];
             delete[] probeBindPosTarSeqs[i];
             delete[] probeBindPosNonTarSeqs[i];
         }

         delete[] probeBindPosTarSeqs;
         delete[] probeBindPosNonTarSeqs;
         for( int i = 0; i < numTarSeqs; i++ )
            delete[] reversedTarSeqs[i];
         for( int i = 0; i < numNonTarSeqs; i++ )
            delete[] reversedNonTarSeqs[i];
         reversedTarSeqs.clear();
         reversedNonTarSeqs.clear();
         return -1;
     }

    /***************************************************************************************************/
    // finally start approxiation search: group probes with common prefix
    // do the search for a group
    ProbeQuickSearch* ptr = new ProbeQuickSearch();

    time = QTime::currentTime();
    timeStr = time.toString();
    logDesign += timeStr;
    logDesign += ":	Start searching for probes in target sequences.\r\n\r\n";

    // 1. left probe candidates in target sequences==========================================================
    pdProgDlg->setRange(0, numProbes);
    pdProgDlg->computation(3);

    pr1st = 0;	prlast = 1;
    while( prlast < numProbes ){
        //Group candidates  with common prefix together.
        if( isPrefixProbe( probeCandid[prlast-1], probeCandid[prlast] ) )
            prlast++;
        else{ //Find the end of the original group
            prNum = prlast - pr1st; //The size of current group
            for( int i = pr1st, j = 0; i < prlast; i++, j++ ){  //record the length of each element at this group
                prLen = 0;
                while( probeCandid[i][prLen] >= 0 )
                    prLen++;
                probeGroupLens[j] = prLen;
            }
            // Do approximation search for elements in this group
            ptr->initMember( probeCandid[prlast-1], prNum, probeGroupLens);
            for( int i = 0; i < numTarSeqs; i++ ){
                ptr->searchProbesInTarSeq(tarSeqs[i], reversedTarSeqs[i] );
                for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
                    probeBindPosTarSeqs[0][j][i] = ptr->bindingPosStart[k];
                    probeBindPosTarSeqs[1][j][i] = ptr->bindingPosEnd[k];
                }
            }
            pr1st = prlast++;
            pdProgDlg->setValue(prlast);

            //user abort
            if(pdProgDlg->wasCanceled()){
                // free memory
                for( int i = 0; i < 2; i++ ){
                    for( int j = 0; j < numProbes; j++ )
                        delete[] probeBindPosTarSeqs[i][j];
                    for( int j = 0; j < numProbes; j++ )
                        delete[] probeBindPosNonTarSeqs[i][j];
                    delete[] probeBindPosTarSeqs[i];
                    delete[] probeBindPosNonTarSeqs[i];
                }

                delete[] probeBindPosTarSeqs;
                delete[] probeBindPosNonTarSeqs;
                for( int i = 0; i < numTarSeqs; i++ )
                    delete[] reversedTarSeqs[i];
                for( int i = 0; i < numNonTarSeqs; i++ )
                    delete[] reversedNonTarSeqs[i];
                reversedTarSeqs.clear();
                reversedNonTarSeqs.clear();

                return -1;
            }
        }
    }
    prNum = prlast - pr1st;
    for( int i = pr1st, j = 0; i < prlast; i++, j++ ){
        prLen = 0;
        while( probeCandid[i][prLen] >= 0 )
            prLen++;
        probeGroupLens[j] = prLen;
    }
    ptr->initMember( probeCandid[prlast-1], prNum, probeGroupLens );
    // start searching
    for( int i = 0; i < numTarSeqs; i++ ){
        ptr->searchProbesInTarSeq( tarSeqs[i], reversedTarSeqs[i] );
        for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
            probeBindPosTarSeqs[0][j][i] = ptr->bindingPosStart[k];
            probeBindPosTarSeqs[1][j][i] = ptr->bindingPosEnd[k];
        }
    }
    pdProgDlg->setValue(prlast);

    //user abort
    if(pdProgDlg->wasCanceled()){
        // free memory
        for( int i = 0; i < 2; i++ ){
            for( int j = 0; j < numProbes; j++ )
                delete[] probeBindPosTarSeqs[i][j];
            for( int j = 0; j < numProbes; j++ )
                delete[] probeBindPosNonTarSeqs[i][j];
            delete[] probeBindPosTarSeqs[i];
            delete[] probeBindPosNonTarSeqs[i];
        }
        delete[] probeBindPosTarSeqs;
        delete[] probeBindPosNonTarSeqs;

        for( int i = 0; i < numTarSeqs; i++ )
            delete[] reversedTarSeqs[i];
        for( int i = 0; i < numNonTarSeqs; i++ )
            delete[] reversedNonTarSeqs[i];
        reversedTarSeqs.clear();
        reversedNonTarSeqs.clear();

        return -1;
    }

    time = QTime::currentTime();
    timeStr = time.toString();
    logDesign += timeStr;
    logDesign += ":	Start searching for probes in non-target sequences.\r\n\r\n";

    // 2. probe candidates in non-target sequences=======================================================
    pdProgDlg->setRange(0, numProbes);
    pdProgDlg->computation(4);

    pr1st = 0;	prlast = 1;
    while( prlast < numProbes ){
        if( isPrefixProbe( probeCandid[prlast-1], probeCandid[prlast] ) )
            prlast++;
        else{
            prNum = prlast - pr1st;
            // get a group
            for( int i = pr1st, j = 0; i < prlast; i++, j++ ){
                prLen = 0;
                while( probeCandid[i][prLen] >= 0 )
                    prLen++;
                probeGroupLens[j] = prLen;
            }
            ptr->initMember( probeCandid[prlast-1], prNum, probeGroupLens);
            // start searching
            for( int i = 0; i < numNonTarSeqs; i++ ){
                ptr->searchProbesInNonTarSeq( nontarSeqs[i], reversedNonTarSeqs[i] );
                for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
                    probeBindPosNonTarSeqs[0][j][i] = ptr->bindingPosStart[k];
                    probeBindPosNonTarSeqs[1][j][i] = ptr->bindingPosEnd[k];
                }
            }
            pr1st = prlast++;
            pdProgDlg->setValue(prlast);

            //user abort
            if(pdProgDlg->wasCanceled()){
                // free memory
                for( int i = 0; i < 2; i++ ){
                    for( int j = 0; j < numProbes; j++ )
                        delete[] probeBindPosTarSeqs[i][j];
                    for( int j = 0; j < numProbes; j++ )
                        delete[] probeBindPosNonTarSeqs[i][j];
                    delete[] probeBindPosTarSeqs[i];
                    delete[] probeBindPosNonTarSeqs[i];
                }
                delete[] probeBindPosTarSeqs;
                delete[] probeBindPosNonTarSeqs;
                for( int i = 0; i < numTarSeqs; i++ )
                    delete[] reversedTarSeqs[i];
                for( int i = 0; i < numNonTarSeqs; i++ )
                    delete[] reversedNonTarSeqs[i];
                reversedTarSeqs.clear();
                reversedNonTarSeqs.clear();
                return -1;
            }
        }
    }
    prNum = prlast - pr1st;
    for( int i = pr1st, j = 0; i < prlast; i++, j++ ){
        prLen = 0;
        while( probeCandid[i][prLen] >= 0 )
            prLen++;
        probeGroupLens[j] = prLen;
    }
    ptr->initMember( probeCandid[prlast-1], prNum, probeGroupLens );
    // start searching
    for( int i = 0; i < numNonTarSeqs; i++ ){
        ptr->searchProbesInNonTarSeq( nontarSeqs[i], reversedNonTarSeqs[i] );
        for( int j = pr1st, k = 0; j < prlast; j++, k++ ){
            probeBindPosNonTarSeqs[0][j][i] = ptr->bindingPosStart[k];
            probeBindPosNonTarSeqs[1][j][i] = ptr->bindingPosEnd[k];
        }
    }
    pdProgDlg->setValue(prlast);
    //user abort
    if(pdProgDlg->wasCanceled()){
        // free memory
        for( int i = 0; i < 2; i++ ){
            for( int j = 0; j < numProbes; j++ )
                delete[] probeBindPosTarSeqs[i][j];
            for( int j = 0; j < numProbes; j++ )
                delete[] probeBindPosNonTarSeqs[i][j];
            delete[] probeBindPosTarSeqs[i];
            delete[] probeBindPosNonTarSeqs[i];
        }
        delete[] probeBindPosTarSeqs;
        delete[] probeBindPosNonTarSeqs;
        for( int i = 0; i < numTarSeqs; i++ )
            delete[] reversedTarSeqs[i];
        for( int i = 0; i < numNonTarSeqs; i++ )
            delete[] reversedNonTarSeqs[i];
        reversedTarSeqs.clear();
        reversedNonTarSeqs.clear();

        return -1;
    }
    delete ptr;


    // finish searching========================================================================================
    // prepare frequency for single probe in target and non-target sequences
    // record how many times a single probe occurs in target sequences and non-target sequence
    float occ;
    for( int i = 0; i < numProbes; i++ ){
        occ = 0;
        for( int j = 0; j < numTarSeqs; j++ ){
            if( probeBindPosTarSeqs[0][i][j] >= 0 )
                occ++;
        }
        probePctgTarSeqs.push_back( occ/numTarSeqs * 100 );
        occ = 0;
        for( int j = 0; j < numNonTarSeqs; j++ ){
            if( probeBindPosNonTarSeqs[0][i][j] >= 0 )
                occ++;
        }
        probePctgNonTarSeqs.push_back( occ/numNonTarSeqs * 100 );
    }


    pdProgDlg->setValue(0);
    //user abort
    if(pdProgDlg->wasCanceled()){
        // free memory
        for( int i = 0; i < 2; i++ ){
            for( int j = 0; j < numProbes; j++ )
                delete[] probeBindPosTarSeqs[i][j];
            for( int j = 0; j < numProbes; j++ )
                delete[] probeBindPosNonTarSeqs[i][j];
            delete[] probeBindPosTarSeqs[i];
            delete[] probeBindPosNonTarSeqs[i];
        }
        delete[] probeBindPosTarSeqs;
        delete[] probeBindPosNonTarSeqs;

        for( int i = 0; i < numTarSeqs; i++ )
            delete[] reversedTarSeqs[i];
        for( int i = 0; i < numNonTarSeqs; i++ )
            delete[] reversedNonTarSeqs[i];
        reversedTarSeqs.clear();
        reversedNonTarSeqs.clear();
        probePctgTarSeqs.clear();
        probePctgNonTarSeqs.clear();

        return -1;
    }

    // free memory for reversed sequences
    for( int i = 0; i < numTarSeqs; i++ )
        delete[] reversedTarSeqs[i];
    for( int i = 0; i < numNonTarSeqs; i++ )
        delete[] reversedNonTarSeqs[i];
    reversedTarSeqs.clear();
    reversedNonTarSeqs.clear();


    // pair off probe candidates and primer pairs=======================================================
    int numProbe; //Occurrence of Probe
    //int numTriple; //Occurrence of Triple
    float numPosStart;
    float numPosEnd;
    //float tmDiff;
    int bdPos[2];
    //int nTriplesLeft[3];
    // fill searching result in probe list
    pdProgDlg->computation(5);
    pdProgDlg->setRange(0,numProbes);

    //int numProbeList = probeList.size();
    Probe *pb;
    //Triple *tp;
    QVector<short> randSelSeqs;

    /***************************************************************************************************/
    // Prepare average annealing position for all candidate in target and non-target sequences.
    float** probeBindPosTarSeqsAvg;
    try{
        probeBindPosTarSeqsAvg = new float*[numProbes]; //average position of left probes
        for(int i = 0; i< numProbes; i++)
            probeBindPosTarSeqsAvg[i] = new float[2];
    }catch(  std::bad_alloc& e){
        QMessageBox::about(0, "Fatal error", "Fatal error: out of memory for probe searching.\nProgram is forced to terminate");
        /*Clean Memory: (for Error)
          1. Target&Nontar Seqs
          2. Primer Candidates
          3. Primer Pairs
          4. probe Candidates
          5. probes
          6. Triples (in Primer Pair)
          7. T & P
        */
        DataTools::cleanMemoryAllData();
        // free memory
        for( int i = 0; i < 2; i++ ){
            for( int j = 0; j < numProbes; j++ )
                delete[] probeBindPosTarSeqs[i][j];
            for( int j = 0; j < numProbes; j++ )
                delete[] probeBindPosNonTarSeqs[i][j];
            delete[] probeBindPosTarSeqs[i];
            delete[] probeBindPosNonTarSeqs[i];
        }
        delete[] probeBindPosTarSeqs;
        delete[] probeBindPosNonTarSeqs;
        probePctgTarSeqs.clear();
        probePctgNonTarSeqs.clear();
        return -1;
    }

    for( int i = 0; i < numProbes; i++ ){
        numPosStart = 0; numPosEnd = 0; numProbe = 0;
        for( int j = 0; j < numTarSeqs; j++ ){
            if( probeBindPosTarSeqs[0][i][j] >= 0 ){
                assert(probeBindPosTarSeqs[1][i][j] > 0);
                numProbe++;
                numPosStart += probeBindPosTarSeqs[0][i][j];
                numPosEnd += probeBindPosTarSeqs[1][i][j];  // probe check both start and ending positions
            }
        }
        probeBindPosTarSeqsAvg[i][0] = numProbes>0?(numPosStart / numProbe):0;
        probeBindPosTarSeqsAvg[i][1] = numProbes>0?(numPosEnd / numProbe):0;
    }

    /***************************************************************************************************/
    // for information of probe design report window

    //time = QTime::currentTime();
    //timeStr = time.toString();
    //logDesign += timeStr;


    /*if( probeEstimate){	// compute estimated PCR information
        for( int i = 0; i < numProbes; i++ ){
            // first check if this probe can matches to some primer pairs
            // skip if left probe ending position is on the right of right probe starting postion
            try{
                pb = new Probe(); //create a probe pair for it.
                pb->posBindingTar = new short*[2];
                pb->posBindingNontar = new short*[2];
            }catch(  std::bad_alloc& e ){
                QMessageBox::about(0,"Fatal error", "Fatal error: out of memory.\nThe possible solutions:\n"
                        "1. Install more physical memory.\n2. Close all other applications.\n"
                        "3. Assign stricter probe settings.\n"
                        "4. Compute probe for a single length at a time.\n"
                        "Program is forced to terminate.");
                // free memory
                for( int i = 0; i < 2; i++ ){
                    for( int j = 0; j < numProbes; j++ )
                        delete[] probeBindPosTarSeqs[i][j];
                    for( int j = 0; j < numProbes; j++ )
                        delete[] probeBindPosNonTarSeqs[i][j];
                    delete[] probeBindPosTarSeqs[i];
                    delete[] probeBindPosNonTarSeqs[i];
                }

                delete[] probeBindPosTarSeqs;
                delete[] probeBindPosNonTarSeqs;
                delete[] probeBindPosTarSeqsAvg;
                probePctgTarSeqs.clear();
                probePctgNonTarSeqs.clear();
                return FATAL_ERROR;
            }
            tools::seqInt2Str(probeCandid[i],pb->probeSeq);

            // compute frequencies for probe in target sequences
            // only pick 100 random target sequences to consider
            rand100position( numTarSeqs, randSelSeqs );
            if( (int)randSelSeqs.size() < numTarSeqs )
                pb->isEstimateTar = true;
            else
                pb->isEstimateTar = false;   //# of target sequences is small: check in all target sequences


            numProbe = 0;
            for( int j = 0; j < randSelSeqs.size(); j++ ){
                bdPos[0] = probeBindPosTarSeqs[0][i][j];
                bdPos[1] = probeBindPosTarSeqs[1][i][j];

                if( bdPos[0] >= 0 ){//probe appears
                    numProbe++;   //this primer exists at current tar Seqs
                }
            }
            if( randSelSeqs.size() != 0 )
                pb->pctgInTargetSeqs = (float)numProbe / randSelSeqs.size() * 100;
            else
                pb->pctgInTargetSeqs = 0;

            // compute frequencies for probe in non-target sequences
            // only pick 100 random non-target sequences to consider
            rand100position( numNonTarSeqs, randSelSeqs );
            if( (int)randSelSeqs.size() < numNonTarSeqs )
                pb->isEstimateNonTar = 1;
            else
                pb->isEstimateNonTar = 0;
            numProbe = 0;
            for( int j = 0; j < randSelSeqs.size(); j++ ){
                bdPos[0] = probeBindPosNonTarSeqs[0][i][j];
                bdPos[1] = probeBindPosNonTarSeqs[1][i][j];
                // probe both appear and positions are correct
                if( bdPos[0] >= 0){
                    numProbe++;
                }
            }
            if( randSelSeqs.size() != 0 )
                pb->pctgInNonTarSeqs = (float)numProbe / randSelSeqs.size() * 100;
            else
                pb->pctgInNonTarSeqs = 0;

            pb->tmProbe= meltingTemp(pb->probeSeq);
            // GC content
            pb->GCNum= GCcontent(pb->probeSeq);
            // probe self-complementarity
            pb->selfComp = ppIntraComplementarity(pb->probeSeq);
            //pb->self3Comp = pp3IntraComplementarity(pb->probeSeq, probeIntraBase3end);

            // sigle probe average annealing positions in target
            // note lPrBindPosTarSeqsAvg[i] saves ending position for probes,
            // need to change it to starting position for probes
            pb->bindPosTarSeqsAvg[0] = probeBindPosTarSeqsAvg[i][0];
            pb->bindPosTarSeqsAvg[1] = probeBindPosTarSeqsAvg[i][1];

            // probe frequency in target and non-target sequences
            pb->pctgInTargetSeqs = probePctgTarSeqs[i];
            pb->pctgInNonTarSeqs = probePctgNonTarSeqs[i];

            // indicating exact information computed
            pb->isEstimateTar = 0;
            pb->isEstimateNonTar = 0;

            pb->marked = 0;

            pb->posBindingTar[0] =  probeBindPosTarSeqs[0][i];
            pb->posBindingTar[1] =  probeBindPosTarSeqs[1][i];
            pb->posBindingNontar[0] = probeBindPosNonTarSeqs[0][i];
            pb->posBindingNontar[1] = probeBindPosNonTarSeqs[1][i];

            probeList.push_back(pb);
            pdProgDlg->setValue(i+1);
        }
        //user abort
        if(pdProgDlg->wasCanceled()){
            // free memory
            for( int i = 0; i < 2; i++ ){
                for( int j = 0; j < numProbes; j++ )
                    delete[] probeBindPosTarSeqs[i][j];
                for( int j = 0; j < numProbes; j++ )
                    delete[] probeBindPosNonTarSeqs[i][j];
                delete[] probeBindPosTarSeqs[i];
                delete[] probeBindPosNonTarSeqs[i];
            }

            delete[] probeBindPosTarSeqs;
            delete[] probeBindPosNonTarSeqs;
            delete[] probeBindPosTarSeqsAvg;

            probePctgTarSeqs.clear();
            probePctgNonTarSeqs.clear();
            return -1;
        }
    }
    else{// compute exact PCR information*/
        for( int i = 0; i < numProbes; i++ ){
            // first check if this probe can matches to some primer pairs
            // skip if left probe ending position is on the right of right probe starting postion
            try{
                pb = new Probe();
                pb->posBindingTar = new short*[2];
                pb->posBindingNontar = new short*[2];
                for(int k = 0; k < 2; k++){
                    pb->posBindingTar[k] = new short[numTarSeqs];
                    pb->posBindingNontar[k] = new short[numNonTarSeqs];
                }

            }catch(  std::bad_alloc& e ){
                QMessageBox::about(0,"Fatal error", "Fatal error: out of memory.\nThe possible solutions:\n"
                        "1. Install more physical memory.\n2. Close all other applications.\n"
                        "3. Assign stricter probe settings.\n"
                        "4. Compute probe for a single length at a time.\n"
                        "Program is forced to terminate.");
                // free memory
                for( int i = 0; i < 2; i++ ){
                    for( int j = 0; j < numProbes; j++ )
                        delete[] probeBindPosTarSeqs[i][j];
                    for( int j = 0; j < numProbes; j++ )
                        delete[] probeBindPosNonTarSeqs[i][j];
                    delete[] probeBindPosTarSeqs[i];
                    delete[] probeBindPosNonTarSeqs[i];
                }

                delete[] probeBindPosTarSeqs;
                delete[] probeBindPosNonTarSeqs;
                delete[] probeBindPosTarSeqsAvg;
                probePctgTarSeqs.clear();
                probePctgNonTarSeqs.clear();
                return FATAL_ERROR;
            }
            tools::seqInt2Str(probeCandid[i],pb->probeSeq);
            // compute frequencies for probe in target sequences
            numProbe = 0;
            int numAmbiTargets = 0;
            int numAmbiBases = 0;
            bool flag;

            for( int j = 0; j < numTarSeqs; j++ ){
                bdPos[0] = probeBindPosTarSeqs[0][i][j];
                bdPos[1] = probeBindPosTarSeqs[1][i][j];

                if( bdPos[0] >= 0 ){//probe appears
                    numProbe++;   //this primer exists at current tar Seqs
                }

                flag = false;
                for(int k = bdPos[0] ; k <= bdPos[1]; k++){
                    if(tarSeqs[j][k] >3){
                        numAmbiBases++;
                        flag = true;
                    }
                }
                if(flag){
                    numAmbiTargets++;
                }
            }

            if( numTarSeqs != 0 )
                pb->pctgInTargetSeqs = (float)numProbe / numTarSeqs * 100;
            else
                pb->pctgInTargetSeqs = 0;

            // compute frequencies for probe pair in non-target
            numProbe = 0;
            for( int j = 0; j < numNonTarSeqs; j++ ){
                bdPos[0] = probeBindPosNonTarSeqs[0][i][j];
                bdPos[1] = probeBindPosNonTarSeqs[1][i][j];
                // probe both appear and positions are correct
                if( bdPos[0] >= 0){
                    numProbe++;
                }
            }
            if( numNonTarSeqs != 0 )
                pb->pctgInNonTarSeqs = (float)numProbe / numNonTarSeqs * 100;
            else
                pb->pctgInNonTarSeqs = 0;

            pb->tmProbe= meltingTemp(pb->probeSeq);
            // GC content
            pb->GCNum= GCcontent(pb->probeSeq);
            // probe self-complementarity
            pb->selfComp = ppIntraComplementarity(pb->probeSeq);
            //pb->self3Comp = pp3IntraComplementarity(pb->probeSeq, probeIntraBase3end);

            // sigle probe average annealing positions in target
            // note lPrBindPosTarSeqsAvg[i] saves ending position for probes,
            // need to change it to starting position for probes
            pb->bindPosTarSeqsAvg[0] = probeBindPosTarSeqsAvg[i][0];
            pb->bindPosTarSeqsAvg[1] = probeBindPosTarSeqsAvg[i][1];

            // probe frequency in target and non-target sequences
            pb->pctgInTargetSeqs = probePctgTarSeqs[i];
            pb->pctgInNonTarSeqs = probePctgNonTarSeqs[i];

            // indicating exact information computed
            //pb->isEstimateTar = 0;
            //pb->isEstimateNonTar = 0;
            pb->numAmbiguousTargets = numAmbiTargets ;
            pb->numAmbiguousBases = numAmbiBases;

            pb->marked = 0;


            for(int k = 0; k < numTarSeqs; k++){
                pb->posBindingTar[0][k] = probeBindPosTarSeqs[0][i][k];
                pb->posBindingTar[1][k] = probeBindPosTarSeqs[1][i][k];
            }
            for(int k = 0; k < numNonTarSeqs; k++){
                pb->posBindingNontar[0][k] = probeBindPosNonTarSeqs[0][i][k];
                pb->posBindingNontar[1][k] = probeBindPosNonTarSeqs[1][i][k];
            }
            /*pb->posBindingTar[0] =  probeBindPosTarSeqs[0][i];
            pb->posBindingTar[1] =  probeBindPosTarSeqs[1][i];
            pb->posBindingNontar[0] = probeBindPosNonTarSeqs[0][i];
            pb->posBindingNontar[1] = probeBindPosNonTarSeqs[1][i];*/

            probeList.push_back(pb);
            pdProgDlg->setValue(i+1);
        }
        //user abort
        if(pdProgDlg->wasCanceled()){
            // free memory
            for( int i = 0; i < 2; i++ ){
                for( int j = 0; j < numProbes; j++ )
                    delete[] probeBindPosTarSeqs[i][j];
                for( int j = 0; j < numProbes; j++ )
                    delete[] probeBindPosNonTarSeqs[i][j];
                delete[] probeBindPosTarSeqs[i];
                delete[] probeBindPosNonTarSeqs[i];
            }

            delete[] probeBindPosTarSeqs;
            delete[] probeBindPosNonTarSeqs;
            delete[] probeBindPosTarSeqsAvg;

            probePctgTarSeqs.clear();
            probePctgNonTarSeqs.clear();
            return -1;
        }
    //}

    time = QTime::currentTime();
    timeStr = time.toString();
    logDesign += timeStr;
    logDesign += ":	Generating primer-probe set report with " + QString("%1").arg( probeList.size()) + " probes.\r\n\r\n";
    pdProgDlg->setValue(0);

    // free memory
    for( int i = 0; i < 2; i++ ){
        for( int j = 0; j < numProbes; j++ )
            delete[] probeBindPosTarSeqs[i][j];
        for( int j = 0; j < numProbes; j++ )
            delete[] probeBindPosNonTarSeqs[i][j];
        delete[] probeBindPosTarSeqs[i];
        delete[] probeBindPosNonTarSeqs[i];
    }
    delete[] probeBindPosTarSeqs;
    delete[] probeBindPosNonTarSeqs;
    delete[] probeBindPosTarSeqsAvg;
    probePctgTarSeqs.clear();
    probePctgNonTarSeqs.clear();
    return 0;
}

int Designer::checkTriple(PrimerPair* pr)
{
    int numTriple; //Occurrance of Triple
    int numTarSeqs = (int)tarSeqs.size();
    int numNonTarSeqs = (int)nontarSeqs.size();
    float tmDiff;
    int bdPos[2];
    int nTriplesLeft[4];

    for( int i = 0; i < 4; i++ )
        nTriplesLeft[i] = 0;

    int numProbe = probeList.size();
    Triple *tp;
    for(int i = 0; i < numProbe; i++)
    {
        Probe* pb = probeList[i];
        if( pb->bindPosTarSeqsAvg[0] <= pr->bindPosTarSeqsAvgLeft || pb->bindPosTarSeqsAvg[1] >= pr->bindPosTarSeqsAvgRight )
            continue;
        tp = new Triple();
        tp->Pb = pb;

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
            int rightEnd =  pr->posBindingNontar[1][k];
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

        nTriplesLeft[0]++;
        tp->pbPrComp = tripleInterComplementarity(pb->probeSeq, pr->left, pr->rightRevsComp);
        //tp->pbPr3Comp = triple3InterComplementarity(pb->probeSeq, pr->left, pr->rightRevsComp);



        // 1. filter by occurrences
        if( tp->pctgInTargetSeqs <= 0 ){
            delete tp;
            continue;
        }
        nTriplesLeft[1]++;


        // 2. filter by Tm difference
        float tmLeft = pr->tmLeft;
        float tmRight = pr->tmRight;

        tmDiff = min(pb->tmProbe - tmLeft,  pb->tmProbe - tmRight);
        if( tmDiff < probeTmGreaterMin ){
            delete tp;
            continue;
        }
        tmDiff = max(pb->tmProbe - tmLeft,  pb->tmProbe - tmRight);
        if( tmDiff > probeTmGreaterMax ){
            delete tp;
            continue;
        }
        nTriplesLeft[2]++;


        // 3. filter by inter & intra complementary
        if( tp->pbPrComp > maxPbPrComp ){
            delete tp;
            continue;
        }
        nTriplesLeft[3]++;
        /*if( tp->pbPr3Comp > max3PbPrComp ){
            delete tp;
            continue;
        }
        nTriplesLeft[4]++;*/


        pr->tripleList.push_back(tp);
    }
    QTime time = QTime::currentTime();
    QString timeStr = time.toString();
    logDesign += timeStr;
    logDesign += ":	Start checking Primer-Pair "+pr->left+".."+pr->rightRevsComp+" with " + QString("%1").arg(nTriplesLeft[0] ) + " possible primer-probe sets.\r\n" ;
    logDesign += "\t\tCheck occurrence frequency, " + QString("%1").arg(nTriplesLeft[1] ) + " sets left.\r\n";
    logDesign += "\t\tCheck Tm difference, " + QString("%1").arg(nTriplesLeft[2] ) + " sets left.\r\n";
    logDesign += "\t\tCheck inter-complementarity, " + QString("%1").arg(nTriplesLeft[3] ) + " sets left.\r\n\r\n";
    //logDesign += "\t\tCheck 3' inter-complementarity, " + QString("%1").arg(nTriplesLeft[4] ) + " sets left.\r\n\r\n";
    return 0;
}

int Designer::startPrimerDesign()
{
    getLogInfo();
    //1. allocate memory first
    int rt = allocateMemory();
    if(rt < 0){
        /*Clean Memory: (for Error)
          1. Target&Nontar Seqs
          2. Primer Candidates
          3. Primer Pairs
          4. probe Candidates
          5. probes
          6. Triples (in Primer Pair)
          7. T & P
        */
        DataTools::cleanMemoryAllData();
        return rt;
    }


    //2. read target and non-target sequences


    /*QString logFilePath = "";
    #ifdef MAC
        QString appPath = QDir::currentPath();
        logFilePath += "file:/";
        logFilePath += appPath;
        logFilePath += "/";
    #endif

    #ifdef WIN
        logFilePath += "Log\\";
    #else
        logFilePath += "Log/";
    #endif


    QDate date = QDate::currentDate();
    QString dateString = date.toString();

    QFile file(logFilePath+dateString+"_Log.txt");
    file.open(QIODevice::Append| QIODevice::Text);
    QTextStream out(&file);
    QTime time = QTime::currentTime();
    out << time.toString()<<"\t Start reading target and non-target sequences \n";
    file.close();*/

    QTime time = QTime::currentTime();
    DataTools::writeErrorLog(time.toString()+"\t Start reading target and non-target sequences \n");

    rt = readSequence();
    if(rt < 0){
        /*Clean Memory: (for Error)
          1. Target&Nontar Seqs
          2. Primer Candidates
          3. Primer Pairs
          4. probe Candidates
          5. probes
          6. Triples (in Primer Pair)
          7. T & P
        */
        DataTools::cleanMemoryAllData();
        return rt;
    }


    //3. filter duplicated sequences and sequences not within range
    /*file.open(QIODevice::Append| QIODevice::Text);
    time = QTime::currentTime();
    out << time.toString()<<"\t Filter out duplicated sequences\n";
    file.close();*/

    time = QTime::currentTime();
    DataTools::writeErrorLog(time.toString()+"\t Filter out duplicated sequences\n");

    rt = filterSequence();
    if(rt < 0){
        /*Clean Memory: (for Error)
          1. Target&Nontar Seqs
          2. Primer Candidates
          3. Primer Pairs
          4. probe Candidates
          5. probes
          6. Triples (in Primer Pair)
          7. T & P
        */
        DataTools::cleanMemoryAllData();
        return rt;
    }
    
    time = QTime::currentTime();
    QString timeStr = time.toString();
    logDesign += "\r\n";
    logDesign += timeStr;
    logDesign += ":	Start primer design process.\r\n";

    pdProgDlg = new PrimerDesignProgressDlg();
    pdProgDlg->setWindowModality(Qt::ApplicationModal);

    pdProgDlg->computation(1);
    //4. read user uploaded primer candidate file if there is one
    //   choose primer candidates from target sequences if user didn't provide primer candidates
    /*file.open(QIODevice::Append| QIODevice::Text);
    time = QTime::currentTime();
    out << time.toString()<<"\t Collecting primer candidates\n";
    file.close();*/

    time = QTime::currentTime();
    DataTools::writeErrorLog(time.toString()+"\t Collecting primer candidates\n");

    if( usrLoad )
        rt = readPrimer();
    else
        rt = choosePrimerCandid();
    if( rt < 0 ){
        // close progress windowd
        delete pdProgDlg;
        /*Clean Memory: (for Error)
          1. Target&Nontar Seqs
          2. Primer Candidates
          3. Primer Pairs
          4. probe Candidates
          5. probes
          6. Triples (in Primer Pair)
          7. T & P
        */
        DataTools::cleanMemoryAllData();
        return rt;
    }
    //5. filter primer candidates
    /*file.open(QIODevice::Append| QIODevice::Text);
    time = QTime::currentTime();
    out << time.toString()<<"\t Filter out Primer candidates\n";
    file.close();*/

    time = QTime::currentTime();
    DataTools::writeErrorLog(time.toString()+"\t Filter out Primer candidates\n");

    pdProgDlg->computation(2);
    rt = filterPrimerCandid();
    if( rt < 0 ){
        // close progress window
        delete pdProgDlg;
        /*Clean Memory: (for Error)
          1. Target&Nontar Seqs
          2. Primer Candidates
          3. Primer Pairs
          4. probe Candidates
          5. probes
          6. Triples (in Primer Pair)
          7. T & P
        */
        DataTools::cleanMemoryAllData();
        return rt;
    }
    //6. do approximate searching for every pair of primers in target and non-target sequences
    /*file.open(QIODevice::Append| QIODevice::Text);
    time = QTime::currentTime();
    out << time.toString()<<"\t Do approximation search\n";
    file.close();*/

    time = QTime::currentTime();
    DataTools::writeErrorLog(time.toString()+"\t Do approximation search\n");

    rt = quickSearchPrimerInSeqs();
    if( rt < 0 ){
        // close progress window
        delete pdProgDlg;
        /*Clean Memory: (for Error)
          1. Target&Nontar Seqs
          2. Primer Candidates
          3. Primer Pairs
          4. probe Candidates
          5. probes
          6. Triples (in Primer Pair)
          7. T & P
        */
        DataTools::cleanMemoryAllData();
        return rt;
    }
    // close progress window
    delete pdProgDlg;
    logDesign += "\r\n";
    time = QTime::currentTime();
    timeStr = time.toString();
    logDesign += timeStr;
    logDesign += ":	Terminate primer design process.";


    /*QString tmpStr;
    if(primerPairList.size() != 0) {
        tmpStr = QString("%1 primer pairs found.\r\n\r\n").arg(primerPairList.size());
    }else{
        tmpStr = QString("%1 primer pairs found.\r\n\r\n").arg(primerPairList.size());
        //tmpStr = QString("<span style=\" color:#ff0000;\">No primer pairs found.</span>");
    }*/


    //logDesign = tmpStr;
    logDesign =
        QString(
            "The report below shows the number of candidate primers meeting "
            "each selectivity criterion.\r\n"
            "You can increase the number of primers by repeating the process "
            "and relaxing these criteria in the Primer Design Settings window.\r\n\r\n"

            "To repeat design process using different criteria, click \"Change Criteria\".\r\n"
            "To go back to main window, click OK.\r\n\r\n"
            "==================================================================\r\n")
    + logDesign;

    /*if(primerPairList.size() == 0) {
        QString tmpStr = "No primer pair meeting current criteria found.\r\n\r\n"
                         "Please see the report to get the information about\r\n"
                         "number of candidates filtered out by each crietria.\r\n\r\n"
                         "You can retry with relaxed primer settings.";
        QMessageBox::about(0, "Warning", tmpStr);
    }*/
    return 0;
}


int Designer::startProbeDesign()
{
    DataTools::cleanMemoryProbeData();
    getLogInfo();

    //1. allocate memory first
    int rt = allocateMemory();
    if(rt < 0){
        /*Clean Memory: (for Error)
          4. probe Candidates
          5. probes
          6. Triples (in Primer Pair)
          8. tripleShowList
          9. T&P
          10. Score
        */
        DataTools::cleanMemoryProbeData();
        return rt;
    }

    QTime time = QTime::currentTime();
    QString timeStr = time.toString();
    logDesign += "\r\n";
    logDesign += timeStr;
    logDesign += ":	Start primer-probe design process.\r\n";

    pdProgDlg = new PrimerDesignProgressDlg();
    pdProgDlg->setWindowModality(Qt::ApplicationModal);

    pdProgDlg->computation(1);

    // 2. read user uploaded probe candidate file if there is one
    //   choose probe candidates from target sequences if user didn't provide probe candidates

    /*QString logFilePath = "";
    #ifdef MAC
        QString appPath = QDir::currentPath();
        logFilePath += "file:/";
        logFilePath += appPath;
        logFilePath += "/";
    #endif

    #ifdef WIN
        logFilePath += "Log\\";
    #else
        logFilePath += "Log/";
    #endif


    QDate date = QDate::currentDate();
    QString dateString = date.toString();

    QFile file(logFilePath+dateString+"_Log.txt");



    file.open(QIODevice::Append| QIODevice::Text);
    QTextStream out(&file);
    time = QTime::currentTime();
    out << time.toString()<<"\t Collecting probe candidates \n";
    file.close();*/

    time = QTime::currentTime();
    DataTools::writeErrorLog(time.toString()+"\t Collecting probe candidates \n");


    if( usrLoad )
        rt = readProbe();
    else
        rt = chooseProbeCandid();
    if( rt < 0 ){// close progress window
        delete pdProgDlg;
        /*Clean Memory: (for Error)
          4. probe Candidates
          5. probes
          6. Triples (in Primer Pair)
          8. tripleShowList
          9. T&P
          10. Score
        */
        DataTools::cleanMemoryProbeData();
        return rt;
    }

    // 3. filter probe candidates
    /*file.open(QIODevice::Append| QIODevice::Text);
    time = QTime::currentTime();
    out << time.toString()<<"\t Filter out probe candidates\n";
    file.close();*/

    time = QTime::currentTime();
    DataTools::writeErrorLog(time.toString()+"\t Filter out probe candidates\n");


    pdProgDlg->computation(2);
    rt = filterProbeCandid();
    if( rt < 0 ){
        // close progress window
        delete pdProgDlg;
        /*Clean Memory: (for Error)
          4. probe Candidates
          5. probes
          6. Triples (in Primer Pair)
          8. tripleShowList
          9. T&P
          10. Score
        */
        DataTools::cleanMemoryProbeData();
        return rt;
    }
    // 4. do approximate searching for every probes in target and non-target sequences
    /*file.open(QIODevice::Append| QIODevice::Text);
    time = QTime::currentTime();
    out << time.toString()<<"\t Do approximation search\n";
    file.close();*/

    time = QTime::currentTime();
    DataTools::writeErrorLog(time.toString()+"\t Do approximation search\n");


    rt = quickSearchProbeInSeqs();
    if( rt < 0 ){
        // close progress window
        delete pdProgDlg;
        /*Clean Memory: (for Error)
          4. probe Candidates
          5. probes
          6. Triples (in Primer Pair)
          8. tripleShowList
          9. T&P
          10. Score
        */
        DataTools::cleanMemoryProbeData();
        return rt;
    }
    // close progress window
    delete pdProgDlg;

/*
    // 5. call result dialog and add primer pair list
    TripleReportWindow  *prWin = new TripleReportWindow();
    prWin->show();*/
    return 0;
}

int Designer::allocateMemory(void)
{
    /*// allocate scoring table and pointer table for primer search
    if( DataTools::allocMemScoringTable() == FATAL_ERROR ){
        Clean Memory: (for Error)
          4. probe Candidates
          5. probes
          6. Triples (in Primer Pair)
          8. tripleShowList
          9. T&P
          10. Score

        DataTools::cleanMemoryAllData();
        exit(-1);
    }*/
    constA = 5;
    constA.Sqrt();
    constA.Sub(1);
    constA.Div(2);
    return 0;
}

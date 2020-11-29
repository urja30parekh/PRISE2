#include "DataTools.h"
void (*DataTools::_OnInsertData)(struct SequenceList* seqPt) ;
void (*DataTools::_OnCreateModel)();
//void (*DataTools::_OnDisplayAllColumn)();
//void (*DataTools::_OnLoadPrimerData)();
//void (*DataTools::_OnInsertPrimer)(int idx);
//void (*DataTools::_OnLoadProbeData)();
//void (*DataTools::_OnInsertProbe)(int idx);

DataTools::DataTools()
{
    queryNames = new QStringList();
    querySeqs = new QStringList();
}
DataTools::~DataTools()
{
    cleanMemorySeqs();
    delete queryNames;
    delete querySeqs;
}
qint64 DataTools::lastNCBIRequestTime;

bool DataTools::SearchInputAccs(QString Accs)
{
    if(Accs == "") {
        QMessageBox::about(Q_NULLPTR, "Empty Accession List", "Empty Accession List: No Input Accessions");
        return false;
    }

    int range = Accs.length();
    MyProgressDlg pDlg("Searching Seed Sequence(s) in NCBI", 0, range);
    pDlg.setWindowModality(Qt::ApplicationModal);
    int pos;
    QString acc, gi, sequence;

    Accs.replace("\r\n", "[");
    Accs.replace("\n", "[");

    QRegExp rx("[A-Z]\\S+(\\[|$)");

    while( Accs.indexOf(rx) >=0)
    {
        pos = Accs.indexOf("[");
        pos = (pos>=0?pos:Accs.length());

        acc = Accs.left(pos);
        Accs.remove(0,pos+1);
        pDlg.setValue(range - Accs.length());

        pos = Accs.indexOf("[");
        if( acc.length() > 0 ){
            QString sGetFromURL = "https://www.ncbi.nlm.nih.gov/entrez/eutils/efetch.fcgi?rettype=gb&retmode=txt&db=nucleotide&id="+acc;
            sequence = getSequence(sGetFromURL);
            if( sequence == "" ){
                QMessageBox::about(Q_NULLPTR,tr("Accession not found"),"Cannot find accession " + acc + " in NCBI!");
                pDlg.reject();

                return false;
            }
            queryNames->append(acc);
            querySeqs->append(sequence);
        }

        if (pDlg.wasCanceled()){
            QMessageBox::about(Q_NULLPTR, "Abort", "User Abort : Accession Processing is not finished");
            return false;
        }
    }
    return true;
}



bool DataTools::SearchInputAccs(QString Accs, QStringList* qNameList, QStringList* qSeqList)
{
    if(Accs == "") {
        QMessageBox::about(Q_NULLPTR, "Empty Accession List", "Empty Accession List: No Input Accessions");
        return false;
    }

    int range = Accs.length();
    MyProgressDlg pDlg("Searching Seed Sequence(s) in NCBI", 0, range);
    pDlg.setWindowModality(Qt::ApplicationModal);
    int pos;
    QString acc, gi, sequence;

    Accs.replace("\r\n", "[");
    Accs.replace("\n", "[");

    QRegExp rx("[A-Z]\\S+(\\[|$)");

    while( Accs.indexOf(rx) >=0)
    {
        pos = Accs.indexOf("[");
        pos = (pos>=0?pos:Accs.length());

        acc = Accs.left(pos);
        Accs.remove(0,pos+1);
        pDlg.setValue(range - Accs.length());

        pos = Accs.indexOf("[");
        if( acc.length() > 0 ){
            QString sGetFromURL ="https://www.ncbi.nlm.nih.gov/entrez/eutils/efetch.fcgi?rettype=gb&retmode=txt&db=nucleotide&id="+acc;
            sequence = getSequence(sGetFromURL);
            if( sequence == "" ){
                QMessageBox::about(Q_NULLPTR,tr("Accession not found"),"Cannot find accession " + acc + " in NCBI!");
                pDlg.reject();

                return false;
            }
            qNameList->append(acc);
            qSeqList->append(sequence);
        }

        if (pDlg.wasCanceled()){
            QMessageBox::about(Q_NULLPTR, "Abort", "User Abort : Accession Processing is not finished");
            return false;
        }
    }
    return true;
}


bool DataTools::GetInputSeedSeqs(QString SeedFile){
    QString seq;
    QFile file(SeedFile);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::about(Q_NULLPTR, "File Error", "Seed file doesn't exist or File corrupted");
        return false;
    }
    QTextStream fileStream ( &file );
    QString newStr= fileStream.readAll();
    newStr = newStr.replace(QChar(13),'\n');
    QTextStream stream (&newStr);

    QString line, sName;
    line = stream.readLine();
    if( line.at(0) != '>' ){
        QMessageBox::about(Q_NULLPTR, "File Error",SeedFile +" is not a FASTA file.");
        return false;
    }

    if( stream.atEnd()){
        QMessageBox::about(Q_NULLPTR, "File Error","Error reading seed sequence " + sName + ".\r\nPlease double check your seed sequence file." );
        querySeqs->clear();
        queryNames->clear();
        return false;
    }
    while( !stream.atEnd() ) {
        if( line == "" || line.at(0) != '>' )
            continue;
        line.remove(0,1);   // remove ">"
        queryNames->append(line);
        seq.clear();
        sName = line;

        while( !stream.atEnd() && (line = stream.readLine()) == "" ){
        }
        while(!line.startsWith('>')){
            seq += line.toUpper();
            if(stream.atEnd())
                break;
            line = stream.readLine();
        }
        seq.remove(' ');
        // get a sequence
        querySeqs->append(seq);
        if( seq == "" ){
            QMessageBox::about(Q_NULLPTR, "File Error","Error reading seed sequence " + sName + ".\r\nPlease double check your seed sequence file." );
            querySeqs->clear();
            queryNames->clear();
            return false;
        }
        //}
    }
    file.close();

    // check for sequence reading
    if( queryNames->size() !=querySeqs->size() ){
         QMessageBox::about(Q_NULLPTR, "File Error",SeedFile + " contains different number of sequence notations and sequences!" );
         return false;
    }
    return true;
}


bool DataTools::GetInputSeedSeqs(QString SeedFile, QStringList* qNameList, QStringList* qSeqList){
    QString seq;
    QFile file(SeedFile);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::about(Q_NULLPTR, "File Error", "Seed file doesn't exist or File corrupted");
        return false;
    }
    QTextStream fileStream ( &file );
    QString newStr= fileStream.readAll();
    newStr = newStr.replace(QChar(13),'\n');
    QTextStream stream (&newStr);

    QString line, sName;
    line = stream.readLine();
    if( line.at(0) != '>' ){
        QMessageBox::about(Q_NULLPTR, "File Error",SeedFile +" is not a FASTA file.");
        return false;
    }

    if( stream.atEnd()){
        QMessageBox::about(Q_NULLPTR, "File Error","Error reading seed sequence " + sName + ".\r\nPlease double check your seed sequence file." );
        qSeqList->clear();
        qNameList->clear();
        return false;
    }
    while( !stream.atEnd() ) {
        if( line == "" || line.at(0) != '>' )
            continue;
        line.remove(0,1);   // remove ">"
        qNameList->append(line);
        seq.clear();
        sName = line;

        while( !stream.atEnd() && (line = stream.readLine()) == "" ){
        }
        while(!line.startsWith('>')){
            seq += line.toUpper();
            if(stream.atEnd())
                break;
            line = stream.readLine();
        }
        seq.remove(' ');
        // get a sequence
        qSeqList->append(seq);
        if( seq == "" ){
            QMessageBox::about(Q_NULLPTR, "File Error","Error reading seed sequence " + sName + ".\r\nPlease double check your seed sequence file." );
            qSeqList->clear();
            qNameList->clear();
            return false;
        }
        //}
    }
    file.close();

    // check for sequence reading
    if( qNameList->size() !=qSeqList->size() ){
         QMessageBox::about(Q_NULLPTR, "File Error",SeedFile + " contains different number of sequence notations and sequences!" );
         return false;
    }
    return true;
}

QNetworkReply::NetworkError DataTools::downloadURL(QUrl url, QByteArray &data)
{
    // This is so we don't query NCBI so quickly that it gives "Too many requests" error.
    const int MIN_MS_BETWEEN_REQUESTS = 334;


    qint64 now = QDateTime::currentMSecsSinceEpoch();
    if (DataTools::lastNCBIRequestTime + MIN_MS_BETWEEN_REQUESTS > now) {
        qDebug() << "Making a request too quickly, so we are taking a break for " << (DataTools::lastNCBIRequestTime + MIN_MS_BETWEEN_REQUESTS) - now << " milliseconds";
        QThread::msleep((DataTools::lastNCBIRequestTime + MIN_MS_BETWEEN_REQUESTS) - now);
    }
    DataTools::lastNCBIRequestTime = QDateTime::currentMSecsSinceEpoch();

    QNetworkAccessManager manager;
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::readyRead, [&data, &reply]() {
        data += reply->readAll();

        //terminate early if exceed max
        if(!downloadAllSeqs && data.length() > MAX_SEQUENCE_DOWNLOAD) {
            reply->close();
            emit reply->finished();
        }
    });
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    if(!downloadAllSeqs && data.length() > MAX_SEQUENCE_DOWNLOAD) {
        delete reply;
        return QNetworkReply::NoError;
    }

    if (reply->error() != QNetworkReply::NoError)
    {
        return reply->error();
    } else {
        data += reply->readAll();
        //qDebug() << "Got" << data.length() << "bytes of data, MAX: " << MAX_SEQUENCE_DOWNLOAD;
        delete reply;
        return QNetworkReply::NoError;
    }
}

QString DataTools::getSequence(QString sGetFromURL)
{
    QUrl url(sGetFromURL);
    QByteArray InfoNCBI;

    QNetworkReply::NetworkError errorCode = downloadURL(url, InfoNCBI);
    if (errorCode != QNetworkReply::NoError)
    {
        QMessageBox::about(Q_NULLPTR,tr("Internet Error "), tr("Internet Error %1: Failed to connect to NCBI.\t\nPlease check your internect connection.").arg(errorCode));
        return "";
    }
    QString Info(InfoNCBI);

    QHash<QString, QString> hashinfo = parseSeqInfo(Info);
    return  getSequence(hashinfo);
}

void DataTools::SaveQueryList()
{
    QueryList *queryPtr;
    //int pos;
    int len;
    for( int i = 0; i < querySeqs->size(); i++ ){
        queryPtr = new QueryList;
        //queryPtr->gi = queryNames->at(i);
        //pos = queryPtr->gi.indexOf("gi|");
        //if( pos >= 0 ){
        //    queryPtr->gi.remove(0, pos+3);
        //    pos = queryPtr->gi.indexOf('|');
        //    queryPtr->gi = queryPtr->gi.left(pos);
        //}
        queryPtr->definition = queryNames->at(i);
        len = (static_cast<QString>(querySeqs->at(i))).length();
        queryPtr->length = len;
        queryPtr->seqInt = new short[static_cast<unsigned long>(queryPtr->length+1)];
        queryPtr->seqIntRev = new short[static_cast<unsigned long>(queryPtr->length+1)];
        queryPtr->seqInt[0] = -1;
        queryPtr->seqIntRev[0] = -1;

        for( int j = 0; j < len; j++ ){
            QChar  b = (static_cast<QString>(querySeqs->at(i))).at(j);
            short bInt = static_cast<short>(tools::base2int(b));
            queryPtr->seqInt[j+1] = bInt;
            queryPtr->seqIntRev[len-j] = 3-bInt;
            if( bInt < 0 ){
        //        QMessageBox::about(Q_NULLPTR,"Sequence Error ","Sequence reading error for sequence " + queryPtr->gi +
        //                           "\nProgram is forced to terminate.\nPlease report this problem to the developer.");
                cleanMemorySeqs();
                exit(-1);
            }else if (bInt >=4 && bInt != 9){
               // QMessageBox::about(Q_NULLPTR,"Sequence Error ","Sequence reading error for sequence " + queryPtr->gi +
              //                     "\nProgram is forced to terminate.\nPlease make sure that seed sequence contains exact base A, C, G or T.");
                cleanMemorySeqs();
                exit(-1);
            }
        }
        qList.push_back(queryPtr);
    }
}

QString DataTools::GetAccessionFromHitTable(QString line)
{
    QString str;
    int pos = line.indexOf("\t");
    line.remove(0, pos + 1);
    pos = line.indexOf(".");
    return line.left(pos);
}

QString DataTools::GetSeqNameFromHitTable(const QString &line)
{
    return line.left(line.indexOf("\t"));
}

int DataTools::FindSeqWithGI(int, QString)
{
    return -1;
}

bool DataTools::ParseHitTable(const QString &filename)
{
    //open Hit Table File.
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::about(Q_NULLPTR, "File Error", "File doesn't exist or File corrupted");
        return false;
    }

    // check file type
    QTextStream stream ( &file );
    QString line, sName;
    line = stream.readLine();
    if( stream.atEnd()  || ((line.indexOf("# BLASTN") < 0) && (line.indexOf("# blastn") < 0)) ){
        printf("not hit table. First line: %s\n", line.toStdString().c_str());
        QMessageBox::about(Q_NULLPTR, "File Error", filename + " is not a \"hit table\" file!");
        return false;
    }

    //int index;
    //Start to read file
     while( !stream.atEnd() ) {
         line = stream.readLine();
         if( line == ""){
             continue;
         }
         // skip hit table header lines
         if( line.at(0)== '#' ){
             continue;
         }

         struct SequenceList* seqPtr =  new SequenceList();
         for( int i = 0; i < qList.size(); i++ ){
             seqPtr->identity.push_back(0);
         }

         //TODO: Daniel, This is not as well optimized, come back to this later....
         QStringList tokens = line.split(QRegExp("\\s+"));

         //seqPtr->gi		= GetGIFromHitTable(line);
         seqPtr->accession	= GetAccessionFromHitTable(line);//tokens[1].left(tokens[1].indexOf("."));
         qDebug() << "Accession " << seqPtr->accession.toStdString().c_str() << "\n";
         seqPtr->seqName	= GetSeqNameFromHitTable(line);//tokens[0];
         //qDebug() << seqPtr->accession << seqPtr->seqName;
         // initialize other stuff
         seqPtr->marked	= 0;
         seqPtr->length		= 0;
         seqPtr->definition	= "";
         seqPtr->organism	= "";
         seqPtr->authors	= "";
         seqPtr->title		= "";
         seqPtr->seqInt		= Q_NULLPTR;
         seqPtr->reverseComplement = false;


         //alignment length
         int length  = tokens[3].toInt();

         // query start/end, subject start/end
         int q_start = tokens[6].toInt();
         int q_end   = tokens[7].toInt();
         int s_start = tokens[8].toInt();
         int s_end   = tokens[9].toInt();

         if (s_start > s_end) {
             qDebug() << "Sequence " << seqPtr->accession << " is reversed" << "\n";
             seqPtr->reverseComplement = true;
             swap(s_start, s_end);
         }

         // the final start and end values are calculated based on qstart/end
         // and sstart/end
         //according to Chrobak.
         seqPtr->start = s_start - q_start + 1;
         //seqPtr->end = s_end + qAbs(q_end - q_start) - q_end;
         // We need to set the end only when we know the seed lengths
         // seqPtr->end   = seqPtr->start + length - 1;

         sList.push_back(seqPtr);
     }
     file.close();
     return true;
}

/*return value:
    -1     : internet error or data error
     0     : search finished
     1     : user abort*/
int DataTools::SearchNCBI()
{
    int range = qList.size()+sList.size();
    qDebug() << "qList size: " << qList.size() << ", sList size: %d\n" << sList.size();
    MyProgressDlg pDlg("Searching sequences in NCBI nucleotide database", 0, range);
    pDlg.setWindowModality(Qt::ApplicationModal);
    pDlg.show();

    // Preprocess maxSeedLength and the initial download list.
    int i;
    int maxSeedLength = 0;
    for(i = 0; i < qList.size(); i++ ){
        QApplication::processEvents();

        pDlg.setValue(i);
        pDlg.setLabelText("Searching sequences in NCBI nucleotide database\n\n"+
                           tr("Processed %1 sequences out of %2 \n"
                              "%3 download failures").arg(i+1).arg(range).arg(notDownloadList.size()));

        SequenceList * seqPtr = new SequenceList;
        for( int j = 0; j < qList.size(); j++ ) {
            seqPtr->identity.push_back(0);
        }
        //seqPtr->gi		= "SEED " + qList[i]->gi;
        seqPtr->accession	= "";
        seqPtr->seqName   	= ">Seed sequence|";
        seqPtr->marked		= 0;
        seqPtr->length		= qList[i]->length;
        seqPtr->definition	= qList[i]->definition;
        seqPtr->organism	= "";
        seqPtr->source		= "";
        seqPtr->features	= "";
        seqPtr->authors		= "";
        seqPtr->title		= "";
        seqPtr->seqInt		= new short[static_cast<unsigned long>(seqPtr->length+1)];

        for( int j = 0; j <= seqPtr->length; j++ )
                seqPtr->seqInt[j] = qList.at(i)->seqInt[j];

        maxSeedLength = max(maxSeedLength, qList[i]->length);

        sList.insert(sList.begin()+i, seqPtr );

        // downloadList.insert(downloadList.begin() + i, seqPtr);
        _OnInsertData(seqPtr);

        if(pDlg.wasCanceled()){
            QMessageBox::about(Q_NULLPTR, "Abort", "User Abort : Download is not finished");
            cleanMemorySeqs();
            return 1;
        }
    }



    bool shouldAskBeforeDownloading = false;
    int nToBeDownloaded = sList.length();

    bool downloaded[sList.length()];
    bool deleted[sList.length()];
    bool tooLongToDownload[sList.length()];

    memset(downloaded, 0, sizeof downloaded);
    memset(deleted, 0, sizeof deleted);
    memset(tooLongToDownload, 0, sizeof tooLongToDownload);

    while (nToBeDownloaded) {
        int nNewDownloadFailures = 0;
        if (shouldAskBeforeDownloading) {
            MyConfirmDlg* cDlg = new MyConfirmDlg(QString("%1 sequences are not downloaded successfully due to connection errors.\n\n\n\n"
                                                          "Do you want to retry?").arg(nToBeDownloaded));
            if(!cDlg->exec()){
                qDebug() << "Cancelled - stop retrying and exit";
                break;
                delete cDlg;
            } else {
                qDebug() << "Accepted retry";
                delete cDlg;
            }
        }
        shouldAskBeforeDownloading = true;

        for(int cnt = i = 0; i < sList.size(); i++ ){
            if (downloaded[i]) continue;
            if (deleted[i]) continue;

            QApplication::processEvents();
            // user abort
            if (pDlg.wasCanceled()) {
                cleanMemorySeqs();
                QMessageBox::about(Q_NULLPTR, "Abort", "User Abort : Download is not finished");
                return 1;
            }
            pDlg.setValue(cnt);
            pDlg.setLabelText("Searching sequences in NCBI nucleotide database\n\n"+
                               tr("Processed %1 sequences out of %2\n"
                                  "%3 download failures").arg(++cnt).arg(nToBeDownloaded).arg(nNewDownloadFailures));


            // How much missing from any end should we tolerate, before we decide to discard the result?
            const int FUDGE_TOLERANCE = 25;

            // How much extra "padding" should we download from each end?
            const int FUDGE_INCLUSION = 25;

            // We can only set the end here, after we know the max seed length. There wasn't enough
            // info in the hit table to determine the end.
            sList[i]->end = sList[i]->start + maxSeedLength - 1;

            int re = SearchGIinNCBI ( sList[i], FUDGE_INCLUSION, FUDGE_TOLERANCE, maxSeedLength);

            qDebug() << "NCBI Database Return Value:" << re;
            switch(re)
            {
            case(-4): // Sequence's start is not available in ncbi
                qDebug() << "Hit with accession: " << sList[i]->accession << " matches on a sequence that doesn't have enough at the beginning. It would have started at: " << sList[i] -> start;

                deleted[i] = true;
                break;
            case(-3): // Sequence's end is not available in ncbi
                qDebug() << "Query with accession " << sList[i]->accession <<
                            " does not match NCBI sequence completely - it would run off the end. ";
                qDebug() << "Got length " << sList[i]->length << " but needed " << maxSeedLength;

                deleted[i] = true;
                break;
            case(-2): //data error
            case(-1): //internet error
                // delete the rest unfinished sequences
               /* for(int j = sList.size()-1; j >= i; j-- ){
                    delete sList[j];
                }
                sList.erase(sList.begin() + i, sList.end());
                delete pDlg;
                return -1;*/
                nNewDownloadFailures ++;
                downloaded[i] = false; // no-op
                continue;

            case(1):  //didn't find the sequence
                nNewDownloadFailures ++;
                downloaded[i] = false; // no-op
                break;
            case(2): //sequence is too long
                nNewDownloadFailures ++;
                downloaded[i] = false; // no-op
                tooLongToDownload[i] = true;
                deleted[i] = true;
                continue;
            default:
                downloaded[i] = true;
                break;
            }
            //ahiya ek method create karo ema sList[i] passs karavo and ene directly ek word or excel ma print..
            ///////////////////////////////////////////////////////////////////////////////////////////


            // [Collect model data to QString]
            QString textData;

//            int rows = std::extent<decltype(sList), 0>::value;
//            int cols = std::extent<decltype(sList), 1>::value;
            QString str = "Number, Accession Number, % Identity, Length, Definition, Organism";


            QStringList headers = str.split(",");
//            for( int i = 1; i < this->tableModel->columnCount(); i++ ){
//                QString header = tableModel->headerData(i, Qt::Horizontal).toString();
//                headers.push_back(header);
//            }
            DataTools::saveSeqList2TXT("/Users/ianbonafede/Downloads/Save_Downloaded_Seq.txt",sList, headers);




//            for (int i = 1; i < rows; i++) {
//                for (int j = 1; j < cols; j++) {

//                        textData += sList[i][j++].;
//                        textData += ", ";     // for .csv file format
//                        textData += sList[i][j++].definition;
//                        textData += ", ";     // for .csv file format
//                }
//                textData += "\n";             // (optional: for new line segmentation)
//            }

//            textData += sList[0][1].accession += '\n';
//            textData += sList[0][2].accession += '\n';
//            textData += sList[0][3].accession += '\n';
//            textData += sList[0][4].accession += '\n';
//            textData += sList[0][5].accession += '\n';
//            textData += sList[1][0].accession += '\n';
//            textData += sList[1][1].accession += '\n';
//            textData += sList[1][2].accession += '\n';
//            textData += sList[1][3].accession += '\n';
//            textData += sList[1][4].accession += '\n';
//            textData += sList[1][5].accession += '\n';

            // [Save to file] (header file <QFile> needed)
            // .csv
//            QFile csvFile("/Users/ianbonafede/Downloads/test.csv");
//            if(csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {

//                QTextStream out(&csvFile);
//                out << sList.data();
//                out << '\n';
//                out << sList.indexOf(0,1);
//                out << '\n';
//                out << sList.value(1);

//                csvFile.close();
//            }

            // .txt
//            QFile txtFile("/Users/ianbonafede/Downloads/test.txt");
//            if(txtFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {

//                QTextStream out(&txtFile);
//                out << sList.data();
//                out << '\n';
//                out << sList.indexOf(0,1);
//                out << '\n';
//                out << sList.value(1);


//                txtFile.close();
//            }
            /// ///////////////////////////////////////////////////////////////////////////////////////////
            _OnInsertData(sList[i]); //show data in tableView
        }

        nToBeDownloaded = 0;
        for (int i = 0; i < sList.length(); i++) {
            if (!downloaded[i]) {
                nToBeDownloaded ++;
            }
        }
    }

    // now, after caching the stuff to do to the lists in the retry loop, actually
    // do it.
    for (int i = 0; i < sList.length(); i++) {
        if (tooLongToDownload[i]) {
            accTooLongSeqToDownload.append(sList[i]->accession);
        }
    }

    for (int i = 0; i < sList.length(); i++) {
        if (!deleted[i] && !downloaded[i]) {
            notDownloadList.append(sList[i]);
        }
    }

    // iterate backwards for deleting so we can delete one by one without changing
    // the indexes.
    for (int i = sList.length() - 1; i >= 0; i--) {
        if (deleted[i]) {
            sList.remove(i);
        }
    }

    return 0;
}

QHash<QString, QString> DataTools::parseSeqInfo(QString Information)
{
    QHash<QString, QString> hashinfo;

    int pos;
    QRegExp rx("\\n([A-Z]|//)");
    while((pos = Information.indexOf(rx, 1)) >=0){
        QString field = Information.left(pos);
        QString title = field.left(field.indexOf(" "));
        QString content = field.remove(QRegExp(title+" *"));
        hashinfo.insertMulti(title, content);
        Information = Information.remove(0,pos+1);
    }
    return hashinfo;
}

int DataTools::getLengthInfo (QHash<QString, QString> hashinfo)
{
    QHash<QString, QString>::const_iterator i = hashinfo.find("LOCUS");
    if(i == hashinfo.end()){
        return 0;
    }
    QString locus  = i.value();
    QString sLen;
    int iLen;
    QRegExp rx(" \\d+ bp");
    int pos = locus.indexOf(rx);
    if (pos >=0){
        locus = locus.remove(0,pos+1);
        sLen = locus.left(locus.indexOf(" bp"));
        bool* OK = new bool(true);
        iLen = sLen.toInt(OK);
        if(OK)
        {
            delete OK;
            return iLen;
        }
        delete OK;
    }
    return -1;
}

bool DataTools::getGI (QHash<QString, QString> hashinfo, QString& gi)
{
    QHash<QString, QString>::const_iterator i = hashinfo.find("VERSION");
    if(i == hashinfo.end()){
        return false;
    }
    gi  = i.value();

    int pos = gi.indexOf("GI:");
    if (pos >=0){
        gi = gi.remove(0,pos+3);
        return true;
    }
    return false;
}

QString DataTools::getSequence (QHash<QString, QString> hashinfo)
{
    QHash<QString, QString>::const_iterator i = hashinfo.find("ORIGIN");
    if(i == hashinfo.end()){ // need to re-search NCBI for complete sequence
        return "NONE";
    }
    QString seq= i.value();
    int startPos = seq.indexOf('\n');
    seq.remove(0, startPos);

    int pos = seq.indexOf("https://");
    if( pos >= 0 ){ // need to re-search NCBI for complete sequence
        QMessageBox::about(Q_NULLPTR, "GB file format problem", "Contain URL,Need to research");
        return "NONE";
    }

    QRegExp rx(" |1|2|3|4|5|6|7|8|9|0|\\a|((\\r)?\\n)");
    seq.remove(rx);
    seq = seq.toUpper();
    return seq;
}

QString DataTools::getSequenceFASTA(QString gi){
    QString NCBI_URL = "https://www.ncbi.nlm.nih.gov/entrez/eutils/efetch.fcgi?rettype=fasta&retmode=txt&db=nucleotide&id=";

    QString sGetFromURL = NCBI_URL+gi;


    QUrl url(sGetFromURL);
    QByteArray seqRead;

    int errorCode = downloadURL(url, seqRead);
    if (errorCode != 0 ){

        DataTools::writeErrorLog(QString("Internet Error %1: Failed to connect to NCBI.\n").arg(errorCode));
        DataTools::writeErrorLog(sGetFromURL);
        return "NONE";
    }

    QString seqFASTA(seqRead);
    if (seqFASTA == "\n"){
        return "NONE";
    }

    int firstLine = seqFASTA.indexOf("\n");
    seqFASTA = seqFASTA.remove(0, firstLine+1);
    //seqFASTA.replace("\n","");
    seqFASTA.remove('\n');

    //cout << seqFASTA.toStdString() << endl;

    return seqFASTA;
}


bool DataTools::getSOURCEandORGANISM (QHash<QString, QString> hashinfo, QString& source, QString& organism )
{
     QHash<QString, QString>::const_iterator i = hashinfo.find("SOURCE");
     if(i == hashinfo.end()){
         return true;
     }
     QString all  = i.value();
     QString sLen;
     QRegExp rx("\n  ORGANISM *");

     int pos = all.indexOf(rx);
     if (pos >=0){
         source = all.left(pos);
         source.remove("\n");
         all.remove(0,pos);
         all.remove(rx);
         rx.setPattern("\n  [A-Z]");
         pos = (all.indexOf(rx)>=0?all.indexOf(rx):all.length());
         organism = all.left(pos);
         organism.remove(0,organism.indexOf("\n")+1);
         organism = organism.trimmed();
         organism.remove("\n");
         return true;
     }
     return false;
}

bool DataTools::getAUTHORSandTITLE (QHash<QString, QString> hashinfo, QString& authors, QString& titles )
{
    authors = "";
    titles = "";

    QString test;
    QHash<QString, QString>::iterator i = hashinfo.find("REFERENCE");

    while (i != hashinfo.end() && i.key() == "REFERENCE"){
        QString tmp_authors = "", tmp_title = "";
        QString all  = i.value();
        QRegExp rx("\n  [A-Z]*");

         int pos = all.indexOf(rx, 1);
         all.remove(0,pos+3);
         while((pos = all.indexOf(rx, 1)) >= 0){
             QString field = all.left(pos);
             QString title = field.left(field.indexOf(" "));
             QString content = field.remove(QRegExp(title+" *"));
             all = all.remove(0,pos+3);
             if(title == "AUTHORS"){
                 tmp_authors =  content;
             }else if (title == "TITLE"){
                 tmp_title = content;
             }
         }
         if(tmp_authors == "" && tmp_title == ""){
             QMessageBox::about(Q_NULLPTR,tr("data Error "), tr("Cannot parse information from NCBI: Reference"));
             return false;
         }
         authors += (tmp_authors+";");
         authors.remove("\n");
         titles += (tmp_title+";");
         titles.remove("\n");
         ++i;
     }
     return true;
}

/*return value:
    -4     : NCBI doesn't have the beggining of the sequence
    -3     : NCBI doesn't have the end of the sequence
    -2     : data error
    -1     : internet error
     0     : search finished
     1     : sequence not found
     2     : sequence too long*/
int DataTools::SearchGIinNCBI(struct SequenceList* seqPtr, int fudgeInclusion, int fudgeTolerance, int maxSeedLength)
{
    int seq_start = seqPtr->start;

    int seq_end = seqPtr->end;


    // We use this tolerated length to determine if enough of the end sequence got read.
    // For this tolerated length, it is assuming the start + inclusion will be successful.
    int toleratedLength = maxSeedLength - fudgeTolerance - fudgeInclusion;

    seq_start -= fudgeInclusion;
    seq_end += fudgeInclusion;


    if (seq_start < 1 && seq_start >= (-fudgeTolerance - fudgeInclusion + 1)) {
        // We are before the start of the sequence, but there's enough in here to still include it.
        seqPtr->length -= (-seq_start + 1);
        toleratedLength = -seq_start + 1;
        seq_start = 1;
    }

    if (seq_start < 1) {
        return -4; // The query requires parts of the sequence that starts before the sequence
    } else {
        QString NCBI_URL =
                QString("https://www.ncbi.nlm.nih.gov/entrez/eutils/efetch.fcgi?"
                        "rettype=gb"
                        "&retmode=txt"
                        "&db=nucleotide"
                        "&id=%1"
                        "&seq_start=%2"
                        "&seq_stop=%3")
                        .arg(seqPtr->accession)
                        .arg(seq_start)
                        .arg(seq_end);

        qDebug() << "Downloading " << NCBI_URL << "\n";

        QUrl url(NCBI_URL);
        QByteArray InfoNCBI;

        int errorCode = downloadURL(url, InfoNCBI);
        if (errorCode != 0 ){

            DataTools::writeErrorLog(QString("Internet Error %1: Failed to connect to NCBI.\n").arg(errorCode));
            DataTools::writeErrorLog(NCBI_URL);
            return -1;
        }
        QString Info(InfoNCBI);
        if (Info == "\n"){
            return 1;
        }
        //qDebug() << Info;
        int res = assignSeq(seqPtr, parseSeqInfo(Info));

        if (res) {
            return res;
        }


        if (seqPtr->length < toleratedLength) {
            return -3;
        }

        return 0;
    }
}

/*return value:
    -2     : data error
     0     : search finished
     2     : sequence too long*/
int DataTools::assignSeq(struct SequenceList* seqPtr, QHash<QString, QString> hashInfo)
{
    int retrievedLength = getLengthInfo(hashInfo);

    if ( retrievedLength <=0 ){
        QMessageBox::about(Q_NULLPTR,tr("data Error "), tr("Cannot parse information from NCBI: length"));
        return -2;
    }else if( !downloadAllSeqs && seqPtr->length > MAX_SEQUENCE_DOWNLOAD){
        return 2;
    }

    seqPtr->length = retrievedLength;


    QHash<QString, QString>::const_iterator i = hashInfo.find("DEFINITION");
    if(i != hashInfo.end()){
        seqPtr->definition = i.value();
        seqPtr->definition.remove("\n");
    }

    if(!getSOURCEandORGANISM(hashInfo,seqPtr->source, seqPtr->organism) ){
        QMessageBox::about(Q_NULLPTR,tr("data Error "), tr("Cannot parse information from NCBI: Source, Organism"));
        return -2;
    }

    if( !isSimple){
        if(!getAUTHORSandTITLE(hashInfo,seqPtr->authors, seqPtr->title) ){
            QMessageBox::about(Q_NULLPTR,tr("data Error "), tr("Cannot parse information from NCBI: Authors, Title"));
            return -2;
        }
        QHash<QString, QString>::const_iterator i = hashInfo.find("FEATURES");
        seqPtr->features = i.value();
        seqPtr->features.remove(0,seqPtr->features.indexOf("\n")+1);
        seqPtr->features = seqPtr->features.trimmed();
        seqPtr->features.remove("\n");
    }

    QString seq = getSequence(hashInfo);
    if (seq.length() !=seqPtr->length) {
        QMessageBox::about(Q_NULLPTR,tr("data Error "), QString("Sequence length doesn't match\n%1 : %2").arg(seq.length()).arg(seqPtr->length));
        cleanMemorySeqs();
        return -2;
    }

    short *seq2int = new short[static_cast<unsigned long>(seq.length()+1)];
    seq2int[0] = -1;
    for(int i = 0; i<seq.length(); i++){
        seq2int[i+1] = static_cast<short>(tools::base2int(seq.at(i)));
        if (seq2int[i+1] <0){
            cleanMemorySeqs();
            exit(1);
        }
    }
    seqPtr->seqInt = seq2int;
//    isrev = seqPtr->reverseComplement;
    if (seqPtr->reverseComplement) {
       /* // Reverse
        for (int i = 0; i < seq.length()/2; i++) {
            swap(seqPtr->seqInt[i+1], seqPtr->seqInt[seq.length() - i]);
        }
        // Complement
        for (int i = 0; i < seq.length(); i++) {
            seqPtr->seqInt[i+1] = tools::int2complement(seqPtr->seqInt[i+1]);*/

        std::cout << "Without complement : " << seq.toStdString() << std::endl;
        std::cout << "Accession Number : " << seqPtr->accession.toStdString() << std::endl;

        std::string DNAseq = seq.toStdString();
            reverse(DNAseq.begin(), DNAseq.end());
            for (std::size_t i = 0; i < DNAseq.length(); ++i){
                switch (DNAseq[i]){
                case 'A':
                    DNAseq[i] = 'T';
                    break;
                case 'C':
                    DNAseq[i] = 'G';
                    break;
                case 'G':
                    DNAseq[i] = 'C';
                    break;
                case 'T':
                    DNAseq[i] = 'A';
                    break;
                }
            }
            std::cout << "reverse complement : " << DNAseq << std::endl;
        }

    return 0;
}

bool DataTools::loadSeed(QString SeedAccs, QString SeedFile)
{
    if(SeedAccs != ""){ //user input sequence Accessions
        if(! SearchInputAccs(SeedAccs)){
            return false;
        }
        return true;
    }
    if(SeedFile != "" ){ // user uploaded seed sequence file
        if(! GetInputSeedSeqs(SeedFile)){
            return false;
        }
    }
    return true;
}

bool DataTools::getFastaSequences(QString FASTAFile)
{
    QStringList seqNames, seqs;
    QString seq;
    SequenceList *seqPtr;
    short* seqInt;
    int pos;

    seqNames.clear();
    seqs.clear();

    QFile file(FASTAFile);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::about(Q_NULLPTR, "File Error", "FASTA file doesn't exist or File corrupted");
        return false;
    }

    // start reading
    QTextStream stream ( &file );
    QString line, sName;
    line = stream.readLine();
    if( line.at(0) != '>' ){
        QMessageBox::about(Q_NULLPTR, "File Error",FASTAFile +" is not a FASTA file.");
        return false;
    }

    while( !stream.atEnd() ) {
        QApplication::processEvents();
        if( line == "" || line.at(0) != '>' )
            continue;
        line.remove(0,1);   // remove ">"
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
        seq.remove(' ');
        seqs.append(seq);
    }
    // finish reading
    file.close();

    int range = seqs.size();
    MyProgressDlg pDlg("Loading FASTA sequences", 0, range);
    pDlg.setWindowModality(Qt::ApplicationModal);

    for(int i = 0; i<seqs.size(); i++){
        QApplication::processEvents();
        pDlg.setValue(i);
        pDlg.setLabelText("Loading FASTA Sequence\n\n"+
                           tr("%1 were finished out of %2\n").arg(i).arg(range));
        seqInt = new short[static_cast<unsigned long>(static_cast<QString>(seqs[i]).length()+1)];
        seqInt[0] = -1;
        int j = 0;
        while( j < seqs.at(i).size()){
            seqInt[j+1] = static_cast<short>(tools::base2int(seqs.at(i).at(j)));
            j++;
        }
        seqPtr = new SequenceList;
        line = seqNames[i];
        pos = line.indexOf("gi|");
        if( pos < 0 ){
            line=tr("%1").arg(i+1);
            //seqPtr->gi = "FASTA "+line;
        }else{
            line.remove(0, pos+3 );
            pos = line.indexOf("|");
            //seqPtr->gi = "FASTA " + line.left(pos);
        }

        seqPtr->accession = "";
        seqPtr->authors = "";
        seqPtr->definition = seqNames[i];
        seqPtr->features = "";
        seqPtr->length = static_cast<QString>(seqs[i]).size();
        seqPtr->marked = 0;
        seqPtr->organism = "";
        seqPtr->seqInt = seqInt;
        seqPtr->seqName = ">";
        seqPtr->source = "";
        seqPtr->title = "";
        for( int k = 0; k < qList.size(); k++ )
                seqPtr->identity.push_back(0.0f);

        sList.push_back(seqPtr);

        if (pDlg.wasCanceled()){
            cleanMemorySeqs();
            QMessageBox::about(Q_NULLPTR, "Abort", "User Abort : Download is not finished");
            return 1;
        }
        _OnInsertData(seqPtr);
    }

    seqNames.clear();
    seqs.clear();;
    return true;
}

bool DataTools::UpdateIdentity()
{
    QString str;
    accTooLongSeqToAlign.clear();

    for(int i = 0; i < qList.size(); i++){
        if( qList[i]->length > MAX_ALIGN_SIZE ){
            //accTooLongSeqToAlign.append(qList[i]->gi);
        }
    }

    int range = sList.size();
    MyProgressDlg pDlg("Updating percentage identity of each subject sequence and the corresponding seed sequence", 0, range);
    pDlg.setWindowModality(Qt::ApplicationModal);

    for(int i = 0; i < range; i++){
        pDlg.setValue(i);
        pDlg.setLabelText("Updating % identity of subject and seed sequences\n\n"+
                           tr("%1 sequences out of %2 have been updated\n").arg(i).arg(range));

        if(sList[i]->length > MAX_ALIGN_SIZE)
            accTooLongSeqToAlign.append( sList[i]->accession);

        for(int j = 0; j < qList.size(); j++){
            if (qList[j]->length > MAX_ALIGN_SIZE || sList[i]->length > MAX_ALIGN_SIZE){
                sList[i]->identity[j] = TINY_IDENTITY_VALUE;
                continue;
            }
            float forward = sAlign->NWalign(qList[j]->seqInt, qList[j]->length+1, sList[i]->seqInt, sList[i]->length+1);
            float reverse = sAlign->NWalign(qList[j]->seqIntRev, qList[j]->length+1, sList[i]->seqInt, sList[i]->length+1);
            //            float forward = sAlign->HirschbergAlign(qList[j]->seqInt, qList[j]->length+1, sList[i]->seqInt, sList[i]->length+1);
            //            float reverse = sAlign->HirschbergAlign(qList[j]->seqIntRev, qList[j]->length+1, sList[i]->seqInt, sList[i]->length+1);
            sList[i]->identity[j] = max(forward, reverse);
        }

        _OnInsertData(sList[i]);
        if (pDlg.wasCanceled()){
            i++;
            while( i < sList.size() ){
                    _OnInsertData(sList[i]);
                    i++;
            }
            return false;
        }
    }
    return true;
}

void DataTools::loadSeqList(QString filename, QVector<struct SequenceList*>& loadList){
    cleanMemorySeqs();
    //Open Sequence File.
    QFile file(filename);
    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::about(Q_NULLPTR, "File Error", "File doesn't exist or File corrupted");
        return;
    }

    // check file type
    QByteArray compressedData, origData;
    QTextStream stream(&origData);
    QString line;

    compressedData = file.readAll();
    origData = qUncompress(compressedData);

    file.close();
    struct SequenceList* seqPtr;
    struct QueryList* qPtr;
    int qListSize, loadListSize;
    int iPolicy;

    line = stream.readLine();
    if(line.compare("Sequence list file version 1.0, please don't modify.")){
        QMessageBox::about(Q_NULLPTR, "File Error", QString("%1 is not a sequence list file.").arg(filename));
        return;
    }

    line = stream.readLine(); // read line  "Pairwise alignment settings:"

    // Load settings
    stream >> mismatchPenalty >> gapOpenPenalty >> gapExtnPenalty >> iPolicy >> qListSize;
    policy  = static_cast<countGapPolicy>(iPolicy);

    // Load query lists
    line = stream.readLine(); // start to read seed sequence
    for(int i = 0; i < qListSize; i++){
        qPtr = new QueryList;
        qList.push_back(qPtr);

        line = stream.readLine();
        qPtr->definition = line;
        //qPtr->gi = qPtr->definition;

        //int pos = qPtr->gi.indexOf("gi\\|");
        //if(pos >= 0){
        //    qPtr->gi.remove("gi\\|*\\|");
        //}

        QString lenStr;
        stream >> lenStr;
        int len = lenStr.toInt();
        stream.readLine();

        qPtr->length=len;
        Q_ASSERT(len > 0);
        qPtr->seqInt = new short[static_cast<unsigned long>(len)];
        qPtr->seqIntRev = new short[static_cast<unsigned long>(len)];
        qPtr->seqInt[0] = -1;
        qPtr->seqIntRev[0] = -1;

        for(int j = 0; j < len; j++){
            bool OK;
            QString tmpBase=stream.read(1);
            int base = tmpBase.toInt(&OK);

            if(!OK){
                QMessageBox::about(Q_NULLPTR, "File Error", "Error loading sequence list file! \n Please report the problem to the developer");
                cleanMemorySeqs();
                return;
            }else if (base < 0 || (base >=4 && base != 9)){
                //QMessageBox::about(Q_NULLPTR,"Sequence Error ","Sequence reading error for sequence " + qPtr->gi +
                //                   "\nProgram is forced to terminate.\nPlease make sure that seed sequence contains exact base A, C, G or T.");
                cleanMemorySeqs();
                exit(-1);
            }
            qPtr->seqInt[j+1] = static_cast<short>(base);
            qPtr->seqIntRev[len-j] = 3-static_cast<short>(base);
        }
        line = stream.readLine();
    }


    // Load sequences
    int i_isSimpleMode;
    stream >> loadListSize;
    stream.readLine();
    stream >> i_isSimpleMode;
    isSimple = static_cast<bool>(i_isSimpleMode);
    _OnCreateModel();
    stream.readLine();

    int range = loadListSize;
    float id;
    MyProgressDlg pDlg("Loading Sequence List", 0, range);
    pDlg.setWindowModality(Qt::ApplicationModal);

    for(int i = 0; i < range; i++){
        QApplication::processEvents();
        pDlg.setValue(i);
        pDlg.setLabelText("Loading Sequence List\n\n"+
                           tr("%1 sequences out of %2 have been processed\n").arg(i).arg(range));

        seqPtr = new SequenceList;

        line = stream.readLine();
        //seqPtr->gi = line;
        line = stream.readLine();
        seqPtr->accession = line;
        line = stream.readLine();
        seqPtr->definition = line;
        line = stream.readLine();
        seqPtr->organism = line;

        if (!isSimple){
            line = stream.readLine();
            seqPtr->source = line;
            line = stream.readLine();
            seqPtr->features = line;
            line = stream.readLine();
            seqPtr->authors = line;
            line = stream.readLine();
            seqPtr->title = line;
        }

        line = stream.readLine();
        seqPtr->seqName = line;

        for(int j = 0; j < qList.size(); j++){
            stream >> id;
            seqPtr->identity.push_back(id);
        }

        stream >> seqPtr->length;
        line = stream.readLine();

        seqPtr->seqInt = new short[static_cast<unsigned long>(seqPtr->length+1)];
        seqPtr->seqInt[0] = -1;
        for(int j =1; j <= seqPtr->length; j++){
            bool OK;
            int base = stream.read(1).toInt(&OK);
            if(!OK){
                QMessageBox::about(Q_NULLPTR, "File Error", "Error loading sequence list file! \n Please report the problem to the developer");
                cleanMemorySeqs();
                return;
            }
            seqPtr->seqInt[j] = static_cast<short>(base);
        }
        line = stream.readLine();
        seqPtr->marked = 0;

        loadList.push_back(seqPtr);
        _OnInsertData(seqPtr);

        if (pDlg.wasCanceled()){
            return;
        }
    }

    file.close();

    QMessageBox::about(Q_NULLPTR, "File loaded", QString("%1 sequences were loaded to sequence data: %2").arg(loadList.size()).arg(filename));
}


/*
void DataTools::saveSeqList(QString filename){
    QFile file(filename);
    if (!file.open(QFile::WriteOnly))
    {
        QMessageBox::about(0, "File Error", "Error saving sequence list file! \n Please report the problem to the developer");
        return;
    }

    QByteArray origData;
    QTextStream in(&origData);
    in << "Sequence list file version 1.0, please don't modify.\n"
            <<"Pairwise alignment settings:\n"
            <<mismatchPenalty << " " << gapOpenPenalty << " " << gapExtnPenalty << " " << policy <<endl
            <<qList.size() << endl;

    for(int i = 0; i < qList.size(); i++){
        in << qList[i]->definition << endl
                << qList[i]->length << endl;
        for(int j = 1; j <= qList[i]->length; j++){
            in << qList[i]->seqInt[j];
        }
        in <<endl;
    }
    in << sList.size() << endl << isSimple << endl;

    int range = sList.size();
    MyProgressDlg *PDlg = new MyProgressDlg("Saving Sequence List...",0, range);
    struct SequenceList* seqPtr;
    for(int i = 0; i < range; i++){
        PDlg->setValue(i);
        PDlg->setLabelText("Saving Sequence List...\n\n"+
                           tr("%1 were finished out of %2\n").arg(i).arg(range));
        seqPtr = sList[i];

        in << seqPtr->gi << endl
                << seqPtr->accession << endl
                << seqPtr->definition << endl
                << seqPtr->organism << endl;
        if(!isSimple){
            in << seqPtr->source << endl
                    << seqPtr->features << endl
                    << seqPtr->authors << endl
                    << seqPtr->title << endl;
        }
        in << seqPtr->seqName << endl;
        for(int j = 0; j < qList.size() ; j++){
            in << seqPtr->identity.at(j)<< " ";
        }
        in<< endl
                << seqPtr->length << endl;
        for(int j = 1; j <= seqPtr->length; j++){
            in << seqPtr->seqInt[j];
        }
        in << endl;
    }
    QByteArray compressedData = qCompress(origData);
    file.write(compressedData);

    PDlg->close();
    delete PDlg;
    file.close();

    QMessageBox::about(0, "File saved", QString("%1 sequences were saved to sequence file: %2").arg(sList.size()).arg(filename));
}
*/
void DataTools::saveSeqList(QString filename, QVector<struct SequenceList*>saveList){
    QFile file(filename);
    if (!file.open(QFile::WriteOnly))
    {
        QMessageBox::about(Q_NULLPTR, "File Error", "Error saving sequence list file! \n Please report the problem to the developer");
        return;
    }

    QByteArray origData;
    QTextStream in(&origData);
    in << "Sequence list file version 1.0, please don't modify.\n"
            <<"Pairwise alignment settings:\n"
            <<mismatchPenalty << " " << gapOpenPenalty << " " << gapExtnPenalty << " " << policy <<endl
            <<qList.size() << endl;

    for(int i = 0; i < qList.size(); i++){
        in << qList[i]->definition << endl
                << qList[i]->length << endl;
        for(int j = 1; j <= qList[i]->length; j++){
            in << qList[i]->seqInt[j];
        }
        in <<endl;
    }
    in << saveList.size() << endl << isSimple << endl;

    int range = saveList.size();
    MyProgressDlg pDlg("Saving Sequence List", 0, range);
    pDlg.setWindowModality(Qt::ApplicationModal);

    struct SequenceList* seqPtr;
    for(int i = 0; i < range; i++){
        QApplication::processEvents();
        pDlg.setValue(i);
        pDlg.setLabelText("Saving Sequence List\n\n"+
                           tr("%1 were finished out of %2\n").arg(i).arg(range));
        seqPtr = saveList[i];

        in      << seqPtr->accession << endl
                << seqPtr->definition << endl
                << seqPtr->organism << endl;
        if(!isSimple){
            in << seqPtr->source << endl
                    << seqPtr->features << endl
                    << seqPtr->authors << endl
                    << seqPtr->title << endl;
        }
        in << seqPtr->seqName << endl;
        for(int j = 0; j < qList.size() ; j++){
            in << seqPtr->identity.at(j)<< " ";
        }
        in<< endl
                << seqPtr->length << endl;
        for(int j = 1; j <= seqPtr->length; j++){
            in << seqPtr->seqInt[j];
        }
        in << endl;
    }
    QByteArray compressedData = qCompress(origData);
    file.write(compressedData);
    file.close();
    QMessageBox::about(Q_NULLPTR, "File saved", QString("%1 sequences were saved to sequence file: %2").arg(saveList.size()).arg(filename));
}

void DataTools::saveSeqList2TXT(QString filename, QVector<struct SequenceList*>saveList, QStringList headers)
{
    QFile file(filename);
    if (!file.open(QFile::WriteOnly))
    {
        QMessageBox::about(Q_NULLPTR, "File Error", "Error saving sequence list file! \n Please report the problem to the developer");
        return;
    }

    QTextStream out(&file);
    // output headers
    out << "No.";
    for( int i = 1; i < headers.size(); i++ ){
        out << "\t" << headers[i];
    }
    // output actual data-data is being split and tab demilited attributes are being generated
    struct SequenceList* seqPtr;
    for( int i = 0; i < saveList.size(); i++ ){
        seqPtr = saveList[i];
        out << endl << i+1 << "\t" << seqPtr->accession;
        for( int j = 0; j < qList.size(); j++ ){
            if( seqPtr->identity[j] > TINY_IDENTITY_VALUE )
                out << "\t" << seqPtr->identity[j] << "%";
            else
                out << "\tNA";
        }
        out << "\t" << seqPtr->length << "\t" << seqPtr->definition << "\t" << seqPtr->organism;
        if( !isSimple )
            out << "\t" << seqPtr->source << "\t" << seqPtr->features
                    << "\t" << seqPtr->authors << "\t" << seqPtr->title;
    }

    file.close();
    QMessageBox::about(Q_NULLPTR, "File saved", QString("%1 sequences were saved to .txt file: %2").arg(saveList.size()).arg(filename));
}

void DataTools::saveFASTASeqListAs(QString filename, QString FASTAContent)
{
    QFile file(filename);
    if (!file.open(QFile::WriteOnly))
    {
        QMessageBox::about(Q_NULLPTR, "File Error", "Error saving sequence list file! \n Please report the problem to the developer");
        return;
    }

    QTextStream out(&file);

    out << FASTAContent;
    file.close();
    QMessageBox::about(Q_NULLPTR, "Data saved", "All sequences were saved to FASTA file: "+filename);
}

void DataTools::appendFASTASeqListTo(QString filename, QString FASTAContent)
{
    QFile file(filename);
    if (!file.open(QFile::Append))
    {
        QMessageBox::about(Q_NULLPTR, "File Error", "Error saving sequence list file! \n Please report the problem to the developer");
        return;
    }

    QTextStream out(&file);

    int len, nLineCount = FASTAContent.count("\n");

    QString strLine;
    // Dump every line of text of the richedit control.
    for( int i = 0; i < nLineCount; i++ ){
        // length of line i:
        int idx = FASTAContent.indexOf("\n");
        len = idx;
        if( len <= 0 )
                continue;
        strLine = FASTAContent.left(len);
        FASTAContent = FASTAContent.remove(0,len+1);

        if( strLine[0] != '>' )
                strLine = strLine.toUpper();
        out << strLine << endl;
    }
    file.close();
    QMessageBox::about(Q_NULLPTR, "Data appended", QString("%1 sequences were appended to FASTA file: ").arg(nLineCount/2)+filename);
}

void DataTools::copySequenceList(struct SequenceList* dest, struct SequenceList* source)
{
        //dest->gi			= source->gi;
        dest->accession		= source->accession;
        dest->length		= source->length;
        dest->definition	= source->definition;
        dest->organism		= source->organism;
        dest->source		= source->source;
        dest->features		= source->features;
        dest->authors		= source->authors;
        dest->title			= source->title;

        for( int i = 0; i < source->identity.size(); i++ )
                dest->identity.push_back(source->identity[i]);

        //copy actual sequence
        dest->seqInt = new short[static_cast<unsigned long>(source->length+1)];
        for( int i = 0; i <= source->length; i++ )
                dest->seqInt[i] = source->seqInt[i];
}

bool  DataTools::readSeqFile(QString filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::about(Q_NULLPTR, "File Error", "Sequence file doesn't exist or File corrupted");
        return false;
    }

    QTextStream stream ( &file );
    QString seqInfo;
    seqInfo = stream.readAll();
    QStringList seqInfoList;

    int pos1 = seqInfo.indexOf("LOCUS");
    if(pos1 < 0)
        return false;
    int pos2 = seqInfo.indexOf("LOCUS",pos1+1);
    while(1){
        if(pos2 < 0) {
            QString Info = seqInfo.mid(pos1, seqInfo.length()-pos1);
            seqInfoList.push_back(Info);
            break;
        }
        QString Info = seqInfo.mid(pos1, pos2-pos1);
        seqInfoList.push_back(Info);
        pos1 = pos2;
        pos2 = seqInfo.indexOf("LOCUS",pos1+1);
    }

    int range = seqInfoList.size();
    MyProgressDlg pDlg("Reading sequence file",0, range);
    pDlg.setWindowModality(Qt::ApplicationModal);

    struct SequenceList* seqPtr;
    for(int i = 0; i < range; i++){
        seqPtr = new SequenceList;
        pDlg.setValue(i);
        QHash<QString, QString> hashInfo;
        hashInfo = parseSeqInfo(seqInfoList[i]);
        /*if(!getGI(hashInfo, seqPtr->gi)){
            delete pDlg;
            QMessageBox::about(Q_NULLPTR, "getGI Info Error", "Please check the sequence (.gb) file");
            return false;
        }*/
        QHash<QString, QString>::const_iterator it = hashInfo.find("ACCESSION");
        if(it != hashInfo.end()){
            seqPtr->accession = it.value();
        }
        if(assignSeq(seqPtr, hashInfo) == -2 ){
            QMessageBox::about(Q_NULLPTR, "Create Sequence Error", "Please check the sequence (.gb) file");
            return false;
        }
        // sequence % identity
        for( int i = 0; i < qList.size(); i++ )
                seqPtr->identity.push_back(TINY_IDENTITY_VALUE);
        compList.push_back(seqPtr);
    }
    return true;
}

void  DataTools::compareSeq()
{
    // clean old memory
    for( int i = 0; i < notInSeqList.size(); i++ ){
            delete[] notInSeqList[i]->seqInt;
            delete notInSeqList[i];
    }
    notInSeqList.clear();
    for( int i = 0; i < notInFileList.size(); i++ ){
            delete[] notInFileList[i]->seqInt;
            delete notInFileList[i];
    }
    notInFileList.clear();

    //(Daniel) Since we don't have GI numbers, this code is useless....
/*    int range = sList.size() + compList.size();
    MyProgressDlg *pDlg = new MyProgressDlg("Comparing",0, range);
    pDlg->setWindowModality(Qt::ApplicationModal);

    int j;
    struct SequenceList* seqPtr;
    // process sequence list
    for( int i = 0; i < sList.size(); i++ ){
        pDlg->setValue(i);
        for( j = 0; j < compList.size(); j++ ){
                if( sList[i]->gi == compList[j]->gi )
                        break;
        }
       if(j >= compList.size() ){ // didn't find this sequence in sequence file
            //create a new sequence
            seqPtr = new SequenceList();
            copySequenceList( seqPtr, sList[i] );
            notInFileList.push_back( seqPtr );
        }
    }

    // process sequences in sequence file
    for( int i = 0; i < compList.size(); i++ ){
        pDlg->setValue(sList.size()+i);
        for( j = 0; j < sList.size(); j++ ){
                if( compList[i]->gi == sList[j]->gi )
                        break;
        }
        if(j >= sList.size() ){ // didn't find this sequence in sequence table
            //create a new sequence
            seqPtr = new SequenceList();
            copySequenceList( seqPtr, compList[i] );
            notInSeqList.push_back( seqPtr );
        }
    }
    delete pDlg;*/
}

void DataTools::cleanMemorySeqs()
{
    delete queryNames;
    delete querySeqs;
    queryNames = new QStringList();
    querySeqs = new QStringList();

    for( int i = 0; i < sList.size(); i++ ){
            delete[] sList.at(i)->seqInt;
            delete sList.at(i);
    }
    sList.clear();
    for( int i = 0; i < missingList.size(); i++ ){
        delete missingList[i];
    }
    missingList.clear();

    for( int i = 0; i < notDownloadList.size(); i++ ){
        delete notDownloadList[i];
    }
    notDownloadList.clear();

    for( int i = 0; i < qList.size(); i++ ){
            delete[] qList.at(i)->seqInt;
            delete qList.at(i);
    }
    qList.clear();
    for( int i = 0; i < compList.size(); i++ ){
            delete[] compList[i]->seqInt;
            delete compList[i];
    }
    compList.clear();
    for( int i = 0; i < notInSeqList.size(); i++ ){
            delete[] notInSeqList[i]->seqInt;
            delete notInSeqList[i];
    }
    notInSeqList.clear();
    for( int i = 0; i < notInFileList.size(); i++ ){
            delete[] notInFileList[i]->seqInt;
            delete notInFileList[i];
    }
    notInFileList.clear();
}

void DataTools::writeErrorLog(QString str){
    QString logFilePath = "";
    #ifdef MAC
        QString appPath = QDir::currentPath();
        //logFilePath += "file:/";
        logFilePath += appPath;
        logFilePath += "/";
    #endif

    #ifdef WIN
        logFilePath += "Log\\";
    #else
        logFilePath += "Log/";
    #endif

    QDir dir(logFilePath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }


    QDate date = QDate::currentDate();
    QString dateString = date.toString();

    QFile file(logFilePath+dateString+"_Log.txt");

    if(!file.exists()){
        QDir dir(logFilePath);
        QStringList entities = dir.entryList(QDir::Files, QDir::Time);

        for(int i = numLogFiles-1; i < entities.size() ; i++){
            //QMessageBox::about(0,"",entities[i]);
            dir.remove(entities[i]);
        }
    }



    file.open(QIODevice::Append| QIODevice::Text);
    QTextStream out(&file);
    out << str;
    file.close();
}

void DataTools::cleanMemoryAllData()
{

    int total = lPrimerCandid.size() + rPrimerCandid.size() + primerPairList.size()
              + tarSeqs.size() + nontarSeqs.size() + probeCandid.size() + probeList.size();


    int finish = 1;
    int range = total;

    MyProgressDlg pDlg("Clean memory", 0, range);
    pDlg.setWindowModality(Qt::ApplicationModal);

    // 1. sequences
    tarSeqNames.clear();
    nontarSeqNames.clear();
    for(int i = 0; i < tarSeqs.size(); i++ )
        delete[] tarSeqs[i];
    pDlg.setValue(finish++);
    for(int i = 0; i < nontarSeqs.size(); i++ )
        delete[] nontarSeqs[i];
    pDlg.setValue(finish++);
    tarSeqs.clear();
    nontarSeqs.clear();

    // 2. Others
    DataTools::cleanMemoryPrimerCands(&pDlg);
    DataTools::cleanMemoryPrimer(&pDlg);
    DataTools::cleanMemoryProbeCands(&pDlg);
    DataTools::cleanMemoryProbe(&pDlg);
    //DataTools::cleanMemoryScoringTable();
}

/*void DataTools::cleanMemoryPrimerReport()
{
        int total = (int) (lPrimerCandid.size() + rPrimerCandid.size() +
                                primerPairList.size() + tarSeqs.size() + nontarSeqs.size() );
        total += (int) (probeCandid.size() + probeList.size());// + tripleList.size());


        int finish = 1;
        int range = total;

        MyProgressDlg *pDlg = new MyProgressDlg("Clean memory...",0, range);
        pDlg->setWindowModality(Qt::ApplicationModal);

        // clean memory

        // 1. Triple list
        int numTriple = (int)tripleList.size();
        for( int i = 0; i < numTriple; i++ ){
                delete tripleList[i];
                pDlg->setValue(finish++);
        }
        //tripleList.clear();
        //tripleShowList.clear();

        // 2. probes
        int numProbe = (int)probeList.size();
        for( int i = 0; i < numProbe; i++ ){
                delete probeList[i];
                pDlg->setValue(finish++);
        }
        probeList.clear();

        // 3. probe candidates
        for( unsigned i = 0; i < probeCandid.size(); i++ ){
                delete[] probeCandid[i];
                pDlg->setValue(finish++);
        }
        probeCandid.clear();

        // 4. primer pair list
        int numPair = (int)primerPairList.size();
        for( int i = 0; i < numPair; i++ ){
                delete primerPairList[i];
                pDlg->setValue(finish++);
        }
        primerPairList.clear();
        primerPairShowList.clear();

        // 5. target/non-target sequences
        tarSeqNames.clear();
        nontarSeqNames.clear();
        for( unsigned i = 0; i < tarSeqs.size(); i++ ){
                delete[] tarSeqs[i];
                pDlg->setValue(finish++);
        }
        for( unsigned i = 0; i < nontarSeqs.size(); i++ ){
                delete[] nontarSeqs[i];
                pDlg->setValue(finish++);
        }
        tarSeqs.clear();
        nontarSeqs.clear();

        // 6. primer candidates
        for( unsigned i = 0; i < lPrimerCandid.size(); i++ ){
                delete[] lPrimerCandid[i];
                pDlg->setValue(finish++);
        }
        for( unsigned i = 0; i < rPrimerCandid.size(); i++ ){
                delete[] rPrimerCandid[i];
                pDlg->setValue(finish++);
        }
        lPrimerCandid.clear();
        rPrimerCandid.clear();

        delete pDlg;

        //OnDisplayAllColumn();
       //_OnDisplayAllColumn();

        // for sort order of each column && save or not for each column in save excel dialog
        for( int i = 0; i < A_NUM_COLUMNS; i++ )
            primerSortOrder[i] = 0;

        dispPrimerSel = -1;
        addPrimerMauallyReady = false;
        // for display primer list conditionally, there are 4 conditions
        for( int i = 0; i < 4; i++ ){
            dispPrimerCondTitle[i] = 1;
            dispPrimerCondGtLt[i] = -1;
            dispPrimerCondValue[i] = 0.0;
            dispPrimerCondAllowCancel = FALSE;
        }


        // for sort order of each column && save or not for each column in save excel dialog
        for( int i = 0; i < A_NUM_COLUMNS; i++ )
            tripleSortOrder[i] = 0;

        dispTripleSel = -1;
        addTripleMauallyReady = false;
        // for display triple list conditionally, there are 4 conditions
        for( int i = 0; i < 4; i++ ){
            dispTripleCondTitle[i] = 1;
            dispTripleCondGtLt[i] = -1;
            dispTripleCondValue[i] = 0.0;
            dispTripleCondAllowCancel = FALSE;
        }

}*/

/*void DataTools::cleanMemoryTripleReport()
{
    int total = (int) (probeCandid.size() + probeList.size());// + tripleList.size());

    int finish = 1;
    int range = total;

    MyProgressDlg *pDlg = new MyProgressDlg("Clean memory...",0, range);
    pDlg->setWindowModality(Qt::ApplicationModal);

    // clean memory

    // 1. Triple list
    int numTriple = (int)tripleList.size();
    for( int i = 0; i < numTriple; i++ ){
            delete tripleList[i];
            pDlg->setValue(finish++);
    }
    //tripleList.clear();
    //tripleShowList.clear();

    int numProbe = (int)probeList.size();
    for( int i = 0; i < numProbe; i++ ){
            delete probeList[i];
            pDlg->setValue(finish++);
    }
    probeList.clear();

    // 2. probe candidates
    for( unsigned i = 0; i < probeCandid.size(); i++ ){
            delete[] probeCandid[i];
            pDlg->setValue(finish++);
    }
    probeCandid.clear();

    delete pDlg;

    //_OnDisplayAllColumn();
    //_OnDisplayAllColumn();

    // for sort order of each column && save or not for each column in save excel dialog
    for( int i = 0; i < A_NUM_COLUMNS; i++ )
        tripleSortOrder[i] = 0;

    dispTripleSel = -1;
    addTripleMauallyReady = false;
    for( int i = 0; i < 4; i++ ){
        dispTripleCondTitle[i] = 1;
        dispTripleCondGtLt[i] = -1;
        dispTripleCondValue[i] = 0.0;
        dispTripleCondAllowCancel = FALSE;
    }
}*/

/*void DataTools::cleanMemoryAllData(void)
{
    // clean memory
    for( unsigned int i = 0; i < lPrimerCandid.size(); i++ )
            delete[] lPrimerCandid[i];
    lPrimerCandid.clear();
    for( unsigned int i = 0; i < rPrimerCandid.size(); i++ )
            delete[] rPrimerCandid[i];
    rPrimerCandid.clear();
    lPrimerBindPosAvg.clear();
    rPrimerBindPosAvg.clear();
}*/

void DataTools::cleanMemoryPrimer(MyProgressDlg*) //clean primer pairs, and clean probes as well
{
    /*int numPair = (int)primerPairList.size();
    int currBar = pDlg->value()+1;
    for( int i = 0; i < numPair; i++ ){
        PrimerPair *tmp = primerPairList[i];
        for(int j = 0; j < tmp->tripleList.size(); j++){
            Triple* tp = tmp->tripleList[j];
            delete tp;
        }
        tmp->tripleList.clear();
        for(int j = 0;  j < 2 ; j++){
            delete[] tmp->posBindingTar[j];
            delete[] tmp->posBindingNontar[j];
        }
        delete[] tmp->posBindingTar;
        delete[] tmp->posBindingNontar;
        delete tmp;
        pDlg->setValue(currBar++);
    }*/
    primerPairList.clear();
}


void DataTools::cleanMemoryPrimerCands()
{
    // clean memory
    for(int i = 0; i < lPrimerCandid.size(); i++ ){
        delete[] lPrimerCandid[i];
    }
    lPrimerCandid.clear();
    for(int i = 0; i < rPrimerCandid.size(); i++ ){
        delete[] rPrimerCandid[i];
    }
    rPrimerCandid.clear();
    lPrimerBindPosAvg.clear();
    rPrimerBindPosAvg.clear();
}

void DataTools::cleanMemoryPrimerCands(MyProgressDlg* pDlg)
{
    // clean memory
    int currBar = pDlg->value()+1;
    for(int i = 0; i < lPrimerCandid.size(); i++ ){
        delete[] lPrimerCandid[i];
        pDlg->setValue(currBar++);
    }
    lPrimerCandid.clear();
    for(int i = 0; i < rPrimerCandid.size(); i++ ){
        delete[] rPrimerCandid[i];
        pDlg->setValue(currBar++);
    }
    rPrimerCandid.clear();
    lPrimerBindPosAvg.clear();
    rPrimerBindPosAvg.clear();
}

void DataTools::cleanMemoryProbeData(void)
{
    //ataTools::cleanMemoryTriple();
    DataTools::cleanMemoryProbeCands();
    DataTools::cleanMemoryProbe();
}

void DataTools::cleanMemoryProbe()
{
    /*int numProbe = (int)probeList.size();
    for( int i = 0; i < numProbe; i++ ){
        Probe *tmp = probeList[i];
        for(int j = 0;  j < 2 ; j++){
            delete[] tmp->posBindingTar[j];
            delete[] tmp->posBindingNontar[j];
        }
        delete[] tmp->posBindingTar;
        delete[] tmp->posBindingNontar;
        delete tmp;
    }*/
    probeList.clear();
}
void DataTools::cleanMemoryProbe(MyProgressDlg* pDlg)
{
    int currBar = pDlg->value()+1;
    int numProbe = probeList.size();
    for( int i = 0; i < numProbe; i++ ){
        Probe *tmp = probeList[i];
        for(int j = 0;  j < 2 ; j++){
            delete[] tmp->posBindingTar[j];
            delete[] tmp->posBindingNontar[j];
        }
        delete[] tmp->posBindingTar;
        delete[] tmp->posBindingNontar;
        delete tmp;
        pDlg->setValue(currBar++);
    }
    probeList.clear();
}

void DataTools::cleanMemoryProbeCands()
{
    for(int i = 0; i < probeCandid.size(); i++ ){
        delete[] probeCandid[i];
    }
    probeCandid.clear();
    probeBindPosAvg.clear();
}


void DataTools::cleanMemoryProbeCands(MyProgressDlg* pDlg)
{
    int currBar = pDlg->value()+1;
    for(int i = 0; i < probeCandid.size(); i++ ){
        delete[] probeCandid[i];
        pDlg->setValue(currBar++);
    }
    probeCandid.clear();
    probeBindPosAvg.clear();
}

void DataTools::cleanMemoryTriple()
{
    int numPair = primerPairList.size();
    for( int i = 0; i < numPair; i++ ){
        PrimerPair *tmp = primerPairList[i];
        cleanMemoryTriple(tmp);
    }
}

void DataTools::cleanMemoryTriple(PrimerPair* pr)
{
    for(int j = 0; j < pr->tripleList.size(); j++){
        Triple* tp = pr->tripleList[j];
        delete tp;
    }
    pr->tripleList.clear();
}

/*
int DataTools::allocMemScoringTable()
{
    allocScTab = true;
    // allocate scoring table and pointer table for primer search
    try {
        T = new float** [MAX_PRIMER_LEN];
        P = new int** [MAX_PRIMER_LEN];
    }
    catch ( std::bad_alloc& e ) {
        QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for scoring tables.\n Program is forced to terminate");
        return FATAL_ERROR;
    }
    for( int i = 0; i < MAX_PRIMER_LEN; i++ ){
        try {
            T[i] = new float*[MAX_SEQUENCE_LEN];
            P[i] = new int*[MAX_SEQUENCE_LEN];
        }
        catch ( std::bad_alloc& e ) {
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for scoring tables.\n Program is forced to terminate");
            return FATAL_ERROR;
        }

        for( int j = 0; j < MAX_SEQUENCE_LEN; j++ ){
            T[i][j] = new float[4];
            P[i][j] = new int[4];
        }
    }
    return 0;
}*/


/*void DataTools::cleanMemoryScoringTable(void)
{
    if(allocScTab){
        for( int i = 0; i < MAX_PRIMER_LEN; i++ ){
            for( int j = 0; j < MAX_SEQUENCE_LEN; j++ ){
                delete[] T[i][j];
                delete[] P[i][j];
            }
            delete[] T[i];
            delete[] P[i];
            //dumpDlg->setValue(i+1);
        }
        delete[] T;
        delete[] P;
    }
    allocScTab = false;
}*/



/*void DataTools::releaseMemScoringTable(CDialog *pDlg)
{
        CPrimerDumpMemStatusDlg *dumpDlg = new CPrimerDumpMemStatusDlg();
        pDlg->EnableWindow(FALSE);
        dumpDlg->Create(IDD_PRIMER_DUMPMEM_STATUS, pDlg);
        dumpDlg->m_progressBar.SetRange(1, MAX_PRIMER_LEN);

        // clean scoring table and pointer table
        for( int i = 0; i < MAX_PRIMER_LEN; i++ ){
                for( int j = 0; j < MAX_SEQUENCE_LEN; j++ ){
                        delete[] T[i][j];
                        delete[] P[i][j];
                }
                delete[] T[i];
                delete[] P[i];

                dumpDlg->m_progressBar.SetPos(i+1);
                dumpDlg->PeekStatus();
        }
        delete[] T;
        delete[] P;
        dumpDlg->m_progressBar.SetPos(MAX_PRIMER_LEN+1);
        dumpDlg->PeekStatus();

        dumpDlg->DestroyWindow();
        pDlg->EnableWindow(TRUE);
        pDlg->SetFocus();
}*/

float DataTools::getCostForAmbigBase(int base1, int base2, bool binaryCost)
{
    int numBases1 = 0;
    int numBases2 = 0;
    while( baseMatrix[base1][numBases1] >= 0 )	numBases1++;
    while( baseMatrix[base2][numBases2] >= 0 )	numBases2++;

    if(binaryCost == false) {
        float avg = 0;
        for(int i = 0; i < numBases1; i++ ){
            for( int j = 0; j < numBases2; j++ )
                avg += simCost[baseMatrix[base1][i]][baseMatrix[base2][j]];
        }
        avg = avg / (numBases1 * numBases2);
        return avg;
    } else {
        for(int i = 0; i < numBases1; i++ ){
            for( int j = 0; j < numBases2; j++ ){
                 if (baseMatrix[base1][i] == baseMatrix[base2][j])
                    return 0;
            }
        }
        return 1.0f;
    }
}

void DataTools::buildSimCost( bool binaryCost)
{
    for( int i = 0; i < 15; i++ ){
        for( int j = 0; j < 15; j++)
            simCost[i][j] = 1;
    }
    for( int i = 0; i < 15; i++ )
        simCost[i][i] = 0;
    for(int i = 0; i < 15; i++ ){
        for(int j = 0; j < i; j++ ){
            simCost[i][j] = getCostForAmbigBase(i, j, binaryCost);
            simCost[j][i] = simCost[i][j];
        }
    }
}

/*
void DataTools::defaultSeqProperty()
{

}*/

void DataTools::defaultPrimer()
{
    // prepare default settings
    primerCostType  = false;
    primerLenMin	= 14;
    primerLenMax	= 22;
    primerCandidFrequency   = 100.0f;
    primerCandidTopPctg 	= 100.0f;
    primerPcrSizeMin		= 50;
    primerPcrSizeMax		= 200;
    primerAvoidACT          = true;
    primerAvoidG            = true;
    primerAvoidCGat3end     = true;
    primerNumACT            = 4;
    primerNumG              = 4;
    primerNumCGat3end       = 3;
    //primerEstimate            = 1;
    primerTmMin		= 48;
    primerTmMax     = 58;
    primerNaConsent	= 0.1f;
    primerTmDiff    = 2;
    primerGCMin		= 25;
    primerGCMax		= 75;
    primerMaxPrPrComp	= 10;
    primerMax3PrPrComp	= 4;
    primerInterBase3end	= 8;
    primerMaxSelfComp	= 10;
    primerMax3SelfComp	= 4;
    primerIntraBase3end	= 8;

    primerCostType = false;

    primerBindBitmap_tar1to3 = 0;		// values 0-6: primer-sequence annealing pictures for base 0-3 on 3' end: indicate 100% to 0% chance
    primerBindBitmap_nontar1to3 = 6;	// same for non-target
    primerBindBitmap_tar4 = 0;		// values 0-2: high, medium, low. primer-sequence annealing for base 4 to 5' end.
    primerBindBitmap_nontar4 = 2;

    setPrimerMismatchAllowance(primerBindBitmap_tar1to3,
                             primerBindBitmap_tar4,
                             primerBindBitmap_nontar1to3,
                             primerBindBitmap_nontar4);

    for( int i = 30; i < MAX_PRIMER_LEN; i++ ){
        primerPosWtTar[i] = primerPosWtTar[29];
        primerPosWtNonTar[i] = primerPosWtNonTar[29];
    }

    buildSimCost(primerCostType);
    primerShowAdvance = false;

    maxIndel		= 0;
    indelCost		= 5;
}

void DataTools::defaultProbe()
{
    probeLenMin = 14;
    probeLenMax = 30;
    probeCandidFrequency = 100.0f;
    probeCandidTopPctg = 100.0f;
    probeAvoidACT = true;
    probeAvoidG = true;
    probeAvoidGat5end = true;
    probeNumACT = 4;
    probeNumG = 4;
    //probeNumGat5end = 3;
    //probeEstimate = true;	// true: estimate PCR Info. false: exact PCR Info.
    probeTmMin = 58;
    probeTmMax = 72;
    probeTmGreaterMin = 6;
    probeTmGreaterMax = 10;
    probeNaConsent = 0.1f;
    probeGCMin = 25;
    probeGCMax = 75;
    maxPbPrComp = 10;
    //max3PbPrComp = 4;
    //tripleInterBase3end	= 8;
    probeMaxSelfComp = 10;
    //probeMax3SelfComp	= 4;
    //probeIntraBase3end	= 8;

    probeCostType = false;


    probeBindContMatches_tar = 15;		// values 0-15: probe-sequence annealing
    probeBindContMatches_nontar = 0;

    setProbeMismatchAllowance(probeBindContMatches_tar,
                             probeBindContMatches_nontar);

    for( int i = 15; i < MAX_PROBE_LEN/2+1; i++ ){
        probePosWtTar[i] = probePosWtTar[14];
        probePosWtNonTar[i] = probePosWtNonTar[14];
    }

    buildSimCost(primerCostType);
    probeShowAdvance = false;

    maxIndel		= 0;
    indelCost		= 5;
}

void DataTools::Initialize()
{
        // initial global variables
        baseMatrix[BASE_A][0] = BASE_A;
        baseMatrix[BASE_A][1] = -1;
        baseMatrix[BASE_C][0] = BASE_C;
        baseMatrix[BASE_C][1] = -1;
        baseMatrix[BASE_G][0] = BASE_G;
        baseMatrix[BASE_G][1] = -1;
        baseMatrix[BASE_T][0] = BASE_T;
        baseMatrix[BASE_T][1] = -1;
        baseMatrix[BASE_B][0] = BASE_C;
        baseMatrix[BASE_B][1] = BASE_G;
        baseMatrix[BASE_B][2] = BASE_T;
        baseMatrix[BASE_B][3] = -1;
        baseMatrix[BASE_D][0] = BASE_A;
        baseMatrix[BASE_D][1] = BASE_G;
        baseMatrix[BASE_D][2] = BASE_T;
        baseMatrix[BASE_D][3] = -1;
        baseMatrix[BASE_H][0] = BASE_A;
        baseMatrix[BASE_H][1] = BASE_C;
        baseMatrix[BASE_H][2] = BASE_T;
        baseMatrix[BASE_H][3] = -1;
        baseMatrix[BASE_K][0] = BASE_G;
        baseMatrix[BASE_K][1] = BASE_T;
        baseMatrix[BASE_K][2] = -1;
        baseMatrix[BASE_M][0] = BASE_A;
        baseMatrix[BASE_M][1] = BASE_C;
        baseMatrix[BASE_M][2] = -1;
        baseMatrix[BASE_N][0] = BASE_A;
        baseMatrix[BASE_N][1] = BASE_C;
        baseMatrix[BASE_N][2] = BASE_G;
        baseMatrix[BASE_N][3] = BASE_T;
        baseMatrix[BASE_N][4] = -1;
        baseMatrix[BASE_R][0] = BASE_A;
        baseMatrix[BASE_R][1] = BASE_G;
        baseMatrix[BASE_R][2] = -1;
        baseMatrix[BASE_S][0] = BASE_G;
        baseMatrix[BASE_S][1] = BASE_C;
        baseMatrix[BASE_S][2] = -1;
        baseMatrix[BASE_V][0] = BASE_A;
        baseMatrix[BASE_V][1] = BASE_C;
        baseMatrix[BASE_V][2] = BASE_G;
        baseMatrix[BASE_V][3] = -1;
        baseMatrix[BASE_W][0] = BASE_A;
        baseMatrix[BASE_W][1] = BASE_T;
        baseMatrix[BASE_W][2] = -1;
        baseMatrix[BASE_Y][0] = BASE_C;
        baseMatrix[BASE_Y][1] = BASE_T;
        baseMatrix[BASE_Y][2] = -1;

        markConditionalCheck[0] = 0;	markConditionalCheck[1] = 0;
        markConditionalCheck[2] = 0;	markConditionalCheck[3] = 0;
        markIdMin = 0;	markIdMax = 0;	markLenMin = 0;	markLenMax = 0;
        markDefConstraint = "";	markOrgConstraint = "";
        markContain[0] = 1;	markContain[1] = 1;
        queryId = 0;

        // for target and non-target sequence design: find next sequence dialog
        for( int i = 0; i < 6; i++ )	findConditionalCheck[i] = 0;
        findIdMin = 0.0f;
        findIdMax = 0.0f;
        findLenMin = 0;
        findLenMax = 0;
        findDefConstraint = "";
        findOrgConstraint = "";
        findAuthorConstraint = "";
        findTitleConstraint = "";
        for( int i = 0; i < 4; i++ )	findContain[i] = 1; // 1 for contain, 0 for not contain


        design		= 1; // initially start designing new primer list
        //primerSettingOption = 0; // initially start now
        //showSimMeasure		= 0; // initially start now
        //showSimMeasureAdv	= 0; // initially start now
        targetSeqFilename = "";
        nontargetSeqFilename = "";
        dupSeqFilename = "";
        lenOutRangeSeqFilename = "";

        rmDupSeqs = false;
        rmLenOutRangeSeqs = true;

        dpDupSeqs = false;
        dpLenOutRangeSeqs = false;
        seqLenMin = 60;
        seqLenMax = 4000;
        usrLoad = false;	// initially not use user's primers
        primerFilename = "";

        restoreDefaultSetting();
}

void DataTools::setPrimerPosWtTarget(int num, ...)
{
    // to store the list of arguments
    va_list args;
    // initialize arguments to store all values
    va_start(args, num);

    for( int i = 0; i < num; i++)
        primerPosWtTar[i] = (float)va_arg(args, int);
    for( int i = 30; i < MAX_PRIMER_LEN; i++ )
        primerPosWtTar[i] = primerPosWtTar[29];

    // clean up the list
    va_end(args);
}

void DataTools::setPrimerPosWtNonTarget(int num, ...)
{
    // to store the list of arguments
    va_list args;
    // initialize arguments to store all values
    va_start(args, num);

    for( int i = 0; i < num; i++)
        primerPosWtNonTar[i] = (float)va_arg(args, int);
    for( int i = 30; i < MAX_PRIMER_LEN; i++ )
        primerPosWtNonTar[i] =primerPosWtNonTar[29];

    // clean up the list
    va_end(args);
}

void DataTools::setProbePosWtTarget(int num, ...)
{
    // to store the list of arguments
    va_list args;
    // initialize arguments to store all values
    va_start(args, num);

    for( int i = 0; i < num; i++)
        probePosWtTar[i] = (float)va_arg(args, int);
    for( int i = num; i < MAX_PROBE_LEN/2+1; i++ )
        probePosWtTar[i] = probePosWtTar[num-1];

    // clean up the list
    va_end(args);
}

void DataTools::setProbePosWtNonTarget(int num, ...)
{
    // to store the list of arguments
    va_list args;
    // initialize arguments to strore all values
    va_start(args, num);

    for( int i = 0; i < num; i++)
        probePosWtNonTar[i] = (float)va_arg(args, int);
    for( int i = num; i < MAX_PROBE_LEN/2+1; i++ )
        probePosWtNonTar[i] =probePosWtNonTar[num-1];

    // clean up the list
    va_end(args);
}

void DataTools::setPrimerMismatchAllowance(int tarPosSel_1to3, int tarPosSel_4, int nonPosSel_1to3, int nonPosSel_4)
{
    //int posSel_1to3 = ui->verticalSlider_tarBase1to3->value();
    //primerBindBitmap_tar1to3 = posSel_1to3;
    //int posSel_4 = ui->verticalSlider_tarBase4->value();
    //primerBindBitmap_tar4 = posSel_4;

    switch( tarPosSel_1to3 ){
    case 0: // 100%
        switch( tarPosSel_4 ){
        case 0: // high
            setPrimerPosWtTarget(30, 0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,3,3,3);
            break;
        case 1: // intermediate
            setPrimerPosWtTarget(30, 0,0,0,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3);
            break;
        default: // low
            setPrimerPosWtTarget(30, 0,0,0,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4);
            break;
        }
        break;
    case 1: // 90%
        switch( tarPosSel_4 ){
        case 0: // high
            setPrimerPosWtTarget(30, 0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,3,3,3);
            break;
        case 1: // intermediate
            setPrimerPosWtTarget(30, 0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3);
            break;
        default: // low
            setPrimerPosWtTarget(30, 0,0,1,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4);
            break;
        }
        break;
    case 2: // 80%
        switch( tarPosSel_4 ){
        case 0: // high
            setPrimerPosWtTarget(30, 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,3,3,3);
            break;
        case 1: // intermediate
            setPrimerPosWtTarget(30, 0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3);
            break;
        default: // low
            setPrimerPosWtTarget(30, 0,1,1,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5);
            break;
        }
        break;
    case 3: // 70%
        switch( tarPosSel_4 ){
        case 0: // high
            setPrimerPosWtTarget(30, 0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3);
            break;
        case 1: // intermediate
            setPrimerPosWtTarget(30, 0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3);
            break;
        default: // low
            setPrimerPosWtTarget(30, 0,1,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5);
            break;
        }
        break;
    case 4: // 20%
        switch( tarPosSel_4 ){
        case 0: // high
            setPrimerPosWtTarget(30, 1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3);
            break;
        case 1: // intermediate
            setPrimerPosWtTarget(30, 1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3);
            break;
        default: // low
            setPrimerPosWtTarget(30, 1,1,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5);
            break;
        }
        break;
    case 5: // 10%
        switch( tarPosSel_4 ){
        case 0: // high
            setPrimerPosWtTarget(30, 1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3);
            break;
        case 1: // intermediate
            setPrimerPosWtTarget(30, 1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3);
            break;
        default: // low
            setPrimerPosWtTarget(30, 1,2,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5);
            break;
        }
        break;
    default: // 0%
        switch( tarPosSel_4 ){
        case 0: // high
            setPrimerPosWtTarget(30, 1,2,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5);
            break;
        case 1: // intermediate
            setPrimerPosWtTarget(30, 1,2,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5);
            break;
        default: // low
            setPrimerPosWtTarget(30, 1,2,3,3,3,3,3,4,4,4,4,4,4,4,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6);
            break;
        }
        break;
    }

    /*posSel_1to3 = ui->verticalSlider_nontarBase1to3->value();
    primerBindBitmap_nontar1to3 = posSel_1to3;
    posSel_4 = ui->verticalSlider_nontarBase4->value();
    primerBindBitmap_nontar4 = posSel_4;*/

    switch( nonPosSel_1to3 ){
    case 0: // 100%
        switch( nonPosSel_4 ){
        case 0: // high
            setPrimerPosWtNonTarget(30, 0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,3,3,3);
            break;
        case 1: // intermediate
            setPrimerPosWtNonTarget(30, 0,0,0,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3);
            break;
        default: // low
            setPrimerPosWtNonTarget(30, 0,0,0,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4);
            break;
        }
        break;
    case 1: // 90%
        switch( nonPosSel_4 ){
        case 0: // high
            setPrimerPosWtNonTarget(30, 0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,3,3,3);
            break;
        case 1: // intermediate
            setPrimerPosWtNonTarget(30, 0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3);
            break;
        default: // low
            setPrimerPosWtNonTarget(30, 0,0,1,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4);
            break;
        }
        break;
    case 2: // 80%
        switch( nonPosSel_4 ){
        case 0: // high
            setPrimerPosWtNonTarget(30, 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,3,3,3);
            break;
        case 1: // intermediate
            setPrimerPosWtNonTarget(30, 0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3);
            break;
        default: // low
            setPrimerPosWtNonTarget(30, 0,1,1,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5);
            break;
        }
        break;
    case 3: // 70%
        switch( nonPosSel_4 ){
        case 0: // high
            setPrimerPosWtNonTarget(30, 0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3);
            break;
        case 1: // intermediate
            setPrimerPosWtNonTarget(30, 0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3);
            break;
        default: // low
            setPrimerPosWtNonTarget(30, 0,1,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5);
            break;
        }
        break;
    case 4: // 20%
        switch( nonPosSel_4 ){
        case 0: // high
            setPrimerPosWtNonTarget(30, 1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3);
            break;
        case 1: // intermediate
            setPrimerPosWtNonTarget(30, 1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3);
            break;
        default: // low
            setPrimerPosWtNonTarget(30, 1,1,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5);
            break;
        }
        break;
    case 5: // 10%
        switch( nonPosSel_4 ){
        case 0: // high
            setPrimerPosWtNonTarget(30, 1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3);
            break;
        case 1: // intermediate
            setPrimerPosWtNonTarget(30, 1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3);
            break;
        default: // low
            setPrimerPosWtNonTarget(30, 1,2,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5);
            break;
        }
        break;
    default: // 0%
        switch( nonPosSel_4 ){
        case 0: // high
            setPrimerPosWtNonTarget(30, 1,2,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5);
            break;
        case 1: // intermediate
            setPrimerPosWtNonTarget(30, 1,2,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5);
            break;
        default: // low
            setPrimerPosWtNonTarget(30, 1,2,3,3,3,3,3,4,4,4,4,4,4,4,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6);
            break;
        }
        break;
    }
}

void DataTools::setProbeMismatchAllowance(int contMatches_tar, int contMatches_nontar)
{
    switch( contMatches_tar ){
    case 0: // low
        setProbePosWtTarget(15, 1,1,1,1,1,1,2,2,2,2,2,2,3,3,3);
        break;
    case 1:
        setProbePosWtTarget(15, 0,1,1,1,1,1,1,2,2,2,2,2,3,3,3);
        break;
    case 2:
        setProbePosWtTarget(15, 0,0,1,1,1,1,1,2,2,2,2,2,2,3,3);
        break;
    case 3:
        setProbePosWtTarget(15, 0,0,0,1,1,1,1,1,2,2,2,2,2,3,3);
        break;
    case 4:
        setProbePosWtTarget(15, 0,0,0,0,1,1,1,1,1,2,2,2,2,2,3);
        break;
    case 5:
        setProbePosWtTarget(15, 0,0,0,0,0,1,1,1,1,2,2,2,2,3,3);
        break;
    case 6:
        setProbePosWtTarget(15, 0,0,0,0,0,0,1,1,1,1,2,2,2,3,3);
        break;
    case 7:
        setProbePosWtTarget(15, 0,0,0,0,0,0,0,1,1,1,2,2,2,2,3);
        break;
    case 8:
        setProbePosWtTarget(15, 0,0,0,0,0,0,0,0,1,1,1,2,2,2,3);
        break;
    case 9:
        setProbePosWtTarget(15, 0,0,0,0,0,0,0,0,0,1,1,1,1,2,2);
        break;
    case 10:
        setProbePosWtTarget(15, 0,0,0,0,0,0,0,0,0,0,1,1,1,2,2);
        break;
    case 11:
        setProbePosWtTarget(15, 0,0,0,0,0,0,0,0,0,0,0,1,1,1,2);
        break;
    case 12:
        setProbePosWtTarget(15, 0,0,0,0,0,0,0,0,0,0,0,0,1,1,1);
        break;
    case 13:
        setProbePosWtTarget(15, 0,0,0,0,0,0,0,0,0,0,0,0,0,1,1);
        break;
    case 14:
        setProbePosWtTarget(15, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1);
        break;
    case 15: //high
        setProbePosWtTarget(15, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
        break;
    default:
        QMessageBox::about(Q_NULLPTR, "Error value","Wrong parameter, pleasen contact the programmer");
        exit(1);
    }
// MX
    switch( contMatches_nontar ){
    case 0: // low
        setProbePosWtNonTarget(15, 1,2,2,3,3,3,4,4,4,4,5,5,5,5,5);
        break;
    case 1:
        setProbePosWtNonTarget(15, 0,1,2,2,3,3,3,4,4,4,4,5,5,5,5);
        break;
    case 2:
        setProbePosWtNonTarget(15, 0,0,1,2,2,3,3,3,4,4,4,4,5,5,5);
        break;
    case 3:
        setProbePosWtNonTarget(15, 0,0,0,1,2,2,3,3,3,4,4,4,4,5,5);
        break;
    case 4:
        setProbePosWtNonTarget(15, 0,0,0,0,1,2,2,3,3,3,4,4,4,4,5);
        break;
    case 5:
        setProbePosWtNonTarget(15, 0,0,0,0,0,1,2,2,3,3,3,4,4,4,4);
        break;
    case 6:
        setProbePosWtNonTarget(15, 0,0,0,0,0,0,1,2,2,3,3,3,4,4,4);
        break;
    case 7:
        setProbePosWtNonTarget(15, 0,0,0,0,0,0,0,1,2,2,3,3,3,4,4);
        break;
    case 8:
        setProbePosWtNonTarget(15, 0,0,0,0,0,0,0,0,1,2,2,3,3,3,4);
        break;
    case 9:
        setProbePosWtNonTarget(15, 0,0,0,0,0,0,0,0,0,1,2,2,3,3,3);
        break;
    case 10:
        setProbePosWtNonTarget(15, 0,0,0,0,0,0,0,0,0,0,1,2,2,3,3);
        break;
    case 11:
        setProbePosWtNonTarget(15, 0,0,0,0,0,0,0,0,0,0,0,1,2,2,3);
        break;
    case 12:
        setProbePosWtNonTarget(15, 0,0,0,0,0,0,0,0,0,0,0,0,1,2,2);
        break;
    case 13:
        setProbePosWtNonTarget(15, 0,0,0,0,0,0,0,0,0,0,0,0,0,1,2);
        break;
    case 14:
        setProbePosWtNonTarget(15, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1);
        break;
    case 15: //high
        setProbePosWtNonTarget(15, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
        break;
    default:
        QMessageBox::about(Q_NULLPTR, "Error value","Wrong parameter, pleasen contact the programmer");
        exit(1);
    }
}

void DataTools::restoreDefaultSetting()
{
    defaultPrimer();
    defaultProbe();
}

// return starting annealing position of primer in sequence
// if not anneal, return -1;
int DataTools::searchPrPbInNonbindingSeq(short *primer, short *sequence, int lft_rht)
{
    int score;
    int bestScore = 10000, bestPos = 0;
    int pLen = 0, sLen = 0;
    int nBaseFrom5end, nBaseFrom3end, minMatches5end, minMatches3end, nMatch;
    int i, j;


    // get primer length and sequence length
    while( primer[pLen] >= 0 )      pLen++;
    while( sequence[sLen] >= 0 )    sLen++;
    // get max mismatch number of 5' end and 3' end, respectively
    nBaseFrom3end = bindingContentDivider3end;
    nBaseFrom5end = pLen > nBaseFrom3end ? ( pLen - nBaseFrom3end ) : 0;
    minMatches3end = static_cast<int> ( bindingContentMatchPctg3end / 100 * nBaseFrom3end );
    minMatches5end = static_cast<int> ( bindingContentMatchPctg5end / 100 * nBaseFrom5end );


    // start searching

    if( lft_rht == LEFT_PRIMER ){	// left primer: get left most annealing position
        for( i = 0; i <= sLen - pLen; i++ ){
            score = 0;

            for( j = 0; j < pLen; j++ )
                if( sequence[i+j] != primer[j] )    score++;

            // record best score even seen
            if( bestScore > score ){
                bestScore = score;
                bestPos = i;
            }
        }
        // check if user's constraints of annealing content for non-annealing sequences are satisfied
        // left primer: left part (base 0) is 5' end, check 5' end first
        nMatch = 0;
        for( i = 0, j = bestPos; i < nBaseFrom5end && j < sLen; i++, j++ )
            if( primer[i] == sequence[j] )	nMatch++;
        if( nMatch < minMatches5end )	return -1;
        // check 3' end next
        nMatch = 0;
        while( i < pLen && j < sLen ){
            if( primer[i] == sequence[j] ){
                nMatch++;
            }
            i++;
            j++;
        }
        if( nMatch < minMatches3end )	return -1;
    }
    else{	// right primer: get right most annealing position
        for( i = 0; i <= sLen - pLen; i++ ){
            score = 0;

            for( j = 0; j < pLen; j++ )
                if( sequence[i+j] != primer[j] )    score++;

            // record best score even seen
            if( bestScore >= score ){
                bestScore = score;
                bestPos = i;
            }
        }
        // check if user's constraints of annealing content for non-annealing sequences are satisfied
        // right primer: left part (base 0) is 3' end, check 3' end first
        nMatch = 0;
        for( i = 0, j = bestPos; i < nBaseFrom3end && j < sLen; i++, j++ )
            if( primer[i] == sequence[j] )	nMatch++;
        if( nMatch < minMatches3end )	return -1;
        // check 5' end next
        nMatch = 0;
        while( i < pLen && j < sLen ){
            if( primer[i] == sequence[j] ){
                nMatch++;
            }
            i++;
            j++;
        }
        if( nMatch < minMatches5end )	return -1;
    }
    return bestPos;	// return the most possible annealing position
}


// return the 3' end mismatch rank information of primer in non-target sequences
// formula: No. mismatches of the 1st 3' base * 90% + No. mismatches of the 2nd base * 10%
// note: average left primer and right primer
float DataTools::get3endMismatchInfo(PrimerPair *pair)
{
    // get primer int sequence
    QString lprimerStr = pair->left, rprimerStr = pair->right;

    int pLen = lprimerStr.length();
    short *lprimer = new short[static_cast<unsigned long>(pLen+1)];
    for( int i = 0; i < pLen; i++ )
        lprimer[i] = static_cast<short>(tools::base2int(lprimerStr[i]));
    lprimer[pLen] = -1;

    pLen = pair->right.length();
    short *rprimer = new short[static_cast<unsigned long>(pLen+1)];
    for( int i = 0; i < pLen; i++ )
        rprimer[i] = static_cast<short>(tools::base2int(rprimerStr[i]));
    rprimer[pLen] = -1;

    PrimerSearch* lft;
    PrimerSearch* rht;
    try{
        lft = new PrimerSearch(lprimer, LEFT_PRIMER);
        rht = new PrimerSearch(rprimer, RIGHT_PRIMER);
    }
    catch( std::bad_alloc &){
        QMessageBox::about(Q_NULLPTR, "Fatal Error","Fatal error: out of memory for primer searching.");
        delete [] lprimer;
        delete [] rprimer;
        return -1;
    }


    QVector<short*> seqPtrBindLeft;
    QVector<short*> seqPtrBindRight;
    int numNonTarSeqs = nontarSeqs.size();
    short *seqPtr;
    int pos[4];


// -------------start primer searching in non-target sequences----------------------------
    for( int i = 0; i < numNonTarSeqs; i++ ){

        seqPtr = nontarSeqs[i];

        // search left and right primers in non-target sequence
        lft->appxSearchNonTarSeqs(seqPtr, LEFT_PRIMER, pos[0], pos[1]);
        rht->appxSearchNonTarSeqs(seqPtr, RIGHT_PRIMER, pos[2], pos[3]);

        // primer pair anneal with this sequence
        if( pos[0] >= 0 && pos[1] < pos[2] ){

            short *bindPtrLeft = seqPtr;
            bindPtrLeft += pos[0];
            seqPtrBindLeft.push_back(bindPtrLeft);

            short *bindPtrRight = seqPtr;
            bindPtrRight += pos[2];
            seqPtrBindRight.push_back(bindPtrRight);
        }
        else{ // primer pair doesn't anneal with this sequence

            // search for best match
            pos[0] = searchPrPbInNonbindingSeq(lprimer, seqPtr, LEFT_PRIMER);
            pos[2] = searchPrPbInNonbindingSeq(rprimer, seqPtr, RIGHT_PRIMER);

            // if the annealing order is right: record annealing info for non_binding sequence
            if( pos[0] >= 0 && pos[2] > 0 && pos[0] + lprimerStr.length() < pos[2] ){
                short *bindPtrLeft = seqPtr;
                bindPtrLeft += pos[0];
                seqPtrBindLeft.push_back(bindPtrLeft);

                short *bindPtrRight = seqPtr;
                bindPtrRight += pos[2];
                seqPtrBindRight.push_back(bindPtrRight);
            }
        }
    }


// -------------start counting the mismatches of the last two bases on 3' end of primers----------------------------
    int mismatch1stBase = 0, mismatch2ndBase = 0;
    int base1st, base2nd;

    base1st = lprimerStr.length() - 1;
    base2nd = base1st - 1;
    for(int i = 0; i < seqPtrBindLeft.size(); i++ ){
        if( lprimerStr[base1st] != seqPtrBindLeft[i][base1st] )
            mismatch1stBase++;
        if( lprimerStr[base2nd] != seqPtrBindLeft[i][base2nd] )
            mismatch2ndBase++;
    }
    base1st = 0;
    base2nd = 1;
    for(int i = 0; i < seqPtrBindRight.size(); i++ ){
        if( rprimerStr[base1st] != seqPtrBindRight[i][base1st] )
            mismatch1stBase++;
        if( rprimerStr[base2nd] != seqPtrBindRight[i][base2nd] )
            mismatch2ndBase++;
    }


// ------------house keeping clean---------------------------------------------
    delete[] lprimer;	delete[] rprimer;
    delete lft;			delete rht;


    return mismatch1stBase * 0.9f + mismatch2ndBase * 0.1f;
}

float DataTools::getPrimerInfo(PrimerPair *pair, int col)
{
    switch( col ){
        case SORT_BY_FORMULA:
            return static_cast<float>(pow( 100 - pair->pctgInTargetSeqs, 2 ) + 0.5 * pow( pair->pctgInNonTarSeqsLeft, 2 ) + 0.5 * pow( pair->pctgInNonTarSeqsRight, 2 ));
        case SORT_BY_MISMATCH:
            return get3endMismatchInfo(pair);
        case A_TARGET_PCTG: // Target%
            return pair->pctgInTargetSeqs;
        case A_LFT_TARGET_PCTG: // Forward Target%
            return pair->pctgInTarSeqsLeft;
        case A_RHT_TARGET_PCTG: // Reverse Target%
            return pair->pctgInTarSeqsRight;
        case A_NONTAR_PCTG: // Non-Target%
            return pair->pctgInNonTarSeqs;
        case A_LFT_NONTAR_PCTG: // Forward Non-Target%
            return pair->pctgInNonTarSeqsLeft;
        case A_RHT_NONTAR_PCTG: // Reverse Non-Target%
            return pair->pctgInNonTarSeqsRight;
        case A_LFT_TAR_BIND_POS: // Left Primer Target Annealing Avg Pos
            return pair->bindPosTarSeqsAvgLeft;
        case A_RHT_TAR_BIND_POS: // Right Primer Target Annealing Avg Pos
            return pair->bindPosTarSeqsAvgRight;
        case A_TAR_PCR_AVG: // Target PCR Avg Size
            return pair->pcrSizeAvgTarSeqs;
        case A_TAR_PCR_MIN: // Target PCR Min Size
            return static_cast<float>(pair->pcrSizeMinTarSeqs);
        case A_TAR_PCR_MAX: // Target PCR Max Size
            return static_cast<float>(pair->pcrSizeMaxTarSeqs);
        case A_NONTAR_PCR_AVG: // Non-Target PCR Avg Size
            return pair->pcrSizeAvgNonTarSeqs;
        case A_NONTAR_PCR_MIN: // Non-Target PCR Min Size
            return static_cast<float>(pair->pcrSizeMinNonTarSeqs);
        case A_NONTAR_PCR_MAX: // Non-Target PCR Max Size
            return static_cast<float>(pair->pcrSizeMaxNonTarSeqs);
        case A_LFT_GC: // Left GC%
            return pair->GCLeft;
        case A_RHT_GC: // Right GC%
            return pair->GCRight;
        case A_LFT_TM: // left Tm(C)
            return pair->tmLeft;
        case A_RHT_TM: // Right Tm(C)
            return pair->tmRight;
        case A_INTER_COMP: // Inter-Complementarity
            return static_cast<float>(pair->prPrComp);
        case A_INTER3_COMP: // 3' Inter-Complementarity
            return static_cast<float>(pair->prPr3Comp);
        case A_LFT_INTRA_COMP: // Left Primer Intra-Complementarity
            return static_cast<float>(pair->selfCompLeft);
        case A_RHT_INTRA_COMP: // Right Primer Intra-Complementarity
            return static_cast<float>(pair->selfCompRight);
        case A_LFT_INTRA3_COMP: // Left Primer 3' Intra-Complementarity
            return static_cast<float>(pair->self3CompLeft);
        case A_RHT_INTRA3_COMP: // Right Primer 3' Intra-Complementarity
            return static_cast<float>(pair->self3CompRight);
        case A_NUM_AMBI_TARGETS_LEFT: // Right Primer 3' Intra-Complementarity
            return static_cast<float>(pair->numAmbiguousTargetsLeft);
        case A_NUM_AMBI_TARGETS_RIGHT: // Right Primer 3' Intra-Complementarity
            return static_cast<float>(pair->numAmbiguousTargetsRight);
        case A_NUM_AMBI_BASES_LEFT: // Right Primer 3' Intra-Complementarity
            return static_cast<float>(pair->numAmbiguousBasesLeft);
        case A_NUM_AMBI_BASES_RIGHT: // Right Primer 3' Intra-Complementarity
            return static_cast<float>(pair->numAmbiguousBasesRight);
        default:
            return -1;
    }
}

float DataTools::getTripleInfo(Triple *tp, int col)
{
    //PrimerPair* Pr = tp->Pr;
    Probe* Pb = tp->Pb;
    switch( col ){
        case SORT_BY_FORMULA:
            return pow(100 - tp->pctgInTargetSeqs, 2 ) + pow( tp->pctgInNonTarSeqs, 2 ) + 0.5*(100 - tp->Pb->pctgInTargetSeqs) + 0.25*(tp->Pb->pctgInNonTarSeqs);
            //return pow(100 - tp->pctgInTargetSeqs, 2 ) + 0.5f * pow( tp->pctgInNonTarSeqs, 2 );
            //return	pow( 100 - pair->pctgInTargetSeqs, 2 ) + 0.5f * pow( pair->pctgInNonTarSeqsLeft, 2 ) + 0.5f * pow( pair->pctgInNonTarSeqsRight, 2 );
        case SORT_BY_MISMATCH:
            break;
            //return get3endMismatchInfo(pair);
        case B_TARGET_PCTG: // Target%
            return tp->pctgInTargetSeqs;
        //case B_LFT_TARGET_PCTG: // Forward Target%
        //    return Pr->pctgInTarSeqsLeft;
        //case B_RHT_TARGET_PCTG: // Reverse Target%
        //    return Pr->pctgInTarSeqsRight;
        case B_PROBE_TARGET_PCTG: // Target%
            return Pb->pctgInTargetSeqs;

        case B_NONTAR_PCTG: // Non-Target%
            return tp->pctgInNonTarSeqs;
        //case B_LFT_NONTAR_PCTG: // Forward Non-Target%
        //    return Pr->pctgInNonTarSeqsLeft;
        //case B_RHT_NONTAR_PCTG: // Reverse Non-Target%
        //    return Pr->pctgInNonTarSeqsRight;
        case B_PROBE_NONTAR_PCTG: // Non-Target%
            return Pb->pctgInNonTarSeqs;

        //case B_LFT_TAR_BIND_POS: // Right Primer Target Annealing Avg Pos
        //    return Pr->bindPosTarSeqsAvgLeft;
        //case B_RHT_TAR_BIND_POS: // Right Primer Target Annealing Avg Pos
        //    return Pr->bindPosTarSeqsAvgRight;
        case B_PROBE_TAR_BIND_POS: // Left Primer Target Annealing Avg Pos
            return Pb->bindPosTarSeqsAvg[0];


        /*case B_TAR_PCR_AVG: // Target PCR Avg Size
            return Pr->pcrSizeAvgTarSeqs;
        case B_TAR_PCR_MIN: // Target PCR Min Size
            return (float)Pr->pcrSizeMinTarSeqs;
        case B_TAR_PCR_MAX: // Target PCR Max Size
            return (float)Pr->pcrSizeMaxTarSeqs;

        case B_NONTAR_PCR_AVG: // Non-Target PCR Avg Size
            return Pr->pcrSizeAvgNonTarSeqs;
        case B_NONTAR_PCR_MIN: // Non-Target PCR Min Size
            return (float)Pr->pcrSizeMinNonTarSeqs;
        case B_NONTAR_PCR_MAX: // Non-Target PCR Max Size
            return (float)Pr->pcrSizeMaxNonTarSeqs;*/

        //case B_LFT_GC: // Left GC%
        //    return Pr->GCLeft;
        //case B_RHT_GC: // Right GC%
        //    return Pr->GCRight;
        case B_PROBE_GC: // Probe GC%
            return Pb->GCNum;

        //case B_LFT_TM: // Left Tm(C)
        //    return Pr->tmLeft;
        //case B_RHT_TM: // Right Tm(C)
        //    return Pr->tmRight;
        case B_PROBE_TM: // probe Tm(C)
            return Pb->tmProbe;

        case B_INTER_COMP: // Inter-Complementarity
            return (float)tp->pbPrComp;
        //case B_INTER3_COMP: // 3' Inter-Complementarity
        //    return (float)tp->pbPr3Comp;

        //case B_LFT_INTRA_COMP: // Right Primer Intra-Complementarity
        //    return (float)Pr->selfCompLeft;
        //case B_RHT_INTRA_COMP: // Right Primer Intra-Complementarity
        //    return (float)Pr->selfCompRight;
        case B_PROBE_INTRA_COMP: // Left Primer Intra-Complementarity
            return (float)Pb->selfComp;

        //case B_LFT_INTRA3_COMP: // Left Primer 3' Intra-Complementarity
        //    return (float)Pr->self3CompLeft;
        //case B_RHT_INTRA3_COMP: // Right Primer 3' Intra-Complementarity
        //    return (float)Pr->self3CompRight;
        //case B_PROBE_INTRA3_COMP: // Right Primer 3' Intra-Complementarity
        //    return (float)Pb->self3Comp;
        case B_NUM_AMBI_TARGETS:
            return (int)Pb->numAmbiguousTargets;
        case B_NUM_AMBI_BASES:
            return (int)Pb->numAmbiguousBases;
        default:
            return -1;
    }
    return 0xffffffffffffffff;
}

void DataTools::loadPpResult(QString filename, PrimerReportWindow* ){
    QFile file(filename);
    if (!file.open(QFile::ReadOnly)){
        QMessageBox::about(Q_NULLPTR, "File Error", "File doesn't exist or File corrupted");
        return;
    }
    QTextStream streamTmp ( &file );
    QString line  = streamTmp.readLine();
    streamTmp.readLine();   // read min size;
    streamTmp.readLine();   // read max size;


    int cntPrimerPairs = 0;

    int range = 500;
    int idx = 0;
    QVector<PrimerPair*> pairList;

    while(!streamTmp.atEnd() && cntPrimerPairs < range){
        PrimerPair *pr = new PrimerPair();

        streamTmp.readLine();   // empty line
        if(streamTmp.atEnd()){
            QMessageBox::about(Q_NULLPTR, "File Error", "The PP primer-pair file format is not correct.\n");
            return;
        }
        line = streamTmp.readLine();
        idx = line.indexOf('\t');
        assert(idx > 0);
        pr->left = line.right( line.size() - idx -1);

        if(streamTmp.atEnd()){
            QMessageBox::about(Q_NULLPTR, "File Error", "The PP primer-pair file format is not correct.\n");
            return;
        }
        line = streamTmp.readLine();
        idx = line.indexOf('\t');
        assert(idx > 0);
        pr->rightRevsComp = line.right( line.size() - idx -1);

        pairList.push_back(pr);

        cntPrimerPairs++;

        if(streamTmp.atEnd()){
            continue;
        }
        line = streamTmp.readLine(); // Estimated amplicon size
    }


    Designer::updataPrimerInfo(pairList);

    primerPairList = pairList + primerPairList;
    primerPairShowList = pairList + primerPairShowList;

    for(QVector<PrimerPair*>::iterator it = pairList.begin(); it < pairList.end(); it++){
        PrimerPair* pr = (*it);
        pr->left = "#" + pr->left;
    }
}

void DataTools::loadPrimerList(QString filename, PrimerReportWindow* parent){
    short * seqptr;
    int numTarSeqs;
    int numNontarSeqs;
    int numPrimerPairs;
    PrimerPair *pair;

    //Open Primer File.
    QFile checkFile(filename);
    if (!checkFile.open(QFile::ReadOnly)){
        QMessageBox::about(Q_NULLPTR, "File Error", "File doesn't exist or File corrupted");
        return;
    }
    QFile file(filename);
    if (!file.open(QFile::ReadOnly)){
        QMessageBox::about(Q_NULLPTR, "File Error", "File doesn't exist or File corrupted");
        return;
    }
    QTextStream streamTmp ( &checkFile );
    QString line  = streamTmp.readLine();
    checkFile.close();

    // check file type (compressed or uncompressed)
    QByteArray compressedData, origData;

    if(line.startsWith("Primer List File")){ //uncompressed file
        origData = file.readAll();
    }else{ //compressed file
        compressedData = file.readAll();
        origData = qUncompress(compressedData);
    }

    QTextStream stream(&origData);
    line = stream.readLine();
    if(line.compare("Primer list file 1.0. Please do not modify.")){
        QMessageBox::about(Q_NULLPTR, "File Error", QString("%1 is not a primer list file!").arg(filename));
        file.close();
        return;
    }

    //int iEstimate;
    line = stream.readLine();

    line = stream.readLine();
    stream >> primerLenMin >> primerLenMax;
    line = stream.readLine();

    line = stream.readLine();
    stream >> primerCandidFrequency;
    line = stream.readLine();

    line = stream.readLine();
    stream >> primerPcrSizeMin >> primerPcrSizeMax;
    line = stream.readLine();

    int iAvoid;
    line = stream.readLine();
    stream >> iAvoid;
    primerAvoidACT = static_cast<bool>(iAvoid);

    stream >> iAvoid;
    primerAvoidG = static_cast<bool>(iAvoid);

    stream >> iAvoid;
    primerAvoidCGat3end = static_cast<bool>(iAvoid);
    line = stream.readLine();

    line = stream.readLine();
    stream >> primerNumACT >> primerNumG >>  primerNumCGat3end;
    line = stream.readLine();


    //line = stream.readLine();
    //stream >> iEstimate;
    //primerEstimate = (bool)iEstimate;
    //line = stream.readLine();

    line = stream.readLine();
    stream >> primerTmMin >> primerTmMax;
    line = stream.readLine();

    line = stream.readLine();
    stream >> primerNaConsent;
    line = stream.readLine();

    line = stream.readLine();
    stream >> primerTmDiff;
    line = stream.readLine();

    line = stream.readLine();
    stream >> primerGCMin >> primerGCMax;
    line = stream.readLine();

    line = stream.readLine();
    stream >> primerMaxPrPrComp;
    line = stream.readLine();

    line = stream.readLine();
    stream >> primerMax3PrPrComp;
    line = stream.readLine();

    line = stream.readLine();
    stream >> primerInterBase3end;
    line = stream.readLine();

    line = stream.readLine();
    stream >> primerMaxSelfComp;
    line = stream.readLine();

    line = stream.readLine();
    stream >> primerMax3SelfComp;
    line = stream.readLine();

    line = stream.readLine();
    stream >> primerIntraBase3end;
    line = stream.readLine();

    line = stream.readLine();
    stream >> maxIndel;
    line = stream.readLine();

    line = stream.readLine();
    stream >> indelCost;
    line = stream.readLine();

    int iPrimerCostType;
    line = stream.readLine();
    stream >> iPrimerCostType;
    line = stream.readLine();
    primerCostType = static_cast<bool>(iPrimerCostType);

    line = stream.readLine();
    for( int i = 0; i < 15; i++ ){
        for( int j = 0; j < 15; j++ )
            stream >> simCost[i][j];
    }
    line = stream.readLine();

    line = stream.readLine();
    for( int i = 0; i < MAX_PRIMER_LEN; i++ )
        stream >> primerPosWtTar[i];
    line = stream.readLine();

    line = stream.readLine();
    for( int i = 0; i < MAX_PRIMER_LEN; i++ )
        stream >> primerPosWtNonTar[i];

    line = stream.readLine();
    line = stream.readLine();
    line = stream.readLine();

    /*Clean Memory: (for loading primer Pair)
      1. Target&Nontar Seqs
      2. Primer Candidates
      3. Primer Pairs
      4. probe Candidates
      5. probes
      6. Triples (in Primer Pair)
    */
    cleanMemoryAllData();



    // 3. read target sequences
    stream >> numTarSeqs;
    line = stream.readLine();
    for( int i = 0; i < numTarSeqs; i++ ){
        line = stream.readLine();
        tarSeqNames.push_back(line);

        line = stream.readLine();
        // add target sequences to theApp
        seqptr = new short[static_cast<unsigned long>(line.length()+1)];
        int j = 0;
        while( j < line.length() ){
            seqptr[j] = static_cast<short>(tools::base2int(line[j]));
            j++;
        }
        seqptr[j] = -1;
        tarSeqs.push_back(seqptr);
    }

    // 4. read non-target sequences
    stream >> numNontarSeqs;
    line = stream.readLine();
    for( int i = 0; i < numNontarSeqs; i++ ){
        line = stream.readLine();
        nontarSeqNames.push_back(line);

        line = stream.readLine();
        // add target sequences to theApp
        seqptr = new short[static_cast<unsigned long>(line.length()+1)];
        int j = 0;
        while( j < line.length()){
            seqptr[j] = static_cast<short>(tools::base2int(line[j]));
            j++;
        }
        seqptr[j] = -1;
        nontarSeqs.push_back(seqptr);
    }

    // 5. read primer pairs
    stream >> numPrimerPairs;
    line = stream.readLine();
    int range = numPrimerPairs;
    MyProgressDlg *pDlg = new MyProgressDlg("Loading primer list..",0, range);
    pDlg->setWindowModality(Qt::ApplicationModal);


    //int iIsEstimateTar;
    //int iIsEstimateNonTar;
    for( int i = 0; i < numPrimerPairs; i++ ){
        pair = new PrimerPair();
        line = stream.readLine();
        pair->left = line;
        line = stream.readLine();
        pair->right = line;
        line = stream.readLine();
        pair->rightRevsComp = line;

        stream	//>> iIsEstimateTar
                        //>> iIsEstimateNonTar
                        >> pair->pctgInTargetSeqs
                        >> pair->pctgInTarSeqsLeft
                        >> pair->pctgInTarSeqsRight
                        >> pair->pctgInNonTarSeqs
                        >> pair->pctgInNonTarSeqsLeft
                        >> pair->pctgInNonTarSeqsRight
                        >> pair->bindPosTarSeqsAvgLeft
                        >> pair->bindPosTarSeqsAvgRight
                        >> pair->pcrSizeAvgTarSeqs
                        >> pair->pcrSizeMinTarSeqs
                        >> pair->pcrSizeMaxTarSeqs
                        >> pair->pcrSizeAvgNonTarSeqs
                        >> pair->pcrSizeMinNonTarSeqs
                        >> pair->pcrSizeMaxNonTarSeqs
                        >> pair->GCLeft
                        >> pair->GCRight
                        >> pair->tmLeft
                        >> pair->tmRight
                        >> pair->prPrComp
                        >> pair->prPr3Comp
                        >> pair->selfCompLeft
                        >> pair->selfCompRight
                        >> pair->self3CompLeft
                        >> pair->self3CompRight
                        >> pair->numAmbiguousTargetsLeft
                        >> pair->numAmbiguousTargetsRight
                        >> pair->numAmbiguousBasesLeft
                        >> pair->numAmbiguousBasesRight;
        line = stream.readLine();

        //allocate memory for recording binding position
        pair->posBindingTar = new short*[2];
        for(int j = 0; j < 2; j++){
            pair->posBindingTar[j] = new short[static_cast<unsigned long>(numTarSeqs)];
        }
        pair->posBindingNontar = new short*[2];
        for(int j = 0; j < 2; j++){
            pair->posBindingNontar[j] = new short[static_cast<unsigned long>(numNontarSeqs)];
        }

        short* posTmp = pair->posBindingTar[0];
        for(int j = 0; j < numTarSeqs; j++){
            stream	>> posTmp[j];
        }
        stream.readLine();

        posTmp = pair->posBindingTar[1];
        for(int j = 0; j < numTarSeqs; j++){
            stream	>> posTmp[j];
        }
        stream.readLine();

        posTmp = pair->posBindingNontar[0];
        for(int j = 0; j < numNontarSeqs; j++){
            stream	>> posTmp[j];
        }
        stream.readLine();

        posTmp = pair->posBindingNontar[1];
        for(int j = 0; j < numNontarSeqs; j++){
            stream	>> posTmp[j];
        }
        stream.readLine();

        //pair->isEstimateTar = (bool) iIsEstimateTar;
        //pair->isEstimateNonTar = (bool) iIsEstimateNonTar;

        pair->marked = 0;
        primerPairList.push_back(pair);
        pDlg->setValue(i+1);
    }
    file.close();
    delete pDlg;
    parent->displayData();

    QString headStr;
    int numTar = tarSeqs.size();
    headStr = QString("# of target sequences\n(out of %1)\nwith ambiguous bases\ncovered by Fw primer").arg(numTar);
    parent->tableModel->setHeaderData(26, Qt::Horizontal, headStr);
    headStr = QString("# of target sequences\n(out of %1)\nwith ambiguous bases\ncovered by Rv primer").arg(numTar);
    parent->tableModel->setHeaderData(27, Qt::Horizontal, headStr);

    QString str;
    str = QString("%1 primer pairs are loaded in memory.\r\n"
                            "%2 primer pairs are displayed in primer pair table.").arg(primerPairList.size()).arg( primerPairShowList.size() );
    QMessageBox QMBox;
    QMBox.setWindowTitle("Primer Info");
    QMBox.setText(str);
    QMBox.exec();
    //QMessageBox::about(0, "Primer Info",str);
}

void DataTools::savePrimerList(QString filename, QVector<int> pMarked)
{

    QFile file(filename);
    if (!file.open(QFile::WriteOnly)){
        QMessageBox QMBox;
        QMBox.setWindowTitle("File Error");
        QMBox.setText("Error saving primer list file! \n Please report the problem to the developer");
        QMBox.exec();
        //QMessageBox::about(0, "File Error", "Error saving primer list file! \n Please report the problem to the developer");
        return;
    }

    QByteArray origData;
    origData.reserve(200000000);
    QByteArray compressedData;
    compressedData.reserve(75000000);
    QTextStream in(&origData);


    in << "Primer list file 1.0. Please do not modify.\n\n"
        << "Primer length range:\n" << primerLenMin << " " << primerLenMax << endl
        << "% target contain a candidate primer at least:\n" << primerCandidFrequency << "%\n"
        << "PCR product size range:\n" << primerPcrSizeMin << " " << primerPcrSizeMax << endl
        << "Base avoidance constraints:\n" << primerAvoidACT << " " << primerAvoidG << " " << primerAvoidCGat3end << endl
        << "Num Avoidance:\n" << primerNumACT << " " << primerNumG << " " << primerNumCGat3end << endl
        //<< "Get estimated or exact PCR product information:\n" << primerEstimate << endl
        << "Melting temperature between:\n" << primerTmMin << " " << primerTmMax << endl
        << "Salt concentration:\n" << primerNaConsent << endl
        << "Primer melting temperature of primers differ no more than:\n" << primerTmDiff << endl
        << "Primer GC% between:\n" << primerGCMin << " " << primerGCMax << endl
        << "Max primer inter-complementarity:\n" << primerMaxPrPrComp << endl
        << "Max primer 3' inter-complementarity:\n" << primerMax3PrPrComp << endl
        << "Number of bases to count at 3' end(inter):\n" << primerInterBase3end << endl
        << "Max primer intra-complementarity:\n" << primerMaxSelfComp << endl
        << "Max primer 3' intra-complementarity:\n" << primerMax3SelfComp << endl
        << "Number of bases to count at 3' end(intra):\n" << primerIntraBase3end << endl
        << "Max number of insertion and deletion allowed:\n" << maxIndel << endl
        << "Insertion and deletion cost:\n" << indelCost << endl
        << "cost function type\n" << primerCostType << endl;

    in << "Mismatch cost:\n";
    for( int i = 0; i < 15; i++ ){
        for( int j = 0; j < 15; j++ )
            in << simCost[i][j] << " ";
    }

    in << "\nPositional weight cost: target sequences:\n";
    for( int i = 0; i < MAX_PRIMER_LEN; i++ )
        in << primerPosWtTar[i] << " ";
    in << "\nnontarget sequences:\n";
    for( int i = 0; i < MAX_PRIMER_LEN; i++ )
        in << primerPosWtNonTar[i] << " ";
    in << "\n---------------End of settings.--------------\n\n";


    int numTarSeqs = tarSeqNames.size();
    int numNontarSeqs = nontarSeqNames.size();
    in << numTarSeqs << endl;
    QString tmpSeq;
    for( int i = 0; i < numTarSeqs; i++ ){
        tools::seqInt2Str(tarSeqs[i],tmpSeq);
        in << tarSeqNames[i] << endl << tmpSeq << endl;
    }
    in << numNontarSeqs << endl;
    for( int i = 0; i < numNontarSeqs; i++ ){
        tools::seqInt2Str(nontarSeqs[i],tmpSeq);
        in << nontarSeqNames[i] << endl << tmpSeq << endl;
    }

    MyProgressDlg *pDlg;
    int numPrimerPairs;
    PrimerPair* pair;
    int range;

    // user choose to save all (at most 35000) primer pairs
    if( primerListSaveAllDispMark == 0 ){
        numPrimerPairs = primerPairList.size();

        range = min(numPrimerPairs, 50000);
        pDlg = new MyProgressDlg("Saving primer list",0, range);
        pDlg->setWindowModality(Qt::ApplicationModal);
        in << numPrimerPairs << endl;

        for( int i = 0; i < range; i++ ){
            pair = primerPairList[i];
            in << pair->left << endl << pair->right << endl << pair->rightRevsComp << endl;
                //<< pair->isEstimateTar << " "
                //<< pair->isEstimateNonTar;
            for( int col = A_TARGET_PCTG; col < A_NUM_COLUMNS; col++ ){
                in << " " << getPrimerInfo( pair, col );
            }
            in << endl;

            short* posBinding =pair->posBindingTar[0];
            for(int j = 0; j < numTarSeqs; j++){
                in << posBinding[j] << " ";
            }
            in << endl;
            posBinding =pair->posBindingTar[1];
            for(int j = 0; j < numTarSeqs; j++){
                in << posBinding[j] << " ";
            }
            in << endl;
            posBinding =pair->posBindingNontar[0];
            for(int j = 0; j < numNontarSeqs; j++){
                in << posBinding[j] << " ";
            }
            in << endl;
            posBinding =pair->posBindingNontar[1];
            for(int j = 0; j < numNontarSeqs; j++){
                in << posBinding[j] << " ";
            }
            in << endl;
            in.flush();
            pDlg->setValue(i+1);
        }
    }
    else if( primerListSaveAllDispMark == 1 ){	// user wants to save all displayed primer pairs
        numPrimerPairs = primerPairShowList.size();
        in << numPrimerPairs << endl;

        range = numPrimerPairs;
        pDlg = new MyProgressDlg("Saving primer list",0, range);
        pDlg->setWindowModality(Qt::ApplicationModal);

        for( int i = 0; i < numPrimerPairs; i++ ){
            pair = primerPairShowList[i];
            in << pair->left << endl << pair->right << endl << pair->rightRevsComp << endl;
                //<< pair->isEstimateTar << " "
                //<< pair->isEstimateNonTar;
            for( int col = A_TARGET_PCTG; col < A_NUM_COLUMNS; col++ ){
                in << " " << getPrimerInfo( pair, col );
            }
            in << endl;

            short* posBinding =pair->posBindingTar[0];
            for(int j = 0; j < numTarSeqs; j++){
                in << posBinding[j] << " ";
            }
            in << endl;
            posBinding =pair->posBindingTar[1];
            for(int j = 0; j < numTarSeqs; j++){
                in << posBinding[j] << " ";
            }
            in << endl;
            posBinding =pair->posBindingNontar[0];
            for(int j = 0; j < numNontarSeqs; j++){
                in << posBinding[j] << " ";
            }
            in << endl;
            posBinding =pair->posBindingNontar[1];
            for(int j = 0; j < numNontarSeqs; j++){
                in << posBinding[j] << " ";
            }
            in << endl;
            in.flush();
            pDlg->setValue(i+1);
        }
    }
    else if( primerListSaveAllDispMark == 2 ){	// user wants to save marked primer pairs only

        // write marked primer pairs to outfile
        numPrimerPairs = (int)pMarked.size();
        in << numPrimerPairs << endl;

        range = numPrimerPairs;
        pDlg = new MyProgressDlg("Saving primer list",0, range);
        pDlg->setWindowModality(Qt::ApplicationModal);

        for( int i = 0; i < numPrimerPairs; i++ ){
            pair = primerPairShowList[pMarked[i]];
            in << pair->left << endl << pair->right << endl << pair->rightRevsComp << endl;
                //<< pair->isEstimateTar << " "
                //<< pair->isEstimateNonTar;
            for( int col = A_TARGET_PCTG; col < A_NUM_COLUMNS; col++ )
                in << " " << getPrimerInfo( pair, col );
            in << endl;

            short* posBinding =pair->posBindingTar[0];
            for(int j = 0; j < numTarSeqs; j++){
                in << posBinding[j] << " ";
            }
            in << endl;
            posBinding =pair->posBindingTar[1];
            for(int j = 0; j < numTarSeqs; j++){
                in << posBinding[j] << " ";
            }
            in << endl;
            posBinding =pair->posBindingNontar[0];
            for(int j = 0; j < numNontarSeqs; j++){
                in << posBinding[j] << " ";
            }
            in << endl;
            posBinding =pair->posBindingNontar[1];
            for(int j = 0; j < numNontarSeqs; j++){
                in << posBinding[j] << " ";
            }
            in << endl;
            in.flush();
            pDlg->setValue(i+1);
        }
    }

    //int a = origData.length();

    // zip the primer list temp file, then remove the temp file

    compressedData = qCompress(origData);

    //int b = compressedData.length();
    file.write(compressedData);
    file.close();

    delete pDlg;

    QMessageBox::about(Q_NULLPTR, "File saved", QString("%1 primer pairs were saved to primer list file: %2").arg(range).arg(filename));
}

void DataTools::savePrimerList2TXT(QString filename, QVector<int> pMarked)
{
    QFile file(filename);
    if (!file.open(QFile::WriteOnly)){
        QMessageBox::about(Q_NULLPTR, "File Error", "Error saving primer to TXT file! \n Please report the problem to the developer");
        return;
    }

    QTextStream out(&file);

    // output column headers
    out << "Primer No.\tForward Primer\tReverse Primer";
    for( int i = A_TARGET_PCTG; i < A_NUM_COLUMNS; i++ ){
        if( primerExcelSaveColumns[i] ){
            QString colName = primerColumns[i];
            colName.replace('\n',' ');
            out << "\t" << colName;
        }
    }
    out << endl;

    int numPrimerPairs;
    PrimerPair *pair;
    MyProgressDlg* pDlg;

    // user choose to save all primer pairs
    if( primerExcelSaveAllDispMark == 0 ){

        numPrimerPairs = primerPairList.size();
        pDlg = new MyProgressDlg("Saving primer list",1,numPrimerPairs, Q_NULLPTR);
        pDlg->setWindowModality(Qt::ApplicationModal);

        for( int i = 0; i < numPrimerPairs; i++ ){
            pair = primerPairList[i];
            out << i+1 << "\t" << pair->left << "\t" << pair->rightRevsComp;
            for( int col = A_TARGET_PCTG; col < A_NUM_COLUMNS; col++ ){
                if( primerExcelSaveColumns[col] && (static_cast<QString>(primerColumns[col]).indexOf('%') >= 0 ))
                    out << "\t" << DataTools::getPrimerInfo( pair, col ) << "%";
                else if( primerExcelSaveColumns[col] )
                    out << "\t" << DataTools::getPrimerInfo( pair, col );
            }
            out << endl;
            out.flush();

            pDlg->setValue(i+1);
        }
    }
    else if( primerExcelSaveAllDispMark == 1 ){	// user wants to save all displayed primer pairs
        numPrimerPairs = primerPairShowList.size();
        pDlg = new MyProgressDlg("Saving primer list",1,numPrimerPairs, Q_NULLPTR);
        pDlg->setWindowModality(Qt::ApplicationModal);

        for( int i = 0; i < numPrimerPairs; i++ ){
            pair = primerPairShowList[i];
            out << i+1 << "\t" << pair->left << "\t" << pair->rightRevsComp;
            for( int col = A_TARGET_PCTG; col < A_NUM_COLUMNS; col++ ){
                if( primerExcelSaveColumns[col] && (static_cast<QString>(primerColumns[col]).indexOf('%') >= 0 ))
                    out << "\t" << DataTools::getPrimerInfo( pair, col ) << "%";
                else if( primerExcelSaveColumns[col] )
                    out << "\t" << DataTools::getPrimerInfo( pair, col );
            }
            out << endl;
            out.flush();

            pDlg->setValue(i+1);
        }
    }
    else if( primerExcelSaveAllDispMark == 2 ){	// user wants to save marked primer pairs only

        // write marked primer pairs to outfile
        numPrimerPairs = pMarked.size();
        pDlg = new MyProgressDlg("Saving primer list",1,numPrimerPairs);
        pDlg->setWindowModality(Qt::ApplicationModal);

        for( int i = 0; i < numPrimerPairs; i++ ){
            pair = primerPairShowList[pMarked[i]];
            out << i+1 << "\t" << pair->left << "\t" << pair->rightRevsComp;
            for( int col = A_TARGET_PCTG; col < A_NUM_COLUMNS; col++ ){
                if( primerExcelSaveColumns[col] && (static_cast<QString>(primerColumns[col]).indexOf('%') >= 0 ))
                    out << "\t" << DataTools::getPrimerInfo( pair, col ) << "%";
                else if( primerExcelSaveColumns[col] )
                    out << "\t" << DataTools::getPrimerInfo( pair, col );
            }
            out << endl;
            out.flush();

            pDlg->setValue(i+1);
        }
    }

    file.close();

    delete pDlg;

    QMessageBox::about(Q_NULLPTR, "File saved", QString("%1 primer pairs were saved to file: %2").arg(numPrimerPairs).arg(filename));
}

void DataTools::saveText2File(QString filename, QString text)
{
    QFile file(filename);
    if (!file.open(QFile::WriteOnly)){
        QMessageBox::about(Q_NULLPTR, "File Error", "Error saving Information to TXT file! \n Please report the problem to the developer");
        return;
    }

    QTextStream out(&file);
    out << text;

    file.close();
    QMessageBox::about(Q_NULLPTR, "Information Saved", "All contents were saved to file: "+filename);
}

void DataTools::savePrimerPairs(QString filename, QVector<int> pMarked)
{
    QFile file(filename);
    if (!file.open(QFile::WriteOnly)){
        QMessageBox::about(Q_NULLPTR, "File Error", "Error saving Information to TXT file! \n Please report the problem to the developer");
        return;
    }
    QTextStream out(&file);

    int numPrimerPairs;
    PrimerPair *pair;
    MyProgressDlg* pDlg;

    // user choose to save all primer pairs
    if( primerExcelSaveAllDispMark == 0 ){
        numPrimerPairs = primerPairList.size();

        pDlg = new MyProgressDlg("Saving primer pairs",1,numPrimerPairs, Q_NULLPTR);
        pDlg->setWindowModality(Qt::ApplicationModal);

        for( int i = 0; i < numPrimerPairs; i++ ){
                pair = primerPairList[i];
                out << pair->left << ".." << pair->rightRevsComp << endl;
                out.flush();

                pDlg->setValue(i+1);
        }
    }
    else if( primerExcelSaveAllDispMark == 1 ){	// user wants to save all displayed primer pairs
        numPrimerPairs = primerPairShowList.size();
        pDlg = new MyProgressDlg("Saving primer pairs",1,numPrimerPairs, Q_NULLPTR);
        pDlg->setWindowModality(Qt::ApplicationModal);

        for( int i = 0; i < numPrimerPairs; i++ ){
            pair = primerPairShowList[i];
            out << pair->left << ".." << pair->rightRevsComp << endl;
            out.flush();

            pDlg->setValue(i+1);
        }
    }
    else if( primerExcelSaveAllDispMark == 2 ){	// user wants to save marked primer pairs only
        // write marked primer pairs to outfile
        numPrimerPairs = pMarked.size();
        pDlg = new MyProgressDlg("Saving primer pairs",1,numPrimerPairs, Q_NULLPTR);
        pDlg->setWindowModality(Qt::ApplicationModal);

        for( int i = 0; i < numPrimerPairs; i++ ){
            pair = primerPairShowList[pMarked[i]];
            out << pair->left << ".." << pair->rightRevsComp << endl;
            out.flush();

            pDlg->setValue(i+1);
        }
    }
    file.close();

    delete pDlg;

    QMessageBox::about(Q_NULLPTR, "File saved", QString("%1 primer pairs were saved to file: %2").arg(numPrimerPairs).arg(filename));
}


/*
void DataTools::addDisplayPrimerList()
{
    MyProgressDlg *pDlg = new MyProgressDlg("Adding primers to primer table...",1,primerPairShowList.size(),0);
    pDlg->setWindowModality(Qt::ApplicationModal);

    // add all primer pairs in result list one by one
    for( unsigned int i = 0; i < primerPairShowList.size(); i++ ){
        _OnInsertPrimer(i);
        pDlg->setValue(i+1);
    }
    delete pDlg;
}*/

void DataTools::sortPrimerList()
{
    int numPrimerPairs = primerPairShowList.size();
    float *baseArray;
    try{
        baseArray = new float[static_cast<unsigned long>(numPrimerPairs)];
    }
    catch( std::bad_alloc &){
        QMessageBox::about(Q_NULLPTR,"Fatal Error","Fatal error: out of memory for sorting.\nProgram is forced to terminate.");
        /*Clean Memory: (for loading triple)
          1. Target&Nontar Seqs
          2. Primer Candidates
          3. Primer Pairs
          4. probe Candidates
          5. probes
          6. Triples (in Primer Pair)
          7. T & P
        */
        DataTools::cleanMemoryAllData();
        exit(-1);
    }

    PrimerPair *pair;
    float temp;
    float *baseArray1, *baseArray2;
    try{
        baseArray1 = new float[static_cast<unsigned long>(numPrimerPairs)];
        baseArray2 = new float[static_cast<unsigned long>(numPrimerPairs)];
    }
    catch( std::bad_alloc &){
        QMessageBox::about(Q_NULLPTR,"Fatal Error","Fatal error: out of memory for sorting.\nProgram is forced to terminate.");
        QMessageBox::about(Q_NULLPTR,"Fatal Error","Fatal error: out of memory for sorting.\nProgram is forced to terminate.");
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
        exit(-1);
    }

    MyProgressDlg *pDlg = new MyProgressDlg("Collecting primer information",1, numPrimerPairs,Q_NULLPTR);
    pDlg->setWindowModality(Qt::ApplicationModal);

    for( int i = 0; i < numPrimerPairs; i++ ){
        baseArray1[i] = getPrimerInfo( primerPairShowList[i], primerSortBy[0] );
        baseArray2[i] = getPrimerInfo( primerPairShowList[i], primerSortBy[1] );

        pDlg->setValue(i);
    }

    delete pDlg;

    pDlg = new MyProgressDlg("Sorting primer list",1, numPrimerPairs*3,Q_NULLPTR);
    pDlg->setWindowModality(Qt::ApplicationModal);


    // ascending & ascending order
    if( primerSortOrder[0] == ASCENDING && primerSortOrder[1] == ASCENDING ){
        // insertion sort according to baseArray1, then by baseArray2
        for( int i = 1; i < numPrimerPairs; i++ ){
            for( int j = 0; j < i; j++){
                if( baseArray1[j] > baseArray1[i] ){
                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    pair = primerPairShowList[i];
                    primerPairShowList[i] = primerPairShowList[j];
                    primerPairShowList[j] = pair;
                }
                else if( baseArray1[j] == baseArray1[i] && baseArray2[j] > baseArray2[i] ){
                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    pair = primerPairShowList[i];
                    primerPairShowList[i] = primerPairShowList[j];
                    primerPairShowList[j] = pair;
                }
            }
            pDlg->setValue(i);
        }
    }
    // ascending & descending order
    else if( primerSortOrder[0] == ASCENDING && primerSortOrder[1] == DESCENDING ){
        // insertion sort according to baseArray1, then by baseArray2
        for( int i = 1; i < numPrimerPairs; i++ ){
            for( int j = 0; j < i; j++){
                if( baseArray1[j] > baseArray1[i] ){
                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    pair = primerPairShowList[i];
                    primerPairShowList[i] = primerPairShowList[j];
                    primerPairShowList[j] = pair;
                }
                else if( baseArray1[j] == baseArray1[i] && baseArray2[j] < baseArray2[i] ){
                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    pair = primerPairShowList[i];
                    primerPairShowList[i] = primerPairShowList[j];
                    primerPairShowList[j] = pair;
                }
            }
            pDlg->setValue(i);
        }
    }
    // descending & ascending order
    else if( primerSortOrder[0] == DESCENDING && primerSortOrder[1] == ASCENDING ){
        // insertion sort according to baseArray1, then by baseArray2
        for( int i = 1; i < numPrimerPairs; i++ ){
            for( int j = 0; j < i; j++){
                if( baseArray1[j] < baseArray1[i] ){
                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    pair = primerPairShowList[i];
                    primerPairShowList[i] = primerPairShowList[j];
                    primerPairShowList[j] = pair;
                }
                else if( baseArray1[j] == baseArray1[i] && baseArray2[j] > baseArray2[i] ){
                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    pair = primerPairShowList[i];
                    primerPairShowList[i] = primerPairShowList[j];
                    primerPairShowList[j] = pair;
                }
            }
            pDlg->setValue(i);
        }
    }
    else{ // descending & descending order
        // insertion sort according to baseArray
        for( int i = 1; i < numPrimerPairs; i++ ){
            for( int j = 0; j < i; j++){
                if( baseArray1[j] < baseArray1[i] ){
                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    pair = primerPairShowList[i];
                    primerPairShowList[i] = primerPairShowList[j];
                    primerPairShowList[j] = pair;
                }
                else if( baseArray1[j] == baseArray1[i] && baseArray2[j] < baseArray2[i] ){
                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    pair = primerPairShowList[i];
                    primerPairShowList[i] = primerPairShowList[j];
                    primerPairShowList[j] = pair;
                }
            }
            pDlg->setValue(i);
        }
    }

    // only "sort by", "then by", no "then by", "then by"
    if( primerSortBy[2] <= 0 && primerSortBy[3] <= 0 ){

        delete[] baseArray1;
        delete[] baseArray2;

        delete pDlg;

        //DataTools::addDisplayPrimerList();
        return;
    }

    float *baseArray3;
    try{
        baseArray3 = new float[static_cast<unsigned long>(numPrimerPairs)];
    }
    catch(  std::bad_alloc &){
        QMessageBox::about(Q_NULLPTR,"Fatal Error","Fatal error: out of memory for sorting.\nProgram is forced to terminate.");
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
        exit(-1);
    }
    for( int i = 0; i < numPrimerPairs; i++ )
        baseArray3[i] = getPrimerInfo( primerPairShowList[i], primerSortBy[2] );


    // sort by "then by" 2

    // ascending order
    if( primerSortOrder[2] == ASCENDING ){
        // insertion sort according to baseArray3
        for( int i = 1; i < numPrimerPairs; i++ ){
            for( int j = 0; j < i; j++){
                if( baseArray1[j] == baseArray1[i] && baseArray2[j] == baseArray2[i] && baseArray3[j] > baseArray3[i] ){
                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    temp = baseArray3[i];
                    baseArray3[i] = baseArray3[j];
                    baseArray3[j] = temp;
                    pair = primerPairShowList[i];
                    primerPairShowList[i] = primerPairShowList[j];
                    primerPairShowList[j] = pair;
                }
            }
            pDlg->setValue(numPrimerPairs + i);
        }
    }
    // descending order
    else if( primerSortOrder[2] == DESCENDING ){
        // insertion sort according to baseArray3
        for( int i = 1; i < numPrimerPairs; i++ ){
            for( int j = 0; j < i; j++){
                if( baseArray1[j] == baseArray1[i] && baseArray2[j] == baseArray2[i] && baseArray3[j] < baseArray3[i] ){
                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    temp = baseArray3[i];
                    baseArray3[i] = baseArray3[j];
                    baseArray3[j] = temp;
                    pair = primerPairShowList[i];
                    primerPairShowList[i] = primerPairShowList[j];
                    primerPairShowList[j] = pair;
                }
            }
            pDlg->setValue(numPrimerPairs + i);
        }
    }

    // "sort by" -> "then by" -> "then by", no more "then by"
    if( primerSortBy[3] <= 0 ){

        delete[] baseArray1;
        delete[] baseArray2;
        delete[] baseArray3;

        delete pDlg;

        //DataTools::addDisplayPrimerList();
        return;
    }

    float *baseArray4;
    try{
        baseArray4 = new float[static_cast<unsigned long>(numPrimerPairs)];
    }
    catch(  std::bad_alloc &){
        QMessageBox::about(Q_NULLPTR,"Fatal Error","Fatal error: out of memory for sorting.\nProgram is forced to terminate.");
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
        exit(-1);
    }

    for( int i = 0; i < numPrimerPairs; i++ )
         baseArray4[i] = getPrimerInfo( primerPairShowList[i], primerSortBy[3] );


    // sort by "then by" 3

    // ascending order
    if( primerSortOrder[3] == ASCENDING ){
        // insertion sort according to baseArray4
        for( int i = 1; i < numPrimerPairs; i++ ){
            for( int j = 0; j < i; j++){
                if( baseArray1[j] == baseArray1[i] && baseArray2[j] == baseArray2[i] &&
                    baseArray3[j] == baseArray3[i] && baseArray4[j] > baseArray4[i] ){

                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    temp = baseArray3[i];
                    baseArray3[i] = baseArray3[j];
                    baseArray3[j] = temp;
                    temp = baseArray4[i];
                    baseArray4[i] = baseArray4[j];
                    baseArray4[j] = temp;
                    pair = primerPairShowList[i];
                    primerPairShowList[i] = primerPairShowList[j];
                    primerPairShowList[j] = pair;
                }
            }

            pDlg->setValue( numPrimerPairs*2 + i );
        }
    }
    // descending order
    else if( primerSortOrder[3] == DESCENDING ){
        // insertion sort according to baseArray4
        for( int i = 1; i < numPrimerPairs; i++ ){
            for( int j = 0; j < i; j++){
                if( baseArray1[j] == baseArray1[i] && baseArray2[j] == baseArray2[i] &&
                    baseArray3[j] == baseArray3[i] && baseArray4[j] < baseArray4[i] ){

                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    temp = baseArray3[i];
                    baseArray3[i] = baseArray3[j];
                    baseArray3[j] = temp;
                    temp = baseArray4[i];
                    baseArray4[i] = baseArray4[j];
                    baseArray4[j] = temp;
                    pair = primerPairShowList[i];
                    primerPairShowList[i] = primerPairShowList[j];
                    primerPairShowList[j] = pair;
                }
            }

            pDlg->setValue( numPrimerPairs*2 + i );
        }
    }
    delete[] baseArray1;
    delete[] baseArray2;
    delete[] baseArray3;
    delete[] baseArray4;

    delete pDlg;
//    DataTools::addDisplayPrimerList();
}

void DataTools::sortTripleList(QVector<Triple*> &tripleShowList)
{
    unsigned long numTriples = static_cast<unsigned long>(tripleShowList.size());
    Triple *tp;
    float temp;
    float *baseArray1, *baseArray2;

    try{
        baseArray1 = new float[numTriples];
        baseArray2 = new float[numTriples];
    }
    catch( std::bad_alloc &){
        QMessageBox::about(Q_NULLPTR,"Fatal Error","Fatal error: out of memory for sorting.\nProgram is forced to terminate.");
        /*Clean Memory: (for Error)
          1. Target&Nontar Seqs
          2. Primer Candidates
          3. Primer Pairs
          4. probe Candidates
          5. probes
          6. Triples (in Primer Pair)
          7. PrimerShowList
          8. tripleShowList
          9. T & P
          10. Score
        */
        DataTools::cleanMemoryAllData();
        exit(-1);
    }

    MyProgressDlg *pDlg = new MyProgressDlg("Collecting probe information",1, numTriples,Q_NULLPTR);
    pDlg->setWindowModality(Qt::ApplicationModal);

    for( int i = 0; i < numTriples; i++ ){
        baseArray1[i] = getTripleInfo( tripleShowList[i], tripleSortBy[0] );
        baseArray2[i] = getTripleInfo( tripleShowList[i], tripleSortBy[1] );

        pDlg->setValue(i);
    }

    delete pDlg;

    pDlg = new MyProgressDlg("Sorting triple list",1, numTriples*3,Q_NULLPTR);
    pDlg->setWindowModality(Qt::ApplicationModal);

    // ascending & ascending order
    if( tripleSortOrder[0] == ASCENDING && tripleSortOrder[1] == ASCENDING ){
        // insertion sort according to baseArray1, then by baseArray2
        for( int i = 1; i < numTriples; i++ ){
            for( int j = 0; j < i; j++){
                if( baseArray1[j] > baseArray1[i] ){
                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    tp = tripleShowList[i];
                    tripleShowList[i] = tripleShowList[j];
                    tripleShowList[j] = tp;
                }
                else if( baseArray1[j] == baseArray1[i] && baseArray2[j] > baseArray2[i] ){
                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    tp = tripleShowList[i];
                    tripleShowList[i] = tripleShowList[j];
                    tripleShowList[j] = tp;
                }
            }
            pDlg->setValue(i);
        }
    }
    // ascending & descending order
    else if( tripleSortOrder[0] == ASCENDING && tripleSortOrder[1] == DESCENDING ){
        // insertion sort according to baseArray1, then by baseArray2
        for( int i = 1; i < numTriples; i++ ){
            for( int j = 0; j < i; j++){
                if( baseArray1[j] > baseArray1[i] ){
                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    tp = tripleShowList[i];
                    tripleShowList[i] = tripleShowList[j];
                    tripleShowList[j] = tp;
                }
                else if( baseArray1[j] == baseArray1[i] && baseArray2[j] < baseArray2[i] ){
                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    tp = tripleShowList[i];
                    tripleShowList[i] = tripleShowList[j];
                    tripleShowList[j] = tp;
                }
            }
            pDlg->setValue(i);
        }
    }
    // descending & ascending order
    else if( tripleSortOrder[0] == DESCENDING && tripleSortOrder[1] == ASCENDING ){
        // insertion sort according to baseArray1, then by baseArray2
        for( int i = 1; i < numTriples; i++ ){
            for( int j = 0; j < i; j++){
                if( baseArray1[j] < baseArray1[i] ){
                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    tp = tripleShowList[i];
                    tripleShowList[i] = tripleShowList[j];
                    tripleShowList[j] = tp;
                }
                else if( baseArray1[j] == baseArray1[i] && baseArray2[j] > baseArray2[i] ){
                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    tp = tripleShowList[i];
                    tripleShowList[i] = tripleShowList[j];
                    tripleShowList[j] = tp;
                }
            }
            pDlg->setValue(i);
        }
    }
    else{ // descending & descending order
        // insertion sort according to baseArray
        for( int i = 1; i < numTriples; i++ ) {
            for( int j = 0; j < i; j++) {
                if( baseArray1[j] < baseArray1[i] ){
                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    tp = tripleShowList[i];
                    tripleShowList[i] = tripleShowList[j];
                    tripleShowList[j] = tp;
                }
                else if( baseArray1[j] == baseArray1[i] && baseArray2[j] < baseArray2[i] ){
                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    tp = tripleShowList[i];
                    tripleShowList[i] = tripleShowList[j];
                    tripleShowList[j] = tp;
                }
            }
            pDlg->setValue(i);
        }
    }

    // only "sort by", "then by", no "then by", "then by"
    if( tripleSortBy[2] <= 0 && tripleSortBy[3] <= 0 ){

        delete[] baseArray1;
        delete[] baseArray2;

        delete pDlg;

        //DataTools::addDisplayPrimerList();
        return;
    }

    float *baseArray3;
    try{
        baseArray3 = new float[static_cast<unsigned long>(numTriples)];
    }
    catch(  std::bad_alloc &){
        QMessageBox::about(Q_NULLPTR,"Fatal Error","Fatal error: out of memory for sorting.\nProgram is forced to terminate.");
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
        exit(-1);
    }
    for( int i = 0; i < numTriples; i++ )
        baseArray3[i] = getTripleInfo( tripleShowList[i], tripleSortBy[2] );


    // sort by "then by" 2

    // ascending order
    if( tripleSortOrder[2] == ASCENDING ){
        // insertion sort according to baseArray3
        for( int i = 1; i < numTriples; i++ ){
            for( int j = 0; j < i; j++){
                if( baseArray1[j] == baseArray1[i] && baseArray2[j] == baseArray2[i] && baseArray3[j] > baseArray3[i] ){
                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    temp = baseArray3[i];
                    baseArray3[i] = baseArray3[j];
                    baseArray3[j] = temp;
                    tp = tripleShowList[i];
                    tripleShowList[i] = tripleShowList[j];
                    tripleShowList[j] = tp;
                }
            }
            pDlg->setValue(numTriples + i);
        }
    }
    // descending order
    else if( tripleSortOrder[2] == DESCENDING ){
        // insertion sort according to baseArray3
        for( int i = 1; i < numTriples; i++ ){
            for( int j = 0; j < i; j++){
                if( baseArray1[j] == baseArray1[i] && baseArray2[j] == baseArray2[i] && baseArray3[j] < baseArray3[i] ){
                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    temp = baseArray3[i];
                    baseArray3[i] = baseArray3[j];
                    baseArray3[j] = temp;
                    tp = tripleShowList[i];
                    tripleShowList[i] = tripleShowList[j];
                    tripleShowList[j] = tp;
                }
            }
            pDlg->setValue(numTriples + i);
        }
    }

    // "sort by" -> "then by" -> "then by", no more "then by"
    if( tripleSortBy[3] <= 0 ){

        delete[] baseArray1;
        delete[] baseArray2;
        delete[] baseArray3;

        delete pDlg;

        //DataTools::addDisplayPrimerList();
        return;
    }

    float *baseArray4;
    try{
        baseArray4 = new float[static_cast<unsigned long>(numTriples)];
    }
    catch(  std::bad_alloc &){
        QMessageBox::about(Q_NULLPTR,"Fatal Error","Fatal error: out of memory for sorting.\nProgram is forced to terminate.");
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
        exit(-1);
    }

    for( int i = 0; i < numTriples; i++ )
         baseArray4[i] = getTripleInfo( tripleShowList[i], tripleSortBy[3] );


    // sort by "then by" 3

    // ascending order
    if( tripleSortOrder[3] == ASCENDING ){
        // insertion sort according to baseArray4
        for( int i = 1; i < numTriples; i++ ){
            for( int j = 0; j < i; j++){
                if( baseArray1[j] == baseArray1[i] && baseArray2[j] == baseArray2[i] &&
                    baseArray3[j] == baseArray3[i] && baseArray4[j] > baseArray4[i] ){

                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    temp = baseArray3[i];
                    baseArray3[i] = baseArray3[j];
                    baseArray3[j] = temp;
                    temp = baseArray4[i];
                    baseArray4[i] = baseArray4[j];
                    baseArray4[j] = temp;
                    tp = tripleShowList[i];
                    tripleShowList[i] = tripleShowList[j];
                    tripleShowList[j] = tp;
                }
            }

            pDlg->setValue( numTriples*2 + i );
        }
    }
    // descending order
    else if( tripleSortOrder[3] == DESCENDING ){
        // insertion sort according to baseArray4
        for( int i = 1; i < numTriples; i++ ){
            for( int j = 0; j < i; j++){
                if( baseArray1[j] == baseArray1[i] && baseArray2[j] == baseArray2[i] &&
                    baseArray3[j] == baseArray3[i] && baseArray4[j] < baseArray4[i] ){

                    temp = baseArray1[i];
                    baseArray1[i] = baseArray1[j];
                    baseArray1[j] = temp;
                    temp = baseArray2[i];
                    baseArray2[i] = baseArray2[j];
                    baseArray2[j] = temp;
                    temp = baseArray3[i];
                    baseArray3[i] = baseArray3[j];
                    baseArray3[j] = temp;
                    temp = baseArray4[i];
                    baseArray4[i] = baseArray4[j];
                    baseArray4[j] = temp;
                    tp = tripleShowList[i];
                    tripleShowList[i] = tripleShowList[j];
                    tripleShowList[j] = tp;
                }
            }

            pDlg->setValue( numTriples*2 + i );
        }
    }
    delete[] baseArray1;
    delete[] baseArray2;
    delete[] baseArray3;
    delete[] baseArray4;

    delete pDlg;
}



int DataTools::deletePrimerFromPrimerMem(QString primer, QVector<PrimerPair*> &pList, bool destroyMem)
{
        QString p;
        int whichPrimer = 0;
        while( whichPrimer < pList.size() ){
            p = pList[whichPrimer]->left + ".." + pList[whichPrimer]->rightRevsComp;
            if( primer == p )
                break;
            whichPrimer++;
        }
        if( whichPrimer >= pList.size() ){
            QMessageBox::about(Q_NULLPTR,"Error: Primer not found","Error in searching selected primer pair in primer pair memory.\n"
                                    "Please report the problem to the developer.");
            return -1;
        }

        if(destroyMem) // wants to destroy the memory completely
            delete pList[whichPrimer];
        pList.erase( pList.begin() + whichPrimer );

        return 0;
}


int DataTools::deleteTripleFromTripleMem(QString tripleSeq, QVector<Triple*> &pList, bool destroyMem)
{
        QString p;
        int whichTriple = 0;
        while( whichTriple < pList.size() ){
            Triple *tp = pList[whichTriple];
\
            p = tp->Pb->probeSeq;
            if( tripleSeq == p )
                break;
            whichTriple++;
        }
        if( whichTriple >= pList.size() ){
            QMessageBox::about(Q_NULLPTR,"Error: Probe not found","Error in searching selected probe in probe memory.\n"
                                    "Please report the problem to the developer.");
            return -1;
        }

        if(destroyMem) // wants to destroy the memory completely
            delete pList[whichTriple];
        pList.erase( pList.begin() + whichTriple );

        return 0;
}

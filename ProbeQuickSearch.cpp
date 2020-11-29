#include "ProbeQuickSearch.h"

ProbeQuickSearch::ProbeQuickSearch()
{
    allocMemScoringTable();
}

ProbeQuickSearch::~ProbeQuickSearch()
{
    cleanMemoryScoringTable();
}

int ProbeQuickSearch::allocMemScoringTable()
{
    // allocate scoring table and pointer table for probe search
    try {
        T = new float** [MAX_PROBE_LEN/2+1];
        P = new int** [MAX_PROBE_LEN/2+1];
        revT = new float** [MAX_PROBE_LEN/2+1];
        revP = new int** [MAX_PROBE_LEN/2+1];
    }
    catch ( std::bad_alloc& e ) {
        QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for scoring tables.\n Program is forced to terminate");
        exit(1);
    }
    for( int i = 0; i < MAX_PROBE_LEN/2+1; i++ ){
        try {
            T[i] = new float*[MAX_SEQUENCE_LEN];
            P[i] = new int*[MAX_SEQUENCE_LEN];
            revT[i] = new float*[MAX_SEQUENCE_LEN];
            revP[i] = new int*[MAX_SEQUENCE_LEN];
        }
        catch ( std::bad_alloc& e ) {
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for scoring tables.\n Program is forced to terminate");
            exit(1);
        }

        for( int j = 0; j < MAX_SEQUENCE_LEN; j++ ){
            try {
                T[i][j] = new float[maxIndel+1];
                P[i][j] = new int[maxIndel+1];
                revT[i][j] = new float[maxIndel+1];
                revP[i][j] = new int[maxIndel+1];
            }
            catch ( std::bad_alloc& e ) {
                QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for scoring tables.\n Program is forced to terminate");
                exit(1);
            }
        }
    }

    return 0;
}

void ProbeQuickSearch::cleanMemoryScoringTable(void)
{
    for( int i = 0; i < MAX_PROBE_LEN/2+1; i++ ){
        for( int j = 0; j < MAX_SEQUENCE_LEN; j++ ){
            delete[] T[i][j];
            delete[] P[i][j];
            delete[] revT[i][j];
            delete[] revP[i][j];
        }
        delete[] T[i];
        delete[] P[i];
        delete[] revT[i];
        delete[] revP[i];
    }
    delete[] T;
    delete[] P;
    delete[] revT;
    delete[] revP;
}

void ProbeQuickSearch::initMember(short* pr, int prNum, short* prLen)
{
    this->probeNum = prNum;

    if( probeNum <= 0 ){
        QMessageBox::about(0, "Search Error", "Probe quick search starting error.\nProgram is forced to terminate.\nPlease report to developer!");
        exit(-1);
    }
    for( int i = 0; i < probeNum; i++ )
        probesLen[i] = prLen[i];

    int probeLen = probesLen[probeNum-1];

    // right probe: don't reverse
    for(int i = 0; i < probeLen; i++)
        probe[i] = pr[i];
    probe[probeLen] = -1;
}


// Class function caller need to keep in mind that
// sequence need to be 3'-5' for left probe
// and 5'-3' for right probe.
int ProbeQuickSearch::searchProbesInTarSeq(short* seq, short* revSeq)
{
    // get the longest probe length
    int probeLen = probesLen[probeNum-1];
    int posStart, posEnd;
    // get sequence lenth
    int seqLen = 0;
    while(seq[seqLen] >= 0)	seqLen++;
    float minScore;
    //int col, indelIndex;


    int mid = probeLen/2;

    //separate probe into two parts, and reverse the left part
    short* revLeft;
    short* right;

    int lenRevLeft = mid;
    revLeft  = new short[lenRevLeft];

    int lenRight = probeLen - mid;
    right = new short[lenRight];

    for(int i = 0; i< lenRevLeft; i++){
        revLeft[i] = probe[mid-1-i];
    }
    for(int i = 0; i < lenRight; i++){
        right[i] = probe[mid+i];
    }

    // start searching, fill tables
    // 1. Initialization
    for( int i = 0; i < MAX_PROBE_LEN/2+1; i++ ){
        for( int k = 0; k <= maxIndel; k++ ){
            T[i][0][k] = COST_INFINITE;
            P[i][0][k] = -1;
            revT[i][0][k] = COST_INFINITE;
            revP[i][0][k] = -1;
        }
    }
        //  Right part
    T[0][0][0] = simCost[right[0]][seq[0]];
    if( T[0][0][0] > probePosWtTar[0] )	T[0][0][0] = COST_INFINITE; // If exceed the constraint, then restart;
        //  Left part
    revT[0][0][0] = simCost[revLeft[0]][revSeq[0]];
    if( revT[0][0][0] > probePosWtTar[0] )	revT[0][0][0] = COST_INFINITE; // If exceed the constraint, then restart;

    // 2. fill the 1st row
    float tmpT;
    int tmpP;
    short tmpProbe, tmpSeq;
    float tmpScore;
    float tmpWeight = probePosWtTar[0]; //constraint on starting cost

    for( int j = 1; j < seqLen; j++ ){
        //Right part
        tmpProbe = right[0];
        tmpSeq = seq[j];
        tmpT = simCost[tmpProbe][tmpSeq]; // cost for first base in probe v.s. seq[j]
        tmpP = -1;
        if( tmpT > tmpWeight ){  //cannot start the matching.
            tmpT = COST_INFINITE;
            tmpP = -1;
        }
        T[0][j][0] = tmpT;
        P[0][j][0] = tmpP;

        //Left part
        tmpProbe = revLeft[0];
        tmpSeq = revSeq[j];
        tmpT = simCost[tmpProbe][tmpSeq]; // cost for first base in probe v.s. seq[j]
        tmpP = -1;
        if( tmpT > tmpWeight ){  //cannot start the matching.
            tmpT = COST_INFINITE;
            tmpP = -1;
        }
        revT[0][j][0] = tmpT;
        revP[0][j][0] = tmpP;

        for( int k = 1; k <= maxIndel; k++ ){
            //Right part
            tmpT = T[0][j-1][k-1] + indelCost;
            tmpP = 1;
            if( tmpWeight < tmpT ){
                tmpT = COST_INFINITE;
                tmpP = -1;
            }
            T[0][j][k] = tmpT;
            P[0][j][k] = tmpP;

            //Left part
            tmpT = T[0][j-1][k-1] + indelCost;
            tmpP = 1;
            if( tmpT > tmpWeight ){
                tmpT = COST_INFINITE;
                tmpP = -1;
            }
            revT[0][j][k] = tmpT;
            revP[0][j][k] = tmpP;
        }
    }

    // 3. fill the whole table
    for( int i = 1; i < lenRight; i++ ){
        tmpProbe = right[i];
        tmpWeight = probePosWtTar[i];
        for( int j = 1; j < seqLen; j++ ){
            tmpSeq = seq[j];
            tmpScore = simCost[tmpProbe][tmpSeq];
            tmpT = T[i-1][j-1][0] + tmpScore; //  accumulated cost to probe[i] v.s. seq[j]
            tmpP = 0;
            if( tmpT > tmpWeight ){ //exceed the threshold: restart
                tmpT = COST_INFINITE;
                tmpP = -1;
            }
            T[i][j][0] = tmpT;
            P[i][j][0] = tmpP;

            for( int k = 1; k <= maxIndel; k++ ){
                tmpT = T[i-1][j-1][k] + tmpScore;
                tmpP = 0;
                if( tmpT > T[i][j-1][k-1] + indelCost ){
                    tmpT = T[i][j-1][k-1] + indelCost;
                    tmpP = 1;
                }
                if( T[i-1][j][k-1] + indelCost < tmpT ){
                    tmpT = T[i-1][j][k-1] + indelCost;
                    tmpP = 2;
                }

                if( tmpT > tmpWeight ){
                    tmpT = COST_INFINITE;
                    tmpP = -1;
                }
                T[i][j][k] = tmpT;
                P[i][j][k] = tmpP;
            }
        }
    }

    for( int i = 1; i < lenRevLeft; i++ ){
        tmpProbe = revLeft[i];
        tmpWeight = probePosWtTar[i];
        for( int j = 1; j < seqLen; j++ ){
            tmpSeq = revSeq[j];
            tmpScore = simCost[tmpProbe][tmpSeq];
            tmpT = revT[i-1][j-1][0] + tmpScore; //  accumulated cost to probe[i] v.s. seq[j]
            tmpP = 0;
            if( tmpT > tmpWeight ){ //exceed the threshold: restart
                tmpT = COST_INFINITE;
                tmpP = -1;
            }
            revT[i][j][0] = tmpT;
            revP[i][j][0] = tmpP;

            for( int k = 1; k <= maxIndel; k++ ){
                tmpT =revT[i-1][j-1][k] + tmpScore;
                tmpP = 0;

                if(revT[i][j-1][k-1] + indelCost < tmpT ){
                    tmpT = revT[i][j-1][k-1] + indelCost;
                    tmpP = 1;
                }
                if( revT[i-1][j][k-1] + indelCost < tmpT ){
                    tmpT = revT[i-1][j][k-1] + indelCost;
                    tmpP = 2;
                }

                if( tmpT > tmpWeight ){
                    tmpT = COST_INFINITE;
                    tmpP = -1;
                }
                revT[i][j][k] = tmpT;
                revP[i][j][k] = tmpP;
            }
        }
    }

    //Find Best score.
    minScore = FLT_MAX;

    for( int pb = 0; pb < probeNum; pb++ ){

        posStart = -1;
        posEnd = -1;

        probeLen = probesLen[pb];
        lenRevLeft = probesLen[pb]/2;
        lenRight = probesLen[pb] - lenRevLeft;

        for(int k1 = 0; k1 <= maxIndel; k1++){
            for(int k2 = 0; k2 <= maxIndel - k1; k2++){
                for(int j = probeLen-1+k1; j < seqLen; j++){

                    float currScore  = T[lenRight-1][j][0] + revT[lenRevLeft-1][seqLen+probeLen-j+k1+k2-2][0];
                    if (currScore < minScore){
                        minScore = currScore;
                        posStart = j-probeLen-k1-k2+1;
                        posEnd = j;
                    }
                }
            }
        }
        bindingPosStart[pb] = posStart;
        bindingPosEnd[pb] = posEnd;
    }

    delete[] right;
    delete[] revLeft;





/*
    // trace back for probe group
    for( int pr = 0; pr < probeNum; pr++ ){
        probeLen = probesLen[pr];	// start from the pr-th probe
        minScore = FLT_MAX;
        // find the minimum score in the last row of the table: start from end of sequence

        posStart = -1;
        posEnd = -1;
        int jMin = seqLen - 1;

        for( int j = seqLen-1; j >= 0; j-- ){
            for( int k = 0; k <= maxIndel; k++ ){
                float currentScore = T[probeLen-1][j][k];
                if (currentScore < COST_INFINITE){//If it satisfies the throshold: we still need to consider the distance from another direction
                    bool validflag = true;
                    int i = probeLen-1, j1 = j, k1 = k; // tmp vars, for trace back
                    float score2 = 0; //calculate cost from another direction;


                    while( P[i][j1][k1] >= 0 ){
                        int direction =  P[i][j1][k1];
                        if( direction == 0 ){ // diagonal
                            score2 += simCost[seqs[j1]][probe[i]];
                            i--; j1--;
                        }
                        else if( direction == 1 ){ // left
                            score2 += indelCost;
                            j1--; k1--;
                        }
                        else if( direction == 2 ){ // above
                            score2 += indelCost;
                            i--; k1--;
                        }
                        else{
                            QMessageBox::about(0, "Search Error", "Similarity search error 1.2. Please report to the developer!");
                            return -1;
                        }

                        //Check if we need to continue or not.
                        if (score2 > probePosWtNonTar[probeLen-1-i]){
                            validflag = false;
                            break;
                        }
                    }
                    if( validflag == true){
                        if (i != 0 || score2 != currentScore){
                            QMessageBox::about(0, "Search Error", "Similarity search error 2.2. Please report to the developer!");
                            return -1;
                        }
                        if( currentScore < minScore){
                            minScore = currentScore;
                            col = j;
                            jMin = j1;
                            indelIndex = k;
                        }
                    }
                }
            }
        }
        if( minScore < COST_INFINITE ){ // find a match satisfying user specification
            posStart = jMin;
            posEnd = col;
        }else{ // didn't find a match
            posStart = -1;
            posEnd = -1;
        }
        bindingPosStart[pr] = posStart;
        bindingPosEnd[pr] = posEnd;
    }*/
    // finish earching
    return 0;
}

/*
int ProbeQuickSearch::searchProbesInTarSeq2(short* sequence)
{
    // get the longest probe length
    int probeLen = probesLen[probeNum-1];
    int posStart, posEnd;
    // get sequence lenth
    int seqLen = 0;
    while(sequence[seqLen] >= 0)	seqLen++;
    float minScore;
    int col, indelIndex;

    // start searching, fill tables
    // 1. fill the 1st column
    for( int i = 0; i < probeLen; i++ ){
        for( int k = 0; k <= maxIndel; k++ ){
            T[i][0][k] = COST_INFINITE;
            P[i][0][k] = -1;
        }
    }
    T[0][0][0] = simCost[probe[0]][sequence[0]];
    if( probePosWtTar[0] < T[0][0][0] )	T[0][0][0] = COST_INFINITE;
    // 2. fill the 1st row
    float tmpT;
    int tmpP;
    float tmpScore;
    float tmpWeight = probePosWtTar[0];
    short tmpProbe, tmpSeq;
    tmpProbe = probe[0];
    for( int j = 1; j < seqLen; j++ ){
        tmpSeq = sequence[j];
        tmpT = simCost[tmpProbe][tmpSeq];
        tmpP = -1;
        if( tmpWeight < tmpT ){
            tmpT = COST_INFINITE;
            tmpP = -1;
        }
        T[0][j][0] = tmpT;
        P[0][j][0] = tmpP;

        for( int k = 1; k <= maxIndel; k++ ){
            tmpT = T[0][j-1][k-1] + indelCost;
            tmpP = 1;
            if( tmpWeight < tmpT ){
                tmpT = COST_INFINITE;
                tmpP = -1;
            }
            T[0][j][k] = tmpT;
            P[0][j][k] = tmpP;
        }
    }
    // 3. fill the whole table
    for( int i = 1; i < probeLen; i++ ){
        tmpProbe = probe[i];
        tmpWeight = probePosWtTar[i];
        for( int j = 1; j < seqLen; j++ ){
            tmpSeq = sequence[j];
            tmpScore = simCost[tmpProbe][tmpSeq];
            tmpT = T[i-1][j-1][0] + tmpScore;
            tmpP = 0;
            if( tmpWeight < tmpT ){
                tmpT = COST_INFINITE;
                tmpP = -1;
            }
            T[i][j][0] = tmpT;
            P[i][j][0] = tmpP;


            for( int k = 1; k <= maxIndel; k++ ){
                tmpT =T[i-1][j-1][k] + tmpScore;
                tmpP = 0;
                if(T[i][j-1][k-1] + indelCost < tmpT ){
                    tmpT = T[i][j-1][k-1] + indelCost;
                    tmpP = 1;
                }
                if( T[i-1][j][k-1] + indelCost < tmpT ){
                    tmpT = T[i-1][j][k-1] + indelCost;
                    tmpP = 2;
                }

                if( tmpWeight < tmpT ){
                    tmpT = COST_INFINITE;
                    tmpP = -1;
                }
                T[i][j][k] = tmpT;
                P[i][j][k] = tmpP;
            }
        }
    }

    // trace back for probe group
    for( int pr = 0; pr < probeNum; pr++ ){
        probeLen = probesLen[pr];	// start from the pr-th probe
        minScore = FLT_MAX;
        // find the minimum score in the last row of the table: start from end of sequence
        for( int j = seqLen-1; j >= 0; j-- ){
            for( int k = 0; k <= maxIndel; k++ ){
                if( T[probeLen-1][j][k] < minScore ){
                    minScore = T[probeLen-1][j][k];
                    col = j;
                    indelIndex = k;
                }
            }
        }
        // start tracing back
        float b = COST_INFINITE;

        if( minScore < COST_INFINITE ){ // find a match satisfying user specification
            int i = probeLen-1, j = col, k = indelIndex;
            while( P[i][j][k] >= 0 ){
                if( P[i][j][k] == 0 ){ // diagonal
                    i--; j--;
                }
                else if( P[i][j][k] == 1 ){ // left
                    j--; k--;
                }
                else if( P[i][j][k] == 2 ){ // above
                    i--; k--;
                }
                else{
                    QMessageBox::about(0, "Search Error", "Similarity search error 1.2. Please report to the developer!");
                    return -1;
                }
            }
            if( i != 0 ){
                QMessageBox::about(0, "Search Error", "Similarity search error 2.2. Please report to the developer!");
                return -1;
            }
            posStart = j;
            posEnd = col;
        }
        else{ // didn't find a match
            posStart = -1;
            posEnd = -1;
        }

        bindingPosStart[pr] = posStart;
        bindingPosEnd[pr] = posEnd;
    }


    // finish earching
    return 0;
}
*/


// Class function caller need to keep in mind that
// sequence need to be 3'-5' for left probe
// and 5'-3' for right probe.
int ProbeQuickSearch::searchProbesInNonTarSeq(short* seq, short* revSeq)
{
    // get the longest probe length
    int probeLen = probesLen[probeNum-1];
    int posStart, posEnd;
    // get sequence lenth
    int seqLen = 0;
    while(seq[seqLen] >= 0)	seqLen++;
    float minScore;
    //int col, indelIndex;


    int mid = probeLen/2;

    //separate probe into two parts, and reverse the left part
    short* revLeft;
    short* right;

    int lenRevLeft = mid;
    revLeft  = new short[lenRevLeft];

    int lenRight = probeLen - mid;
    right = new short[lenRight];

    for(int i = 0; i< lenRevLeft; i++){
        revLeft[i] = probe[mid-1-i];
    }
    for(int i = 0; i < lenRight; i++){
        right[i] = probe[mid+i];
    }

    // start searching, fill tables
    // 1. Initialization
    for( int i = 0; i < MAX_PROBE_LEN/2+1; i++ ){
        for( int k = 0; k <= maxIndel; k++ ){
            T[i][0][k] = COST_INFINITE;
            P[i][0][k] = -1;
            revT[i][0][k] = COST_INFINITE;
            revP[i][0][k] = -1;
        }
    }
        //  Right part
    T[0][0][0] = simCost[right[0]][seq[0]];
    if( T[0][0][0] > probePosWtNonTar[0] )	T[0][0][0] = COST_INFINITE; // If exceed the constraint, then restart;
        //  Left part
    revT[0][0][0] = simCost[revLeft[0]][revSeq[0]];
    if( revT[0][0][0] > probePosWtNonTar[0] )	revT[0][0][0] = COST_INFINITE; // If exceed the constraint, then restart;

    // 2. fill the 1st row
    float tmpT;
    int tmpP;
    short tmpProbe, tmpSeq;
    float tmpScore;
    float tmpWeight = probePosWtNonTar[0]; //constraint on starting cost

    for( int j = 1; j < seqLen; j++ ){
        //Right part
        tmpProbe = right[0];
        tmpSeq = seq[j];
        tmpT = simCost[tmpProbe][tmpSeq]; // cost for first base in probe v.s. seq[j]
        tmpP = -1;
        if( tmpT > tmpWeight ){  //cannot start the matching.
            tmpT = COST_INFINITE;
            tmpP = -1;
        }
        T[0][j][0] = tmpT;
        P[0][j][0] = tmpP;

        //Left part
        tmpProbe = revLeft[0];
        tmpSeq = revSeq[j];
        tmpT = simCost[tmpProbe][tmpSeq]; // cost for first base in probe v.s. seq[j]
        tmpP = -1;
        if( tmpT > tmpWeight ){  //cannot start the matching.
            tmpT = COST_INFINITE;
            tmpP = -1;
        }
        revT[0][j][0] = tmpT;
        revP[0][j][0] = tmpP;

        for( int k = 1; k <= maxIndel; k++ ){
            //Right part
            tmpT = T[0][j-1][k-1] + indelCost;
            tmpP = 1;
            if( tmpWeight < tmpT ){
                tmpT = COST_INFINITE;
                tmpP = -1;
            }
            T[0][j][k] = tmpT;
            P[0][j][k] = tmpP;

            //Left part
            tmpT = T[0][j-1][k-1] + indelCost;
            tmpP = 1;
            if( tmpT > tmpWeight ){
                tmpT = COST_INFINITE;
                tmpP = -1;
            }
            revT[0][j][k] = tmpT;
            revP[0][j][k] = tmpP;
        }
    }

    // 3. fill the whole table
    for( int i = 1; i < lenRight; i++ ){
        tmpProbe = right[i];
        tmpWeight = probePosWtNonTar[i];
        for( int j = 1; j < seqLen; j++ ){
            tmpSeq = seq[j];
            tmpScore = simCost[tmpProbe][tmpSeq];
            tmpT = T[i-1][j-1][0] + tmpScore; //  accumulated cost to probe[i] v.s. seq[j]
            tmpP = 0;
            if( tmpT > tmpWeight ){ //exceed the threshold: restart
                tmpT = COST_INFINITE;
                tmpP = -1;
            }
            T[i][j][0] = tmpT;
            P[i][j][0] = tmpP;

            for( int k = 1; k <= maxIndel; k++ ){
                tmpT = T[i-1][j-1][k] + tmpScore;
                tmpP = 0;
                if( tmpT > T[i][j-1][k-1] + indelCost ){
                    tmpT = T[i][j-1][k-1] + indelCost;
                    tmpP = 1;
                }
                if( T[i-1][j][k-1] + indelCost < tmpT ){
                    tmpT = T[i-1][j][k-1] + indelCost;
                    tmpP = 2;
                }

                if( tmpT > tmpWeight ){
                    tmpT = COST_INFINITE;
                    tmpP = -1;
                }
                T[i][j][k] = tmpT;
                P[i][j][k] = tmpP;
            }
        }
    }

    for( int i = 1; i < lenRevLeft; i++ ){
        tmpProbe = revLeft[i];
        tmpWeight = probePosWtNonTar[i];
        for( int j = 1; j < seqLen; j++ ){
            tmpSeq = revSeq[j];
            tmpScore = simCost[tmpProbe][tmpSeq];
            tmpT = revT[i-1][j-1][0] + tmpScore; //  accumulated cost to probe[i] v.s. seq[j]
            tmpP = 0;
            if( tmpT > tmpWeight ){ //exceed the threshold: restart
                tmpT = COST_INFINITE;
                tmpP = -1;
            }
            revT[i][j][0] = tmpT;
            revP[i][j][0] = tmpP;

            for( int k = 1; k <= maxIndel; k++ ){
                tmpT =revT[i-1][j-1][k] + tmpScore;
                tmpP = 0;

                if(revT[i][j-1][k-1] + indelCost < tmpT ){
                    tmpT = revT[i][j-1][k-1] + indelCost;
                    tmpP = 1;
                }
                if( revT[i-1][j][k-1] + indelCost < tmpT ){
                    tmpT = revT[i-1][j][k-1] + indelCost;
                    tmpP = 2;
                }

                if( tmpT > tmpWeight ){
                    tmpT = COST_INFINITE;
                    tmpP = -1;
                }
                revT[i][j][k] = tmpT;
                revP[i][j][k] = tmpP;
            }
        }
    }

    //Find Best score.
    minScore = FLT_MAX;

    for( int pb = 0; pb < probeNum; pb++ ){

        posStart = -1;
        posEnd = -1;

        probeLen = probesLen[pb];
        lenRevLeft = probesLen[pb]/2;
        lenRight = probesLen[pb] - lenRevLeft;

        for(int k1 = 0; k1 <= maxIndel; k1++){
            for(int k2 = 0; k2 <= maxIndel - k1; k2++){
                for(int j = probeLen-1+k1; j < seqLen; j++){

                    float currScore  = T[lenRight-1][j][0] + revT[lenRevLeft-1][seqLen+probeLen-j+k1+k2-2][0];
                    if (currScore < minScore){
                        minScore = currScore;
                        posStart = j-probeLen-k1-k2+1;
                        posEnd = j;
                    }
                }
            }
        }
        bindingPosStart[pb] = posStart;
        bindingPosEnd[pb] = posEnd;
    }

    delete[] right;
    delete[] revLeft;
    /*For Probes, both ends are important*/
    // trace back for probe group
    /*for( int pr = 0; pr < probeNum; pr++ ){
        probeLen = probesLen[pr];	// start from the pr-th probe
        minScore = FLT_MAX;

        posStart = -1;
        posEnd = -1;
        int jMin = seqLen - 1;

        // find the minimum score in the last row of the table: start from end of sequence
        for( int j = seqLen-1; j >= 0; j-- ){
            for( int k = 0; k <= maxIndel; k++ ){
                double currentScore = T[probeLen-1][j][k];
                if( currentScore < COST_INFINITE ){ // find a match satisfying user specification
                    bool validflag = true;
                    int i = probeLen-1, j1 = j, k1 = k; // tmp vars, for trace back
                    double score2 = 0; //calculate cost from another direction;

                    while( P[i][j1][k1] >= 0 ){
                        int direction =  P[i][j1][k1];
                        if( direction == 0 ){ // diagonal
                            score2 += simCost[sequence[j1]][probe[i]];
                            i--; j1--;
                        }
                        else if( direction == 1 ){ // left
                            score2 += indelCost;
                            j1--; k1--;
                        }
                        else if( direction == 2 ){ // above
                            score2 += indelCost;
                            i--; k1--;
                        }
                        else{
                            QMessageBox::about(0, "Search Error", "Similarity search error 1.2. Please report to the developer!");
                            return -1;
                        }

                        //Check if we need to continue or not.
                        if (score2 > probePosWtNonTar[probeLen-1-i]){
                            validflag = false;
                            break;
                        }
                    }

                    if(validflag == true){
                        bool isEqual = (score2 == currentScore);
                        if( i!=0 || ! score2==currentScore){
                        //if( i!=0 || !fabs(score2-currentScore)<0.0000){
                            //int isEqual = (score2 == currentScore);
                            //int tmpInt = score2 - currentScore;
                            QString m = QString("score2 =%1 , currentScore =%2 , equal? %3").arg(score2).arg(currentScore).arg(isEqual);
                            QMessageBox::about(0, "Search Error", m);
                            QMessageBox::about(0, "Search Error", "Similarity search error 2.22. Please report to the developer!");
                            return -1;
                        }

                        if( currentScore < minScore){
                            minScore = currentScore;
                            col = j;
                            jMin = j1;
                            indelIndex = k;
                        }
                    }
                }
            }
        }
        if( minScore < COST_INFINITE ){
            posStart = jMin;
            posEnd = col;
        }else{ // didn't find a match
            posStart = -1;
            posEnd = -1;
        }
        bindingPosStart[pr] = posStart;
        bindingPosEnd[pr] = posEnd;
    }*/
    // finish earching
    return 0;
}
/*
int ProbeQuickSearch::searchProbesInNonTarSeq2(short* sequence)
{
    // get the longest probe length
    int probeLen = probesLen[probeNum-1];
    int posStart, posEnd;
    // get sequence lenth
    int seqLen = 0;
    while(sequence[seqLen] >= 0)	seqLen++;
    float minScore;
    int col, indelIndex;

    // start searching, fill tables
    // 1. fill the 1st column
    for( int i = 0; i < probeLen; i++ ){
        for( int k = 0; k <= maxIndel; k++ ){
            T[i][0][k] = COST_INFINITE;
            P[i][0][k] = -1;
        }
    }
    T[0][0][0] = simCost[probe[0]][sequence[0]];
    if( probePosWtNonTar[0] < T[0][0][0] )	T[0][0][0] = COST_INFINITE;
    // 2. fill the 1st row
    float tmpT;
    int tmpP;
    float tmpScore;
    float tmpWeight = probePosWtNonTar[0];
    short tmpProbe, tmpSeq;
    tmpProbe = probe[0];
    for( int j = 1; j < seqLen; j++ ){
        tmpSeq = sequence[j];
        tmpT = simCost[tmpProbe][tmpSeq];
        tmpP = -1;
        if( tmpWeight < tmpT ){
            tmpT = COST_INFINITE;
            tmpP = -1;
        }
        T[0][j][0] = tmpT;
        P[0][j][0] = tmpP;

        for( int k = 1; k <= maxIndel; k++ ){
            tmpT = T[0][j-1][k-1] + indelCost;
            tmpP = 1;
            if( tmpWeight < tmpT ){
                tmpT = COST_INFINITE;
                tmpP = -1;
            }
            T[0][j][k] = tmpT;
            P[0][j][k] = tmpP;
        }
    }
    // 3. fill the whole table
    for( int i = 1; i < probeLen; i++ ){
        tmpProbe = probe[i];
        tmpWeight = probePosWtNonTar[i];
        for( int j = 1; j < seqLen; j++ ){
            tmpSeq = sequence[j];
            tmpScore = simCost[tmpProbe][tmpSeq];
            tmpT = T[i-1][j-1][0] + tmpScore;
            tmpP = 0;
            if( tmpWeight < tmpT ){
                tmpT = COST_INFINITE;
                tmpP = -1;
            }
            T[i][j][0] = tmpT;
            P[i][j][0] = tmpP;

            for( int k = 1; k <= maxIndel; k++ ){
                tmpT = T[i-1][j-1][k] + tmpScore;
                tmpP = 0;
                if( T[i][j-1][k-1] + indelCost < tmpT ){
                    tmpT = T[i][j-1][k-1] + indelCost;
                    tmpP = 1;
                }
                if( T[i-1][j][k-1] + indelCost < tmpT ){
                    tmpT = T[i-1][j][k-1] + indelCost;
                    tmpP = 2;
                }

                if( tmpWeight < tmpT ){
                    tmpT = COST_INFINITE;
                    tmpP = -1;
                }
                T[i][j][k] = tmpT;
                P[i][j][k] = tmpP;
            }
        }
    }

    // trace back for probe group
    for( int pr = 0; pr < probeNum; pr++ ){
        probeLen = probesLen[pr];	// start from the pr-th probe
        minScore = FLT_MAX;
        // find the minimum score in the last row of the table: start from end of sequence
        for( int j = seqLen-1; j >= 0; j-- ){
            for( int k = 0; k <= maxIndel; k++ ){
                if( T[probeLen-1][j][k] < minScore ){
                    minScore = T[probeLen-1][j][k];
                    col = j;
                    indelIndex = k;
                }
            }
        }
        // start tracing back
        // start tracing back
        if( minScore < COST_INFINITE ){ // find a match satisfying user specification
            int i = probeLen-1, j = col, k = indelIndex;
            while( P[i][j][k] >= 0 ){
                    if( P[i][j][k] == 0 ){ // diagonal
                        i--; j--;
                    }
                    else if( P[i][j][k] == 1 ){ // left
                        j--; k--;
                    }
                    else if( P[i][j][k] == 2 ){ // above
                        i--; k--;
                    }
                    else{
                        QMessageBox::about(0, "Search Error", "Similarity search error 1.2. Please report to the developer!");
                        return -1;
                    }
            }
            if( i != 0 ){
                QMessageBox::about(0, "Search Error", "Similarity search error 2.2. Please report to the developer!");
                return -1;
            }
            posStart = j;
            posEnd = col;
        }
        else{ // didn't find a match
            posStart = -1;
            posEnd = -1;
        }

        bindingPosStart[pr] = posStart;
        bindingPosEnd[pr] = posEnd;
    }
    // finish earching
    return 0;
}
*/

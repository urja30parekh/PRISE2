#include "PrimerQuickSearch.h"

PrimerQuickSearch::PrimerQuickSearch()
{
    allocMemScoringTable();
}

PrimerQuickSearch::~PrimerQuickSearch()
{
    cleanMemoryScoringTable();
}

int PrimerQuickSearch::allocMemScoringTable()
{
    // allocate scoring table and pointer table for primer search
    try {
        T = new float** [MAX_PRIMER_LEN];
        P = new int** [MAX_PRIMER_LEN];
    }
    catch ( std::bad_alloc& e ) {
        QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for scoring tables.\n Program is forced to terminate");
        exit(1);
    }
    for( int i = 0; i < MAX_PRIMER_LEN; i++ ){
        try {
            T[i] = new float*[MAX_SEQUENCE_LEN];
            P[i] = new int*[MAX_SEQUENCE_LEN];
        }
        catch ( std::bad_alloc& e ) {
            QMessageBox::about(0, "Fatal Error", "Fatal error: out of memory for scoring tables.\n Program is forced to terminate");
            exit(1);
        }

        for( int j = 0; j < MAX_SEQUENCE_LEN; j++ ){
            T[i][j] = new float[maxIndel+1];
            P[i][j] = new int[maxIndel+1];
        }
    }
    return 0;
}

void PrimerQuickSearch::cleanMemoryScoringTable(void)
{
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

void PrimerQuickSearch::initMember(short* pr, int prNum, short* prLen, int lft_rht)
{
    this->primerNum = prNum;

    if( primerNum <= 0 ){
        QMessageBox::about(0, "Search Error", "Primer quick search starting error.\nProgram is forced to terminate.\nPlease report to developer!");
        exit(-1);
    }
    for( int i = 0; i < primerNum; i++ )
        primersLen[i] = prLen[i];
    this->lft_rht = lft_rht;

    int primerLen = primersLen[primerNum-1];
    if( this->lft_rht == LEFT_PRIMER ){
        // left primer: reverse primer to make it 3'-5'
        for(int i = 0, j = primerLen-1; i < primerLen; i++, j--)
            primer[i] = pr[j];
        primer[primerLen] = -1;
    }
    else if( lft_rht == RIGHT_PRIMER ){
        // right primer: don't reverse
        for(int i = 0; i < primerLen; i++)
            primer[i] = pr[i];
        primer[primerLen] = -1;
    }
}


// Class function caller need to keep in mind that
// sequence need to be 3'-5' for left primer
// and 5'-3' for right primer.
int PrimerQuickSearch::searchPrimersInTarSeq(short* sequence)
{
    // get the longest primer length
    int primerLen = primersLen[primerNum-1];
    int posStart, posEnd;
    // get sequence lenth
    int seqLen = 0;
    while(sequence[seqLen] >= 0)	seqLen++;
    float minScore;
    int col, indelIndex;

    // start searching, fill tables
    // 1. fill the 1st column
    for( int i = 0; i < primerLen; i++ ){
        for( int k = 0; k <= maxIndel; k++ ){
            T[i][0][k] = COST_INFINITE;
            P[i][0][k] = -1;
        }
    }
    T[0][0][0] = simCost[primer[0]][sequence[0]];
    if( primerPosWtTar[0] < T[0][0][0] )	T[0][0][0] = COST_INFINITE;
    // 2. fill the 1st row
    float tmpT;
    int tmpP;
    float tmpScore;
    float tmpWeight = primerPosWtTar[0];
    short tmpPrimer, tmpSeq;
    tmpPrimer = primer[0];
    for( int j = 1; j < seqLen; j++ ){
        tmpSeq = sequence[j];
        tmpT = simCost[tmpPrimer][tmpSeq];
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
    for( int i = 1; i < primerLen; i++ ){
        tmpPrimer = primer[i];
        tmpWeight = primerPosWtTar[i];
        for( int j = 1; j < seqLen; j++ ){
            tmpSeq = sequence[j];
            tmpScore = simCost[tmpPrimer][tmpSeq];
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

    // trace back for primer group
    for( int pr = 0; pr < primerNum; pr++ ){
        primerLen = primersLen[pr];	// start from the pr-th primer
        minScore = FLT_MAX;
        // find the minimum score in the last row of the table: start from end of sequence
        for( int j = seqLen-1; j >= 0; j-- ){
            for( int k = 0; k <= maxIndel; k++ ){
                if( T[primerLen-1][j][k] < minScore ){
                    minScore = T[primerLen-1][j][k];
                    col = j;
                    indelIndex = k;
                }
            }
        }
        // start tracing back
        float b = COST_INFINITE;

        if( minScore < COST_INFINITE ){ // find a match satisfying user specification
            int i = primerLen-1, j = col, k = indelIndex;
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
            if( lft_rht == LEFT_PRIMER ){
                posStart = seqLen - 1 - col;
                posEnd = seqLen - 1 - j;
            }
            else if( lft_rht == RIGHT_PRIMER ){
                posStart = j;
                posEnd = col;
            }
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


/*
// Class function caller need to keep in mind that
// sequence need to be 3'-5' for left primer
// and 5'-3' for right primer.
int PrimerQuickSearch::searchPrimersInTarSeq(short* sequence)
{
    // get the longest primer length
    int primerLen = primersLen[primerNum-1];
    int posStart, posEnd;
    // get sequence lenth
    int seqLen = 0;
    while(sequence[seqLen] >= 0)	seqLen++;
    float minScore;
    int col, indelIndex;

    // start searching, fill tables
    // 1. fill the 1st column
    for( int i = 0; i < primerLen; i++ ){
        for( int k = 0; k <= maxIndel; k++ ){
            T[i][0][k] = COST_INFINITE;
            P[i][0][k] = -1;
        }
    }
    T[0][0][0] = simCost[primer[0]][sequence[0]];
    if( posWtTar[0] < T[0][0][0] )	T[0][0][0] = COST_INFINITE;
    // 2. fill the 1st row
    for( int j = 1; j < seqLen; j++ ){
        T[0][j][0] = simCost[primer[0]][sequence[j]];
        P[0][j][0] = -1;
        if( posWtTar[0] < T[0][j][0] ){
            T[0][j][0] = COST_INFINITE;
            P[0][j][0] = -1;
        }

        for( int k = 1; k <= maxIndel; k++ ){
            T[0][j][k] = T[0][j-1][k-1] + indelCost;
            P[0][j][k] = 1;
            if( posWtTar[0] < T[0][j][k] ){
                T[0][j][k] = COST_INFINITE;
                P[0][j][k] = -1;
            }
        }
    }
    // 3. fill the whole table
    for( int i = 1; i < primerLen; i++ ){
        for( int j = 1; j < seqLen; j++ ){
            T[i][j][0] = T[i-1][j-1][0] + simCost[primer[i]][sequence[j]];
            P[i][j][0] = 0;
            if( posWtTar[i] < T[i][j][0] ){
                T[i][j][0] = COST_INFINITE;
                P[i][j][0] = -1;
            }

            for( int k = 1; k <= maxIndel; k++ ){
                T[i][j][k] =T[i-1][j-1][k] + simCost[primer[i]][sequence[j]];
                P[i][j][k] = 0;
                if(T[i][j-1][k-1] + indelCost < T[i][j][k] ){
                    T[i][j][k] = T[i][j-1][k-1] + indelCost;
                    P[i][j][k] = 1;
                }
                if( T[i-1][j][k-1] + indelCost < T[i][j][k] ){
                    T[i][j][k] = T[i-1][j][k-1] + indelCost;
                    P[i][j][k] = 2;
                }

                if( posWtTar[i] < T[i][j][k] ){
                    T[i][j][k] = COST_INFINITE;
                    P[i][j][k] = -1;
                }
            }
        }
    }

    // trace back for primer group
    for( int pr = 0; pr < primerNum; pr++ ){
        primerLen = primersLen[pr];	// start from the pr-th primer
        minScore = FLT_MAX;
        // find the minimum score in the last row of the table: start from end of sequence
        for( int j = seqLen-1; j >= 0; j-- ){
            for( int k = 0; k <= maxIndel; k++ ){
                if( T[primerLen-1][j][k] < minScore ){
                    minScore = T[primerLen-1][j][k];
                    col = j;
                    indelIndex = k;
                }
            }
        }
        // start tracing back
        float b = COST_INFINITE;

        if( minScore < COST_INFINITE ){ // find a match satisfying user specification
            int i = primerLen-1, j = col, k = indelIndex;
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
            if( lft_rht == LEFT_PRIMER ){
                posStart = seqLen - 1 - col;
                posEnd = seqLen - 1 - j;
            }
            else if( lft_rht == RIGHT_PRIMER ){
                posStart = j;
                posEnd = col;
            }
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
}*/


// Class function caller need to keep in mind that
// sequence need to be 3'-5' for left primer
// and 5'-3' for right primer.
int PrimerQuickSearch::searchPrimersInNonTarSeq(short* sequence)
{
    // get the longest primer length
    int primerLen = primersLen[primerNum-1];
    int posStart, posEnd;
    // get sequence lenth
    int seqLen = 0;
    while(sequence[seqLen] >= 0)	seqLen++;
    float minScore;
    int col, indelIndex;

    // start searching, fill tables
    // 1. fill the 1st column
    for( int i = 0; i < primerLen; i++ ){
        for( int k = 0; k <= maxIndel; k++ ){
            T[i][0][k] = COST_INFINITE;
            P[i][0][k] = -1;
        }
    }
    T[0][0][0] = simCost[primer[0]][sequence[0]];
    if( primerPosWtNonTar[0] < T[0][0][0] )	T[0][0][0] = COST_INFINITE;
    // 2. fill the 1st row
    float tmpT;
    int tmpP;
    float tmpScore;
    float tmpWeight = primerPosWtNonTar[0];
    short tmpPrimer, tmpSeq;
    tmpPrimer = primer[0];
    for( int j = 1; j < seqLen; j++ ){
        tmpSeq = sequence[j];
        tmpT = simCost[tmpPrimer][tmpSeq];
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
    for( int i = 1; i < primerLen; i++ ){
        tmpPrimer = primer[i];
        tmpWeight = primerPosWtNonTar[i];
        for( int j = 1; j < seqLen; j++ ){
            tmpSeq = sequence[j];
            tmpScore = simCost[tmpPrimer][tmpSeq];
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

    // trace back for primer group
    for( int pr = 0; pr < primerNum; pr++ ){
        primerLen = primersLen[pr];	// start from the pr-th primer
        minScore = FLT_MAX;
        // find the minimum score in the last row of the table: start from end of sequence
        for( int j = seqLen-1; j >= 0; j-- ){
            for( int k = 0; k <= maxIndel; k++ ){
                if( T[primerLen-1][j][k] < minScore ){
                    minScore = T[primerLen-1][j][k];
                    col = j;
                    indelIndex = k;
                }
            }
        }
        // start tracing back
        // start tracing back
        if( minScore < COST_INFINITE ){ // find a match satisfying user specification
            int i = primerLen-1, j = col, k = indelIndex;
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
            if( lft_rht == LEFT_PRIMER ){
                posStart = seqLen - 1 - col;
                posEnd = seqLen - 1 - j;
            }
            else if( lft_rht == RIGHT_PRIMER ){
                posStart = j;
                posEnd = col;
            }
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


/*
// Class function caller need to keep in mind that
// sequence need to be 3'-5' for left primer
// and 5'-3' for right primer.
int PrimerQuickSearch::searchPrimersInNonTarSeq(short* sequence)
{
    // get the longest primer length
    int primerLen = primersLen[primerNum-1];
    int posStart, posEnd;
    // get sequence lenth
    int seqLen = 0;
    while(sequence[seqLen] >= 0)	seqLen++;
    float minScore;
    int col, indelIndex;

    // start searching, fill tables
    // 1. fill the 1st column
    for( int i = 0; i < primerLen; i++ ){
        for( int k = 0; k <= maxIndel; k++ ){
            T[i][0][k] = COST_INFINITE;
            P[i][0][k] = -1;
        }
    }
    T[0][0][0] = simCost[primer[0]][sequence[0]];
    if( posWtNonTar[0] < T[0][0][0] )	T[0][0][0] = COST_INFINITE;
    // 2. fill the 1st row
    for( int j = 1; j < seqLen; j++ ){
        T[0][j][0] = simCost[primer[0]][sequence[j]];
        P[0][j][0] = -1;
        if( posWtNonTar[0] < T[0][j][0] ){
            T[0][j][0] = COST_INFINITE;
            P[0][j][0] = -1;
        }

        for( int k = 1; k <= maxIndel; k++ ){
            T[0][j][k] = T[0][j-1][k-1] + indelCost;
            P[0][j][k] = 1;
            if( posWtNonTar[0] < T[0][j][k] ){
                T[0][j][k] = COST_INFINITE;
                P[0][j][k] = -1;
            }
        }
    }
    // 3. fill the whole table
    for( int i = 1; i < primerLen; i++ ){
        for( int j = 1; j < seqLen; j++ ){
            T[i][j][0] = T[i-1][j-1][0] + simCost[primer[i]][sequence[j]];
            P[i][j][0] = 0;
            if( posWtNonTar[i] < T[i][j][0] ){
                T[i][j][0] = COST_INFINITE;
                P[i][j][0] = -1;
            }

            for( int k = 1; k <= maxIndel; k++ ){
                T[i][j][k] = T[i-1][j-1][k] + simCost[primer[i]][sequence[j]];
                P[i][j][k] = 0;
                if( T[i][j-1][k-1] + indelCost < T[i][j][k] ){
                    T[i][j][k] = T[i][j-1][k-1] + indelCost;
                    P[i][j][k] = 1;
                }
                if( T[i-1][j][k-1] + indelCost < T[i][j][k] ){
                    T[i][j][k] = T[i-1][j][k-1] + indelCost;
                    P[i][j][k] = 2;
                }

                if( posWtNonTar[i] < T[i][j][k] ){
                    T[i][j][k] = COST_INFINITE;
                    P[i][j][k] = -1;
                }
            }
        }
    }

    // trace back for primer group
    for( int pr = 0; pr < primerNum; pr++ ){
        primerLen = primersLen[pr];	// start from the pr-th primer
        minScore = FLT_MAX;
        // find the minimum score in the last row of the table: start from end of sequence
        for( int j = seqLen-1; j >= 0; j-- ){
            for( int k = 0; k <= maxIndel; k++ ){
                if( T[primerLen-1][j][k] < minScore ){
                    minScore = T[primerLen-1][j][k];
                    col = j;
                    indelIndex = k;
                }
            }
        }
        // start tracing back
        if( minScore < COST_INFINITE ){ // find a match satisfying user specification
            int i = primerLen-1, j = col, k = indelIndex;
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
            if( lft_rht == LEFT_PRIMER ){
                posStart = seqLen - 1 - col;
                posEnd = seqLen - 1 - j;
            }
            else if( lft_rht == RIGHT_PRIMER ){
                posStart = j;
                posEnd = col;
            }
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

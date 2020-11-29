#include "PrimerSearch.h"

PrimerSearch::PrimerSearch(short* pr, int lft_rht)
{
    allocMemScoringTable();
    primerLen = 0;
    while(pr[primerLen] >= 0)	primerLen++;

    if( lft_rht == LEFT_PRIMER ){
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

PrimerSearch::~PrimerSearch(void)
{
    bindingPosStartTarget.clear();
    bindingPosEndTarget.clear();
    bindingPosStartNonTar.clear();
    bindingPosEndNonTar.clear();
    cleanMemoryScoringTable();
}


int PrimerSearch::allocMemScoringTable()
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
            T[i][j] = new float[4];
            P[i][j] = new int[4];
        }
    }
    return 0;
}

void PrimerSearch::cleanMemoryScoringTable(void)
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



int PrimerSearch::searchPrimerInTarSeqs(QVector<short*> sequences, int lft_rht)
{
    bindingPosStartTarget.clear();
    bindingPosEndTarget.clear();

    int numSeqs = (int)sequences.size();
    for( int i = 0; i < numSeqs; i++ ){
        int bindingPosStart = 0, bindingPosEnd = 0;
        if( appxSearchTarSeqs(sequences[i], lft_rht, bindingPosStart, bindingPosEnd) < 0 )
            return -1;
        bindingPosStartTarget.push_back(bindingPosStart);
        bindingPosEndTarget.push_back(bindingPosEnd);
    }
    return 0;
}

int PrimerSearch::searchPrimerInNonTarSeqs(QVector<short*> sequences, int lft_rht)
{
    bindingPosStartNonTar.clear();
    bindingPosEndNonTar.clear();

    int numSeqs = (int)sequences.size();
    for( int i = 0; i < numSeqs; i++ ){
        int bindingPosStart = 0, bindingPosEnd = 0;
        if( appxSearchNonTarSeqs(sequences[i], lft_rht, bindingPosStart, bindingPosEnd) < 0 )
            return -1;
        bindingPosStartNonTar.push_back(bindingPosStart);
        bindingPosEndNonTar.push_back(bindingPosEnd);
    }
    return 0;
}


int PrimerSearch::appxSearchTarSeqs(short* seq, int lft_rht, int& bindingPosStart, int& bindingPosEnd)
{
    int seqLen = 0;
    // get sequence length
    while(seq[seqLen] >= 0)	seqLen++;
    int *sequence = new int[seqLen+1];
    if( lft_rht == LEFT_PRIMER ){
        // left primer: copy sequence and make it 3'-5'
        for(int i = 0, j = seqLen-1; i < seqLen; i++, j--)
            sequence[i] = seq[j];
        sequence[seqLen] = -1;
    }
    else if( lft_rht == RIGHT_PRIMER ){
        // right primer: don't reverse
        for(int i = 0; i < seqLen; i++)
            sequence[i] = seq[i];
        sequence[seqLen] = -1;
    }


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
    for( int j = 1; j < seqLen; j++ ){
        T[0][j][0] = simCost[primer[0]][sequence[j]];
        P[0][j][0] = -1;
        if( primerPosWtTar[0] < T[0][j][0] ){
            T[0][j][0] = COST_INFINITE;
            P[0][j][0] = -1;
        }

        for( int k = 1; k <= maxIndel; k++ ){
            T[0][j][k] = T[0][j-1][k-1] + indelCost;
            P[0][j][k] = 1;
            if( primerPosWtTar[0] < T[0][j][k] ){
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
            if( primerPosWtTar[i] < T[i][j][0] ){
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

                if( primerPosWtTar[i] < T[i][j][k] ){
                    T[i][j][k] = COST_INFINITE;
                    P[i][j][k] = -1;
                }
            }
        }
    }

    // trace back for primer
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
                QMessageBox::about(0,"Search Error","Similarity search error 1.2. Please report to the developer!");
                return -1;
            }
        }
        if( i != 0 ){
            QMessageBox::about(0,"Search Error","Similarity search error 2.2. Please report to the developer!");
            return -1;
        }
        if( lft_rht == LEFT_PRIMER ){
            bindingPosStart = seqLen - 1 - col;
            bindingPosEnd = seqLen - 1 - j;
        }
        else if( lft_rht == RIGHT_PRIMER ){
            bindingPosStart = j;
            bindingPosEnd = col;
        }
    }
    else{ // didn't find a match
        bindingPosStart = -1;
        bindingPosEnd = -1;
    }


    // finish searching
    delete[] sequence;
    return 0;
}


int PrimerSearch::appxSearchNonTarSeqs(short* seq, int lft_rht, int& bindingPosStart, int& bindingPosEnd)
{
    int seqLen = 0;
    // get sequence length
    while(seq[seqLen] >= 0)	seqLen++;
    int *sequence = new int[seqLen+1];
    if( lft_rht == LEFT_PRIMER ){
        // left primer: copy sequence and make it 3'-5'
        for(int i = 0, j = seqLen-1; i < seqLen; i++, j--)
            sequence[i] = seq[j];
        sequence[seqLen] = -1;
    }
    else if( lft_rht == RIGHT_PRIMER ){
        // right primer: don't reverse
        for(int i = 0; i < seqLen; i++)
            sequence[i] = seq[i];
        sequence[seqLen] = -1;
    }


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
    for( int j = 1; j < seqLen; j++ ){
        T[0][j][0] = simCost[primer[0]][sequence[j]];
        P[0][j][0] = -1;
        if( primerPosWtNonTar[0] < T[0][j][0] ){
            T[0][j][0] = COST_INFINITE;
            P[0][j][0] = -1;
        }

        for( int k = 1; k <= maxIndel; k++ ){
            T[0][j][k] = T[0][j-1][k-1] + indelCost;
            P[0][j][k] = 1;
            if( primerPosWtNonTar[0] < T[0][j][k] ){
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
            if( primerPosWtNonTar[i] < T[i][j][0] ){
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

                if( primerPosWtNonTar[i] < T[i][j][k] ){
                    T[i][j][k] = COST_INFINITE;
                    P[i][j][k] = -1;
                }
            }
        }
    }


    // trace back for primer
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
                    QMessageBox::about(0,"Search Error","Similarity search error 1.2. Please report to the developer!");
                    return -1;
                }
            }
            if( i != 0 ){
                QMessageBox::about(0,"Search Error","Similarity search error 2.2. Please report to the developer!");
                return -1;
            }
            if( lft_rht == LEFT_PRIMER ){
                bindingPosStart = seqLen - 1 - col;
                bindingPosEnd = seqLen - 1 - j;
            }
            else if( lft_rht == RIGHT_PRIMER ){
                bindingPosStart = j;
                bindingPosEnd = col;
            }
    }
    else{ // didn't find a match
        bindingPosStart = -1;
        bindingPosEnd = -1;
    }


    // finish searching
    delete[] sequence;
    return 0;
}

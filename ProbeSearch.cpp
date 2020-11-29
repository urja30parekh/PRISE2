#include "ProbeSearch.h"

ProbeSearch::ProbeSearch(short* pr)
{
    probeLen = 0;
    while(pr[probeLen] >= 0)	probeLen++;

    for(int i = 0; i < probeLen; i++)
        probe[i] = pr[i];
    probe[probeLen] = -1;
}

ProbeSearch::~ProbeSearch(void)
{
    bindingPosStartTarget.clear();
    bindingPosEndTarget.clear();
    bindingPosStartNonTar.clear();
    bindingPosEndNonTar.clear();
}



int ProbeSearch::searchProbeInTarSeqs(QVector<short*> sequences)
{
    bindingPosStartTarget.clear();
    bindingPosEndTarget.clear();

    int numSeqs = (int)sequences.size();
    for( int i = 0; i < numSeqs; i++ ){
        int bindingPosStart = 0, bindingPosEnd = 0;
        if( appxSearchTarSeqs(sequences[i], bindingPosStart, bindingPosEnd) < 0 )
            return -1;
        bindingPosStartTarget.push_back(bindingPosStart);
        bindingPosEndTarget.push_back(bindingPosEnd);
    }
    return 0;
}

int ProbeSearch::searchProbeInNonTarSeqs(QVector<short*> sequences)
{
    bindingPosStartNonTar.clear();
    bindingPosEndNonTar.clear();

    int numSeqs = (int)sequences.size();
    for( int i = 0; i < numSeqs; i++ ){
        int bindingPosStart = 0, bindingPosEnd = 0;
        if( appxSearchNonTarSeqs(sequences[i], bindingPosStart, bindingPosEnd) < 0 )
            return -1;
        bindingPosStartNonTar.push_back(bindingPosStart);
        bindingPosEndNonTar.push_back(bindingPosEnd);
    }
    return 0;
}


int ProbeSearch::appxSearchTarSeqs(short* seq, int& bindingPosStart, int& bindingPosEnd)
{
    int seqLen = 0;
    // get sequence length
    while(seq[seqLen] >= 0)	seqLen++;
    int *sequence = new int[seqLen+1];

    for(int i = 0; i < seqLen; i++)
        sequence[i] = seq[i];
    sequence[seqLen] = -1;


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
    for( int j = 1; j < seqLen; j++ ){
        T[0][j][0] = simCost[probe[0]][sequence[j]];
        P[0][j][0] = -1;
        if( probePosWtTar[0] < T[0][j][0] ){
            T[0][j][0] = COST_INFINITE;
            P[0][j][0] = -1;
        }

        for( int k = 1; k <= maxIndel; k++ ){
            T[0][j][k] = T[0][j-1][k-1] + indelCost;
            P[0][j][k] = 1;
            if( probePosWtTar[0] < T[0][j][k] ){
                T[0][j][k] = COST_INFINITE;
                P[0][j][k] = -1;
            }
        }
    }
    // 3. fill the whole table
    for( int i = 1; i < probeLen; i++ ){
        for( int j = 1; j < seqLen; j++ ){
            T[i][j][0] = T[i-1][j-1][0] + simCost[probe[i]][sequence[j]];
            P[i][j][0] = 0;
            if( probePosWtTar[i] < T[i][j][0] ){
                T[i][j][0] = COST_INFINITE;
                P[i][j][0] = -1;
            }

            for( int k = 1; k <= maxIndel; k++ ){
                T[i][j][k] = T[i-1][j-1][k] + simCost[probe[i]][sequence[j]];
                P[i][j][k] = 0;
                if( T[i][j-1][k-1] + indelCost < T[i][j][k] ){
                    T[i][j][k] = T[i][j-1][k-1] + indelCost;
                    P[i][j][k] = 1;
                }
                if( T[i-1][j][k-1] + indelCost < T[i][j][k] ){
                    T[i][j][k] = T[i-1][j][k-1] + indelCost;
                    P[i][j][k] = 2;
                }

                if( probePosWtTar[i] < T[i][j][k] ){
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
            if( T[probeLen-1][j][k] < minScore ){
                minScore = T[probeLen-1][j][k];
                col = j;
                indelIndex = k;
            }
        }
    }
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
                QMessageBox::about(0,"Search Error","Similarity search error 1.2. Please report to the developer!");
                return -1;
            }
        }
        if( i != 0 ){
            QMessageBox::about(0,"Search Error","Similarity search error 2.2. Please report to the developer!");
            return -1;
        }

        bindingPosStart = j;
        bindingPosEnd = col;
    }
    else{ // didn't find a match
        bindingPosStart = -1;
        bindingPosEnd = -1;
    }


    // finish searching
    delete[] sequence;
    return 0;
}


int ProbeSearch::appxSearchNonTarSeqs(short* seq, int& bindingPosStart, int& bindingPosEnd)
{
    int seqLen = 0;
    // get sequence length
    while(seq[seqLen] >= 0)	seqLen++;
    int *sequence = new int[seqLen+1];

    for(int i = 0; i < seqLen; i++)
        sequence[i] = seq[i];
    sequence[seqLen] = -1;


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
    for( int j = 1; j < seqLen; j++ ){
        T[0][j][0] = simCost[probe[0]][sequence[j]];
        P[0][j][0] = -1;
        if( probePosWtNonTar[0] < T[0][j][0] ){
            T[0][j][0] = COST_INFINITE;
            P[0][j][0] = -1;
        }

        for( int k = 1; k <= maxIndel; k++ ){
            T[0][j][k] = T[0][j-1][k-1] + indelCost;
            P[0][j][k] = 1;
            if( probePosWtNonTar[0] < T[0][j][k] ){
                T[0][j][k] = COST_INFINITE;
                P[0][j][k] = -1;
            }
        }
    }
    // 3. fill the whole table
    for( int i = 1; i < probeLen; i++ ){
        for( int j = 1; j < seqLen; j++ ){
            T[i][j][0] = T[i-1][j-1][0] + simCost[probe[i]][sequence[j]];
            P[i][j][0] = 0;
            if( probePosWtNonTar[i] < T[i][j][0] ){
                T[i][j][0] = COST_INFINITE;
                P[i][j][0] = -1;
            }

            for( int k = 1; k <= maxIndel; k++ ){
                T[i][j][k] = T[i-1][j-1][k] + simCost[probe[i]][sequence[j]];
                P[i][j][k] = 0;
                if( T[i][j-1][k-1] + indelCost < T[i][j][k] ){
                    T[i][j][k] = T[i][j-1][k-1] + indelCost;
                    P[i][j][k] = 1;
                }
                if( T[i-1][j][k-1] + indelCost < T[i][j][k] ){
                    T[i][j][k] = T[i-1][j][k-1] + indelCost;
                    P[i][j][k] = 2;
                }

                if( probePosWtNonTar[i] < T[i][j][k] ){
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
            if( T[probeLen-1][j][k] < minScore ){
                minScore = T[probeLen-1][j][k];
                col = j;
                indelIndex = k;
            }
        }
    }
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
                    QMessageBox::about(0,"Search Error","Similarity search error 1.2. Please report to the developer!");
                    return -1;
                }
            }
            if( i != 0 ){
                QMessageBox::about(0,"Search Error","Similarity search error 2.2. Please report to the developer!");
                return -1;
            }


            bindingPosStart = j;
            bindingPosEnd = col;

    }
    else{ // didn't find a match
        bindingPosStart = -1;
        bindingPosEnd = -1;
    }


    // finish searching
    delete[] sequence;
    return 0;
}


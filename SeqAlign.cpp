#include "SeqAlign.h"

SeqAlign::SeqAlign(int seqLen)
{
    maxSeqLen = seqLen+2;

    // allocate memory for score tables
    S = new short*[maxSeqLen];
    D = new short*[maxSeqLen];
    I = new short*[maxSeqLen];
    P = new short*[maxSeqLen];
    for( int i = 0; i < maxSeqLen; i++ ){
        S[i] = new short[maxSeqLen];
        D[i] = new short[maxSeqLen];
        I[i] = new short[maxSeqLen];
        P[i] = new short[maxSeqLen];
    }
}

SeqAlign::~SeqAlign(void)
{   // clean memory
    for( int i = 0; i < maxSeqLen; i++ ){
        delete[] S[i];
        delete[] D[i];
        delete[] I[i];
        delete[] P[i];
    }
    delete[] S;
    delete[] D;
    delete[] I;
    delete[] P;
}

void SeqAlign::initializeScore()
{
    for( int i = 0; i < 15; i++ ){
        for( int j = 0; j < 15; j++ )
            score[i][j] = 0 - mismatchPenalty;
        score[i][i] = 1;
    }
}

// pairwise alignment using Needleman-Wunsch algorithm with end gap free panelty
// return sequence identity percentage
float SeqAlign::NWalign(short* A, int lenA, short* B, int lenB)
{
    initializeScore();
    short scr;

    S[0][0] = 0;	D[0][0] = 0;	I[0][0] = 0;
    P[0][0] = -1;
    // left end of sequences gap free
    for( int i = 1; i < lenA; i++ ){
        D[i][0] = 0;
        S[i][0] = 0;
        I[i][0] = MINSCORE;
        P[i][0] = DELETION;
    }
    for( int j = 1; j < lenB; j++ ){
        I[0][j] = 0;
        S[0][j] = 0;
        D[0][j] = MINSCORE;
        P[0][j] = INSERTION;
    }

    for( int i = 1; i < lenA-1; i++ ){
        for( int j = 1; j < lenB-1; j++ ){
            D[i][j] = D[i-1][j] - gapExtnPenalty;
            scr = S[i-1][j] - gapOpenPenalty - gapExtnPenalty;
            if( D[i][j] < scr )
                D[i][j] = scr;

            I[i][j] = I[i][j-1] - gapExtnPenalty;
            scr = S[i][j-1] - gapOpenPenalty - gapExtnPenalty;
            if( I[i][j] < scr )
                I[i][j] = scr;

            S[i][j] = S[i-1][j-1] + score[A[i]][B[j]];
            P[i][j] = SUBSTITUTION;
            if( D[i][j] > S[i][j] ){
                S[i][j] = D[i][j];
                P[i][j] = DELETION;
            }
            if( I[i][j] > S[i][j] ){
                S[i][j] = I[i][j];
                P[i][j] = INSERTION;
            }
        }
    }

    // right end of sequence gap free
    for( int i = 1; i < lenA-1; i++ ){
        D[i][lenB-1] = S[i-1][lenB-1];
        I[i][lenB-1] = I[i][lenB-2] - gapExtnPenalty;
        scr = S[i][lenB-2] - gapOpenPenalty - gapExtnPenalty;
        if( I[i][lenB-1] < scr )
            I[i][lenB-1] = scr;

        S[i][lenB-1] = S[i-1][lenB-2] + score[A[i]][B[lenB-1]];
        P[i][lenB-1] = SUBSTITUTION;
        if( D[i][lenB-1] > S[i][lenB-1] ){
            S[i][lenB-1] = D[i][lenB-1];
            P[i][lenB-1] = DELETION;
        }
        if( I[i][lenB-1] > S[i][lenB-1] ){
            S[i][lenB-1] = I[i][lenB-1];
            P[i][lenB-1] = INSERTION;
        }
    }
    for( int j = 1; j < lenB-1; j++ ){
        I[lenA-1][j] = S[lenA-1][j-1];
        D[lenA-1][j] = D[lenA-2][j] - gapExtnPenalty;
        scr = S[lenA-2][j] - gapOpenPenalty - gapExtnPenalty;
        if( D[lenA-1][j] < scr )
            D[lenA-1][j] = scr;

        S[lenA-1][j] = S[lenA-2][j-1] + score[A[lenA-1]][B[j]];
        P[lenA-1][j] = SUBSTITUTION;
        if( D[lenA-1][j] > S[lenA-1][j] ){
            S[lenA-1][j] = D[lenA-1][j];
            P[lenA-1][j] = DELETION;
        }
        if( I[lenA-1][j] > S[lenA-1][j] ){
            S[lenA-1][j] = I[lenA-1][j];
            P[lenA-1][j] = INSERTION;
        }
    }

    D[lenA-1][lenB-1] = S[lenA-2][lenB-1];
    I[lenA-1][lenB-1] = S[lenA-1][lenB-2];
    S[lenA-1][lenB-1] = S[lenA-2][lenB-2] + score[A[lenA-1]][B[lenB-1]];
    P[lenA-1][lenB-1] = SUBSTITUTION;
    if( D[lenA-1][lenB-1] > S[lenA-1][lenB-1] ){
        S[lenA-1][lenB-1] = D[lenA-1][lenB-1];
        P[lenA-1][lenB-1] = DELETION;
    }
    if( I[lenA-1][lenB-1] > S[lenA-1][lenB-1] ){
        S[lenA-1][lenB-1] = I[lenA-1][lenB-1];
        P[lenA-1][lenB-1] = INSERTION;
    }

    // trace back to find the actual alignment
    deque<short> AA, BB;
    int n = lenA-1, m = lenB-1;
    while( P[n][m] >= 0 ){
        if( P[n][m] == SUBSTITUTION ){
            AA.push_front( A[n] );
            BB.push_front( B[m] );
            n--;
            m--;
        }
        else if( P[n][m] == DELETION ){
            AA.push_front( A[n] );
            BB.push_front( INDEL );
            n--;
        }
        else if( P[n][m] == INSERTION ){
            AA.push_front( INDEL );
            BB.push_front( B[m] );
            m--;
        }
    }
    if( n != 0 || m != 0 ){ 
        QMessageBox::about(0, "Alignment Error", "Fatal error in sequence pairwise alignment.\nProgram is forced to terminate.\nPlease report to developer.");
        exit(-1);
    }

    // calculate identity percentage
    float id = 0;
    int numA = 0, numB = 0, numAB = 0;

    // 1 for count all gaps, 2 for count internal gap only, 3 for count no gaps
    if(  policy == Internal_End ){ // count all gaps
        for( unsigned i = 0; i < AA.size(); i++ ){
            if( AA[i] == BB[i] )
                    id++;
        }
        id = id / (float)AA.size() * 100;
    }
    else if( policy == NonGap ){ // count no gap
        for( unsigned i = 0; i < AA.size(); i++ ){
            if( AA[i] == INDEL || BB[i] == INDEL )
                continue;
            if( AA[i] == BB[i] )
                id++;
            numAB++;
        }
        id = id / (float)numAB * 100;
    }
    else{
        for( unsigned i = 0; i < AA.size(); i++ ){ // count internal gap only
            if( AA[i] != INDEL )	numA++;
            if( BB[i] != INDEL )	numB++;
            if( ( AA[i] == INDEL && (numA == 0 || numA >= lenA -1) ) || ( BB[i] == INDEL && (numB == 0 || numB >= lenB -1) ) )
                continue;

            if( AA[i] == BB[i] )
                id++;
            numAB++;
        }
        id = id / (float)numAB * 100;
    }
    return id;
}




// pairwise alignment using Needleman-Wunsch algorithm with end gap free panelty
// record the alignment result in the cstring array
float SeqAlign::NWalign(short* A, int lenA, short* B, int lenB, QStringList *result)
{
        initializeScore();
        short scr;

        S[0][0] = 0;	D[0][0] = 0;	I[0][0] = 0;
        P[0][0] = -1;
        // left end of sequences gap free
        for( int i = 1; i < lenA; i++ ){
                D[i][0] = 0;
                S[i][0] = 0;
                I[i][0] = MINSCORE;
                P[i][0] = DELETION;
        }
        for( int j = 1; j < lenB; j++ ){
                I[0][j] = 0;
                S[0][j] = 0;
                D[0][j] = MINSCORE;
                P[0][j] = INSERTION;
        }

        for( int i = 1; i < lenA-1; i++ ){
                for( int j = 1; j < lenB-1; j++ ){
                        D[i][j] = D[i-1][j] - gapExtnPenalty;
                        scr = S[i-1][j] - gapOpenPenalty - gapExtnPenalty;
                        if( D[i][j] < scr )
                                D[i][j] = scr;

                        I[i][j] = I[i][j-1] - gapExtnPenalty;
                        scr = S[i][j-1] - gapOpenPenalty - gapExtnPenalty;
                        if( I[i][j] < scr )
                                I[i][j] = scr;

                        S[i][j] = S[i-1][j-1] + score[A[i]][B[j]];
                        P[i][j] = SUBSTITUTION;
                        if( D[i][j] > S[i][j] ){
                                S[i][j] = D[i][j];
                                P[i][j] = DELETION;
                        }
                        if( I[i][j] > S[i][j] ){
                                S[i][j] = I[i][j];
                                P[i][j] = INSERTION;
                        }
                }
        }


        // right end of sequence gap free

        for( int i = 1; i < lenA-1; i++ ){
                D[i][lenB-1] = S[i-1][lenB-1];
                I[i][lenB-1] = I[i][lenB-2] - gapExtnPenalty;
                scr = S[i][lenB-2] - gapOpenPenalty - gapExtnPenalty;
                if( I[i][lenB-1] < scr )
                        I[i][lenB-1] = scr;

                S[i][lenB-1] = S[i-1][lenB-2] + score[A[i]][B[lenB-1]];
                P[i][lenB-1] = SUBSTITUTION;
                if( D[i][lenB-1] > S[i][lenB-1] ){
                        S[i][lenB-1] = D[i][lenB-1];
                        P[i][lenB-1] = DELETION;
                }
                if( I[i][lenB-1] > S[i][lenB-1] ){
                        S[i][lenB-1] = I[i][lenB-1];
                        P[i][lenB-1] = INSERTION;
                }
        }
        for( int j = 1; j < lenB-1; j++ ){
                I[lenA-1][j] = S[lenA-1][j-1];
                D[lenA-1][j] = D[lenA-2][j] - gapExtnPenalty;
                scr = S[lenA-2][j] - gapOpenPenalty - gapExtnPenalty;
                if( D[lenA-1][j] < scr )
                        D[lenA-1][j] = scr;

                S[lenA-1][j] = S[lenA-2][j-1] + score[A[lenA-1]][B[j]];
                P[lenA-1][j] = SUBSTITUTION;
                if( D[lenA-1][j] > S[lenA-1][j] ){
                        S[lenA-1][j] = D[lenA-1][j];
                        P[lenA-1][j] = DELETION;
                }
                if( I[lenA-1][j] > S[lenA-1][j] ){
                        S[lenA-1][j] = I[lenA-1][j];
                        P[lenA-1][j] = INSERTION;
                }
        }

        D[lenA-1][lenB-1] = S[lenA-2][lenB-1];
        I[lenA-1][lenB-1] = S[lenA-1][lenB-2];
        S[lenA-1][lenB-1] = S[lenA-2][lenB-2] + score[A[lenA-1]][B[lenB-1]];
        P[lenA-1][lenB-1] = SUBSTITUTION;
        if( D[lenA-1][lenB-1] > S[lenA-1][lenB-1] ){
                S[lenA-1][lenB-1] = D[lenA-1][lenB-1];
                P[lenA-1][lenB-1] = DELETION;
        }
        if( I[lenA-1][lenB-1] > S[lenA-1][lenB-1] ){
                S[lenA-1][lenB-1] = I[lenA-1][lenB-1];
                P[lenA-1][lenB-1] = INSERTION;
        }


        // trace back to find the actual alignment
        deque<short> AA, BB;
        unsigned n = lenA-1, m = lenB-1;
        while( P[n][m] >= 0 ){
                if( P[n][m] == SUBSTITUTION ){
                        AA.push_front( A[n] );
                        BB.push_front( B[m] );
                        n--;
                        m--;
                }
                else if( P[n][m] == DELETION ){
                        AA.push_front( A[n] );
                        BB.push_front( INDEL );
                        n--;
                }
                else if( P[n][m] == INSERTION ){
                        AA.push_front( INDEL );
                        BB.push_front( B[m] );
                        m--;
                }
        }

        if( n != 0 || m != 0 ){
                QMessageBox::about(0, "Alignment Error", "Fatal error in sequence pairwise alignment.\nProgram is forced to terminate.\nPlease report to developer.");
                exit(-1);
        }

        // calculate identity percentage
        float id = 0;
        int numA = 0, numB = 0, numAB = 0;
        QString idStr;
        int mismatch = 0, gapInter = 0, gapEnd = 0;

        // 1 for count all gaps, 2 for count internal gap only, 3 for count no gap
        if( policy == Internal_End ){ // count all gaps
                for( unsigned i = 0; i < AA.size(); i++ ){
                        if( AA[i] != INDEL )	numA++;
                        if( BB[i] != INDEL )	numB++;
                        if( ( AA[i] == INDEL && (numA == 0 || numA >= lenA -1) ) || ( BB[i] == INDEL && (numB == 0 || numB >= lenB -1) ) )
                                gapEnd++;
                        else if( AA[i] == INDEL || BB[i] == INDEL )
                                gapInter++;

                        if( AA[i] == BB[i] )
                                id++;
                        else if( AA[i] < INDEL && BB[i] < INDEL )
                                mismatch++;
                }
                idStr = QString("%1").arg(id);
                id = id / (float)AA.size() * 100;
        }
        else if( policy == NonGap ){ // count no gaps
                for( unsigned i = 0; i < AA.size(); i++ ){
                        if( AA[i] != INDEL )	numA++;
                        if( BB[i] != INDEL )	numB++;
                        if( ( AA[i] == INDEL && (numA == 0 || numA >= lenA -1) ) || ( BB[i] == INDEL && (numB == 0 || numB >= lenB -1) ) )
                                gapEnd++;
                        else if( AA[i] == INDEL || BB[i] == INDEL )
                                gapInter++;

                        if( AA[i] == INDEL || BB[i] == INDEL ) // skip all gaps
                                continue;
                        if( AA[i] == BB[i] )
                                id++;
                        else if( AA[i] < INDEL && BB[i] < INDEL )
                                mismatch++;
                        numAB++;
                }
                idStr = QString("%1").arg(id);
                id = id / (float)numAB * 100;
        }
        else{
                for( unsigned i = 0; i < AA.size(); i++ ){ // count internal gap only
                        if( AA[i] != INDEL )	numA++;
                        if( BB[i] != INDEL )	numB++;
                        if( ( AA[i] == INDEL && (numA == 0 || numA >= lenA -1) ) || ( BB[i] == INDEL && (numB == 0 || numB >= lenB -1) ) )
                                gapEnd++;
                        else if( AA[i] == INDEL || BB[i] == INDEL )
                                gapInter++;

                        if( ( AA[i] == INDEL && (numA == 0 || numA >= lenA -1) ) || ( BB[i] == INDEL && (numB == 0 || numB >= lenB -1) ) )
                                continue;  // skip end gap
                        if( AA[i] == BB[i] )
                                id++;
                        else if( AA[i] < INDEL && BB[i] < INDEL )
                                mismatch++;
                        numAB++;
                }
                idStr = QString("%1").arg(id);
                id = id / (float)numAB * 100;
        }


        // fill the result string
        QString strA, strB, strMiddle, str;

        str = "Identity: " + idStr + "; Mismatch: ";
        strA = QString("%1").arg(mismatch);
        str += strA + "; Internal gaps: ";
        strA = QString("%1").arg(gapInter);
        str += strA + "; End gaps: ";
        strA = QString("%1").arg(gapEnd);
        str += strA + ".\n";
        result->clear();;
        result->append(str);

        int oldA, oldB;
        numA=0; numB=0;
        while( AA.size() > 0 ){
                oldA = numA+1;	oldB = numB+1;
                n = 0;
                strA.clear();	strB.clear();	strMiddle.clear();
                while( n < 100 && AA.size() > 0 ){
                        if( AA[0] == BB[0] )	strMiddle.append('|');
                        else				strMiddle.append(' ');

                        if( AA[0] != INDEL )	numA++;
                        if( BB[0] != INDEL )	numB++;

                        strA.append(tools::int2base(AA[0]));
                        strB.append(tools::int2base(BB[0]));
                        AA.erase( AA.begin() );
                        BB.erase( BB.begin() );
                        n++;
                }
                if( numA == 0 && oldA > numA )	oldA--;
                else if( oldA > numA ){
                        numA = lenA;
                        oldA = lenA;
                }
                if( numB == 0 && oldB > numB )	oldB--;
                else if( oldB > numB ){
                        numB = lenB;
                        oldB = lenB;
                }

                str = QString("%1").arg(oldA);
                //if( str.length() < 3 )	str.append("\t");
                strA.insert( 0, str + "\t" );
                str = QString("%1").arg(oldB);
                //if( str.length() < 3 )	str.append("\t");
                strB.insert( 0, str + "\t" );
                strMiddle.insert( 0, "\t" );

                str = QString("%1").arg(numA);
                strA.append("\t" + str);
                str = QString("%1").arg(numB);
                strB.append("\t" + str);

                result->append(strA);
                result->append(strMiddle);
                result->append(strB);
        }
        return id;
    }


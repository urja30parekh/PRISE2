#include <QString>
#pragma once
// close "depricated warning message"

//#define MAC
//#define WIN
#define _CRT_SECURE_NO_DEPRECATE 1


// WINVER is defined defaultly to 0x0501 for Win XP
// defining it as 0x0400 will allow it to run on Windows 2000 as well.
#define WINVER 0x0400

using namespace std;


#define wxUSE_MEMORY_TRACING 0  //gt - default is 1
#define wxUSE_GLOBAL_MEMORY_OPERATORS 0  //gt - default is 1

#define BASE_A 0
#define BASE_C 1
#define BASE_G 2
#define BASE_T 3
#define BASE_B 4
#define BASE_D 5
#define BASE_H 6
#define BASE_K 7
#define BASE_M 8
#define BASE_N 9
#define BASE_R 10
#define BASE_S 11
#define BASE_V 12
#define BASE_W 13
#define BASE_Y 14
#define INDEL 15

#define LEFT_PRIMER 0
#define RIGHT_PRIMER 1

#define FLT_MAX         3.402823466e+38F        /* max value */
#define COST_INFINITE FLT_MAX/2

#define MAX_ALPHABET_SIZE 4
#define MAX_PRIMER_LEN 121
//#define MAX_PRIMER_LEN 41
#define MAX_PROBE_LEN 121
//#define MAX_PROBE_LEN 41
#define MAX_SEQUENCE_LEN 8000
#define MAX_SEQUENCE_DOWNLOAD 50000

//#define HASH_OCC_SIZE 1048576  // 4^10
// 4^8
#define HASH_OCC_SIZE 65536

#define LESSTHAN 0
#define GREATERTHAN 1

#define TINY_IDENTITY_VALUE 0.0001f
// for pairwise sequence alignment
#define MAX_ALIGN_SIZE 6000
#define SUBSTITUTION 0
#define INSERTION 1
#define DELETION 2

#define MINSCORE -20000
#define FATAL_ERROR -2


// for primer pair report window
#define A_NUM_COLUMNS		30
#define A_PRIMER_NO		0
#define A_PRIMER_PAIR		1
#define A_TARGET_PCTG		2
#define A_LFT_TARGET_PCTG	3
#define A_RHT_TARGET_PCTG	4
#define A_NONTAR_PCTG		5
#define A_LFT_NONTAR_PCTG	6
#define A_RHT_NONTAR_PCTG	7
#define A_LFT_TAR_BIND_POS	8
#define A_RHT_TAR_BIND_POS	9
#define A_TAR_PCR_AVG		10
#define A_TAR_PCR_MIN		11
#define A_TAR_PCR_MAX		12
#define A_NONTAR_PCR_AVG	13
#define A_NONTAR_PCR_MIN	14
#define A_NONTAR_PCR_MAX	15
#define A_LFT_GC		16
#define A_RHT_GC		17
#define A_LFT_TM		18
#define A_RHT_TM		19
#define A_INTER_COMP		20
#define A_INTER3_COMP		21
#define A_LFT_INTRA_COMP	22
#define A_RHT_INTRA_COMP	23
#define A_LFT_INTRA3_COMP	24
#define A_RHT_INTRA3_COMP	25
#define A_NUM_AMBI_TARGETS_LEFT	26
#define A_NUM_AMBI_TARGETS_RIGHT 27
#define A_NUM_AMBI_BASES_LEFT    28
#define A_NUM_AMBI_BASES_RIGHT   29
//#define A_NUM_TAR       30

// for primer-probe triples report window
#define B_NUM_COLUMNS		13
#define B_TRIPLE_NO		0
//#define B_PRIMER_PAIR		1
#define B_PROBE                 1
#define B_TARGET_PCTG		2
//#define B_LFT_TARGET_PCTG	4
//#define B_RHT_TARGET_PCTG	5
#define B_PROBE_TARGET_PCTG	3
#define B_NONTAR_PCTG		4
//#define B_LFT_NONTAR_PCTG	8
//#define B_RHT_NONTAR_PCTG	9
#define B_PROBE_NONTAR_PCTG	5
//#define B_LFT_TAR_BIND_POS	11
//#define B_RHT_TAR_BIND_POS	12
#define B_PROBE_TAR_BIND_POS	6
//#define B_TAR_PCR_AVG		14
//#define B_TAR_PCR_MIN		15
//#define B_TAR_PCR_MAX		16
//#define B_NONTAR_PCR_AVG	17
//#define B_NONTAR_PCR_MIN	18
//#define B_NONTAR_PCR_MAX	19
//#define B_LFT_GC		20
//#define B_RHT_GC		21
#define B_PROBE_GC		7
//#define B_LFT_TM		23
//#define B_RHT_TM		24
#define B_PROBE_TM		8
#define B_INTER_COMP	9
//#define B_INTER3_COMP   10
//#define B_INTER3_COMP		27
//#define B_LFT_INTRA_COMP	28
//#define B_RHT_INTRA_COMP	29
#define B_PROBE_INTRA_COMP	10
//#define B_PROBE_INTRA3_COMP	12
#define B_NUM_AMBI_TARGETS	11
#define B_NUM_AMBI_BASES    12
//#define B_NUM_TAR           15
//#define B_PROBE_INTRA3_COMP	33


#define SORT_BY_FORMULA		0
#define SORT_BY_MISMATCH	1
#define ASCENDING		0
#define DESCENDING		1


// for SequenceReportDlg mark/unmark and find sequences
#define SOURCE		1
#define FEATURES	2
#define AUTHORS		3
#define TITLE		4

// for user help displaying
#define HELPMANUAL	1
#define HELPTUTORIAL	2


enum countGapPolicy {
    NonGap =0,
    Internal =1,
    Internal_End =2
};

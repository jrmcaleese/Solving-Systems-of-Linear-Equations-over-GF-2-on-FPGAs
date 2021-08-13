#ifndef ARRAYHANDLING_H
#define ARRAYHANDLING_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "VariableExhaustiveSearch.h"

extern int A[Equations][Variables];
extern int b[Equations];
extern int AddOne[Variables - 1];
extern int InitSystem[InitEqs][Variables];
extern int InitSums[InitEqs];
extern int x[Variables-1]; //an array for storing values in the binary counter
extern int y[Variables-1]; // an array for storing temp results in the binary counter
extern int EqSum[Equations]; //an array for storing the sums of each equation to test them against their respective values in b[]
extern int invalid[MaxBin]; //an array for storing invalid solutions
extern int valid[MaxBin]; //an array for storing the which solutions are valid
extern int reducedvalid[MaxBin];
extern int validsols[MaxBin];
extern int tempA[MaxCombs][Variables];
extern int tempb[MaxCombs];
extern int reducedA[MaxCombs][Variables];
extern int reducedb[MaxCombs];
extern int samecoef[MaxCombs];

int randbit(void);
int GenArrays(void);
int LoadArrays(void);
int LinCombs(int arr[InitEqs][Variables]);
int PrintLCArrays(void);
void InvertRows(int arr1[][Variables], int size);
int PrintreducedArrays(bool repeat);
#endif
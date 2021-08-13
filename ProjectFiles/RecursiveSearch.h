#ifndef RECURSIVESEARCH_H
#define RECURSIVESEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "parameters.h"


extern int RowWeights[Equations];
extern int CoefIndices[Variables - 1];
extern int stopPoint[Equations];
extern int depth;
extern int tempsum[Equations];
extern int tempsol[Variables - 1];
extern int answers[MaxBin];
extern int answercount;
extern int binsize;
extern int fullbinsize[Equations];
extern int row[Equations][Variables];
extern int temprow[Variables];
extern int looplimit[Equations];

void GetRowWeights(int arr1[Equations][Variables], int Weights[Equations]);
int countOnes(int arr1[], int limit);
void SortArray(int InputArray[Equations][Variables], int rw[Equations]);
int bin2dec();
void recursivesolve(int InputArray[Equations][Variables]);
#endif
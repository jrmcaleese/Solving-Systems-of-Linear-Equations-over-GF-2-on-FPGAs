#ifndef REFINEDSEARCH_H
#define REFINEDSEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "parameters.h"


extern int RowWeights[Equations];

int truthtable(int RHS, int One, int A);
void GetRowWeights(int arr1[Equations][Variables], int Weights[Equations]);
void SortArray(int InputArray[Equations][Variables], int rw[Equations]);



#endif
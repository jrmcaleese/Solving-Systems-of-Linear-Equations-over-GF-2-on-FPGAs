#ifndef REFINEDSEARCH_H
#define REFINEDSEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "parameters.h"


extern int RowWeights[Equations];
extern int ColWeights[(Variables - 1)];

int truthtable(int RHS, int One, int A);
void GetRowWeights(int arr1[Equations][Variables], int Weights[Equations]);
void GetColWeights(int arr1[Equations][Variables], int Weights[(Variables - 1)]);
void SortRows(int InputArray[Equations][Variables], int rw[Equations], int cw[(Variables - 1)]);



#endif
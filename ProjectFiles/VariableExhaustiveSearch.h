#ifndef VARIABLEEXHAUSTIVESEARCH_H
#define VARIABLEEXHAUSTIVESEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "parameters.h"



void ZeroReset(void);
void BinArrayAdd(int length, int arr1[], int arr2[]);
void SystemDeterminance(int CurrentEqs);
int GenerateSolutions(int arr1[MaxBin], int arr2[MaxBin], int arr3[MaxBin]);
int PrintSolutions(int arr1[MaxBin], bool repeat);
void ExhaustiveSearch(int LHS[][Variables], int RHS[], int v[MaxBin], int CurrentEqs);
void VariableSearch(int LHS[][Variables], int RHS[], int v1[MaxBin], int v2[MaxBin], int vs[MaxBin], bool repeat);

extern int GA, LA, LC1, LC2, PA;
extern int CoefProd; // product of a polynomial coefficient with the test variable
extern int CarryBit; //carry bit needed in the binary counter
extern int SolCount; //tracking the number of valid solutions for the initial system
extern int FinalCount;
extern int InvCount; //tracking the number of invalid solutions
extern int ValidRows; //tracking the number of equations for which a possible solution is valid




#endif
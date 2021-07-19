#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "VariableExhaustiveSearch.h"
#include "parameters.h"
#include "SimilarityCheck.h"
#include "ArrayHandling.h"

int RowWeights[Equations] = { 0 };
int ColWeights[(Variables - 1)] = { 0 };

void GetRowWeights(int arr1[Equations][Variables], int Weights[Equations])
{
    int i, j;
    int sum;
    for (i = 0; i < Equations; i++)
    {
        sum = 0;
        for (j = 0; j < (Variables - 1); j++)
        {
            if (arr1[i][j] == 1)
            {
                sum++;
            }
        }
        Weights[i] = sum;
    }
}
void GetColWeights(int arr1[Equations][Variables], int Weights[(Variables-1)])
{
    int i, j;
    int sum;
    for (i = 0; i < (Variables-1); i++)
    {
        sum = 0;
        for (j = 0; j < Equations; j++)
        {
            if (arr1[i][j] == 1)
            {
                sum++;
            }
        }
        Weights[i] = sum;
    }
}

void SortRows(int InputArray[Equations][Variables], int rw[Equations], int cw[(Variables-1)])
{
    int i, j, k;
    int temprow[Variables] = { 0 };
    int tempcoef = 0;
    int tempweight = 0;
    int temp = 0;

    GetRowWeights(InputArray, rw);
    GetColWeights(InputArray, cw);
    for (i = 0; i < Equations; i++)
    {
        printf("\nRowWeight[%d]=%d", i, rw[i]);
    }
    printf("\n");
    //Rearrange the equations so the number of coefficients per matrix row decreases from top to bottom
    for (i = 0; i < (Equations-1); i++)
    {
        //Bubble sorting each row of the matrix
        for (j = 0; j < Equations - 1 - i; j++)
        {
            if (rw[j] < rw[j + 1])
            {
                for (k = 0; k < (Variables); k++)
                {
                    temprow[k] = InputArray[i+1][k];
                    InputArray[i+1][k] = InputArray[i][k];
                    InputArray[i][k] = temprow[k];
                }
                tempweight = rw[j+1];
                rw[j+1] = rw[j];
                rw[j] = tempweight;

                temp = InitSums[j + 1];
                InitSums[j + 1] = InitSums[j];
                InitSums[j] = temp;
            }
        }
    }
    /*
    //Rearrange the columns in an attempt to create an upper triangle of zeroes in the matrix
    for (i = 0; i < (Equations); i++)
    {
        tempcoef = 0;
        //If sorting the first row, follow these instructions
        if (i == 0)
        {
            
            //For sorting an array, you need to subtract 1 from the for loop limit 
            //to make sure you dont accidentally compare the last value against nothing. 
            //The limit was already (Variables - 1) to account for the last value in the row being the RHS.
            for (j = 0; j < (Variables - 2); j++)
            {
                for (k = 0; k < (Variables - 2) - j; k++)
                {
                    if (InputArray[i][k] > InputArray[i][k + 1])
                    {
                        tempcoef = InputArray[i][k];
                        InputArray[i][k] = InputArray[i][k + 1];
                        InputArray[i][k + 1] = tempcoef;
                    }
                }
            }
        }
        //If sorting any row beyond the first, follow these instructions
        else
        {
            for (j = 0; j < (Variables - 2); j++)
            {
                for (k = 0; k < (Variables - 2) - j; k++)
                {
                    //If the value one row up is 1,
                    //then this column doesnt need to be moved
                    if (InputArray[i - 1][k] == 1)
                    {
                        break;
                    }
                    //Otherwise, if the value one row up and one column over is 1,
                    //then you've finished sorting for this row
                    else if (InputArray[i - 1][k + 1] == 1)
                    {
                        break;
                    }
                    //otherwise continue sorting
                    else if (InputArray[i][k] > InputArray[i][k + 1])
                    {
                        tempcoef = InputArray[i][k];
                        InputArray[i][k] = InputArray[i][k + 1];
                        InputArray[i][k + 1] = tempcoef;
                    }
                }
            }
        }
    }
    */
}
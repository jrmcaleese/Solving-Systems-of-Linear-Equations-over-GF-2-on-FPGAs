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

void SortArray(int InputArray[Equations][Variables], int rw[Equations])
{
    int f, g, h, i, j, k;
    int temprow[Variables] = { 0 };
    int tempcoef[Equations] = { 0 };
    int tempweight = 0;
    int temp = 0;
    int count = Variables - 1;

    GetRowWeights(InputArray, rw);
    //Sort the equations so the number of coefficients per matrix row decreases from top to bottom
    for (i = 0; i < (Equations-1); i++)
    {
        //Bubble sorting each row of the matrix
        for (j = 0; j < Equations - 1 - i; j++)
        {
            if (rw[j] < rw[j + 1])
            {
                for (k = 0; k < (Variables); k++)
                {
                    temprow[k] = InputArray[j+1][k];
                    InputArray[j+1][k] = InputArray[j][k];
                    InputArray[j][k] = temprow[k];
                }
                tempweight = rw[j+1];
                rw[j+1] = rw[j];
                rw[j] = tempweight;
            }
        }
    }
    
    //Rearrange the columns in an attempt to create an upper triangle of zeroes in the matrix
    for (i = 0; i < Equations; i++) //Increment through each row
    {
        if (count < 0)
        {
            break;
        }
        //Sort the columns based on the 1s in the current row of outer loop
        for (j = 0; j < count; j++)
        {
            for (k = 0; k < count - 1 - j; k++)
            {
                //If the value at this index in the current row is greater than the next value
                //then swap the entire columns
                if (InputArray[i][k] > InputArray[i][k + 1])
                {
                    for (h = 0; h < Equations; h++)
                    {
                        tempcoef[h] = InputArray[h][k];
                        InputArray[h][k] = InputArray[h][k + 1];
                        InputArray[h][k + 1] = tempcoef[h];
                    }
                }
            }
        }
        //In the current row, find the first index with a one so we have a new stopping point when sorting the next row
        for (j = 0; j < Variables - 1; j++)
        {
            if (InputArray[i][j] == 1)
            {
                count = j;
                break;
            }
        }
    }
}
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
int CoefIndices[Variables - 1] = { 0 };
int stopPoint[Equations] = { 0 };


int factorial(int n)
{
    int i;
    int output=1;
    for (i = 2; i <= n; i++)
    {
        output *= i;
    }
    return output;
}

int ncr(int max, int choose)
{
    int output;
    output = factorial(max) / (factorial(choose) * factorial(max - choose));
    return output;
}
int totalsols(int InputArray[Equations][Variables],int rw[Equations])
{
    int i,j;
    int total = 0;
    for (i = 0; i < Equations; i++)
    {
        if (InputArray[i][Variables - 1] == 0)
        {
            if (rw[i] % 2 == 0)
            {
                for (j = rw[i]; j > -1; j -= 2)
                {
                    total += factorial(rw[i], j);
                }
            }
            else if (rw[i] % 2 == 1)
            {
                for (j = rw[i]; j > 0; j -= 2)
                {
                    total += factorial(rw[i], j);
                }
            }
        }
        else if (InputArray[i][Variables - 1] == 1)
        {
            if (rw[i] % 2 == 0)
            {
                for (j = rw[i]; j > -1; j -= 2)
                {
                    total += factorial(rw[i], j);
                }
            }
            else if (rw[i] % 2 == 1)
            {
                for (j = rw[i]; j > 0; j -= 2)
                {
                    total += factorial(rw[i], j);
                }
            }
        }
    }
    return total;
}
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
void GetCoefIndices(int arr1[Variables], int indices[Variables - 1])
{
    int i;
    for (i = 0; i < (Variables - 1); i++)
    {
        if (arr1[i] == 1)
        {
            indices[i] = i;
        }
    }
}

int countOnes(int arr1[],int limit)
{
    int i;
    int onescount = 0;
    for (i = 0; i < limit; i++)
    {
        if (arr1[i] == 1)
        {
            onescount++;
        }
    }
    return onescount;
}

void SortArray(int InputArray[Equations][Variables], int rw[Equations])
{
    int h, i, j, k;
    int temprow[Variables] = { 0 };
    int tempcoef[Equations] = { 0 };
    int tempweight = 0;
    int temp = 0;
    stopPoint[0] = Variables - 1;

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
        if (stopPoint[i] < 0)
        {
            break;
        }
        //Sort the columns based on the 1s in the current row of outer loop
        for (j = 0; j < stopPoint[i]; j++)
        {
            for (k = 0; k < stopPoint[i] - 1 - j; k++)
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
                stopPoint[i+1] = j;
                break;
            }
        }
    }
}

void findSols(int InputArray[Equations][Variables], int rw[Equations])
{
    int h,i,j,k;
    int count;
    int tempsol[Variables - 1] = { 0 };
    int binsize;
    int solutions = totalsols(InputArray, rw);
    for (h = 0; h < solutions; h++)
    {
        //For Each Row in the sorted array
        for (i = 0; i < Equations; i++)
        {
            //If the RHS of the current row is 1
            if (InputArray[i][(Variables - 1)] == 1)
            {
                //Then, while counting down from the stopPoint of the current row to the stopPoint for the next row
                //(AKA only the sorted 1s on this row)
                //Find and store all solutions with an odd number of coefficients
                for (j = stopPoint[i] - 1; j >= stopPoint[i + 1]; j--)
                {
                    binsize = ((stopPoint[i] - 1) - stopPoint[i + 1]);
                    //increment through possible binary numbers
                    BinArrayAdd(binsize, x, y);
                    count = countOnes(x, (Variables - 1));
                    //if the binary number has an odd number of coefficients append it to the temp solution at the proper indices
                    if (count % 2 == 1)
                    {
                        for (k = 0; k < Variables - 1; k++)
                        {
                            tempsol[k + stopPoint[i + 1]] = x[k];
                        }
                    }
                }
            }
            //If the RHS of the current row is 0
            else if (InputArray[i][(Variables - 1)] == 0)
            {
                //Then, while counting down from the stopPoint of the current row until the stopPoint for the next row 
                //(AKA only the sorted 1s on this row)
                //Find and store all solutions with an even number of coefficients
                for (j = stopPoint[i] - 1; j >= stopPoint[i + 1]; j--)
                {
                    binsize = (stopPoint[i] - 1) - stopPoint[i + 1];
                    //increment through possible binary numbers
                    BinArrayAdd(binsize, x, y);
                    count = countOnes(x, (Variables - 1));
                    //if the binary number has an even number of coefficients append it to the temp solution at the proper indices
                    if (count % 2 == 0)
                    {
                        for (k = 0; k < Variables - 1; k++)
                        {
                            tempsol[k + stopPoint[i + 1]] = x[k];
                        }
                    }
                }
            }
        }
    }
}

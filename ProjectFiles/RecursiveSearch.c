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
int depth = 0;
int tempsum[Equations] = {0};
int tempsol[Variables - 1] = { 0 };
int tempones = 0;
int answers[MaxBin] = { 0 };
int answercount = 0;
int binsize = 0;
int fullbinsize[Equations] = { 0 };
int row[Equations][Variables] = { 0 };
int temprow[Variables] = { 0 };
int looplimit[Equations] = { 0 };

void GetRowWeights(int arr1[Equations][Variables], int Weights[Equations])
{
    //get the number of ones for each row
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

int countOnes(int arr1[],int limit)
{
    //a function for counting the number of ones in an array
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
    //sorting the array into the format required for finding solutions
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
        if (i == 0)
        {
            for (j = 0; j < Variables-1; j++)
            {
                for (k = 0; k < Variables - 2 - j; k++)
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
                    stopPoint[i] = j;
                    break;
                }
            }
        }
        else
        {
            if (stopPoint[i - 1] <= 0)
            {
                break;
            }
            //Sort the columns based on the 1s in the current row of outer loop
            for (j = 0; j < stopPoint[i - 1]; j++)
            {
                for (k = 0; k < stopPoint[i - 1] - 1 - j; k++)
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
                    stopPoint[i] = j;
                    break;
                }
            }
        }
        
    }
}

int bin2dec()
{
    int i;
    int tempval=0;
    for (i = 0; i < Variables - 1; i++)
    {
        if (row[depth][i] == 1)
        {
            tempval += 1 << i;
        }
    }
    return tempval;
}

void RecursiveSearch(int InputArray[Equations][Variables])
{
    int i, j, k;
    int sameones = 0;
    //calculate the loop limit for the given row as 
    //( (2^S_i) - 1 ) where S_i is the size in bits of the clustered column
    //Bitshifting to the appropriate depth serves as a quick shortcut for raising to the correct power of 2
    int size;
    if (depth == 0)
    {
        size = (Variables - 1) - stopPoint[depth];
    }
    else
    {
        size = stopPoint[depth - 1] - stopPoint[depth];
    }
    looplimit[depth] = (1 << size)-1;
    for (i = 0; i <= looplimit[depth]; i++)
    {
        if (looplimit[depth] == 0 && depth != (Equations - 1)) //if we've found a full solution but we aren't at max depth, we need to check remaining rows against the full solution
        {
            for (j = depth; j < Equations; j++)
            {
                sameones = 0;
                int remainingrows = Equations - depth;
                EqSum[j] = 0; //making sure EqSum is zero before calculating for the current row so only the correct answer for this iteration is recorded
                for (k = 0; k < Variables - 1; k++)
                {
                    if ((InputArray[j][k] == 1) && (tempsol[k] == 1))
                    {
                        sameones += 1; //if a coefficient bit in the current equation and its corresponding bit in the solution are both 1, then increment sameones
                    }
                }
                if (sameones % 2 == 1)
                {
                    EqSum[j] = 1; //if sameones is odd, the equation sum is odd
                }
                else if (sameones % 2 == 0)
                {
                    EqSum[j] = 0; //if sameones is even, the equation sum is even
                }
                if (EqSum[j] == InputArray[depth][Variables - 2]) //if the EqSum equals the RHS value the solution is valid
                {
                    if (j == remainingrows)
                    {
                        answers[answercount] = bin2dec(tempsol); //find the decimal equivalent of the answer and append it to an output array
                        answercount++;
                        continue;
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    break;
                }
            }
        }

        if (looplimit[depth] > 0)
        {
            if (looplimit[depth] = 1)
            {
                //if we only have a 1 bit wide group
                //instead of iterating and testing both 0 and 1, we can just set the tempsum equal to the RHS value and guarantee the valid value.
                //only 1 of the two options can be valid.
                tempsum[depth] = InputArray[depth][Variables - 1];
            }
            else
            {
                //Increment through the possible partial solutions for this row
                if (depth == 0)
                {
                    BinArrayAdd(Variables - 1, stopPoint[depth], row[depth], temprow);
                }
                else
                {
                    BinArrayAdd(stopPoint[depth - 1], stopPoint[depth], row[depth], temprow);
                }


                //multiply the possible partial solution with the relevant portion of the current row
                if (depth == 0) //if on the first equation, solve from stop point to the end
                {
                    for (j = stopPoint[depth]; j < Variables - 1; j++)
                    {
                        tempsum[depth] = tempsum[depth] ^ (row[depth][j] & InputArray[depth][j]);
                    }
                }
                else //if not on the first equation, solve from the stop point to the stop point of the last equation
                {
                    for (j = stopPoint[depth]; j < stopPoint[depth - 1]; j++)
                    {
                        tempsum[depth] = tempsum[depth] ^ (row[depth][j] & InputArray[depth][j]);
                    }
                }
                //add the tempsum of the previous rows to this one
                tempsum[depth] = tempsum[depth] ^ tempsum[depth - 1];
            }

            if (tempsum[depth] == InputArray[depth][Variables - 1]) //if the tempsum of this row summed with that of all previous rows is equal to the RHS of the current row, the partial solution is valid, so the function should recurse
            {
                //copy the partial solution
                if (depth == 0) //if on the first row, stop copying the partial sol at the last index point
                {
                    for (j = stopPoint[depth]; j < Variables - 1; j++)
                    {
                        tempsol[j] = row[depth][j];
                    }
                }
                else //otherwise stop at the point from the last row
                {
                    for (j = stopPoint[depth]; j < stopPoint[depth - 1]; j++)
                    {
                        tempsol[j] = row[depth][j];
                    }
                }
                if (depth == (Equations - 1)) //if we're at the max depth, then there's no more solution to find after this point
                {
                    answers[answercount] = bin2dec(tempsol); //find the decimal equivalent of the answer and append it to an output array
                    answercount++;
                    continue;
                }
                depth += 1;
                RecursiveSearch(InputArray);
            }

            else
            {
                continue;
            }
        }
        
        //otherwise the function will return to the top of the for loop
    }
}




#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "parameters.h"
#include "ArrayHandling.h"
#include "SimilarityCheck.h"

int A[Equations][Variables] = { 0 };
int b[Equations]={0};
int AddOne[Variables-1] = { 0 };
int x[Variables-1] = { 0 }; //an array for storing final results in the binary counter
int y[Variables-1] = { 0 }; // an array for temp storing results in the binary counter
int EqSum[Equations] = { 0 }; //an array for storing the sums of each equation to test them against their respective values in b[]
int invalid[MaxBin] = { 0 }; //an array for storing invalid solutions
int valid[MaxBin] = { 0 }; //an array for marking the which solutions are valid
int reducedvalid[MaxBin] = { 0 };// an array for marking the valid solutions of a reduced system
int validsols[MaxBin] = { 0 };
int tempA[MaxCombs][Variables] = { 0 }; //holds the linear combinations of the input system so the reduced system can later be populated
int tempb[MaxCombs] = { 0 }; //same as tempA but for the RHS of the input system
int reducedA[MaxCombs][Variables] = { 0 }; //holds the input system after it's been reduced
int reducedb[MaxCombs] = { 0 }; //holds the RHS of the input system after it's been recued
int samecoef[MaxCombs] = { 0 }; //for counting the number of coefficients in common between two equations during SimCheck

int randbit(void) //Generates random bits
{ 
    int n=rand() % 2;
    return n;
}

int GenArrays(void) 
{
    //populates A[] and b[] with random bits
    //only up to the number of equations specified by InitEqs
    int i, j;
    srand(time(0));
    for(i=0;i<InitEqs;i++)
    {
        for(j=0;j<Variables;j++)
        {
            A[i][j]=randbit();
        }
    }
    for(i=0;i<InitEqs;i++)
    {
        b[i]=randbit();
    } 
    AddOne[(Variables-2)]=1;
    return 1;
}

int LoadArrays(void) 
{
    //For Loading Preset Arrays into the program
    int i, j;
    for(i=0;i<InitEqs;i++)
    {
        for(j=0;j<Variables;j++)
        {
            A[i][j]=InitSystem[i][j];
        }
        b[i]=InitSums[i];
    }
    AddOne[(Variables-2)]=1;
    return 1;
}

int LinCombs(int arr[InitEqs][Variables]) //creates all possible linear combinations of the Initial Equations and appends them into A[] and b[] after the Initial Equations
{
    int i=0;
    int h, j, k;
    for(h=0;h<InitEqs-1;h++) //Iterates through all initial equations but hte last one, which will have already been combined with all the others
    {
        for(j=1+h;j<InitEqs;j++) //within the iteration of the current first equation, iterate through all the following equations
        {
            for(k=0;k<Variables;k++) //get the linear combination of the values at each respective arr and store the resulting values to the next free row in A[]
            {
                arr[i+InitEqs][k]=arr[h][k]^arr[j][k];
            }
            i++;
            if (i==Equations) //if the counter i is equal to the maximum possible number of linear combinations, break out of the function
            {
                break;
            }
        } 
    }
    return 1;
}

int PrintLCArrays(void)
{
    int i, j;
    printf("A[] = \n");
    for (i = 0; i < Equations; i++)
    {
        printf("{");
        for (j = 0; j < Variables; j++)
        {
            printf("%d", A[i][j]);
            if (j == (Variables - 1))
            {
                printf("}, //EQUATION %d\n", (i + 1));
            }
            else
            {
                printf(",");
            }
        }
        if (i == (Equations - 1))
        {
            printf("\n\n");
        }
    }
    printf("\nb[] = {");
    for (i = 0; i < Equations; i++)
    {
        printf("%d", b[i]);
        if (i == (Equations - 1))
        {
            printf("}\n");
        }
        else
        {
            printf(",");
        }
    }
}

void InvertRows(int arr1[][Variables], int size)
{
    int i,j,count;
    for (i = 0; i < size; i++)
    {
        count = 0;
        printf("Equation %d count =", (i + 1));
        for (j = 0; j < Variables; j++)
        {
            if (arr1[i][j] == 1)
            {
                count = count + 1;
            }
        }
        printf("%d\n", count);
        if(count>=0)
        {
            for (j = 0; j < Variables; j++)
            {
                if (arr1[i][j] == 1)
                {
                    arr1[i][j] = 0;
                }
                else if (arr1[i][j] == 0)
                {
                    arr1[i][j] = 1;
                }
            }
        }
    }
}

int PrintreducedArrays(bool repeat) //prints A[] and b[] in a readable format
{
    int i, j;
    if(ReduceSystem==true)
    {
        if(repeat == false)
        {
            printf("InitSystem[] = \n");
            for (i = 0; i < InitEqs; i++)
            {
                printf("{");
                for (j = 0; j < Variables; j++)
                {
                    printf("%d", InitSystem[i][j]);
                    if (j == (Variables - 1))
                    {
                        printf("}, //EQUATION %d\n", (i + 1));
                    }
                    else
                    {
                        printf(",");
                    }
                }
                if (i == (InitEqs - 1))
                {
                    printf("\n");
                }
            }
            printf("InitSols[] = {");
            for (i = 0; i < InitEqs; i++)
            {
                printf("%d", tempb[i]);
                if (i == (InitEqs - 1))
                {
                    printf("}\n\n");
                }
                else
                {
                    printf(",");
                }
            }
        }
        
        printf("SameCoefs[] = {");
        for (i = 0; i < MaxCombs; i++)
        {
            printf("%d", samecoef[i]);
            if (i == (MaxCombs - 1))
            {
                printf("}\n\n");
            }
            else
            {
                printf(",");
            }
        }
        printf("tempA[] = \n");
        for (i = 0; i < MaxCombs; i++)
        {
            printf("{");
            for (j = 0; j < Variables; j++)
            {
                printf("%d", tempA[i][j]);
                if (j == (Variables - 1))
                {
                    printf("}, //EQUATION %d\n", (i + 1));
                }
                else
                {
                    printf(",");
                }
            }
            if (i == (MaxCombs - 1))
            {
                printf("\n");
            }
        }

        
        printf("reducedA[] = \n");
        for (i = 0; i < MaxCombs; i++)
        {
            printf("{");
            for (j = 0; j < Variables; j++)
            {
                printf("%d", reducedA[i][j]);
                if (j == (Variables - 1))
                {
                    printf("}, //EQUATION %d\n", (i + 1));
                }
                else
                {
                    printf(",");
                }
            }
            if (i == (MaxCombs - 1))
            {
                printf("\n");
            }
        }

        printf("tempb[] = {");
        for (i = 0; i < MaxCombs; i++)
        {
            printf("%d", tempb[i]);
            if (i == (MaxCombs - 1))
            {
                printf("}\n\n\n\n");
            }
            else
            {
                printf(",");
            }
        }
        printf("\nreducedb[] = {");
        for (i = 0; i < MaxCombs; i++)
        {
            printf("%d", reducedb[i]);
            if (i == (MaxCombs - 1))
            {
                printf("}\n");
            }
            else
            {
                printf(",");
            }
        }

    }
    
    printf("--------------------\n");
    return 1;
}

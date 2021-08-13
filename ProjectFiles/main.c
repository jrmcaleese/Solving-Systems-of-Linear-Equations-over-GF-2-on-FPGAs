

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "parameters.h"
#include "VariableExhaustiveSearch.h"
#include "SimilarityCheck.h"
#include "ArrayHandling.h"
#include "RecursiveSearch.h"

//Usage bools
bool RandSystem = false; //toggles whether the program uses a preset system or generates a random one
bool doLinCombs = false; //toggles whether A and b are fully populated with their linear combinations
bool ReduceSystem = false; //toggles whether the SimCheck function is used to reduce the system
bool ReduceSearch = false; //toggles whether the ExhaustiveSearch function compares the number of 1s to skip certain solutions
bool RepeatReduce = false; //toggles whether the system reduction happens once or happens repeatedly by LoopReduce times


//Debug bools
bool ShowBinaryValids = false; //toggles whether the just the decimal equivalents of the solutions are shown or the full binary solution sets.
bool ShowFullDebug = false; //toggles detailed print statements for each tested solution. When false only valid solutions are displayed.
bool ListSols = true; //toggles whether the full list of valid solutions is printed together at the end of each search
bool csvFriendly = false; //lists the solutions in a format that is copyable to a csv or excel file
bool SimCheckDebug = false; //toggles the debug print statements for the SimCheck2d function

//The Input system is stored below with the Left Hand Side coefficients stored in InitSystem, and the Right Hand Side stored in InitSols
int InitSystem[Equations][Variables] =
{
    
    {0,0,0,1,0,1,0,1,1,1,0},//0
    {0,1,1,1,0,0,0,0,1,0,1},//1
    {0,0,0,0,1,0,1,1,0,1,0},//2
    {1,1,0,1,1,0,0,0,0,1,0},//3
    {0,1,0,0,0,0,0,1,0,0,1},//4
};
int InitSums[InitEqs] = { 0,1,0,0,1 };

int main()
{
    printf("Start Program\n"); //Marking the Start of the output file
    int i, j;
    AddOne[Variables - 2] = 1;

    //print the initial system
    for (i = 0; i < Equations; i++)
    {
        for (j = 0; j < Variables; j++)
        {
            printf("%d,",InitSystem[i][j]);
        }
        printf("\n");
    }
    printf("==========\n");

    //sort the system
    SortArray(InitSystem, RowWeights);
    //print the sorted system
    for (i = 0; i < Equations; i++)
    {
        for (j = 0; j < Variables; j++)
        {
            printf("%d,", InitSystem[i][j]);
        }
        printf("\n");
    }
    printf("==========\n");

    //find solutions
    RecursiveSearch(InitSystem);
    
    //print the solutions
    printf("The solutions to this system are:\n");
    for (i = 0; i < answercount; i++)
    {
        printf("%d,", answers[i]);
        if (i == answercount - 1)
        {
            printf("\n\n");
        }
        else if (i % 10 == 0)
        {
            printf("\n");
        }
    }

    printf("\nEnd Program");
    return 0;
}
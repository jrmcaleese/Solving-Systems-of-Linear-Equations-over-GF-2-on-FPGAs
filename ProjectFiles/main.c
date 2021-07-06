/*
======================================================================================================================================================
======================================================================================================================================================
A Modified Exhaustive Search Algorithm for Finding Solutions to Systems of Equations over GF(2)
Author: James R. McAleese
Last Updated: July 3rd, 2021

TO USE: 

1. Edit the following define statements in parameters.h to properly set your values:
    - InitEqs: The original number of equations in your system

    - MaxCombs: The number of possible 2 equation linear combinations for a system with a size equal to InitEqs

    - Equations: The maximum number of equations in the system based on the InitEqs plus MaxCombs

    - Variables: The number of variables in the system
    
    - MaxBin:The maximum possible decimal value of a binary number with the same number of digits as variables in your system. 
             This is defined manually because C does not natively allow exponentiation without functions and does not allow function calling in declarations/define statements

    - MinSame: The minimum number of coefficients that two equations must have in common to pass the SimCheck2d() function


2. This program can perform a variety of functions on various kinds of systems, toggle the Usage Bools below to control the output:
    -To use a preset system: Set RandSystem equal to FALSE

    -To use a random system: Set RandSystem equal to TRUE

    -To reduce the size of the initial system: Set reduceSystem to TRUE

    -To reduce/streamline the exhaustive search: Set reduceSearch to TRUE

    -To find the linear combinations of the input system before searching for solutions: Set doLinCombs to TRUE

------------------------------------------------------------------------------------------------------------------------------------------------------

TO DEBUG:

The program always displays the decimal representation of valid solutions. 
To display more information toggle the Debug Bools below:

    - ShowBinaryValids: When set to TRUE, the program prints the full readout of 1s and 0s of each valid solution to the system

    - ShowFullDebug: When set to TRUE, the program prints the full readout of when each tested solution hits an equation where it isn't valid

    - ListSols: When set to TRUE, the program prints all the valid solutions for a system are printed in a single list at the end of a search

    - SimCheckDebug: When set to TRUE, the program prints the full readout of the internal math of the SimCheck2d() function

======================================================================================================================================================
======================================================================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "parameters.h"
#include "VariableExhaustiveSearch.h"
#include "SimilarityCheck.h"
#include "ArrayHandling.h"

//Usage bools
bool RandSystem = false; //toggles whether the program uses a preset system or generates a random one
bool doLinCombs = false; //toggles whether A and b are fully populated with their linear combinations
bool reduceSystem = true; //toggles whether the SimCheck function is used to reduce the system
bool reduceSearch = true; //toggles whether the ExhaustiveSearch function compares the number of 1s to skip certain solutions
bool RepeatReduce = false; //toggles whether the system reduction happens once or happens repeatedly by LoopReduce times


//Debug bools
bool ShowBinaryValids = false; //toggles whether the just the decimal equivalents of the solutions are shown or the full binary solution sets.
bool ShowFullDebug = false; //toggles detailed print statements for each tested solution. When false only valid solutions are displayed.
bool ListSols = true; //toggles whether the full list of valid solutions is printed together at the end of each search
bool csvFriendly = true; //lists the solutions in a format that is copyable to a csv or excel file
bool SimCheckDebug = false; //toggles the debug print statements for the SimCheck2d function

//The Input system is stored below with the Left Hand Side coefficients stored in InitSystem, and the Right Hand Side stored in InitSols
int InitSystem[InitEqs][Variables] =
{
    //{0,0,0,1,0,1,0,1,1,1},//1
    //{0,1,1,1,0,0,0,0,1,0},//2
    //{0,1,1,0,0,1,0,1,0,1},//6
    {0,0,0,1,0,1,0,1,1,1},//0
    {0,1,1,1,0,0,0,0,1,0},//1
    {0,0,0,0,1,0,1,1,0,1},//2
    {1,1,0,1,1,0,0,0,0,1},//3
    {0,1,0,0,0,0,0,1,0,0},//4
    /*
    {0,0,0,1,0,1,0,1,1,1},
    {0,1,1,1,0,0,0,0,1,0},
    {0,0,0,0,1,0,1,1,0,1},
    {1,1,0,1,1,0,0,0,0,1},
    {0,1,0,0,0,0,0,1,0,0},
    {1,1,0,0,0,0,0,0,0,1},
    {1,0,1,0,0,1,1,0,1,1},
    {1,0,1,0,1,1,1,1,1,1},
    {0,0,1,0,0,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1}

    {0,0,0,1},
    {0,1,1,0},
    {1,1,0,1},
    {0,1,0,1}
    */
};
int InitSums[InitEqs] = { 0,1,0,0,1 };//{ 0,1,1 };{ 0,1,1,0 }; { 0,1,0,0,1,0,1,0,1,0 };

int main()
{
    printf("Start Program\n"); //Marking the Start of the output file
    if(RandSystem==true)
    {
        GA = GenArrays(); //Populate A[] and b[] with initial sets of random binary digits
        if(ShowFullDebug==true)
        {
            if (GA == 1)
            {
                printf("GenArrays: PASSED\n");
            }
            else
            {
                printf("GenArrays: FAILED\n");
            }
        }
        printf("This is a randomly generated system.\n\n");
    }
    else if(RandSystem == false)
    {
        LoadArrays();
        LA = LoadArrays(); //Populate A[] and b[] with initial sets of random binary digits
        if(ShowFullDebug==true)
        {
            if (LA == 1)
            {
                printf("LoadArrays: PASSED\n");
            }
            else
            {
                printf("LoadArrays: FAILED\n");
            }
        }
        printf("This is a preset system.\n\n");
    }
    if (doLinCombs == true)//Linear combinations of A and b aren't always needed even when not reducing a system
    {
        LC1=LinCombs1d(b); //Calculate all possible Linear combinations of the initial equations in A[] and b[] and fill them in the remaining rows of the respective matrices
        LC2=LinCombs2d(A);
        if(ShowFullDebug==true)
        {
            if ((LC1 == 1) && (LC2 == 1))
            {
                printf("LinCombs: PASSED\n");
            }
            else
            {
                printf("LinCombs: FAILED\n");
            }
        }
        //InvertRows(A, Equations);
        PrintLCArrays();
    }
    else
    {
        PrintLCArrays();
        //InvertRows(A, InitEqs);
        PrintLCArrays();
    }
    VariableSearch(A, b, valid, valid, validsols, false);
    //InvertRows(A, InitEqs); //Resetting A to its original state to make sure it reduces properly

    if(reduceSystem==true) //main() follows these instructions if we are trying to reduce the size of the input system before searching for solutions
    {
        ZeroReset();
        SimCheck(A, tempA, reducedA, b, tempb, reducedb, false);
        //InvertRows(reducedA,MaxCombs);
        if (ShowFullDebug == true)
        {
            printf("SimCheck: PASSED\n");
        }
        PrintreducedArrays(false); 
        if (ShowFullDebug == true)
        {
            printf("PrintArrays = PASSED\n");
        }
        printf("Init System reduced 1 time\n..............................\n");
        VariableSearch(reducedA, reducedb, valid, reducedvalid, validsols, true);

        if (RepeatReduce == true)
        {
            for (g = 0; g < (LoopReduce - 1); g++) //upper bound is (LoopReduce-1) because we have already reduced once by this point
            {
                ZeroReset();
                SimCheck(reducedA, tempA, reducedA, reducedb, tempb, reducedb, true); //the last input is set to true so that after tempA and tempb are filled reduceA and reduceb are zeroed out before being repopulated.
                if(ShowFullDebug==true)
                {
                    printf("SimCheck: PASSED\n");
                }               
                PrintreducedArrays(true); //Print the now fully filled A[] and b[]
                if (ShowFullDebug == true)
                {
                    printf("PrintArrays = PASSED\n");
                }
                printf("Init System reduced %d times\n..............................\n",(g+2));
                VariableSearch(reducedA, reducedb, valid, reducedvalid, validsols, true);
            }
        }
    }
    printf("\nEnd Program");
    return 0;   
}
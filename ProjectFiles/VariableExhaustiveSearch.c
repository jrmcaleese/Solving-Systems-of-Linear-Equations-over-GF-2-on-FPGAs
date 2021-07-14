#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "VariableExhaustiveSearch.h"
#include "parameters.h"
#include "SimilarityCheck.h"
#include "ArrayHandling.h"


int f, g, h, i, j, k;
int InvCount = 0; //tracking the number of invalid solutions
int SolCount = 0;
int FinalCount = 0;
int CarryBit=0;
int CoefProd = 0;
int ValidRows = 0; //tracking the number of equations for which a possible solution is valid
int GA, LA, LC1, LC2, PA = 0; //default variables to store return variables from functions so we can be sure they worked

void ZeroReset(void) 
{
    /*
    Resets relevant global variables to required initial conditions
    to avoid possible data errors when solving systems of multiple size
    or when variables don't rewrite properly between loop iterations
    */
    for (i=0;i<Variables;i++)
    {
        x[i]=0;
        y[i]=0;
        invalid[i] = 0;
        valid[i] = 0;
    }
    for (i=0;i<Equations;i++)
    {
        EqSum[i]=0;
    }
    for (i = 0; i < MaxCombs; i++)
    {
        samecoef[i] = 0;
    }
    for (i=0;i<Variables;i++)
    {
        if(i<(Variables-1))
        {
            AddOne[i]=0;
        }
        else
        {
            AddOne[i]=1;
        }
    }
    InvCount=0;
    ValidRows=0;
    SolCount=0;
    FinalCount = 0;
    CarryBit=0;
}

void BinArrayAdd(void)
{
    /*
    Generates the possible solution sets. In this for loop the Variables are treated as bits in a ripple carry adder so we can increment through all solutions. 
    Outside this for loop the carry bit has no effect and the sets are treated as arrays of individual values again.
    */
    for (i=(Variables-1);i>-1;i--) //this loop is a 1-bit ripple CarryBit adder looped into a multi-bit ripple CarryBit adder
    {
        y[i]=(x[i]^AddOne[i])^CarryBit; //XOR addition of the current elements in x and AddOne and then with the carrybit, all stored in y[i]

        if (x[i]==1 && AddOne[i]==1) //if x and AddOne were both 1, the CarryBit bit will be 1 next loop
        {
            CarryBit=1;
        }
        else if (x[i]==1 && CarryBit==1) //if x and the CarryBit bit were both 1, the CarryBit bit will be 1 next loop
        {
            CarryBit=1;
        }
        else if (CarryBit==1 && AddOne[i]==1) //if AddOne and the CarryBit bit were both 1, the CarryBit bit will be 1 next loop
        {
            CarryBit=1;
        }
        else //otherwise the CarryBit bit will be 0 next loop
        {
            CarryBit=0;
        }
        x[i]=y[i]; //now that the CarryBit bit has been assigned, the value from y can be moved into x so the values are in place for the next iteration of the outermost loop

    }  
}

void SystemDeterminance(int CurrentEqs)
{
    /*
    Prints whether or not the current System is under or over determined
    */
    if ((CurrentEqs)<Variables)
    {
        printf("This is an underdetermined system.\n");
    }
    else if((CurrentEqs)>Variables)
    {
        printf("This is an overdetermined system.\n");
    }
    else
    {
        printf("This is a determined system.\n");
    }
}

int GenerateSolutions(int v1[MaxBin],int v2[MaxBin], int vs[MaxBin])
{
    /*
    Listing all together the decimal representations of the solutions for the given system. Makes it easier to actually track patterns in solutions
    */
    int c = 0;
    for (i = 0; i < MaxBin; i++)
    {
        if ((v1[i] == 1)&&(v2[i] == 1))
        {
            vs[c] = (i + 1);
            c++;
        }
        else
        {
            vs[c] = 0;
            c++;
        }
    }
    return c;
}
int PrintSolutions(int arr1[MaxBin],bool repeat)
{
    if(csvFriendly==false)
    {
        if (SolCount > 0)
        {
            int count;
            if (repeat == false)
            {
                count = 1;
            }
            else
            {
                count = 0;
            }
            for (i = 0; i < SolCount; i++)
            {

                if (arr1[i] == 0) //if the value is 0 then we've printed all solutions
                {
                    printf("\n\n");
                    return count;
                }
                printf("%d", arr1[i]);
                if (i == (SolCount - 1)) //if i is one less than SolCount then we've reached the end of the solutions
                {
                    printf("\n\n");
                    return count;
                }
                else if ((i + 1) % 20 == 0) //start a new line every 20 solutions printed
                {
                    printf(",\n");
                }
                else //print a comma between solutions
                {
                    printf(",");
                }
                count++;
            }
        }
        else if (SolCount == 0) //Printing a fail message is a clearer output than leaving blank space in the event of no solutions
        {
            printf("\n-----NO SOLUTIONS FOUND-----\n\n");
        }
    }
    else
    {
        if (SolCount > 0)
        {
            int count;
            if (repeat == false)
            {
                count = 1;
            }
            else
            {
                count = 0;
            }
            //printf("\nThe valid solutions for this system are:\n");
            for (i = 0; i < 1023; i++)
            {

                /*
                if (arr1[i] == 0) //if the value is 0 then we've printed all solutions
                {
                    printf("\n\n");
                    return count;
                }
                */
                printf("%d", arr1[i]);
                if (i == (SolCount - 1)) //if i is one less than SolCount then we've reached the end of the solutions
                {
                    printf("\n\n");
                    return count;
                }
                else //print a comma between solutions
                {
                    printf("\t");
                }
                count++;
            }
        }
        else if (SolCount == 0) //Printing a fail message is a clearer output than leaving blank space in the event of no solutions
        {
            printf("\n-----NO SOLUTIONS FOUND-----\n\n");
        }
    }
    
}

void ExhaustiveSearch(int LHS[][Variables],int RHS[], int v[MaxBin], int FirstEq, int LastEq)
{
    /*
    1. The central search function of the whole program. On each loop, BinArrayAdd() is used to generate a new possible solution set as an array equal in length to the number of varaibles in the system, 
       incrementing all the way up to the MaxBin value. 

    2. The values at each index in the tested solution are then AND multiplied by their respective values in the first equation of matrix arr1[] and those products are XOR added together
       with the final sum stored in an array called EqSum[] at the index equal to the equation number being tested.

    3. This sum is then compared against the value at the same index in b[], and if they are equal the ValidRows count is incremented by 1. 
       Otherwise the InvCount (invalid solution count) is incremented by 1 to show that the solution is not valid for this equation and therefore the system.

    4. Steps 2 and 3 are repeated for all remaining rows in the system. If at the end ValidRows is equal to the number of equations, then the index number of the tested solution (k+1) 
       is stored in the first available spot in the array valid[] and SolCount (a tally of the total number of valid solutions) is incremented by 1.

    5. Steps 1-4 are repeated until all possible solutions have been incremented through and tested.

    6. When debugging is on the index numbers of each solution are printed along with the SolCount and the number of solutions tested.
    */
    int sameones;
    /*
    printf("Current Row = {");
    for (i = FirstEq; i < LastEq; i++)
    {
        for (j = 0; j < Variables; j++)
        {
            printf("%d,", LHS[i][j]);
        }
    }
    printf("}\n\n");
    */
    for(k=0;k<MaxBin;k++) //this largest loop controls the whole process, running through steps 1-4 until all possible solutions have been tested
    {
        BinArrayAdd();
        //printf("Solution tested = %d\n", (k + 1));
        
        for (i=FirstEq;i<LastEq;i++) //for each row/equation in A
        {
            sameones = 0;
            EqSum[i] = 0; //making sure EqSum is zero before calculating for the current row so only the correct answer for this iteration is recorded
            for (j = 0; j < Variables; j++)
            {
                if ((LHS[i][j] == 1) && (x[j] == 1))
                {
                    sameones += 1;
                }
            }

            if (sameones % 2 == 1)
            {
                EqSum[i] = 1;
            }
            else if(sameones % 2 == 0)
            {
                EqSum[i] = 0;
            }
            //printf("EqSum[%d] = %d\n", i, EqSum[i]);
            //printf("  RHS[%d] = %d\n", i, RHS[i]);
            if (EqSum[i] != RHS[i]) //if the equation sum does not equal the b value for this equation, save the current k value + 1 to the array 'invalid', then increment invalidcount
            {
                invalid[InvCount]=(k+1);
                
                InvCount+=1;
                ValidRows=0;
                //printf("Invalid\n");
                break;
            }
            else if(EqSum[i] == RHS[i])//otherwise if the equation some does equal the solution for this equation, increment the ValidRows count
            {
                ValidRows+=1;
                //printf("ValidRows = %d\n", ValidRows);
            }
        }
        if (ValidRows == 1) //if the solution is valid for all rows/equations, then record the k+1 value into 'valid' at the index equal to the current SolCount
        {
            v[k]=1;
            SolCount+=1;
           //printf("Solutions Found = %d\n", SolCount);
            ValidRows=0;
        }
        //printf("\n--------------------\n");
    }
}

void VariableSearch(int LHS[][Variables], int RHS[], int v1[MaxBin], int v2[MaxBin], int vs[MaxBin], int FirstEq, int LastEq, bool repeat) //A,b,valid,reducedvalid,validsols,repeat
{
    ZeroReset(); //Reset variables to ensure the search works properly on each iteration of the for loop
    printf("Row %d:\t", (f+1));
    //printf("For a %d Equation System with %d Variables:\n", (LastEq-FirstEq), Variables); //print the current system size
    ExhaustiveSearch(LHS, RHS, v1, FirstEq, LastEq); //Search for all solutions in a system of this size
    if (ListSols == true) //Print the decimal equivalents of the solutions if desired
    {
        GenerateSolutions(v1, v2, vs);
        FinalCount = PrintSolutions(vs, repeat);
    }
    /*
    SystemDeterminance((Equations - h)); //Print the determinance of the system at it's current size
    printf("Total Solutions, Reduced System: %d out of the %d tested\n\n", FinalCount, MaxBin); //print the total number of solutions found compared to the number tested
    printf("----------------------------------------------------------------------------------------------------\n");
    printf("||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
    printf("----------------------------------------------------------------------------------------------------\n");
    */
    f += 1;
}

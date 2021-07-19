#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "parameters.h"
#include "SimilarityCheck.h"
#include "ArrayHandling.h"

void SimCheck(int arr1[][Variables], int arr2[][Variables], int arr3[][Variables], int arr4[MaxCombs], int arr5[MaxCombs], int arr6[MaxCombs], bool repeat)
{
    /*
    Compares similarity between rows of A, 
    temporarily stores their XOR comparisons in arr2, 
    and stores the resulting list
    of similar/reduced equations in reducedA
    */
    int h, i, j, k;
    int m, n, o = 0;
    int rsize = sizeof(arr1[0]) / sizeof(arr1);
    for(i=0;i<(rsize-1);i++)
    {
        for (j = (i + 1); j < rsize;j++) //iterate through all possible combinations of A and A
        {
            arr5[m] = arr4[i] ^ arr4[j];
            for (k = 0; k < Variables; k++)
            {
                arr2[m][k] = arr1[i][k] ^ arr1[j][k]; //store the linear combination of the two equations in arr2
                if (SimCheckDebug == true)
                {
                    printf("    arr1[%d(i)][%d(k)] = %d\n", i, k, arr1[i][k]);
                    printf("    XOR\n");
                    printf("    arr1[%d(j)][%d(k)] = %d\n---\n", j, k, arr1[j][k]);
                    printf("arr2[%d(m)][%d(k)] = %d\n\n", m, k, arr2[m][k]);
                }
                if (arr1[i][k] == arr1[j][k]) //At least one coefficient needs to be 1 to be relevant
                {
                    samecoef[m] += 1; //Record the number of same coefficients between these two equations
                    if (SimCheckDebug == true)
                    {
                        printf("samecoef[%d] = %d\n\n\n", m, samecoef[m]);
                    }
                }
            }
            if (SimCheckDebug == true)
            {
                printf("TOTAL samecoef[%d] = %d\n----------\n", m, samecoef[m]);
            }
            if (samecoef[m]>=MinSame) //if the number of same coefficients between these two rows is greater than the MinSame, increment n.
            {
                n+=1; //at the end of this loop n will equal the number of all valid reduction lin combs
            }
            if (samecoef[m] == Variables)
            {
                break;
            }
            m += 1;
        }
        if (SimCheckDebug == true)
        {
            printf("samecoef={");
            for (h = 0; h < m; h++)
            {
                printf("%d", samecoef[h]);
                if (h == (m - 1))
                {
                    printf("}\n");
                }
                else
                {
                    printf(",");
                }
            }
            printf("\n\n\n\n");
        }
    }
    
    if (repeat == true)
    {
        for (i = 0; i < rsize; i++)
        {
            for (k = 0; k < Variables; k++)
            {
                arr3[i][k] = 0;
            }
            arr6[i] = 0;
        }
    }

    for (i = 0; i < m; i++) //Now cycle through all the equations again
    {
        if (samecoef[i] >= MinSame) //The values of samecoef correspond to the number of same coefficients for the corresponding equation in arr2, so if the samecoef value is greater than the MinSame the equation is worth logging
        {
            for (k = 0; k < Variables; k++)
            {
                arr3[o][k] = arr2[i][k]; //copy the valid equation into a row in reducedA
                if(SimCheckDebug==true)
                {
                    printf("   arr2[%d][%d] = %d\n", i, k, arr2[i][k]);
                    printf("arr3[%d][%d] = %d\n---\n", o, k, arr3[o][k]);
                }
            }
            arr6[o] = arr5[i];
            if (SimCheckDebug == true)
            {
                printf("   arr5[%d] = %d\n", i, arr5[i]);
                printf("arr6[%d] = %d\n---\n", o, arr6[o]);
            }
            o += 1; //increment o to track the number of valid equations copied into reducedA
        }
        if (o == n)
        {
            break;
        }
    }
}

A Modified Exhaustive Search Algorithm for Solving Systems of Quadratic Equations over GF(2)
Author: James R. McAleese
Last Updated: July 6th, 2021

TO USE: 

1. Edit the following define statements in parameters.h to properly set your values:
    - InitEqs: The original number of equations in your system

    - MaxCombs: The number of possible 2 equation linear combinations for a system with a size equal to InitEqs

    - Equations: The maximum number of equations in the system. Equal to InitEqs + MaxCombs

    - Variables: The number of variables per equation in the system
    
    - MaxBin:The maximum possible decimal value of a binary number with the same number of digits as variables in your system. 
             This is defined manually because C does not natively allow exponentiation without functions and does not allow function calling in declarations/define statements

    - MinSame: The minimum number of coefficients that two equations must have in common to pass the SimCheck2d() function

    -Loop Reduce: The number of times the program attempts to reduce the initial system. Only takes effect if both ReduceSystem and RepeatReduce are TRUE


2. This program can perform a variety of functions on various kinds of systems, toggle the Usage Bools below to control the output:

    -RandSystem : When set to TRUE, InitSystem and InitSol are ignored and instead a random system is generated.
                  When set to FALSE, InitSystem and InitSol are used as program input.

    -doLinCombs: When set to TRUE the linear combinations of the input system are found and appended to the system before solutions are found

    -ReduceSystem: When set to TRUE the program attempts to reduce the complexity of the input system by replacing equations with linear combinations containing fewer coefficients

    -ReduceSearch: When set to TRUE the program uses whatever alternate method of exhaustive search is being tested instead of the default

    -RepeatReduce: WHen set to TRUE the program loops and attempts to repeatedly reduce the system and further decrease complexity

    

------------------------------------------------------------------------------------------------------------------------------------------------------

TO DEBUG:

The program always displays the decimal representation of valid solutions. 
To display more information toggle the Debug Bools below:

    - ShowBinaryValids: When set to TRUE, the program prints the full readout of 1s and 0s of each valid solution to the system

    - ShowFullDebug: When set to TRUE, the program prints the full readout of when each tested solution hits an equation where it isn't valid

    - ListSols: When set to TRUE, the program prints all the valid solutions for a system are printed in a single list at the end of a search

    - csvFriendly: When set to TRUE the solutions of ListSols are printed in a format that is copyable into a csv or excel file.

    - SimCheckDebug: When set to TRUE, the program prints the full readout of the internal math of the SimCheck() function

======================================================================================================================================================
======================================================================================================================================================
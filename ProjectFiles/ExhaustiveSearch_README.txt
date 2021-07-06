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
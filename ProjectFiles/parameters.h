#ifndef PARAMETERS_H
#define PARAMETERS_H


#define InitEqs 5 //The number of initial Equations in the system
#define MaxCombs ((InitEqs*(InitEqs-1))/2) //the maximum number of 2 equation linear combinations from a system the size of InitEqs
#define Equations 5 //(InitEqs+MaxCombs) //The maximum number of equations in the combined system
#define SearchEqs 5 //The number of Equations for the program to start looking through solutions for
#define Variables 10 //The number of variables in the system
#define MaxBin 1023 //The largest binary number possible with "Variables" number of bits.
#define MinSame ((Variables/2)-(Variables/2)%1) //The minimum number of coefficients two equations must have in common to be reduced
#define LoopReduce 5 //If reducing the system, this sets the number of times you wish to repeatedly reduce it

//Usage bools
extern bool RandSystem;
extern bool ReduceSystem;
extern bool ReduceSearch;
extern bool doLinCombs;
extern bool RepeatReduce;


//Debug bools
extern bool ShowBinaryValids; //toggles whether the just the decimal equivalents of the solutions are shown or the full binary solution sets.
extern bool ShowFullDebug; //toggles detailed prextern int statements for each tested solution. When false only valid solutions are displayed.
extern bool ListSols;
extern bool csvFriendly;
extern bool SimCheckDebug;

#endif
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>


double fromInput(const char *, double min, double max);
double FN (double nutot, double t, int THR);
double FP (double nub, double t, int THR);
double poisson(int N, double mu);
double randReal(double max, double min);
int randInt(int max, int min);

double fattoriale(int N);

//established whethere there is a combination with FN<MAXFN and FP<MAXFP 
int canDetect(double nub, double nus, double time, int* THR, double maxfp,double maxfn);
//if it returns 0 the max time is not enough
int findBestTime(double nub,double nus,double *tbest, int* THRbest,double maxfp,double maxfn, double maxt, int tsteps);
int findBestTHR(double nub,double nus,double time,double maxfp,double maxfn);



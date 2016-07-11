#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>

#include "fpfnlib.h"

using namespace std;


double randReal(double min, double max)
{
	return min+(max-min)*(lrand48()+1.)/(RAND_MAX+1.);
}
int randInt(int min, int max)
{
	return min+lrand48()%(max-min+1);
}
//establishes whether there is a combination with FN<MAXFN and FP<MAXFP 
int canDetect(double nub, double nus, double time, int* THR, double maxfp,double maxfn){
	int limit=(int) (nus*time +0.6);
	if(time==10)printf("nub=%lf nus=%lf\n",nub,nus);
	
	if(limit<1) return 0;
	int thr;
	for(thr=1;thr<=limit;thr++){
		*THR=thr;
		double FPos=FP(nub,time,thr);
		double FNeg=FN(nus,time,thr);
		if(time==10)printf("THR=%d FP=%lf FN=%lf\n",thr,FPos,FNeg);
		if((FPos<=maxfp) && ( FNeg<=maxfn)) 
			return 1;//detectable
	}
	return 0;
	
}


int findBestTime(double nub,double nus,double *tbest, int* THRbest,double maxfp,double maxfn,double maxt, int tsteps)
{ 
	int i;
	for(i=0;i<tsteps;i++)
    {
		*tbest=i*maxt/(tsteps-1.);
		if(canDetect(nub,nus,*tbest,THRbest,maxfp,maxfn))return 1;
    }
	return 0;
}


int findBestTHR(double nub,double nutot,double time,double maxfp,double maxfn)
{
	int limit=(int) (nutot*time +0.6);
	double chi2best=1E12;
	int THR;
	int res=0;
	if(limit>1)
    {
		for(THR=1;THR<limit;THR++)
		{
			double chi2=pow(FP(nub,time,THR)-maxfp,2)+pow(FN(nutot,time,THR)-maxfn,2);
			if(chi2<chi2best)
			{
				chi2best=chi2;
				res=THR;
			}
		}
    }
	else
    {
		res=1;
    }
	return res;
}

double FP (double nub, double t, int THR){
	return 1.-FN(nub,t,THR);
}

double FN (double nutot, double t, int THR){
	double res=0;
	int i;
	double mu=nutot*t;
	if(mu<100){
		for(i=0; i<THR; i++) 
		{
			double tmp=poisson(i,mu);
			res+=tmp;
		}
	} else {
		res=0.5+erf((THR-mu)/sqrt(2*mu))/2;
	}
	return res;
}

double poisson(int N, double mu)
{
	double res;
	if(N<20 && mu<20){
		double expo=exp(-mu);
		
		double ratio=1.;
		for(int k=1;k<=N;k++)//RF c'era un errore!! k<N ...
		{
			ratio*=mu/k;
		}
		res=expo*ratio;
		printf("mu=%lf, N=%d pois=%lf erf=%lf\n",mu,N,res,(erf((N+0.5-mu)/sqrt(2*mu))-erf((N-0.5-mu)/sqrt(2*mu)))/2.);
		if(res<=0 || res >1){
			printf("something is rotten in poisson(%d, %lf)=%lf\n",N,mu,res);
			exit(10);
		}
	} else {
		res=(erf((N+0.5-mu)/sqrt(2*mu))-erf((N-0.5-mu)/sqrt(2*mu)))/2.;
	}
	
	return res;
	
}

double fattoriale (int N){
	if(N<=1) return 1;
	double f=1;
	int i;
	for (i=2; i<=N; i++) f*=i;
	return f;
}

double fromInput(const char *s, double min , double max) {
	printf("Dammi %.2lf <= %s <= %.2lf\n",  min, s,max);
	double var;
	do { 
		scanf("%lf",&var);
		if(var < min || var > max) printf("Il valore inserito e' fuori range, reinserirlo!\n");
	} while(var < min || var > max); //la funzione continua a chiedere anche nei casi in cui i valori inseriti coincidono con gli estremi
	return var;
}




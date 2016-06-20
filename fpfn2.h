#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>

#include "myStod.h"
#include "myStoi.h"

#define MAXTIME 10.
#define MAXTHR 100.
#define NMC 10000

double fromInput(const char *, double min, double max);
double FN (double nutot, double t, int THR);
double FP (double nub, double t, int THR);
double poisson(int N, double mu);
double randReal(double max, double min);
int randInt(int max, int min);

double fattoriale(int N);
void findBestTime(double nub,double nutot,double *tbest, int* THRbest);
int findBestTHR(double nub,double nutot,double time);


double randReal(double min, double max)
{
  return min+(max-min)*(lrand48()+1.)/(RAND_MAX+1.);
}
int randInt(int min, int max)
{
  return min+lrand48()%(max-min+1);
}

void findBestTime(double nub,double nutot,double *tbest, int* THRbest)
{
  double t,chi2best=1E12;
  int THR,i;
  //	double T;
  for(i=0;i<NMC;i++)
    {
      t=randReal(0,MAXTIME);
      THR=randInt(1,MAXTHR);
      double chi2=pow(FP(nub,t,THR)-0.01,2)+pow(FN(nutot,t,THR)-0.01,2);
      if(chi2<chi2best)
	{
	  chi2best=chi2;
	  *tbest=t;
	  *THRbest=THR;
	}
      //					printf("t=%lf THR=%d FP=%e FN=%e\n",t,THR,FP(nub,t,THR),FN(nutot,t,THR));
      
    }
}


int findBestTHR(double nub,double nutot,double time)
{
  /* cout<<"in f"<<endl; */
  /* cout<<"nutot: "<<nutot<<endl; */
  /* cout<<"time: "<<time<<endl; */
  int limit=(int) (nutot*time +0.6);
  /* cout<<"limit: "<<limit<<endl; */
  double chi2best=1E12;
  int THR;
  int res=0;
  //  for(i=0;i<NMC;i++)
  if(limit>1)
    {
      for(THR=1;THR<limit;THR++)
	{
	  /* cout<<"THR: "<<THR<<endl; */
	  /* cout.flush(); */
	  //      THR=randInt(1,MAXTHR);
	  double chi2=pow(FP(nub,time,THR)-0.001,2)+pow(FN(nutot,time,THR)-0.01,2);
	  if(chi2<chi2best)
	    {
	      chi2best=chi2;
	      res=THR;
	    }
      //					printf("t=%lf THR=%d FP=%e FN=%e\n",t,THR,FP(nub,t,THR),FN(nutot,t,THR));
	}
    }
  else
    {
      res=1;
    }
  return res;
}

double FP (double nub, double t, int THR){
	/* double res=0; */
	/* int i; */
	/* for (i=0; i<THR; i++) res+=poisson(i,nub*t); */
	/* return 1-res; */
  return 1.-FN(nub,t,THR);
}

double FN (double nutot, double t, int THR){
	double res=0;
	int i;
	for(i=0; i<THR; i++) 
	  {
	    double tmp=poisson(i,nutot*t);
	    //	    printf("FN: %d \t %e \n",i,tmp);

	    //  if(!isnan(tmp))
	    res+=tmp;
	  }
	return res;
}

double poisson(int N, double mu)
{
  double expo=exp(-mu);
  /* double pot=pow(mu,N); */
  /* double fact=fattoriale(N); */
  /* double res=0.; */
  /* res=expo*pot/fact; */
  /* printf("poisson: %e \t %e \t %e \t %e \n",res,expo,pot,fact); */
  //  return exp(-mu)*pow(mu,N)/fattoriale(N);

  double ratio=1.;
  for(int k=1;k<N;k++)
    {
      //      printf("ratio: %e \t %d \n",mu,i);
      ratio*=mu/k;
    }
  //  printf("poisson: %e \t %e \n",expo,ratio);
  return expo*ratio;

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



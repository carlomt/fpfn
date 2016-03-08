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

using namespace std;

double fromInput(const char *, double min, double max);
double FN (double nutot, double t, int THR);
double FP (double nub, double t, int THR);
double poisson(int N, double mu);
double randReal(double max, double min);
int randInt(int max, int min);

double fattoriale(int N);
void findBestTime(double nub,double nutot,double *tbest, int* THRbest);
void findBestTHR(double nub,double nutot,double time, int* THRbest);

void print_help(string fname="executable");

int main(int argc, char *argv[])
{
  
  cout<<argv[0]<< " v: 2.1"<<endl;
  cout<<"Last edit:   Mar 8 2016."<<endl;
  cout<<"Compiled at: "<< __DATE__ <<", "<< __TIME__<<"."<<endl;
  
  string execname=argv[0];
  string ofname="output.txt";

  double nub0=-1.;
  double nut0=-1;
  
  int steps=100;
  double timelimit=10.;

  vector<double> nums;
  
  if(argc==1)
    {
      print_help(argv[0]);
      exit(0); 
    }
  else
    {
      for(int i=1;i<argc;i++)
	{
	  //	  cout<<"argv["<<i<<"]: "<<argv[i]<<endl;
	  if(argv[i][0] =='-')
	    {
	      string option(argv[i]);
	      cout<<"option: "<<i<<" "<<option<<endl;
	      if(option.compare("-help")==0)
		{
		  print_help(argv[0]);
		  exit(0); 
		}
	      else if(option.compare("-t")==0)
		{
		  //		  steps=myStoi(argv[++i])*10;
		  string tmp=argv[++i];
		  timelimit=myStod(tmp);
		  cout<<"time limit: "<<timelimit<<" s."<<endl;
		}
	      else if(option.compare("-s")==0)
		{
		  string tmp=argv[++i];
		  steps=myStoi(tmp);
		  cout<<"number of steps: "<<steps<<endl;
		}
	      else if(option.compare("-o")==0)
		{
		  ofname=argv[++i];
		}
	      else
		{
		  cout<<"option not recognized: "<<option<<endl;
		  exit(-1);
		}
	    }
	  else
	    {
	      nums.push_back(myStod(argv[i]));
	    }
	}//end loop args
    }
  

  if(nums.size()!=2)
    {
      cout<<"ERROR: you passed "<<nums.size()<<"numbers while 2 where expected."<<endl;
      exit(-1);
    }
  if(nums[0]<nums[1])
    {
      nub0=nums[0];
      nut0=nums[1];
    }
  else
    {
      nub0=nums[1];
      nut0=nums[0];
    }
  
  cout<<"Frequency on tumour: "<<nut0<<" Hz."<<endl;
  cout<<"Frequency on healty tissue: "<<nub0<<" Hz."<<endl;

  srand48(time(0));
  
  double thr=24.;
  double time=1.;

  FILE *f = fopen(ofname.c_str(), "w");
  
  if(f == NULL)
    {
      printf("Error opening file!\n");
      exit(1);
    }
  fprintf(f,"#time \t FP \t FN \n");

  double stepsize=timelimit/steps;
  cout<<"step size: "<<stepsize<<endl;
  for(int i=1; i<=steps; i++)
    {
      time=i*stepsize;
      
      thr*=time;
      
      int THRbest;
      
      findBestTHR(nub0,nut0,time, &THRbest);
      
      double fp=FP(nub0,time,THRbest);
      double fn= FN(nut0,time,THRbest);
      
      fprintf(f,"%lf \t %e \t %e \t %d \n",time,fp,fn,THRbest);
    }
  
  fclose(f);
  
		
}

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


void findBestTHR(double nub,double nutot,double time, int* THRbest)
{
  /* cout<<"in f"<<endl; */
  /* cout<<"nutot: "<<nutot<<endl; */
  /* cout<<"time: "<<time<<endl; */
  int limit=(int) (nutot*time +0.6);
  /* cout<<"limit: "<<limit<<endl; */
  double chi2best=1E12;
  int THR;
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
	      *THRbest=THR;
	    }
      //					printf("t=%lf THR=%d FP=%e FN=%e\n",t,THR,FP(nub,t,THR),FN(nutot,t,THR));
	}
    }
  else
    {
      *THRbest=1;
    }
  return;
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


void print_help(string fname)
{
  cout<<"Source: "<<__FILE__<<endl;
  cout<<endl;
  cout<<"Usage  : "<<fname<<" (option) freqBkg freqSig"<<endl;
  //  cout<<"Option : -verbose  (show debug output)"<<endl;
  cout<<"Option : -o set output filename (default \'output.txt\')"<<endl;
  cout<<"Option : -t set max time in sec for the loop (default: 10 sec) "<<endl;
  cout<<"Option : -s set number of loop steps (default: 100) "<<endl;
  cout<<"Option : -help     (show this help)"<<endl;
  //    printf("       : -log (Log filename)\n"); 
  cout<<endl;

  return;
}

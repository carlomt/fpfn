#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXTIME 10.
#define MAXTHR 100.
#define NMC 100000

double fromInput(const char *, double min, double max);
double FN (double nutot, double t, int THR);
double FP (double nub, double t, int THR);
double poisson(int N, double mu);
double randReal(double max, double min);
int randInt(int max, int min);

double fattoriale(int N);
void findBestTime(double nub,double nutot,double *tbest, int* THRbest);
void findBestTHR(double nub,double nutot,double time, int* THRbest);
int main(){
	srand48(time(0));
	/* double nub0=fromInput("background rate",0,1E12); */
	/* double nut0=fromInput("signal rate",nub0,1E12); */
	
	double nub0=12.;
	double nut0=28.;
	double thr=24.;
	double time=1.;

	FILE *f = fopen("output.txt", "w");
	if(f == NULL)
	  {
	    printf("Error opening file!\n");
	    exit(1);
	  }
	fprintf(f,"#time \t FP \t FN \n");
	for(int i=1; i<=30; i++)
	  {
	    time=i*.1;
	    //time=7.0;
	    thr*=time;
	    
	    //	    double tbest;
	    int THRbest;
	    //	    findBestTime(nub0,nut0,&tbest, &THRbest);
	    findBestTHR(nub0,nut0,time, &THRbest);

	    /* double fp=FP(nub0,time,thr); */
	    /* double fn= FN(nut0,time,thr); */
	    /* double fp=FP(nub0,tbest,THRbest); */
	    /* double fn= FN(nut0,tbest,THRbest); */
	    double fp=FP(nub0,time,THRbest);
	    double fn= FN(nut0,time,THRbest);

	    /* printf("data frequenza segnale %lf Hz e frequenza rumore %lf Hz\n",nut0,nub0); */
	    /* printf("con soglia a %lf conteggi e tempo %lf sec.\n",thr,time); */
	    /* printf("esempio FP=%lf FN=%lf\n",fp,fn); */
	    //	    fprintf(f,"%lf \t %e \t %e \t %lf \n",time,fp,fn,thr);
	    //	    fprintf(f,"%lf \t %e \t %e \t %d \n",tbest,fp,fn,THRbest);
	    fprintf(f,"%lf \t %e \t %e \t %d \n",time,fp,fn,THRbest);
	  }

	fclose(f);

	/* double tbest; */
	/* int THRbest; */
	/* findBestTime(nub0,nut0,&tbest, &THRbest); */
	/* printf("the best is for t=%lf THR=%d\n",tbest,THRbest); */

	/* printf("er mejo FP=%lf FN=%lf\n",FP(nub0,tbest,THRbest),FN(nut0,tbest,THRbest)); */
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
  double t,chi2best=1E12;
  int THR,i;
  for(i=0;i<NMC;i++)
    {
      THR=randInt(1,MAXTHR);
      double chi2=pow(FP(nub,time,THR)-0.001,2)+pow(FN(nutot,time,THR)-0.01,2);
      if(chi2<chi2best)
	{
	  chi2best=chi2;
	  *THRbest=THR;
	}
      //					printf("t=%lf THR=%d FP=%e FN=%e\n",t,THR,FP(nub,t,THR),FN(nutot,t,THR));
      
    }
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

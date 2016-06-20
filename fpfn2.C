#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>

#include "myStod.h"
#include "myStoi.h"

#include "fpfn2.h"

using namespace std;

void print_resume();
void print_help(string fname="executable");

int main(int argc, char *argv[])
{
  
  cout<<argv[0]<< " v: 2.2"<<endl;
  cout<<"Last edit:   Jun 4 2016."<<endl;
  cout<<"Compiled at: "<< __DATE__ <<", "<< __TIME__<<"."<<endl;
  
  string execname=argv[0];
  string ofname="output.txt";

  double nub0=-1.;
  double nut0=-1;
  
  int steps=100;
  double timelimit=5.;

  vector<double> nums;

  double firstTimeBelow01Time=-1., firstTimeBelow01FP=-1., firstTimeBelow01FN=-1., firstTimeBelow01THR=-1.;
  bool firstTimeBelow01Setted=false;
  
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
      
      int THRbest=findBestTHR(nub0,nut0,time);
      
      double fp=FP(nub0,time,THRbest);
      double fn= FN(nut0,time,THRbest);
      
      fprintf(f,"%lf \t %e \t %e \t %d \n",time,fp,fn,THRbest);
      if(!firstTimeBelow01Setted)
	{
	  if(fp<=0.01 && fn<=0.01 && fp>0 && fn>0)
	    {
	      firstTimeBelow01Setted=true;
	      firstTimeBelow01Time=time;
	      firstTimeBelow01FP=fp;
	      firstTimeBelow01FN=fn;
	      firstTimeBelow01THR=THRbest;
	    }
	}
    }
  
  fclose(f);
  print_resume();
  if(firstTimeBelow01Setted)
    {
      cout<<" After "<<firstTimeBelow01Time<<" sec. the probability of false positive and false negative are below 1%"<<endl;
      cout<<" FP: "<<firstTimeBelow01FP<<endl;
      cout<<" FN: "<<firstTimeBelow01FN<<endl;
      cout<<" THR: "<<firstTimeBelow01THR<<endl;
    }
  else
    {
      cout<<" After "<<timelimit<<" sec. the probability of false positive and false negative are NOT below 1%"<<endl;
      cout<<"try to run again increasing time limit (option -t)."<<endl;
      
    }
}

void print_help(string fname)
{
  cout<<"Source: "<<__FILE__<<endl;
  cout<<endl;
  cout<<"Usage  : "<<fname<<" (option) freqBkg freqSig"<<endl;
  //  cout<<"Option : -verbose  (show debug output)"<<endl;
  cout<<"Option : -o set output filename (default \'output.txt\')"<<endl;
  cout<<"Option : -t set max time in sec for the loop (default: 5 sec) "<<endl;
  cout<<"Option : -s set number of loop steps (default: 100) "<<endl;
  cout<<"Option : -help     (show this help)"<<endl;
  //    printf("       : -log (Log filename)\n"); 
  cout<<endl;
  print_resume();
  cout<<endl;
  return;
}

void print_resume()
{
  cout<<"To plot results open gnuplot and type:"<<endl<<"l 'plot.gp'"<<endl;
}

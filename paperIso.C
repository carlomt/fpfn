#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>

#include "myStod.h"
#include "myStoi.h"

#include "fpfnlib.h"
#define MAXFP 0.01
#define MAXFN 0.05
//Referece mass specific activity
#define AREF 3.0
//Maximum allowed time for detection
#define TREF 1.0
//Maximum and minimum considered SUV, steps and reference SUV
#define MINSUV 1
#define MAXSUV 8
#define SUVSTP 8
#define SUVREF 4
//Maximum considered SUV and reference SUV
#define MINTNR 4
#define MAXTNR 30
#define TNRSTP 14
#define TNRREF 4
//Number of isotopes (dovrebbe essere 12 a meno di test)
#define NISO 12

using namespace std;

void print_resume();
void print_help(string fname="executable");
int readinput(string filename, string iso[], double RFAR[NISO],double RTUM[NISO], double RHT[NISO]);

int main(int argc, char *argv[])
{
    double AMIN,RFAR[NISO], RTUM[NISO],  RHT[NISO];//Rates on tumor and healthy tissues
	string iso[NISO];
	string execname=argv[0];
	string ifname="input.dat";
	string ofname="output";
	
	int i, steps=1000;
	double timelimit=25.;
	
	vector<double> nums;
	
	
	
	for( i=1;i<argc;i++)
	{
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
				print_help(argv[0]);
				exit(-1);
			}
		}
		else
		{
			cout<< "non recognized argument"<<argv[++i]<<endl;
			print_help(argv[0]);
			exit(-2); 
		}
	}//end loop args
	/*
	double tt;int a;
	findBestTime(10,40,&tt,&a,MAXFP,MAXFN,timelimit,steps);
    printf("Amin=%lf\n",3*tt);
	exit(9);
	*/
	if(!readinput(ifname, iso, RFAR,RTUM,  RHT)){
		printf("error in input file format\n");
		exit(-3);
	}
	int is;
	for (i=0; i<NISO; i++) {
		printf("processing isotope %s\n",iso[i].c_str());
		for(is=0;is<SUVSTP;is++){
			double SUV=(SUVSTP==1)?MINSUV:MINSUV+(MAXSUV-MINSUV)*is/(SUVSTP-1.);
			char filename[80];
			sprintf(filename,"%s_%s_%d", ofname.c_str(),iso[i].c_str(),(int)SUV);
			FILE *f = fopen(filename, "w");
			
			if(f == NULL)
			{
				printf("Error opening file!\n");
				exit(1);
			}
			int it;
			fprintf(f,"# TNR \t\t Min Activity\n");
			for(it=0;it<TNRSTP;it++){
				double TNR=MINTNR+it*(MAXTNR-MINTNR)/(TNRSTP-1);
				//printf("processing SUV=%lf TNR=%lf\n",SUV,TNR);
				double tmin;
				int thr;
				double rht=RFAR[i]+RHT[i]*(SUV/TNR)/(SUVREF/TNRREF);
				double rtum=RFAR[i]+RTUM[i]*(SUV/SUVREF);
				findBestTime(rht,rtum,&tmin,&thr,MAXFP,MAXFN,timelimit,steps);
				AMIN=tmin*AREF/TREF;
				if(SUV==4 && TNR==8)printf("tmin, Amin %10.2lf %10.2lf\n",tmin,AMIN);

				fprintf(f,"%10.3lf %10.3lf\n",TNR,AMIN);
			}
			/*
			 */
			fclose(f);
		}//loop over SUV
	}//loop over isotopes
}

void print_help(string fname)
{
	cout<<"Source: "<<__FILE__<<endl;
	cout<<endl;
	cout<<"Usage  : "<<fname<<" (option) freqBkg freqSig"<<endl;
	cout<<"Option : -o set output filename (default \'output.txt\')"<<endl;
	cout<<"Option : -t set max time in sec for the loop (default: 5 sec) "<<endl;
	cout<<"Option : -s set number of loop steps (default: 100) "<<endl;
	cout<<"Option : -help     (show this help)"<<endl;
	cout<<endl;
	print_resume();
	cout<<endl;
	return;
}

void print_resume()
{
	cout<<"To plot results open gnuplot and type:"<<endl<<"l 'plot.gp'"<<endl;
}

int readinput(string filename, string iso[NISO],double RFAR[NISO], double RTUM[NISO],double RHT[NISO]){
	FILE *f = fopen(filename.c_str(), "r");
	
	if(f == NULL)
    {
		printf("Error opening input file %s!\n",filename.c_str());
		return 0;
    }
	char name[100];
	if(fscanf(f,"%s%s%s%s", name,name,name,name)==EOF)return 0;
	int i;
	for(i=0;i<NISO;i++){
		if(!fscanf(f,"%s%lf%lf%lf", name,&RFAR[i],&RTUM[i],&RHT[i]))return 0;
		iso[i]=name;
	}
	return 1;
}

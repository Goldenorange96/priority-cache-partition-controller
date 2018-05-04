#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include "papi.h"
#include <sys/time.h>
#include <fstream>
#define RAM 192
#define NOTH 8

int rest;
using namespace std;
	
typedef struct info {
	
	string comment;
	int pid;
	int nice;
	int prio;
	long long int cacheMiss;

}INFO;


int calcEqualPartSize(int index)
{	
	if (index > 0)
	{
		rest = RAM%index;
		return (RAM /index);
	}
	return 0;
}

void calcBinRange(int partSize, int part, string &strRange, int amount)
{
	char tmp [32];
	int lo = part * partSize;
	int hi;
	if(rest > 0 && amount == part)
	{hi = ((part + 1) * partSize) +rest -1;}
	else
	{hi = ((part + 1) * partSize) -1;}
	snprintf(tmp, 10, "%d-%d", lo, hi);
	//strcat(strRange, tmp);
	strRange+= tmp;
	//cout << endl<<tmp << endl;
	return;	
}


string generatePartitionsCommand (string binRange, int part, int pid)
{
	//cd /sys/fs/cgroup/cpuset/; echo 2954 > part1/cgroup.procs;cd /sys/fs/cgroup/palloc/; echo 2954 > part1/cgroup.procs;echo 0-32670 > 		part1/palloc.bins

	string cmd = "cd /sys/fs/cgroup/cpuset/; echo " + to_string (pid) + " > part" + to_string(part) + "/cgroup.procs;cd /sys/fs/cgroup/palloc/; echo "+ to_string (pid) + " > part"+ to_string(part)  +"/cgroup.procs;echo " + binRange + " > part" + to_string(part) +"/palloc.bins";
	
	return cmd;
}


void doPartition (int amount, int size, INFO pids[])
{
	string tmpStr;	
	//echo unique PID to each cgroup.procs for each partition. 
	if(amount > 0)
	{
		for (int i = 0; i < amount; i++)
		{	
			tmpStr = "";
			calcBinRange(size, i, tmpStr, amount-1);
			tmpStr = generatePartitionsCommand(tmpStr, i+1, pids[i].pid);
			//system(tmpStr.c_str());
			cout << tmpStr << endl;
		}
	}
	return;
}

/*void createPartitions (int amount)
{
	string pallocPath = "mkdir /sys/fs/cgroup/palloc/part0";
	string cpusetPath = "mkdir /sys/fs/cgroup/cpuset/part0";
	stringstream convert;

	for (int i = 0; i < amount; i++)
	{
		convert << (i+1);
		pallocPath.erase (pallocPath.end()-1) ;
		pallocPath += convert.str();
		cpusetPath.erase (cpusetPath.end()-1);
		cpusetPath += convert.str();
		convert.str("");
		system(pallocPath.c_str());
		system(cpusetPath.c_str());	
	}
	return;
}*/


void assignoprio(int amount, INFO oprio[])
{
	for(int i = 0; i< amount; i++)
	{
		string path = "sudo sh assignOrio.sh ";
		char pid[8];
		snprintf(pid, 8 ,"%d", oprio[i].pid);
		path += pid;
		//system(path.c_str());
		cout << path.c_str() << "\n";
	}

}

void *cacheMisses (void *arg)
{
	INFO *pids = (INFO *) arg;
	int pid = pids->pid;
	long long values[1];
	int retval = 0;
	int EventSet = PAPI_NULL;
	pids->cacheMiss = 0;

	//cout << "Recording cache misses for pid: " << pid << endl;	

	if((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT )
    		{std::cout << "Papi error" << retval;}

       if ( (retval = PAPI_create_eventset(&EventSet)) != PAPI_OK)
		{std::cout << "Papi error" << retval;}
              

       if ((retval = PAPI_assign_eventset_component(EventSet, 0)) != PAPI_OK)
             {printf("2\n");}

       /* Add Total Instructions Executed to the EventSet */
		//std::cout << i << std::endl;
		if ( (retval = PAPI_add_event(EventSet, PAPI_L3_TCM)) != PAPI_OK)
             		 	{printf("3\n");}
       	

   
       //printf("There are %d events in the event set\n", (unsigned int)number);t 

       if ((retval = PAPI_attach(EventSet, pid)) != PAPI_OK)
		{printf("4\t PAPI error code: %s\n", PAPI_strerror(retval));}
            
   	/* Start counting */
	
	 if ( (retval = PAPI_start(EventSet)) != PAPI_OK)
                  {printf("5\n");}

	PAPI_reset (EventSet);
	usleep(1000000);
	
	if ( (retval = PAPI_stop(EventSet, values)) != PAPI_OK)
             	{printf("6\n");}
	
        if ( (retval=PAPI_read(EventSet, values)) != PAPI_OK)
		{printf("7\n");}
           	
   	
        pids->cacheMiss = values[0];
	PAPI_cleanup_eventset(EventSet);
	//cout << pids -> pid << ": "<<pids->cacheMiss << endl;
	//PAPI_destroy_eventset(EventSet);
	//g++ -std=c++11 -o mon monitore.cpp /home/jj/Downloads/papi/src/libpapi.a

	pthread_exit(NULL);

}

int calculateTotalCM(int noProcs, INFO * procs)
{
	long long int totalCM = 0;
	for(int i = 0; i < noProcs; i++)
	{
		totalCM += procs[i].cacheMiss;
	}
	return totalCM;
}

void repartition (int amount, INFO * pids, int totalCM)
{
	int total_cacheM = 0;
	int bins = 0;
	int first = 0;
	int last = 0;
	string execute;
	float procent;
	for (int i = 0; i < amount; i++)
	{
		//cout << "PID for element " << i << " is " << pids[i].pid << endl;	
		bins = 0;
		string range="";
		if(pids[i].cacheMiss > 0)
		{	procent = (pids[i].cacheMiss/ (float)totalCM);
			bins = floor((pids[i].cacheMiss/ (float)totalCM)*192);
			last = first + bins;
			if (i+1 == amount)
				{range = to_string(first) + "-"+ to_string(192);}
			else
				{range = to_string(first) + "-"+ to_string(last);}
			first = last+1;
			//cout << "Range for PID " << pids[i].pid << ": " << range << "procent: "<< procent << endl;
		}
		else	
		{range = "0";}
		execute = generatePartitionsCommand(range, i+1, pids[i].pid);
		//system(execute.c_str());
		//cout << execute << endl;
		sleep(2);
		//cout << range << endl;
		
	}
	
		


return;
}



int main ()
{	
	FILE *fp = NULL;
	char out[30][20];
	int partSize;
	stringstream convert;
	pthread_t thread[NOTH];
	system("sudo sh initPalloc.sh");
	while(1)
	{
		//read stream 
		INFO *newprio = new INFO[8];
		INFO *newOprio = new INFO[8];
		int i = 0;
		int index = 0;
		int prio = 0;
		int oPrio = 0;
		fp = popen("top -bn1 | head  -n15 | tail -n8 | awk '{print$1}{print$3}{print$4}{print$12}'", "r");
		if (fp != NULL)
		{
			while ((fgets (out[i], sizeof(out)-1, fp)) != NULL)
			{
				//printf("%s", out[i]);		
				i++;
			}

		}
		//convert the data to struct		
		string tmp;;
		int niceTmp;
		for(int j = 0; j<i; j+=4 )
		{
	
			tmp = out[j+3];
			tmp.erase (tmp.end()-1);  //dubbel checka!!!!!!!!!!!!!!!!!!!

			if(tmp.compare("sobel_cpu_+") == 0)
			{
				niceTmp  = atoi(out[j+2]);
				//printf("NI: %d\n", niceTmp);
				if(niceTmp < 0)
				{
					//printf("true\n");
					newprio [prio].pid = atoi(out[j]);
					newprio [prio].prio = atoi(out[j+1]);
					newprio [prio].nice = atoi(out[j+2]);
					newprio[prio].comment = tmp;
					prio++;
				
				}
				else
				{
					newOprio [oPrio].pid = atoi(out[j]);
					newOprio [oPrio].prio = atoi(out[j+1]);
					newOprio [oPrio].nice = atoi(out[j+2]);
					newOprio[oPrio].comment=tmp;
					oPrio++;
				}
				index ++;
			}
		}
		
		int retV;
		for(int i = 0; i < index; i++)
		{	
			retV = pthread_create(&thread[i], NULL, cacheMisses, (void *)&newprio[i]);
			if(retV)
			{
				cout << "can't create threads\n";
			}
		}
		retV = 0;
		for(int i = 0; i < index; i++)
		{
			retV = pthread_join(thread[i],NULL);
			if(retV)
			{
				cout << "can't joint the threads\n";
			}
		}
/*
		if((retV = calculateTotalCM(prio, newprio)) < 1)
		{
			cout << "No cache misses were recorded" << endl;
			partSize = calcEqualPartSize(prio);
			doPartition (prio, partSize, newprio);
		}	
		else
		{
			repartition(prio, newprio, retV);
		}
		assignoprio(oPrio, newOprio);*/


			repartition(prio, newprio, calculateTotalCM(prio, newprio));
		
		
		
		//cout << newprio[0].cacheMiss << endl;

		//1. Attach PAPI monitoring to prio-processes. 
		//2. Start counting.
		//3. Sleep
		//4. Read Counters, store in file or smt, mark what pid it comes from. 
		//5. Reset counters. 

		//step 3. Calculate partition size

		//step 4. Assign PIDs and Bins to new partitions.
	//doPartition (prio, partSize, newprio);
	
		//int tt [3] = {100, 50, 70};

		//step 5. Sleep for one second.
		
		sleep(1);

		//DISPATCH AND SYNC THREADS
	}
	
	return 0;
}








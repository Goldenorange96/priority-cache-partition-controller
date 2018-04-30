#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <papi.h>
#include <pthread.h>
#define RAM 192


int rest;


typedef struct info {
	
	char comment[30];
	int pid;
	int nice;
	int prio;

}INFO;


int calcRepart (int index)
{	
	if (index > 0)
	{
	rest = RAM%index;
	int partSize = (RAM /index);
	return partSize;
	}
	return 0;
}

void calcBinRange(int partSize, int part, char* strRange, int amount)
{
	char* tmp = malloc(32);
	int lo = part * partSize;
	int hi;
	if(rest > 0 && amount == part)
	{hi = ((part + 1) * partSize) +rest -1;}
	else
	{hi = ((part + 1) * partSize) -1;}
	snprintf(tmp, 10, "%d-%d", lo, hi);
	strcat(strRange, tmp);
	free(tmp);
	//printf("%s	%d\n", strRange, rest);
	return;	
}

void doPartition (int amount, int size, INFO pids[])
{
	/*
	echo unique PID to each cgroup.procs for each partition. 
	*/
	if(amount > 0)
	{
	char *basePathCpu = (char*)malloc(sizeof(char)*50);
	char *basePathPalloc = (char*)malloc(sizeof(char)*50);
	strcpy(basePathCpu, "cd /sys/fs/cgroup/cpuset/; echo ");
	strcpy(basePathPalloc, "cd /sys/fs/cgroup/palloc/; echo ");
	char pid[8];
	char binRange[16];
	char partNum[9] = " > part0";
	char fileProc[16] = "/cgroup.procs;";
	char fileBins[32] = "/palloc.bins";
	
	char *tmpStr, *tmpStr2, *tmpStr3;
	for (int i = 0; i < amount; i++)
	{	
		tmpStr = (char*)malloc(sizeof(char)*256);
		partNum[7] = (i+1) + '0';
		snprintf(pid, 8 ,"%d", pids[i].pid);
		strcpy(tmpStr, basePathCpu);
		strcat(tmpStr, pid);
		strcat(tmpStr, partNum);
		strcat(tmpStr, fileProc);
		strcat(tmpStr, basePathPalloc);
		strcat(tmpStr, pid);
		strcat(tmpStr, partNum);
		strcat(tmpStr, fileProc);
		strcat(tmpStr, "echo ");
		calcBinRange(size, i, tmpStr, amount-1);
		strcat(tmpStr, partNum);
		strcat(tmpStr, fileBins);
		//printf("%s\n", tmpStr);
		system(tmpStr);
		free(tmpStr);
		tmpStr = NULL;		
		/*free(tmpStr2);
		tmpStr2 = NULL;*/
	}
	}
	return;
}


void createPartitions (int amount)
{
	char pallocPath [40] = "mkdir /sys/fs/cgroup/palloc/part0";
	char cpusetPath [40] = "mkdir /sys/fs/cgroup/cpuset/part0";
	int arrsizepalloc = strlen(pallocPath);
	int arrsizeCpusets = strlen(cpusetPath);
	for (int i = 0; i < amount; i++)
	{
		pallocPath[arrsizepalloc-1] = (i+1) + '0';
		cpusetPath[arrsizeCpusets-1]= (i+1) + '0';
		system(pallocPath);
		system(cpusetPath);	
	}
	return;
}

void removeExistParts()
{
	system("rmdir /sys/fs/cgroup/cpuset/part*; rmdir /sys/fs/cgroup/palloc/part*");
}

void assignoprio(int amount, INFO oprio[])
{
	printf("\n%d\n", amount);
	for(int i = 0; i< amount; i++)
	{
		char path [30] = "sudo sh assignOrio.sh ";
		char pid[8];
		snprintf(pid, 8 ,"%d", oprio[i].pid);
		strcat (path, pid);
		//printf("%s\n", path);
		system(path);
	}


}


int main ()
{
	
	FILE *fp = NULL;
	char out[30][20];
	system("sudo sh initPalloc.sh");
	while(1)
	{
		//read stream 
		INFO *new = (INFO*)malloc(sizeof(INFO)*8);
		INFO *newOprio = (INFO*)malloc(sizeof(INFO)*8);
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
		char tmp[30];
		int niceTmp;
		for(int j = 0; j<i; j+=4 )
		{
	
			strcpy(tmp,out[j+3] );
			tmp[strcspn(tmp, "\n")] = '\0';
			//printf("temp: %s\n\n",tmp);
			if(strcmp(tmp, "sobel_cpu_+") == 0)
			{
				//strcpy(niceTmp,out[j+2] );
				//niceTmp[strcspn(niceTmp, "\n")] = '\0';
				niceTmp  = atoi(out[j+2]);
				printf("NI: %d\n", niceTmp);
				if(niceTmp < 0)
				{
					printf("true\n");
					new [prio].pid = atoi(out[j]);
					new [prio].prio = atoi(out[j+1]);
					new [prio].nice = atoi(out[j+2]);
					strcpy(new[prio].comment,tmp );
					prio ++;
				
				}
				else
				{
					newOprio [oPrio].pid = atoi(out[j]);
					newOprio [oPrio].prio = atoi(out[j+1]);
					newOprio [oPrio].nice = atoi(out[j+2]);
					strcpy(newOprio[oPrio].comment,tmp );
					oPrio++;
				}
				index ++;
			}
		}

		//1. Attach PAPI monitoring to prio-processes. 
		//2. Start counting.
		//3. Sleep
		//4. Read Counters, store in file or smt, mark what pid it comes from. 
		//5. Reset counters. 

		//for (int i = 0; i<prio ; i++)
		//{printf("%s	%d	%d	%d	%d\n", new[i].comment, new[i].pid,new[i].prio, new[i].nice, prio);}
	
		//step 1. Remove current partitions
		//removeExistParts();	
		//step 2. Generate new
		//createPartitions(index);
		//step 3. Calculate partition size
		partSize = calcRepart (prio);
		//step 4. Assign PIDs and Bins to new partitions.
		doPartition (prio, partSize, new);
		assignoprio(oPrio, newOprio);
		//step 5. Sleep for one second.
		sleep(1);

		//DISPATCH AND SYNC THREADS
	}
	
	return 0;
}

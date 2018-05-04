#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <papi.h>
#include <pthread.h>
#define RAM 255




int main ()
{
	
	FILE *fp = NULL;
	char out[30][20];
	int pids[10];
	char pid1 [8];
	char pid2 [8];
	char path [50] = "sudo sh script.sh ";
	//system("sudo sh initPalloc.sh");
	int i = 0;
	int index = 0;
	fp = popen("top -bn1 | head  -n9 | tail -n2 | awk '{print$1}'", "r");
	if (fp != NULL)
	{
		while ((fgets (out[i], sizeof(out)-1, fp)) != NULL)
		{
			//printf("%s", out[i]);		
			i++;
		}

	}
	for(int j= 0; j<i; j++)
	{
		pids[j] = atoi(out[j]);	
		//printf("%d\n", pids[j]);
	}
		
	snprintf(pid1, 8 ,"%d", pids[0]);
	strcat(path, pid1);
	//printf("%s", path );
	system(path);
	
return 0;
}

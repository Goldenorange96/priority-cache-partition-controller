
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <iomanip>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include "papi.h"
#include <sys/time.h>
#include <fstream>

int main(int argc, char **arv)
{
	int pid = atoi(arv[1]);
	long long values[1];
	int retval = 0;
	int EventSet = PAPI_NULL;
	values[0] = 0;
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
	while(1)
	{
		/*int inp = 0;
		std::cin >> inp;
		if (inp == 1)
			break;*/
		usleep(1000000);	
		break;
	
	}
	
	if ( (retval = PAPI_stop(EventSet, values)) != PAPI_OK)
             	{printf("6\n");}
	
        if ( (retval=PAPI_read(EventSet, values)) != PAPI_OK)
		{printf("7\n");}
           	
   	std::cout<< values[0] << "\n";
       
    PAPI_cleanup_eventset(EventSet);
    //PAPI_destroy_eventset(EventSet);
//g++ -std=c++11 -o mon monitore.cpp /home/jj/Downloads/papi/src/libpapi.a

return 0;
}


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
/* Prints the argument list, one argument to a line, of the process
given by PID. */

#define PAGE_SIZE ( sysconf(_SC_PAGESIZE) / 1024 )

int processMemory(pid_t pid)
{
    FILE* statm;
    int resident,size;
    char filename[24];
    
    snprintf (filename, sizeof (filename), "/proc/%d/statm", (int) pid);
    
    statm = fopen (filename, "r");
    fscanf
    (
        statm,
        "%d %d",
        &size,
        &resident
        
    );
    fclose (statm);
    resident = (resident * PAGE_SIZE) * 100;
    return resident; 
}

int totalMemory()
{
    FILE* meminfo;
    int memTotal;
    char filename[24];
    
    snprintf (filename, sizeof (filename), "/proc/meminfo");
    
    meminfo = fopen (filename, "r");
    fscanf
    (
        meminfo,
        "MemTotal: %d kB\n",
        &memTotal
    );
    fclose (meminfo);
    return memTotal;
}

/*
void print_process_arg_list (pid_t pid)
{
	FILE* statm;
        FILE* meminfo;
	int size,resident,totalMemSize,freeMemSize;
        double memPer;
	char filename[24];
	char filenameS[24];


	snprintf (filename, sizeof (filename), "/proc/%d/statm", (int) pid);

	statm = fopen (filename, "r");
	fscanf
    	(
            statm,
            "%d %d ",
            &size,
            &resident
    	);
	fclose (statm);

        resident = (resident * PAGE_SIZE) * 100; 
        
        /// ---------
        
        snprintf (filenameS, sizeof (filenameS), "/proc/meminfo", (int) pid);
	meminfo = fopen (filenameS, "r");
	fscanf
    	(
            meminfo,
            "MemTotal: %d kB\n"
            "MemFree: %d kB\n",
            &totalMemSize, &freeMemSize
            
    	);
	fclose (meminfo);
        
        
        printf ("Memoria %f % \n", (float)resident / totalMemSize );
	
}
*/

int main (int argc, char* argv[])
{
	pid_t pid = (pid_t) atoi (argv[1]);
        
        while(1)
        {
            int pMemory = processMemory(pid);
        int tMemory = totalMemory();
            sleep(1);
                printf ("Memoria %0.02f%% - Processo id: %d\n", (float) pMemory  / tMemory, pid );
        }
        return 0;
}
















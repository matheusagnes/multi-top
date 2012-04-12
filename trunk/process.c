
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/param.h> //Biblioteca que define a variável HZ do kernel(/usr/include/asm/param.h #define HZ 100)
/* Prints the argument list, one argument to a line, of the process
given by PID. */

#define PAGE_SIZE ( sysconf(_SC_PAGESIZE) / 1024 )

struct memoryInfo
{
  int total;
  int free;
  int buffers;
  int cached;	
}; 

struct process_struct
{
    int pid;                        //pid do processo
    char nome[15];                  //nome do processo
    unsigned long userJiffies;      //jiffies em modo usuario
    unsigned long kernelJiffies;    //jiffies em modo kernel
    unsigned long totalJiffies;     //total atual dos jiffies modo usuario+kernel
    unsigned long ultimaConsulta;   //guarda valor do ultimo total
    unsigned long realJiffies;      //valor resultante de jiffies entre a ultimaConsulta e o total atual
    float memoria;                  //valor de memoria utilizado
    float tempoDecorrido;           //tempo decorrido desde a ultima consulta
    int intervaloCaptura;           //intervalo de atualização das informacoes
    int ativo;
};

struct process_struct processes[50];

void gotoxy(int x, int y) 
{ 
	printf("%c[%d;%df",0x1B,y,x); 
}

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

void processCpu(n)
{
    FILE *  stat_file;
    int     pathSize = (strlen(PROC_DIR)+1)+strlen(this->getPID())+8;
    char    filePath[pathSize]; 
      
    unsigned long uns_long_null, utime, stime;
    int int_null;
    char char_null[100];

	//trabalha string com caminho do arquivo
    snprintf(filePath, pathSize, "proc/%s/stat", this->getPID());  
                     
    //abre o arquivo para leitura    
	if((stat_file = fopen(filePath, "r")) == NULL )
	{
		return 0;		
	}

    fscanf
    (
        FileID, 
        "%d %s %c %d "
        "%d %d %d %d "
        "%lu %lu %lu %lu "
        "%lu %lu %lu",
        &int_null, &char_null, &char_null, &int_null,
        &int_null, &int_null, &int_null, &int_null,
        &uns_long_null, &uns_long_null, &uns_long_null, &uns_long_null,
        &uns_long_null, &utime, &stime
    );
    fclose(FileID);     

    float totalTime = (float) utime + stime;   
    jiffies = totalTime - lastJiffies;
    lastJiffies = totalTime;      
           
    //CALCULA PERCENTUAL DE PROCESSAMENTO   //2 delay do update da tela
    cpuPercent = (float)((jiffies / (HZ * 2)) * 100);    
    
    return 1;  
}

struct memoryInfo totalMemory()
{
    struct memoryInfo info;	
    FILE* meminfo;
    char filename[24];
	int memTotal, memFree,Buffers,Cached;    

    snprintf (filename, sizeof (filename), "/proc/meminfo");
    
    meminfo = fopen (filename, "r");
    fscanf
    (
        meminfo,
        "MemTotal: %d kB\n"
		"MemFree: %d kB\n"
		"Buffers: %d kB\n"
		"Cached: %d kB\n",
        &memTotal,
        &memFree,
        &Buffers,
        &Cached
    );
    fclose (meminfo);
	
	info.total = memTotal;
    info.free  = memFree;
    info.buffers  =  Buffers;
    info.cached  = Cached; 
    return info;
}

int main (int argc, char* argv[])
{
	pid_t pid = (pid_t) atoi (argv[1]);
        
    while(1)
    {
        int pMemory = processMemory(pid);

    	struct memoryInfo memoria = totalMemory();

		int tMemory = memoria.total;
		int tMemoryFree = memoria.free;
		int tCached = memoria.cached;
		int tBuffers = memoria.buffers;

		gotoxy(2,3);
        sleep(1);
        printf ("Memoria %0.02f%% - Processo id: %d \n", (float) pMemory  / tMemory, pid );
	   	
        gotoxy(2,5);
        printf ("Memoria total %0.2fMB  \n",  (float) (tMemory / 1024) );
	 
     	gotoxy(2,4);
        printf ("Memoria livre %0.2fMB  \n", (float) ( (tMemoryFree / 1024) +  (tCached / 1024) +  (tBuffers / 1024)) );

    }
    return 0;
}
















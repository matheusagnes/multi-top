
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
    int pid; //pid do processo
/*
    unsigned long userJiffies; //jiffies em modo usuario
    unsigned long kernelJiffies; //jiffies em modo kernel
    unsigned long totalJiffies; //total atual dos jiffies modo usuario+kernel
    unsigned long ultimaConsulta; //guarda valor do ultimo total
    unsigned long realJiffies; //valor resultante de jiffies entre a ultimaConsulta e o total atual
*/
    
    float jiffies;
    float lastJiffies;
    float cpuPercent;
    
    float memoria; //valor de memoria utilizado
    float tempoDecorrido; //tempo decorrido desde a ultima consulta
    int intervaloCaptura; //intervalo de atualização das informacoes
    int ativo;
    int delay;
    char nome[15]; //nome do processo
    
};

struct process_struct processes[50];

void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}

void processMemory(int n)
{
    FILE* statm;
    int resident, size;
    char filename[24];

    snprintf(filename, sizeof (filename), "/proc/%d/statm", processes[n].pid);

    statm = fopen(filename, "r");
    fscanf
            (
            statm,
            "%d %d",
            &size,
            &resident

            );
    fclose(statm);
    resident = (resident * PAGE_SIZE) * 100;
    processes[n].memoria = (float) resident;
}

void processCpu(int n)
{
    unsigned long uns_long_null, utime, stime;
    int int_null;
    char char_null;

    FILE* stat;
    char filename[24];

    snprintf(filename, sizeof (filename), "/proc/%d/stat",  processes[n].pid);

    stat = fopen(filename, "r");

    fscanf
            (
            stat,
            "%d %s %c %d "
            "%d %d %d %d "
            "%lu %lu %lu %lu "
            "%lu %lu %lu",
            &int_null, &char_null, &char_null, &int_null,
            &int_null, &int_null, &int_null, &int_null,
            &uns_long_null, &uns_long_null, &uns_long_null, &uns_long_null,
            &uns_long_null, &utime, &stime
            );
    fclose(stat);

    float totalTime = (float) utime + stime;
    processes[n].jiffies = totalTime - processes[n].lastJiffies;
    
    processes[n].lastJiffies = totalTime;

    //CALCULA PERCENTUAL DE PROCESSAMENTO   //2 delay do update da tela
    processes[n].cpuPercent = (float) ((processes[n].jiffies / (HZ * processes[n].delay)) * 100);
}

struct memoryInfo totalMemory()
{
    struct memoryInfo info;
    FILE* meminfo;
    char filename[24];
    int memTotal, memFree, Buffers, Cached;

    snprintf(filename, sizeof (filename), "/proc/meminfo");

    meminfo = fopen(filename, "r");
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
    fclose(meminfo);

    info.total = memTotal;
    info.free = memFree;
    info.buffers = Buffers;
    info.cached = Cached;
    return info;
}

int main(int argc, char* argv[])
{
    // ultimo parametro = delay
    // os primeiros paramentros sao os pids
    int delay = atoi(argv[argc-1]);
    int i;

    for (i = 1; i < argc - 1; i++)
    {        
        processes[i].pid = (int) atoi(argv[i]);        
        processes[i].delay = delay;
        processCpu(i);
    }
        
    //processes[1].pid = (int) pid;
    //processes[1].delay = (int) atoi(argv[2]);   
    

    
    while (1)
    {
        sleep( delay );
        for (i = 1; i < argc - 1; i++)
        {
            processCpu(i);
            processMemory(i);

            struct memoryInfo memoria = totalMemory();

            int tMemory = memoria.total;

            gotoxy(2, 3+i*2);
            printf("Memoria %0.02f%% - Processo id: %d \n", (float) processes[i].memoria / tMemory, processes[i].pid);
            gotoxy(2, 4+i*2);
            printf("CPU %0.02f%%  - Processo id: %d \n", processes[i].cpuPercent, processes[i].pid);
            gotoxy(2, 5+i*2);
            //printf("Memoria total %0.2fMB  \n", (float) (tMemory / 1024));
            //gotoxy(2, 6);
            //printf("Memoria livre %0.2fMB  \n", (float) ((tMemoryFree / 1024) + (tCached / 1024) + (tBuffers / 1024)));
        }

    }

    return 0;
}
















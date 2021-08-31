#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mman.h>
#define size 3

int main(int argc, char const *argv[])
{

    char str[256];
    char *ptr;
    pid_t pid;
    int *shared = mmap(NULL, size*sizeof(int),PROT_READ| PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    for (int i = 0; i < 3; i++)
    {
    
        if ((pid = fork()) < 0)
            perror("fork");
        else if (!pid)
        {

       
            shared[i] = getpid();
           exit(0);
        }
        else
        {
            int status;
            if ((pid = waitpid(pid, &status, 0) < 0))
                perror("waitpid");
           
        }
    }
    
     
    for (int j = 0; j < size; j++)
    {
        printf("procces id:%d\n", shared[j]);
    }

     munmap(shared, size*sizeof(int) );
    return 0;  
}

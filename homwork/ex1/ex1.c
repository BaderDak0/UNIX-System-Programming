#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

void cat_ls_operation( char * ptr , int flag){
    FILE *fi, *fp;
   struct dirent *de; // Pointer for directory entry
    char str2[256];


  if (strcmp(ptr, "ls") == 0)
        {

            ptr = strtok(NULL, " \n");
            strcpy(str2, ptr);
            printf("%s", str2);
            DIR *dr = opendir(str2);

            if (dr == NULL) // opendir returns NULL if couldn't open directory
            {
                printf("Could not open current directory");
                return ;
            }
            ptr = strtok(NULL, " ");
            if (ptr != NULL && strcmp(ptr, ">") == 0)
            {
                ptr = strtok(NULL, " ");
                fi = fopen(ptr, "w");
                flag = 1;
            }
            while ((de = readdir(dr)) != NULL)
            {
                if (flag == 1)
                {

                    fprintf(fi, "%s", de->d_name);
                }
                else
                    printf("%s\n", de->d_name);
            }
            if (flag == 1)
            {

                fclose(fi);
            }

            printf("\n");
        }
        else
        {
            char buff[500];
            ptr = strtok(NULL, " \n");
            strcpy(str2, ptr);
            fp = fopen(ptr, "r");
            ptr = strtok(NULL, " ");
            if (ptr != NULL && strcmp(ptr, ">") == 0)
            {
                ptr = strtok(NULL, " ");
                fi = fopen(ptr, "w");
                flag = 1;
            }
            while (fgets(buff, 1000, fp) != NULL)
            {

                if (flag == 1)
                {

                    fprintf(fi, "%s", buff);
                }
                else
		   {
                    printf("%s\n", buff);
			}
            }
	   fclose(fp);
	    if(flag==1)
	   fclose(fi);
        }






}


int main(int argc, char const *argv[])
{
 
    int flag = 0;
    char str[256];
    char *ptr;
    pid_t pid;
   
    while (1)
    {
        printf("> ");
        // opendir() returns a pointer of DIR type.
        fgets(str, 256, stdin);
        ptr = strtok(str, " \n");
        if (strcmp(ptr, "ls") != 0 && strcmp(ptr, "cat") != 0)
        {
            printf("ERROR");
            return 0;
        }
        if ((pid = fork()) < 0)
            perror("fork");
        else if (!pid)
        {
            
            printf("%d\n",getpid());
            cat_ls_operation(ptr,flag);

        }
        else
        {
            int status;
            if ((pid = waitpid(pid, &status, 0) < 0))
                perror("waitpid");

        }
      
        flag = 0;
    }
 

    return 0;
}

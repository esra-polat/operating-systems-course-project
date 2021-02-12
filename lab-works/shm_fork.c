#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>

int
main (int argvc, char **argv)
{       
   int segment_id;
   char* shared_memory;
   const int size = 4096; 
   segment_id = shmget(IPC_PRIVATE, size, S_IRUSR | S_IWUSR);
   shared_memory = (char*) shmat(segment_id, NULL, 0);
   sprintf(shared_memory, "Hi there!");
   printf("*%s\n", shared_memory);
   pid_t pid;
   printf("*BEFORE: %s\n", shared_memory);
   pid = fork();
   if (pid == 0) {
        printf("I am child\n");
        sprintf(shared_memory, "This is child writing: Hi there!");
        printf("*%s\n", shared_memory);
        printf("my pid is: %d\n", getpid());
        printf("my parent pid is: %d\n", getppid());
    }
    else {
        printf("I am a parent\n");
        sprintf(shared_memory, "Parent: This is child writing: Hi there!");
        printf("*AFTER:%s\n", shared_memory);
        printf("my pid is: %d\n", getpid());
        printf("my child pid is: %d\n", pid);    
    }

    /* This code executes in both processes */
    printf("exiting ... %d\n", getpid());
    exit(0);
}



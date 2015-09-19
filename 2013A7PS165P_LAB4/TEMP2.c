


//Program to test whether standard input is capable of seeking or not.
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

int main(){
	int fifo51 = open("fifo5",O_RDWR);
			printf("fifo 5 opened for writing and unblocked\n");

			if(STDOUT_FILENO!=fifo51)
			{
				if(dup2(fifo51,STDOUT_FILENO)==-1) perror("dup2");
			}
			//printf("ls1\n");
			execlp("ls","ls","-l",(char*) NULL);
			perror("execlp");
	return 0;
}
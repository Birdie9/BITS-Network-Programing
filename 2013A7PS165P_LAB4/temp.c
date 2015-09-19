#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>

int main(){
	int fifo1 = open("fifo1",O_RDONLY|O_NONBLOCK);
	if(fifo1==-1) perror("");
	int n;
	char buf[1024];
	printf("fifo1\n");
	while((n=read(fifo1,buf,1024))>0)
	{
		write(STDOUT_FILENO,buf,n);
	}
	int fifo2 = open("fifo2",O_RDONLY|O_NONBLOCK);
	//int n;
	//char buf[1024];
	while((n=read(fifo2,buf,1024))>0)
	{
		write(STDOUT_FILENO,buf,n);
	}
	int fifo3 = open("fifo3",O_RDONLY|O_NONBLOCK);
	//int n;
	//char buf[1024];
	while((n=read(fifo3,buf,1024))>0)
	{
		write(STDOUT_FILENO,buf,n);
	}
	int fifo4 = open("fifo4",O_RDONLY|O_NONBLOCK);
	//int n;
	//char buf[1024];
	while((n=read(fifo4,buf,1024))>0)
	{
		write(STDOUT_FILENO,buf,n);
	}

	int fifo5 = open("fifo5",O_RDONLY|O_NONBLOCK);
	//int n;
	//char buf[1024];
	while((n=read(fifo5,buf,1024))>0)
	{
		write(STDOUT_FILENO,buf,n);
	}
}
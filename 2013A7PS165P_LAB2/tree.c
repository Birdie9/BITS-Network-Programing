#include<sys/types.h>
#include<stdio.h>
#include<sys/wait.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>
void makeChildren(int n,int level,int pos)
{
	extern int errno;
	int i;

	int status;
	for(i=1;i<=n;i++)
	{
		pid_t pid=fork();
		if(pid==0)
		{
			makeChildren(n-1,level+1,i);
			return;
		}
		
	}
	pid_t pi;
	while (pi = waitpid(-1, NULL, 0)) {
   if (errno == ECHILD) {
      break;
   }
}

	
	printf("All children exited\n");
	printf("%d\t%d\t%d\t%d\n",level,getpid(),getppid(),pos);
	
}
int main(int argc,char** argv){
	
	int n;
	int level=1;
	int pos=1;
	pid_t pid;
	
	if(argc<2)
	{
		printf("%s please enter N\n",argv[0]);
		return 0;
	}
	else n=atoi(argv[1]);
	printf("level\tpid\tppid\tposition\n");
	makeChildren(n,1,1);
	
	return 0;

}

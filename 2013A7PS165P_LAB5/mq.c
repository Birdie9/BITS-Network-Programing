#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<time.h>
#include<sys/stat.h>
#include<stdio.h>
typedef struct msg{
	long type;
	int i;
}message;
pid_t *children;
pid_t par;

int n;
int count=0,id;
int getMsg(){
	
    int r = rand();
    return r;
}

void ha(int sig)
{
	int m=getMsg(time);
	int i;
	printf("sender pid: %ld msg: %d\n",getpid(),m);
		message msg;
		msg.type=par;
		msg.i=m;
		if(msgsnd(id,&msg,sizeof(long),0)==-1)perror("msgsnd");
	alarm(5);
}

void ha2(int sig)
{
int i;
while(1)
	{
		message msg;
		int numBytes=msgrcv(id,&msg,sizeof(long),par,IPC_NOWAIT);
		if(numBytes!=-1) 
			{
				count++;
			}
		else break;
	}
for(i=1;i<=n;i++) kill(children[i],SIGKILL);
printf("total count: %d\n",count);
fflush(stdin);
if(msgctl(id,IPC_RMID,NULL)==-1)perror("msgctl remove");
exit(0);
}

int main(int argc,char**argv){
	if(argc<2)
	{
		printf("%s please enter N\n",argv[0]);
		return 0;
	}
	else n=atoi(argv[1]);
	signal(SIGINT,ha2);
	int i;
	srand(time(NULL));
	id = msgget(IPC_PRIVATE,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
	if(id==-1) {perror("msgget"); exit(0);}

	children = (pid_t*) malloc(sizeof(pid_t)*(n+1));
	
	par=getpid();
	children[0]=par;
	int ret;
	for(i=0;i<n;i++)
	{
		ret=fork();
		if(ret==0) 
		{
			alarm(5);
			signal(SIGINT,SIG_IGN);
			signal(SIGALRM,ha);
			break;
		}
		else children[i+1]=ret;
	}
while(1)
	{
		message msg;
		int numBytes=msgrcv(id,&msg,sizeof(long),getpid(),IPC_NOWAIT);
		if(numBytes!=-1) 
			{
				if(msg.type!=par)
					printf("reciever pid %ld: msg: %d\n",msg.type,msg.i);
				else
				 {
				 	for(i=1;i<=n;i++)
					{
						msg.type=(long)children[i];
						if(msgsnd(id,&msg,sizeof(long),0)==-1)perror("msgsnd");
					}
					count++;
				 }
			}
	}
return 0;
}
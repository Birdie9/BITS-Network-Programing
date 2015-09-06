#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <errno.h> 
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#define MAX_LEN 100
#define MAX_CMD_SIZE 30

int execvErr=0;

struct job{
	pid_t pid;
	struct job* next;
};
typedef struct job job;
job* head =NULL,*cur=NULL;


void addJob(pid_t pid)
{
	cur->next=(job*)malloc(sizeof(job));
				cur->next->pid= pid;
				cur->next->next=NULL;
				cur=cur->next;
}
void removeJob(pid_t pid)
{
	job* cur1=head->next;
	job*prev=head;
	while(cur1!=NULL)
	{
		if(cur1->pid == pid )
		{
			prev->next=cur1->next;
			free(cur1);
			cur1=prev->next;
		}
		else
		{
			prev=cur1;
			cur1=cur1->next;
		}
	}
	cur=prev;
}
void jobs(){
	printf("LISTING BACKGROUND JOBS\n");
	job* cur1  = head->next;
	int i=1;
	while(cur1!=NULL)
		{
			printf("job[%d] pid: %d\n",i++,cur1->pid);
			cur1=cur1->next;
		}
}


void handle_sigchld(int sig)
{
	int status;
	pid_t pid;
	pid= waitpid(-1, &status, WNOHANG);
	{
			if(WIFSTOPPED(status)) 
		{
			addJob(pid);
		}
		else if(WIFEXITED(status)) removeJob(pid);		
	}	
}



int main()
{
	printf("README: To start a background-job in background give the command: start <pid>\nTo stop a job running in background give the command: stop <pid>\nTo bring a stopped(running) background job in foreground give the command: fg <pid>\n");
	
	signal(SIGCHLD,handle_sigchld);
	signal(SIGTTOU, SIG_IGN);

	/*initialising linked list for storing background jobs*/
    head=(job*)malloc(sizeof(job));
	cur=head;
	head->pid=0;


	sigset_t blockset,origset;
	sigemptyset(&blockset);
	sigaddset(&blockset,SIGINT);
	sigaddset(&blockset,SIGQUIT);
	sigaddset(&blockset,SIGTSTP);
	if(sigprocmask(SIG_BLOCK,&blockset,&origset)==-1) perror("sigprocmask");
	
	char cmd[MAX_LEN];
	while(1)
	{
		
		execvErr=0;
		printf("command:");
		fflush(stdin);
		if(fgets(cmd,MAX_LEN,stdin)==NULL)
			break;

		if(strcmp(cmd,"\n")==0) continue;

		if(strcmp(cmd,"jobs\n")==0)
		{
			jobs();
			continue;
		} 


		/*bringing a bg job to fg*/
		if(cmd[0]=='f' && cmd[1]=='g' && cmd[2]==' ')
		{
			pid_t pid=atoi(cmd+3);
			fflush(stdin);
			tcsetpgrp(0,pid);
			removeJob(pid);
			kill(pid,SIGCONT);
			int status;
			waitpid(pid,&status,WUNTRACED);
			if(WIFSTOPPED(status))
			{
				addJob(pid);
			}
			tcsetpgrp(0,getpid());
			fflush(stdin);
			continue;
		}

		/*starting a bg job to run in background*/
		if(cmd[0]=='s' && cmd[1] == 't' && cmd[2] == 'a' && cmd[3]=='r' && cmd[4] =='t')
		{
			pid_t pid = atoi(cmd+6);
			fflush(stdin);
			kill(pid,SIGCONT);
			continue;
		}

		/*stopping a bg job*/
		if(cmd[0]=='s' && cmd[1] == 't' && cmd[2] == 'o' && cmd[3]=='p' )
		{
			pid_t pid = atoi(cmd+5);
			kill(pid,SIGSTOP);
			continue;
		}

		/*using shell to handle pipes*/
		if(strrchr(cmd,'|'))
		{
			pid_t pid=fork();
			if(pid==0)
			{
				sigset_t emptyset;
				sigemptyset(&emptyset);
				if(sigprocmask(SIG_SETMASK,&emptyset,&origset)==-1) perror("sigprocmask");
				signal(SIGTTOU, SIG_IGN);
				setpgid(getpid(),getpid());
				tcsetpgrp(0,getpid());
				fflush(stdout);
				execl("/bin/sh","sh","-c",cmd,(char*) NULL);
				perror("execl");
			}
			else
			{
				int status;
				waitpid(pid,&status,WUNTRACED);
				if(WIFSTOPPED(status))
				{
					addJob(pid);
				}
				tcsetpgrp(0,getpid());	
			}
			continue;
		}

		/*checking if a job is background job and tokeinzing string into main command and its flags*/
		char * amp = strrchr(cmd,'&');
		if(amp!=NULL)
			{
				*amp='\n';
				*(amp+1)='\0';
			}
		char* args[100];
		int argcnt=0;
		int i;
		for(i=0;i<100;i++)
		{
			while(cmd[i]==' ')i++;
			if(cmd[i]=='\0') break;
			else
			{
				int temp=i;
				while(cmd[i]!=' ' && cmd[i]!='\n' && cmd[i]!='\0' )i++;
				cmd[i]='\0';
				args[argcnt] = (char*) malloc(sizeof(char)*MAX_CMD_SIZE);
				strcpy(args[argcnt],cmd+temp);
				argcnt++;
				
			}
		}
		args[argcnt]=(char*)NULL;
		
		
		/*starting a process in background*/
		if(amp != NULL)
		{
			pid_t pid;
			
			pid=fork();
			if(pid==0)
			{
				fflush(stdin);
				fflush(stdout);
				sigset_t emptyset;
				sigemptyset(&emptyset);
				if(sigprocmask(SIG_SETMASK,&emptyset,&origset)==-1) ;
				setpgid(getpid(),getpid());
				execvp(args[0],args);
				perror("execv\n");
				execvErr=1;
			}
			else
			{
				if(!execvErr)
					addJob(pid);
				waitpid(pid,0,WNOHANG);	
			}
		}
		/*starting a process in foreground*/
		else
		{

			pid_t pid;
			pid=fork();
			if(pid==0)
			{
				sigset_t emptyset;
				sigemptyset(&emptyset);
				if(sigprocmask(SIG_SETMASK,&emptyset,&origset)==-1) perror("sigprocmask");
				signal(SIGTTOU, SIG_IGN);
				setpgid(getpid(),getpid());
				tcsetpgrp(0,getpid());
				fflush(stdout);	
				execvp(args[0],args);
				perror("execv\n");
			}
			else
			{
				int status;
				waitpid(pid,&status,WUNTRACED);
				if(WIFSTOPPED(status))
				{
					addJob(pid);
				}
				tcsetpgrp(0,getpid());
			}
		}
	}
	return 0;
}
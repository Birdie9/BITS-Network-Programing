#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>

int main(){
	
	int fifo1,fifo2,fifo3,fifo4,fifo5;
	pid_t ls1,grep1,grep2,wc1,wc2;
	if( mkfifo("fifo1",S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP)==-1) perror("fifo1");
	if( mkfifo("fifo2",S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP)==-1) perror("fifo2");
	if( mkfifo("fifo3",S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP)==-1) perror("fifo3");
	if( mkfifo("fifo4",S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP)==-1) perror("fifo4");
	if( mkfifo("fifo5",S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP)==-1) perror("fifo5");

	//fifo1 = open("fifo1",O_RDONLY|O_NONBLOCK);
	//fifo2 = open("fifo2",O_RDONLY|O_NONBLOCK);
	//fifo3 = open("fifo3",O_RDONLY|O_NONBLOCK);
	//fifo4 = open("fifo4",O_RDONLY|O_NONBLOCK);
	
	ls1 = fork();
	switch(ls1)
	{
		case -1: 
			perror("fork1");
			break;
		case 0:
			;
			printf("fifo 1 opened for writing and  blocked\n");

			int fifo51 = open("fifo1",O_WRONLY);
			printf("fifo 1 opened for writing and unblocked\n");

			if(STDOUT_FILENO!=fifo51)
			{
				if(dup2(fifo51,STDOUT_FILENO)==-1) perror("dup2 in ls");
			}
			//printf("ls1\n");
			execlp("ls","ls","-l",(char*) NULL);
			perror("execlp");
			break;
		default:
			//wait(NULL);
			//printf("fifo 5 opened for reading and  blocked\n");
			//fifo5 = open("fifo5",O_RDONLY);
			//printf("fifo 5 opened for reading and  unblocked\n");
			grep1 = fork();
			switch(grep1)
			{
				case -1: 
					perror("fork3");
					break;
				case 0:
					;
					printf("fifo 1 opened for reading and  blocked\n");
					int fifo10 = open("fifo1",O_RDONLY);
					printf("fifo 1 opened for reading and  unblocked\n");
					printf("fifo 3 opened for writing and  blocked\n");

					int fifo31 = open("fifo3",O_WRONLY);
					printf("fifo 3 opened for writing and  unblocked\n");

					if(STDIN_FILENO!=fifo10)
					{
						if(dup2(fifo10,STDIN_FILENO)==-1) perror("dup2 in grep input");
					}
					if(STDOUT_FILENO!=fifo31)
					{
						if(dup2(fifo31,STDOUT_FILENO)==-1) perror("dup2 in grep output");
					}
					
					execlp("grep","grep","^d",(char*) NULL);
					perror("execlp grep");
					break;
				default:
					;
					printf("fifo 1 opened for writing and  blocked\n");
					//int fifo11 = open("fifo1",O_WRONLY);
					printf("fifo 1 opened for writing and  unblocked\n");
			
					/*grep2 = fork();
					switch(grep2)
					{
						case -1: 
							perror("fork4");
							break;
						case 0:
							;
							printf("fifo 2 opened for reading and  blocked\n");
							int fifo20 = open("fifo2",O_RDONLY);
							printf("fifo 2 opened for reading and  unblocked\n");
							printf("fifo 4 opened for writing and  blocked\n");
							int fifo41 = open("fifo4",O_WRONLY);
							printf("fifo 4 opened for writing and  unblocked\n");

							if(STDIN_FILENO!=fifo20)
							{
								if(dup2(fifo20,STDIN_FILENO)==-1) perror("dup2");
							}
							if(STDOUT_FILENO!=fifo41)
							{
								if(dup2(fifo41,STDOUT_FILENO)==-1) perror("dup2");
							}
							execlp("grep","grep","^-",(char*) NULL);
							perror("execlp grep");
							break;
						default:
							;
							printf("fifo 2 opened for writing and  blocked\n");
							int fifo21 = open("fifo2",O_WRONLY);
							printf("fifo 2 opened for writing and  unblocked\n");*/
							wc1 = fork();
							switch(wc1)
							{
								case -1: 
									perror("fork5");
									break;
								case 0:
									;
									printf("fifo 3 opened for reading and  blocked\n");
									int fifo30 = open("fifo3",O_RDONLY);
									printf("fifo 3 opened for reading and  unblocked\n");
									if(STDIN_FILENO!=fifo30)
									{
										if(dup2(fifo30,STDIN_FILENO)==-1) perror("dup2 in wc");
									}
									execlp("wc","wc","-l",(char*) NULL);
									perror("execlp");
									break;
								default:
									//printf("NUMBER OF FILES PRESENT:\n");
									;
									/*wc2 = fork();
									switch(wc2)
									{
										case -1: 
											perror("fork6");
											break;
										case 0:
											;
											printf("fifo 4 opened for reading and  blocked\n");
											int fifo40 = open("fifo4",O_RDONLY);
											printf("fifo 4 opened for reading and  unblocked\n");
											if(STDIN_FILENO!=fifo40)
											{
												if(dup2(fifo40,STDIN_FILENO)==-1) perror("dup2");
											}
											execlp("wc","wc","-l",(char*) NULL);
											perror("execlp");
											break;
										default:
											break;

									}*/
									break;
							}
							int n;
							char buf[1024];
							/*while((n=read(fifo5,buf,1024))>0)
							{
								write(fifo11,buf,n);
								//write(fifo21,buf,n);
							}
							if(n<0) perror("read fifo5");
							if(close(fifo5)==-1) perror("close fifo5");
							if(close(fifo11)==-1) perror("close fifo11");*/
							//if(close(fifo21)==-1)perror("close fifo21");
							//printf("finished writing fifo1 and fifo2\n");
							break;
					//}
					int status;
					waitpid(ls1,&status,0);
					 if(WIFEXITED(status))
					 	printf("ls1 exited normally\n");
					

					waitpid(grep1,&status,0);
					printf("grep1 status:%d\n",status);
					
					//waitpid(grep2,&status,NULL);
					//printf("grep2 status:%d\n",status);
					
					waitpid(wc1,&status,0);
					printf("wc1 status:%d\n",status);
					
					//waitpid(wc2,&status,NULL);
					//printf("wc2 status:%d\n",status);
					

					break;
			}		
	}
	
	//printf("NUMBER OF DIRECTORIES PRESENT:\n");
	
//close(fifo1);
//close(fifo2);
//close(fifo3);
//close(fifo4);
remove("fifo1");
remove("fifo2");
remove("fifo3");
remove("fifo4");
remove("fifo5");
return 0;

}
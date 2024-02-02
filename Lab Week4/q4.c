#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
int main()
{
	char input[256];
	int pipe1[2],pipe2[2];
	pid_t pid0,pid1,pid2;
	
	while(1)
	{
		printf("enter your command of form 'command_1|command_2|command_3' or 'quit' \n");
		fgets(input,sizeof(input),stdin);
		input[strlen(input)-1]='\0';
		
		if(strcmp(input,"quit")==0)
		{
			printf("program terminated");
			break;
		}
		char *commands[3];
		for(int i=0;i<3;i++)
		{
			commands[i]=NULL;
		}	
		commands[0]=strtok(input,"|");
		commands[1]=strtok(NULL,"|");
		commands[2]=strtok(NULL,"|");
		if(commands[0] && commands[1] && commands[2])
		{	
		pipe(pipe1);
		pid0=fork();
		if(pid0<0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if(pid0==0)
		{
			close(pipe1[0]);
			dup2(pipe1[1],STDOUT_FILENO);
			close(pipe1[1]);
			char *args[]={"/bin/sh","-c",commands[0],NULL};
			execvp(args[0],args);
			perror("execl");
			exit(EXIT_FAILURE);
			
		}
		pipe(pipe2);
		pid1=fork();
		if(pid1<0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if(pid1==0)
		{
			close(pipe1[1]);
			close(pipe2[0]);
			dup2(pipe1[0],STDIN_FILENO);
			dup2(pipe2[1],STDOUT_FILENO);
			close(pipe1[0]);
			close(pipe2[1]);
			char *args[]={"/bin/sh","-c",commands[1],NULL};
			execvp(args[0],args);
						perror("execv");
			exit(EXIT_FAILURE);
		}
		pid2=fork();
		if(pid2<0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if(pid2==0)
		{
			close(pipe1[0]);
			close(pipe1[1]);
			close(pipe2[1]);
			dup2(pipe2[0],STDIN_FILENO);
			close(pipe2[0]);
			char *args[]={"/bin/sh","-c",commands[1],NULL};
			execvp(args[0],args);
						perror("execv");
			exit(EXIT_FAILURE);
		}
		close(pipe1[0]);
		close(pipe1[1]);
		close(pipe2[0]);
		close(pipe2[1]);
		waitpid(pid0,NULL,0);
		waitpid(pid1,NULL,0);
		waitpid(pid2,NULL,0);
		}
		else
		{
			printf("invalid format");
		}		
			
	}
	return 0;
}							
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

void parse_input(char *line, char **args){
	//splits raw input line into tokens for execution
	int i = 0;
	args[i] = strtok(line, " \t\n");
	while(args[i] != NULL && i < MAX_ARGS - 1){
		i++;
		args[i] = strtok(NULL, " \t\n");
	}
	args[i] = NULL;
}

int main(){
	char line[MAX_LINE];
	char *args[MAX_ARGS];
	pid_t pid;
	int status;

	while(1){
		printf("Minishell> ");
		fflush(stdout);

		if(fgets(line, sizeof(line), stdin) == NULL){
			printf("\n");
			break;
		}
		parse_input(line, args);


		if(args[0] == NULL){
			continue;
		}


		if(strcmp(args[0], "exit") == 0){
			break;
		}

		if(strcmp(args[0], "cd") == 0){
			if(args[1] == NULL){
				fprintf(stderr, "minishell: expected argument to \"cd\"\n");
			}else{
				if(chdir(args[1]) != 0){
					perror("minishell");
				}
			}
			continue;
		}


		pid = fork();
		if(pid < 0){
			perror("minishell: fork failed");
			exit(1);
		}else if(pid == 0){
			if(execvp(args[0], args) == -1){
				perror("minishell");
			}
			exit(EXIT_FAILURE);
		}else{
			do{
				waitpid(pid, &status, WUNTRACED);
			}while(!WIFEXITED(status) && !WIFSIGNALED(status));
		}
	}
	return 0;
}

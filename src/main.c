#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LENGTH 1024

char *read_split_cmd(char *buf, char *split_input[], size_t sz, size_t max) {
    if (fgets(buf, sz, stdin) == NULL) {
        return NULL;
    }

    buf[strcspn(buf, "\n")] = '\0';
    char* token = strtok(buf, " ");
    int index = 0;

    while(token != NULL){
        split_input[index] = malloc(strlen(token) + 1);
        strcpy(split_input[index], token);
        index++;
        token = strtok(NULL, " ");
    }
    return " ";
}

int main(int argn, const char** argv){
    char userInput[MAX_LENGTH];
    size_t szUI = sizeof(userInput); 
    char *inputSplit[MAX_LENGTH];
    size_t szIS = sizeof(inputSplit);

    memset(inputSplit, 0, szIS);
    const char* prefix = "> ";
    int num;

    if (argn == 2) prefix = argv[1];

    while(1){
    	printf("%s ",prefix);
        read_split_cmd(userInput, inputSplit, szUI, szIS);
    	if (strcmp(inputSplit[0], "exit") == 0) exit(0);
    	
        pid_t pid = fork();
        if (pid == 0) {
            if (strpbrk(inputSplit[0], "/") != 0) {
                if (execvp(inputSplit[0], inputSplit) == -1) {
                    perror(inputSplit[0]);
                    return 0;
                }
            }
            char tmp[MAX_LENGTH];
            strcpy(tmp, "/bin/");
            strcat(tmp, inputSplit[0]);
            if (execvp(tmp, inputSplit) == -1) {
                perror(inputSplit[0]);
                return 0;
            }
            memset(tmp, 0, sizeof(tmp));
            read_split_cmd(userInput, inputSplit, szUI, szIS);
        } else {
        	wait(&num);
            printf("Child %d exited with %i", pid, WEXITSTATUS(num));
            printf("\n");
        }
        memset(userInput, 0, szUI);
        memset(inputSplit, 0, szIS);
    }
    return 0; 
}
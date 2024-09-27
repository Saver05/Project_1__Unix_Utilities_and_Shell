#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define TOKEN_DELIMITERS " \t\r\n"

char *read_input() {
    char *line = NULL;
    size_t buf = 0;
    ssize_t len;

    len = getline(&line, &buf, stdin);

    if(len == -1) {
        free(line);
        return NULL;
    }

    return line;
}

char **parse_input(char *input) {
    int bufsize = 64;
    int position = 0;
    char *token;
    char **tokens = malloc(bufsize * sizeof(char *));

    token = strtok(input, TOKEN_DELIMITERS);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += TOKEN_DELIMITERS;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "Reallocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, TOKEN_DELIMITERS);
    }
    tokens[position] = NULL;
    return tokens;
}

int main(void) {

    while(1) {
        printf("$ ");
        fflush(stdout);

        char *input = read_input();
        if(input == NULL) {
            break;
        }

        char **tokens = parse_input(input);
        if(tokens[0] == NULL) {
            free(input);
            free(tokens);
            break;
        }

        int pid = fork();
        if(pid == 0) {
            if (execvp(tokens[0], tokens) == -1) {
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
        }
        if (pid>0){
            int status;
            wait(&status);
        }else {
            perror("fork failed");
        }
        free(input);
        free(tokens);
    }
}



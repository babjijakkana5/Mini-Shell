#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
 

#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND  	3

#define STOPPED		4
#define RUNNING		5
#define DONE		6

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


typedef struct Node{
	char process[50];
	int child_pid;
	int state;
	struct Node *link;
}Slist;


extern char *external_cmd[156];
extern char *builtins[];

extern int status;
extern pid_t pid;
extern char prompt[30];
extern char input_string[40];
extern Slist *head;
extern int fg_flag;


void scan_input(char *prompt, char *input_string);
char *get_command(char *input_string);

void copy_change(char *prompt, char *input_string);

int check_command_type(char *command);
void echo(char *input_string, int status);
void execute_internal_commands(char *input_string);
void signal_handler(int sig_num);
void extract_external_commands(char **external_commands);
void n_pipe(char **exter, int count, int size);
void execute_external_commands(char *input_string);
void signal_handler(int signum);
void insert_first(Slist **head, char *input_string);
void print_list(Slist **head);
void delete_first(Slist **head,pid_t child_pid);

#endif


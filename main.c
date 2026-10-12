#include "minishell.h"

char *external_cmd[156];

char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval","set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source","exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};

int status;

pid_t pid;

char prompt[30] = "minishell$:";

char input_string[40];

Slist *head;

int fg_flag = 0;

int main()
{

        system("clear");

        extract_external_commands(external_cmd);

        signal(SIGINT,signal_handler);

        signal(SIGTSTP,signal_handler);

        signal(SIGCHLD,signal_handler);

        scan_input(prompt,input_string);

}

void scan_input(char *prompt, char *input_string)
{
        while(1)
        {
                printf(ANSI_COLOR_CYAN "%s" ANSI_COLOR_RESET ,prompt);
                fflush(stdout);

                memset(input_string,'\0',40);

                scanf(" %[^\n]",input_string);

                char *res = strstr(input_string,"PS1");
                if(res != NULL)
                {
                        int i=0;
                        while(input_string[i])
                        {
                                if(input_string[i] == ' ')
                                {
                                        printf("Invalid Input\n");
                                        break;
                                }
                                i++;
                        }

                        if(input_string[i] == '\0')
                        {
                                char *next = res + strlen("PS1=");
                                strcpy(prompt,next);
                        }
                }

                char *command = get_command(input_string);

                if(command == NULL)
                {
                        continue;
                }

                int type = check_command_type(command);

                free(command);

                if(type == EXTERNAL)
                {
                        pid = fork();

                        if(pid > 0)
                        {
                                waitpid(pid,&status,WUNTRACED);

                                if(WIFSIGNALED(status))
                                {
                                        printf("\n");
                                }

                                if(WIFEXITED(status) || WIFSIGNALED(status))
                                {
                                        pid = 0;
                                }
                        }
                        else if(pid == 0)
                        {
                                signal(SIGINT,SIG_DFL);
                                signal(SIGTSTP,SIG_DFL);
                                execute_external_commands(input_string);
                                exit(0);
                        }
                }
                else if(type == BUILTIN)
                {
                        execute_internal_commands(input_string);
                        pid = 0;
                }
                else if(strcmp(input_string,"jobs") == 0)
                {
                        print_list(&head);
                        pid = 0;
                }
                else if(strcmp(input_string,"fg") == 0)
                {
                        if(head != NULL)
                        {
                                pid = (*head).child_pid;
                                fg_flag = 1;

                                kill(pid,SIGCONT);

                                waitpid(pid,&status,WUNTRACED);

                                if(WIFSTOPPED(status))
                                {
                                        head->state = STOPPED;
                                        printf(ANSI_COLOR_RED "\n[1]+  Stopped                         %s\n" ANSI_COLOR_RESET,(*head).process);
                                }
                                else if(WIFEXITED(status) || WIFSIGNALED(status))
                                {
                                        if(WIFSIGNALED(status))
                                        {
                                                printf("\n");
                                        }
                                        delete_first(&head,pid);
                                }

                                pid = 0;
                                fg_flag = 0;
                        }
                }
                else if(strcmp(input_string,"bg") == 0)
                {
                        if(head != NULL && head->state == STOPPED)
                        {
                                if(kill((*head).child_pid,SIGCONT) == 0)
                                {
                                        (*head).state = RUNNING;

                                printf(ANSI_COLOR_YELLOW "[1]+  Running                         %s &\n" ANSI_COLOR_RESET,(*head).process);
                                }
                        }
                }
                else
                {
                        printf("Command not found\n");
                }
        }
}

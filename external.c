#include "minishell.h"


void extract_external_commands(char **external_commands)
{
        int fd = open("mini.txt", O_RDONLY);

        char buff;
        int i = 0, j = 0;
        char cmd[100];

        while(read(fd,&buff,1))
        {
                if(buff != '\n')
                {
                        cmd[i] = buff;
                        i++;
                }
                else
                {
                        cmd[i] = '\0';
                        external_commands[j] = malloc(strlen(cmd)+1);
                        strcpy(external_commands[j],cmd);
                        i = 0;
                        j++;
                }
        }

        if(i > 0)
        {
                cmd[i] = '\0';

                external_commands[j] = malloc(strlen(cmd) + 1);
                strcpy(external_commands[j], cmd);
        }

        close(fd);
}

void execute_external_commands(char *input_string)
{
        char *cmd_with_options[strlen(input_string)];

        int i = 0, j = 0, k = 0, pipe_count = 0;
        char cmd[100];
        while(input_string[i])
        {
                if(input_string[i] == '|')
                {
                        pipe_count++;
                        if(i == 0 || input_string[i + 1] == '\0' || input_string[i + 1] == '|')
                        {
                                printf("Inavlid arguments\n");
                                return;
                        }
                }
                if(input_string[i] != ' ')
                {
                        cmd[j] = input_string[i];
                        j++;
                }
                else
                {
                        cmd[j] = '\0';
                        cmd_with_options[k] = malloc(strlen(cmd) + 1);
                        strcpy(cmd_with_options[k],cmd);
                        k++;
                        j = 0;
                }
                i++;
        }

        if(j > 0)
        {
                cmd[j] = '\0';
                cmd_with_options[k] = malloc(strlen(cmd) + 1);
                strcpy(cmd_with_options[k],cmd);
                k++;
        }

        cmd_with_options[k] = NULL;

        if(pipe_count != 0)
        {
                n_pipe(cmd_with_options,pipe_count,strlen(input_string));
        }
        else
        {
                execvp(cmd_with_options[0],cmd_with_options);
        }
}


void n_pipe(char **exter, int count, int size)
{
        int arr[size];
        arr[0] = 0;
        int j = 0, k = 1;
        while(exter[j])
        {
                if(strcmp(exter[j],"|") == 0)
                {
                        exter[j] = NULL;
                        arr[k++] = j + 1;
                }
                j++;
        }

        for(int i = 0;i < count + 1;i++)
        {
                int fd[2];
                if(i < count)
                {
                        pipe(fd);
                }

                pid_t pid = fork();

                if(pid > 0)
                {
                        if(i < count)
                        {
                                dup2(fd[0],0);
                                close(fd[0]);
                                close(fd[1]);
                        }
                        wait(&status);
                }
                else if(pid == 0)
                {
                        if(i < count)
                        {
                                close(fd[0]);
                                dup2(fd[1],1);
                        }
                        execvp(exter[arr[i]],exter+(arr[i]));
                }
        }
}

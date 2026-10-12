#include "minishell.h"


char *get_command(char *input_string)
{
        char *command = malloc(strlen(input_string) + 1);

        if(command == NULL)
        {
                return NULL;
        }

        int i = 0;
        while(input_string[i] != ' ' && input_string[i] != '\0')
        {
                command[i] = input_string[i];
                i++;
        }

        command[i] = '\0';

        return command;
}


int check_command_type(char *command)
{
        int i = 0;

        while(external_cmd[i] != NULL)
        {
                if(strcmp(external_cmd[i],command) == 0)
                {
                        return EXTERNAL;
                }
                i++;
        }

        i = 0;

        while(builtins[i] != NULL)
        {
                if(strcmp(builtins[i],command) == 0)
                {
                        return BUILTIN;
                }
                i++;
        }

        return NO_COMMAND;

}


void execute_internal_commands(char *input_string)
{
        if(strcmp(input_string,"exit") == 0)
        {
                exit(0);
        }
        else if(strcmp(input_string,"pwd") == 0)
        {
                char path[100];
                getcwd(path,100);

                printf("%s\n",path);
        }
        else if(strcmp(input_string,"cd") == 0)
        {
                chdir(input_string + 3);
        }
        else if(strcmp(input_string,"echo $$") == 0)
        {
                printf("%d\n",getpid());
        }
        else if(strcmp(input_string,"echo $?") == 0)
        {
                if(WIFEXITED(status))
                {
                        printf("%d\n",WEXITSTATUS(status));
                }
        }
        else if(strcmp(input_string,"echo $SHELL") == 0)
        {
                char *path = getenv("SHELL");
                printf("%s\n",path);
        }
}

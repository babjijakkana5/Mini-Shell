#include "minishell.h"

void signal_handler(int signum)
{
        if(signum == SIGINT)
        {
                if(pid == 0)
                {
                        printf(ANSI_COLOR_CYAN "\n%s" ANSI_COLOR_RESET,prompt);
                        fflush(stdout);
                }
        }
        else if(signum == SIGTSTP)
        {
                if(pid == 0) //if child not present
                {
                        printf("\n%s",prompt);
                        fflush(stdout);
                }
                else if(fg_flag == 1)
                {

                }
                else
                {
                        insert_first(&head,input_string);

                        printf(ANSI_COLOR_RED "\n[1]+  Stopped                          %s\n" ANSI_COLOR_RESET,input_string);

                        pid = 0;
                }
        }
        else if(signum == SIGCHLD)
        {
                pid_t child_pid;

                while((child_pid = waitpid(-1,&status,WNOHANG)) > 0)
                {
                        Slist *temp = head;

                        while(temp != NULL)
                        {
                                if(temp->child_pid == child_pid)
                                {
                                        if(WIFEXITED(status) || WIFSIGNALED(status))
                                        {
                                                temp->state = DONE;
                                        }
                                        break;
                                }

                                temp = temp->link;
                        }
                }
        }
}

void insert_first(Slist **head, char *input_string)
{
        Slist *newNode = malloc(sizeof(Slist));

        if(newNode == NULL)
        {
                return;
        }

        strcpy(newNode->process,input_string);

        newNode->child_pid = pid;

        newNode->state = STOPPED;

        newNode->link = *head;

        *head = newNode;
}

void delete_first(Slist **head, pid_t child_pid)
{
        Slist *temp = *head, *prev = NULL;

        while(temp != NULL)
        {
                if(temp->child_pid == child_pid)
                {
                        if(prev == NULL)
                        {
                                *head = temp->link;
                        }
                        else
                        {
                                prev->link = temp->link;
                        }

                        free(temp);
                        return;
                }

                prev = temp;
                temp = temp->link;
        }
}

void print_list(Slist **head)
{
        Slist *temp = *head, *next;

        while(temp != NULL)
        {
                if(temp->state == STOPPED)
                {
                        printf(ANSI_COLOR_RED "[1]+  Stopped                         %s\n" ANSI_COLOR_RESET,temp->process);
                }
                else if(temp->state == RUNNING)
                {
                        printf(ANSI_COLOR_YELLOW "[1]+  Running                         %s\n" ANSI_COLOR_RESET,temp->process);
                }
                else if(temp->state == DONE)
                {
                        printf(ANSI_COLOR_GREEN "[1]+  Done                            %s\n" ANSI_COLOR_RESET,temp->process);

                        next = temp->link;
                        delete_first(head,temp->child_pid);
                        temp = next;
                        continue;
                }

                temp = temp->link;
        }

}

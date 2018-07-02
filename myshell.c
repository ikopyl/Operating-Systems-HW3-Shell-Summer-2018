/****************************************************************
 * Name        :                                                *
 * Class       : CSC 415                                        *
 * Date        :                                                *
 * Description :  Writting a simple bash shell program          *
 * 	          that will execute simple commands. The main   *
 *                goal of the assignment is working with        *
 *                fork, pipes and exec system calls.            *
 ****************************************************************/
//May add more includes
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

//Max amount allowed to read from input
#define BUFFERSIZE 256
//Shell prompt
#define PROMPT "myShell >> "
//sizeof shell prompt
#define PROMPTSIZE sizeof(PROMPT)
#define TERMINATION_CMD "exit"

#define ARGVMAX 32

void repl();
void display_prompt();
void print_newline_char();
void strip(char *, char);

ssize_t get_user_input(char *);

void reallocate(char **, size_t);
void err_exit(const char *);
void verify_memory_allocation(const char *);
void check_for_errors(ssize_t, const char *);

int main(int* argc, char** argv)
{
    repl();
    return 0;
}

void repl()
{
    char * token;
    char * delimeter = " \t";

    char *buf = (char *) calloc(BUFFERSIZE, sizeof(char));
    verify_memory_allocation(buf);

    char ** myargv = calloc(ARGVMAX, sizeof(char *));       // allocate initially: 32 * size_t (pointer size)
    size_t max_items_allowed = ARGVMAX;
    size_t myargc = 0;

    ssize_t bytes_read = 0;

    do {
        display_prompt();
        while ((bytes_read = get_user_input(buf)))
        {
            strip(buf, '\n');
            if (strcmp(buf, TERMINATION_CMD) == 0)
                return;

            token = strtok(buf, delimeter);
            while (token)
            {
                if (myargc - 1 >= max_items_allowed)
                    reallocate((char **) &myargv, (max_items_allowed *= 2));

                myargv[myargc++] = token;

                printf("%s\n", token);
                printf("\t\t%s\n", myargv[myargc - 1]);

                token = strtok(NULL, delimeter);
            }

//            // I'm not sure if I need this
//            free(buf);
//            buf = (char *) calloc(BUFFERSIZE, sizeof(char));
//            verify_memory_allocation(buf);

            if (bytes_read < BUFFERSIZE)
                break;
        }
        check_for_errors(bytes_read, "Read error...");
    } while (bytes_read > 0);           // Terminated by EOF (Ctrl+D)

    print_newline_char();

    free(myargv);
    free(buf);
    myargv = NULL;
    buf = NULL;
}

void strip(char * cmd, char character)
{
    while (*cmd++ != character);
    *(cmd - 1) = '\0';
}

void reallocate(char ** array, size_t number_of_items)
{
    *array = realloc(*array, number_of_items * sizeof(char *));
}

void display_prompt()
{
    ssize_t bytes_written = write(STDOUT_FILENO, PROMPT, PROMPTSIZE);
    check_for_errors(bytes_written, "Write error...");
}

void print_newline_char()
{
   ssize_t bytes_written = write(STDOUT_FILENO, "\n", 1);
   check_for_errors(bytes_written, "Write error...");
}

ssize_t get_user_input(char * buf)
{
    return read(STDIN_FILENO, buf, BUFFERSIZE);
}

void err_exit(const char * error_message)
{
   perror(error_message);
   exit(errno);
}

void check_for_errors(ssize_t status_code, const char * error_message)
{
    if (status_code < 0)
        err_exit(error_message);
}

void verify_memory_allocation(const char * ptr)
{
    if (!ptr)   // i.e. if ptr <= 0
        err_exit("Unable to allocate dynamic memory...");
}


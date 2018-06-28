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

void repl();
void display_prompt();
void print_newline_char();
void delete_newline_char(char *);

ssize_t get_user_input(char *);

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

    ssize_t bytes_read = 0;

    do {
        display_prompt();
        while ((bytes_read = get_user_input(buf)))
        {
            delete_newline_char(buf);

            if (strcmp(buf, TERMINATION_CMD) == 0)
                return;

            token = strtok(buf, delimeter);
            while (token)
            {
                printf("%s\n", token);
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

    free(buf);
    buf = NULL;
}

void delete_newline_char(char * cmd)
{
    while (*cmd++ != '\n');
    *(cmd - 1) = '\0';
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


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
#define BUFFERSIZE 256                  // RELEASE MODE
//#define BUFFERSIZE 8                  // DEBUG MODE
//#define BUFFERSIZE 4                  // DEBUG MODE

#define PROMPT "myShell >> "            // shell prompt
#define PROMPTSIZE sizeof(PROMPT)       // sizeof shell prompt
#define TERMINATION_CMD "exit"

#define ARGVMAX 32                      // initial number of tokens, but the value will double after each realloc()

int repl();
void display_prompt();

size_t strip(char *, char, ssize_t);

ssize_t get_user_input(char *);

void callocate_myargv(char **, size_t *, size_t *);
void reallocate(char **, size_t);
void err_exit(const char *);
void verify_memory_allocation(const char *);
void check_for_errors(ssize_t, const char *);

int main(int* argc, char** argv)
{
    return repl();
}

int repl()
{
    char * token;
    char * delimeter = " \t";

    char *buf = (char *) calloc(BUFFERSIZE, sizeof(char));
    verify_memory_allocation(buf);

    char** myargv = NULL;
    size_t myargc = 0;
    size_t max_items_allowed = 0;
    callocate_myargv((char **) &myargv, &myargc, &max_items_allowed);
    // allocate initially: ARGVMAX * sizeof(char *)

    ssize_t bytes_read = 0;

    do {
        display_prompt();
        while ((bytes_read = get_user_input(buf)))
        {
            size_t eol = strip(buf, '\n', bytes_read);
            if (strcmp(buf, TERMINATION_CMD) == 0)
                return EXIT_SUCCESS;

            token = strtok(buf, delimeter);
            while (token)
            {
                if (myargc - 1 >= max_items_allowed)
                    reallocate((char **) &myargv, (max_items_allowed *= 2));

                myargv[myargc++] = token;

//                printf("%s\n", token);
                printf("\t\t%s\n", myargv[myargc - 1]);

                token = strtok(NULL, delimeter);
            }

            if (bytes_read < BUFFERSIZE || eol)
                break;          // clear the buffer & display console prompt again
        }
        check_for_errors(bytes_read, "Read error...");

        free(buf);
        buf = calloc(BUFFERSIZE, sizeof(char));
        verify_memory_allocation(buf);

        free(myargv);
        callocate_myargv((char **) &myargv, &myargc, &max_items_allowed);

    } while (bytes_read > 0);           // Terminated by EOF (Ctrl+D)

    free(myargv);
    free(buf);
    myargv = NULL;
    buf = NULL;

    return EXIT_SUCCESS;
}

/** My implementation is a bit verbose and boring,
 * but I tried to make the code safe: I should account
 * for a case when a character is not found,
 * thus I need to include a boundary check for an array.
 *
 * Return either the last position where the character was
 * stripped, or return 0 if match was not found in the array. */
size_t strip(char * buf, const char character, const ssize_t bytes_read)
{
    size_t position = 0;
    for (int i = 0; i < bytes_read; i++) {
        if (buf[i] == character) {
           buf[i] = '\0';
           position = (size_t) i;
        }
    }
    return position;
}

void callocate_myargv(char ** myargv, size_t * myargc, size_t * max_items_allowed) {
    *myargv = calloc(ARGVMAX, sizeof(char *));
    verify_memory_allocation(*myargv);
    *max_items_allowed = ARGVMAX;
    *myargc = 0;
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


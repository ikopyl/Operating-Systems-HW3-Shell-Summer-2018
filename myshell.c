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
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

//Max amount allowed to read from input
#define BUFFERSIZE 256

#define PROMPT "myShell >> "            // shell prompt
#define PROMPTSIZE sizeof(PROMPT)       // sizeof shell prompt
#define TERMINATION_CMD "exit"

#define ARGVMAX 32                      // initial number of tokens, but the value will double after each realloc()
static size_t MAX_ITEMS_ALLOWED;

int repl();
void display_prompt();

size_t strip(char *, char, ssize_t);
ssize_t get_user_input(char *);
size_t tokenize_input(char *, char *, char **, size_t);

void callocate_myargv(char **, size_t *);
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
    char * delimiter = " \t";

    char * buf = NULL;
    char** myargv = NULL;
    size_t myargc = 0;
    ssize_t bytes_read = 0;

    do {
        buf = calloc(BUFFERSIZE, sizeof(char));
        verify_memory_allocation(buf);

        callocate_myargv((char **) &myargv, &myargc);

        display_prompt();
        while ((bytes_read = get_user_input(buf)))
        {
            size_t eol = strip(buf, '\n', bytes_read);
            if (strcmp(buf, TERMINATION_CMD) == 0)
                return EXIT_SUCCESS;

            myargc = tokenize_input(buf, delimiter, myargv, myargc);

            if (bytes_read < BUFFERSIZE || eol)
                break;          // clear buf & display console prompt again
        }
        check_for_errors(bytes_read, "Read error...");

//        printf("Current value of myargc: %zu\n", myargc);                    // DEBUG INFO
        myargv[myargc] = '\0';

        /** next 4 lines - DEBUG INFO */
//        size_t position = 0;
//        while (myargv[position]) {
//            printf("%s\n", myargv[position++]);
//        }

        // processes start here:

        int status = 0;
        pid_t pid = fork();
        check_for_errors(pid, "Fork failed...");

        if (pid == 0) {
            execvp(myargv[0], myargv);
        } else {
            wait(&status);
//            waitpid(pid, &status, WNOHANG);
        }

        

        free(buf);
        free(myargv);
        myargv = NULL;
        buf = NULL;

    } while (bytes_read > 0);           // Terminated by EOF (Ctrl+D)

    return EXIT_SUCCESS;
}

size_t tokenize_input(char * buf, char * delimiter, char ** myargv, size_t myargc)
{
    char * token = strtok(buf, delimiter);
    while (token) {

        if (myargc - 1 >= MAX_ITEMS_ALLOWED)
            reallocate(myargv, (MAX_ITEMS_ALLOWED *= 2));

        myargv[myargc++] = token;
        token = strtok(NULL, delimiter);
    }
    return myargc;
}

/** Return either the last position where the character was
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

void callocate_myargv(char ** myargv, size_t * myargc) {
    *myargv = calloc(ARGVMAX, sizeof(char *));
    verify_memory_allocation(*myargv);
    MAX_ITEMS_ALLOWED = ARGVMAX;
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


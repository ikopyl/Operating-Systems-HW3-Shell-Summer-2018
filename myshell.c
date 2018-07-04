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
#define PATH_MAX 4096

#define HOME "~"
#define LINK_TO_PREVIOUS_WORKING_DIRECTORY "-"
#define HOMESIZE sizeof(HOME)

#define PROMPT "myShell >> "            // shell prompt
#define PROMPTSIZE sizeof(PROMPT)       // sizeof shell prompt
#define BUILTIN_EXIT "exit"
#define BUILTIN_CD "cd"
#define BUILTIN_PWD "pwd"

#define ARGVMAX 32                      // initial number of tokens, but the value will double after each realloc()
static size_t MAX_ITEMS_ALLOWED;
static char * CURRENT_WORKING_DIRECTORY;

int repl();
void display_prompt();

size_t strip(char *, char, ssize_t);
ssize_t get_user_input(char *);
size_t tokenize_input(char *, char *, char **, size_t);

void callocate_myargv(char **, size_t *);
void reallocate(char **, size_t);

void err_exit(const char *);
void verify_memory_allocation(const char *);
void check_for_errors_and_terminate(ssize_t, const char *);
void check_for_errors_gracefully(ssize_t, const char *);

void execute_builtin(void (*fptr_builtin)(char *), char *);

int main(int* argc, char** argv)
{
    return repl();
}

int repl()
{
    CURRENT_WORKING_DIRECTORY = NULL;

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
            if (strcmp(buf, BUILTIN_EXIT) == 0)
                return EXIT_SUCCESS;

            myargc = tokenize_input(buf, delimiter, myargv, myargc);

            if (bytes_read < BUFFERSIZE || eol)
                break;          // clear buf & display console prompt again
        }
        check_for_errors_and_terminate(bytes_read, "Read error...");

//        printf("Current value of myargc: %zu\n", myargc);                    // DEBUG INFO
        myargv[myargc] = '\0';



        // builtins start here:


        if (strcmp(myargv[0], BUILTIN_CD) == 0) {

            CURRENT_WORKING_DIRECTORY = getcwd(CURRENT_WORKING_DIRECTORY, PATH_MAX);

            int status = 0;
            char * path = 0;

            if (strcmp(myargv[1], HOME) == 0) {
//                char * home = getenv("HOME");
                path = getenv("HOME");
                status = chdir(path);
            } else if (myargv[1][0] == '$') {
//                char * expanded_path = getenv(myargv[1]);
                path = getenv((const char *) myargv[1][1]);         // excluding $
                status = chdir(path);
            } else if (strcmp(myargv[1], LINK_TO_PREVIOUS_WORKING_DIRECTORY) == 0) {
                path = getenv("OLDPWD");
                chdir(getenv(path));
            } else {
                path = myargv[1];
                status = chdir(path);
            }
            check_for_errors_gracefully(status, myargv[1]);

            if (status) {
                status = setenv("OLDPWD", CURRENT_WORKING_DIRECTORY, 1);
                check_for_errors_gracefully(status, myargv[1]);
            }

            continue;
        }


        if (strcmp(myargv[0], BUILTIN_PWD) == 0) {

            CURRENT_WORKING_DIRECTORY = getcwd(CURRENT_WORKING_DIRECTORY, PATH_MAX);
            ssize_t bytes_written = printf("%s\n", CURRENT_WORKING_DIRECTORY);
            check_for_errors_gracefully(bytes_written, "Write error...");
            continue;
        }





        /** next 4 lines - DEBUG INFO */
//        size_t position = 0;
//        while (myargv[position]) {
//            printf("%s\n", myargv[position++]);
//        }

        // processes start here:
        int status = 0;
        pid_t pid = fork();
        check_for_errors_and_terminate(pid, "Failed to fork the existing process...");

        if (pid == 0) {
            execvp(myargv[0], myargv);
            err_exit(myargv[1]);
        } else {
            wait(&status);
//            waitpid(pid, &status, WNOHANG);             // should I use it for & use-case?
        }



        free(buf);
        free(myargv);
        myargv = NULL;
        buf = NULL;

    } while (bytes_read > 0);           // Terminated by EOF (Ctrl+D)

    return EXIT_SUCCESS;
}

void execute_builtin(void (*fptr_builtin)(char *), char * argsv)
{
    (*fptr_builtin)(argsv);
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
    check_for_errors_and_terminate(bytes_written, "Write error...");
}

ssize_t get_user_input(char * buf)
{
    return read(STDIN_FILENO, buf, BUFFERSIZE);
}

void err_exit(const char * error_message)
{
   perror(error_message);
   exit(EXIT_FAILURE);
}

void check_for_errors_and_terminate(ssize_t status_code, const char * error_message)
{
    if (status_code < 0)
        err_exit(error_message);
}

void check_for_errors_gracefully(ssize_t status_code, const char * error_message)
{
    if (status_code < 0)
        perror(error_message);
}

void verify_memory_allocation(const char * ptr)
{
    if (!ptr)   // i.e. if ptr <= 0
        err_exit("Unable to allocate dynamic memory...");
}


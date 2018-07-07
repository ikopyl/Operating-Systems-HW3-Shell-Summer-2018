/****************************************************************
 * Name        : Ilya Kopyl                                     *
 * Class       : CSC 415                                        *
 * Date        : 07/06/2018                                     *
 * Description :  Writing a simple bash shell program           *
 * 	              that will execute simple commands. The main   *
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

#define BUFFERSIZE 256                  /** Max number of bytes allowed to be read from input (at once) */
#define PATH_MAX 4096                   /** Max number of bytes in a path */

#define BUILTIN_CD "cd"
#define BUILTIN_PWD "pwd"
#define BUILTIN_EXIT "exit"

#define PIPE_SYMBOL "|"
#define REDIRECT_IN_SYMBOL "<"
#define REDIRECT_OUT_TRUNC_SYMBOL ">"
#define REDIRECT_OUT_APPEND_SYMBOL ">>"
#define BACKGROUND_PROCESS_SYMBOL "&"

#define HOME_SHORTCUT "~"
#define OLDPWD_SHORTCUT "-"

#define ENV_VAR_HOME "HOME"
#define ENV_VAR_OLDPWD "OLDPWD"
#define ENV_VAR_CHAR_RECOGNIZER '$'     /** if a token in input string starts with '$', it will be treated as an environment variable */

#define ARGVMAX 32                      /** initial number of tokens in myargv, but the value will double after each realloc() */

static size_t MAX_ITEMS_ALLOWED;
static char * CWD;
static char BACKGROUND_PROCESS;

static char REDIRECT_IN_DETECTED;
static char REDIRECT_OUT_TRUNC_DETECTED;
static char REDIRECT_OUT_APPEND_DETECTED;

static char * INFILE_PATH;
static char * OUTFILE_PATH;
static char * PATH_TO_FILE;

static int IN_FD;
static int OUT_FD;

static const char * HOME_PATH;

int repl();
void display_prompt();

ssize_t strip(char *, char, ssize_t);
size_t strip_myargv(char **, size_t *, const char *);

size_t strip_pipes_myargv(char**, size_t *, char ***, size_t *);

ssize_t get_user_input(char *);
size_t tokenize_input(char *, char *, char **, size_t);

void callocate_myargv(char **, size_t *);
void reallocate(char **, size_t);

void err_exit(const char *);
void verify_memory_allocation(const char *);
void check_for_errors_and_terminate(ssize_t, const char *);
void check_for_errors_gracefully(ssize_t, const char *);

void expand_home_directory_in_path(char **, const size_t *);
char * collapse_home_path(char *);
char * generate_prompt(char *);

char is_background_process(char **, size_t *);
void parse_redirects(char **, size_t *);

void enable_redirects();
void enable_input_redirect();
void enable_output_redirects();

void disable_redirects(const int *, const int *);

void builtin_cd(char **, const size_t *);
void builtin_pwd();

char builtin_found_and_executed(char **, const size_t *);
void execute_process(char **, size_t *);

int open_to_read(const char *);
int open_to_append_write(const char *);
int open_to_trunc_write(const char *);

void close_fd(int *);

int main(int* argc, char** argv)
{
    return repl();
}

int repl()
{
    HOME_PATH = getenv(ENV_VAR_HOME);
    CWD = NULL;

    IN_FD = 0;
    OUT_FD = 1;

    BACKGROUND_PROCESS = 0;

    char * delimiter = " \t";

    char * buf = NULL;
    char** myargv = NULL;
    size_t myargc = 0;
    ssize_t bytes_read = 0;

    do {
        ssize_t eol = 0;                /** stores a position of a newline character */

        buf = calloc(BUFFERSIZE, sizeof(char));
        verify_memory_allocation(buf);

        callocate_myargv((char **) &myargv, &myargc);

        display_prompt();
        while ((bytes_read = get_user_input(buf)))
        {
            eol = strip(buf, '\n', bytes_read);

            myargc = tokenize_input(buf, delimiter, myargv, myargc);

            /** if true, proceed to executing the commands, and then clear buf & display shell prompt again */
            if (bytes_read < BUFFERSIZE || eol >= 0)
                break;
        }
        check_for_errors_and_terminate(bytes_read, "Read error...");

        /** if input string is empty, skip the rest of the code and continue to a new loop iteration */
        if (eol == 0)
            continue;

        myargv[myargc] = '\0';
        BACKGROUND_PROCESS = is_background_process(myargv, &myargc);

        /** should precede the redirects parsing*/
        expand_home_directory_in_path(myargv, &myargc);

        /** redirects parsing starts here */
        parse_redirects(myargv, &myargc);

        /** code for handling builtins starts here: */
        if (builtin_found_and_executed(myargv, &myargc))
            continue;

        /** code for handling processes starts here: */
        execute_process(myargv, &myargc);


        free(buf);
        free(myargv);
        myargv = NULL;
        buf = NULL;

    } while (bytes_read > 0);           /** loop can also be terminated by EOF (Ctrl + D) */

    return EXIT_SUCCESS;
}

int open_to_read(const char * path)
{
    int fd = open(path, O_RDONLY);
    check_for_errors_gracefully(fd, "Failed to open a file to read from ");
    return fd;
}

int open_to_append_write(const char * path)
{
    int fd = open(path, O_CREAT|O_WRONLY|O_APPEND,  S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    check_for_errors_gracefully(fd, "Failed to open a file to write to (APPEND) ");
    return fd;
}

int open_to_trunc_write(const char * path)
{
    int fd = open(path, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    check_for_errors_gracefully(fd, "Failed to open a file to write to (TRUNCATE) ");
    return fd;
}

void close_fd(int * fd)
{
    check_for_errors_gracefully(close(*fd), "Failed to close the file descriptor...");
}

char builtin_found_and_executed(char **myargv, const size_t * myargc)
{
    if (*myargc == 1 && strcmp(myargv[0], BUILTIN_EXIT) == 0)
        exit(EXIT_SUCCESS);

    if (strcmp(myargv[0], BUILTIN_CD) == 0) {
        builtin_cd(myargv, myargc);
        return 1;
    }

    if (strcmp(myargv[0], BUILTIN_PWD) == 0) {
        builtin_pwd();
        return 1;
    }

    return 0;
}

void execute_process(char ** myargv, size_t * myargc)
{
    int status = 0;
    int stdin_backup = dup(STDIN_FILENO);
    int stdout_backup = dup(STDOUT_FILENO);


    /** pipes detection: */
    char ** tail_myargv = NULL;
    size_t tail_myargc = 0;
    size_t pipe_detected = 0;

    if (strip_pipes_myargv(myargv, myargc, &tail_myargv, &tail_myargc))
        pipe_detected = 1;



    int pipe_fd[2];                                 /** pipe file descriptor */
    if (pipe_detected) {
        status = pipe(pipe_fd);
        check_for_errors_gracefully(status, "Pipe failed to create");
    }

    pid_t pid1 = fork();
//    printf("[process %d has started.]\n", pid1);                            // DEBUG ONLY, DELETE BEFORE RELEASE
    check_for_errors_gracefully(pid1, "Fork failed...");


    if (pid1 == 0)          /** 1st child - head (on LHS from pipe) */
    {
        if (pipe_detected) {
            close(pipe_fd[0]);                          /** closing unused pipe file descriptor */
            dup2(pipe_fd[1], STDOUT_FILENO);            /** connecting output of pid1 to input of pid2 */

            enable_input_redirect();                    /** enable STDIN redirect; should only be applied to the first command */
        }
        else
        {
            enable_redirects();                         /** enable any redirects if available */
        }


        /** moving a background child to another process group */
        if (BACKGROUND_PROCESS)
            setpgid(pid1, 0);

        execvp(myargv[0], myargv);
        err_exit("Execvp of first child failed...");
    }
    else if (pid1 > 0)
    {
        if (pipe_detected)
        {
            int pid2 = fork();
            check_for_errors_gracefully(pid2, "Fork failed...");

            if (pid2 == 0)
            {
                close(pipe_fd[1]);                  /** closing the unused pipe file descriptor */
                dup2(pipe_fd[0], STDIN_FILENO);     /** connecting input of pid2 to output of pid1 */

                enable_output_redirects();          /** enable STDOUT redirects; should only be applied to the last command */

                execvp(tail_myargv[0], tail_myargv);
                perror("Execvp of second child failed...");
            }
            else if (pid2 > 0)
            {
                close(pipe_fd[0]);
                close(pipe_fd[1]);

                while(!wait(&status));
                while(!wait(&status));
            }
        }

        if (!BACKGROUND_PROCESS)
        {
            setpgid(pid1, getpgid(pid1));

            /** 0: wait for any child process whose group id is equal to that of the calling process */
            if ((pid1 = waitpid(0, &status, WUNTRACED)))
            {
                if (WIFEXITED(status)) {
//                    printf("[process %d exited with code %d]\n", pid1, WEXITSTATUS(status));                // DEBUG ONLY
                }
            }
        }

        /** -1: wait for any child process */
        if ((pid1 = waitpid(-1, &status, WNOHANG)) > 0)
        {
            if (WIFEXITED(status)) {
//                printf("[process %d exited with code %d]\n", pid1, WEXITSTATUS(status));                    // DEBUG ONLY
            }
        }

    }

    disable_redirects(&stdin_backup, &stdout_backup);

}

void expand_home_directory_in_path(char ** myargv, const size_t * myargc)
{
    for (int i = 1; i < *myargc; i++) {
        if (myargv[i][0] == HOME_SHORTCUT[0])
        {
            char * expanded_path = calloc(PATH_MAX, sizeof(char));
            expanded_path = memcpy(expanded_path, HOME_PATH, strlen(HOME_PATH));

            expanded_path = strcat(expanded_path, myargv[i] + 1);       /** leaving '~' behind the scope */
            myargv[i] = expanded_path;                                  /** myargv[i] now stores a pointer to expanded_path */
        }
    }
}

char * collapse_home_path(char * path)
{
    size_t home_len = strlen(HOME_PATH);
    char * extract = calloc(home_len, sizeof(char));

    memcpy(extract, path, home_len);

    if (strcmp(extract, HOME_PATH) == 0) {
        path = path + home_len - 1;
        path[0] = '~';
    }

    return path;
}

char * generate_prompt(char * path)
{
    path = collapse_home_path(path);
    char * new_prompt = calloc(PATH_MAX, sizeof(char));

    char * intro = "msh ";
    char * outro = " $ \0";

    memcpy(new_prompt, intro, strlen(intro));
    new_prompt = strcat(new_prompt, path);

    new_prompt = strcat(new_prompt, outro);

    return new_prompt;
}


size_t strip_pipes_myargv(char** myargv, size_t * myargc, char *** tail_myargv, size_t * tail_argc)
{
    size_t position = 0;
    while ((strip_myargv(myargv, myargc, PIPE_SYMBOL)))
    {
        *tail_myargv = &myargv[*myargc + 1];

        position = *myargc + 1;
        while(myargv[position]) {
//            printf("%s\n", myargv[position++]);
            position++;
        }
        *tail_argc = position - 1 - *myargc;
    }

    return position;
}

/** Function for parsing the redirect characters. Returns the position of the
 * first matched char * of 1 character from the end of myargv. Sets PATH_TO_FILE
 * to the value of the next token in the myargv array after the matched character (if it exists).
 * It replaces the found character with \0 and decrements myargc accordingly. */
size_t strip_myargv(char ** myargv, size_t * myargc, const char * search_item)
{
    for (int i = (int) (*myargc - 1); i > 0; i--)
    {
        if (strcmp((const char *) myargv[i], search_item) == 0)
        {
            if (*myargc - i > 1) {
                PATH_TO_FILE = myargv[i+1];
            }
            myargv[i] = '\0';
            *myargc = (size_t) i;

            return i;
        }
    }
    return 0;
}

char is_background_process(char ** myargv, size_t *myargc)
{
    if (strcmp(myargv[*myargc - 1], BACKGROUND_PROCESS_SYMBOL) == 0) {
        myargv[*myargc - 1] = '\0';
        *myargc -= 1;
        return 1;
    }
    return 0;
}

void parse_redirects(char ** myargv, size_t * myargc)
{
    if (strip_myargv(myargv, myargc, REDIRECT_OUT_TRUNC_SYMBOL)) {
        REDIRECT_OUT_TRUNC_DETECTED = 1;
        OUTFILE_PATH = PATH_TO_FILE;
        PATH_TO_FILE = NULL;
    }

    if (strip_myargv(myargv, myargc, REDIRECT_OUT_APPEND_SYMBOL)) {
        REDIRECT_OUT_APPEND_DETECTED = 1;
        OUTFILE_PATH = PATH_TO_FILE;
        PATH_TO_FILE = NULL;
    }

    if (strip_myargv(myargv, myargc, REDIRECT_IN_SYMBOL)) {
        REDIRECT_IN_DETECTED = 1;
        INFILE_PATH = PATH_TO_FILE;
        PATH_TO_FILE = NULL;
    }
}

void enable_redirects()
{
    enable_input_redirect();
    enable_output_redirects();
}

void enable_input_redirect()
{
    if (REDIRECT_IN_DETECTED) {
        IN_FD = open_to_read(INFILE_PATH);
        dup2(IN_FD, STDIN_FILENO);
    }
}

void enable_output_redirects()
{
    if (REDIRECT_OUT_TRUNC_DETECTED) {
        OUT_FD = open_to_trunc_write(OUTFILE_PATH);
        dup2(OUT_FD, STDOUT_FILENO);
    }

    if (REDIRECT_OUT_APPEND_DETECTED) {
        OUT_FD = open_to_append_write(OUTFILE_PATH);
        dup2(OUT_FD, STDOUT_FILENO);
    }
}

void disable_redirects(const int * stdin_backup, const int * stdout_backup)
{
    if (REDIRECT_IN_DETECTED) {
        close_fd(&IN_FD);
        dup2(*stdin_backup, STDIN_FILENO);
        REDIRECT_IN_DETECTED = 0;
    }

    if (REDIRECT_OUT_TRUNC_DETECTED) {
        close_fd(&OUT_FD);
        dup2(*stdout_backup, STDOUT_FILENO);
        REDIRECT_OUT_TRUNC_DETECTED = 0;
    }

    if (REDIRECT_OUT_APPEND_DETECTED) {
        close_fd(&OUT_FD);
        dup2(*stdout_backup, STDOUT_FILENO);
        REDIRECT_OUT_APPEND_DETECTED = 0;
    }
}

void builtin_cd(char ** myargv, const size_t * myargc)
{
    CWD = getcwd(CWD, PATH_MAX);
    char * path = 0;

    if (strcmp(myargv[1], HOME_SHORTCUT) == 0)
        path = (char *) HOME_PATH;
    else if (*myargc > 2 &&                                         /** bash also does not support redirects for cd */
            ((strcmp(myargv[1], REDIRECT_IN_SYMBOL) == 0)
             || (strcmp(myargv[1], REDIRECT_OUT_TRUNC_SYMBOL) == 0)
             || (strcmp(myargv[1], REDIRECT_OUT_APPEND_SYMBOL) == 0)
            )) {
        path = (char *) HOME_PATH;                               /** in any unusual situation - just go home */
    }
    else if (strcmp(myargv[1], OLDPWD_SHORTCUT) == 0)
        path = getenv(ENV_VAR_OLDPWD);
    else if (myargv[1][0] == ENV_VAR_CHAR_RECOGNIZER)
        path = getenv((const char *) myargv[1] + 1);            /** excluding '$' character */
    else
        path = myargv[1];

    ssize_t status = chdir(path);
    check_for_errors_gracefully(status, myargv[1]);

    if (status >= 0) {
        status = setenv(ENV_VAR_OLDPWD, CWD, 1);
        check_for_errors_gracefully(status, myargv[1]);
    }
}

void builtin_pwd()
{
    int stdin_backup = dup(STDIN_FILENO);
    int stdout_backup = dup(STDOUT_FILENO);
    enable_redirects();

    CWD = getcwd(CWD, PATH_MAX);
    ssize_t bytes_written = write(STDOUT_FILENO, CWD, strlen(CWD));
    check_for_errors_gracefully(bytes_written, "Write error...");
    bytes_written = write(STDOUT_FILENO, "\n", 1);
    check_for_errors_gracefully(bytes_written, "Write error...");

    disable_redirects(&stdin_backup, &stdout_backup);
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
 * stripped, or return -1 if match was not found in the array. */
ssize_t strip(char * buf, const char character, const ssize_t bytes_read)
{
    ssize_t position = -1;
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
    CWD = getcwd(CWD, PATH_MAX);
    char * new_prompt = generate_prompt(CWD);

    ssize_t bytes_written = write(STDOUT_FILENO, new_prompt, strlen(new_prompt));                   /** dynamic prompt */
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


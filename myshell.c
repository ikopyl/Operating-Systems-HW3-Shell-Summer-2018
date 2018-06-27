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

void repl();
void display_prompt();
ssize_t get_user_input(char *);
void verify_memory_allocation(const char *);
void check_for_errors(ssize_t, const char *);

int main(int* argc, char** argv)
{
    repl();
    return 0;
}

void repl()
{
    char *buf = (char *) calloc(BUFFERSIZE, sizeof(char));
    verify_memory_allocation(buf);

    while (1)
    {

        ssize_t bytes_read = 0;
        ssize_t bytes_written = 0;

        display_prompt();

//    while ((bytes_read = get_user_input(buf)))
        while ((bytes_read = read(0, buf, BUFFERSIZE))) {
//            printf("Bytes read: %lu\n", bytes_read);                 // DEBUG ONLY, DELETE BEFORE RELEASE
            bytes_written = write(1, buf, (size_t) bytes_read);
//            printf("\nBytes written: %lu\n", bytes_written);           // DEBUG ONLY, DELETE BEFORE RELEASE
//            puts("---------------------------------------");          // DEBUG ONLY, DELETE BEFORE RELEASE
            check_for_errors(bytes_written, "Write error...");

            // I'm not sure if I need this
            free(buf);
            buf = (char *) calloc(BUFFERSIZE, sizeof(char));
            verify_memory_allocation(buf);

            if (bytes_read < BUFFERSIZE)
                break;
        }
        check_for_errors(bytes_read, "Read error...");

        if (bytes_read == 0)
            break;
    }
    free(buf);
    buf = NULL;
}

void display_prompt()
{
    ssize_t bytes_written = write(1, PROMPT, PROMPTSIZE);
    check_for_errors(bytes_written, "Write error...");
}

ssize_t get_user_input(char * buf) {
    display_prompt();
    return read(0, buf, BUFFERSIZE);
}

void check_for_errors(ssize_t status_code, const char * error_message) {
    if (status_code < 0) {
        perror(error_message);
        exit(errno);
    }
}

void verify_memory_allocation(const char * ptr)
{
    if (!ptr) {     // i.e. if ptr <= 0
        perror("Unable to allocate dynamic memory...");
        exit(errno);
    }
}


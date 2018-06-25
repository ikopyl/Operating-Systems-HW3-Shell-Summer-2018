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

void verify_memory_allocation(size_t);
void check_for_errors(ssize_t, char *);

int main(int* argc, char** argv)
{
    char *buf = (char *) calloc(BUFFERSIZE, sizeof(char));
    verify_memory_allocation((size_t) buf);

    ssize_t bytes_read = 0;
    ssize_t bytes_written = 0;

    while ((bytes_read = read(0, buf, BUFFERSIZE)))
    {
        bytes_written = write(1, buf, bytes_read);
        check_for_errors(bytes_written, "Write error...");
    }
    check_for_errors(bytes_read, "Read error...");


    return 0;
}

void check_for_errors(ssize_t status_code, char * error_message) {
    if (status_code < 0) {
        perror(error_message);
        exit(errno);
    }
}

void verify_memory_allocation(size_t ptr)
{
    if (!ptr) {     // i.e. if ptr <= 0
        perror("Unable to allocate dynamic memory...");
        exit(errno);
    }
}


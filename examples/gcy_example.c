#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define GCY_MODE 1
#define GCY_IMPLEMENTATION 1
#include "../src/gcy.h"

int main()
{
    pid_t process_id = fork();

    if (process_id == -1)
    {
        perror("Fork failed!!!\n");
        exit(EXIT_FAILURE);
    }

    if (process_id == 0)
    {
        for (size_t i = 0; i < 50; ++i)
        {
            GCY_MALLOC(i);
        }
        exit(EXIT_SUCCESS);
    }

    if (process_id != 0)
    {
        int status = 0;
        waitpid(process_id, &status, 0);
    }
}

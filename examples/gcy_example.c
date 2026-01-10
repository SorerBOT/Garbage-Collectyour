#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define GCY_MODE
#define GCY_IMPLEMENTATION
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
        void* array[50];
        for (size_t i = 0; i < 50; ++i)
        {
            if (i % 2 == 0)
            {
                array[i] = GCY_MALLOC(i);
            }
            else
            {
                array[i] = GCY_CALLOC(i, 1);
            }
        }
        for (size_t i = 0; i < 50; ++i)
        {
            GCY_FREE(array[i]);
        }
        exit(EXIT_SUCCESS);
    }

    if (process_id != 0)
    {
        int status = 0;
        waitpid(process_id, &status, 0);
    }
}

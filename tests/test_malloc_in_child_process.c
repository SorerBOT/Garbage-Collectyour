#include <stdio.h>
#include <unistd.h>

#include "external/cunit.h"

#define GCY_MODE 1
#include "../src/gcy.h"


CUNIT_TEST(test_malloc_in_child_process)
{
    pid_t child_pid = fork();

    if (child_pid == -1)
    {
        perror("Error: fork in Garbage-Collectyour tests");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0)
    {
        int* my_leaky_allocation = GCY_MALLOC(sizeof(int));
        exit(EXIT_SUCCESS);
    }

    if (child_pid > 0)
    {
        int status = 0;
        waitpid(child_pid, &status, 0);
        CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), 1);
    }
}

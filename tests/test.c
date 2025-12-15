#include <string.h>
#include <stdio.h>
#define GCY_MODE 1
#define GCY_IMPLEMENTATION 1
#include "../gcy.h"

#define CHECK_MARK "✅"

typedef enum
{
    false,
    true
} boolean;

void assert(boolean condition);

int main()
{
    char* my_str = "hello world\n";
    char* ptr_1 = (char*)GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_2 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_3 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_4 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_5 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_6 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_7 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_8 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_9 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    memcpy(ptr_1, my_str, strlen(my_str));

    assert(gcy_debug_get_allocations_count() == 9);
    assert(gcy_debug_get_allocations_count() == 9);
    assert(gcy_debug_get_allocations_count() == 9);
    assert(gcy_debug_get_allocations_count() == 9);
    assert(gcy_debug_get_allocations_count() == 9);


    GCY_FREE(ptr_1);
    GCY_FREE(ptr_2);
    GCY_FREE(ptr_3);
    GCY_FREE(ptr_4);
    GCY_FREE(ptr_5);
    GCY_FREE(ptr_6);
    GCY_FREE(ptr_7);
    GCY_FREE(ptr_8);
    GCY_FREE(ptr_9);
}

void assert(boolean condition)
{
    static size_t test_number = 1;
    if (condition)
    {
        printf("Test: %lu PASSED ✅\n", test_number);
    }
    else
    {
        printf("Test: %lu FAILED ❌\n", test_number);
    }
    ++test_number;
}

#include <string.h>
#include <stdio.h>
#define GCY_MODE 1
#define GCY_IMPLEMENTATION 1
#include "../lib/gcy.h"

int main()
{
    char* my_str = "hello world\n";
    char* ptr = (char*)GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_1 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_2 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_3 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_4 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_5 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_6 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_7 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_8 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    memcpy(ptr, my_str, strlen(my_str));

    GCY_FREE(ptr);
    GCY_FREE(ptr_1);
    GCY_FREE(ptr_2);
    GCY_FREE(ptr_3);
    GCY_FREE(ptr_4);
    GCY_FREE(ptr_5);
    GCY_FREE(ptr_6);
    GCY_FREE(ptr_7);
    GCY_FREE(ptr_8);
}

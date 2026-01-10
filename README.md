# Garbage-Collectyour
As the name implies, this is not a Garbage Collector, but it also isn't far from working towards the same end. The Garbage Collector _header only_ library, aims to create a wrapper around C's _malloc_ and _free_ which would keep track of memory allocations, and provide the user with a detailed rundown of memory issues when the program exits, hence Garbage CollectYOUR.

## Features
* Header Only, the library file is single, and ready to mingle!
* Barely any runtime overhead! Every `malloc` / `free` call has an overhead of `O(1)`
## Inspiration
Felt like it'd be interesting to learn how Valgrind's memcheck works under the hood. A short google-search taught me that Valgrind somehow takes the compiled code, disassembles adds its own shenanigans and recompiles it, then allocates whatever memory the user allocates a second time in order to keep track of it.

Personally, I'm not there yet, but perhaps this would be the cornerstone.
On a serious note, I use Valgrind's memcheck occasionally, and I feel as if this _petit_ library might actually have a significant advantage over Valgrind, which is the SPEED with which it runs. In Valgrind's own words:
>Your program will run much slower (eg. 20 to 30 times) than normal, and use a lot more memory. Memcheck will issue messages about memory errors and leaks that it detects.

on the other hand, my program's memory footprint is significantly smaller (at least if the application is big) and the overhead is only a fragment of Valgrind's.

## Example
In the following code, the programmer (me) forgot to deallocate some memory. 
```c
#include <string.h>
#include <stdio.h>
#define GCY_MODE
#define GCY_IMPLEMENTATION
#include "gcy.h"

int main()
{
    char* my_str = "hello world\n";
    size_t len = strlen(my_str) + 1;
    char* ptr = (char*)GCY_MALLOC(len * sizeof(char));
    char* ptr_1 = GCY_MALLOC(len * sizeof(char));
    char* ptr_2 = GCY_MALLOC(len * sizeof(char));
    char* ptr_3 = GCY_MALLOC(len * sizeof(char));
    char* ptr_4 = GCY_MALLOC(len * sizeof(char));
    char* ptr_5 = GCY_MALLOC(len * sizeof(char));
    char* ptr_6 = GCY_MALLOC(len * sizeof(char));
    char* ptr_7 = GCY_MALLOC(len * sizeof(char));
    char* ptr_8 = GCY_MALLOC(len * sizeof(char));
    memcpy(ptr, my_str, strlen(my_str));

    GCY_FREE(ptr);
    GCY_FREE(ptr_1);
    GCY_FREE(ptr_2);
}
```
Garbage Collect-your steps into action after the your code finishes, and outputs the following:
```shell
=============================================
Garbage Collect-your Data:
File: gcy_example.c, line: 13, size: 12, address: 0x6000022540a0
File: gcy_example.c, line: 14, size: 12, address: 0x6000022540c0
File: gcy_example.c, line: 15, size: 12, address: 0x6000022540e0
File: gcy_example.c, line: 16, size: 12, address: 0x600002254100
File: gcy_example.c, line: 17, size: 12, address: 0x600002254120
File: gcy_example.c, line: 18, size: 12, address: 0x600002254140
=============================================
```
You now know exactly what memory you forgot to free. Say you fixed your code, to get:
```c
#include <string.h>
#include <stdio.h>
#define GCY_MODE
#define GCY_IMPLEMENTATION
#include "gcy.h"

int main()
{
    char* my_str = "hello world\n";
    size_t len = strlen(my_str) + 1;
    char* ptr = (char*)GCY_MALLOC(len * sizeof(char));
    char* ptr_1 = GCY_MALLOC(len * sizeof(char));
    char* ptr_2 = GCY_MALLOC(len * sizeof(char));
    char* ptr_3 = GCY_MALLOC(len * sizeof(char));
    char* ptr_4 = GCY_MALLOC(len * sizeof(char));
    char* ptr_5 = GCY_MALLOC(len * sizeof(char));
    char* ptr_6 = GCY_MALLOC(len * sizeof(char));
    char* ptr_7 = GCY_MALLOC(len * sizeof(char));
    char* ptr_8 = GCY_MALLOC(len * sizeof(char));
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
```
Garbage Collect-you will now inform you that that all allocations were free'd:
```shell
=============================================
Garbage Collect-your Data:
No garbage to collect.
=============================================
```

#ifndef GCY_H
#define GCY_H

#ifdef GCY_MODE
#include <stdlib.h>
typedef struct Allocation
typedef struct Allocations

void* gcy_malloc(size_t size);
void gcy_free(void* ptr);
void gcy_print_errors();

#define GCY_MALLOC(size) gcy_malloc((size), __FILE__, __LINE__)
#define GCY_FREE(ptr) gcy_free((ptr))
#else
#define GCY_MALLOC(size) malloc((size))
#define GCY_FREE(ptr) free((ptr))
#endif

#endif

#ifdef GCY_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    size_t size;
    char* file;
    int line;
    void* ptr;
} Allocation;
typedef struct
{
    Allocation* alloc;
    Allocations* next;
} Allocations;

Allocations* allocList = NULL;

void* gcy_malloc(size_t size, char* file, char* line)
{
    void* ptr = malloc(size);
    if (ptr == NULL)
    {
        fprintf(stderr, "Error: malloc");
    }

    Allocations* root_new = malloc(sizeof(Allocations));
    if (root_new == NULL)
    {
        fprintf(stderr, "Error: malloc");
    }

    root_new->alloc = malloc(sizeof(Allocation));
    if (root_new->alloc == NULL)
    {
        fprintf(stderr, "Error: malloc");
    }
    root_new->next = allocList;
    allocList = root_new;

    *root_new->alloc = (Allocation)
    {
        .size = size,
        .file = file,
        .line = line,
        .ptr  = ptr
    };

}
#define GCY_FREE(ptr) gcy_free((ptr))
{}

#endif

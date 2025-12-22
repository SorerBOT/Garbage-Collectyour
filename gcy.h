/*
 * Copyright (c) 2025 Alon Filler
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef GCY_H
#define GCY_H

#define EXIT_INCONSISTENT_ALLOC_LIST 11

#include <stdlib.h>
#ifdef GCY_MODE

typedef struct
{
    size_t size;
    char* file;
    int line;
    void* ptr;
} GCY_Allocation;
typedef struct GCY_AllocationsList
{
    GCY_Allocation* alloc;
    struct GCY_AllocationsList* next;
} GCY_AllocationsList;

void* gcy_malloc(size_t size, char* file, int line);
void gcy_free(void* ptr);
void gcy_print_allocations();
GCY_AllocationsList* gcy_debug_get_allocations();
size_t gcy_debug_get_allocations_count();

#define GCY_MALLOC(size) gcy_malloc((size), __FILE__, __LINE__)
#define GCY_FREE(ptr) gcy_free((ptr))
#else /* GCY_MODE */
#define GCY_MALLOC(size) malloc((size))
#define GCY_FREE(ptr) free((ptr))
#endif
#endif

#ifdef GCY_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>


GCY_AllocationsList* allocList = NULL;
GCY_AllocationsList* last_allocation = NULL;
size_t allocsCount = 0;

void* gcy_malloc(size_t size, char* file, int line)
{
    void* ptr = malloc(size);
    if (ptr == NULL)
    {
        fprintf(stderr, "Error: malloc");
        exit(EXIT_FAILURE);
    }

    GCY_AllocationsList* root_new = (GCY_AllocationsList*)malloc(sizeof(GCY_AllocationsList));
    if (root_new == NULL)
    {
        fprintf(stderr, "Error: malloc in Garbage-Collectyour library allocation");
        exit(EXIT_FAILURE);
    }

    root_new->alloc = (GCY_Allocation*)malloc(sizeof(GCY_Allocation));
    if (root_new->alloc == NULL)
    {
        fprintf(stderr, "Error: malloc in Garbage-Collectyour library allocation");
        exit(EXIT_FAILURE);
    }
    *root_new->alloc = (GCY_Allocation)
    {
        .size = size,
        .file = file,
        .line = line,
        .ptr  = ptr
    };
    root_new->next = NULL;

    if (last_allocation == NULL) /* This implies that allocList is also NULL */
    {
        if (allocList != NULL)
        {
            fprintf(stderr, "Error: GCY internal error.");
            exit(EXIT_INCONSISTENT_ALLOC_LIST);
        }

        last_allocation = root_new;
        allocList = root_new;
    }
    else
    {
        last_allocation->next = root_new;
        last_allocation = root_new;
    }

    ++allocsCount;
    return ptr;
}
void gcy_free_allocation_node(GCY_AllocationsList* node)
{
    free(node->alloc->ptr);
    free(node);
}

void gcy_free(void* ptr)
{
    if (ptr == NULL)
    {
        return;
    }
    GCY_AllocationsList* temp = allocList;
    if (temp->alloc->ptr == ptr)
    {
        allocList = allocList->next;
        gcy_free_allocation_node(temp);
        --allocsCount;
        return;
    }
    while (temp != NULL)
    {
        if (temp->next->alloc->ptr == ptr)
        {
            GCY_AllocationsList* ptr_node = temp->next;
            temp->next = temp->next->next;
            gcy_free_allocation_node(ptr_node);
            --allocsCount;
            return;
        }
        temp = temp->next;
    }
}
void gcy_print_allocation(const GCY_Allocation* allocation)
{
    printf("File: %s, line: %d, size: %lu, address: %p\n", allocation->file, allocation->line, allocation->size, allocation->ptr);
}

__attribute__((destructor))
void gcy_print_allocations()
{

    printf("=============================================\n");
    printf("Garbage Collect-your Data:\n");
    if (allocList == NULL)
    {
        printf("No garbage to collect.\n");
    }
    GCY_AllocationsList* temp = allocList;
    while (temp != NULL)
    {
        gcy_print_allocation(temp->alloc);
        temp = temp->next;
    }
    printf("=============================================\n");
}
GCY_AllocationsList* gcy_debug_get_allocations()
{
    return allocList;
}
size_t gcy_debug_get_allocations_count()
{
    return allocsCount;
}

#endif /* GCY_IMPLEMENTATION */

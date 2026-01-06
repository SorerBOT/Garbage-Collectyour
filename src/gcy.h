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

typedef struct _GCY_linked_list
{
    struct _GCY_linked_list* next_node;
} GCY_linked_list_t;

typedef struct
{
    GCY_linked_list_t list_data;
    size_t size;
    char* file;
    int line;
    void* ptr;
} GCY_Allocation;

void* gcy_malloc(size_t size, char* file, int line);
void gcy_free(void* ptr);
void gcy_print_allocations();
GCY_Allocation* gcy_debug_get_allocations();
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


GCY_Allocation* allocList = NULL;
GCY_Allocation* last_allocation = NULL;
size_t allocsCount = 0;

void* gcy_malloc(size_t size, char* file, int line)
{
    void* ptr = malloc(size);
    if (ptr == NULL)
    {
        fprintf(stderr, "Error: malloc");
        exit(EXIT_FAILURE);
    }

    GCY_Allocation* root_new = malloc(sizeof(GCY_Allocation));
    if (root_new == NULL)
    {
        fprintf(stderr, "Error: malloc in Garbage-Collectyour library allocation");
        exit(EXIT_FAILURE);
    }

    *root_new = (GCY_Allocation)
    {
        .list_data = (GCY_linked_list_t)
        {
            .next_node = NULL
        },
        .size = size,
        .file = file,
        .line = line,
        .ptr  = ptr
    };

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
        last_allocation->list_data.next_node = (GCY_linked_list_t*) root_new;
        last_allocation = root_new;
    }

    ++allocsCount;
    return ptr;
}

void gcy_free_allocation_node(GCY_Allocation* node)
{
    free(node->ptr);
    free(node);
}

void gcy_free(void* ptr)
{
    if (ptr == NULL)
    {
        return;
    }
    GCY_Allocation* temp = allocList;
    if (temp->ptr == ptr)
    {
        allocList = (GCY_Allocation*) allocList->list_data.next_node;
        gcy_free_allocation_node(temp);
        --allocsCount;
        return;
    }
    while (temp != NULL)
    {
        GCY_Allocation* next_node = (GCY_Allocation*) temp->list_data.next_node;
        if (next_node->ptr == ptr)
        {
            temp->list_data.next_node = next_node->list_data.next_node;
            gcy_free_allocation_node(next_node);
            --allocsCount;
            return;
        }
        temp = next_node;
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
    GCY_Allocation* temp = allocList;
    while (temp != NULL)
    {
        gcy_print_allocation(temp);
        temp = (GCY_Allocation*) temp->list_data.next_node;
    }
    printf("=============================================\n");
}

GCY_Allocation* gcy_debug_get_allocations()
{
    return allocList;
}

size_t gcy_debug_get_allocations_count()
{
    return allocsCount;
}

#endif /* GCY_IMPLEMENTATION */

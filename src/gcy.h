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

typedef enum
{
    GCY_EVENT_ALLOC,
    GCY_EVENT_FREE
} GCY_Event_Type;

typedef struct
{
    size_t size;
    const char* file;
    int line;
    const void* ptr;
    GCY_Event_Type event_type;
    size_t event_number;
} GCY_Event;

typedef struct
{
    volatile size_t length;
    pid_t original_process_id;
    GCY_Event events[];
} GCY_Profiler;

void* gcy_malloc(size_t size, const char* file, int line);
void* gcy_calloc(size_t count, size_t size, const char* file, int line);
void gcy_free(void* ptr);
void gcy_print_allocations();
GCY_Event* gcy_debug_get_allocations();
size_t gcy_debug_get_allocations_count();

#define GCY_MALLOC(size) gcy_malloc((size), __FILE__, __LINE__)
#define GCY_CALLOC(count, size) gcy_calloc((count), (size), __FILE__, __LINE__)
#define GCY_FREE(ptr) gcy_free((ptr))


#else /* GCY_MODE */
#define GCY_MALLOC(size) malloc((size))
#define GCY_FREE(ptr) free((ptr))
#endif
#endif

#ifdef GCY_IMPLEMENTATION

#include <sys/mman.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#define GCY_KILO 1024
#define GCY_MEGA (GCY_KILO * 1024)
#define GCY_PROFILER_SIZE (32 * GCY_MEGA)

static GCY_Profiler* profiler = NULL;
static int gcy__internal_event_cmp(const void* first_event, const void* second_event);
static void gcy__internal_init_profiler();
static void gcy__internal_print_overview();
static void gcy__internal_append_allocation_event(const void* ptr, size_t size, const char* file, int line);

static int gcy__internal_event_cmp(const void* first, const void* second)
{
    const GCY_Event* first_event = first;
    const GCY_Event* second_event = second;

    if (first_event->ptr != second_event->ptr)
    {
        return first_event->ptr - second_event->ptr;
    }

    return first_event->event_number - second_event->event_number;
}



/*
 *        [ (ALLOCATED MEMORY, address, size, file name, line number), (FREE MEMORY, address) ]
 */
__attribute__((constructor))
static void gcy__internal_init_profiler()
{
    if (profiler != NULL)
    {
        return;
    }

    profiler = mmap(NULL, GCY_PROFILER_SIZE,
            PROT_READ | PROT_WRITE,
            MAP_ANON | MAP_SHARED,
            -1, 0);

    if (profiler == NULL)
    {
        perror("GCY: failed to allocate profiler.\n");
        exit(EXIT_FAILURE);
    }

    *profiler = (GCY_Profiler)
    {
        .length = 0,
        .original_process_id = getpid()
    };
}

__attribute__((destructor))
static void gcy__internal_print_overview()
{
    if (getpid() != profiler->original_process_id)
    {
        return;
    }

    printf("=====================================\n");
    printf("Garbage Collect-your Data:\n");

    size_t total_leaks = 0;
    size_t total_leaks_bytes = 0;

    qsort(profiler->events, profiler->length, sizeof(GCY_Event), gcy__internal_event_cmp);

    for (size_t i = 0; i < profiler->length; ++i)
    {
        GCY_Event event = profiler->events[i];

        if (event.event_type == GCY_EVENT_ALLOC)
        {
            bool is_freed = false;
            if (i < profiler->length - 1)
            {
                GCY_Event event_next = profiler->events[i+1];
                if (event.ptr == event_next.ptr
                        && event_next.event_type == GCY_EVENT_FREE)
                {
                    is_freed = true;
                }
            }

            if (!is_freed)
            {
                ++total_leaks;
                total_leaks_bytes += event.size;
                printf("File: %s, line: %d, size: %lu, address: %p\n", event.file, event.line, event.size, event.ptr);
            }
        }

    }

    printf("Leaked a total of %lu bytes in %lu allocations.\n", total_leaks_bytes, total_leaks);
    printf("=====================================\n");

    munmap(profiler, GCY_PROFILER_SIZE);
}

static void gcy__internal_append_allocation_event(const void* ptr, size_t size, const char* file, int line)
{
    size_t old_length = __sync_fetch_and_add(&profiler->length, 1);

    GCY_Event event =
    {
        .ptr    = ptr,
        .size   = size,
        .event_type  = GCY_EVENT_ALLOC,
        .file   = file,
        .line   = line,
        .event_number = old_length
    };

    profiler->events[old_length] = event;
}
void* gcy_calloc(size_t count, size_t size, const char* file, int line)
{
    void* ptr = calloc(count, size);

    if (ptr == NULL)
    {
        perror("GCY: failed to allocate memory for user.\n");
        exit(EXIT_FAILURE);
    }

    gcy__internal_append_allocation_event(ptr, count * size, file, line);

    return ptr;
}
void* gcy_malloc(size_t size, const char* file, int line)
{
    void* ptr = malloc(size);

    if (ptr == NULL)
    {
        perror("GCY: failed to allocate memory for user.\n");
        exit(EXIT_FAILURE);
    }

    gcy__internal_append_allocation_event(ptr, size, file, line);

    return ptr;
}

void gcy_free(void* ptr)
{
    free(ptr);

    size_t old_length = __sync_fetch_and_add(&profiler->length, 1);

    GCY_Event event =
    {
        .ptr            = ptr,
        .event_type     = GCY_EVENT_FREE,
        .event_number   = old_length
    };

    profiler->events[old_length] = event;
}


#endif /* GCY_IMPLEMENTATION */

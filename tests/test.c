#include <string.h>
#include <stdio.h>
#define GCY_MODE 1
#define GCY_IMPLEMENTATION 1
#include "../lib/gcy.h"
#include "../external/cunit.h"

CUNIT_TEST(test_single)
{
    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), 0);
    char* my_str = "hello world\n";
    char* ptr_1 = GCY_MALLOC(strlen(my_str) * sizeof(char));

    CUNIT_ASSERT_PTR_NOT_NULL(ptr_1);
    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), 1);

    GCY_FREE(ptr_1);
    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), 0);
}

CUNIT_TEST(test_multiple)
{
    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), 0);
    char* my_str = "hello world\n";
    char* ptr_1 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    CUNIT_ASSERT_PTR_NOT_NULL(ptr_1);
    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), 1);

    char* ptr_2 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    CUNIT_ASSERT_PTR_NOT_NULL(ptr_2);
    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), 2);

    char* ptr_3 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    CUNIT_ASSERT_PTR_NOT_NULL(ptr_3);
    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), 3);

    GCY_FREE(ptr_1);
    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), 2);

    GCY_FREE(ptr_2);
    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), 1);

    GCY_FREE(ptr_3);
    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), 0);
}

CUNIT_TEST(free_in_different_order)
{
    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), 0);
    char* my_str = "hello world\n";
    char* ptr_1 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_2 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_3 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_4 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_5 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_6 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_7 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_8 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_9 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_10 = GCY_MALLOC(strlen(my_str) * sizeof(char));
    char* ptr_11 = GCY_MALLOC(strlen(my_str) * sizeof(char));

    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), 11);

    /*
     * Freeing one of the allocations
     */
    char* working_ptr = ptr_4;
    GCY_FREE(working_ptr);
    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), 10);
    size_t amount_of_allocs_found = 0;
    GCY_Allocation* allocs = gcy_debug_get_allocations();
    while (allocs != NULL)
    {
        CUNIT_ASSERT_PTR_NEQ(allocs->ptr, working_ptr);
        ++amount_of_allocs_found;
        allocs = (GCY_Allocation*) allocs->list_data.next_node;
    }
    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), amount_of_allocs_found);

    /*
     * Freeing another one of the allocations
     */
    working_ptr = ptr_8;
    GCY_FREE(working_ptr);
    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), 9);
    amount_of_allocs_found = 0;
    allocs = gcy_debug_get_allocations();
    while (allocs != NULL)
    {
        CUNIT_ASSERT_PTR_NEQ(allocs->ptr, working_ptr);
        ++amount_of_allocs_found;
        allocs = (GCY_Allocation*) allocs->list_data.next_node;
    }
    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), amount_of_allocs_found);

    /*
     * Freeing another one of the allocations
     */
    working_ptr = ptr_11;
    GCY_FREE(working_ptr);
    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), 8);
    amount_of_allocs_found = 0;
    allocs = gcy_debug_get_allocations();
    while (allocs != NULL)
    {
        CUNIT_ASSERT_PTR_NEQ(allocs->ptr, working_ptr);
        ++amount_of_allocs_found;
        allocs = (GCY_Allocation*) allocs->list_data.next_node;
    }
    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), amount_of_allocs_found);

    /*
     * Freeing another one of the allocations
     */
    working_ptr = ptr_1;
    GCY_FREE(working_ptr);
    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), 7);
    amount_of_allocs_found = 0;
    allocs = gcy_debug_get_allocations();
    while (allocs != NULL)
    {
        CUNIT_ASSERT_PTR_NEQ(allocs->ptr, working_ptr);
        ++amount_of_allocs_found;
        allocs = (GCY_Allocation*) allocs->list_data.next_node;
    }
    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), amount_of_allocs_found);

    /*
     * Freeing another one of the allocations
     */
    working_ptr = ptr_3;
    GCY_FREE(working_ptr);
    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), 6);
    amount_of_allocs_found = 0;
    allocs = gcy_debug_get_allocations();
    while (allocs != NULL)
    {
        CUNIT_ASSERT_PTR_NEQ(allocs->ptr, working_ptr);
        ++amount_of_allocs_found;
        allocs = (GCY_Allocation*) allocs->list_data.next_node;
    }
    CUNIT_ASSERT_INT_EQ(gcy_debug_get_allocations_count(), amount_of_allocs_found);
}

int main()
{
    cunit_run_registered_tests();
    cunit_free_tests(); /* This is completely optional as this function also runs in the destructor */
}

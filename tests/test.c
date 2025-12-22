#include <string.h>
#include <stdio.h>
#define GCY_MODE 1
#define GCY_IMPLEMENTATION 1
#include "../gcy.h"
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

int main()
{
    cunit_run_registered_tests();
    cunit_free_tests(); /* This is completely optional as this function also runs in the destructor */
}

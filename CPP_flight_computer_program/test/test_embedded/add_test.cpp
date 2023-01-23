#include <unity.h>

void test_add(void)
{
    TEST_ASSERT_EQUAL(2, 1 + 1);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_add);
    return UNITY_END();
}

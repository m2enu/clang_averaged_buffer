/**
 * @file    test_main.c
 * @brief   Unit test template
 * @author  m2enu
 * @par     License
 * https://github.com/m2enu/clang_averaged_buffer/blob/main/LICENSE
 */
#include "unity.h"
#include "test_simple_averaged_buffer.h"

/** Unused indicator to suppress compiler warning
 */
#define UNUSED_PARAMETER(x)         ((void)x)

/** Unit test setup
 */
void setUp(void)
{
}

/** Unit test teardown
 */
void tearDown(void)
{
}

/** Unit test main
 */
int main(int argc, char **argv)
{
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);

    UNITY_BEGIN();
    RUN_TEST(TestSimpleAveragedBuffer);
    return UNITY_END();
}

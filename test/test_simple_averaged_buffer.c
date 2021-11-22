/**
 * @file    test_averaged_buffer_simple.c
 * @brief   Unit test for simple averaged buffer
 * @author  m2enu
 * @par     License
 * https://github.com/m2enu/clang_averaged_buffer/blob/main/LICENSE
 */
#include "test_simple_averaged_buffer.h"
#include "iaveraged_buffer.h"
#include "unity.h"
#include <stddef.h>

void TestSimpleAveragedBuffer(void)
{
    /* Prepare test target */
    static const AveragedBufferConfig config = {AVERAGED_BUFFER_ALGO_SIMPLE, 4, NULL};
    IAveragedBuffer* pTarget = AveragedBufferCreate(&config);
    TEST_ASSERT_NOT_EQUAL(NULL, pTarget);

    /* Acquire average on buffer empty */
    AveragedBufferType ErrorCode;
    TEST_ASSERT_EQUAL(0, AveragedBufferAverage(pTarget, &ErrorCode));
    TEST_ASSERT_EQUAL(AVERAGED_BUFFER_ERROR_NONE, ErrorCode);

    /* Add value to the buffer */
    TEST_ASSERT_EQUAL(AVERAGED_BUFFER_ERROR_NONE, AveragedBufferAdd(pTarget, 100));
    TEST_ASSERT_EQUAL(AVERAGED_BUFFER_ERROR_NONE, AveragedBufferAdd(pTarget, 200));
    TEST_ASSERT_EQUAL(AVERAGED_BUFFER_ERROR_NONE, AveragedBufferAdd(pTarget, 300));
    TEST_ASSERT_EQUAL(AVERAGED_BUFFER_ERROR_NONE, AveragedBufferAdd(pTarget, 400));

    /* Acquire average on buffer full */
    TEST_ASSERT_EQUAL(250, AveragedBufferAverage(pTarget, NULL));   /* NULL is acceptable for error code */
    TEST_ASSERT_EQUAL(250, AveragedBufferAverage(pTarget, &ErrorCode));
    TEST_ASSERT_EQUAL(AVERAGED_BUFFER_ERROR_NONE, ErrorCode);

    /* Add value to the buffer when full */
    TEST_ASSERT_EQUAL(AVERAGED_BUFFER_ERROR_NONE, AveragedBufferAdd(pTarget, 800));
    TEST_ASSERT_EQUAL(425, AveragedBufferAverage(pTarget, &ErrorCode));
    TEST_ASSERT_EQUAL(AVERAGED_BUFFER_ERROR_NONE, ErrorCode);

    /* Clear the buffer */
    TEST_ASSERT_EQUAL(AVERAGED_BUFFER_ERROR_NONE, AveragedBufferClear(pTarget));
    TEST_ASSERT_EQUAL(0, AveragedBufferAverage(pTarget, &ErrorCode));
    TEST_ASSERT_EQUAL(AVERAGED_BUFFER_ERROR_NONE, ErrorCode);
}
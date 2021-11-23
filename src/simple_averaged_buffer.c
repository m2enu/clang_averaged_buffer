/**
 * @file    simple_averaged_buffer.c
 * @brief   Implementation for Simple Averaged Buffer
 * @author  m2enu
 * @par     License
 * https://github.com/m2enu/clang_averaged_buffer/blob/main/LICENSE
 */
#include "iaveraged_buffer.h"
#include <stdlib.h>
#include <stddef.h>

/** Clear the Simple Averaged Buffer
 * @param   pThis   Pointer to the instance of Simple Averaged Buffer
 * @return  Error code
 * @retval  AVERAGED_BUFFER_ERROR_NONE          Success
 * @retval  AVERAGED_BUFFER_ERROR_PARAMETER     pThis is invalid
 */
static AveragedBufferError Clear(IAveragedBuffer* pThis);

/** Add new data into the buffer
 * @param   pThis   Pointer to the instance of Simple Averaged Buffer
 * @return  Error code
 * @retval  AVERAGED_BUFFER_ERROR_NONE          Success
 * @retval  AVERAGED_BUFFER_ERROR_PARAMETER     pThis is invalid
 */
static AveragedBufferError Add(IAveragedBuffer* pThis, AveragedBufferType Value);

/** Acquire average of the Simple Averaged Buffer
 * @param   pThis   Pointer to the instance of Simple Averaged Buffer
 * @param   pError  Store the error code of this API (NULL is acceptable)
 * @return  Calculated average
 */
static AveragedBufferType Average(const IAveragedBuffer* pThis, AveragedBufferError* pError);

/** Implementation of the Averaged Buffer interface */
static const AveragedBufferMethods INTERFACE_METHODS = {
    &Clear,
    &Add,
    &Average
};

/** Create the instance of Simple Averaged Buffer
 * @param   pConfig     Pointer to configuration of Averaged Buffer
 * @return  Pointer to the instance of Simple Averaged Buffer
 * @return  NULL        Creation failed
 * @attention   This API is not allowed to call from except `iaveraged_buffer.c`.
 */
IAveragedBuffer* SimpleAveragedBufferCreate(const AveragedBufferConfig* pConfig)
{
    if (   (pConfig == NULL)
        || (AveragedBufferIsInvalidAlgorithm(pConfig->Algorithm))
        || (AveragedBufferIsInvalidLength(pConfig->Length)) )
    {
        return NULL;
    }

    IAveragedBuffer* pRet = AVERAGED_BUFFER_ALLOC(sizeof(IAveragedBuffer));
    if (AVERAGED_BUFFER_ALLOC_FAILED(pRet))
    {
        return NULL;
    }

    pRet->pBuffer = AVERAGED_BUFFER_ALLOC(pConfig->Length);
    if (AVERAGED_BUFFER_ALLOC_FAILED(pRet->pBuffer))
    {
        AVERAGED_BUFFER_FREE(pRet);
        return NULL;
    }

    pRet->pConfig = pConfig;
    pRet->pMethod = &INTERFACE_METHODS;
    pRet->Count = 0;
    return pRet;
}

static AveragedBufferError Clear(IAveragedBuffer* pThis)
{
    if (AveragedBufferIsInvalidInstance(pThis))
    {
        return AVERAGED_BUFFER_ERROR_PARAMETER;
    }
    size_t i;
    for (i = 0; i < pThis->pConfig->Length; i++)
    {
        *(pThis->pBuffer + i) = 0;
    }
    pThis->Count = 0;
    return AVERAGED_BUFFER_ERROR_NONE;
}

static AveragedBufferError Add(IAveragedBuffer* pThis, AveragedBufferType Value)
{
    if (AveragedBufferIsInvalidInstance(pThis))
    {
        return AVERAGED_BUFFER_ERROR_PARAMETER;
    }
    size_t index = pThis->Count % pThis->pConfig->Length;
    *(pThis->pBuffer + index) = Value;
    pThis->Count++;     /* TODO: consider overflow */
    return AVERAGED_BUFFER_ERROR_NONE;
}

static AveragedBufferType Average(const IAveragedBuffer* pThis, AveragedBufferError* pError)
{
    if (AveragedBufferIsInvalidInstance(pThis))
    {
        if (pError != NULL)
        {
            *pError = AVERAGED_BUFFER_ERROR_PARAMETER;
        }
        return 0;
    }

    const size_t count = (pThis->Count >= pThis->pConfig->Length) ?  pThis->pConfig->Length: pThis->Count;
    size_t i;
    AveragedBufferType average = 0;
    for (i = 0; i < count; i++)
    {
        average += (*(pThis->pBuffer + i) / count);
    }
    if (pError != NULL)
    {
        *pError = AVERAGED_BUFFER_ERROR_NONE;
    }
    return average;
}

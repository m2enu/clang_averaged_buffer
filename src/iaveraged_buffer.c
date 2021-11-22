/**
 * @file    iaveraged_buffer.c
 * @brief   Interface of averaged buffer
 * @author  m2enu
 * @par     License
 * https://github.com/m2enu/clang_averaged_buffer/blob/main/LICENSE
 */
#include "iaveraged_buffer.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/** Maximum length of the Averaged Buffer */
#define AVERAGED_BUFFER_LENGTH_MAX      (4096u)

/** Definition of the method to create the instance that has `IAveragedBuffer` instance. */
typedef IAveragedBuffer*    (*AveragedBufferCreationMethod)(const AveragedBufferConfig*);

/** Declaration of API to create the instance that has `IAveragedBuffer` interface.
 * @attention   Some static analysis tools will warn this prototype declaration.
 *              This software ignore this warning to give the priority to hide this API
 *              from outside of Averaged Buffer class.
 */
extern IAveragedBuffer* SimpleAveragedBufferCreate(const AveragedBufferConfig* pConfig);

/** Array of creation method
 * @attention   The order of this array must be matched as the order of
 *              `AveragedBufferAlgorithm` enumeration.
 */
static const AveragedBufferCreationMethod CREATION_METHODS[] = {
    &SimpleAveragedBufferCreate     /* Simple averaging */
};

IAveragedBuffer* AveragedBufferCreate(const AveragedBufferConfig* pConfig)
{
    if (   (pConfig == NULL)
        || (AveragedBufferIsInvalidAlgorithm(pConfig->Algorithm))
        || (AveragedBufferIsInvalidLength(pConfig->Length)) )
    {
        return NULL;
    }

    const AveragedBufferCreationMethod create = CREATION_METHODS[pConfig->Algorithm];
    IAveragedBuffer* pRet = create(pConfig);
    return pRet;
}

AveragedBufferError AveragedBufferClear(IAveragedBuffer* pThis)
{
    if ((pThis == NULL) || (pThis->pMethod == NULL) || (pThis->pMethod->Clear == NULL))
    {
        return AVERAGED_BUFFER_ERROR_PARAMETER;
    }
    return pThis->pMethod->Clear(pThis);
}

AveragedBufferError AveragedBufferAdd(IAveragedBuffer* pThis, AveragedBufferType Value)
{
    if ((pThis == NULL) || (pThis->pMethod == NULL) || (pThis->pMethod->Add == NULL))
    {
        return AVERAGED_BUFFER_ERROR_PARAMETER;
    }
    return pThis->pMethod->Add(pThis, Value);
}

AveragedBufferType AveragedBufferAverage(const IAveragedBuffer* pThis, AveragedBufferError* pError)
{
    if ((pThis == NULL) || (pThis->pMethod == NULL) || (pThis->pMethod->Average == NULL))
    {
        return AVERAGED_BUFFER_ERROR_PARAMETER;
    }
    return pThis->pMethod->Average(pThis, pError);
}

bool AveragedBufferIsInvalidInstance(const IAveragedBuffer* pThis)
{
    return ((pThis == NULL) || (pThis->pConfig == NULL) || (pThis->pBuffer == NULL));
}

bool AveragedBufferIsInvalidAlgorithm(AveragedBufferAlgorithm Algorithm)
{
    return ((Algorithm < 0) || (Algorithm >= AVERAGED_BUFFER_ALGO_MAX));
}

bool AveragedBufferIsInvalidLength(size_t Length)
{
    return (Length >= AVERAGED_BUFFER_LENGTH_MAX);
}

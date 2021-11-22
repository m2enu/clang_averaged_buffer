/**
 * @file    iaveraged_buffer.h
 * @brief   Interface of averaged buffer
 * @author  m2enu
 * @par     License
 * https://github.com/m2enu/clang_averaged_buffer/blob/main/LICENSE
 */
#ifndef IAVERAGED_BUFFER_H_INCLUDE
#define IAVERAGED_BUFFER_H_INCLUDE

/* includes */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* definitions */
#ifndef AVERAGED_BUFFER_ALLOC
#define AVERAGED_BUFFER_ALLOC               malloc  /**< Macro to allocate memory for buffer */
#endif

#ifndef AVERAGED_BUFFER_ALLOC_FAILED
#define AVERAGED_BUFFER_ALLOC_FAILED(x)     ((x) == NULL)   /**< Macro to validate the allocation is failed */
#endif

#ifndef AVERAGED_BUFFER_FREE
#define AVERAGED_BUFFER_FREE                free    /**< Macro to purge memory of allocated buffer */
#endif

/* forward declarations */
struct AveragedBufferMethods;

/* type definitions */
typedef int32_t     AveragedBufferType;     /**< Data type definition of averaged buffer */

/** Error code of Averaged Buffer APIs */
typedef enum AveragedBufferError
{
    AVERAGED_BUFFER_ERROR_NONE          = 0,    /**< No Error */
    AVERAGED_BUFFER_ERROR_PARAMETER     = -1,   /**< Invalid parameter */
} AveragedBufferError;

/** Enumeration of Averaged Buffer Algorithm */
typedef enum AveragedBufferAlgorithm
{
    AVERAGED_BUFFER_ALGO_SIMPLE,        /**< Simple averaging (it can be used as moving averaging also) */
    AVERAGED_BUFFER_ALGO_WEIGHTED,      /**< Weighted averaging */
    AVERAGED_BUFFER_ALGO_MAX,           /**< Algorithm enumeration max, it must be located on the last */
} AveragedBufferAlgorithm;

/** Configuration of Averaged Buffer Instance */
typedef struct AveragedBufferConfig
{
    const AveragedBufferAlgorithm   Algorithm;  /**< Averaging algorithm */
    const size_t                    Length;     /**< Length of the buffer */
    const void*                     Parameter;  /**< Averaging parameter: some algorithm never use this */
} AveragedBufferConfig;

/** Interface defition of Averaged Buffer */
typedef struct IAveragedBuffer
{
    const struct AveragedBufferMethods*     pMethod;    /**< Pointer to the interface methods */
    const AveragedBufferConfig*             pConfig;    /**< Pointer to the configuration */
          size_t                            Count;      /**< Current valid data count in the buffer */
          AveragedBufferType*               pBuffer;    /**< Pointer to the head of data buffer to be averaged */
} IAveragedBuffer;

/** Interface methods of Averaged Buffer */
typedef struct AveragedBufferMethods
{
    AveragedBufferError     (*Clear)(IAveragedBuffer*);                                 /**< Clear the buffer */
    AveragedBufferError     (*Add)(IAveragedBuffer*, AveragedBufferType);               /**< Add new data into the buffer */
    AveragedBufferType      (*Average)(const IAveragedBuffer*, AveragedBufferError*);   /**< Returns the average */
} AveragedBufferMethods;

/* function declarations */
/** Create new instance of Averaged Buffer as per specified configuration
 * @param       pConfig     Pointer to the configuration
 * @return      Pointer to the instance
 * @retval      NULL        Creation failed
 * @attention   Instance that has `IAveragedBuffer` interface must be created via this API.
 */
IAveragedBuffer* AveragedBufferCreate(const AveragedBufferConfig* pConfig);

/** Clear the buffer
 * @param   pThis       Instance of Averaged Buffer
 * @return  Error code
 * @retval  AVERAGED_BUFFER_ERROR_NONE          Success
 * @retval  AVERAGED_BUFFER_ERROR_PARAMETER     pThis is invalid
 */
AveragedBufferError AveragedBufferClear(IAveragedBuffer* pThis);

/** Add new data into the buffer
 * @param   pThis       Instance of Averaged Buffer
 * @param   Value       Value to be added into the Averaged Buffer
 * @return  Error code
 * @retval  AVERAGED_BUFFER_ERROR_NONE          Success
 * @retval  AVERAGED_BUFFER_ERROR_PARAMETER     pThis is invalid
 */
AveragedBufferError AveragedBufferAdd(IAveragedBuffer* pThis, AveragedBufferType Value);

/** Acquire average of the Averaged Buffer
 * @param   pThis       Instance of Averaged Buffer
 * @param   pError      Store the error code of this API (NULL is acceptable)
 * @return  Calculated average
 * @note    Able to specify `NULL` for `pError`.
 *          - If `pError` is not `NULL`, following error code will be stored as the result of this API.
 *          - AVERAGED_BUFFER_ERROR_NONE: Success
 *          - AVERAGED_BUFFER_ERROR_PARAMETER: pThis is invalid
 *          - If `pError` is `NULL`, this API will run properly but above error code doesn't stored.
 */
AveragedBufferType AveragedBufferAverage(const IAveragedBuffer* pThis, AveragedBufferError* pError);

/** Returns true if specified Averaged Buffer instance is invalid
 * @param   pThis       Pointer to the instance of Averaged Buffer
 * @return  Specified instance is invalid or not
 * @retval  true        Invalid
 * @retval  false       Valid
 */
bool AveragedBufferIsInvalidInstance(const IAveragedBuffer* pThis);

/** Returns true if specified algorithm enumeration is invalid
 * @param   Algorithm   Enumeration of Averaged Buffer algorithm
 * @return  Specified algorithm enumeration is invalid or not
 * @retval  true        Invalid
 * @retval  false       Valid
 */
bool AveragedBufferIsInvalidAlgorithm(AveragedBufferAlgorithm Algorithm);

/** Returns true if specified length is invalid
 * @param   Length      Length of Averaged Buffer
 * @return  Specified length is invalid or not
 * @retval  true        Invalid
 * @retval  false       Valid
 */
bool AveragedBufferIsInvalidLength(size_t Length);

#endif
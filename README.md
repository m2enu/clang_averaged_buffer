# Averaged Buffer

- `Averaged Buffer` is an wrapper to handle an array and serve an averaged value of the array by some algorithm (e.g. simple averaging, weighted averaging).
- Able to use via an OOP-like interface in C language.

# Features

- Store `int32_t` values into the buffer and serve its averaged value.
- Able to handle via the same interface for different averaging algorithm. So you only need modification for configuration of the buffer, need NO modification for your application.

# Requirements

- C99 or above
- Memory allocation methods (e.g. malloc / free via stdlib.h. Your own implemented memory allocator is also available.)

# Author / License

- m2enu
- This software is under [MIT License](https://github.com/m2enu/clang_averaged_buffer/blob/main/LICENS)

# Installation

- Clone this repository and place source files in your source code directory.

# Usage

- `Averaged Buffer` has an OOP-like interface: it must implement `IAveragedBuffer` interface.
- The APIs of `IAveragedBuffer` interface must pass the pointer of itself as the 1st argument like below.

```c
IAveragedBuffer* pThis = AveragedBufferCreate(...);
AveragedBufferAdd(pThis, 100);  /* Add new value: pass `IAveragedBuffer*` to 1st argument */
AveragedBufferType value = AveragedBufferAverage(pThis);    /* Acquire the average: pass `IAveragedBuffer*` to 1st argument */
```

## Create the instance

- The instance of the buffer must be created via `AveragedBufferCreate()` API with the configuration.
- As shown in following example,

    * The configuration is vary for different instance.
    * But the API to create the instance is same.

```c
/**
 * @brief   Configuration of the Averaged Buffer
 * @attention   The entity of configuration must be placed in the static area. The buffer might cause an unexpected behavior if configuration is placed in the stack area.
 */
static const AveragedBufferConfig configForSimple = {
    AVERAGED_BUFFER_ALGO_SIMPLE,    /* Averaging algorithm */
    128,    /* Length of the buffer */
    NULL    /* Parameter for averaging algorithm */
};

/**
 * @brief   Entity of simple averaging buffer
 */
static IAveragedBuffer* pSimple = AveragedBufferCreate(&configForSimple);

/**
 * @brief   Entity of weighted averaging buffer
 */
static IAveragedBuffer* pWeighted;

void func_initialize(void)
{
    /** Configuration for weighted averaging buffer
     * @attention   The buffer might cause an unexpected behavior if you remove `static` at next line.
     */
    static const AveragedBufferConfig configForWeighted = {
        AVERAGED_BUFFER_ALGO_WEIGHTED,
        10,
        &weightedConfig
    };
    /* Create the instance of weighted averaging buffer */
    pWeighted = AveragedBufferCreate(&configForWeighted);
}
```

## Configuration

- Configuration is provided via `AveragedBufferConfig` structure.
- It has following parameters.

| Parameter name    | Type                          | Description                               |
|-------------------|-------------------------------|-------------------------------------------|
| Algorithm         | enum AveragedBufferAlgorithm  | Averaging algorithm                       |
| Length            | size_t                        | Length of the buffer                      |
| Parameter         | void*                         | Averaging parameters for some algorithm   |

### Averaging parameter

#### For simple averaging buffer

- There is no averaging parameters.
- Hence `Parameter` must be specified as `NULL`.

#### For weighted averaging buffer

- Not implemented yet for current version.

## Adding the value

- The value that has `AveragedBufferType` type will be added via `AveragedBufferAdd()` API.

```c
void func_add(AveragedBufferType Value)
{
    /* Add value to each Averaged Buffers */
    AveragedBufferAdd(pSimple, Value);
    AveragedBufferAdd(pWeighted, Value);
}
```

## Acquire the average

- Able to acquire the average via `AverageBufferAverage()` API.
- 2nd argument is an option: pass NOT NULL pointer if you need the error code of this API.

```c
void func_average(void)
{
    /** Acquire the average of simple averaging buffer.
     * The error code of this API will be disposed because
     * 2nd argument is NULL.
     */
    AveragedBufferType value1 = AveragedBufferAverage(pSimple, NULL);
    /** Acquire the average of weighted averaging buffer.
     * The error code of this API will be stored into `ErrorCode`.
     */
    AveragedBufferError ErrorCode;
    AveragedBufferType value2 = AveragedBufferAverage(pWeighted, &ErrorCode);
}
```

# TODO

- [ ] Implement weighted averaging algorithm
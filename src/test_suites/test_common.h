#ifndef __TEST_COMMON_H__
#define __TEST_COMMON_H__
#include <stdint.h>
#include <stddef.h>
#include "board.h"

#define TEST_MATRIX_SZ (3)
#define TEST_MATRIX_COUNT (TEST_MATRIX_SZ*TEST_MATRIX_SZ)

typedef struct _timing_result {
    float dense_cycle_count_avg;
    uint32_t dense_cycle_count_max;
    uint32_t dense_cycle_count_min;

    float sparse_cycle_count_avg;
    uint32_t sparse_cycle_count_max;
    uint32_t sparse_cycle_count_min;
} TimingResult_t;

void TEST_create_random_data_f32(float* data, size_t len);

void TEST_create_random_sparse_f32(size_t nz_count, float* data, size_t len);

void TEST_create_random_data_q15(int16_t* data, size_t len);

void TEST_create_random_sparse_q15(size_t nz_count, int16_t* data, size_t len);

void DWT_Init() ;

void DWT_DeInit();

static inline uint32_t DWT_GetCycles()
{
    return DWT->CYCCNT;
}
#endif // !__TEST_COMMON_H__
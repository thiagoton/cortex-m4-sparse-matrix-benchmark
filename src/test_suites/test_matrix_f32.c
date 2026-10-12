#include <arm_math.h>
#include <stdio.h>
#include <fsl_rnga.h>
#include "test_common.h"

arm_matrix_instance_f32 A;
arm_matrix_instance_f32 B;

float A_data[TEST_MATRIX_COUNT];
float B_data[TEST_MATRIX_COUNT];
float C_data[TEST_MATRIX_COUNT];

int run_test_matrix_f32(uint16_t rounds, size_t nz_count, TimingResult_t *results)
{
    /**
     * Dense matrix multiplication
     */
    arm_matrix_instance_f32 C;

    arm_mat_init_f32(&A, TEST_MATRIX_SZ, TEST_MATRIX_SZ, A_data);
    arm_mat_init_f32(&B, TEST_MATRIX_SZ, TEST_MATRIX_SZ, B_data);
    arm_mat_init_f32(&C, TEST_MATRIX_SZ, TEST_MATRIX_SZ, C_data);

    results->dense_cycle_count_avg = 0.f;
    results->dense_cycle_count_max = 0.f;
    results->dense_cycle_count_min = infinityf();

    for (uint16_t n = 0u; n < rounds; ++n)
    {
        TEST_create_random_data_f32(A_data, TEST_MATRIX_COUNT);
        TEST_create_random_sparse_f32(nz_count, B_data, TEST_MATRIX_COUNT);

        DWT_Init();
        uint32_t elapsed_cycles = DWT_GetCycles();
        int status = arm_mat_mult_f32(&A, &B, &C);
        elapsed_cycles = DWT_GetCycles() - elapsed_cycles;
        DWT_DeInit();

        if (status == ARM_MATH_SUCCESS)
        {
            results->dense_cycle_count_avg += elapsed_cycles;
            results->dense_cycle_count_max = MAX(results->dense_cycle_count_max, elapsed_cycles);
            results->dense_cycle_count_min = MIN(results->dense_cycle_count_min, elapsed_cycles);
        }
        else
        {
            return -1;
        }
    }

    results->dense_cycle_count_avg = results->dense_cycle_count_avg / rounds;
    return 0;
}

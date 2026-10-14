#include <arm_math.h>
#include <stdio.h>
#include <fsl_rnga.h>
#include "test_common.h"
#include "sparse_matrix.h"

arm_matrix_instance_f32 A;
arm_matrix_instance_f32 B;
arm_matrix_instance_f32 C;
arm_matrix_instance_f32 D;

float A_data[TEST_MATRIX_COUNT]; //= {1.,2.,3., 4.,5.,6.,  7.,8.,9.};
float B_data[TEST_MATRIX_COUNT]; //= {1.,0.,0., 0.,0.,-1., 0.,.5,0.};
float C_data[TEST_MATRIX_COUNT]; // will hold reference result
float D_data[TEST_MATRIX_COUNT]; // will hold values to be compared to reference

/**
 * Sparse Matrix allocated arrays
 */
sparse_mat_instance_f32_t Bs;
float Bs_data[TEST_MATRIX_COUNT];
uint16_t Bs_col_ptrs[TEST_MATRIX_SZ + 1];
uint16_t Bs_rows[TEST_MATRIX_COUNT];

extern void naive_mat_mult_f32(arm_matrix_instance_f32 *A, arm_matrix_instance_f32 *B, arm_matrix_instance_f32 *out);

int run_test_matrix_f32(uint16_t rounds, size_t nz_count, TimingResult_t *results)
{
    /**
     * Dense matrix multiplication
     */
    assert(nz_count <= TEST_MATRIX_COUNT);

    arm_mat_init_f32(&A, TEST_MATRIX_SZ, TEST_MATRIX_SZ, A_data);
    arm_mat_init_f32(&B, TEST_MATRIX_SZ, TEST_MATRIX_SZ, B_data);
    arm_mat_init_f32(&C, TEST_MATRIX_SZ, TEST_MATRIX_SZ, C_data);
    arm_mat_init_f32(&D, TEST_MATRIX_SZ, TEST_MATRIX_SZ, D_data);

    results->arm_cycle_count_avg = 0.f;
    results->arm_cycle_count_max = 0u;
    results->arm_cycle_count_min = infinityf();

    results->sparse_cycle_count_avg = 0.f;
    results->sparse_cycle_count_max = 0u;
    results->sparse_cycle_count_min = infinityf();

    results->naive_cycle_count_avg = 0.f;
    results->naive_cycle_count_max = 0u;
    results->naive_cycle_count_min = infinityf();

    for (uint16_t n = 0u; n < rounds; ++n)
    {
        uint32_t elapsed_cycles;
        int status;
        TEST_create_random_data_f32(A_data, TEST_MATRIX_COUNT);
        TEST_create_random_sparse_f32(nz_count, B_data, TEST_MATRIX_COUNT);

        /**
         * Naive
         */
        TEST_MEASURE_CYCLES(elapsed_cycles, naive_mat_mult_f32(&A, &B, &C));

        results->naive_cycle_count_avg += elapsed_cycles;
        results->naive_cycle_count_max = MAX(results->naive_cycle_count_max, elapsed_cycles);
        results->naive_cycle_count_min = MIN(results->naive_cycle_count_min, elapsed_cycles);

        /**
         * CMSIS
         */
        TEST_MEASURE_CYCLES(elapsed_cycles, status = arm_mat_mult_f32(&A, &B, &D));

        // compare CMSIS x naive results
        for (uint16_t k = 0u; k < TEST_MATRIX_COUNT; ++k)
        {
            assert(fabsf(C_data[k] - D_data[k]) < 1e-6);
        }

        if (status == ARM_MATH_SUCCESS)
        {
            results->arm_cycle_count_avg += elapsed_cycles;
            results->arm_cycle_count_max = MAX(results->arm_cycle_count_max, elapsed_cycles);
            results->arm_cycle_count_min = MIN(results->arm_cycle_count_min, elapsed_cycles);
        }
        else
        {
            return -1;
        }

        /**
         * Sparse
         */
        sparse_mat_init_f32(&Bs, &B, Bs_rows, Bs_col_ptrs, Bs_data);
        TEST_MEASURE_CYCLES(elapsed_cycles, sparse_mat_mult_f32(&A, &Bs, &D));

        results->sparse_cycle_count_avg += elapsed_cycles;
        results->sparse_cycle_count_max = MAX(results->sparse_cycle_count_max, elapsed_cycles);
        results->sparse_cycle_count_min = MIN(results->sparse_cycle_count_min, elapsed_cycles);

        // compare sparse x naive results
        for (uint16_t k = 0u; k < TEST_MATRIX_COUNT; ++k)
        {
            assert(fabsf(C_data[k] - D_data[k]) < 1e-6);
        }
    }

    results->arm_cycle_count_avg = results->arm_cycle_count_avg / rounds;
    results->sparse_cycle_count_avg = results->sparse_cycle_count_avg / rounds;
    results->naive_cycle_count_avg = results->naive_cycle_count_avg / rounds;

    return 0;
}

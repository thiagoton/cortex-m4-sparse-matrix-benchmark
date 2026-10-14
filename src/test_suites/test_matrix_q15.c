#include <arm_math.h>
#include <stdio.h>
#include <fsl_rnga.h>
#include "test_common.h"
#include "sparse_matrix.h"

arm_matrix_instance_q15 A;
arm_matrix_instance_q15 B;
arm_matrix_instance_q15 Bt;
arm_matrix_instance_q15 C; // reference matrix
arm_matrix_instance_q15 D; // matrix to be compared with reference

int16_t A_data[TEST_MATRIX_COUNT];  //= {1.,2.,3., 4.,5.,6.,  7.,8.,9.};
int16_t B_data[TEST_MATRIX_COUNT];  //= {1.,0.,0., 0.,0.,-1., 0.,.5,0.};
int16_t Bt_data[TEST_MATRIX_COUNT]; //= {1.,0.,0., 0.,0.,-1., 0.,.5,0.};
int16_t C_data[TEST_MATRIX_COUNT];
int16_t D_data[TEST_MATRIX_COUNT];

/**
 * Sparse matrix arrays
 */
sparse_mat_instance_q15_t Bs;
int16_t Bs_data[TEST_MATRIX_COUNT];
uint16_t Bs_col_ptrs[TEST_MATRIX_SZ + 1];
uint16_t Bs_rows[TEST_MATRIX_COUNT];

extern arm_status arm_mat_mult_fast_q15_2(const arm_matrix_instance_q15 *pSrcA,
                                          const arm_matrix_instance_q15 *pSrcB,
                                          arm_matrix_instance_q15 *pDst,
                                          q15_t *pState);

extern void naive_mat_mult_q15(arm_matrix_instance_q15 *A, arm_matrix_instance_q15 *B, arm_matrix_instance_q15 *out);

int run_test_matrix_q15(uint16_t rounds, size_t nz_count, TimingResult_t *results)
{
    /**
     * Dense matrix multiplication
     */
    assert(nz_count <= TEST_MATRIX_COUNT);

    arm_mat_init_q15(&A, TEST_MATRIX_SZ, TEST_MATRIX_SZ, A_data);
    arm_mat_init_q15(&B, TEST_MATRIX_SZ, TEST_MATRIX_SZ, B_data);
    arm_mat_init_q15(&Bt, TEST_MATRIX_SZ, TEST_MATRIX_SZ, Bt_data);
    arm_mat_init_q15(&C, TEST_MATRIX_SZ, TEST_MATRIX_SZ, C_data);

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
        TEST_create_random_data_q15(A_data, TEST_MATRIX_COUNT);
        TEST_create_random_sparse_q15(nz_count, B_data, TEST_MATRIX_COUNT);

        /**
         * Naive
         */
        TEST_MEASURE_CYCLES(elapsed_cycles, naive_mat_mult_q15(&A, &B, &C));

        results->naive_cycle_count_avg += elapsed_cycles;
        results->naive_cycle_count_max = MAX(results->naive_cycle_count_max, elapsed_cycles);
        results->naive_cycle_count_min = MIN(results->naive_cycle_count_min, elapsed_cycles);

        /**
         * CMSIS
         */
        // transpose matrix, because custom CMSIS multiplication is not doing that anymore.
        arm_mat_trans_q15(&B, &Bt);
        TEST_MEASURE_CYCLES(elapsed_cycles, status = arm_mat_mult_fast_q15_2(&A, &Bt, &D, NULL));

        // compare CMSIS x naive results
        for (uint16_t k = 0u; k < TEST_MATRIX_COUNT; ++k)
        {
            assert(C_data[k] == D_data[k]);
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
        sparse_mat_init_q15(&Bs, &B, Bs_rows, Bs_col_ptrs, Bs_data);
        TEST_MEASURE_CYCLES(elapsed_cycles, sparse_mat_mult_q15(&A, &Bs, &D));

        results->sparse_cycle_count_avg += elapsed_cycles;
        results->sparse_cycle_count_max = MAX(results->sparse_cycle_count_max, elapsed_cycles);
        results->sparse_cycle_count_min = MIN(results->sparse_cycle_count_min, elapsed_cycles);

        for (uint16_t k = 0u; k < TEST_MATRIX_COUNT; ++k)
        {
            assert(C_data[k] == D_data[k]);
        }
    }

    results->arm_cycle_count_avg = results->arm_cycle_count_avg / rounds;
    results->sparse_cycle_count_avg = results->sparse_cycle_count_avg / rounds;
    results->naive_cycle_count_avg = results->naive_cycle_count_avg / rounds;

    return 0;
}

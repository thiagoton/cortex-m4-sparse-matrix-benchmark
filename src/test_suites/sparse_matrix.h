#ifndef __SPARSE_MATRIX_H__
#define __SPARSE_MATRIX_H__
#include <stdint.h>
#include <arm_math.h>

typedef struct __sm_smatrix_csc_f
{
    float *data;
    uint16_t *rows;
    uint16_t rows_sz;
    uint16_t *col_ptrs;
    uint16_t col_ptrs_sz;
} sparse_mat_instance_f32_t;

typedef struct __sm_smatrix_csc_q15
{
    int16_t *data;
    uint16_t *rows;
    uint16_t rows_sz;
    uint16_t *col_ptrs;
    uint16_t col_ptrs_sz;
} sparse_mat_instance_q15_t;

void sparse_mat_init_f32(sparse_mat_instance_f32_t *smatrix, arm_matrix_instance_f32 *dense, uint16_t *rows, uint16_t *col_ptrs, float *data);

int sparse_mat_mult_f32(arm_matrix_instance_f32 *dense, sparse_mat_instance_f32_t *smatrix, arm_matrix_instance_f32 *out);

void sparse_mat_init_q15(sparse_mat_instance_q15_t *smatrix, arm_matrix_instance_q15 *dense, uint16_t *rows, uint16_t *col_ptrs, int16_t *data);

int sparse_mat_mult_q15(arm_matrix_instance_q15 *dense, sparse_mat_instance_q15_t *smatrix, arm_matrix_instance_q15 *out);

#endif // !__SPARSE_MATRIX_H__
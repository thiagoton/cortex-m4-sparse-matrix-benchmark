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
} SM_SMatrixCscF_t;

typedef struct __sm_smatrix_csc_q15
{
    int16_t *data;
    uint16_t *rows;
    uint16_t rows_sz;
    uint16_t *col_ptrs;
    uint16_t col_ptrs_sz;
} SM_SMatrixCscQ15_t;

void SM_CscInit(SM_SMatrixCscF_t *smatrix, arm_matrix_instance_f32 *dense, uint16_t *rows, uint16_t *col_ptrs, float *data);

int SM_Mult_DenseXSparse(arm_matrix_instance_f32 *dense, SM_SMatrixCscF_t *smatrix, arm_matrix_instance_f32 *out);

void SM_CscInitQ15(SM_SMatrixCscQ15_t *smatrix, arm_matrix_instance_q15 *dense, uint16_t *rows, uint16_t *col_ptrs, int16_t *data);

int SM_Mult_DenseXSparseQ15(arm_matrix_instance_q15 *dense, SM_SMatrixCscQ15_t *smatrix, arm_matrix_instance_q15 *out);

#endif // !__SPARSE_MATRIX_H__
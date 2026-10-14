#include <arm_math.h>
#include <assert.h>

void naive_mat_mult_q15(arm_matrix_instance_q15 *A, arm_matrix_instance_q15 *B, arm_matrix_instance_q15 *out)
{
    assert(out->numCols == B->numCols);
    assert(out->numRows == A->numRows);
    assert(A->numCols == B->numRows);

    for (uint16_t col = 0u; col < B->numCols; ++col)
    {
        int16_t *pCol = B->pData + col;
        int16_t *pColStart = pCol;
        for (uint16_t row = 0u; row < A->numRows; ++row)
        {
            int64_t value = 0;
            int16_t *pRow = A->pData + row * A->numCols;
            for (uint16_t n = 0u; n < B->numRows; ++n)
            {
                value += (*pRow) * (*pCol);
                pRow += 1;
                pCol += B->numCols;
            }
            out->pData[col + row * (B->numCols)] = (__SSAT((value >> 15), 16));
            pCol = pColStart;
        }
    }
}

void naive_mat_mult_f32(arm_matrix_instance_f32 *A, arm_matrix_instance_f32 *B, arm_matrix_instance_f32 *out)
{
    assert(out->numCols == B->numCols);
    assert(out->numRows == A->numRows);
    assert(A->numCols == B->numRows);

    for (uint16_t col = 0u; col < B->numCols; ++col)
    {
        float *pCol = B->pData + col;
        float *pColStart = pCol;
        for (uint16_t row = 0u; row < A->numRows; ++row)
        {
            float value = 0.f;
            float *pRow = A->pData + row * A->numCols;
            for (uint16_t n = 0u; n < B->numRows; ++n)
            {
                value += (*pRow) * (*pCol);
                pRow += 1;
                pCol += B->numCols;
            }
            out->pData[col + row * (B->numCols)] = value;
            pCol = pColStart;
        }
    }
}
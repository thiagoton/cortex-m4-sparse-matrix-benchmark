#include "sparse_matrix.h"

void SM_CscInit(SM_SMatrixCscF_t *sparse, arm_matrix_instance_f32 *dense, uint16_t *rows, uint16_t *col_ptrs, float *data)
{
    uint16_t *sm_cur_col_ptrs = col_ptrs;
    float *sm_cur_data = data;
    uint16_t *sm_cur_row = rows;

    sparse->data = data;
    sparse->rows = rows;
    sparse->col_ptrs = col_ptrs;
    sparse->rows_sz = 0u;
    sparse->col_ptrs_sz = dense->numCols + 1u;

    // copy input dense data into output sparse data. Since this is a CSC sparse matrix, it needs to be iterated by row first
    *sm_cur_col_ptrs = 0u;
    sm_cur_col_ptrs += 1;
    for (uint16_t col = 0u; col < dense->numCols; ++col)
    {
        // when starting a new row, the col ptrs must be set to previous index, because there is not row data yet.
        *sm_cur_col_ptrs = *(sm_cur_col_ptrs - 1);
        for (uint16_t row = 0u; row < dense->numRows; ++row)
        {
            uint16_t idx = (row * dense->numCols) + col;
            if (dense->pData[idx] == 0.f)
            {
                continue;
            }

            // push data into sparsed matrix and update rows/col pointers
            *sm_cur_col_ptrs += 1u;           // update col_ptrs value to accommodate pushed row
            *sm_cur_data = dense->pData[idx]; // push new sparse matrix data
            sm_cur_data++;                    // update sparse matrix data pointer, so new data is pushed in the end
            *sm_cur_row = row;                // update row index corresponding to dense matrix coordinate
            sparse->rows_sz += 1;             // update row array size and insert position
            sm_cur_row++;
        }
        sm_cur_col_ptrs += 1; // finished row, move to next col_ptrs
    }
}

int SM_Mult_DenseXSparse(arm_matrix_instance_f32 *dense, SM_SMatrixCscF_t *sparse, arm_matrix_instance_f32 *out)
{
    float *out_row = out->pData;
    float *dense_row = dense->pData;
    for (uint16_t drow_idx = 0u; drow_idx < dense->numRows; drow_idx++)
    {
        uint16_t* sdata_idx_start = sparse->col_ptrs;
        uint16_t* sdata_idx_end = sparse->col_ptrs + 1u;
        for (uint16_t scol_ptr_idx = 0u; scol_ptr_idx < sparse->col_ptrs_sz - 1u; scol_ptr_idx++)
        {
            float out_value = 0.f;
            for (uint16_t sdata_idx = *sdata_idx_start; sdata_idx < *sdata_idx_end; sdata_idx++)
            {
                uint16_t srow_idx = sparse->rows[sdata_idx];
                out_value += dense_row[srow_idx] * sparse->data[sdata_idx];
            }
            out_row[scol_ptr_idx] = out_value;

            sdata_idx_start += 1u;
            sdata_idx_end += 1;
        }

        dense_row += (dense->numCols);
        out_row += (out->numCols);
    }

    return 0;
}

void SM_CscInitQ15(SM_SMatrixCscQ15_t *sparse, arm_matrix_instance_q15 *dense, uint16_t *rows, uint16_t *col_ptrs, int16_t *data)
{
    uint16_t *sm_cur_col_ptrs = col_ptrs;
    int16_t *sm_cur_data = data;
    uint16_t *sm_cur_row = rows;

    sparse->data = data;
    sparse->rows = rows;
    sparse->col_ptrs = col_ptrs;
    sparse->rows_sz = 0u;
    sparse->col_ptrs_sz = dense->numCols + 1u;

    // copy input dense data into output sparse data. Since this is a CSC sparse matrix, it needs to be iterated by row first
    *sm_cur_col_ptrs = 0u;
    sm_cur_col_ptrs += 1;
    for (uint16_t col = 0u; col < dense->numCols; ++col)
    {
        // when starting a new row, the col ptrs must be set to previous index, because there is not row data yet.
        *sm_cur_col_ptrs = *(sm_cur_col_ptrs - 1);
        for (uint16_t row = 0u; row < dense->numRows; ++row)
        {
            uint16_t idx = (row * dense->numCols) + col;
            if (dense->pData[idx] == 0)
            {
                continue;
            }

            // push data into sparsed matrix and update rows/col pointers
            *sm_cur_col_ptrs += 1u;           // update col_ptrs value to accommodate pushed row
            *sm_cur_data = dense->pData[idx]; // push new sparse matrix data
            sm_cur_data++;                    // update sparse matrix data pointer, so new data is pushed in the end
            *sm_cur_row = row;                // update row index corresponding to dense matrix coordinate
            sparse->rows_sz += 1;             // update row array size and insert position
            sm_cur_row++;
        }
        sm_cur_col_ptrs += 1; // finished row, move to next col_ptrs
    }
}

int SM_Mult_DenseXSparseQ15(arm_matrix_instance_q15 *dense, SM_SMatrixCscQ15_t *sparse, arm_matrix_instance_q15 *out)
{
    int16_t *out_row = out->pData;
    int16_t *dense_row = dense->pData;
    for (uint16_t drow_idx = 0u; drow_idx < dense->numRows; drow_idx++)
    {
        uint16_t* sdata_idx_start = sparse->col_ptrs;
        uint16_t* sdata_idx_end = sparse->col_ptrs + 1u;
        for (uint16_t scol_ptr_idx = 0u; scol_ptr_idx < sparse->col_ptrs_sz - 1u; scol_ptr_idx++)
        {
            q63_t out_value = 0;
            for (uint16_t sdata_idx = *sdata_idx_start; sdata_idx < *sdata_idx_end; sdata_idx++)
            {
                uint16_t srow_idx = sparse->rows[sdata_idx];
                out_value += ((int32_t)dense_row[srow_idx]) * ((int32_t)sparse->data[sdata_idx]);
            }
            out_row[scol_ptr_idx] = (__SSAT((out_value >> 15), 16));

            sdata_idx_start += 1u;
            sdata_idx_end += 1;
        }

        dense_row += (dense->numCols);
        out_row += (out->numCols);
    }

    return 0;
}
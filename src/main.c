/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_rnga.h"
#include "test_suites/test_common.h"
#include "test_suites/test_suites.h"

#define NMEMB(a) (sizeof(a)/sizeof(a[0]))

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    char ch;

    const uint8_t test_nz_elems[] = {1, 3, 5, 9};

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    RNGA_Init(RNG);
    RNGA_Seed(RNG, 1234u);

    TimingResult_t results;
    for (uint8_t n = 0u; n < NMEMB(test_nz_elems); ++n)
    {
        const uint8_t nz_elems = test_nz_elems[n];
        int status = run_test_matrix_f32(1000, nz_elems, &results);

        PRINTF("f32, SZ: %d, NZ: %d, ", TEST_MATRIX_SZ, (int)nz_elems);
        PRINTF("davg: %u,dmax: %u,dmin: %u, ", (uint32_t)(results.dense_cycle_count_avg), results.dense_cycle_count_max, results.dense_cycle_count_min);
        PRINTF("savg: %u, smax: %u, smin: %u\n", (uint32_t)(results.sparse_cycle_count_avg), results.sparse_cycle_count_max, results.sparse_cycle_count_min);
    }

    for (uint8_t n = 0u; n < NMEMB(test_nz_elems); ++n)
    {
        const uint8_t nz_elems = test_nz_elems[n];
        int status = run_test_matrix_q15(1000, nz_elems, &results);

        PRINTF("q15, SZ: %d, NZ: %d, ", TEST_MATRIX_SZ, (int)nz_elems);
        PRINTF("davg: %u,dmax: %u,dmin: %u, ", (uint32_t)(results.dense_cycle_count_avg), results.dense_cycle_count_max, results.dense_cycle_count_min);
        PRINTF("savg: %u, smax: %u, smin: %u\n", (uint32_t)(results.sparse_cycle_count_avg), results.sparse_cycle_count_max, results.sparse_cycle_count_min);
    }

    while (1)
    {
        // ch = GETCHAR();
        // PUTCHAR(ch);
    }
}

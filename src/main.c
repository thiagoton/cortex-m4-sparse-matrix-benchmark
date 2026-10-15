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

#define NMEMB(a) (sizeof(a) / sizeof(a[0]))
#define NUM_ROUNDS (500u)

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
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    RNGA_Init(RNG);
    RNGA_Seed(RNG, 1234u);

    uint8_t nz_step = 2u;
    if (TEST_MATRIX_SZ > 5)
    {
        nz_step = (uint8_t)(TEST_MATRIX_COUNT * 0.1);
    }

    TimingResult_t results;
    for (uint8_t nz_elems = 1u; nz_elems <= TEST_MATRIX_COUNT; nz_elems += nz_step)
    {
        int status = run_test_matrix_f32(NUM_ROUNDS, nz_elems, &results);
        assert(status == 0);

        PRINTF("f32, SZ: %d, NZ: %d, ", TEST_MATRIX_SZ, (int)nz_elems);
        PRINTF("navg: %u, nmax: %u, nmin: %u, ", (uint32_t)(results.naive_cycle_count_avg), results.naive_cycle_count_max, results.naive_cycle_count_min);
        PRINTF("davg: %u, dmax: %u, dmin: %u, ", (uint32_t)(results.arm_cycle_count_avg), results.arm_cycle_count_max, results.arm_cycle_count_min);
        PRINTF("savg: %u, smax: %u, smin: %u\n", (uint32_t)(results.sparse_cycle_count_avg), results.sparse_cycle_count_max, results.sparse_cycle_count_min);
    }

    for (uint8_t nz_elems = 1u; nz_elems <= TEST_MATRIX_COUNT; nz_elems += nz_step)
    {
        int status = run_test_matrix_q15(NUM_ROUNDS, nz_elems, &results);
        assert(status == 0);

        PRINTF("q15, SZ: %d, NZ: %d, ", TEST_MATRIX_SZ, (int)nz_elems);
        PRINTF("navg: %u, nmax: %u, nmin: %u, ", (uint32_t)(results.naive_cycle_count_avg), results.naive_cycle_count_max, results.naive_cycle_count_min);
        PRINTF("davg: %u, dmax: %u, dmin: %u, ", (uint32_t)(results.arm_cycle_count_avg), results.arm_cycle_count_max, results.arm_cycle_count_min);
        PRINTF("savg: %u, smax: %u, smin: %u\n", (uint32_t)(results.sparse_cycle_count_avg), results.sparse_cycle_count_max, results.sparse_cycle_count_min);
    }

    while (1)
    {
    }
}

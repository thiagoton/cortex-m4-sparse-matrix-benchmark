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

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    RNGA_Init(RNG);
    RNGA_Seed(RNG, 1234u);

    PRINTF("hello world.\r\n");

    TimingResult_t result_f32;
    int status = run_test_matrix_f32(10, 4, &result_f32);

    PRINTF("avg = %u\nmax = %u\nmin = %u\n", (uint32_t)(result_f32.dense_cycle_count_avg*1000u), result_f32.dense_cycle_count_max, result_f32.dense_cycle_count_min);

    while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }
}

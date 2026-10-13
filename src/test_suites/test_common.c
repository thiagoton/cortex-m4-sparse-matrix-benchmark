#include "test_common.h"
#include "board.h"
#include <fsl_rnga.h>

void TEST_create_random_data_f32(float *data, size_t len)
{
    while (len--)
    {
        int32_t rand;
        RNGA_GetRandomData(RNG, &rand, sizeof(rand));
        *data = (float)((rand % 100) / 10.f);
        data += 1;
    }
}

void TEST_create_random_sparse_f32(size_t nz_count, float *data, size_t len)
{
    for (size_t i = 0u; i < len; ++i)
    {
        *(data + i) = 0.f;
    }

    while (nz_count--)
    {
        uint16_t pos;
        int32_t value;
        RNGA_GetRandomData(RNG, &pos, sizeof(pos));
        pos = pos % len;

        while (*(data + pos) != 0)
        {
            pos = (pos + 1u) % len;
        }

        RNGA_GetRandomData(RNG, &value, sizeof(value));
        *(data + pos) = (float)((value % 100) / 10.f);
    }
}


void DWT_Init() 
{
    /* Make sure the DWT trace fucntion is enabled. */
    if (CoreDebug_DEMCR_TRCENA_Msk != (CoreDebug_DEMCR_TRCENA_Msk & CoreDebug->DEMCR))
    {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    }

    /* CYCCNT not supported on this device. */
    assert(DWT_CTRL_NOCYCCNT_Msk != (DWT->CTRL & DWT_CTRL_NOCYCCNT_Msk));
    DWT->CYCCNT = 0U;

    /* Read CYCCNT directly if CYCCENT has already been enabled, otherwise enable CYCCENT first. */
    if (DWT_CTRL_CYCCNTENA_Msk != (DWT_CTRL_CYCCNTENA_Msk & DWT->CTRL))
    {
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    }
}

void DWT_DeInit()
{
    CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
}


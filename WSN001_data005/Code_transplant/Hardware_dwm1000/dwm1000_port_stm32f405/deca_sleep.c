/*! ----------------------------------------------------------------------------
 * @file    deca_sleep.c
 * @brief   platform dependent sleep implementation
 *
 * @attention
 *
 * Copyright 2015 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author DecaWave
 */

//#include "port.h"

#include "deca_device_api.h"
#include "sleep.h"
#include "delay_tim4.h"//

/* Wrapper function to be used by decadriver. Declared in deca_device_api.h */
void deca_sleep(unsigned int time_ms)
{
    Timedelay_tim4_ms(time_ms);
}

void sleep_ms(unsigned int time_ms)
{
    Timedelay_tim4_ms(time_ms);
}

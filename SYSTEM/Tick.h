#ifndef __TICK_H__
#define __TICK_H__

#include <stdint.h>
#include "ti_msp_dl_config.h"
#include "./BTN/BTN.h"

#ifdef __cplusplus
extern "C" {
#endif
extern volatile uint32_t Tick;

void delay_ms(uint8_t ms);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef __TICK_H__ */
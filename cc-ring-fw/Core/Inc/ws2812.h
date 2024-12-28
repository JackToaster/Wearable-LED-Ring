#ifndef WS2812_H
#define WS2812_H
#include "main.h"

#define DELAY_SHORT 10
#define DELAY_LONG 20
#define DIMMING 16

void InitWS2812(void);

void SendWS2812(uint8_t* led_out);

#endif
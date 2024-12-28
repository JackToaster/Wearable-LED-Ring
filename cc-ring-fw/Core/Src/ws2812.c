#include "ws2812.h"
#include "embeddedout.h"

#define T0H 14
#define T0L 20
#define T1H 10
#define T1L 12

void __attribute__((optimize("O0"))) delay_0H(void) {
    __ASM volatile (
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0"
    );
}

void __attribute__((optimize("O0"))) delay_L(void) {
    
    __ASM volatile (
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0"
    );
}

void __attribute__((optimize("O0"))) delay_1H(void) {
    __ASM volatile (
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0\n\t"
        "mov     r0, r0"
    );
}

uint16_t dither_value = 0;

void InitWS2812(void) {
    HAL_GPIO_WritePin(led_dat_GPIO_Port, led_dat_Pin, 0);
}

// bit-banged WS2812 protocol
void __attribute__((optimize("O3"))) SendWS2812(uint8_t* lightarray) {
    dither_value = (dither_value + (DIMMING / 2) + 1) % DIMMING;

    for(uint8_t i = 0; i < NUM_LIN_LEDS; ++i) {

        uint8_t* color_base = &lightarray[i*3];
        
        uint32_t set_bitmask = led_dat_Pin;
        uint32_t reset_bitmask = (uint32_t)led_dat_Pin << 16;
        __disable_irq();
        delay_1H();
        delay_0H();
        delay_L();
        for(uint8_t color = 0; color < 3; ++color) {
            uint8_t color_channel = ((uint16_t)color_base[color] + dither_value) / DIMMING;
            for(uint8_t bit = 0; bit < 8; ++bit) {
                led_dat_GPIO_Port->BSRR = set_bitmask;
                if(color_channel & (0b10000000 >> bit)) {
                    delay_1H();
                    led_dat_GPIO_Port->BSRR = reset_bitmask;
                    delay_L();
                } else {
                    led_dat_GPIO_Port->BSRR = set_bitmask;
                    delay_0H();
                    led_dat_GPIO_Port->BSRR = reset_bitmask;
                    delay_L();
                }
            }
        }
        // required for four-byte per pixel LED's 
        // volatile uint8_t zero = 0;
        // for(uint8_t bit = 0; bit < 8; ++bit) {
        //     led_dat_GPIO_Port->BSRR = set_bitmask;
        //     if(zero & bit) {
        //         delay_1H();
        //         led_dat_GPIO_Port->BSRR = reset_bitmask;
        //         delay_L();
        //     } else {
        //         led_dat_GPIO_Port->BSRR = set_bitmask;
        //         delay_0H();
        //         led_dat_GPIO_Port->BSRR = reset_bitmask;
        //         delay_L();
        //     }
        // }
        __enable_irq();
    }

}

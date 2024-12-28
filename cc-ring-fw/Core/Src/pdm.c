#include "pdm.h"

#define DECIMATION_BYTES (DECIMATION / 8)

extern DMA_HandleTypeDef hdma_spi1_rx;
extern SPI_HandleTypeDef hspi1;

uint8_t spi_buffer[SPI_BUFFER_SIZE];
uint8_t buffer_half = 0;
uint32_t sample_counter = 0;


volatile uint32_t err_counter = 0;
float filtered_value = 0.0;

uint8_t ones_table[256];
uint8_t countones_byte(uint8_t i) {
    return ones_table[i];
}

void pdm_decode(uint8_t* buffer, uint32_t n_samples) {
    for(uint32_t sample_n = 0; sample_n < n_samples; sample_n += DECIMATION_BYTES) {
        int16_t sample = 0;
        for(uint32_t buff_byte = 0; buff_byte < DECIMATION_BYTES; buff_byte++) {
            uint8_t buff_value = buffer[sample_n + buff_byte];
            sample += (int16_t) countones_byte(buff_value);
        }
        // remove DC bias (center around 0)
        sample -= (DECIMATION / 2);
        sample *= PDM_VALUE_SCALE;
        
        filtered_value = ((float) sample + (filtered_value * (float) PDM_IIR)) / (float)(PDM_IIR + 1);

        if(filtered_value > 4095.0) { filtered_value = 4095.0; }
        if(filtered_value < -4095.0) { filtered_value = -4095.0; }

        GotSample((int16_t) filtered_value);
        sample_counter++;
        // TODO Any kind of filtering
    }
}


void rx_lower(SPI_HandleTypeDef *h) {
    pdm_decode(&spi_buffer[0], SPI_BUFFER_SIZE / 2);
}

void rx_upper(SPI_HandleTypeDef *h) {
    pdm_decode(&spi_buffer[SPI_BUFFER_SIZE / 2], SPI_BUFFER_SIZE / 2);
}


void pdm_start(void) {
    // fill ones table

    for(uint32_t i = 0; i < 256; ++i) {
        for(uint8_t bit = 0; bit < 8; ++bit){
            if(i & (1 << bit)) {
                ones_table[i]++;
            }
        }
    }

    HAL_StatusTypeDef stat = HAL_SPI_RegisterCallback(&hspi1, HAL_SPI_RX_HALF_COMPLETE_CB_ID, rx_lower);
    if(stat != HAL_OK) {
        err_counter++;
    }
    stat = HAL_SPI_RegisterCallback(&hspi1, HAL_SPI_RX_COMPLETE_CB_ID, rx_upper);
    if(stat != HAL_OK) {
        err_counter++;
    }
    stat = HAL_SPI_Receive_DMA(&hspi1, spi_buffer, SPI_BUFFER_SIZE);
    if(stat != HAL_OK) {
        err_counter++;
    }
}
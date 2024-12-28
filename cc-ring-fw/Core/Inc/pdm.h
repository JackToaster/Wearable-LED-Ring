#ifndef PDM_H
#define PDM_H
#include "main.h"

#define DECIMATION 64
#define SPI_BUFFER_SIZE 1024

#define PDM_IIR 31
#define PDM_VALUE_SCALE 20000 // scale up by this amount

void pdm_start(void);

void pdm_chain(void);


#endif
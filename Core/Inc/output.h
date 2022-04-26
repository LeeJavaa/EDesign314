/*
 * output.h
 *
 *  Created on: Apr 22, 2022
 *      Author: Lee Johnson 24058661
 */

#include "main.h"
#include "stateManagement.h"
#include <stdlib.h>
#include "math.h"

#define SIN_BUFFER_SIZE 10000

extern DAC_HandleTypeDef hdac1;
extern DMA_HandleTypeDef hdma_dac1_ch1;

uint32_t sinBuffer[SIN_BUFFER_SIZE];

uint16_t sinBufferChanged;
uint16_t sinOutputOn;
uint16_t offsetChanged;
uint16_t dcOutputOn;

uint16_t dcOffset;
uint16_t acOffset;
uint16_t amplitude;
uint16_t frequency;
uint32_t outputDCOffset;
uint32_t outputACOffset;
uint32_t outputAmplitude;

int16_t getValue(char *stringValue);
int32_t calibrate(uint32_t sample);
void updateOffset(uint16_t newOffset);
void updateAmplitude(uint16_t newAmplitude);
void updateFrequency(uint16_t newFrequency);
void dcOut();
void calcSin();
void acOut();

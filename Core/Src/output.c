/*
 * output.c
 *
 *  Created on: Apr 22, 2022
 *      Author: Lee Johnson 24058661
 */

#include "output.h"

#define PI 3.1415926

// Setting default values
uint16_t dcOffset = 1000; // mA or mV
uint16_t acOffset = 1200; // mA or mV
uint16_t amplitude = 1000; // mA or mV
uint16_t frequency = 1000; // Hz
// These will be the output values and if they are different from the variables above, we know that we have a change in what needs to be output, and we can
// handle that accordingly.
uint32_t outputDCOffset = 0;
uint32_t outputACOffset = 0;
uint32_t outputAmplitude = 0;

uint16_t sinBufferChanged = 0;
uint16_t offsetChanged = 0;
uint16_t sinOutputOn = 0;
uint16_t dcOutputOn = 0;

int16_t getValue(char *stringValue){

	int16_t value = atoi(stringValue);

	return value;
}

int32_t calibrate(uint32_t sample){
	uint32_t calibratedVal = sample/(2.15);

	return calibratedVal;
}

void updateOffset(uint16_t newOffset) {
	// Check if DC or AC and then update offset
	if(STATE_OUTPUT_TYPE_DC){
		dcOffset = newOffset;
		offsetChanged = 1;
	}else if(STATE_OUTPUT_TYPE_SINUSOIDAL){
		acOffset = newOffset;
	}
}

void updateAmplitude(uint16_t newAmplitude) {
	// Update Amplitude
	amplitude = newAmplitude;
}

void updateFrequency(uint16_t newFrequency) {
	// Update Frequency
	frequency = newFrequency;
}

void dcOut() {
	// DC output
	if(outputState == STATE_OUTPUT_TYPE_DC && outputState == STATE_OUTPUT_ON){
		if(sinOutputOn == 1){
			HAL_DAC_Stop_DMA(&hdac1, DAC1_CHANNEL_1);
			sinOutputOn = 0;
		}
		if(dcOutputOn == 0 || offsetChanged == 1){
			uint32_t sample = (uint32_t) (dcOffset*4096)/(3.3*1000);
			uint32_t calSample = calibrate(sample);
			HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, calSample);
			dcOutputOn = 1;
		}




		// do edge case checks for sample
		// do sample calibration
	}

}

void calcSin() {
	// Calculates sinusoidal values to be output
	// if sinOffset != acOffset: Calculate offset val
	uint16_t changedParam = 0;
	if(outputACOffset != acOffset){
		// the offset has been changed
		outputACOffset = (uint32_t) (acOffset*4096)/(3.3*1000);
		changedParam = 1;
	}

	if(outputAmplitude != amplitude){
		// the amplitude has been changed
		outputAmplitude = amplitude/1000;
		changedParam = 1;
	}

	if(changedParam == 1){
		for (int i = 0; i < SIN_BUFFER_SIZE; i++){
			sinBuffer[i] = calibrate(outputAmplitude*((sin(i*2*PI/SIN_BUFFER_SIZE) + 1)*(4096/2)) + outputACOffset);
		}
		sinBufferChanged = 1;
		changedParam = 0;
	}
}

void acOut() {
	// Sinusoidal output
	// if AC on and Output on:
	// if AC not already on || sinBuffer changed
	// start DAC DMA with sinBuffer
	if(outputType == STATE_OUTPUT_TYPE_SINUSOIDAL && outputState == STATE_OUTPUT_ON){
		if(sinOutputOn == 0 || sinBufferChanged == 1){
			HAL_DAC_Start_DMA(&hdac1, DAC1_CHANNEL_1, sinBuffer, SIN_BUFFER_SIZE, DAC_ALIGN_12B_R);
			sinOutputOn = 1;
			sinBufferChanged = 0;
			dcOutputOn = 0;
		}
	}
}


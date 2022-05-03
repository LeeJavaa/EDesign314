/*
 * output.c
 *
 *  Created on: Apr 22, 2022
 *      Author: Lee Johnson 24058661
 */

#include "output.h"
#include <stdio.h>

#define PI 3.1415926

// Setting default values
float dcOffset = 1000; // mA or mV
float acOffset = 1200; // mA or mV
float amplitude = 1000; // mA or mV
float frequency = 1000; // Hz
// These will be the output values and if they are different from the variables above, we know that we have a change in what needs to be output, and we can
// handle that accordingly.
float outputDCOffset = 0;
float outputACOffset = 0;
float outputAmplitude = 0;
float outputFrequency = 0;

// Math
const float convert = (4096)/((float) 3.3*1000);
const float convertCal = (4096)/((float) 3.3*1000*2.15);
const float convertCalAmp = (4096)/((float) 3.3*1000*2.15*2);
const float sinConst = (2*PI)/100000;

uint8_t sinBufferChanged = 0;
uint8_t offsetChanged = 0;
uint8_t changedParam = 0;
uint8_t sinOutputOn = 0;
uint8_t dcOutputOn = 0;

uint8_t acOffsetChanged = 1;
uint8_t amplitudeChanged = 1;
uint8_t frequencyChanged = 1;






// Debug
uint8_t nelson = 1;


float getValue(char *stringValue){

	int16_t value = atoi(stringValue);

	return (float) value;
}

float calibrate(float sample){
	float calibratedVal = sample/((float) (2.15));

	return calibratedVal;
}

void updateOffset(float newOffset) {
	// Check if DC or AC and then update offset
	if(outputType == STATE_OUTPUT_TYPE_DC){
		dcOffset = newOffset;
		offsetChanged = 1;
	}else if(outputType == STATE_OUTPUT_TYPE_SINUSOIDAL){
		acOffset = newOffset;
		acOffsetChanged = 1;
	}
}

void updateAmplitude(float newAmplitude) {
	// Update Amplitude
	amplitude = newAmplitude;
	amplitudeChanged = 1;
}

void updateFrequency(float newFrequency) {
	// Update Frequency
	frequency = newFrequency;
	frequencyChanged = 1;
}

void dcOut() {
	// DC output
	if(outputType == STATE_OUTPUT_TYPE_DC && outputState == STATE_OUTPUT_ON){
		if(sinOutputOn == 1){
			HAL_DAC_Stop_DMA(&hdac1, DAC1_CHANNEL_1);
			sinOutputOn = 0;
		}
		if(dcOutputOn == 0 || offsetChanged == 1){
			float sample = dcOffset*convertCal;
			for (int i = 0; i < 100; i++){
				  dc_val[i] = (uint32_t) sample;
			}
			HAL_DAC_Start_DMA(&hdac1, DAC1_CHANNEL_1, dc_val, 100, DAC_ALIGN_12B_R);
			dcOutputOn = 1;
			offsetChanged = 0;
		}
	}

	if(dcOutputOn == 1 && outputState == STATE_OUTPUT_OFF){
		HAL_DAC_Stop_DMA(&hdac1, DAC1_CHANNEL_1);
		dcOutputOn = 0;
	}

}

void calcSin() {
	// Calculates sinusoidal values to be output
	// if sinOffset != acOffset: Calculate offset val
	// Look in workbook to understand this, very confusing calculation

	// Debug
	uint32_t time1 = HAL_GetTick();
	if(acOffsetChanged){
		// the offset has been changed
		outputACOffset = (float) acOffset*convertCal;
		acOffsetChanged = 0;
		changedParam = 1;
	}

	if(amplitudeChanged){
		// the amplitude has been changed
		outputAmplitude = (float) amplitude*convertCalAmp;
		amplitudeChanged = 0;
		changedParam = 1;
	}

	if(frequencyChanged){
		// the amplitude has been changed
		frequencyChanged = 0;
		changedParam = 1;
	}

	if(changedParam == 1){
		nelson = 1;
		for (int i = 0; i < SIN_BUFFER_SIZE; i++){
			sinBuffer[i] = (uint32_t) ((outputAmplitude)*((sin(i*frequency*sinConst))) + outputACOffset);
		}
		sinBufferChanged = 1;
		changedParam = 0 ;
	}

	// Debug
	uint32_t time2 = HAL_GetTick() - time1;
	char a[4];
	sprintf(a, "%ld", time2);
	if(nelson){
		HAL_UART_Transmit(&huart2, a, 4, 50);
		nelson = 0;
	} else {
		;
	}

}

void acOut() {
	// Sinusoidal output
	// if AC on and Output on:
	// if AC not already on || sinBuffer changed
	// start DAC DMA with sinBuffer
	if(outputType == STATE_OUTPUT_TYPE_SINUSOIDAL && outputState == STATE_OUTPUT_ON){
		if(dcOutputOn){
			HAL_DAC_Stop_DMA(&hdac1, DAC1_CHANNEL_1);
			dcOutputOn = 0;
		}
		if(sinOutputOn == 0 || sinBufferChanged == 1){
			HAL_DAC_Start_DMA(&hdac1, DAC1_CHANNEL_1, sinBuffer, SIN_BUFFER_SIZE, DAC_ALIGN_12B_R);
			sinOutputOn = 1;
			sinBufferChanged = 0;
		}
	}
	if(sinOutputOn == 1 && outputState == STATE_OUTPUT_OFF){
		HAL_DAC_Stop_DMA(&hdac1, DAC1_CHANNEL_1);
		sinOutputOn = 0;
	}
}


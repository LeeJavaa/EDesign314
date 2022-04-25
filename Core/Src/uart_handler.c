/*
 * uart_handler.c
 *
 *  Created on: 06 Mar 2022
 *      Author: seanm
 */
#include "uart_handler.h"
#include "stateManagement.h"
#include "output.h"

int8_t isValidMsg(uint8_t *buffer, char *check, uint8_t length) {
	uint8_t matches = 0;
	for (int i = 0; i < length; i++) {
		if (*(check + i) == *(buffer + i)) {
			matches++;
		}
	}
	if (matches == length) {
		return 1;
	}
	return 0;
}

void clearBuffer()
{
	rxBufferOffset =0;
	memset(rxDataBuffer,0,20);
}

void decodeMsg()
{
	msgCode =0;
	snprintf((char*) paramValue, 4, "0000");
	//set measurement mode
	if (isValidMsg(rxDataBuffer, "@,$,", 4) && rxBufferOffset >= 9) {
		msgCode = MSG_SET_MEASUREMENT*MSG_MODE;
		if (isValidMsg(rxDataBuffer + 4, "DV,!\n", 5)) {
			msgCode += MSG_DV;
			setState(STATE_MEAS_DV);
		} else if (isValidMsg(rxDataBuffer + 4, "DI,!\n", 5)) {
			msgCode += MSG_DI;
			setState(STATE_MEAS_DI);
		} else if (isValidMsg(rxDataBuffer + 4, "AV,!\n", 5)) {
			msgCode += MSG_AV;
			setState(STATE_MEAS_AV);
		} else if (isValidMsg(rxDataBuffer + 4, "AI,!\n", 5)) {
			msgCode += MSG_AI;
			setState(STATE_MEAS_AI);
		} else if (isValidMsg(rxDataBuffer + 4, "TC,!\n", 5)) {
			msgCode += MSG_TC;
			setState(STATE_MEAS_TC);
		}

		// request measurement
	} else if (isValidMsg(rxDataBuffer, "@,*,m,", 6) && rxBufferOffset >=10) {
		msgCode = MSG_REQUEST_MEASUREMENT*MSG_MODE;
		if (isValidMsg(rxDataBuffer + 6, "f,!\n", 4)) {
			msgCode += MSG_FREQUENCY;
			sprintf((char*) txData,"@,m,f,%04d,!\n", measureFrequency());
			HAL_UART_Transmit(&huart2, txData, 13, 10);
		} else if (isValidMsg(rxDataBuffer + 6, "t,!\n", 4)) {
			msgCode += MSG_TYPE;
		} else if (isValidMsg(rxDataBuffer + 6, "a,!\n", 4)) {
			msgCode += MSG_AMPLITUDE;
			sprintf((char*) txData,"@,m,a,%04d,!\n", (uint16_t) (measureAmplitude()*ADC_TO_MV));
			HAL_UART_Transmit(&huart2, txData, 13, 10);
		} else if (isValidMsg(rxDataBuffer + 6, "o,!\n", 4)) {
			msgCode += MSG_OFFSET;
			sprintf((char*) txData,"@,m,o,%04d,!\n", (uint16_t) (measureDCOffset()*ADC_TO_MV));
			HAL_UART_Transmit(&huart2, txData, 13, 10);
		} else if (isValidMsg(rxDataBuffer + 6, "d,!\n", 4)) {
			msgCode += MSG_DUTYCYCLE;
		} else if (isValidMsg(rxDataBuffer + 6, "c,!\n", 4)) {
			msgCode += MSG_TEMPERATURE;
		}

		//request status
	}else if (isValidMsg(rxDataBuffer, "@,*,s,", 6) && rxBufferOffset >= 10) {
		msgCode = MSG_REQUEST_STATUS*MSG_MODE;
		if (isValidMsg(rxDataBuffer + 6, "1,!\n", 3)) {
			msgCode += MSG_OUTPUT_ON;
			setOutState(STATE_OUTPUT_ON);
			sprintf((char*) txData,"@,%s,%s,%d,!\n", stateToString(), outTypeToString(),outputState);
			HAL_UART_Transmit(&huart2, txData, 11, 10);
		} else if (isValidMsg(rxDataBuffer + 6, "0,!\n", 3)) {
			msgCode += MSG_OUTPUT_OFF;
			setOutState(STATE_OUTPUT_OFF);
			sprintf((char*) txData,"@,%s,%s,%d,!\n", stateToString(), outTypeToString(),outputState);
			HAL_UART_Transmit(&huart2, txData, 11, 10);
		}

		//set output
	}else if (isValidMsg(rxDataBuffer, "@,\",", 4) && rxBufferOffset >= 14) {
		msgCode = MSG_SET_OUTPUT*MSG_MODE;
		if (isValidMsg(rxDataBuffer + 4, "f,", 2)) {
			strncpy((char*) paramValue, (char*) rxDataBuffer+6, 4);
			uint16_t newFreq = getValue(paramValue);
			msgCode += MSG_FREQUENCY;
			updateFrequency(newFreq);
		} else if (isValidMsg(rxDataBuffer + 4, "t,", 2)) {
			msgCode += MSG_TYPE;
			if (isValidMsg(rxDataBuffer + 6, "d",1)){
				// Set state to DC
				setOutType(STATE_OUTPUT_TYPE_DC);
			} else if (isValidMsg(rxDataBuffer + 6, "s",1)) {
				// Set state to AC
				setOutType(STATE_OUTPUT_TYPE_SINUSOIDAL);
			}
		} else if (isValidMsg(rxDataBuffer + 4, "a,", 2)) {
			strncpy((char*) paramValue, (char*) rxDataBuffer+6, 4);
			uint16_t newAmp = getValue(paramValue);
			msgCode += MSG_AMPLITUDE;
			updateAmplitude(newAmp);
		} else if (isValidMsg(rxDataBuffer + 4, "o,", 2)) {
			strncpy((char*) paramValue, (char*) rxDataBuffer+6, 4);
			uint16_t newOff = getValue(paramValue);
			msgCode += MSG_OFFSET;
			updateOffset(newOff);
		} else if (isValidMsg(rxDataBuffer + 4, "d,", 2)) {
			msgCode += MSG_DUTYCYCLE;
		} else if (isValidMsg(rxDataBuffer + 4, "c,", 2)) {
			msgCode += MSG_TEMPERATURE;
		}
		if (msgCode > MSG_REQUEST_MEASUREMENT*MSG_MODE)
		{
			strncpy((char*) paramValue, (char*) rxDataBuffer+6, 4);
		}
	}

	//TODO LCD display

	if (msgCode %10 !=0)
	{
		clearBuffer();
	}

	if (rxBufferOffset>14)
	{
		while(rxDataBuffer[0] != '@' && rxBufferOffset >0)
		{
			for (int i =0; i < rxBufferOffset; i++)
			{
				rxDataBuffer[i] = rxDataBuffer[i+1];
			}
			rxBufferOffset--;
		}
	}
}

void msgProcessing() {
	decodeMsg();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	/* Prevent unused argument(s) compilation warning */
	if (huart == &huart2) {
		*(rxDataBuffer + rxBufferOffset++) = *rxData;
		HAL_UART_Receive_IT(&huart2, rxData, 1);
		msgProcessing();
	}

	/* NOTE : This function should not be modified, when the callback is needed,
	 the HAL_UART_RxCpltCallback can be implemented in the user file.
	 */
}

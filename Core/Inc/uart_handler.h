/*
 * uart_handler.h
 *
 *  Created on: 06 Mar 2022
 *      Author: seanm
 */
#pragma once

#include "main.h"
#include <string.h>
#include "stdio.h"
#include "measurements.h"
#include "stateManagement.h"

extern UART_HandleTypeDef huart2;

#define MSG_SET_MEASUREMENT 1
#define MSG_SET_OUTPUT 2
#define MSG_REQUEST_STATUS 3
#define MSG_REQUEST_MEASUREMENT 4
#define MSG_DISPLAY_LCD 5
#define MSG_DV 1
#define MSG_DI 2
#define MSG_AV 3
#define MSG_AI 4
#define MSG_TC 5
#define MSG_TYPE 1
#define MSG_AMPLITUDE 2
#define MSG_OFFSET 3
#define MSG_FREQUENCY 4
#define MSG_DUTYCYCLE 5
#define MSG_TEMPERATURE 6
#define MSG_MODE 10
#define MSG_OUTPUT_ON 1
#define MSG_OUTPUT_OFF 2


uint8_t paramValue[4];
uint8_t msgCode;
uint8_t txData[100];
uint8_t rxData[1];
uint8_t rxDataBuffer[20];
uint8_t rxBufferOffset;

void clearBuffer();
void msgProcessing();
void decodeMsg();
int8_t isValidMsg(uint8_t *buffer, char *check, uint8_t length);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

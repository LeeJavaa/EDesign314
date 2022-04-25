/*
 * stateManagement.h
 *
 *  Created on: 10 Mar 2022
 *      Author: seanm
 */
#pragma once

#include "main.h"

#define STATE_MEAS_DV 1
#define STATE_MEAS_DI 2
#define STATE_MEAS_AV 3
#define STATE_MEAS_AI 4
#define STATE_MEAS_TC 5
#define STATE_MENU_DISPLAY 6
#define STATE_OUTPUT_ON 1
#define STATE_OUTPUT_OFF 0
#define STATE_OUTPUT_TYPE_DC 1
#define STATE_OUTPUT_TYPE_SINUSOIDAL 2
#define STATE_OUTPUT_TYPE_PULSE 3


uint8_t state;
uint8_t outputState;
uint8_t outputType;

void setState(uint8_t newState);
void setOutType(uint8_t newOutType);
void setOutState(uint8_t newOutState);
const char* stateToString();
const char* outTypeToString();
void initState();


/* INC_STATEMANAGEMENT_H_ */

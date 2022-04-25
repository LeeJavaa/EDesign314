/*
 * stateManagement.c
 *
 *  Created on: 10 Mar 2022
 *      Author: seanm
 */
#include "stateManagement.h"

void initState() {
	state = STATE_MEAS_DV;
	outputState = STATE_OUTPUT_OFF; // needs to be off
	outputType = STATE_OUTPUT_TYPE_DC;
}

void setState(uint8_t newState) {
	if (newState > 0 && newState <=6)
	{
		if (newState !=STATE_MENU_DISPLAY)
		{
			GPIOA->ODR = GPIOA->ODR & (!0b1<<8);
		}
		state = newState;
	}
}

void setOutType(uint8_t newOutType)
{
	if (newOutType > 0 && newOutType <=3)
		{
			outputType = newOutType;
		}
}

void setOutState(uint8_t newOutState)
{
	if (newOutState >= 0 && newOutState <=1)
	{
		outputState = newOutState;
	}
}

const char* stateToString()
{
	switch (state)
	{
	case 1:
		return "DV";
		break;
	case 2:
		return "DI";
		break;
	case 3:
		return "AV";
		break;
	case 4:
		return "AI";
		break;
	case 5:
		return "TC";
		break;
	}
	return "";
}

const char* outTypeToString()
{
	switch (outputType)
	{
	case 1:
		return "d";
		break;
	case 2:
		return "s";
		break;
	case 3:
		return "p";
		break;
	}
	return "";
}


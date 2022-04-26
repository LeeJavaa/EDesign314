/*
 * stateManagement.c
 *
 *  Created on: 10 Mar 2022
 *      Author: seanm
 */
#include "stateManagement.h"

void initState() {
	state = STATE_MEAS_DV;
	outputState = STATE_OUTPUT_OFF;
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

void setMeasParam(uint8_t newMeasParam)
{
	if (newMeasParam >= 1 && newMeasParam <=6)
	{
		measurementParameter = newMeasParam;
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

uint8_t outTypeToString()
{
	switch (outputType)
	{
	case 1:
		return (uint8_t) 'd';
		break;
	case 2:
		return (uint8_t) 's';
		break;
	case 3:
		return (uint8_t) 'p';
		break;
	}
	return (uint8_t) 'E';
}

uint8_t outputToString()
{
	switch (outputState)
	{
	case 0:
		return (uint8_t) '0';
		break;
	case 1:
		return (uint8_t) '1';
		break;
	}
	return (uint8_t) 'E';
}

uint8_t measParamToString()
{
	switch (state)
	{
	case 1:
		return (uint8_t) 't';
		break;
	case 2:
		return (uint8_t) 'a';
		break;
	case 3:
		return (uint8_t) 'o';
		break;
	case 4:
		return (uint8_t) 'f';
		break;
	case 5:
		return (uint8_t) 'd';
		break;
	case 6:
		return (uint8_t) 'c';
		break;
	}
	return (uint8_t) 'E';
}


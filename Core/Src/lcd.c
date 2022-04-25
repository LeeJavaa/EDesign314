/*
 * lcd.c
 *
 *  Created on: Apr 24, 2022
 *      Author: Lee Johnson 24058661
 */

#include "lcd.h"

void LCD_DATA(unsigned char Data){
	if(Data & 1){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
	}
	if(Data & 2){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
	}
	if(Data & 4){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
	}
	if(Data & 8){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
	}
}

void LCD_CMD(unsigned char CMD){
	// Select Command Register
	// RS = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	// Move the Command Data to LCD
	LCD_DATA(CMD);
	// Send the EN Clock Signal
	// EN = 1;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
	HAL_Delay(1);
	// EN=0;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
}

void LCD_init(){
	// The Init. Procedure
	LCD_DATA(0x00);
	HAL_Delay(31);
	LCD_CMD(0x03);
	HAL_Delay(5);
	LCD_CMD(0x03);
	HAL_Delay(1);
	LCD_CMD(0x03);
	LCD_CMD(0x02);
	LCD_CMD(0x02);
	LCD_CMD(0x08);
	LCD_CMD(0x00);
	LCD_CMD(0x0C);
	LCD_CMD(0x00);
	LCD_CMD(0x06);
}

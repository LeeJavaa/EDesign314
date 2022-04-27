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
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_Delay(1);
	// EN=0;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
}

void LCD_init(){
	// The initialization Procedure
	HAL_Delay(15);
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
	LCD_CMD(0x0F); // On Display, On Cursor, Cursor blink
	LCD_CMD(0x00);
	LCD_CMD(0x06); // Increment
}

void LCD_Write_Char(char Data){
	char Low4, High4;
	Low4 = Data & 0x0F;
	High4 = Data & 0xF0;
	// RS = 1 = Data mode
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
	LCD_DATA(High4>>4); // Shifting the upper 4 bits of the byte to a nibble so that the data can be send
	// EN = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_Delay(1);
	// EN = 0; Data transfer on falling edge
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_Delay(1);
	LCD_DATA(Low4); // Now sending the lower nibble
	// EN = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_Delay(1);
	// EN = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
}

void LCD_Write_String(char *str){
	for(int i = 0; str[i]!= '\0'; i++){
		// While we do not reach the terminating char of the string, write out to the LCD
		LCD_Write_Char(str[i]);
	}
}

void LCD_Clear(){
	// Clear Display: 00000001
	LCD_CMD(0);
	LCD_CMD(1);
}

void LCD_Set_Cursor(unsigned char r, unsigned char c)
{
	// 0x80 is the base
	// r = row, c = column
	unsigned char Temp, Low4, High4;
	if(r==1)
	{
		Temp = 0x80 + c - 1;
		High4 = Temp >> 4;
		Low4 = Temp & 0x0F;
		LCD_CMD(High4);
		LCD_CMD(Low4);
	}
	if(r==2)
	{
		Temp = 0xC0 + c - 1;
		High4 = Temp >> 4;
		Low4 = Temp & 0x0F;
		LCD_CMD(High4);
		LCD_CMD(Low4);
	}
}

void LCD_SR()
{
	// Shift display right 00011100
	LCD_CMD(0x01);
	LCD_CMD(0x0C);
}

void LCD_SL()
{
	// Shift display left 00011000
	LCD_CMD(0x01);
	LCD_CMD(0x08);
}



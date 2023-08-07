#pragma once

#include <avr/io.h>
#include "LCD5110.h"

uint8_t *pPlayingField;
uint8_t	cellSize;
uint8_t numCellsByte;
uint8_t maxFieldX;
uint8_t maxFieldY;

void GOL_Init(uint8_t* pVideoBuff, uint8_t size);
void GOL_Next_Step();
void GOL_Draw_Column(uint8_t *pData, int8_t x);
	
uint8_t GOL_Get_Next_State_Cell(int8_t x, int8_t y);
uint8_t GOL_Cell_Condition_Check(uint8_t isCellAlive, uint8_t amtCell);
uint8_t GOL_Get_bit(int8_t x, int8_t y);

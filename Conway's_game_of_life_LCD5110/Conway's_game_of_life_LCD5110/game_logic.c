#include "game_logic.h"

void GOL_Init(uint8_t* pVideoBuff, uint8_t size) {
	cellSize = size;
	numCellsByte = 8 / size;
	pPlayingField = pVideoBuff;

	if(size == 1) bitMask = GOL_BITMASK_CELL_SIZE_1;
	if(size == 2) bitMask = GOL_BITMASK_CELL_SIZE_2;
	if(size == 4) bitMask = GOL_BITMASK_CELL_SIZE_4;
	if(size == 8) bitMask = GOL_BITMASK_CELL_SIZE_8;
}

void GOL_Next_Step() {
	uint8_t tmpBuff[3][LCD5510_Y] = {};
	
	GOL_Get_Next_State_Cell(tmpBuff[0], 0);
	GOL_Get_Next_State_Cell(tmpBuff[1], 1);

	for(int8_t fieldX = 2; fieldX < LCD5510_X / cellSize; fieldX++) {
		GOL_Get_Next_State_Cell(tmpBuff[2], fieldX);
		GOL_Draw_Column(tmpBuff[1], fieldX - 1);
		
		tmpBuff[1][0] = tmpBuff[2][0];
		tmpBuff[1][1] = tmpBuff[2][1];
		tmpBuff[1][2] = tmpBuff[2][2];
		tmpBuff[1][3] = tmpBuff[2][3];
		tmpBuff[1][4] = tmpBuff[2][4];
		tmpBuff[1][5] = tmpBuff[2][5];
	}
	GOL_Draw_Column(tmpBuff[0], 0);
	GOL_Draw_Column(tmpBuff[2], LCD5510_X / cellSize - 1);
}

void GOL_Draw_Column(uint8_t *pData, int8_t x) {
	x *= cellSize;
	for(uint8_t condition = x + cellSize; x < condition; x++) {
		for(uint8_t y = 0; y < LCD5510_Y; y++) {
			pPlayingField[y * LCD5510_X + x] = pData[y];
		}
	}
}

void GOL_Get_Next_State_Cell(uint8_t *pData, int8_t fieldX) {
	pData[0] = 0;
	pData[1] = 0;
	pData[2] = 0;
	pData[3] = 0;
	pData[4] = 0;
	pData[5] = 0;
			
	struct {
		uint8_t bit0	:1;
		uint8_t bit1	:1;
		uint8_t bit2	:1;
		uint8_t bit3	:1;
		uint8_t bit4	:1;
		uint8_t bit5	:1;
		uint8_t bit6	:1;
		uint8_t bit7	:1;
		uint8_t bit8	:1;
	}bitMap;
	
	bitMap.bit0 = GOL_Get_bit(fieldX-1,	-1);
	bitMap.bit1 = GOL_Get_bit(fieldX,	-1);
	bitMap.bit2 = GOL_Get_bit(fieldX+1,	-1);
	bitMap.bit3 = GOL_Get_bit(fieldX-1,	 0);
	bitMap.bit4 = GOL_Get_bit(fieldX,	 0);
	bitMap.bit5 = GOL_Get_bit(fieldX+1,	 0);
	
	uint8_t bits = bitMask;
	
	for(int8_t fieldY = 0; fieldY < LCD5510_Y * numCellsByte; fieldY++) {
		bitMap.bit6 = GOL_Get_bit(fieldX-1,	fieldY +1);
		bitMap.bit7 = GOL_Get_bit(fieldX,	fieldY +1);
		bitMap.bit8 = GOL_Get_bit(fieldX+1,	fieldY +1);
		
		uint8_t amtCell =	bitMap.bit0 + bitMap.bit1 + bitMap.bit2 + bitMap.bit3 + 
							bitMap.bit5 + bitMap.bit6 + bitMap.bit7 + bitMap.bit8;
		
		if(GOL_Cell_Condition_Check(bitMap.bit4, amtCell))
			pData[fieldY / numCellsByte] |= bits;
			
		bitMap.bit0 = bitMap.bit3;
		bitMap.bit1 = bitMap.bit4;
		bitMap.bit2 = bitMap.bit5;
		bitMap.bit3 = bitMap.bit6;
		bitMap.bit4 = bitMap.bit7;
		bitMap.bit5 = bitMap.bit8;
		
		bits <<= cellSize;
		if(!bits)
			bits = bitMask;
	}
}

uint8_t GOL_Cell_Condition_Check(uint8_t isCellAlive, uint8_t amtCell) {
	if(isCellAlive)
		return amtCell == 2 || amtCell == 3;
	else
		return amtCell == 3;
}

uint8_t GOL_Get_bit(int8_t x, int8_t y) {
	x *= cellSize;
	
	if(x >= LCD5510_X)
		x -= LCD5510_X;
	if(y >= LCD5510_Y * numCellsByte)
		y -= LCD5510_Y * numCellsByte;
	
	if(x < 0)
		x += LCD5510_X;
	if(y < 0)
		y += LCD5510_Y * numCellsByte;
	
	uint8_t bit = y % numCellsByte * cellSize;
	y /= numCellsByte;
	
	if(pPlayingField[y * LCD5510_X + x] & (1 << bit))
		return 1;
	else
		return 0;
}
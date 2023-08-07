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
	uint8_t bits = bitMask;
	for(int8_t fieldY = 0; fieldY < LCD5510_Y * numCellsByte; fieldY++) {
		if(GOL_Get_Next_State_Cell(0, fieldY))
			tmpBuff[0][fieldY / numCellsByte] |= bits;
			
		if(GOL_Get_Next_State_Cell(1, fieldY))
			tmpBuff[1][fieldY / numCellsByte] |= bits;
			
		bits <<= cellSize;
		if(!bits)
			bits = bitMask;
	}
	
	for(int8_t fieldX = 2; fieldX < LCD5510_X / cellSize; fieldX++) {
		tmpBuff[2][0] = 0;
		tmpBuff[2][1] = 0;
		tmpBuff[2][2] = 0;
		tmpBuff[2][3] = 0;
		tmpBuff[2][4] = 0;
		tmpBuff[2][5] = 0;
		
		for(int8_t fieldY = 0; fieldY < LCD5510_Y * numCellsByte; fieldY++) {
			if(GOL_Get_Next_State_Cell(fieldX, fieldY))
				tmpBuff[2][fieldY / numCellsByte] |= bits;
			bits <<= cellSize;
			if(!bits)
				bits = bitMask;
		}
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

uint8_t GOL_Get_Next_State_Cell(int8_t fieldX, int8_t fieldY) {
	uint8_t amtCell = 0;
	amtCell += GOL_Get_bit(fieldX -1,	fieldY -1);
	amtCell += GOL_Get_bit(fieldX,		fieldY -1);
	amtCell += GOL_Get_bit(fieldX +1,	fieldY -1);
	amtCell += GOL_Get_bit(fieldX -1,	fieldY);
	amtCell += GOL_Get_bit(fieldX +1,	fieldY);
	amtCell += GOL_Get_bit(fieldX -1,	fieldY +1);
	amtCell += GOL_Get_bit(fieldX,		fieldY +1);
	amtCell += GOL_Get_bit(fieldX +1,	fieldY +1);
	
	return GOL_Cell_Condition_Check(GOL_Get_bit(fieldX, fieldY), amtCell);
}


uint8_t GOL_Cell_Condition_Check(uint8_t isCellAlive, uint8_t amtCell) {
	if(isCellAlive) {
		if(amtCell == 2 || amtCell == 3)
			return 1;
		else
			return 0;
	}	
	else {	
		if(amtCell == 3)
			return 1;
		else
			return 0;		
	}
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
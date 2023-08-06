#include "game_logic.h"

void GOL_Init(uint8_t* pVideoBuff, uint8_t size) {
	cellSize = size;
	numCellsByte = 8 / size;
	pPlayingField = pVideoBuff;
}

void GOL_Next_Step() {
	uint8_t tmpBuff[3][LCD5510_Y] = {};
		
	for(int8_t y = 0; y < LCD5510_Y * numCellsByte; y++) {
		tmpBuff[0][y / numCellsByte] +=
			GOL_Get_Next_State_Cell(0, y) << (y % numCellsByte);
		tmpBuff[1][y / numCellsByte] +=
			GOL_Get_Next_State_Cell(1, y) << (y % numCellsByte);
	}
	
	
	for(int8_t x = 2; x < LCD5510_X / cellSize; x++) {
		tmpBuff[2][0] = 0;
		tmpBuff[2][1] = 0;
		tmpBuff[2][2] = 0;
		tmpBuff[2][3] = 0;
		tmpBuff[2][4] = 0;
		tmpBuff[2][5] = 0;
		
		for(int8_t y = 0; y < LCD5510_Y * numCellsByte; y++) {
			tmpBuff[2][y / numCellsByte] +=
				GOL_Get_Next_State_Cell(x, y) << (y % numCellsByte);
		}
		GOL_Draw_Column(tmpBuff[1], x - 1);
		tmpBuff[1][0] = tmpBuff[2][0];
		tmpBuff[1][1] = tmpBuff[2][1];
		tmpBuff[1][2] = tmpBuff[2][2];
		tmpBuff[1][3] = tmpBuff[2][3];
		tmpBuff[1][4] = tmpBuff[2][4];
		tmpBuff[1][5] = tmpBuff[2][5];
	}
	
	//GOL_Draw_Column(tmpBuff[0], 0);
	//GOL_Draw_Column(tmpBuff[2], LCD5510_X / cellSize - 1);
}

void GOL_Draw_Column(uint8_t *pData, int8_t x) {
	x *= cellSize;
	for(uint8_t i = 0; i < cellSize; i++) {
		
		for(uint8_t y = 0; y < LCD5510_Y; y++) {
			pPlayingField[y * LCD5510_X + x] = 0;
			
			for(uint8_t bit = 0; bit < 8; bit += cellSize) {
				
				for(uint8_t j = 0; j < cellSize; j++) {
					if(pData[y] & 1<<(bit / cellSize)) 
						pPlayingField[y * LCD5510_X + x] += 1<<(bit + j);
				}
			}
		}
		x++;
	}
}

uint8_t GOL_Get_Next_State_Cell(int8_t x, int8_t y) {
	uint8_t amtCell = 0;
	amtCell += GOL_Get_bit(x-1,	y-1);
	amtCell += GOL_Get_bit(x,	y-1);
	amtCell += GOL_Get_bit(x+1,	y-1);
	amtCell += GOL_Get_bit(x-1,	y);
	amtCell += GOL_Get_bit(x+1,	y);
	amtCell += GOL_Get_bit(x-1,	y+1);
	amtCell += GOL_Get_bit(x,	y+1);
	amtCell += GOL_Get_bit(x+1,	y+1);
	
	return GOL_Cell_Condition_Check(GOL_Get_bit(x, y), amtCell);
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


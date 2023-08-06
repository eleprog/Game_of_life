#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>

#include "LCD5110.h"
#include "game_logic.h"
#include "start_playing_field.h"

uint8_t *videoBuffer = startPlayingField;
	

int main(void) {
	LCD5110_Init();
	GOL_Init(videoBuffer, 2);
	LCD5110_Video_Buff_Send(videoBuffer);
	
    while (1) {
		GOL_Next_Step();
		LCD5110_Video_Buff_Send(videoBuffer);	
    }
}


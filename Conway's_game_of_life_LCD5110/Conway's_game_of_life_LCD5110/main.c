#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>

#include "LCD5110.h"
#include "game_logic.h"
#include "start_playing_field.h"

uint8_t *videoBuffer = startPlayingField;
	
int main(void) {
	DDRC |= 1<<5;
	LCD5110_Init();
	GOL_Init(videoBuffer, 1);
	LCD5110_Video_Buff_Send(videoBuffer);
	
    while (1) {
		PORTC |= 1<<5;
		GOL_Next_Step();
		PORTC &= ~(1<<5);
		LCD5110_Video_Buff_Send(videoBuffer);	
    }
}
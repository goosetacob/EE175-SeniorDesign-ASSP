/* Name: main.c
 * Author: Gustavo Blanco
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	//0 input
	//1 output
	DDRD = 0x40;
	//MUST SET TO OPPOSITVE OF DESIRED I/O
	PORTD = 0x80;

	unsigned char tempSig = 0x00;

	while(1) {
		if (!(PIND && 0x80)) {
			tempSig = 0x40;
		} else {
			tempSig = 0x00;
		}
		PORTD = tempSig;
	}

    	return 0;
}

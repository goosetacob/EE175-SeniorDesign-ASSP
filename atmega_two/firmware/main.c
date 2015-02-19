/* Name: main.c
 * Author: Gustavo Blanco, Russell Perry, Brett Fontaine
 */

// QUICK BIT MANIPULATION RULES
// **n is the index
// Set a bit: bit_fld |= (1 << n)
// Clear a bit: bit_fld &= ~(1 << n)
// Toggle a bit: bit_fld ^= (1 << n)
// Test a bit: bit_fld & (1 << n)

#include "main.h"

int main(void) {
	//0 input
	//1 output
	//MUST SET TO OPPOSITVE OF DESIRED I/O

	// PORT A is for Sonar Sensors: x000x111
	DDRA = 0x07;
	PORTA = 0x70;

	// PORT D is for Motor Control: xxxxxx10
	DDRD = 0x40;
	PORTD = 0x80;

	// Set timer up in CTC mode
	TCCR1A = 0x00;
	TCCR1B = 0x08;

	// 100MHz I2C clock frequency
	TWBR = 0x20;

	while(1) {
		//sonarStat('0');
		//sonarStat('1');
		//sonarStat('2');
		if (!(PIND && 0x80)) {
			PORTD = 0x40;
		} else {
			PORTD = 0x00;
		}
	}

  return 0;
}

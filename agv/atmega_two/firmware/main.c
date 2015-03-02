/* Name: main.c
 * Author: Gustavo Blanco, Russell Perry, Brett Fontaine
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <stdbool.h>
#include "lib/defines.h"
#include "lib/bit.h"
//#include "lib/BasicSerial3.h"

//function prototypes
void usart_init( uint16_t ubrr);
void usart_putchar(char data);
char usart_getchar(void);
unsigned char usart_kbhit(void);
void usart_pstr(char *s);

inline void set_tx() {
	//bitClear(UCSR1B, RXEN1);
	//bitSet(UCSR1B, TXEN1);

  //clear RX bit
  SetBit(UCSRB, RXEN, 0);
  //set TX bit
  SetBit(UCSRB, TXEN, 1);
}

inline void set_rx() {
	//bitSet(UCSR1B, RXEN1);
	//bitClear(UCSR1B, TXEN1);


  //clear TX bit
  SetBit(UCSRB, TXEN, 0);
  //set RX bit
  SetBit(UCSRB, RXEN, 1);
}

inline void rx_packet()
{
	static bool rx_packet_first_time = true;
	static char discard_udr_buffer;

	//skip last 2 bytes sent which are kept in udr
	if(!rx_packet_first_time)
	{
		while (bit_is_clear(UCSRA, RXC));
		discard_udr_buffer = UDR;
		while (bit_is_clear(UCSRA, RXC));
		discard_udr_buffer = UDR;
	}
	rx_packet_first_time = false;

  //...receive servo bytes
}


void command_servo()
{
	set_tx();
	cli();
	tx_packet();
	tx_wait_to_finish();
	set_rx();
	rx_packet();
	sei();
}

int main(void) {
    // configure PORTA as output
    DDRA = 0xFF;
    // setup PORTB data direction as an input
    DDRB = 0;
    // make sure it is high impedance and will not source
    PORTB = 0;
    // fire up the usart
    usart_init (MYUBRR);

    // main loop
    while(1) {
        //start condition
				usart_putchar(0xA5);
				//command class
				usart_putchar(0x00);
				//command list
				usart_putchar(0x04);
				//end condition
				usart_putchar(0x5A);

        _delay_ms(100);

				//GetLat
				char degree = usart_getchar();
				char minutes = usart_getchar();
				char fractional = usart_getchar();
				char minute = usart_getchar();
				char direction = usart_getchar();

    }
}

void usart_init( uint16_t ubrr) {
    // Set baud rate
    UBRRH = (uint8_t)(ubrr>>8);
    UBRRL = (uint8_t)ubrr;
    // Enable receiver and transmitter
    UCSRB = (1<<RXEN)|(1<<TXEN);
    // Set frame format: 8data, 1stop bit
    UCSRC = (1<<URSEL)|(3<<UCSZ0);
}
void usart_putchar(char data) {
    // Wait for empty transmit buffer
    while ( !(UCSRA & (_BV(UDRE))) );
    // Start transmission
    UDR = data;
}
char usart_getchar(void) {
    // Wait for incoming data
    while ( !(UCSRA & (_BV(RXC))) );
    // Return the data
    return UDR;
}
void usart_pstr(char *s) {
    // loop through entire string
    while (*s) {
        usart_putchar(*s);
        s++;
    }
}

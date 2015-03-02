#ifndef DEFINES_H__
#define DEFINES_H__

#define START_CLK	0x0A		  // Start clock with 1:8 prescaler CTC mode
#define START_CLK_N	0x02		// Start clock running 1:8 prescaler in normal mode
#define STOP_CLK	0x08			// Stop clock
#define LCD03		0xC6				// Address of LCD03
#define F_CPU 8000000

//USART BAUDRATE
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#endif

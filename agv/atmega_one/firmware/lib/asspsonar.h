#ifndef SONAR_H
#define SONAR_H

#include "defines.h"

long sonarStat(char sonarLabel);
void sendTrigger(int trigPin);
unsigned int getEcho(int echoPin);
void startTimer(unsigned int time);
void waitForTimer(void);

// sonarStat
// input: char with direction of which sonar sensor to detect:
// 		0 -> front
// 		1 -> right
// 		2 -> left
// output: integer value returned by the sonar sensor
long sonarStat(char sonarLabel) {
  	long distance;

  	int trigPin = -1;
  	int echoPin = -1;

  	if (sonarLabel == 0) {
  		//front
  		trigPin = 0;
  		echoPin = 4;
  	} else if (sonarLabel == 1) {
  		//right
  		trigPin = 1;
  		echoPin = 5;
  	} else if (sonarLabel == 2) {
  		//left
  		trigPin = 2;
  		echoPin = 6;
  	} else {
  		//cout << "no sonar sensor found";
  		return -1;
  	}

  	sendTrigger(trigPin);
  	distance = getEcho(echoPin);

  	return distance;
}

void sendTrigger(int trigPin) {
	//set trigger HIGH
	PORTA |= (1 << trigPin);
	// Wait around 10uS
	startTimer(0x0001);
	waitForTimer();
	//set trigger LOW
	PORTA &= ~(1 << trigPin);
}

unsigned int getEcho(int echoPin) {
	unsigned int range;

	// wait for start of pulse
	while(!(PORTA & (1 << echoPin)));
	// timer to zero
	TCNT1 = 0x00;
	// start timer 1:8 prescaler
	TCCR1B = START_CLK_N;
	// wait for end ofpulse
	while((PORTA & (1 << echoPin)));
	// stop timer and set back to CTC mode
	TCCR1B = STOP_CLK;
	// number divided by 58 will give range
	range = TCNT1/58;

	return(range);
}

void startTimer(unsigned int time) {
	// time to count to
	OCR1A = time;
	// clear compare match flag
	TIFR = 0x10;
	// clear timer
	TCNT1 = 0x00;
	// start timer 1:8 prescaler
	TCCR1B = START_CLK;
}

void waitForTimer(void) {
	while(!(TIFR&0x10));	// wait for timer to set compare match flag
	TCCR1B = STOP_CLK;		// Stop the timer
}

#endif

#ifndef SONAR_H
#define SONAR_H


long sonarStat(char sonarLabel);

void sendTrigger(int trigPin);

unsigned int getEcho(int echoPin);

void startTimer(unsigned int time);

void waitForTimer(void);


#endif

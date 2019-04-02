// 
// little Helper to move servo to middleposition
// needed before assembling

#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPiI2C.h>
#include <pca9685.h>

#define SERVO_MIN_MS 5      
#define SERVO_MAX_MS 27       

#define devId0	0x40
#define devId1	0x41
#define PIN_BASE0 64
#define PIN_BASE1 128
#define PWM_MAX 4096
#define HERTZ 50

int middle = (SERVO_MIN_MS+SERVO_MAX_MS)/2;		//16

long map(long value,long fromLow,long fromHigh,long toLow,long toHigh){
    return (toHigh-toLow)*(value-fromLow) / (fromHigh-fromLow) + toLow;
}

void servoWriteMS(int pin, int ms){     //specific the unit for pulse(5-25ms) with specific duration output by servo pin: 0.1ms
	if(ms > SERVO_MAX_MS) {
		printf("Pin: %i ms: %i too big\n",pin,ms);
		ms = SERVO_MAX_MS;
	}
	if(ms < SERVO_MIN_MS) {
		printf("Pin: %i ms: %i too small\n",pin,ms);
		ms = SERVO_MIN_MS;
	};
	pwmWrite(pin,map(ms,0,200,0,PWM_MAX));
	delay(10);
}


int main (int argc, char *argv[]) {
	int i;
	if(wiringPiSetup() == -1){ //when initialize wiring faiservo,print messageto screen
		printf("setup wiringPi faiservo !");
        return 1;
	}
	if(wiringPiI2CSetup(devId0) == -1){ 
       		printf("setup wiringPi I2C faiservo !");
       	return 1; 
	};
	
	int fd0 = pca9685Setup(PIN_BASE0, devId0, HERTZ);
	if (fd0 < 0)	{
		printf("Error in setup\n");
		return fd0;
	}
	pca9685PWMReset(fd0);
	
	if(wiringPiI2CSetup(devId1) == -1){ 
       		printf("setup wiringPi I2C faiservo !");
       	return 1; 
	};
	int fd1 = pca9685Setup(PIN_BASE1, devId1, HERTZ);
	if (fd1 < 0)	{
		printf("Error in setup\n");
		return fd1;
	}
	pca9685PWMReset(fd1);
	while(run) {for (i=0;i<16;i++) {
		pinMode(16+PIN_BASE0,OUTPUT);
		servoWriteMS(16+PIN_BASE0,16);			
		pinMode(16+PIN_BASE1,OUTPUT);
		servoWriteMS(16+PIN_BASE1,16);	
		
    return 0;
}

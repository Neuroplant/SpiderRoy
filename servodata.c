//**********************************************************************
//CheckServo
//This little Program is made for helping to gather all Servo-Limits
//needed for SpiderRoy.
// min is the position to the front or most close to the body
// max is the position to the back or most away from the body or up
// for ServoNr. see ServoPosition.png, use "99" as dummy for unused
// output to screen and I2CServos.txt
//**********************************************************************
#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPiI2C.h>
#include <pca9685.h>

#define OFFSET_MS 0     
#define SERVO_MIN_MS 5+OFFSET_MS        
#define SERVO_MAX_MS 27+OFFSET_MS       

#define devId0	0x40
#define devId1	0x41
#define PIN_BASE0 64
#define PIN_BASE1 128
#define PWM_MAX 4096
#define HERTZ 50

FILE *fp;

struct s_Servo {
	int min;
	int max;
	int num;
};
struct s_Servo Servo[34];

char key;
char str1[20];

long map(long value,long fromLow,long fromHigh,long toLow,long toHigh){
    return (toHigh-toLow)*(value-fromLow) / (fromHigh-fromLow) + toLow;
}

void servoWriteMS(int pin, int ms);

void servoInit(int pin,int base){        //initialization function for servo PMW pin
	pinMode(pin+PIN_BASE0,OUTPUT);
	Servo[pin].min=16;
	Servo[pin].max=16;
	Servo[pin].num=99;
	servoWriteMS(pin+PIN_BASE0,16);	
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
	printf("Pin: %i ms: %i \n",pin,ms);
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
/////////////////////////0x40	
	for (i=0;i<16;i++) {
		printf("\nTesting Servo Nr. %i an Modul %i\n",i,devId0);
		pinMode(i+PIN_BASE0,OUTPUT);
		Servo[i].min=16;
		Servo[i].max=16;
		servoWriteMS(i+PIN_BASE0,16);			
		printf("Setze MIN Value (+/-/x)\n");
		key=0;
		while (key != 120) {		
			key=getchar();
			if (key==43) {
				Servo[i].min++;
			}
			if (key==45) {
				Servo[i].min--;
			}
			servoWriteMS(i+PIN_BASE0,Servo[i].min);
			printf("Servo %i min:%i\n",i,Servo[i].min);
		}
		printf("Setze MAX Value (+/-/x)\n");
		key=0;
		while (key != 120) {
			key=getchar();
			if (key==43) {
				Servo[i].max++;
			}
			if (key==45) {
				Servo[i].max--;
			}
			servoWriteMS(i+PIN_BASE0,Servo[i].max);
			printf("Servo %i max:%i\n",i,Servo[i].max);
		}
		printf("\nServoNr. :");
		scanf("%s",str1);
		
		Servo[i].num=atoi(str1);
		printf("\nServoNr.. :%d \n",Servo[i].num);
		servoWriteMS(i+PIN_BASE0,(Servo[i].min+Servo[i].max)/2);

	}
/////////////////////////0x41
	for (i=0;i<16;i++) {
		printf("\nTesting Servo Nr. %i an Modul %i\n",i,devId1);
		pinMode(i+PIN_BASE1,OUTPUT);
		Servo[i+17].min=16;
		Servo[i+17].max=16;
		servoWriteMS(i+PIN_BASE1,16);			
		printf("Setze MIN Value (+/-/x)\n");
		key=0;
		while (key != 120) {		
			key=getchar();
			if (key==43) {
				Servo[i+17].min++;
				servoWriteMS(i+PIN_BASE1,Servo[i+17].min);
				printf("Servo %i min:%i\n",i,Servo[i+17].min);
			}
			if (key==45) {
				Servo[i+17].min--;
				servoWriteMS(i+PIN_BASE1,Servo[i+17].min);
				printf("Servo %i min:%i\n",i,Servo[i+17].min);
			}
		}
		printf("Setze MAX Value (+/-/x)\n");
		key=0;
		while (key != 120) {
			key=getchar();
			if (key==43) {
				Servo[i+17].max++;
			}
			if (key==45) {
				Servo[i+17].max--;
			}
			servoWriteMS(i+PIN_BASE1,Servo[i+17].max);
			printf("Servo %i max:%i\n",i,Servo[i+17].max);
		}
		printf("\nServoNr. :");
		scanf("%s",str1);
		Servo[i+17].num=atoi(str1);
		printf("\nServoNr.. :%d \n",Servo[i+17].num);
		servoWriteMS(i+PIN_BASE1,(Servo[i+17].min+Servo[i+17].max)/2);
	}
/////////////////////////////////Ausgabe
	printf("\n\nDatenerfassung beendet\n");
	printf("\nModul: %i\n",PIN_BASE0);
	for (i=0;i<=16;i++) printf("Servo Nr. %i: (%i) min :%i, max:%i \n",Servo[i].num,i,Servo[i].min,Servo[i].max);
	printf("\nModul: %i\n",PIN_BASE1);
	for (i=0;i<=16;i++) printf("Servo Nr. %i: (%i) min :%i, max:%i \n",Servo[i+17].num,i,Servo[i+17].min,Servo[i+17].max);
	fp = fopen("I2CServos.txt", "w");
	fprintf(fp,"\n// Modul: %i\n",PIN_BASE0);
		for (i=0;i<16;i++) {
			if (Servo[i].num<30) {
				fprintf(fp,"\n Servo[%i].pin = %i + PIN_BASE0; \n Servo[%i].min = %i; \n Servo[%i].max = %i; \n Servo[%i].neu = %i; \nServo[%i].alt = %i; \n",Servo[i].num,i,Servo[i].num,Servo[i].min,Servo[i].num,Servo[i].max,Servo[i].num,((Servo[i].min+Servo[i].max)/2),Servo[i].num,((Servo[i].min+Servo[i].max)/2));
			}
		}
		fprintf(fp,"\n// Modul: %i\n",PIN_BASE1);
		for (i=17;i<33;i++) {
			if (Servo[i].num<30) {
				fprintf(fp,"\n Servo[%i].pin = %i + PIN_BASE1; \n Servo[%i].min = %i; \n Servo[%i].max = %i; \n Servo[%i].neu = %i; \nServo[%i].alt = %i; \n",Servo[i].num,(i-17),Servo[i].num,Servo[i].min,Servo[i].num,Servo[i].max,Servo[i].num,((Servo[i].min+Servo[i].max)/2),Servo[i].num,((Servo[i].min+Servo[i].max)/2));
			}
		}
	fclose(fp);       
    return 0;
}

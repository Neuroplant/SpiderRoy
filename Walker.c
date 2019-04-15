// SpiderRoy
// V0.00.1
//gcc RemoteCarIC.c -o Remote -lwiringPi -lm -lpthread 

#include <string.h>
#include <pca9685.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define I2CDEVID0	0x40		//use "gpio i2cdetect" get Address
#define PIN_BASE0 	300
#define I2CDEVID1	0x41		//use "gpio i2cdetect" get Address
#define PIN_BASE1	320

#define MAX_PWM 	4095
#define HERTZ 		50

#define SLOMO 		80		// Bewegungsverzögerung

struct s_Servo {
    int pin;
    int min;
    int max;
    int alt;
    int neu;
	int current;
};
struct s_Servo Servo[21]; // = malloc(20 * sizeof *Servo);

pthread_t t_Servo[21];
pthread_t t_Display;
bool run=true;

int map(int value,int fromLow,int fromHigh,int toLow,int toHigh){
    return (toHigh-toLow)*(value-fromLow) / (fromHigh-fromLow) + toLow;
}
void LegPos (int leg, int input);

void *ServoThread (void *value) {
	int pwm;
	long idNr = (long)value;
	printf("Starte Servo %i\n",idNr);
	while (run) {

		if (Servo[idNr].alt < Servo[idNr].neu) {
			for	(Servo[idNr].current = Servo[idNr].alt;Servo[idNr].current < Servo[idNr].neu;Servo[idNr].current++) {
				pwmWrite(Servo[idNr].pin,map (Servo[idNr].current,0,200,0,MAX_PWM));
				delay(SLOMO);
				printf(" Servo(%2i): %2i < %2i < %2i \n",idNr,Servo[idNr].neu,Servo[idNr].current,Servo[idNr].neu);
			}
			Servo[idNr].current = Servo[idNr].alt = Servo[idNr].neu;
		}
		if (Servo[idNr].alt > Servo[idNr].neu) {
			for	(Servo[idNr].current = Servo[idNr].alt;Servo[idNr].current > Servo[idNr].neu;Servo[idNr].current--) {
				pwmWrite(Servo[idNr].pin,map (Servo[idNr].current,0,200,0,MAX_PWM));
				delay(SLOMO);
				printf(" Servo(%2i): %2i > %2i > %2i \n",idNr,Servo[idNr].neu,Servo[idNr].current,Servo[idNr].neu);
			}
			Servo[idNr].current = Servo[idNr].alt = Servo[idNr].neu ;
		}
    		pwmWrite(Servo[idNr].pin,map (Servo[idNr].current,0,200,0,MAX_PWM));
		delay(SLOMO);
	}
	pthread_exit(NULL);
}
void *DisplayThrad (void *value) {
	while(run) {
		system(clrscr);
		for (int i=0;i<=27;i++) {
			printf("\n");
			for (int j=0;j<21;j++) {
				if 	(Servo[j].min==i) {	printf("-"); }
				else {if Servo[j].max==i) {	printf("-"); }
				else {if Servo[j].neu==i) {	printf("x"); }
				else {if Servo[j].alt==i) {	printf("x"); }
				else {if Servo[j].current==i) {	printf("o"); }
				else {printf(" ");      
				}}}}}
			}
		}
	}
	pthread_exit(NULL);
}

void setupServos(void) {			
	int i;				//insert the content of I2CServos.txt here,
						//legs might break if you use these example values
#include "I2CServos.txt"
	
	Servo[2].pin = 2 + PIN_BASE1; 	//Spare, maybe for a LED-Frontlight, a Claw, lets see
	Servo[2].min = 5; 
 	Servo[2].max = 18; 
 	Servo[2].neu = 11; 
	Servo[2].alt = 11; 
	
	for (i=0;i<=20;i++) {
		pinMode(Servo[i].pin,OUTPUT);
		pthread_create(&t_Servo[i],NULL,ServoThread, (void*)i);
	}
	
}

void WaitFor(int value) {
    while (value != 0) delay(SLOMO);
    return;
}


int legmoveCompleted(int leg) {
	int i, count = 1;
	while (count != 0) {
		count = 0;
		for (i=0;i<=2;i++) {
			count = count + abs((Servo[leg*3+i].alt) - (Servo[leg*3+i].neu));
		}
		//printf(".");
		delay(SLOMO);
	}
	return count;
}
	
	
int allmoveCompleted(void) {
	int i, count = 1;
	while (count !=0) {
		count = 0;
		for (i=0;i<=6;i++) {
			count = count + legmoveCompleted(i);
		}
	}
	return count;
}

void LegPos (int leg, int input);

int move(int leg, int pos) ;

int main(int argc, char* argv[]) {
	//setup*****************************************************************
	int i,dancestep;
	// wiringPi
	if(wiringPiSetup() == -1){ 
        	printf("setup wiringPi faiservo !");
        	return 1; 
    	}
	wiringPiI2CSetup (I2CDEVID0);
	wiringPiI2CSetup (I2CDEVID1);
	int fd0 = pca9685Setup(PIN_BASE0, I2CDEVID0, HERTZ);
	if (fd0 < 0){
		printf("Error in setup\n");
		return fd0;
	}
	int fd1 = pca9685Setup(PIN_BASE1, I2CDEVID1, HERTZ);
	if (fd1 < 0){
		printf("Error in setup\n");
		return fd1;
	}
	pinMode(7,OUTPUT);
	digitalWrite(7,LOW);
	// Reset all output
	pca9685PWMReset(fd0);
	pca9685PWMReset(fd1);
	
	//struct s_Servo Servo[20];
	
	setupServos();
	pthread_create(&t_Display,NULL,DisplayThread, (void*)NULL);

	
	
			       
	
	//loop******************************************************************
	if (argc==1) {
		printf("\n no Arguments: Idle Dance\n");
		while (run){
			//stand up
	    		move(0,30);	    
	    		//Idle dance
	    		for (i=10;i<=14;i++){
				printf("Dance Move %i",i);
				move(0,i);
				
			}
		}
	}
	printf("\n%i\n",argc);
	//Parameter************************************************************
	for (i=1;i<argc;i++) {
		dancestep = atoi(argv[i]);
		printf(" %i/%i Dance Move %s(%i)\n",i,argc,argv[i],dancestep);
		move(0,dancestep);
	}
	
	//end******************************************************************
	
	allmoveCompleted();
	run=false;
	for (i=0;i<20;i++) {
		pthread_join(t_Servo[i],NULL);
		printf("Thread %i/20 closed\n",i);
	}
	return 0;
}
#include "LegPos.sub"
#include "move.sub"

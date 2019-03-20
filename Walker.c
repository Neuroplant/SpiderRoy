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

#define SLOMO 		200		// Bewegungsgeschwindigkeit

struct s_Servo {
    int pin;
    int min;
    int max;
    int alt;
    int neu;
};
struct s_Servo Servo[21]; // = malloc(20 * sizeof *Servo);
	pthread_t t_Servo[21];

bool run=true;

long map(long value,long fromLow,long fromHigh,long toLow,long toHigh){
    return (toHigh-toLow)*(value-fromLow) / (fromHigh-fromLow) + toLow;
}
void LegPos (int leg, int input);

void *ServoThread (void *value) {
	int pwm;
	long idNr = (long)value;
	printf("Starte Servo %i\n",idNr);
	while (run) {

	/*
		if (Servo[idNr].alt < Servo[idNr].neu) {
			for	(pwm = Servo[idNr].alt;pwm < Servo[idNr].neu;pwm++) {
				pwmWrite(Servo[idNr].pin,map (pwm,0,200,0,MAX_PWM));
				delay(SLOMO);
				printf(" Servo %i < %i \n",idNr,Servo[idNr].neu);
			}
			Servo[idNr].alt = Servo[idNr].neu;
		}
		if (Servo[idNr].alt > Servo[idNr].neu) {
			for	(pwm = Servo[idNr].alt;pwm > Servo[idNr].neu;pwm--) {
				pwmWrite(Servo[idNr].pin,map (pwm,0,200,0,MAX_PWM));
				delay(SLOMO);
				printf(" Servo %i > %i \n",idNr,Servo[idNr].neu);
			}
			Servo[idNr].alt = Servo[idNr].neu;
		}
	*/
    		pwmWrite(Servo[idNr].pin,map (Servo[idNr].neu,0,200,0,MAX_PWM));
		delay(SLOMO);
	}
	pthread_exit(NULL);
}

void setupServos(void) {			
	int i;				//insert the content of I2CServos.txt here,
						//legs might break if you use these example values
#inculude "I2CServos.txt"
	
	for (i=0;i<=20;i++) {
		pinMode(Servo[i].pin,OUTPUT);
		pthread_create(&t_Servo[i],NULL,ServoThread, (void*)i);
	}
	
}


void LegPos (int leg, int input) {
    	int joint[3];
	
	switch (input) {
		case 0 : // halte Position
		break;
		case 1 : // unten vorne stand
		joint[0] = 0;
		joint[1] = 0;
		joint[2] = 1;
		break;
		case 2 : // unten mitte stand
		joint[0] = 1;
		joint[1] = 0;
		joint[2] = 1;
		break;
		case 3 : // unten hinten stand
		joint[0] = 2;
		joint[1] = 0;
		joint[2] = 1;
		break;
		case 4 : // mitte vorne stand
		joint[0] = 0;
		joint[1] = 1;
		joint[2] = 1;
		break;
		case 5 : // mitte mitte stand
		joint[0] = 1;
		joint[1] = 1;
		joint[2] = 1;
		break;
		case 6 : // mitte hinten stand
		joint[0] = 2;
		joint[1] = 1;
		joint[2] = 1;
		break; 
		case 7 : // oben vorne stand
		joint[0] = 0;
		joint[1] = 2;
		joint[2] = 1;
		break;
		case 8 : // oben mitte stand
		joint[0] = 1;
		joint[1] = 2;
		joint[2] = 1;
		break;
		case 9 : // oben hinten stand
		joint[0] = 2;
		joint[1] = 2;
		joint[2] = 1;
		break;
		case 10 : // oben vorne gestreckt
		joint[0] = 0;
		joint[1] = 2;
		joint[2] = 0;
		break;
		case 11 : // oben mitte gestreckt
		joint[0] = 1;
		joint[1] = 2;
		joint[2] = 0;
		break;
		case 12 : // oben hinten gestreckt
		joint[0] = 2;
		joint[1] = 2;
		joint[2] = 0;
		break;
		case 13 : // unten vorne eingerollt
		joint[0] = 0;
		joint[1] = 0;
		joint[2] = 2;
		break;
		case 14 : // unten mitte eingerollt
		joint[0] = 1;
		joint[1] = 0;
		joint[2] = 2;
		break;
		case 15 : // unten hinten eingerollt
		joint[0] = 2;
		joint[1] = 0;
		joint[2] = 2;
		break;
		default :// halte Position
		break;
	}
	Servo[leg*3+0].neu 	=	map(joint[0],0,2,Servo[leg*3+0].min,Servo[leg*3+0].max);
	Servo[leg*3+1].neu 	=	map(joint[1],0,2,Servo[leg*3+1].min,Servo[leg*3+1].max);
	Servo[leg*3+2].neu 	=	map(joint[2],0,2,Servo[leg*3+2].min,Servo[leg*3+2].max);

	/*switch (joint[2]) {	//	change factor from 10 to adjust foot-segment
		case 0 :
			Servo[leg*3+2].neu 	=	map(joint[2]*10,0,20,Servo[leg*3+2].min,Servo[leg*3+2].max);
		break;
		case 1 :
			Servo[leg*3+2].neu 	=	map(joint[2]*10,0,20,Servo[leg*3+2].min,Servo[leg*3+2].max);
		break;
		case 2 :
			Servo[leg*3+2].neu 	=	map(joint[2]*10,0,20,Servo[leg*3+2].min,Servo[leg*3+2].max);
		break;
		
	}*/
}

int move(int leg, int pos) {
	int i;
	
	switch (pos) {
	case 0 :	
		
	break;
	case 1 :	//setze fuß vor
		LegPos(leg, 8);
		LegPos(leg, 7);
		LegPos(leg, 4);
	break;
	case 2 :	//setze fuß zurück
		LegPos(leg, 8);
		LegPos(leg, 9);
		LegPos(leg, 6);
	break;
	case 3 :	//ziehe fuß zur mitte
		LegPos(leg, 5);
	break;
	case 4 :	//hebe Fuß
		LegPos(leg, 8);
	break;
	case 5 :
		move(6,4);
		delay(SLOMO);
		move(1,3);
		move(4,3);
		move(5,3);
	break;
	case 6 :	//b drehe rechts
		move(2,2);
		move(3,1);
		move(6,2);
		move(1,4);
		move(4,4);
		move(5,4);
		delay(SLOMO);
		move(2,3);
		move(3,3);
		move(6,3);
	break;
	case 7 :	//a drehe links
		move(1,2);
		move(4,1);
		move(5,2);
		move(2,4);
		move(3,4);
		move(6,4);
		delay(SLOMO);
		move(1,3);
		move(4,3);
		move(5,3);
	break;
	case 8 :	//b drehe links
		move(2,1);
		move(3,2);
		move(6,1);
		move(1,4);
		move(4,4);
		move(5,4);
		delay(SLOMO);
		move(2,3);
		move(3,3);
		move(6,3);
	break;
	//Idle/Dance Moves
	
	case 10 :	//a stomp
		for (i=1;i<=6;i++){ 
	        	LegPos(i,11);
		delay(SLOMO);
	        	LegPos(i,5);
		delay(SLOMO);
	    	}
	break;
	case 11:	//b stomp
		for (i=6;i<=1;i--){ 
	        	LegPos(i,11);
		delay(SLOMO);
	        	LegPos(i,5);
		delay(SLOMO);
	    	}
	break;
	case 12: 	//Räkeln
		for (i=6;i<=1;i--){ 
		        LegPos(i,14);
		delay(SLOMO);
			LegPos(i,10);
		delay(SLOMO);
			LegPos(i,12);
		delay(SLOMO);
		        LegPos(i,5);
		delay(SLOMO);
		}
	break;
	case 13 :	//schunkeln
		move(0,30);
		LegPos(1,0);
		LegPos(2,1);
		LegPos(3,1);
		LegPos(4,1);
		LegPos(5,1);
		LegPos(6,2);
		delay(SLOMO);
		LegPos(1,1);
		LegPos(2,2);
		LegPos(3,1);
		LegPos(4,1);
		LegPos(5,0);
		LegPos(6,1);
		delay(SLOMO);
		LegPos(1,1);
		LegPos(2,1);
		LegPos(3,2);
		LegPos(4,0);
		LegPos(5,1);
		LegPos(6,1);
		delay(SLOMO);
		LegPos(1,2);
		LegPos(2,1);
		LegPos(3,1);
		LegPos(4,1);
		LegPos(5,1);
		LegPos(6,0);
		delay(SLOMO);
		LegPos(1,1);
		LegPos(2,2);
		LegPos(3,1);
		LegPos(4,1);
		LegPos(5,0);
		LegPos(6,1);
		delay(SLOMO);
		LegPos(1,1);
		LegPos(2,1);
		LegPos(3,0);
		LegPos(4,2);
		LegPos(5,1);
		LegPos(6,1);
		delay(SLOMO);
	break;
	//Head moves
	
	case 20 :	//Center
		LegPos(0,5);
		delay(SLOMO);
	break;
	case 21 :	//Nod 
		LegPos(0,8);
		delay(SLOMO);
		LegPos(0,5);
		delay(SLOMO);
	break;
	case 22 :	//Shake 
		LegPos(0,6);
		delay(SLOMO);
		LegPos(0,7);
		delay(SLOMO);
		LegPos(0,5);
		delay(SLOMO);
	break;
	case 23 :	//Sniff 
	break;
	case 24 :	//Center 
	break;
	case 25 :	//Center 
	break;
	case 30 :	//breitbeinig stellen
		move(1,1);
		move(2,1);
		move(3,5);
		move(4,5);
		move(5,2);
		move(6,2);
		delay(SLOMO*6);
	break;
	case 31 :	//a Schritt vor
		move(1,1);
		move(4,1);
		move(5,1);
		move(2,4);
		move(3,4);
		move(6,4);
		delay(SLOMO*6);
		move(1,3);
		move(4,3);
		move(5,3);
	break;
	case 32 :	//b Schritt vor
		move(2,1);
		move(3,1);
		move(6,1);
		move(1,4);
		move(4,4);
		move(5,4);
		delay(SLOMO*6);
		move(2,3);
		move(3,3);
		move(6,3);
	break;
	case 33 :	//a schritt zurück
		move(1,2);
		move(4,2);
		move(5,2);
		move(2,4);
		move(3,4);
		move(6,4);
		delay(SLOMO*6);
		move(1,3);
		move(4,3);
		move(5,3);
	break;
	case 34 :	//b schritt zurück
		move(2,2);
		move(3,2);
		move(6,2);
		move(1,4);
		move(4,4);
		move(5,4);
		delay(SLOMO*6);
		move(2,3);
		move(3,3);
		move(6,3);
	break;
	case 35 :	//a drehe rechts
		move(1,1);
		move(4,2);
		move(5,1);
		move(2,4);
		move(3,4);
		move(6,4);
		delay(SLOMO*6);
		move(2,3);
		move(3,3);
		move(6,3);
	case 36	:	//b drehe rechts
	break;
	case 37 :	//a drehe links
	break;
	case 38 :	//b drehe links
	break;
	default:
	break;
	}
	return 0;
}

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

	
	
			       
	
	//loop******************************************************************
	if (argc==1) {
		printf("\n no Arguments: Idle Dance\n");
		while (run){
			//stand up
	    		move(0,30);	    
	    		//Idle dance
	    		for (i=10;i<=13;i++){
				printf("Dance Move %i",i);
				move(0,i);
				
			}
		}
	}
	printf("\n%i\n",argc);
	//Parameter************************************************************
	for (i=1;i<=argc;i++) {
		dancestep = atoi(argv[i]);
		printf(" %i/%i Dance Move %s(%i)\n",i,argc,argv[i],dancestep);
		move(0,dancestep);
	}
	//end******************************************************************
	run=false;
	for (i=0;i<20;i++) {
		pthread_join(t_Servo[i],NULL);
		printf("Thread %i/20 closed\n",i);
	}
	return 0;
}

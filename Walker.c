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
};
struct s_Servo Servo[21]; // = malloc(20 * sizeof *Servo);
	pthread_t t_Servo[21];

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
			for	(pwm = Servo[idNr].alt;pwm < Servo[idNr].neu;pwm++) {
				pwmWrite(Servo[idNr].pin,map (pwm,0,200,0,MAX_PWM));
				delay(SLOMO);
				printf(" Servo(%2i): %2i < %2i < %2i \n",idNr,Servo[idNr].neu,pwm,Servo[idNr].neu);
			}
			Servo[idNr].alt = Servo[idNr].neu;
		}
		if (Servo[idNr].alt > Servo[idNr].neu) {
			for	(pwm = Servo[idNr].alt;pwm > Servo[idNr].neu;pwm--) {
				pwmWrite(Servo[idNr].pin,map (pwm,0,200,0,MAX_PWM));
				delay(SLOMO);
				printf(" Servo(%2i): %2i > %2i > %2i \n",idNr,Servo[idNr].neu,pwm,Servo[idNr].neu);
			}
			Servo[idNr].alt = Servo[idNr].neu;
		}
    		pwmWrite(Servo[idNr].pin,map (Servo[idNr].neu,0,200,0,MAX_PWM));
		delay(SLOMO);
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

void LegPos (int leg, int input) {
    	int joint[3];
	
	switch (input) {
		case 0 : // Neutral
			joint[0] = 1;
			joint[1] = 1;
			joint[2] = 1;
		break;
		case 1 : // Vorne Unten Stand 
		joint[0] = 0;
		joint[1] = 0;
		joint[2] = 1;
		break;
		case 2 : // Mitte Unten Stand 
		joint[0] = 1;
		joint[1] = 0;
		joint[2] = 1;
		break;
		case 3 : // Hinten Unten Stand 
		joint[0] = 2;
		joint[1] = 0;
		joint[2] = 1;
		break;
		case 4 : // Vorne Mitte Stand 
		joint[0] = 0;
		joint[1] = 1;
		joint[2] = 1;
		break;
		case 5 : // Mitte Mitte Stand (Neutral)
		joint[0] = 1;
		joint[1] = 1;
		joint[2] = 1;
		break;
		case 6 : // Hinten Mitte Stand 
		joint[0] = 2;
		joint[1] = 1;
		joint[2] = 1;
		break; 
		case 7 : // Vorne Hoch Stand 
		joint[0] = 0;
		joint[1] = 2;
		joint[2] = 1;
		break;
		case 8 : // Mitte Hoch Stand 
		joint[0] = 1;
		joint[1] = 2;
		joint[2] = 1;
		break;
		case 9 : // Hinten Hoch Stand 
		joint[0] = 2;
		joint[1] = 2;
		joint[2] = 1;
		break;
		case 10 : // Vorne Hoch Rein 
		joint[0] = 0;
		joint[1] = 2;
		joint[2] = 0;
		break;
		case 11 : // Mitte Hoch Rein 
		joint[0] = 1;
		joint[1] = 2;
		joint[2] = 0;
		break;
		case 12 : // Hinten Hoch Rein 
		joint[0] = 2;
		joint[1] = 2;
		joint[2] = 0;
		break;
		case 13 : // Vorne Unten Raus 
		joint[0] = 0;
		joint[1] = 0;
		joint[2] = 2;
		break;
		case 14 : // Mitte Unten Raus 
		joint[0] = 1;
		joint[1] = 0;
		joint[2] = 2;
		break;
		case 15 : // Hinten Unten Raus 
		joint[0] = 2;
		joint[1] = 0;
		joint[2] = 2;
		break;
		case 16 : // Vorne Oben Raus 
		joint[0] = 0;
		joint[1] = 2;
		joint[2] = 2;
		break;
		case 17 : // Mitte Oben Raus 
		joint[0] = 1;
		joint[1] = 2;
		joint[2] = 2;
		break;
		case 18 : // Hinten Oben Raus 
		joint[0] = 2;
		joint[1] = 2;
		joint[2] = 2;
		break;
		case 19 : // Vorne Unten Rein 
		joint[0] = 0;
		joint[1] = 0;
		joint[2] = 0;
		break;
		case 20 : // Mitte Unten Rein  
		joint[0] = 1;
		joint[1] = 0;
		joint[2] = 0;
		break;
		case 21 : // Hinten Unten Rein  
		joint[0] = 2;
		joint[1] = 0;
		joint[2] = 0;
		break;
		default :// Neutral
			joint[0] = 1;
			joint[1] = 1;
			joint[2] = 1;
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
			legmoveCompleted(leg);
		LegPos(leg, 11);
			legmoveCompleted(leg);
		LegPos(leg, 10);
			legmoveCompleted(leg);
		LegPos(leg, 4);
	break;
	case 2 :	//setze Fuß zur Mitte
			legmoveCompleted(leg);
		LegPos(leg, 11);
			legmoveCompleted(leg);
		LegPos(leg, 5);
	break;
	case 3 :	//setze fuß zurück
			legmoveCompleted(leg);
		LegPos(leg, 11);
			legmoveCompleted(leg);
		LegPos(leg, 12);
			legmoveCompleted(leg);
		LegPos(leg, 6);
	break;
	case 4 :	//ziehe fuß vor
		legmoveCompleted(leg);
	LegPos(leg, 4);
	break;
	case 5 :	//ziehe fuß zur mitte
		legmoveCompleted(leg);
	LegPos(leg, 5);
	break;
	case 6 :	//ziehe fuß zurück
		legmoveCompleted(leg);
	LegPos(leg, 6);
	break;
	
	
	//Idle/Dance Moves
	
	case 10 :	//a stomp
	        LegPos(1,11);
	        	legmoveCompleted(1);
	        LegPos(1,5);
	        	legmoveCompleted(1);
			
			LegPos(3,11);
	        	legmoveCompleted(3);
	        LegPos(3,5);
	        	legmoveCompleted(3);
			
			LegPos(5,11);
	        	legmoveCompleted(5);
	        LegPos(5,5);
				legmoveCompleted(5);
			
			LegPos(6,11);
	        	legmoveCompleted(6);
	        LegPos(6,5);
				legmoveCompleted(6);
			
			LegPos(4,11);
	        	legmoveCompleted(4);
	        LegPos(4,5);
	        	legmoveCompleted(4);
			
			LegPos(2,11);
	        	legmoveCompleted(2);
	        LegPos(2,5);
	        	legmoveCompleted(2);
	break;
	case 11:	//b stomp
	        	LegPos(2,11);
	        	legmoveCompleted(2);
	        	LegPos(2,5);
	        	legmoveCompleted(2);
			LegPos(4,11);
	        	legmoveCompleted(4);
	        	LegPos(4,5);
	        	legmoveCompleted(4);
			LegPos(6,11);
	        	legmoveCompleted(6);
	        	LegPos(6,5);
	        	legmoveCompleted(6);
			LegPos(5,11);
	        	legmoveCompleted(5);
	        	LegPos(5,5);
	        	legmoveCompleted(5);
			LegPos(3,11);
	        	legmoveCompleted(3);
	        	LegPos(3,5);
	        	legmoveCompleted(3);
			LegPos(1,11);
	        	legmoveCompleted(1);
	        	LegPos(1,5);
	        	legmoveCompleted(1);
	break;
	case 12: 	//Räkeln
		for (i=6;i>=0;i--){ 
		printf("Räkeln Bein %i",i);
			LegPos(i,11);
		    legmoveCompleted(i);
			LegPos(i,20);
			legmoveCompleted(i);
			LegPos(i,11);
			legmoveCompleted(i);
		    LegPos(i,16);
		    legmoveCompleted(i);
			LegPos(i,18);
		    legmoveCompleted(i);
			LegPos(i,5);
		    legmoveCompleted(i);
		}
	break;
	case 13 :	//schunkeln
		LegPos(1,11);
		LegPos(2,5);
		LegPos(3,5);
		LegPos(4,5);
		LegPos(5,5);
		LegPos(6,14);
		allmoveCompleted();
		LegPos(1,5);
		LegPos(2,14);
		LegPos(3,5);
		LegPos(4,5);
		LegPos(5,11);
		LegPos(6,5);
		allmoveCompleted();
		LegPos(1,5);
		LegPos(2,5);
		LegPos(3,14);
		LegPos(4,11);
		LegPos(5,5);
		LegPos(6,5);
		allmoveCompleted();
		LegPos(1,14);
		LegPos(2,5);
		LegPos(3,5);
		LegPos(4,5);
		LegPos(5,5);
		LegPos(6,11);
		allmoveCompleted();
		LegPos(1,5);
		LegPos(2,14);
		LegPos(3,5);
		LegPos(4,5);
		LegPos(5,11);
		LegPos(6,5);
		allmoveCompleted();
		LegPos(1,5);
		LegPos(2,5);
		LegPos(3,11);
		LegPos(4,14);
		LegPos(5,5);
		LegPos(6,5);
		allmoveCompleted();
	break;
	case 14 :	//Jump
		for (int i=1;i<=6;i++) {
			LegPos(i,11);
		}
			allmoveCompleted();
//		for (int i=1;i<=6;i++) {
//			LegPos(i,14);
//		}
//		allmoveCompleted();
		for (int i=1;i<=6;i++) {
			LegPos(i,5);
		}
		allmoveCompleted();
	break;
	case 15 : //start position
	for (int i =1; i <= 6 ;i++) {
		LegPos(i,11);
		legmoveCompleted(i);
		LegPos(i,5);
	}
	allmoveCompleted();
	break;
	//Head moves
	
	case 20 :	//Center
		LegPos(0,5);
		legmoveCompleted(0);
	break;
	case 21 :	//Nod 
		legmoveCompleted(0);
		LegPos(0,8);
		legmoveCompleted(0);
		LegPos(0,2);
		legmoveCompleted(0);
		LegPos(0,5);
	break;
	case 22 :	//Shake 
		legmoveCompleted(0);
		LegPos(0,6);
		legmoveCompleted(0);
		LegPos(0,4);
		legmoveCompleted(0);
		LegPos(0,5);
	break;
	case 23 :	//Sniff 
	break;
	case 24 :	//Center 
	break;
	case 25 :	//Center 
	break;
	case 30 :	//breitbeinig stellen
		move(1,5);
		move(2,5);
		move(3,5);
		move(4,5);
		move(5,5);
		move(6,5);
		allmoveCompleted();
	break;
	case 31 :	//a Schritt vor
		allmoveCompleted();
		move(1,1);
		move(4,1);
		move(5,1);
		allmoveCompleted();
		LegPos(2,11);
		LegPos(3,11);
		LegPos(6,11);
		allmoveCompleted();
		move(1,6);
		move(4,6);
		move(5,6);
	break;
	case 32 :	//b Schritt vor
		allmoveCompleted();
		move(2,1);
		move(3,1);
		move(6,1);
		allmoveCompleted();
		LegPos(1,11);
		LegPos(4,11);
		LegPos(5,11);
		allmoveCompleted();
		move(2,6);
		move(3,6);
		move(6,6);
	break;
	case 33 :	//a schritt zurück
		allmoveCompleted();
		move(1,3);
		move(4,3);
		move(5,3);
		allmoveCompleted();
		LegPos(2,11);
		LegPos(3,11);
		LegPos(6,11);
		allmoveCompleted();
		move(1,4);
		move(4,4);
		move(5,4);
	break;
	case 34 :	//b schritt zurück
		allmoveCompleted();
		move(2,3);
		move(3,3);
		move(6,3);
		allmoveCompleted();
		LegPos(1,11);
		LegPos(4,11);
		LegPos(5,11);
		allmoveCompleted();
		move(2,4);
		move(3,4);
		move(6,4);
	break;
	case 35 :	//a drehe rechts
		allmoveCompleted();
		move(1,1);
		move(4,3);
		move(5,1);
		allmoveCompleted();
		LegPos(2,11);
		LegPos(3,11);
		LegPos(6,11);
		allmoveCompleted();
		move(1,6);
		move(4,4);
		move(5,6);
	break;
	case 36 :	//b drehe rechts
		allmoveCompleted();
		move(2,3);
		move(3,1);
		move(6,3);
		allmoveCompleted();
		LegPos(1,11);
		LegPos(4,11);
		LegPos(5,11);
		allmoveCompleted();
		move(2,4);
		move(3,6);
		move(6,4);
	break;
	case 37 :	//a drehe links
		allmoveCompleted();
		move(1,3);
		move(4,1);
		move(5,3);
		allmoveCompleted();
		LegPos(2,11);
		LegPos(3,11);
		LegPos(6,11);
		allmoveCompleted();
		move(1,4);
		move(4,6);
		move(5,4);
	break;
	case 38 :	//b drehe links
		allmoveCompleted();
		move(2,1);
		move(3,3);
		move(6,1);
		allmoveCompleted();
		LegPos(1,11);
		LegPos(4,11);
		LegPos(5,11);
		allmoveCompleted();
		move(2,6);
		move(3,4);
		move(6,6);
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

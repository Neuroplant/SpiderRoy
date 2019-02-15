// SpiderRoy
// V0.00.1
//gcc RemoteCarIC.c -o Remote -lwiringPi -lm -lpthread 
//(will create a makefile as soon as I figure out how)

#include <string.h>
#include "pca9685.h"
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define I2CDEVID0	0x40		//use "gpio i2cdetect" get Address
#define PIN_BASE0 	300
#define I2CDEVID1	0x41		//use "gpio i2cdetect" get Address
#define PIN_BASE1	320

#define MAX_PWM 	4096
#define HERTZ 		50

#define SLOWMO 		100		// Bewegungsgeschwindigkeit

struct s_Servo {
    int pin;
    int min;
    int max;
    int alt;
    int neu;
}
struct s_Servo Servo[20];
pthread_t t_Servo[20];

int run=0;
long map(long value,long fromLow,long fromHigh,long toLow,long toHigh){
    return (toHigh-toLow)*(value-fromLow) / (fromHigh-fromLow) + toLow;
}

void setupServos() {			//take a look at "https://github.com/Reinbert/pca9685/blob/master/examples/calibrate.c"
	int i;				//to get all the data for this section
	
	Servo[0].pin	=	;
	Servo[1].pin	=	;
	Servo[2].pin	=	;
	Servo[3].pin	=	;
	Servo[4].pin	=	;
	Servo[5].pin	=	;
	Servo[6].pin	=	;
	Servo[7].pin	=	;
	Servo[8].pin	=	;
	Servo[9].pin	=	;
	Servo[10].pin	=	;
	Servo[11].pin	=	;
	Servo[12].pin	=	;
	Servo[13].pin	=	;
	Servo[14].pin	=	;
	Servo[15].pin	=	;
	Servo[16].pin	=	;
	Servo[17].pin	=	;
	Servo[18].pin	=	;
	Servo[19].pin	=	;
	
	Servo[0].min	=	;
	Servo[0].max	=	;
	Servo[1].min	=	;
	Servo[1].max	=	;
	Servo[2].min	=	;
	Servo[2].max	=	;
	Servo[3].min	=	;
	Servo[3].max	=	;
	Servo[4].min	=	;
	Servo[4].max	=	;
	Servo[5].min	=	;
	Servo[5].max	=	;
	Servo[6].min	=	;
	Servo[6].max	=	;
	Servo[7].min	=	;
	Servo[7].max	=	;
	Servo[8].min	=	;
	Servo[8].max	=	;
	Servo[9].min	=	;
	Servo[9].max	=	;
	Servo[10].min	=	;
	Servo[10].max	=	;
	Servo[11].min	=	;
	Servo[11].max	=	;
	Servo[12].min	=	;
	Servo[12].max	=	;
	Servo[13].min	=	;
	Servo[13].max	=	;
	Servo[14].min	=	;
	Servo[14].max	=	;
	Servo[15].min	=	;
	Servo[15].max	=	;
	Servo[16].min	=	;
	Servo[16].max	=	;
	Servo[17].min	=	;
	Servo[17].max	=	;
	Servo[18].min	=	;
	Servo[18].max	=	;
	Servo[19].min	=	;
	Servo[19].max	=	;
	
	for (i=1;i<=6;i++) {
	beinPos(i,5);
	}
	
}

void WaitFor(int input) {
    while (input != 0) delay(SLOMO);
    return NULL;
}

*void ServoThread (void *arg) {
	int pwm;
	pthread_t id = pthread_self();
	int idNr = 0;
	while (not pthread_equal(id,ServoNr[idNr]) idNr++;	//find idNr of this task
	
	
	while (run==0) {

		if (Servo[idNr].alt < Servo[idNr].neu) {
			for	(pwm = Servo[idNr].alt;pwm < Servo[idNr].neu;pwm++) {
				WritePWM(Servo[idNr].pin,pwm);
				delay(SLOWMO);
			}
		}
		if (Servo[idNr].alt > Servo[idNr].neu) {
			for	(pwm = Servo[idNr].alt;pwm > Servo[idNr].neu;pwm--) {
				WritePWM(Servo[idNr].pin,pwm);
				delay(SLOWMO);
			}
		}
	   	Servo[idNr].alt=Servo[idNr].neu;
    		WritePWM(Servo[idNr].pin,Servo[idNr].neu);
	}
	return NULL;
}

int legmoveCompleted(int leg) {
   int i, count = 0;
    for (i=0;i<=2;i++) {
        count = count + abs(ServoPos[i,0]) - abs(ServoPos[i,1]);
    }
    return count;
}

int allmoveCompleted() {
    int i, count = 0;
    for (i=0;i<=7;i++) {
        count = count + legmoveCompleted(i);
    }
    return count;
}

void BeinPos(int bein, int input) {
    int joint1,joint2,joint3;
	switch (input) {
		case : 0 // halte Position
		break;
		case : 1 // unten vorne stand
		joint1 = 0;
		joint2 = 0;
		joint3 = 1;
		break;
		case : 2 // unten mitte stand
		joint1 = 1;
		joint2 = 0;
		joint3 = 1;
		break;
		case : 3 // unten hinten stand
		joint1 = 2;
		joint2 = 0;
		joint3 = 1;
		break;
		case : 4 // mitte vorne stand
		joint1 = 0;
		joint2 = 1;
		joint3 = 1;
		break;
		case : 5 // mitte mitte stand
		joint1 = 1;
		joint2 = 1;
		joint3 = 1;
		break;
		case : 6 // mitte hinten stand
		joint0 = 2;
		joint1 = 1;
		joint2 = 1;
		break; 
		case : 7 // oben vorne stand
		joint0 = 0;
		joint1 = 2;
		joint2 = 1;
		break;
		case : 8 // oben mitte stand
		joint0 = 1;
		joint1 = 2;
		joint2 = 1;
		break;
		case : 9 // oben hinten stand
		joint0 = 2;
		joint1 = 2
		joint2 = 1;
		break;
		case : 10 // oben vorne gestreckt
		joint0 = 0;
		joint1 = 2;
		joint2 = 0;
		break;
		case : 11 // oben mitte gestreckt
		joint0 = 1;
		joint1 = 2;
		joint2 = 0;
		break;
		case : 12 // oben hinten gestreckt
		joint0 = 2;
		joint1 = 2;
		joint2 = 0;
		break;
		case : 13 // unten vorne eingerollt
		joint0 = 0;
		joint1 = 0;
		joint2 = 2;
		break;
		case : 14 // unten mitte eingerollt
		joint0 = 1;
		joint1 = 0;
		joint2 = 2;
		break;
		case : 15 // unten hinten eingerollt
		joint0 = 2;
		joint1 = 0;
		joint2 = 2;
		break;
		default // halte Position
		break;
		
	}
	Servo[bein*3+0].neu =	map(joint0,0,2,Servo[bein*3+0].min,Servo[bein*3+0].max);
	Servo[bein*3+1].neu =   map(joint1,0,2,Servo[bein*3+1].min,Servo[bein*3+1].max);
	Servo[bein*3+2].neu =   map(joint2,0,2,Servo[bein*3+2].min,Servo[bein*3+2].max);
}

int move(int leg, int pos) {
	int i;
	
	switch (pos) {
	case 0 :	
		
	break;
	case 1 :	//setze fuß vor
		beinPos(leg, 8);
		beinPos(leg, 7);
		beinPos(leg, 4);
	break;
	case 2 :	//setze fuß zurück
		beinPos(leg, 8);
		beinPos(leg, 9);
		beinPos(leg, 6);
	break;
	case 3 :	//ziehe fuß zur mitte
		beinPos(leg, 5);
	break;
	case 4 :	//hebe Fuß
		beinPos(leg, 8);
	break;
	
	//Idle/Dance Moves
	
	case 10 :	//a stomp
		for (i=1;i<=6;i++){ 
	        	BeinPos(i,11);
	        	WaitFor(legmoveCompleted(i));
	        	BeinPos(i,5);
	        	WaitFor(legmoveCompleted(i));
	    	}
	break;
	case 11:	//b stomp
		for (i=6;i<=1;i--){ 
	        	BeinPos(i,11);
	        	WaitFor(legmoveCompleted(i));
	        	BeinPos(i,5);
	        	WaitFor(legmoveCompleted(i));
	    	}
	break;
	case 12: 	//Räkeln
		for (i=6;i<=1;i--){ 
		        BeinPos(i,14);
		        WaitFor(legmoveCompleted(i));
			BeinPos(i,10);
			WaitFor(legmoveCompleted(i));
			BeinPos(i,12);
			WaitFor(legmoveCompleted(i));
		        BeinPos(i,5);
		        WaitFor(legmoveCompleted(i));
		}
	break;
	case 12 :	//schunkeln
		Step(0);
		BeinPos(1,0);
		BeinPos(2,1);
		BeinPos(3,1);
		BeinPos(4,1);
		BeinPos(5,1);
		BeinPos(6,2);
		WaitFor(allmoveCompleted);
		BeinPos(1,1);
		BeinPos(2,2);
		BeinPos(3,1);
		BeinPos(4,1);
		BeinPos(5,0);
		BeinPos(6,1);
		WaitFor(allmoveCompleted);
		BeinPos(1,1);
		BeinPos(2,1);
		BeinPos(3,2);
		BeinPos(4,0);
		BeinPos(5,1);
		BeinPos(6,1);
		WaitFor(allmoveCompleted);
		BeinPos(1,2);
		BeinPos(2,1);
		BeinPos(3,1);
		BeinPos(4,1);
		BeinPos(5,1);
		BeinPos(6,0);
		WaitFor(allmoveCompleted);
		BeinPos(1,1);
		BeinPos(2,2);
		BeinPos(3,1);
		BeinPos(4,1);
		BeinPos(5,0);
		BeinPos(6,1);
		WaitFor(allmoveCompleted);
		BeinPos(1,1);
		BeinPos(2,1);
		BeinPos(3,0);
		BeinPos(4,2);
		BeinPos(5,1);
		BeinPos(6,1);
		WaitFor(allmoveCompleted);
	break;
	default :
	}
	return 0;
}

int step (int dir) {
	switch (dir) {
		case 0 :	//breitbeinig stellen
			move(1,1);
			move(2,1);
			move(3,5);
			move(4,5);
			move(5,2);
			move(6,2);
			WaitFor(allmoveCompleted);
		break;
		case 1 :	//a Schritt vor
			move(1,1);
			move(4,1);
			move(5,1);
			move(2,4);
			move(3,4);
			move(6,4);
			WaitFor(allmoveCompleted);
			move(1,3);
			move(4,3);
			move(5,3);
		break;
		case 2 :	//b Schritt vor
			move(2,1);
			move(3,1);
			move(6,1);
			move(1,4);
			move(4,4);
			move(5,4);
			WaitFor(allmoveCompleted);
			move(2,3);
			move(3,3);
			move(6,3);
		break;
		case 3 :	//a schritt zurück
			move(1,2);
			move(4,2);
			move(5,2);
			move(2,4);
			move(3,4);
			move(6,4);
			WaitFor(allmoveCompleted);
			move(1,3);
			move(4,3);
			move(5,3);
		break;
		case 4 :	//b schritt zurück
			move(2,2);
			move(3,2);
			move(6,2);
			move(1,4);
			move(4,4);
			move(5,4);
			WaitFor(allmoveCompleted);
			move(2,3);
			move(3,3);
			move(6,3);
		break;
		case 5 :	//a drehe rechts
			move(1,1);
			move(4,2);
			move(5,1);
			move(2,4);
			move(3,4);
			move(6,4);
			WaitFor(allmoveCompleted);
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
			WaitFor(allmoveCompleted);
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
			WaitFor(allmoveCompleted);
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
			WaitFor(allmoveCompleted);
			move(2,3);
			move(3,3);
			move(6,3);
		break;
		default:
	}
	return 0;
}

void main() {
	//setup
	int i;
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
	// Reset all output
	pca9685PWMReset(fd0);
	pca9685PWMReset(fd1);
	
	setupServos();
	for (i=0;i<20;i++) {
		pthread_create(&(t_Servo[i],NULL,&ServoThread,NULL);
	}
		
	//loop
	while (run==0){
	    //stand up
	    step(0);	    
	    //Idle dance
	    for (i=10;i<=13;i++) step(i);
	    
	    
	}
}

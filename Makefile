
LDFLAGS = -lwiringPi -lm -lpthread -lwiringPiPca9685

Spider: Walker.c

	gcc Walker.c -o Walker $(LDFLAGS)

ServoInit:
	gcc servoinit.c -o ServoInit $(LDFLAGS)
	
ServoData:
	gcc servodata.c -o ServoData $(LDFLAGS)


LDFLAGS = -lwiringPi -lm -lpthread -lwiringPiPca9685

Spider: Walker.c

	gcc Walker.c -o Walker $(LDFLAGS)

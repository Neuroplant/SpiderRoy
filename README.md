SpiderRoy
self made Hexapod
#	Hardware
	1	RaspberryPI
	2	16 Channel PWM board
	20 	Microservo
	1	Pappelsperrholz A3 3mm
	Power Supply 5V (more than 2A!) e.g. 
	2 	18650 Akku
	1	Batteryholder 
	1	Downstepper (5V 12A)

#	ServoNr		BeinNr		JointNr

	0		0		0
	1		0		1
	2		0		2	(not used, maybe for a clamp-upgrade some time)
	3		1		0
	4		1		1
	5		1		2
	6		2		0
	7		2		1
	8		2		2
	9		3		0
	10		3		1
	11		3		2
	12		4		0
	13		4		1
	14		4		2
	15		5		0
	16		5		1
	17		5		2
	18		6		0
	19		6		1
	20		6		2
	
Die	Nummer eines Servos ist somit = BeinNr*3 + JointNr
	
#	Anordnung der Beine:
	
			0     (Kopf)
		1		2   (Vorne links, Vorne Rechts)
		3		4   (...)
		5		6   (...)
		
#	JointSchema:
		01      
			2
  0 - Schulter  vor/zurück
  1 - Schulter  hoch/runter
  2 - Knie      rein/raus

# Install:

	Prerequisites:
	http://wiringpi.com/download-and-install/
	...
	gcc Walker.c -o Walker -lwiringPi -lm -lpthread
	
# Usage:
	
	sudo ./Walker [command1, command2, ..]
	commands:
	10	Stomp CW
	11	Stomp CCW
	12	Sprawl
	13	Sway
	20	Center head
	21	Nod head
	22	Shake head
	30	put legs apart
	31	one stepp forward a
	32	one stepp forward b
	33	one stepp back a
	34	one stepp back b
	35	turn right a
	36	turn right b
	37	turn left a
	38	turn left b

# Beinhaltung (LegPos)
		  0   ->   Neutral
		  
		  1   ->   Vorne Unten Stand 
		  2   ->   Mitte Unten Stand 
		  3   ->   Hinten Unten Stand 
		  
		  4   ->   Vorne Mitte Stand 
		  5   ->   Mitte Mitte Stand (Neutral)
		  6   ->   Hinten Mitte Stand 
		  
		  7   ->   Vorne Hoch Stand 
		  8   ->   Mitte Hoch Stand 
		  9   ->   Hinten Hoch Stand 
		 
		 10   ->   Vorne Hoch Rein 
		 11   ->   Mitte Hoch Rein 
		 12   ->   Hinten Hoch Rein 
		 
		 13   ->   Vorne Unten Raus 
		 14   ->   Mitte Unten Raus 
		 15   ->   Hinten Unten Raus 
		 
		 16   ->   Vorne Oben Raus 
		 17   ->   Mitte Oben Raus 
		 18   ->   Hinten Oben Raus 
		 
		 19   ->   Vorne Unten Rein 
		 20   ->   Mitte Unten Rein  
		 21   ->   Hinten Unten Rein  

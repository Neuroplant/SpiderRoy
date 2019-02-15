I## SpiderRoy
self made Hexapod

#	ServoNr		BeinNr		JointNr

	0		0		0
	1		0		1
	2		0		2
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
  2 . Knie      rein/raus

# Install
Prerequisites:
	https://github.com/Reinbert/pca9685
	...

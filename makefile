all: moveOne.o servo.o serial.o
	g++ moveOne.o servo.o serial.o -o run

moveOne: moveOne.cpp
	g++ -c moveOne.cpp

servo: servo.cpp
	g++ -c servo.cpp

serial: serial.cpp
	g++ -c serial.cpp

clean:
	rm *o run



all: main.o servo.o serial.o
	g++ main.o servo.o serial.o -o run

main: main.cpp
	g++ -c main.cpp

servo: servo.cpp
	g++ -c servo.cpp

serial: serial.cpp
	g++ -c serial.cpp

clean:
	rm *o run



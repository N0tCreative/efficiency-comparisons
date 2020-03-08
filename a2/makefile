CC = gcc
CF = -Wall -std=c11 -g

all: p11 p12 p21 p22

p11: p11.c
	$(CC) $(CF) -c p11.c -o p11.o 
	$(CC) $(CF) -o p11 p11.o 
	rm p11.o

p12: p12.c
	$(CC) $(CF) -c p12.c -o p12.o -lm
	$(CC) $(CF) -o p12 p12.o -lm
	rm p12.o

p21: p21.c
	$(CC) $(CF) -c p21.c -o p21.o -lm
	$(CC) $(CF) -o p21 p21.o -lm
	rm p21.o

p22: p22.c
	$(CC) $(CF) -c p22.c -o p22.o -lm
	$(CC) $(CF) -o p22 p22.o -lm
	rm p22.o


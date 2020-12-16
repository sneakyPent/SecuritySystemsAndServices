CC = gcc
DBUG = -g
CCFLAGS = -O2 -Wall -pedantic 	

TARGET = monitor

FILESPATH = ../files/
OUTPUTFILESPATH = ../myResults/
TESTPATH = ../test/


all: monitor

monitor: monitor.c
	$(CC) $(CFLAGS) $(DBUG) monitor.c -o monitor -lpcap

clean:
	rm -f $(TARGET) *.o

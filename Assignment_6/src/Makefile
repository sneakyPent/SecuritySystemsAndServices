CC = gcc
DBUG = -g
CCFLAGS = -O2 -Wall -pedantic 	
OBJFILES = utils.o

TARGET = monitor

FILESPATH = ../files/
OUTPUTFILESPATH = ../myResults/
TESTPATH = ../test/


all: monitor

monitor: monitor.c $(OBJFILES) 
	$(CC) $(CFLAGS) $(DBUG) monitor.c -o monitor $(OBJFILES) -lpcap

clean:
	rm -f $(TARGET) *.o

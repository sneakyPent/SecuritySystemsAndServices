CC = gcc
DBUG = -g
CCFLAGS = -O2 -Wall -pedantic 	
OBJFILES = main.o rsa.o utils.o

TARGET = assign_3

TUCAM=2016030073

FILESPATH = ../files/
OUTPUTFILESPATH = ../files/
TESTPATH = ../test/
OUTPUTFILESPATH = $(TESTPATH)

TASKD1FILE = hpy414_encryptme_pub.txt
TASKD2FILE = hpy414_decryptme_pub.txt
TASKD3FILE = hpy414_encryptme_priv.txt
TASKD4FILE = hpy414_decryptme_priv.txt

TASKD1KEY = hpy414_public.key
TASKD2KEY = hpy414_public.key
TASKD3KEY = hpy414_private.key
TASKD4KEY = hpy414_priv.key

TASKD1OUT = $(TUCAM)_encrypted_pub.txt
TASKD2OUT = $(TUCAM)_decrypted_pub.txt
TASKD3OUT = $(TUCAM)_encrypted_priv.txt
TASKD4OUT = $(TUCAM)_decrypted_priv.txt

all: $(TARGET) 

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) $(DBUG) -o $(TARGET) -lm $(OBJFILES) 

taskA:
	./$(TARGET) -g $(GD) 
taskB:

taskC:

taskD1:
	./$(TARGET) $(GD) -i  $(FILESPATH)$(TASKD1FILE) -o $(OUTPUTFILESPATH)$(TASKD1OUT) -k  $(FILESPATH)$(TASKD1KEY) -e
taskD2:
	./$(TARGET) $(GD) -i  $(FILESPATH)$(TASKD2FILE) -o $(OUTPUTFILESPATH)$(TASKD2OUT) -k  $(FILESPATH)$(TASKD2KEY) -d
taskD3:
	./$(TARGET) $(GD) -i  $(FILESPATH)$(TASKD3FILE) -o $(OUTPUTFILESPATH)$(TASKD3OUT) -k  $(FILESPATH)$(TASKD3KEY) -e
taskD4:
	./$(TARGET) $(GD) -i  $(FILESPATH)$(TASKD4FILE) -o $(OUTPUTFILESPATH)$(TASKD4OUT) -k  $(FILESPATH)$(TASKD4KEY) -d

taskD: taskD1 taskD2 taskD3 taskD4

clean:
	rm -f $(TARGET) *.o
	rm -rf $(TESTPATH)*

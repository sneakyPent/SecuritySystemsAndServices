CC = gcc
DBUG = -g
CCFLAGS = -O2 -Wall -pedantic 	
OBJFILES = main.o rsa.o utils.o

TARGET = assign_3

TUCAM=TUC2016030073

FILESPATH = ../files/
OUTPUTFILESPATH = ../myResults/
TESTPATH = ../test/

TASKAFILE = lorem.txt
TASKBFILE = lorem_encrypted.txt
TASKCFILE = lorem_decrypted.txt

TASKD1FILE = hpy414_encryptme_pub.txt
TASKD2FILE = hpy414_decryptme_pub.txt
TASKD3FILE = hpy414_encryptme_priv.txt
TASKD4FILE = hpy414_decryptme_priv.txt

PUBLICKEY = hpy414_public.key
PRIVATEKEY = hpy414_private.key

TASKD1OUT = $(TUCAM)_encrypted_pub.txt
TASKD2OUT = $(TUCAM)_decrypted_pub.txt
TASKD3OUT = $(TUCAM)_encrypted_priv.txt
TASKD4OUT = $(TUCAM)_decrypted_priv.txt

all: $(TARGET) 

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) $(DBUG) -o $(TARGET) $(OBJFILES) -lm 

taskA:
	rm -rf $(TESTPATH)*
	./$(TARGET) -g $(GD)
taskB:
	./$(TARGET) -e $(GD) -i  $(FILESPATH)$(TASKAFILE) -o $(TESTPATH)$(TASKBFILE) -k $(TESTPATH)$(PUBLICKEY)
taskC:
	./$(TARGET) -d $(GD) -i  $(TESTPATH)$(TASKBFILE) -o $(TESTPATH)$(TASKCFILE) -k $(TESTPATH)$(PRIVATEKEY)
taskD1:
	./$(TARGET) $(GD) -i  $(FILESPATH)$(TASKD1FILE) -o $(OUTPUTFILESPATH)$(TASKD1OUT) -k  $(FILESPATH)$(PUBLICKEY) -e
taskD2:
	./$(TARGET) $(GD) -i  $(FILESPATH)$(TASKD2FILE) -o $(OUTPUTFILESPATH)$(TASKD2OUT) -k  $(FILESPATH)$(PUBLICKEY) -d
taskD3:
	./$(TARGET) $(GD) -i  $(FILESPATH)$(TASKD3FILE) -o $(OUTPUTFILESPATH)$(TASKD3OUT) -k  $(FILESPATH)$(PRIVATEKEY) -e
taskD4:
	./$(TARGET) $(GD) -i  $(FILESPATH)$(TASKD4FILE) -o $(OUTPUTFILESPATH)$(TASKD4OUT) -k  $(FILESPATH)$(PRIVATEKEY) -d

taskD: taskD1 taskD2 taskD3 taskD4

clean:
	rm -f $(TARGET) *.o
	rm -rf $(TESTPATH)*
	rm -rf $(OUTPUTFILESPATH)*

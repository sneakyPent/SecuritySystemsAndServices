CC = gcc
DBUG = -g
CCFLAGS = -O2 -Wall -pedantic
LIBSSL = -lssl -lcrypto

TARGETS = assign_1

FILESPATH = ../files/
OUTPUTFILESPATH = ../files/
RESULTSPATH = ../myResults/

TASKAFILE = encryptme_256.txt
TASKBFILE = hpy414_decryptme_128.txt
TASKCFILE = signme_128.txt
TASKDFILE1 = hpy414_verifyme_128.txt
TASKDFILE2 = hpy414_verifyme_256.txt

TASKAOUT = decryptme_256.txt
TASKBOUT = hpy414_encryptme_128.txt
TASKCOUT = verifyme_128.txt

PASSWORD = TUC2016030073
GIVENPASS = hpy414

# -------------------------TEST FILES----------------------
TESTFILESPATH = ../testFiles/
TESTFILE1 = lorem.txt
TESTFILE2 = lorem-10k-en256.txt
TESTFILE3 = verify-me-256b.txt
TESTFILEOUT1 = test1.txt
TESTFILEOUT2 = test2.txt
TESTFILEOUT3 = test3.txt
TESTPASS = otinanai


all: $(TARGETS)

assign_1: assign_1.c
	$(CC) $(CCFLAGS) $(DBUG) -o $@ $< $(LIBSSL)

taskA:
	./assign_1 $(GD) -b 256 -p $(PASSWORD) -i $(FILESPATH)$(TASKAFILE) -o $(OUTPUTFILESPATH)$(TASKAOUT) -e

taskB:
	./assign_1 $(GD) -b 128 -p $(GIVENPASS) -i $(FILESPATH)$(TASKBFILE) -o $(OUTPUTFILESPATH)$(TASKBOUT) -d

taskC:
	./assign_1 $(GD) -b 128 -p $(PASSWORD) -i $(FILESPATH)$(TASKCFILE) -o $(OUTPUTFILESPATH)$(TASKCOUT) -s

taskD:
	./assign_1 $(GD) -b 128 -p $(GIVENPASS) -i $(FILESPATH)$(TASKDFILE1) -o o128.txt -v
	./assign_1 $(GD) -b 256 -p $(GIVENPASS) -i $(FILESPATH)$(TASKDFILE2) -o o256.txt -v
test:
	./assign_1 $(GD) -b 256 -p $(TESTPASS) -i $(TESTFILESPATH)$(TESTFILE1) -o $(TESTFILESPATH)$(TESTFILEOUT1) -e
	./assign_1 $(GD) -b 256 -p $(TESTPASS) -i $(TESTFILESPATH)$(TESTFILE2) -o $(TESTFILESPATH)$(TESTFILEOUT2) -d
	./assign_1 $(GD) -b 256 -p $(TESTPASS) -i $(TESTFILESPATH)$(TESTFILE3) -o $(TESTFILESPATH)$(TESTFILEOUT3) -v

tasks: taskA taskB taskC taskD 

clean:
	rm -f $(TARGETS) 
	rm -f $(OUTPUTFILESPATH)$(TASKAOUT)
	rm -f $(OUTPUTFILESPATH)$(TASKBOUT)
	rm -f $(OUTPUTFILESPATH)$(TASKCOUT)
	rm -f $(TESTFILESPATH)$(TESTFILEOUT1)	
	rm -f $(TESTFILESPATH)$(TESTFILEOUT2)
	rm -f $(TESTFILESPATH)$(TESTFILEOUT3)

compare:
	diff $(FILESPATH)$(TASKAOUT) $(RESULTSPATH)$(TASKAOUT)
	diff $(FILESPATH)$(TASKBOUT) $(RESULTSPATH)$(TASKBOUT)
	diff $(FILESPATH)$(TASKCOUT) $(RESULTSPATH)$(TASKCOUT)
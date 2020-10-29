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

tasks: taskA taskB taskC taskD 

clean:
	rm -f $(TARGETS) 
	rm -f $(OUTPUTFILESPATH)$(TASKAOUT)
	rm -f $(OUTPUTFILESPATH)$(TASKBOUT)
	rm -f $(OUTPUTFILESPATH)$(TASKCOUT)

compare:
	diff $(FILESPATH)$(TASKAOUT) $(RESULTSPATH)$(TASKAOUT)
	diff $(FILESPATH)$(TASKBOUT) $(RESULTSPATH)$(TASKBOUT)
	diff $(FILESPATH)$(TASKCOUT) $(RESULTSPATH)$(TASKCOUT)
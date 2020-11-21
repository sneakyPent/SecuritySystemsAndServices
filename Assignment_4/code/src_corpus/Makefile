SHELL := /bin/bash
NON_ACCESSIBLE_FILES_NUM = 8
ACCESSIBLE_FILES_NUM = 5
TESTFILESDIR = 
NON_ACCESSIBLE_FILES = $(TESTFILESDIR)FILE_N_
ACCESSIBLE_FILES = $(TESTFILESDIR)FILE_

CC = gcc
DBUG = -g
CCFLAGS =  -Wall -fPIC -shared


TARGETS = logger acmonitor test_aclog
OBJFILES = utils.o
LIBRARY = logger.so


all: $(TARGETS)

logger: logger.c $(OBJFILES)
	$(CC) $(CCFLAGS) -o logger.so logger.c -lcrypto -ldl $(OBJFILES)

acmonitor: acmonitor.c $(OBJFILES)
	$(CC) acmonitor.c -o acmonitor $(OBJFILES)

test_aclog: test_aclog.c $(OBJFILES)
	$(CC) test_aclog.c -o test_aclog $(OBJFILES)

createTestingFiles:
	file=1 ; while [[ $$file -le $(NON_ACCESSIBLE_FILES_NUM) ]] ; do \
        touch $(NON_ACCESSIBLE_FILES)$$file.txt; \
        ((file = file + 1)) ; \
    done
	file=1 ; while [[ $$file -le $(ACCESSIBLE_FILES_NUM) ]] ; do \
        touch $(ACCESSIBLE_FILES)$$file.txt; \
        ((file = file + 1)) ; \
    done
	chmod 000 $(NON_ACCESSIBLE_FILES)*

run: logger.so test_aclog
	LD_PRELOAD=./logger.so ./test_aclog

test: all createTestingFiles  run step2_2 step2_1

step2_1:
	./acmonitor -m

step2_2:
	file=1 ; while [[ $$file -le $(ACCESSIBLE_FILES_NUM) ]] ; do \
		echo "./acmonitor -i $(ACCESSIBLE_FILES)$$file.txt"; \
		./acmonitor -i $(ACCESSIBLE_FILES)$$file.txt; \
        ((file = file + 1)) ; \
    done
	file=1 ; while [[ $$file -le $(NON_ACCESSIBLE_FILES_NUM) ]] ; do \
		echo "./acmonitor -i $(NON_ACCESSIBLE_FILES)$$file.txt"; \
        ./acmonitor -i $(NON_ACCESSIBLE_FILES)$$file.txt; \
        ((file = file + 1)) ; \
    done

cleanObjects:
	rm -rf logger.so
	rm -rf test_aclog
	rm -rf acmonitor
	rm -rf *.o
		
cleanLogs: 
	rm -rf ../logs/file_logging.log

cleanFiles:
	rm -rf $(NON_ACCESSIBLE_FILES)*
	rm -rf $(ACCESSIBLE_FILES)*

clean: cleanObjects cleanLogs cleanFiles

NON_ACCESSIBLE_FILES_NUM = 8
ACCESSIBLE_FILES_NUM = 3
TESTFILESDIR = 
NON_ACCESSIBLE_FILES = $(TESTFILESDIR)FILE_N_
ACCESSIBLE_FILES = $(TESTFILESDIR)FILE_

CC = gcc
DBUG = -g
CCFLAGS =  -Wall -fPIC -shared


TARGETS = logger acmonitor test_aclog createNonAccessibleFiles 
OBJFILES = utils.o
LIBRARY = logger.so


all: $(TARGETS)

logger: logger.c $(OBJFILES)
	$(CC) $(CCFLAGS) -o logger.so logger.c -lcrypto -ldl $(OBJFILES)

acmonitor: acmonitor.c $(OBJFILES)
	$(CC) acmonitor.c -o acmonitor $(OBJFILES)

test_aclog: test_aclog.c $(OBJFILES)
	$(CC) test_aclog.c -o test_aclog $(OBJFILES)

run: logger.so test_aclog
	LD_PRELOAD=./logger.so ./test_aclog

test: all run checkFiles checkMalicious

clean: cleanObjects cleanLogs cleanFiles

cleanObjects:
	rm -rf logger.so
	rm -rf test_aclog
	rm -rf acmonitor
	rm -rf *.o
		
cleanLogs: 
	rm -rf ../logs/file_logging.log

cleanFiles:
	rm -f /lnx/sneaky/test.txt
	rm -f /home/zn/Desktop/test.txt
	rm -rf file_*
	rm -rf $(NON_ACCESSIBLE_FILES)*
	rm -rf $(ACCESSIBLE_FILES)*

createNonAccessibleFiles:
	file=1 ; while [[ $$file -le $(NON_ACCESSIBLE_FILES_NUM) ]] ; do \
        touch $(NON_ACCESSIBLE_FILES)$$file.txt; \
        ((file = file + 1)) ; \
    done
	file=1 ; while [[ $$file -le $(ACCESSIBLE_FILES_NUM) ]] ; do \
        touch $(ACCESSIBLE_FILES)$$file.txt; \
        ((file = file + 1)) ; \
    done
	chmod 000 $(NON_ACCESSIBLE_FILES)*

checkFiles:
	./acmonitor -i $(ACCESSIBLE_FILES)1.txt;
	./acmonitor -i $(ACCESSIBLE_FILES)2.txt;
	./acmonitor -i $(ACCESSIBLE_FILES)3.txt;
	./acmonitor -i $(NON_ACCESSIBLE_FILES)1.txt;
	./acmonitor -i $(NON_ACCESSIBLE_FILES)2.txt;
	./acmonitor -i $(NON_ACCESSIBLE_FILES)3.txt;
	./acmonitor -i $(NON_ACCESSIBLE_FILES)4.txt;
	./acmonitor -i $(NON_ACCESSIBLE_FILES)5.txt;
	./acmonitor -i $(NON_ACCESSIBLE_FILES)6.txt;
	./acmonitor -i $(NON_ACCESSIBLE_FILES)7.txt;
	./acmonitor -i $(NON_ACCESSIBLE_FILES)8.txt;

checkMalicious:
	./acmonitor -m
DEBUG = false
CC = gcc

CFLAGS = -g -O2 -c

# Header files
HEADER = simple_crypto.h

# Executables
CRYPTO = simple_crypto
DEMO = demo

# Source files
SOURCE_CRYPTO = simple_crypto.c 
SOURCE_DEMO = demo.c

SOURCE = $(SOURCE_CRYPTO)

# Object files
OBJS_CRYPTO = $(SOURCE_CRYPTO:.c=.o)
OBJS_DEMO= $(SOURCE_DEMO:.c=.o)
OBJS = $(OBJS_CRYPTO) $(OBJS_DEMO)


# Help menu
USAGE = Usage: make [OPTION]
DESCRIPTION = 
HELPMENU = $(USAGE)\n$(DESCRIPTION)\nOPTIONS:



# ----------------------------------------------------
# 					Basic options

all: crypto run 

crypto:  $(OBJS) ## Create Crypto executable
	 $(CC) -o $(DEMO) $?


clean:  ## Clean Object files and executables
	$(RM) $(OBJS) $(CRYPTO) 
	$(RM) output.*.*

help:	## Print this help message and exit
	@echo "$(HELPMENU)" ; grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "   %-30s %s\n", $$1, $$2}'

count:	## Count number of lines
	wc -l $(SOURCE) $(HEADER) $(MAKEFILE_LIST)

run: # run simple crypto executable
	./$(DEMO)

# ----------------------------------------------------
#					Object files

CRYPTO.o: CRYPTO.c
	$(CC) $(CFLAGS) $?

DEMO.o: DEMO.c
	$(CC) $(CFLAGS) $?	
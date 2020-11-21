# Security System and Services

4rd Assignment: Development of an access control logging system using the C programming language.

## Informations

* Every part of the assignment is completed and works properly according to the given details of the assignment.
* If you want any specification for the implemented functions, you can find them in the header files given as function declerations.
* I have created a utils file for some extra functions.

## test_aclog.c

Tries to read, write and append to 3 of 5 created files from "make createTestingFiles" in which user does have permissions and to 8 created files from "make createTestingFiles" in which user does not have permissions.

## BEFORE MAKE

If you want to run and test my assignment i recommend you to run "make test".  
This compiles the library, creates 5 files with rwx user's permissions and 8 files with no user's permissions (---). Afterwards,it runs test executable with the above mentioned actions and in the last step it runs the acmonitor executable file for malicious users and acmonitor executable for all the files created on "make createTestingFiles". Lastly, you can run "make test" as sudo without "make clean" so as to check that only the first user is the malicious user.

## Usage

```bash
make                            # Compile the library and create the executables.
make logger                     # Compile the logger library.
make acmonitor                  # Create the acmonitor executable.
make test_aclog                 # Create the test_aclog executable.
make run                        # Run the test_aclog executable and preload logger library
make test                       # make all createNonAccessibleFiles run step2_1 tep2_2
make step2_1                    # Run A.C.L.M tool for malicious users.
make step2_2                    # Run A.C.L.M tool for tracking and reporting users for all the files cerated.
make clean                      # Remove every file created after make.
make cleanObjects               # Remove the object files.
make cleanLogs                  # Remove the log file.
make cleanFiles                 # Remove the files created for testing.
make createTestingFiles         # Create some files for testing.
```

## INFO FOR TASKS' FILES

* Log file is located in logs directory.

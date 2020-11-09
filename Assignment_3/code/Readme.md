# Security System and Services

3rd Assignment: Development of an asymmetric encryption tool in C, from scratch

## Informations

Every part of the assignment is completed and works properly according to the given details of the assignment.

If you want any specification for the implemented functions, you can find them in the header file (assign_1.h) given as function declerations.

## Usage

```bash
make                # Compile the library and creates executable assign_3.
make clean          # Clean Object files and executable demo.
make taskA          # Run the taskA
make taskB          # Run the taskB with my added file lorem (580K) and the taskA public key.
make taskC          # Run the taskC with the encrypted file lorem (580K) and the taskA private key.
make tasksD         # Run the whole TASK D with the given files and keys.
make taskD1         # Run the taskD1 Encrypt "hpy414_encryptme_pub.txt" using the hpy414_public.key.
make taskD2         # Run the taskD2 Decrypt “hpy414_decryptme_pub.txt” using the hpy414_public.key.
make taskD3         # Run the taskD3 Encrypt “hpy414_encryptme_priv.txt” using the hpy414_private.key.
make taskD4         # Run the taskD4 Decrypt “hpy414_decryptme_priv.txt” using the hpy414_private.key.
```

## INFO FOR TASKS' FILES

* Files for taskD are located in myResults directory.
* Lorem file (580K) for taskB is located in files directory.
* The output files for task A,B,C are located in test directory.

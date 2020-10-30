
# Security System and Services

2nd Assignment: Development of a symmetric encryption tool in C, using the
OpenSSL toolkit

## Informations

Every part of the assignment is completed and works properly according to the given details of the assignment.

If you want any specification for the implemented functions, you can find them in the header file (assign_1.h) given as function declerations.

## Usage

```bash
make                # Compile the library and creates executable assign_1.
make clean          # Clean Object files and executable demo.
make taskA          # Run the first task of TASK F
make taskB          # Run the second task of TASK F
make taskC          # Run the third task of TASK F
make taskD          # Run the fourth task of TASK F
make tasks          # Run the whole TASK F
```

## TASK F

Files for 1,2 and 3 are located in myResults directory.

4.1 File hpy414_verifyme_128.txt was read succesfully but NOT verified.

4.2 File hpy414_verifyme_256.txt was read succesfully but NOT verified.

## More important EVP and CMACfunctions used in the exercice

* ***EVP_get_digestbyname***: Used to get mesage digest for sha1.
* ***EVP_get_cipherbyname***: Used to get appropriate cipher depend on bit mode (aes-128-ecb aes-256-ecb).
* ***EVP_BytesToKey***: Used to derive the key from the given password and message digest with iteration 1.
* ***EVP_CIPHER_CTX_new***: Used to create the cipher EVP_CIPHER_CTX, will be used in EVP_EncryptInit and decrypt methods.
* ***EVP_EncryptInit_ex***: Used to set up cipher context for encryption.
* ***EVP_DecryptInit_ex***: Used to set up cipher context for decryption.
* ***EVP_EncryptUpdate***: Used to encrypt inl bytes from the buffer in and writes the encrypted version to out.
* ***EVP_EncryptFinal_ex***: Used to encrypt the "final" data, that is any data that remains in a partial block, because of the enabled padding in EVP_EncryptUpdate().
* ***EVP_DecryptUpdate***: Used to decrypt inl bytes from the buffer in and writes the decrypted version to out.
* ***EVP_DecryptFinal_ex***: Used to decrypt the "final" data, that is any data that remains in a partial block, because of the enabled padding in EVP_DecryptUpdate().
* ***CMAC_CTX_new***: Used to create the CMAC_CTX, will be used in CMAC_Init.
* ***CMAC_Init***: Used to set up the CMAC_CTX for gen_cmac.
* ***CMAC_Update***: Used to get the gen cmac from the given context, initialized by given key and bit mode.
* ***CMAC_Final***: Used to decrypt the "final" data after using CMAC_Update.
* ***EVP_CIPHER_CTX_free***: Used to clears all information from a cipher context and free up any allocated memory associate with it.


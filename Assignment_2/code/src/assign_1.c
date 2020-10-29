//
// Created by zn on 10/26/20.
//

#include "assign_1.h"

void print_hex(unsigned char *data, size_t len)
{
    size_t i;

    if (!data)
        printf("NULL data\n");
    else
    {
        for (i = 0; i < len; i++)
        {
            if (!(i % 16) && (i != 0))
                printf("\n");
            printf("%02X ", data[i]);
        }
        printf("\n");
    }
}

void print_string(unsigned char *data, size_t len)
{
    size_t i;

    if (!data)
        printf("NULL data\n");
    else
    {
        for (i = 0; i < len; i++)
            printf("%c", data[i]);
        printf("\n");
    }
}

void usage(void)
{
    printf(
        "\n"
        "Usage:\n"
        "    assign_1 -i in_file -o out_file -p passwd -b bits"
        " [-d | -e | -s | -v]\n"
        "    assign_1 -h\n");
    printf(
        "\n"
        "Options:\n"
        " -i    path    Path to input file\n"
        " -o    path    Path to output file\n"
        " -p    psswd   Password for key generation\n"
        " -b    bits    Bit mode (128 or 256 only)\n"
        " -d            Decrypt input and store results to output\n"
        " -e            Encrypt input and store results to output\n"
        " -s            Encrypt+sign input and store results to output\n"
        " -v            Decrypt+verify input and store results to output\n"
        " -h            This help message\n");
    exit(EXIT_FAILURE);
}

void check_args(char *input_file, char *output_file, unsigned char *password,
                int bit_mode, int op_mode)
{
    if (!input_file)
    {
        print(" No input file!", error);
        usage();
    }

    if (!output_file)
    {
        print("No output file!", error);
        usage();
    }

    if (!password)
    {
        print("No user key!", error);
        usage();
    }

    if ((bit_mode != 128) && (bit_mode != 256))
    {
        char msg[50];
        sprintf(msg, "Bit Mode <%d> is invalid!", bit_mode);
        print(msg, error);
        usage();
    }

    if (op_mode == -1)
    {
        print("Error: No mode", error);
        usage();
    }
}

/** ----------------------------------------------------------------------------------------------------------------- */

void keygen(unsigned char *password, unsigned char *key, int bit_mode)
{
    const EVP_MD *md = NULL;
    const EVP_CIPHER *cipher = NULL;
    char *hash_function_name = "sha1";
    md = EVP_get_digestbyname(hash_function_name);
    if (!md)
    {
        char msg[50] = "Unknown message digest ";
        strcat(msg, hash_function_name);
        print(msg, error);
        exit(1);
    }
    cipher = getAesCipher(bit_mode);
    if (!cipher)
        return;
    if (!EVP_BytesToKey(cipher, md, NULL, password, strlen((const char *)password), 1, key, NULL))
    {
        print("EVP_BytesToKey failure", error);
        exit(1);
    }
    printf("Key generated successfully.Generated key:\n");
    print_hex(key, bit_mode);
}

void encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *ciphertext, int bit_mode)
{
   
    EVP_CIPHER_CTX *context;
    int len, ciphertext_len;
    context = contextInit(key, bit_mode, 'e');
      
    /*Encrypt inl bytes from the buffer in and writes the encrypted version to out.*/
    if (EVP_EncryptUpdate(context, ciphertext, &len, plaintext, plaintext_len) != 1)
    {
        print("EVP_EncryptUpdate failure", error);
        exit(1);
    }
    ciphertext_len = len;
    /* Finalize the encryption. Further ciphertext bytes may be written at this stage.*/
    if (EVP_EncryptFinal_ex(context, ciphertext + len, &len) != 1)
    {
        print( "EVP_EncryptFinal_ex failure", error);
        exit(1);
    }
    ciphertext_len += len;
    
    /* Print message as hex*/
    printf("Encrypted message:\n");
    print_hex(ciphertext, ciphertext_len);

    /* Clean up*/
    EVP_CIPHER_CTX_free(context);
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *plaintext, int bit_mode)
{
    int plaintext_len,len;

    plaintext_len = 0;
    
    EVP_CIPHER_CTX *context=NULL;
    context = contextInit(key, bit_mode, 'd');
    if (EVP_DecryptUpdate(context, plaintext, &len, ciphertext, ciphertext_len) != 1)
    {
        print("EVP_DecryptUpdate failure", error);
        exit(1);
    }
    plaintext_len = len;
    if (EVP_DecryptFinal_ex(context, plaintext + len, &len) != 1)
    {
        print( "EVP_DecryptFinal_ex failure", error);
        exit(1);
    }
    plaintext_len += len;
    
    /* Print message as hex*/
    printf("Decrypted message:\n");
    print_string(plaintext, plaintext_len);
    EVP_CIPHER_CTX_free(context);

    return plaintext_len;
}

void gen_cmac(unsigned char *data, size_t data_len, unsigned char *key,
              unsigned char *cmac, int bit_mode)
{
    CMAC_CTX *context;
    context = cmacContextInit(key,bit_mode);
    size_t cmacLen;

    if (1!=CMAC_Update(context, data, data_len))
    {
        print("CMAC_Update failure", error);
        exit(1);
    }

    if (1!=CMAC_Final(context, cmac, &cmacLen))
    {
        print("CMAC_Final failure", error);
        exit(1);
    }
    printf("CMAC generated successfully.Generated CMAC:\n");
    print_hex(cmac, cmacLen);
    CMAC_CTX_free(context);
}

/*
 * Verifies a CMAC
 */
int verify_cmac(unsigned char *cmac1, unsigned char *cmac2)
{
    int verify;

    verify = 0;

    if (memcmp((const char *)cmac1, (const char *)cmac2, BLOCK_SIZE) == 0)
    {
        printf("File verified.\n");
        verify = 1;
    }
    else
    {
        printf("File not verified.\n");
        verify = 0;
    }

    return verify;
}

/* TODO Develop your functions here... */
void print(char *str, enum mode md)
{
    if (debug == 1)
    {
        switch (md)
        {
        case error:
            printf("\033[1;31m");
            printf("[ERROR]: \033[0m %s\n", str);
            break;
        case info:
            printf("\033[0;36m");
            printf("[INFO]: \033[0m %s\n", str);
            break;
        case success:
            printf("\033[0;32m");
            printf("[SUCCESS]: \033[0m %s\n", str);
            break;
        default:
            printf("%s\n", str);
            break;
        }
    }
}

EVP_CIPHER_CTX *contextInit( unsigned char *key, int bit_mode, char mode)
{

    const EVP_CIPHER *cipher = NULL;
    cipher = getAesCipher(bit_mode);
    EVP_CIPHER_CTX *context;

    if (!(context = EVP_CIPHER_CTX_new()))
    {
        print("EVP_CIPHER_CTX_new failure!", error);
        exit(1);
    }

    if ( mode== 'e'){
        if (1 != EVP_EncryptInit(context, cipher, key, NULL))
            {
                print("EVP_EncryptInit failure!", error);
                exit(1);
            }
    }
    else if (mode=='d'){
        if (1 != EVP_DecryptInit(context, cipher, key, NULL))
        {
            print("EVP_DecryptInit failure!", error);
            exit(1);
        }
    }
    else
    {
        print("Not valid mode for contextInit functions.", error);
    }
   
    return context;
}

const EVP_CIPHER *getAesCipher(int bit_mode)
{
    const EVP_CIPHER *aes_cipher = NULL;
    char *hash_function_name = NULL;
    if (bit_mode == 128)
    {
        hash_function_name = "aes-128-ecb";
    }
    else if (bit_mode == 256)
    {
        hash_function_name = "aes-256-ecb";
    }
    aes_cipher = EVP_get_cipherbyname(hash_function_name);
    if (!aes_cipher)
    {
        print("EVP_get_cipherbyname failure!", error);
        exit(1);
    }
    return aes_cipher;
}

void fileManager(char *fileName, char *mode, unsigned char *plaintext, long *plaintextLength)
{

   
    FILE *fp=NULL;
    if (strcmp(mode, "cmac") != 0){
        print("Opening file.", info);
        fp= fopen(fileName, mode);
        if (fp == NULL)
        {
            char msg[50] = "Cannot find file ";
            strcat(msg, fileName);
            print(msg, error);
            exit(1);
        }
    }
    if (strcmp(mode, "r") == 0)
    {
        long flen;

        /*Calculate the size of the plaintext*/
        fseek(fp, 0L, SEEK_END);
        flen = ftell(fp);
        fseek(fp, 0L, SEEK_SET);
        plaintext = realloc(plaintext, sizeof(char)*flen);
        /* allocate memory for plaintext storing*/
        /*read plaintext*/
        print("Reading file...", info);
        long readlength = fread(plaintext, sizeof(char), flen, fp);
        if (readlength != flen)
        {
            print("Reading error",error);
            exit(1);
        }
        *plaintextLength = flen;
        print("Read file.", success);

    }
    else if (strcmp(mode, "w") == 0)
    {
        long writelength = fwrite(plaintext, sizeof(char), *plaintextLength, fp);
        if (writelength != *plaintextLength)
        {
            print("writing error",error);
            exit(1);
        }
        print("write file.", success);
    }
    else if (strcmp(mode, "a") == 0)
    {
        long writelength = fwrite(plaintext, sizeof(char), *plaintextLength, fp);
        if (writelength != *plaintextLength)
        {
            print("writing error",error);
            exit(1);
        }
        print("write file.", success);
    }
    else if (strcmp(mode, "cmac") == 0)
    {   
        print("Opening file.", info);
        fp = fopen(fileName, "r");
        if (fp == NULL)
        {
            char msg[50] = "Cannot find file ";
            strcat(msg, fileName);
            print(msg, error);
            exit(1);
        }
        long flen=BLOCK_SIZE;

        /*Calculate the size of the plaintext*/
        fseek(fp,-BLOCK_SIZE, SEEK_END);
    
        plaintext = realloc(plaintext, sizeof(char)*flen);
        /* allocate memory for plaintext storing*/
        /*read plaintext*/
        print("Reading file...", info);
        long readlength = fread(plaintext, sizeof(char), flen, fp);
        if (readlength != flen)
        {
            print("Reading error",error);
            exit(1);
        }
        *plaintextLength = flen;
        print("Read file.", success);
    }
    else
    {
        print("Give valid File manager mode (write->wb  or  read->rb)", error);
    }
    print("Closing File...", info);
    fclose(fp);
    print("File closed.", info);
}


CMAC_CTX *cmacContextInit( unsigned char *key, int bit_mode)
{

    const EVP_CIPHER *cipher = NULL;
    cipher = getAesCipher(bit_mode);
    CMAC_CTX *context;

    if (!(context = CMAC_CTX_new()))
    {
        print("CMAC_CTX_new failure!", error);
        exit(1);
    }
    if (1 != CMAC_Init(context,key,BLOCK_SIZE*(bit_mode/128), cipher, NULL))
    {
        print("CMAC_Init failure!", error);
        exit(1);
    }
    return context;
}


/*
 * Encrypts the input file and stores the ciphertext to the output file
 *
 * Decrypts the input file and stores the plaintext to the output file
 *
 * Encrypts and signs the input file and stores the ciphertext concatenated with
 * the CMAC to the output file
 *
 * Decrypts and verifies the input file and stores the plaintext to the output
 * file
 */
int main(int argc, char **argv)
{
    int opt;                 /* used for command line arguments */
    int bit_mode;            /* defines the key-size 128 or 256 */
    int op_mode;             /* operation mode */
    char *input_file;        /* path to the input file */
    char *output_file;       /* path to the output file */
    unsigned char *password; /* the user defined password */
    unsigned char *key;
    unsigned char *plaintext;
    long cmacSize; 
    long plaintextLength;
    unsigned char *ciphertxt;
    long ciphertextLength;
    unsigned char *cmac = NULL;
    unsigned char *txtCMAC = NULL;

    /* Init arguments */
    input_file = NULL;
    output_file = NULL;
    password = NULL;
    plaintext = NULL;
    ciphertxt = NULL;
    cmacSize = BLOCK_SIZE;
    bit_mode = -1;
    op_mode = -1;
    
    
    /*
    * Get arguments
    */
    while ((opt = getopt(argc, argv, "b:i:m:o:p:desvh:tg")) != -1)
    {

        switch (opt)
        {
        case 'g':
            debug = 1;
            print("Enable debug mode on", info);
            break;
        case 'b':
            bit_mode = atoi(optarg);
            print("Set bit mode", info);
            break;
        case 'i':
            input_file = strdup(optarg);
            print("Get input file name", info);
            break;
        case 'o':
            output_file = strdup(optarg);
            print("Get output file name", info);
            break;
        case 'p':
            password = (unsigned char *)strdup(optarg);
            print("Set password", info);
            break;
        case 'e':
            /* if op_mode == 0 the tool encrypts */
            op_mode = 0;
            break;
        case 'd':
            /* if op_mode == 1 the tool decrypts */
            op_mode = 1;
            break;
        case 's':
            /* if op_mode == 2 the tool signs */
            op_mode = 2;
            break;
        case 'v':
            /* if op_mode == 3 the tool verifies */
            op_mode = 3;
            break;
        case 'h':
        default:
            usage();
        }
    }

    /* check arguments */
    check_args(input_file, output_file, password, bit_mode, op_mode);
    

    /* TODO Develop the logic of your tool here... */


    /* Initialize the library */
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CONFIG, NULL);
    
    /* Keygen from password */
    key = malloc(bit_mode * sizeof(char));
    keygen(password, key, bit_mode);

    /* Operate on the data according to the mode */
    switch (op_mode)
    {
    case 0:     /* encrypt */
        print("Start encrypting...", info);
        plaintext = malloc(sizeof(unsigned char*));
        fileManager(input_file, "r", plaintext, &plaintextLength);
        ciphertextLength = ((plaintextLength / BLOCK_SIZE) + 1) * BLOCK_SIZE;
        ciphertxt = malloc(ciphertextLength * sizeof(char));
        encrypt(plaintext,plaintextLength,key,ciphertxt,bit_mode);
        fileManager(output_file, "w", ciphertxt, &ciphertextLength);
        print("File encrypted.", success);
        break;
    case 1:     /* decrypt */
        print("Start decrypting...", info);
        ciphertxt = malloc(sizeof(unsigned char*));
        fileManager(input_file, "r", ciphertxt, &ciphertextLength);
        plaintext = malloc(sizeof(char));
        plaintextLength = decrypt(ciphertxt,ciphertextLength,key,plaintext,bit_mode);
        fileManager(output_file, "w", plaintext, &plaintextLength);
        print("File decrypted.", success);
        break;
    case 2:     /* sign */
        print("Start signing...", info);
        plaintext = malloc(sizeof(unsigned char*));
        fileManager(input_file, "r", plaintext, &plaintextLength);
        ciphertextLength = ((plaintextLength / BLOCK_SIZE) + 1) * BLOCK_SIZE;
        ciphertxt = malloc(ciphertextLength * sizeof(char));
        encrypt(plaintext,plaintextLength,key,ciphertxt,bit_mode);
        cmac = malloc(BLOCK_SIZE * sizeof(char));
        gen_cmac(plaintext, plaintextLength, key, cmac, bit_mode);
        long csize = BLOCK_SIZE;
        fileManager(output_file, "w", ciphertxt, &ciphertextLength);
        fileManager(output_file, "a", cmac, &csize);
        print("File signed.", success);
        break;
    case 3:      /* verify */

        print("Start verifying...", info);
        ciphertxt = malloc(sizeof(unsigned char*));
        fileManager(input_file, "r", ciphertxt, &ciphertextLength);
        plaintext = malloc(sizeof(char));
        long plaintextLength = (long) decrypt(ciphertxt,ciphertextLength-BLOCK_SIZE,key,plaintext,bit_mode);
        cmac = malloc(BLOCK_SIZE * sizeof(char));
        gen_cmac(plaintext, plaintextLength, key, cmac, bit_mode);
        txtCMAC = malloc(BLOCK_SIZE * sizeof(char));

        fileManager(input_file, "cmac", txtCMAC, &cmacSize);
        print("Verifications Completed", info);
        if (verify_cmac(cmac,txtCMAC) == 1)
        {
            print("Verified!", success);
            fileManager(output_file, "w", plaintext, &plaintextLength);
        }
        else
        {
            print("Not verified!", error);
        }
        print("File verification completed.", success);
        printf("GENERATED CMAC:\n");
        print_hex(cmac, BLOCK_SIZE);
        printf("FILE'S CMAC:\n");
        print_hex(txtCMAC, BLOCK_SIZE);
        break;
    }
        
    /* Clean up */
    free(input_file);
    free(output_file);
    free(password);

    /* END */
    return 0;
}

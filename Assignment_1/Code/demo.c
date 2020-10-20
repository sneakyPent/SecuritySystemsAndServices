#include "simple_crypto.h"

void otp()
{
    unsigned char *key;
    char *plaintext;
    if (isatty(fileno(stdin)))
    {

        printf("[OTP] input:");
        // read user input
        plaintext = readInput(stdin, 10);
        int textSize = (int)strlen(plaintext);
        key = randomKey(textSize); // get a random key fot encryption

        char *encr = (char *)OTP_encrypt(plaintext, key, textSize);
        printf("[OTP] encrypted: ");
        for (int i = 0; i < textSize; ++i)
            printf("%c", printCharacter(encr[i]));
        char *decr = (char *)OTP_decrypt(encr, key, textSize);
        printf("\n[OTP] decrypted: ");
        for (int i = 0; i < textSize; ++i)
            printf("%c", printCharacter(decr[i]));
        printf("\n");
    }
    else
    {
        plaintext = readInput(stdin, 10);
        printf("[OTP] input: %s\n", plaintext);
        int textSize = (int)strlen(plaintext);
        key = randomKey(textSize); // get a random key for encryption

        char *encr = (char *)OTP_encrypt(plaintext, key, textSize);
        printf("[OTP] encrypted: ");
        for (int i = 0; i < textSize; ++i)
            printf("%c", printCharacter(encr[i]));
        char *decr = (char *)OTP_decrypt(encr, key, textSize);
        printf("\n[OTP] decrypted: ");
        for (int i = 0; i < textSize; ++i)
            printf("%c", printCharacter(decr[i]));
        printf("\n");
    }
}

void csCipher()
{
    char *plaintext;
    int key;
    if (isatty(fileno(stdin)))
    {
        char plaintext[1024];
        int key;
        printf("[Caesars] input:");
        // read user input
        scanf("%s", plaintext);
        printf("[Caesars] key:");
        // read user input
        scanf("%d", &key);
        int textSize = (int)strlen(plaintext);

        char *encr = (char *)CS_encrypt(plaintext, key, textSize);
        printf("[Caesars] encrypted:");
        for (int i = 0; i < textSize; ++i)
            printf("%c", printCharacter(encr[i]));
        char *decr = (char *)CS_decrypt(encr, key, textSize);
        printf("\n[Caesars] decrypted:");
        for (int i = 0; i < textSize; ++i)
            printf("%c", printCharacter(decr[i]));
        printf("\n");
    }
    else
    {

        // read user input
        plaintext = readInput(stdin, 10);
        printf("[Caesars] input: %s\n", plaintext);
        // read user input
        key = atoi(readInput(stdin, 10));
        printf("[Caesars] key: %d\n", key);

        int textSize = (int)strlen(plaintext);

        char *encr = (char *)CS_encrypt(plaintext, key, textSize);
        printf("[Caesars] encrypted: ");
        for (int i = 0; i < textSize; ++i)
            printf("%c", printCharacter(encr[i]));
        char *decr = (char *)CS_decrypt(encr, key, textSize);
        printf("\n[Caesars] decrypted: ");
        for (int i = 0; i < textSize; ++i)
            printf("%c", printCharacter(decr[i]));
        printf("\n");
    }
}
void vgCipher()
{

    char *plaintext;
    char *key;

    if (isatty(fileno(stdin)))
    {
        printf("[Vigenere] input:");
        // read user input
        scanf("%s", plaintext);
        printf("[Vigenere] key:");
        // read user input
        scanf("%s", key);
        int textSize = (int)strlen(plaintext);

        char *encr = (char *)VG_encrypt(plaintext, key);
        printf("[Vigenere] encrypted: ");
        for (int i = 0; i < textSize; ++i)
            printf("%c", printCharacter(encr[i]));
        char *decr = (char *)VG_decrypt(encr, key);
        printf("\n[Vigenere] decrypted: ");
        for (int i = 0; i < textSize; ++i)
            printf("%c", printCharacter(decr[i]));
        printf("\n");
    }
    else
    {
        // read user input
        plaintext = readInput(stdin, 10);
        printf("[Vigenere] input: %s\n", plaintext);
        // read user input
        key = readInput(stdin, 10);
        printf("[Vigenere] key: %s\n", key);

        int textSize = (int)strlen(plaintext);
        char *encr = (char *)VG_encrypt(plaintext, key);
        printf("[Vigenere] encrypted: ");
        for (int i = 0; i < textSize; ++i)
            printf("%c", printCharacter(encr[i]));
        char *decr = (char *)VG_decrypt(encr, key);
        printf("\n[Vigenere] decrypted: ");
        for (int i = 0; i < textSize; ++i)
            printf("%c", printCharacter(decr[i]));
        printf("\n");
    }
}

int main()
{
    otp();
    csCipher();
    vgCipher();
    return 0;
}
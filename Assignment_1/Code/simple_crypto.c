#include "simple_crypto.h"


int main() {
    unsigned char* key;
    char plaintext[1024];
    printf("[OTP] input:");
    fgets(plaintext,1024,stdin);
    int textSize = (int) strlen(plaintext);
    key = randomKey(textSize);
   
    char* encr = (char*) OTP_encrypt(plaintext, key, textSize);
    printf("[OTP] encrypted:");
    for(int i = 0; i < textSize; ++i)
        printf("%c", printCharacter(encr[i]));
    char* decr = (char*)  OTP_decrypt(encr, key, textSize);
    printf("\n[OTP] decrypted:");
    for(int i = 0; i < textSize; ++i)
        printf("%c", printCharacter(decr[i]));
    return 0;
}


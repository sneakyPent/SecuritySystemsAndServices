#include "simple_crypto.h"


int main() {
    unsigned char* key;
    char plaintext[1024];
    printf("[OTP] input:");
    fgets(plaintext,1024,stdin);
    int textSize = sizeof(plaintext);
    key = randomKey(sizeof(textSize));

    unsigned char* encr = OTP_encrypt(plaintext, key, textSize);
    printf("[OTP] encrypted:");
    for(int i = 0; i < textSize; ++i)
        printf("%c", encr[i]);

    unsigned char* decr = OTP_decrypt(encr, key, textSize);
    printf("[OTP] decrypted:");
    for(int i = 0; i < textSize; ++i)
        printf("%c", decr[i]);
    printf("\n");
    return 0;
}


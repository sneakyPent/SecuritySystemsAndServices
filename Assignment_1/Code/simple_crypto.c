#include "simple_crypto.h"


void otp() {
    unsigned char* key;
    char plaintext[1024];
    printf("[OTP] input:");
    scanf("%s", plaintext);
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
    printf("\n");
}


void csCipher(){
}

void vgCipher(){
}

int main() {
    otp();
    csCipher();
    vgCipher();
    return 0;
}
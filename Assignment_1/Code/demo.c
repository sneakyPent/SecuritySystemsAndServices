#include "simple_crypto.h"


void otp() {
    unsigned char* key;
    char plaintext[1024];
    printf("[OTP] input:");
    // read user input 
    scanf("%s", plaintext);
    int textSize = (int) strlen(plaintext);
    key = randomKey(textSize);  // get a random key fot encryption

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
    
    char plaintext[1024];
    int key;
    printf("[Caesars] input:");
    // read user input 
    scanf("%s", plaintext);
    printf("[Caesars] key:");
    // read user input 
    scanf("%d", &key);
    int textSize = (int) strlen(plaintext);

    char* encr = (char*) CS_encrypt(plaintext, key, textSize);
    printf("[Caesars] encrypted:");
    for(int i = 0; i < textSize; ++i)
        printf("%c", printCharacter(encr[i]));
    char* decr = (char*)  CS_decrypt(encr, key, textSize);
    printf("\n[Caesars] decrypted:");
    for(int i = 0; i < textSize; ++i)
        printf("%c", printCharacter(decr[i]));
    printf("\n");
}


void vgCipher(){
      
    char plaintext[1024];
    int key;
    printf("[Vigenere] input:");
    // read user input 
    scanf("%s", plaintext);
    printf("[Vigenere] key:");
    // read user input 
    scanf("%d", &key);
    int textSize = (int) strlen(plaintext);

    char* encr = (char*) CS_encrypt(plaintext, key, textSize);
    printf("[Vigenere] encrypted:");
    for(int i = 0; i < textSize; ++i)
        printf("%c", printCharacter(encr[i]));
    char* decr = (char*)  CS_decrypt(encr, key, textSize);
    printf("\n[Vigenere] decrypted:");
    for(int i = 0; i < textSize; ++i)
        printf("%c", printCharacter(decr[i]));
    printf("\n");
}

int main() {
    otp();
    csCipher();
    // vgCipher();
    return 0;
}
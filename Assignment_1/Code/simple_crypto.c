#include "simple_crypto.h"


int main() {
    int i;
    unsigned char* key;
    key = randomKey( 8);
    for(i = 0; i < 8; ++i)
        printf("%02X", key[i]);
    printf("\n");
    return 0;
}


// #include "simple_crypto.h"
//     #include <unistd.h>
//     #include <fcntl.h>
//     #include "string.h"

//     int main(){
//             int i;
//             char str[8];
//             ///
//             printf("Type message to be encrypted: ");
//             scanf("%s",str[8]);
//             ///
//             unsigned char buffer[8];
//             int fd = open("/dev/urandom", O_RDONLY);
//             read(fd, buffer, 8);
//             //buffer now contains the random data
//             close(fd);
//             for(i = 0; i < 8; ++i)
//                     printf("%02X", buffer[i]);
//             printf("\n");

//             char output[8];

//             for (i=0; i<strlen(str); i++)
//             {
//                 char temp = str[i] ^ buffer[i];
//                 output[i] = temp;
//             }

//             output[i] = '\0';
//             printf("02Xs", output);


//             return 0;
//     }`11
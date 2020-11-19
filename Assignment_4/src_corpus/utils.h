#ifndef _UTILS_H
#define _UTILS_H


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DELIM ":"
/**
 * @brief Struct in list form for saving users in the
 *          Access Control Log Monitoring tool 
 */
typedef struct userList {
    int user;                   /** The user */
    int filesNotAccessed;       /** The number of files that user tried to access with no permission */
    int mods;                   /** The modifications the user has made on a given file */
    struct userList * nextUser; /** Pointer to the next users of the UserList */
} userList;


enum information{all, modifies, nonPermissions};

userList * addUser(userList * head, int user, int modification, int notPermission);

void printUsers(userList *head, enum information printingInfo);

char * getLineInfo(char * str);

char *trimwhitespace(char *str);

void print_hex(unsigned char *data, size_t len);

char *stringToHex(unsigned char *data, size_t len);

void print_string(unsigned char *data, size_t len);

#endif /* _UTILS_H */

#ifndef _UTILS_H
#define _UTILS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#define DELIM ":"
#define BUF_LEN 256
#define NON_PERMISSION_LIMIT 7

/**
 * @brief Struct in linked list form for saving users in the
 *          Access Control Log Monitoring tool 
 */
typedef struct userList
{
    int user;                           /** The user */
    struct filesList *filesNotAccessed; /** List of files that user tried to access with no permission */
    int mods;                           /** The modifications the user has made on a given file */
    struct userList *nextUser;          /** Pointer to the next users of the UserList */
} userList;

/**
 * @brief Struct in linked list form for saving Files in the
 *          Access Control Log Monitoring tool
 * 
 */
typedef struct filesList
{
    char fileName[3 * BUF_LEN]; /** The current file name*/
    int restFiles;              /** The sum of files from list head until the end of the list */
    struct filesList *nextFile; /** Pointer to the next file of File list */

} filesList;

enum mode
{
    info,
    error,
    success
};

enum information
{
    all,
    modifies,
    nonPermissions
};

filesList *addFile(filesList *head, char *fileName);

userList *addUser(userList *head, int user, int modification, int notPermission, char *fileName);

void printUsers(userList *head, enum information printingInfo);

char *getLineInfo(char *str);

char *trimwhitespace(char *str);

void print_hex(unsigned char *data, size_t len);

char *stringToHex(unsigned char *data, size_t len);

void print_string(unsigned char *data, size_t len);

char *getFilesName(FILE *file);

int getAccess(const char *path, const char *mode);

void print(char *str, enum mode md);

void printFiles(filesList *head);
#endif /* _UTILS_H */

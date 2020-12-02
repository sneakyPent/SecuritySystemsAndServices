#ifndef _UTILS_H
#define _UTILS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>

#define DELIM "·" /** The delimeter for log file line */
#define BUF_LEN 256
#define NON_PERMISSION_LIMIT 7 /** The files limit, defines the malicious user*/

/**
 * @brief Struct in linked list form for saving users in the
 *          Access Control Log Monitoring tool 
 * 
 * @param user The user
 * @param filesNotAccessed List of files that user tried to access with no permission
 * @param mods The modifications the user has made on a given file
 * @param nextUser Pointer to the next users of the UserList
 * 
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
 * @param fileName he current file name
 * @param restFiles The sum of files from list head until the end of the list
 * @param nextFile Pointer to the next file of File list
 */
typedef struct filesList
{
    char fileName[3 * BUF_LEN]; /** The current file name*/
    int restFiles;              /** The sum of files from list head until the end of the list */
    struct filesList *nextFile; /** Pointer to the next file of File list */

} filesList;

/**
 * @brief Enumerator for the print function mode
 * 
 * @param info 0
 * @param error 1
 * @param success 2
 * 
 */
enum mode
{
    info,
    error,
    success
};

/**
 * @brief Enumerator for the printUsers function mode
 *
 * @param all 0
 * @param modifies 1
 * @param nonPermissions 2
 */
enum information
{
    all,
    modifies,
    userName
};

/**
 * @brief Adds the given filename to the files list if it doesn't already exist only if the given fileName is not the empty string.
 * 
 * @param head The files list
 * @param fileName The new filename
 * @return filesList* 
 */
filesList *addFile(filesList *head, char *fileName);

/**
 * @brief Adds the given user to the user list if it doesn't already exist.
 *  If user found, increases his modifications with modification param.
 *  If notPermission flag is 1 adds the given fileName in his files list.
 * 
 * @param head The users list
 * @param user The current user
 * @param modification 0 or 1 
 * @param notPermission 0 or 1
 * @param fileName The file tried to access if he did, or empty string
 * @return userList* 
 */
userList *addUser(userList *head, int user, int modification, int notPermission, char *fileName);

/**
 * @brief Get the LogFile's info for the given line
 * 
 * @param str The line
 * @return char* info
 */
char *getLineInfo(char *str);

/**
 * @brief Get the file's absolute path from File pointer
 * 
 * @param file File pointer
 * @return char* file's path 
 */
char *getFilesName(FILE *file);

// ----------- Trim function and it's assistant functions -----------

/**
 * @brief Returns if the given char c is a whitespace, tab or newline character 
 * * 
 * @param c 
 * @return  1 for true or 0 for false 
 */
int is_white_space(char c);

/**
 * @brief Get the length of a string without the newline character
 */
int get_str_len(char const *str);

/**
 * @brief Returns the correct length of a trimmed string
 */
int get_trim_len(char const *str);

/**
 * @brief Return the given string without trailing and leading whitespaces
 * 
 * @param str 
 * @return char* 
 */
char *trimwhitespace(char *str);

// ------------------------------------------------------------------

/**
 * @brief Returns whether the running user has access for the requested mode in the given file or not
 * 
 * @param path The filename
 * @param mode The requested mode (r,r+,rb,rb+,w,w+,wb,wb+,w,w+,wb,wb+,)
 * @return 1 for access granted or 0 for access denied
 */
int getAccess(const char *path, const char *mode);

/**
 * @brief Return the unsigned char * string in hexadecimmal format 
 * 
 * @param data The string
 * @param len  The string's length
 * @return char* 
 */
char *stringToHex(unsigned char *data, size_t len);

/**
 * @brief Prints the given string in hexadecimal mode
 * 
 * @param data The string
 * @param len The string's length
 */
void print_hex(unsigned char *data, size_t len);

void print_string(unsigned char *data, size_t len);

/**
 * @brief Prints the given string with specific color depends on given mode (used for debugging)
 * 
 * @param str 
 * @param md info, error, success
 */
void print(char *str, enum mode md);

/**
 * @brief Prints the files in the given file list
 * 
 * @param head The file list
 */
void printFiles(filesList *head);

/**
 * @brief Prints the Users in the given user list with the given mode information
 * 
 * @param head The user list
 * @param printingInfo -"modifies" for the user and his modifies on specific file. - "userName" for only the userName.
 * - "all" for the user, his modifications on specific file and his non permitted accesses
 */
void printUsers(userList *head, enum information printingInfo);

/**
* @brief Get the Current Date And Time
 * 
 * @return char** 2D array [2][BUF_LEN] char[0]=time char[1]=date
 */
char **getCurrentDateAndTime();


/**
 * @brief Get the date and time, X minutes ago. 
 * 
 * @param limit The X minutes
 */
char **getDateTimeLimit(int limit);


#endif /* _UTILS_H */

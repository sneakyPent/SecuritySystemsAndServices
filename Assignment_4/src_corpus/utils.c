#include "utils.h"

char *getLineInfo(char *str)
{
    char *ptr = strtok(str, DELIM);
    ptr = strtok(NULL, DELIM);
    return ptr;
}

char *trimwhitespace(char *str)
{
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

char *stringToHex(unsigned char *data, size_t len)
{
    size_t i;
    char *str = malloc(2 * len);

    if (!data)
        printf("NULL data\n");
    else
    {
        for (int i = 0, j = 0; i < len; ++i, j += 2)
            sprintf(str + j, "%02X", data[i] & 0xff);
        return str;
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

/**
 * @brief Get the Files Name from File pointer
 * 
 * @param file File pointer
 * @return char* filenName
 */
char *getFilesName(FILE *file)
{
    // Get the filename from File pointer
    if (file)
    {
        int fileDesc = fileno(file);
        char buff[BUF_LEN];
        char *filename = malloc((sizeof(char) * BUF_LEN));
        char des[25];
        sprintf(des, "/proc/self/fd/%d", fileDesc);
        int len = readlink(des, buff, BUF_LEN);
        snprintf(filename, len + 1, "%s", buff);
        return filename;
    }
    return "error";
}

int getAccess(const char *path, const char *mode)
{
    if (access(path, F_OK) != 0)
        return 0;
    if (strstr(mode, "+") != NULL)
    {
        int r_a = (access(path, R_OK) == 0) ? 0 : 1;
        int w_a = (access(path, W_OK) == 0) ? 0 : 1;
        return (r_a || w_a);
    }
    if (strstr(mode, "r") != NULL)
    {
        print("Read access", info);
        return (access(path, R_OK) == 0) ? 0 : 1;
    }
    else if (strstr(mode, "w") != NULL)
    {
        print("Write access", info);
        return (access(path, W_OK) == 0) ? 0 : 1;
    }
    else if (strstr(mode, "a") != NULL)
    {
        print("Append access", info);
        return (access(path, W_OK) == 0) ? 0 : 1;
    }
    else
        return -1;
}

void print(char *str, enum mode md)
{
    switch (md)
    {
    case error:
        printf("\033[1;31m");
        printf("[ERROR]: \033[0m %s\n", str);
        exit(EXIT_FAILURE);
        break;
    case info:
        printf("\033[0;36m");
        printf("[INFO]: \033[0m %s\n", str);
        break;
    case success:
        printf("\033[0;32m");
        printf("%s\033[0m\n", str);
        break;
    default:
        printf("%s\n", str);
        break;
    }
}

userList *addUser(userList *head, int user, int modification, int notPermission)
{
    userList *current = head;
    if (current == NULL)
    {
        current = malloc(sizeof(userList));
        current->user = user;
        current->filesNotAccessed = notPermission;
        current->mods = modification;
        current->nextUser = NULL;
        return current;
    }
    else
    {
        while (current->nextUser != NULL)
        {
            if (current->user == user)
            {
                current->mods += modification;
                current->filesNotAccessed += notPermission;
                return head;
            }
            current = current->nextUser;
        }
        if (current->user == user)
        {
            current->mods += modification;
            current->filesNotAccessed += notPermission;
            return head;
        }
    
        // If user not found we add him
        current->nextUser = malloc(sizeof(userList));
        current->nextUser->user = user;
        current->nextUser->mods = modification;
        current->nextUser->filesNotAccessed = notPermission;
        current->nextUser->nextUser = NULL;
    }
    return head;
}

void printUsers(userList *head, enum information printingInfo){
    userList * currentUser = head;
    switch (printingInfo)
    {
    case all:
        printf("-----------------------------------------------USERS----------------------------------------------\n");
        printf("| \t\t\t\t\t\t\t\t\t\t\t\t |\n");
        while (currentUser != NULL) {
            printf("|\t UID: %d \t | \t Modifications: %d \t | \t Non permitted Accesses: %d \t |\n", currentUser->user, currentUser-> mods, currentUser->filesNotAccessed);
            currentUser = currentUser->nextUser;
        }
        printf("| \t\t\t\t\t\t\t\t\t\t\t\t |\n");
        printf("--------------------------------------------------------------------------------------------------\n");
        break;
    case modifies:
        printf("---------------------------USERS--------------------------\n");
        printf("| \t\t\t\t\t\t\t |\n");
        while (currentUser != NULL) {
            printf("|\t UID=%d \t | \t Modifications: %d \t |\n", currentUser->user, currentUser-> mods);
            currentUser = currentUser->nextUser;
        }
        printf("| \t\t\t\t\t\t\t |\n");
        printf("----------------------------------------------------------\n");
        break;
    case nonPermissions:
        printf("-----------USERS----------\n");
        printf("| \t\t\t |\n");
        while (currentUser != NULL) {
            if (currentUser->filesNotAccessed > NON_PERMISSION_LIMIT)
                printf("|\t UID=%d \t |\n", currentUser->user);
            currentUser = currentUser->nextUser;
        }
        printf("| \t\t\t |\n");
        printf("--------------------------\n");
        break;
    default:
        printf("-----------------------------------------------USERS----------------------------------------------\n");
        while (currentUser != NULL) {
            printf("|\t UID=%d \t | \t Modifications: %d \t | \t Non permitted Accesses: %d \t |\n", currentUser->user, currentUser-> mods, currentUser->filesNotAccessed);
            currentUser = currentUser->nextUser;
        }
        printf("--------------------------------------------------------------------------------------------------\n");
        break;
    }
    
}
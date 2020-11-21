#include "utils.h"

char *getLineInfo(char *str)
{
    char *ptr = strtok(str, DELIM);
    ptr = strtok(NULL, DELIM);
    return trimwhitespace(ptr);
}

int is_white_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

/**
 * @brief Get the length of a string
 */
int get_str_len(char const *str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len += 1;
    }
    return (len);
}

/**
 * @brief Returns the correct length of a trimmed string
 */
int get_trim_len(char const *str)
{
    int lastPosition = get_str_len(str) - 1;
    while (is_white_space(str[lastPosition]))
    {
        lastPosition -= 1;
    }
    int firstPosition = 0;
    while (is_white_space(str[firstPosition]))
    {
        firstPosition += 1;
    }
    return (lastPosition - firstPosition);
}

char *trimwhitespace(char *str)
{
    char *trim = NULL;
    int i, len, end;
    int start = 0;
    while (is_white_space(str[start]))
    {
        start += 1;
    }
    if (str != NULL)
    {
        i = 0;
        len = get_trim_len(str) + 1;
        trim = (char *)malloc(len);
        while (i < len)
        {
            trim[i] = str[start];
            i += 1;
            start += 1;
        }
        trim[i] = '\0';
    }
    return (trim);
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

filesList *addFile(filesList *head, char *fileName)
{
    filesList *currentFile = head;
    if (strcmp(fileName,"") != 0 )
        if (currentFile == NULL)
        {
            currentFile = malloc(sizeof(filesList));
            strcpy(currentFile->fileName, fileName);
            currentFile->restFiles = 1;
            currentFile->nextFile = NULL;
            return currentFile;
        }
        else
        {
            // Parse the list to check if fil exists
            while (currentFile->nextFile != NULL)
            {
                // By the time we found the file we return the head and do nothing
                if (strstr(currentFile->fileName, fileName) != NULL)
                    return head;
                currentFile = currentFile->nextFile;
            }
            if (strstr(currentFile->fileName, fileName) != NULL)
                return head;
            // If file not found, add it and increase restFiles
            currentFile->nextFile = malloc(sizeof(filesList));
            strcpy(currentFile->nextFile->fileName, fileName);
            head->restFiles += 1;
            currentFile->nextFile->nextFile = NULL;
        }
    return head;
}

userList *addUser(userList *head, int user, int modification, int notPermission, char *fileName)
{
    userList *currentUser = head;
    if (currentUser == NULL)
    {
        currentUser = malloc(sizeof(userList));
        currentUser->user = user;
        currentUser->filesNotAccessed = addFile(currentUser->filesNotAccessed, fileName);
        currentUser->mods = modification;
        currentUser->nextUser = NULL;
        return currentUser;
    }
    else
    {
        while (currentUser->nextUser != NULL)
        {
            if (currentUser->user == user)
            {
                currentUser->mods += modification;
                if (notPermission == 1)
                    currentUser->filesNotAccessed = addFile(currentUser->filesNotAccessed, fileName);
                return head;
            }
            currentUser = currentUser->nextUser;
        }
        if (currentUser->user == user)
        {
            currentUser->mods += modification;
            if (notPermission == 1)
                currentUser->filesNotAccessed = addFile(currentUser->filesNotAccessed, fileName);

            return head;
        }

        // If user not found, add him
        currentUser->nextUser = malloc(sizeof(userList));
        currentUser->nextUser->user = user;
        currentUser->nextUser->mods = modification;
        currentUser->nextUser->filesNotAccessed = addFile(currentUser->filesNotAccessed, fileName);
        currentUser->nextUser->nextUser = NULL;
    }
    return head;
}

void printFiles(filesList *head)
{
    filesList *currentFile = head;
    while (currentFile != NULL)
    {
        if (currentFile == NULL)
            print("error", info);
        printf("|\t filename=%s \t |\n", currentFile->fileName);
        currentFile = currentFile->nextFile;
    }
}

void printUsers(userList *head, enum information printingInfo)
{
    userList *currentUser = head;
    switch (printingInfo)
    {
    case all:
        printf("-----------------------------------------------USERS----------------------------------------------\n");
        printf("| \t\t\t\t\t\t\t\t\t\t\t\t |\n");
        while (currentUser != NULL)
        {
            printf("|\t UID: %d \t | \t Modifications: %d \t | \t Non permitted Accesses: %d \t |\n", currentUser->user, currentUser->mods, currentUser->filesNotAccessed);
            currentUser = currentUser->nextUser;
        }
        printf("| \t\t\t\t\t\t\t\t\t\t\t\t |\n");
        printf("--------------------------------------------------------------------------------------------------\n");
        break;
    case modifies:
        printf("---------------------------USERS--------------------------\n");
        printf("| \t\t\t\t\t\t\t |\n");
        while (currentUser != NULL)
        {
            printf("|\t UID: %d \t | \t Modifications: %d \t |\n", currentUser->user, currentUser->mods);
            currentUser = currentUser->nextUser;
        }
        printf("| \t\t\t\t\t\t\t |\n");
        printf("----------------------------------------------------------\n");
        break;
    case nonPermissions:
        printf("-----------USERS----------\n");
        printf("| \t\t\t |\n");
        while (currentUser != NULL)
        {
            if (currentUser->filesNotAccessed->restFiles > NON_PERMISSION_LIMIT)
                printf("|\t UID: %d    \t |\n", currentUser->user);
            currentUser = currentUser->nextUser;
        }
        printf("| \t\t\t |\n");
        printf("--------------------------\n");
        break;
    default:
        printf("-----------------------------------------------USERS----------------------------------------------\n");
        while (currentUser != NULL)
        {
            printf("|\t UID: %d \t | \t Modifications: %d \t | \t Non permitted Accesses: %d \t |\n", currentUser->user, currentUser->mods, currentUser->filesNotAccessed);
            currentUser = currentUser->nextUser;
        }
        printf("--------------------------------------------------------------------------------------------------\n");
        break;
    }
}
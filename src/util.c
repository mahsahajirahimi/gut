#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <libgen.h>

bool folderExists(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0) {
        return false;
    }
    return S_ISDIR(statbuf.st_mode);
}

char* find_repository(const char *startDir, const char *targetFolder) {
    char *currentDir = strdup(startDir);
    char *folderPath = NULL;
    char *parentDir = NULL;
    size_t pathLen = 0;

    while (1) {
        pathLen = strlen(currentDir) + strlen(targetFolder) + 2;
        folderPath = (char *)malloc(pathLen);
        snprintf(folderPath, pathLen, "%s/%s", currentDir, targetFolder);
        if (folderExists(folderPath)) {
            free(currentDir);
            return folderPath;
        }
        printf("%s\n",currentDir);
        parentDir = dirname(currentDir);
        printf("%s\n",parentDir);
        if (strcmp(currentDir, ".") == 0) {
            break;
        }

        free(currentDir);
        currentDir = strdup(parentDir);
        printf("3 %s\n",currentDir);
    }

    free(currentDir);
    free(folderPath);
    return "";
}

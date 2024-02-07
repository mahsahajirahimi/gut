#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "init.h"
#include <stdbool.h>
#include <dirent.h>
#include <time.h>

const char *globalConfig = ".gutconfig";
const char *folderName = ".gut";
const char *stagefolder = "STAGE";

#define MAX_PATH 1024
#define MAX_COMMIT_MESSAGE_LENGTH 72

int is_gut_repo(char *path)
{
    char gut_path[MAX_PATH];
    struct stat st = {0};

    snprintf(gut_path, sizeof(gut_path), "%s/.gut", path);

    return stat(gut_path, &st) == 0 && S_ISDIR(st.st_mode);
}

int create_gut_repo(char *path)
{
    char gut_path[MAX_PATH];
    snprintf(gut_path, sizeof(gut_path), "%s/.gut", path);
    if (mkdir(gut_path, 0700) != 0)
    {
        perror("mkdir() error");
        return 1;
    }

    char head_path[MAX_PATH];
    snprintf(head_path, sizeof(head_path), "%s/HEAD", gut_path);
    FILE *head_file = fopen(head_path, "w");
    if (head_file != NULL)
    {
        fprintf(head_file, "master\n"); 
        fclose(head_file);
    }
    else
    {
        perror("fopen() error");
        return 1;
    }

    char branches_path[MAX_PATH];
    snprintf(branches_path, sizeof(branches_path), "%s/BRANCHES", gut_path);
    FILE *branches_file = fopen(branches_path, "w");
    if (branches_file != NULL)
    {
        fprintf(branches_file, "master:first\n");
        fclose(branches_file);
    }
    else
    {
        perror("fopen() error");
        return 1;
    }

    char commits_path[MAX_PATH];
    snprintf(commits_path, sizeof(commits_path), "%s/commits", gut_path);
    if (mkdir(commits_path, 0700) != 0)
    {
        perror("mkdir() error");
        return 1;
    }

    char first_commit_path[MAX_PATH];
    snprintf(first_commit_path, sizeof(first_commit_path), "%s/commits/first", gut_path);
    if (mkdir(first_commit_path, 0700) != 0)
    {
        perror("mkdir() error");
        return 1;
    }

    // struct dirent *entry;
    // DIR *dir = opendir(path);
    // if (dir != NULL)
    // {
    //     while ((entry = readdir(dir)) != NULL)
    //     {
    //         if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
    //         {
    //             continue;
    //         }

    //         char sourcePath[MAX_PATH];
    //         snprintf(sourcePath, sizeof(sourcePath), "%s/%s", path, entry->d_name);

    //         char destinationPath[MAX_PATH];
    //         snprintf(destinationPath, sizeof(destinationPath), "%s/%s", first_commit_path, entry->d_name);

    //         if (rename(sourcePath, destinationPath) != 0)
    //         {
    //             perror("Error moving file to first commit");
    //             return 1;
    //         }
    //     }
    //     closedir(dir);
    // }

    char logfile_path[MAX_PATH];
    snprintf(logfile_path, sizeof(logfile_path), "%s/LOGFILE", gut_path);
    FILE *logfile_file = fopen(logfile_path, "w");
    if (logfile_file != NULL)
    {
        fclose(logfile_file);
    }
    else
    {
        perror("fopen() error");
        return 1;
    }


    FILE *logfile = fopen(logfile_path, "a");

    if (logfile != NULL)
    {
        fprintf(logfile, "first:Initial commit\n");
        fclose(logfile);
    }
    else
    {
        perror("fopen() error");
        return 1;
    }

    char stage_path[MAX_PATH];
    snprintf(stage_path, sizeof(stage_path), "%s/STAGE", gut_path);
    if (mkdir(stage_path, 0700) != 0)
    {
        perror("mkdir() error");
        return 1;
    }

    char gutconfig_path[MAX_PATH];
    snprintf(gutconfig_path, sizeof(gutconfig_path), "%s/.gut/.gutconfig", path);
    FILE *gutconfig_file = fopen(gutconfig_path, "w");
    if (gutconfig_file != NULL)
    {
        fprintf(gutconfig_file, "\tname = Your Name\n");
        fprintf(gutconfig_file, "\temail = your.email@example.com\n");
        fclose(gutconfig_file);
    }
    else
    {
        perror("fopen() error");
        return 1;
    }

    return 0;
}

char *findRepository()
{
    char path[MAX_PATH];

    if (getcwd(path, sizeof(path)) == NULL)
    {
        perror("getcwd() error");
        return "";
    }

    char *p = path + strlen(path);

    while (p != path)
    {
        if (is_gut_repo(path))
        {
            return strdup(path);
        }

        while (*p != '/' && p != path)
        {
            p--;
        }

        *p = '\0';
    }

    return "";
}

void handle_init_command()
{
    char path[MAX_PATH];
    char *repositoryPath = findRepository();
    if (strcmp(repositoryPath, "") != 0)
    {
        printf("Already a gut repository (or any of the parent directories): %s\n", repositoryPath);
        free(repositoryPath);
        return;
    }

    if (getcwd(path, sizeof(path)) == NULL)
    {
        perror("getcwd() error");
        return 1;
    }

    if (create_gut_repo(path) != 0)
    {
        perror("mkdir() error");
        return 1;
    }
    printf("Initialized empty gut repository in %s/.gut\n", path);
}

void handle_config(const char *args[], int args_count)
{
    if (args_count > 4)
    {
        FILE *file;
        if (strcmp(args[2], "--global") == 0)
        {
            if (strcmp(args[3], "user.name") == 0)
            {
                file = fopen("~/.gutconfig", "w");
                if (file != NULL)
                {
                    fprintf(file, "name=%s\n", args[4]);
                    fclose(file);
                }
            }
            else if (strcmp(args[3], "user.email") == 0)
            {
                file = fopen("~/.gutconfig", "w");
                if (file != NULL)
                {
                    fprintf(file, "email=%s\n", args[4]);
                    fclose(file);
                }
            }
        }
        else
        {
            if (strcmp(args[2], "user.name") == 0)
            {
                file = fopen(".gutconfig", "w");
                if (file != NULL)
                {
                    fprintf(file, "name=%s\n", args[3]);
                    fclose(file);
                }
            }
            else if (strcmp(args[2], "email") == 0)
            {
                file = fopen(".gutconfig", "w");
                if (file != NULL)
                {
                    fprintf(file, "user.email=%s\n", args[3]);
                    fclose(file);
                }
            }
        }
    }
    else
    {
        printf("Usage: gut config [--global] <config>\n");
    }
}

void handle_add_command(const char *args[], int args_count)
{
    if (args_count < 3)
    {
        printf("Usage: add <file>\n");
        return;
    }

    const char *filename = args[2];

    char *path = findRepository();
    if (strcmp(path, "") == 0)
    {
        printf("gut repository not found.\n");
        return;
    }

    char stagedPath[1024];
    snprintf(stagedPath, sizeof(stagedPath), "%s/.gut/%s/%s", path, stagefolder, filename);

    if (access(stagedPath, F_OK) == 0)
    {
        printf("File or folder '%s' already staged.\n", filename);
        return;
    }
    if (access(filename, F_OK) != 0)
    {
        printf("File or folder '%s' does not exist.\n", filename);
        return;
    }

    // Copy the file or folder to the STAGE folder
    if (copyFileOrFolder(filename, stagedPath) != 0)
    {
        perror("Error copying file to staged folder");
        return;
    }

    printf("File '%s' staged.\n", filename);
}

int copyFileOrFolder(const char *source, const char *destination)
{
    struct stat st;
    if (stat(source, &st) != 0)
    {
        perror("Error stating source file or folder");
        return 1;
    }

    if (S_ISDIR(st.st_mode))
    {
        return copyFolder(source, destination);
    }
    else
    {
        return copyFile(source, destination);
    }
}

int copyFile(const char *source, const char *destination)
{
    FILE *sourceFile = fopen(source, "r");
    printf("Source: %s\n", source);
    if (sourceFile == NULL)
    {
        perror("Error opening source file");
        return 1;
    }

    FILE *destinationFile = fopen(destination, "w");
    if (destinationFile == NULL)
    {
        perror("Error opening destination file");
        fclose(sourceFile);
        return 1;
    }
    char buffer[1024];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0)
    {
        fwrite(buffer, 1, bytesRead, destinationFile);
    }

    fclose(sourceFile);
    fclose(destinationFile);
    return 0;
}

int copyFolder(const char *source, const char *destination)
{
    DIR *dir = opendir(source);
    if (dir == NULL)
    {
        perror("Error opening source folder");
        return 1;
    }

    if (mkdir(destination, 0700) != 0)
    {
        perror("Error creating destination folder");
        closedir(dir);
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char sourcePath[1024];
        snprintf(sourcePath, sizeof(sourcePath), "%s/%s", source, entry->d_name);

        char destinationPath[1024];
        snprintf(destinationPath, sizeof(destinationPath), "%s/%s", destination, entry->d_name);

        if (copyFileOrFolder(sourcePath, destinationPath) != 0)
        {
            perror("Error copying file or folder");
            closedir(dir);
            return 1;
        }
    }

    closedir(dir);
    return 0;
}

void handle_reset_command(const char *args[], int args_count)
{
    if (args_count < 3)
    {
        printf("Usage: gut reset <file or folder>\n");
        return;
    }

    char *path = findRepository();
    if (strcmp(path, "") == 0)
    {
        printf("gut repository not found.\n");
        return;
    }

    char stagePath[1024];
    snprintf(stagePath, sizeof(stagePath), "%s/.gut/%s", path, stagefolder);
    char *fileOrFolder = args[2];
    char filePath[1024];
    snprintf(filePath, sizeof(filePath), "%s/%s", stagePath, fileOrFolder);
    struct stat st;
    if (stat(filePath, &st) == 0)
    {
        if (S_ISDIR(st.st_mode))
        {
            DIR *dir = opendir(filePath);
            if (dir != NULL)
            {
                struct dirent *entry;
                while ((entry = readdir(dir)) != NULL)
                {
                    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                    {
                        continue;
                    }
                    char entryPath[1024];
                    snprintf(entryPath, sizeof(entryPath), "%s/%s", filePath, entry->d_name);
                    if (remove(entryPath) != 0)
                    {
                        perror("Error removing file from STAGE");
                        return;
                    }
                }
                closedir(dir);
            }
        }
    }
    if (remove(filePath) != 0)
    {
        perror("Error removing file or folder from STAGE");
        return;
    }
    printf("File or folder '%s' removed from STAGE.\n", fileOrFolder);
}
void handle_branch(const char *args[], int args_count)
{
    if (args_count == 2)
    {
        FILE *branchesFile = fopen(".gut/BRANCHES", "r");
        if (branchesFile != NULL)
        {
            char branch[256];
            while (fgets(branch, sizeof(branch), branchesFile) != NULL)
            {
                printf("%s", branch);
            }
            fclose(branchesFile);
        }
    }
    else if (args_count == 3)
    {
        const char *branchName = args[2];
        FILE *headFile = fopen(".gut/HEAD", "r");
        if (headFile != NULL)
        {
            char currentBranch[256];
            if (fgets(currentBranch, sizeof(currentBranch), headFile) != NULL)
            {
                currentBranch[strcspn(currentBranch, "\n")] = '\0';
                FILE *branchesFile = fopen(".gut/BRANCHES", "r");
                if (branchesFile != NULL)
                {
                    char branch[256];
                    while (fgets(branch, sizeof(branch), branchesFile) != NULL)
                    {
                        branch[strcspn(branch, "\n")] = '\0';
                        if (strcmp(branch, branchName) == 0)
                        {
                            printf("Error: Branch '%s' already exists.\n", branchName);
                            fclose(branchesFile);
                            fclose(headFile);
                            return;
                        }
                    }
                    fclose(branchesFile);
                }
                FILE *latestCommitFile = fopen(".gut/BRANCHES", "r");
                if (latestCommitFile != NULL)
                {
                    char latestCommit[256];
                    if (fgets(latestCommit, sizeof(latestCommit), latestCommitFile) != NULL)
                    {
                        latestCommit[strcspn(latestCommit, "\n")] = '\0';
                        FILE *branchesFile = fopen(".gut/BRANCHES", "a");
                        if (branchesFile != NULL)
                        {
                            fprintf(branchesFile, "%s:%s\n", branchName, latestCommit);
                            fclose(branchesFile);
                        }
                    }
                    fclose(latestCommitFile);
                }
                FILE *newHeadFile = fopen(".gut/HEAD", "w");
                if (newHeadFile != NULL)
                {
                    fprintf(newHeadFile, "%s\n", branchName);
                    fclose(newHeadFile);
                }
            }
            fclose(headFile);
        }
    }
}

void handle_checkout_command(const char *args[], int args_count)
{
    if (args_count != 3)
    {
        printf("Error: Invalid number of arguments for checkout command.\n");
        return;
    }

    const char *branchOrCommitId = args[2];

    // Check if the input is a commit ID
    if (is_commit_id(branchOrCommitId))
    {
        // Replace all files in the commit_id folder with the source files
        replace_files_with_commit(branchOrCommitId);
    }
    else
    {
        // Move all the branch's commit_id files to the source
        move_branch_files_to_source(branchOrCommitId);
    }
}

int is_commit_id(const char *branchOrCommitId)
{
    char *path = findRepository();
    if (strcmp(path, "") == 0)
    {
        printf("gut repository not found.\n");
        return 1;
    }

    char commitsPath[1024];
    snprintf(commitsPath, sizeof(commitsPath), "%s/.gut/commits", path);
    DIR *dir = opendir(commitsPath);
    if (dir != NULL)
    {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }

            if (strcmp(entry->d_name, branchOrCommitId) == 0)
            {
                closedir(dir);
                return 1;
            }
        }
        closedir(dir);
    }

    return 0;
}

void replace_files_with_commit(const char *commitId)
{
    char *path = findRepository();
    if (strcmp(path, "") == 0)
    {
        printf("gut repository not found.\n");
        return;
    }

    char commitPath[1024];
    snprintf(commitPath, sizeof(commitPath), "%s/.gut/commits/%s", path, commitId);
    DIR *dir = opendir(commitPath);
    if (dir != NULL)
    {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }

            char sourcePath[1024];
            snprintf(sourcePath, sizeof(sourcePath), "%s/%s", commitPath, entry->d_name);

            char destinationPath[1024];
            snprintf(destinationPath, sizeof(destinationPath), "%s/%s", path, entry->d_name);

            if (replace_file(sourcePath, destinationPath) != 0)
            {
                perror("Error replacing file with commit file");
                return;
            }
        }
        closedir(dir);
    }
}

int replace_file(const char *source, const char *destination)
{
    if (remove(destination) != 0)
    {
        perror("Error removing file");
        return 1;
    }

    if (copyFile(source, destination) != 0)
    {
        perror("Error copying file");
        return 1;
    }

    return 0;
}

void move_branch_files_to_source(const char *branch)
{
    char *path = findRepository();
    if (strcmp(path, "") == 0)
    {
        printf("gut repository not found.\n");
        return;
    }

    char branchPath[1024];
    snprintf(branchPath, sizeof(branchPath), "%s/.gut/BRANCHES", path);
    FILE *branchesFile = fopen(branchPath, "r");
    if (branchesFile != NULL)
    {
        char branchLine[256];
        while (fgets(branchLine, sizeof(branchLine), branchesFile) != NULL)
        {
            char *branchName = strtok(branchLine, ":");
            if (strcmp(branchName, branch) == 0)
            {
                char *commitId = strtok(NULL, ":");
                char commitPath[1024];
                snprintf(commitPath, sizeof(commitPath), "%s/.gut/commits/%s", path, commitId);
                DIR *dir = opendir(commitPath);
                if (dir != NULL)
                {
                    struct dirent *entry;
                    while ((entry = readdir(dir)) != NULL)
                    {
                        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                        {
                            continue;
                        }

                        char sourcePath[1024];
                        snprintf(sourcePath, sizeof(sourcePath), "%s/%s", commitPath, entry->d_name);

                        char destinationPath[1024];
                        snprintf(destinationPath, sizeof(destinationPath), "%s/%s", path, entry->d_name);

                        if (replace_file(sourcePath, destinationPath) != 0)
                        {
                            perror("Error replacing file with commit file");
                            return;
                        }
                    }
                    closedir(dir);
                }
            }
        }
        fclose(branchesFile);
    }
}


void handle_commit_command(const char *args[], int args_count)
{
    if (args_count != 3)
    {
        printf("Error: Invalid number of arguments for commit command.\n");
        return;
    }

    const char *commitMessage = args[2];
    size_t messageLength = strlen(commitMessage);

    if (messageLength > MAX_COMMIT_MESSAGE_LENGTH)
    {
        printf("Error: Commit message exceeds maximum length of %d characters.\n", MAX_COMMIT_MESSAGE_LENGTH);
        return;
    }

    char commitFolder[256];
    time_t currentTimestamp = time(NULL);
    snprintf(commitFolder, sizeof(commitFolder), ".gut/commits/%ld", currentTimestamp);

    if (mkdir(commitFolder, 0777) != 0)
    {
        printf("Error: Failed to create commit folder.\n");
        return;
    }

    char *path = findRepository();
    if (strcmp(path, "") == 0)
    {
        printf("gut repository not found.\n");
        return;
    }

    char stagePath[1024];
    snprintf(stagePath, sizeof(stagePath), "%s/.gut/%s", path, stagefolder);
    DIR *dir = opendir(stagePath);
    if (dir != NULL)
    {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }

            char sourcePath[1024];
            snprintf(sourcePath, sizeof(sourcePath), "%s/%s", stagePath, entry->d_name);

            char destinationPath[1024];
            snprintf(destinationPath, sizeof(destinationPath), "%s/%s", commitFolder, entry->d_name);

            if (rename(sourcePath, destinationPath) != 0)
            {
                perror("Error moving file from STAGE to commit folder");
                return;
            }
        }
        closedir(dir);
    }

    FILE *headFile = fopen(".gut/HEAD", "r");
    if (headFile == NULL)
    {
        printf("Error: Failed to open HEAD file.\n");
        return;
    }

    char currentBranch[256];
    if (fgets(currentBranch, sizeof(currentBranch), headFile) == NULL)
    {
        printf("Error: Failed to read current branch from HEAD file.\n");
        fclose(headFile);
        return;
    }

    fclose(headFile);

    FILE *branchesFile = fopen(".gut/BRANCHES", "r+");
    if (branchesFile == NULL)
    {
        printf("Error: Failed to open BRANCHES file.\n");
        return;
    }

    char branchLine[256];
    char updatedBranchLine[256];
    snprintf(updatedBranchLine, sizeof(updatedBranchLine), "%s:%s\n", currentBranch, commitFolder);

    FILE *tempFile = tmpfile();
    if (tempFile == NULL)
    {
        printf("Error: Failed to create temporary file.\n");
        fclose(branchesFile);
        return;
    }

    while (fgets(branchLine, sizeof(branchLine), branchesFile) != NULL)
    {
        if (strncmp(branchLine, currentBranch, strlen(currentBranch)) == 0)
        {
            fputs(updatedBranchLine, tempFile);
        }
        else
        {
            fputs(branchLine, tempFile);
        }
    }

    rewind(tempFile);
    rewind(branchesFile);

    int c;
    while ((c = fgetc(tempFile)) != EOF)
    {
        fputc(c, branchesFile);
    }

    fclose(tempFile);
    fclose(branchesFile);

    FILE *logFile = fopen(".gut/LOGFILE", "a");
    if (logFile == NULL)
    {
        printf("Error: Failed to open LOGFILE.\n");
        return;
    }

    fprintf(logFile, "%ld:%s:%s\n", currentTimestamp, currentBranch, commitMessage);
    fclose(logFile);

    printf("Commit successful.\n");
}

void handle_log_command(const char *args[], int args_count)
{
}

void handle_status_command(const char *args[], int args_count)
{
}
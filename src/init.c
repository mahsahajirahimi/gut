#include <stdio.h>
#include "init.h"
#include <string.h>
#include "uttil.h"
#include <sys/stat.h>
#include <sys/types.h>

const char *folderName = ".gut";
const char *stagefolder = "STAGE";

void handle_init_command() {
    char startDir[1024];
    if (getcwd(startDir, sizeof(startDir)) == NULL) {
         perror("Error getting current directory");
         return;
    }
    char *path = find_repository(startDir,folderName);
    if (strcmp(path,"")!=0) {
         printf("gut repository exist in %s",path);
    } else {
        if (mkdir(folderName, 0777) == 0) {
            printf(folderName);
            printf("Folder '%s' created successfully in the current directory.\n", folderName);
        } else {
            perror("Error creating folder");
            return;
        }
    }
}

void handle_config(const char *args[], int args_count) {
    if (args_count>2)
    {
        if (strcmp(args[2],"--global")==0)
        {
            
        }
        
    }
    
}

   void handle_add_command(const char *args[], int args_count) {
        
    }
    void handle_reset_command(const char *args[], int args_count) {

    }
    void handle_status_command(const char *args[], int args_count) {

    }
    void handle_commit_command(const char *args[], int args_count) {

    }
    void handle_log_command(const char *args[], int args_count) {

    }
    void handle_branch(const char *args[], int args_count) {

    }
    void handle_checkout_command(const char *args[], int args_count) {

    }

#include <stdio.h>
#include <string.h>
#include "command_handler.h"
#include "init.h"

void handle_command(const char *args[], int args_count) {
    if (args_count > 1) {
        if (strcmp(args[1], "init") == 0) {
            handle_init_command();
        } else if (strcmp(args[1], "config") == 0) {
            handle_config(args,args_count);
        } else if (strcmp(args[1], "add") == 0) {
            handle_add_command(args,args_count);
        } else if (strcmp(args[1], "reset") == 0) {
            handle_reset_command(args,args_count);
        } else if (strcmp(args[1], "status") == 0) {
            handle_status_command(args,args_count);
        } else if (strcmp(args[1], "commit") == 0) {
            handle_commit_command(args,args_count);
        } else if (strcmp(args[1], "log") == 0) {
            handle_log_command(args,args_count);
        } else if (strcmp(args[1], "branch") == 0) {
            handle_branch(args,args_count);
        } else if (strcmp(args[1], "checkout") == 0) {
            handle_checkout_command(args,args_count);
        }
    }
}


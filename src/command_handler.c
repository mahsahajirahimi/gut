#include <stdio.h>
#include <string.h>
#include "command_handler.h"
#include "init.h"

void handle_command(const char *args[], int args_count)
{
    if (args_count > 1)
    {
        const char *command = args[1];
        if (strcmp(command, "init") == 0)
        {
            handle_init_command();
        }
        else if (strcmp(command, "config") == 0)
        {
            handle_config(args, args_count);
        }
        else if (strcmp(command, "add") == 0)
        {
            handle_add_command(args, args_count);
        }
        else if (strcmp(command, "reset") == 0)
        {
            handle_reset_command(args, args_count);
        }
        else if (strcmp(command, "status") == 0)
        {
            handle_status_command(args, args_count);
        }
        else if (strcmp(command, "commit") == 0)
        {
            handle_commit_command(args, args_count);
        }
        else if (strcmp(command, "log") == 0)
        {
            handle_log_command(args, args_count);
        }
        else if (strcmp(command, "branch") == 0)
        {
            handle_branch(args, args_count);
        }
        else if (strcmp(command, "checkout") == 0)
        {
            handle_checkout_command(args, args_count);
        }
    }
}

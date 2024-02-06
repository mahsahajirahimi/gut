#ifndef INIT_H
#define INIT_H
    void handle_init_command();
    void handle_config(const char *args[], int args_count);
    void handle_add_command(const char *args[], int args_count);
    void handle_reset_command(const char *args[], int args_count);
    void handle_status_command(const char *args[], int args_count);
    void handle_commit_command(const char *args[], int args_count);
    void handle_log_command(const char *args[], int args_count);
    void handle_branch(const char *args[], int args_count);
    void handle_checkout_command(const char *args[], int args_count);    
#endif
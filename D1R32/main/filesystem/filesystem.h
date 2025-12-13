#pragma once


/* INCLUDES */
//Standart Includes
#include <stdbool.h>

/* MACROS */
/**
* @brief All teh directories have full on access to everyone by default
*        Simplifying, not adding chmod etc in my commands.
*/
#define DIR_MODE 0777

// Functions
/* The base, filesystem handling */
void select_command(char *command, char *curr_dir);

void perform_command(bool (*command)(char *, char *, char *, const char *),
                     char *curr_dir, const char *args);

void path_join(char *out, int size, const char *base, const char *rel);


/* The individual commands */
// Commands about statistics
bool cmd_help(char *res, char *err, char *curr_dir, const char *args);
bool cmd_df(char *res, char *err, char *curr_dir, const char *args);
bool cmd_stat(char *res, char *err, char *curr_dir, const char *args);

// Commands about moving around
bool cmd_cd(char *res, char *err, char *curr_dir, const char *args);

// Commands about creating/destroying
bool cmd_mkdir(char *res, char *err, char *curr_dir, const char *args);
bool cmd_rmdir(char *res, char *err, char *curr_dir, const char *args);
bool cmd_touch(char *res, char *err, char *curr_dir, const char *args);
bool cmd_rm(char *res, char *err, char *curr_dir, const char *args);

// Commands for viewing content
bool cmd_pwd(char *res, char *err, char *curr_dir, const char *args);
bool cmd_ls(char *res, char *err, char *curr_dir, const char *args);
bool cmd_cat(char *res, char *err, char *curr_dir, const char *args);

// Commands for changing content
bool cmd_mv(char *res, char *err, char *curr_dir, const char *args);
bool cmd_write(char *res, char *err, char *curr_dir, const char *args);
bool cmd_append(char *res, char *err, char *curr_dir, const char *args);
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
/**
* @brief Receiving a command name, select the function related to it and run it.
*
* @param command The name of the command
* @param curr_dir The name of teh current directory
*/
void select_command(char *command, char *curr_dir);
/**
* @brief Having recognized the command, perform it.
*
* @param command Pointer to the function of the command
* @param curr_dir The name of teh current directory
* @param args The arguments of the command
*/
void perform_command(bool (*command)(char *, char *, char *, const char *),
                     char *curr_dir, const char *args);
/**
* @brief Join base path to its expansion.
*
* @param out Where to save the new path
* @param size Size of the output holder
* @param base The current directory to which to attach
* @param rel The path to append to the @p base
*/
void path_join(char *out, int size, const char *base, const char *rel);


/* The individual commands */
// Commands about statistics
/**
* @brief Print available commands (help)
*
* @param res Where to save the command's result
* @param err Where to sa an error message
* @param curr_dir The current directory
* @param args Arguments for the command (empty string)
*
* @return True if the command succeed, False otherwise.
*/
bool cmd_help(char *res, char *err, char *curr_dir, const char *args);
/**
* @brief Basic statistics of the disk (df)
*
* @param res Where to save the command's result
* @param err Where to sa an error message
* @param curr_dir The current directory
* @param args Arguments for the command (empty string)
*
* @return True if the command succeed, False otherwise.
*/
bool cmd_df(char *res, char *err, char *curr_dir, const char *args);
/**
* @brief Basic info about a file/directory (stat file/dir)
*
* @param res Where to save the command's result
* @param err Where to sa an error message
* @param curr_dir The current directory
* @param args Arguments for the command (Name of file/dir)
*
* @return True if the command succeed, False otherwise.
*/
bool cmd_stat(char *res, char *err, char *curr_dir, const char *args);

// Commands about moving around
/**
* @brief Move to another directory (cd dir)
*
* @param res Where to save the command's result
* @param err Where to sa an error message
* @param curr_dir The current directory
* @param args Arguments for the command (the directory name)
*
* @return True if the command succeed, False otherwise.
*/
bool cmd_cd(char *res, char *err, char *curr_dir, const char *args);

// Commands about creating/destroying
/**
* @brief Create a new directory (mkdir new_dir)
*
* @param res Where to save the command's result
* @param err Where to sa an error message
* @param curr_dir The current directory
* @param args Arguments for the command (name of the dir to create)
*
* @return True if the command succeed, False otherwise.
*/
bool cmd_mkdir(char *res, char *err, char *curr_dir, const char *args);
/**
* @brief Remove a directory (rmdir new_dir)
*
* @param res Where to save the command's result
* @param err Where to sa an error message
* @param curr_dir The current directory
* @param args Arguments for the command (the dir to remove)
*
* @return True if the command succeed, False otherwise.
*/
bool cmd_rmdir(char *res, char *err, char *curr_dir, const char *args);
/**
* @brief Create a new file (touch new_file)
*
* @param res Where to save the command's result
* @param err Where to sa an error message
* @param curr_dir The current directory
* @param args Arguments for the command (the name of the new file)
*
* @return True if the command succeed, False otherwise.
*/
bool cmd_touch(char *res, char *err, char *curr_dir, const char *args);
/**
* @brief Remove a file (rm new_file)
*
* @param res Where to save the command's result
* @param err Where to sa an error message
* @param curr_dir The current directory
* @param args Arguments for the command (the file to remove)
*
* @return True if the command succeed, False otherwise.
*/
bool cmd_rm(char *res, char *err, char *curr_dir, const char *args);

// Commands for viewing content
/**
* @brief Print current working path (pwd)
*
* @param res Where to save the command's result
* @param err Where to sa an error message
* @param curr_dir The current directory
* @param args Arguments for the command (empty string)
*
* @return True if the command succeed, False otherwise.
*/
bool cmd_pwd(char *res, char *err, char *curr_dir, const char *args);
/**
* @brief List content of a directory (ls nothing/dir_path)
*
* @param res Where to save the command's result
* @param err Where to sa an error message
* @param curr_dir The current directory
* @param args Arguments for the command (empty string or dir name)
*
* @return True if the command succeed, False otherwise.
*/
bool cmd_ls(char *res, char *err, char *curr_dir, const char *args);
/**
* @brief See content of a file (cat file_name)
*
* @param res Where to save the command's result
* @param err Where to sa an error message
* @param curr_dir The current directory
* @param args Arguments for the command (the file name)
*
* @return True if the command succeed, False otherwise.
*/
bool cmd_cat(char *res, char *err, char *curr_dir, const char *args);

// Commands for changing content
/**
* @brief Rename a file (mv file_a file_b)
*
* @param res Where to save the command's result
* @param err Where to sa an error message
* @param curr_dir The current directory
* @param args Arguments for the command (name of the original and its new name)
*
* @return True if the command succeed, False otherwise.
*/
bool cmd_mv(char *res, char *err, char *curr_dir, const char *args);
/**
* @brief Write to a file (write file content). Will delete previous file content
*
* @param res Where to save the command's result
* @param err Where to sa an error message
* @param curr_dir The current directory
* @param args Arguments for the command (the file name and the content to write)
*
* @return True if the command succeed, False otherwise.
*/
bool cmd_write(char *res, char *err, char *curr_dir, const char *args);
/**
* @brief Append to a file (append file content)
*
* @param res Where to save the command's result
* @param err Where to sa an error message
* @param curr_dir The current directory
* @param args Arguments for the command (the file name and the content to append)
*
* @return True if the command succeed, False otherwise.
*/
bool cmd_append(char *res, char *err, char *curr_dir, const char *args);
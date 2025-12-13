#pragma once


/* INCLUDES */
//Standart Includes
#include <stdbool.h>


// Functions
/* The base, filesystem handling */
void select_command(char *command, char *curr_dir);

void perform_command(bool (*command)(char *, char *));


/* The individual commands */
bool cmd_df(char *res, char *err);
#pragma once


/* INCLUDES */
//Standart Includes
#include <stdbool.h>


/* PROTOTYPES */
void select_command(char *command, char *curr_dir);

void perform_command(bool (*command)(char *, char *));

bool cmd_df(char *res, char *err);
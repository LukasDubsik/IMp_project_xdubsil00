//User defined Includes
#include "helper_functions.h"

//Standart Includes
#include <string.h>
#include <ctype.h>


void trim(char *inp){
    //Where the string starts
    char *start = inp;
    //Its end
    char *end = start + strlen(start);
    //Empty string
    if (end == start) return;

    end--;
    //Iterate until we reach point where there are no whitespaces
    while (end >= start && isspace((unsigned char)*end)) {
        end--;
    }

    //Set the last char as end of string
    end[1] = '\0';
}
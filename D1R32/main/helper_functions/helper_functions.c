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
    //Its position - disregarding '\0'
    size_t pos = strlen(start) - 1;

    //Iterate until we reach point where there are no whitespaces
    while (end > start && isspace(end[pos])) {
        end--;
    }

    //Set the last char as end of string
    inp[pos + 1] = '\0';
}
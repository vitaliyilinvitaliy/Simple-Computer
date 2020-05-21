#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "mySimpleComputer.h"
#include "myTerm.h"


#define SUCCESSFULLY 0
#define INVALID_NUMBER_OF_ARGUMENTS 1
#define INVALID_SIZE_OF_ARGUMENTS 2
#define WRONG_EXTENSION 3
#define FILE_NOT_FOUND 4
#define FILE_OPEN 5
#define COULD_NOT_OPEN_OR_CREATE_FILE 6
#define EMPTY_FILE 7
#define COMMAND_NOT_FOUND 8
#define ERROR_ENCODE 9
#define INCORRECT_NUMBER_FORMAT 10
#define TOO_FEW_COMMANDS 11
#define DIGIT_TOO_BIG 12

int RAM[100];

struct command_system
{
    int operand : 16;
    int com : 7;
};






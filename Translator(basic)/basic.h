#include <stdio.h>
#include <stdlib.h>
//#include <stddef.h>
#include <string.h>
#include "myTerm.h"
#include <stdbool.h>

#define TOO_FEW_ARGUMENTS 0
#define INVALID_SIZE_OF_ARGUMENTS 1
#define FILE_NOT_FOUND 2
#define SUCCESSFULLY 3
#define WRONG_EXTENSION 4
#define FILE_IS_FOUND 5
#define FILE_IS_NOT_OPEN 6
#define MEMORY_ERROR 7
#define WRONG_LINE_ORDER 8
#define MISSING_COMMAND_OR_PARAMETERS 9
#define WRONG_REGISTER 10
#define COMMAND_NOT_FOUND 11
#define MEMORY_OVERLOAD 12
#define LINE_NOT_FOUND 13


#define INPUT 0x0014
#define PRINT 0x0015
#define GOTO 0x0016
#define IF 0x0017
#define LET 0x0018
#define REM 0x0019
#define END 0x001A




struct stack{
    int number;
    struct stack *prev;
};

struct basic_line
{
    int number_line;
    char command[256];
    char parameters[256];
    int operand;
};

struct asm_line
{
    int number_cell;
    char command[256];
    int *operand;
    int value;
};



#pragma once
#include "mySimpleComputer.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "myReadKey.h"
#include "myBigChars.h"

int ALU(int command, int operand);
int read_value(int operand);
int CU(void);
int check16(char *number);
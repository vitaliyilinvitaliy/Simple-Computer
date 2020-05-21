#pragma once
#include "myTerm.h"
#include "mySimpleComputer.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct myBigChars
{
    long int Zero[2];// = {1382171196, 1010976330};
    long int One[2];// = {269752336, 940576784};
    long int Two[2];// = {270804024, 1006896136};
    long int Three[2];// = {940580924, 943998016};
    long int Four[2];// = {1041372184, 940576784};
    long int Five[2];// = {1077675068, 943998016};
    long int Six[2];// = {67388536, 943998012};
    long int Seven[2];// = {135274556, 67372036};
    long int Eight[2];// = {1010975292, 1010975298};
    long int Nine[2];// = {2084717116, 1010843712};
    long int Plus[2];// = {4279769112, 404232447};
    long int A[2];// = {1009001496, 606348324};
    long int B[2];// = {1007952924, 1011106884};
    long int C[2];// = {33694744, 405012994};
    long int D[2];// = {1145316380, 472138820};
    long int E[2];// = {470025276, 1006896132};
    long int F[2];// = {470025276, 67372036};
    long int Minus[2];// = {4278190080,255};
};

static int TerminalWrite[3];

int bc_printA(char *str);
int bc_box(int x1, int y1, int x2, int y2);
int bc_printbigchar(long int *, int x, int y, enum colors, enum colors);
int bc_setbigcharpos(int *big, int x, int y, int value);
int bc_getbigcharpos(int *big, int x, int y, int *value);
int bc_bigcharwrite(int fd, long int *big, int count);
int bc_bigcharread(int fd, long int *big, int need_count, int *count);
int bc_drawInterfase(int);
void bc_setvaluebigchar(char, long int *);
void bc_drawdigit(int digit);
void bc_settextterminal(int, int);
void bc_printtextterminal(void);
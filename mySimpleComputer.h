#pragma once
#include <stddef.h>
#include <stdio.h>
#include "myTerm.h"

#define MEMORY_OVERFLOW 0       //переполнение при выполнении операции
#define DIVISION_ERROR_BY_0 1   //ошибка деления на ноль
#define BORDER_OVERLOAD_ERROR 2 //ошибка выхода за границы памяти
#define IGNORING_TACT_PULSES 3  //игнорирование тактовых импульсов
#define CODE_ERROR 4            //не верная команда

static int RandomAccessMemory[100];

struct mySimpleComputer
{
    int register_flag : 5;
    int accumulator : 15;
    int instruction_counter : 16;
};

int sc_memoryInit();
int sc_memorySet(int, int);
int sc_memoryGet(int, int *);

int sc_memorySave(char *);
int sc_memoryLoad(char *);

int sc_commandEncode(int, int, int *);
int sc_commandDecode(int, int *, int *);

int sc_printMemory(int);
void sc_printRegFlag(void);

int sc_regInit(void);
int sc_regSet(int, int);
int sc_regGet(int, int *);

int sc_accInit(void);
int sc_accSet(int);
int sc_accGet(int *);
void sc_printAccum(void);

int sc_instrcInit(void);
int sc_instrcSet(int);
int sc_instrcGet(int *);
void sc_printInstrCount(void);

void sc_printOper(void);
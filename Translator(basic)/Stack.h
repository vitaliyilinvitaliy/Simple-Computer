#pragma once 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

struct stack{
    char Litter[256];
    struct stack *prev;
};

int push(char data[], struct stack **head_stack);
void pop_del(struct stack **head_stack,char ret[]);
void pop_get(struct stack *head_stack,char ret[]);
void print_stack(const struct stack *head);


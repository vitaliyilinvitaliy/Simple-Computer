#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



struct stack{
    char Litter[256];
    int operand;
    struct stack *prev;
    int *store_acc;
};

int conversion_rpn(const char *fromcalculate, struct stack **head_stack);
int priority(char Sign);

int push(char data[], struct stack **head_stack,int *operand);
int pop_del(struct stack **head_stack,char ret[],int** store);
int pop_get(struct stack *head_stack,char ret[]);
void print_stack(const struct stack *head);
int del(struct stack **head_stack);
void fill_operand(struct stack **head_stack);
int check_empty_stack(struct stack *calculate);
int size_stack(struct stack *head_stack);
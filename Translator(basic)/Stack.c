#include "Stack.h"


int push(char data[], struct stack **head_stack)
{
    struct stack *Node = (struct stack *)malloc(sizeof(struct stack));
    if (Node == NULL)
    {
        return 1;     
    }
    Node->prev = *head_stack;
    strcpy(Node->Litter,data);
    *head_stack = Node;
    return 0;
}

void pop_del(struct stack **head_stack,char ret[])
{
    struct stack *Node = NULL;
    if (*head_stack != NULL)
    {
        Node = *head_stack;
        strcpy(ret,(*head_stack)->Litter);
        (*head_stack) = (*head_stack)->prev;
        free(Node);
    }
}

void pop_get(struct stack *head_stack,char ret[]){
    if(head_stack != NULL){
        strcpy(ret,head_stack->Litter);
    }
}

void print_stack(const struct stack *head)
{
    while (head)
    {
        printf("%s ", head->Litter);
        head = head->prev;
    }
}
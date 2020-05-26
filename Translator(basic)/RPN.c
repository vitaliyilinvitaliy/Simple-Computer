#include "RPN.h"

int priority(char Sign)
{
    switch (Sign)
    {
    case '+':
        return 1;
    case '-':
        return 1;
    case '*':
        return 2;
    case '/':
        return 2;
    case '(':
        return 0;
    case ')':
        return 0;
    default:
        return 0;
    }
}

int conversion_rpn(const char *fromcalculate, struct stack **let)
{
    struct stack *SingStack = NULL;
    struct stack *Calculate = NULL;
    //printf("%s\n",fromcalculate);
    int *store = NULL;
    char copy[256][256];
    int j = 0;
    for (int i = 0; i < strlen(fromcalculate); j++, i++)
    {
        int k = 0;
        while (fromcalculate[i] != ' ' && fromcalculate[i] != '\t' && i < 255)
        {
            if (j > 1)
            {
                copy[j - 2][k] = fromcalculate[i];
            }
            i++;
            k++;
            copy[j - 2][k] = '\0';
        }
        //printf("\n");
    }
    bool flag = false;

    for (int i = 0; i < j - 2; i++)
    {
        if ((copy[i][0] >= 'A' && copy[i][0] <= 'Z') || (copy[i][0] >= '0' && copy[i][0] <= '9'))
        {
            push(copy[i], &Calculate,NULL);
        }
        else
        {
            if (copy[i][0] == '*' || copy[i][0] == '/' || copy[i][0] == '+' || copy[i][0] == '-')
            {
                char get_st[256] = {0};
                pop_get(SingStack, get_st);
                if (strlen(get_st) == 1)
                {
                    int pr1 = priority(get_st[0]);
                    int pr2 = priority(copy[i][0]);
                    if (pr1 == pr2)
                    {
                        push(get_st, &Calculate,NULL);
                        pop_del(&SingStack, get_st,&store);
                    }
                    push(copy[i], &SingStack,NULL);
                }
                else
                {
                    push(copy[i], &SingStack,NULL);
                }
            }
            else
            {
                if (copy[i][0] == '(')
                {
                    push(copy[i], &SingStack,NULL);
                }
                else
                {
                    if (copy[i][0] == ')')
                    {
                        char get_st[256] = {0};
                        do
                        {
                            if (strlen(get_st) == 1)
                            {
                                push(get_st, &Calculate,NULL);
                            }
                            pop_del(&SingStack, get_st,&store);
                        } while (get_st[0] != '(');
                    }
                }
            }
        }
    }
    char get_st[256] = {0};
    while (!pop_del(&SingStack, get_st,&store))
    {
        push(get_st, &Calculate,NULL);
    }
    
    del(let);
   
    while (!pop_del(&Calculate, get_st,&store))
    {
        push(get_st,let,NULL);
    }
    
}

int push(char data[], struct stack **head_stack,int *operand)
{
    struct stack *Node = (struct stack *)malloc(sizeof(struct stack));
    if (Node == NULL)
    {
        return 1;
    }
    Node->prev = *head_stack;
    Node->store_acc = operand;
    strcpy(Node->Litter, data);
    *head_stack = Node;
    return 0;
}

int pop_del(struct stack **head_stack, char ret[],int **store)
{
    struct stack *Node = NULL;
    if (*head_stack != NULL)
    {
        Node = *head_stack;
        strcpy(ret, (*head_stack)->Litter);
        *store= (*head_stack)->store_acc;
        (*head_stack) = (*head_stack)->prev;
        free(Node);
        return 0;
    }
    else
    {
        return 1;
    }
}

int pop_get(struct stack *head_stack, char ret[])
{
    if (head_stack != NULL)
    {
        strcpy(ret, head_stack->Litter);
        return 0;
    }else
    {
        return 1;
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

int del(struct stack **head_stack)
{
    if (*head_stack != NULL)
    {
        while (*head_stack != NULL)
        {
            struct stack *Node;
            Node = *head_stack;
            *head_stack = (*head_stack)->prev;
            free(Node);
        }
        return 0;
    }
    return 1;
}

int check_empty_stack(struct stack *calculate){
    if(calculate == NULL){
        return 1;
    }else return 0;
}
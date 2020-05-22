#include "basic.h"

struct asm_line asm_code[100];
int RAM[100] = {0};
int perem[26] = {0};
int perem_adress = 99;
int RAM_adress = 0;

void error_print(char text[])
{
    mt_setfgcolor(red);
    printf("%s\n", text);
    mt_setfgcolor(reset);
    exit(1);
}

void succes(char *text)
{
    mt_setfgcolor(green);
    printf("%s\n", text);
    mt_setfgcolor(reset);
}

void error_handler(int error)
{
    switch (error)
    {
    case TOO_FEW_ARGUMENTS:
    {
        error_print("TOO FEW ARGUMENTS");
        break;
    }
    case INVALID_SIZE_OF_ARGUMENTS:
    {
        error_print("INVALID SIZE OF NAME FILES\n");
        break;
    }
    case FILE_NOT_FOUND:
    {
        error_print("FILE NOT FOUND\n");
        break;
    }
    case SUCCESSFULLY:
    {
        succes("SUCCESSFULLY\n");
        break;
    }
    case WRONG_EXTENSION:
    {
        error_print("WRONG EXTENSION\n");
        break;
    }
    case FILE_IS_FOUND:
    {
        succes("FILE IS FOUND\n");
        break;
    }
    case FILE_IS_NOT_OPEN:
    {
        error_print("FILE IS NOT OPEN\n");
        break;
    }
    case MEMORY_ERROR:
    {
        error_print("COILD NOT ALLOCATE MEMORY\n");
        break;
    }
    case WRONG_LINE_ORDER:
    {
        error_print("WRONG LINE ORDER\n");
        break;
    }
    case MISSING_COMMAND_OR_PARAMETERS:
    {
        error_print("MISSING COMMAND OR PARAMETERS\n");
        break;
    }
    case WRONG_REGISTER:
    {
        error_print("WRONG_REGISTER\n");
        break;
    }
    case COMMAND_NOT_FOUND:
    {
        error_print("COMMAND_NOT_FOUND\n");
        break;
    }
    case MEMORY_OVERLOAD:
    {
        error_print("MEMORY OVERLOAD\n");
        break;
    }
    case LINE_NOT_FOUND:
    {
        error_print("LINE NOT FOUND\n");
        break;
    }
    case TOO_LONG_VARIABLE:{
        error_print("TOO LONG VARIABLE\n");
        break;
    }
    }
}

void push(int data, struct stack **head_stack)
{
    struct stack *Node = (struct stack *)malloc(sizeof(struct stack));
    if (Node == NULL)
    {
        error_handler(MEMORY_ERROR);
    }
    Node->prev = *head_stack;
    Node->number = data;
    *head_stack = Node;
}

int pop(struct stack **head_stack)
{
    struct stack *Node = NULL;
    int data = 0;
    if (*head_stack != NULL)
    {
        Node = *head_stack;
        data = (*head_stack)->number;
        (*head_stack) = (*head_stack)->prev;
        free(Node);
        return data;
    }
    return -1;
}

void print_stack(const struct stack *head)
{
    while (head)
    {
        printf("%d ", head->number);
        head = head->prev;
    }
}

int check_name_file(int argc, char *argv[])
{
    if (argc == 3)
    {
        size_t asem = strlen(argv[1]);
        size_t bas = strlen(argv[2]);
        if (bas < 5 || asem < 4)
        {
            error_handler(INVALID_SIZE_OF_ARGUMENTS);
        }
        else
        {
            char *expansion_asm = strrchr(argv[1], '.');
            char *expansion_bas = strrchr(argv[2], '.');

            if (!strcmp(expansion_asm, ".sa") && !strcmp(expansion_bas, ".bas"))
            {
                FILE *fp = fopen(argv[2], "r");
                if (fp == NULL)
                {
                    error_handler(FILE_NOT_FOUND);
                }
                else
                {
                    fclose(fp);
                    error_handler(FILE_IS_FOUND);
                }
            }
            else
            {
                error_handler(WRONG_EXTENSION);
            }
        }
    }
    else
    {
        error_handler(TOO_FEW_ARGUMENTS);
    }
}

int empty_str(const char *str)
{
    int counter = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
        {
            counter++;
        }
    }
    if (strlen(str) == counter)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int amount_line_in_file(char *bas_name)
{
    FILE *file_bas = NULL;
    file_bas = fopen(bas_name, "rt");

    char str[256];

    if (file_bas == NULL)
    {
        error_handler(FILE_IS_NOT_OPEN);
    }

    int amount = 0;
    while (fgets(str, sizeof(char) * 256, file_bas))
    {
        if (!empty_str(str))
        {
            amount++;
        }
    }
    fclose(file_bas);
    return amount;
}

char *get_parameters(char *a)
{
    int i = 0, j = 0, space = 0, flag = 0, stop = 2;

    while (a[i] != '\n' && a[i] != '\0')
    {
        if (space < stop)
        {
            if (a[i] == ' ' || a[i] == '\t')
                flag = 1;
            else if (flag == 1)
            {
                space++;
                flag = 0;
            }
        }
        if (space == stop)
        {
            return (&a[i]);
        }
        ++i;
    }
    return NULL;
}

int check_str_syntax(char *command, char *parameters)
{
    if (strlen(command) == 0 || strlen(parameters) == 0 && strcmp(command, "END") != 0)
    {
        error_handler(MISSING_COMMAND_OR_PARAMETERS);
    }
    for (int i = 0; i < strlen(command); i++)
    {
        if (command[i] < 'A' || command[i] > 'Z')
        {
            error_handler(WRONG_REGISTER);
        }
    }
}

void check_syntax(struct basic_line *basic_code, size_t n)
{
    int prev_number = 0;
    for (size_t i = 0; i < n; i++)
    {
        /* printf("%d %s %s\n", basic_code[i].number_line,
               basic_code[i].command,
               basic_code[i].parameters);*/
        if (i > 0)
        {
            if (basic_code[i].number_line <= basic_code[i - 1].number_line)
            {
                error_handler(WRONG_LINE_ORDER);
            }
        }
        check_str_syntax(basic_code[i].command, basic_code[i].parameters);
    }
}

int check_command(char *command)
{
    if (strcmp(command, "INPUT") == 0)
    {
        return 0x0014;
    }
    else if (strcmp(command, "PRINT") == 0)
    {
        return 0x0015;
    }
    else if (strcmp(command, "GOTO") == 0)
    {
        return 0x0016;
    }
    else if (strcmp(command, "IF") == 0)
    {
        return 0x0017;
    }
    else if (strcmp(command, "LET") == 0)
    {
        return 0x0018;
    }
    else if (strcmp(command, "REM") == 0)
    {
        return 0x0019;
    }
    else if (strcmp(command, "END") == 0)
    {
        return 0x001A;
    }
    else
    {
        error_handler(COMMAND_NOT_FOUND);
    }
}

int is_digit(char *dig)
{
    for (int i = 0; i < strlen(dig); i++)
    {
        if (dig[i] < '0' || dig[i] > '9')
        {
            return -1;
        }
    }
    return 0;
}

int *add_perem(char Litter, bool digit)
{
    int pos = Litter - 'A';

    if (pos >= 0 && pos < 26 && Litter <= 'Z' && Litter >= 'A' && !digit)
    {

        if (perem[pos] == 0)
        {
            perem[pos] = perem_adress;
            RAM[perem_adress] = perem_adress;
            perem_adress--;
        }

        return &perem[pos];
    }
    else
    {
        if (digit)
        {

            RAM[perem_adress] = perem_adress;
            int copy = perem_adress;
            perem_adress--;
            return &RAM[copy];
        }
    }
    return NULL;
}

int add_command(char command_asm[], int *operand, struct basic_line *basic_l, int number_c, int value, bool str_b)
{
    asm_code[RAM_adress].number_cell = number_c;
    strcpy(asm_code[RAM_adress].command, command_asm);
    asm_code[RAM_adress].operand = operand;

    if (value >= -0x3FFF && value <= 0x3FFF)
    {
        asm_code[RAM_adress].value = value;
    }
    else
    {
        exit(1);
    }
    if (str_b)
    {
        (*basic_l).operand = RAM_adress;
    }
    RAM_adress++;
}




int add_asm_line(char command[], char parameters[], struct basic_line *basic_line, const int number, const size_t n,bool fl_b)
{
    int code_com = check_command(command);
    char A[20], Sign[20], B[20];
    int *adress_d;
    int adr = 0;
    char com_if[256]={0},param_if[256]={0};
    switch (code_com)
    {
    case 0x0014:
    {
        /* INPUT */
        add_command("READ", add_perem(parameters[0], false), &basic_line[number], RAM_adress, 0, fl_b);
        break;
    }
    case 0x0015:
    {
        /* PRINT */
        add_command("WRITE", add_perem(parameters[0], false), &basic_line[number], RAM_adress, 0, fl_b);
        break;
    }
    case 0x0016:
    {
        /* GOTO */
        adr = atoi(parameters);
        bool flag = false;
        for (int j = 0; j < n; j++)
        {
            if (basic_line[j].number_line == adr)
            {
                add_command("JUMP", &basic_line[j].operand, &basic_line[number], RAM_adress, 0, fl_b);
                flag = true;
                break;
            }
        }
        if (!flag)
        {
            error_handler(LINE_NOT_FOUND);
        }
        break;
    }
    case 0x0017:
    {
        /* IF */
        sscanf(parameters, "%s %s %s", A, Sign, B);

        if (is_digit(A) == 0)
        {
            adress_d = add_perem(' ', true);
            add_command("=", NULL, &basic_line[number], *adress_d, atoi(A), fl_b);
            add_command("LOAD", adress_d, &basic_line[number], RAM_adress, 0, false);
        }
        else
        {
            if (strlen(A) == 1)
            {
                adress_d = add_perem(A[0], false);
                printf("adr = %d\n", *adress_d);
                add_command("LOAD", adress_d, &basic_line[number], RAM_adress, 0, false);
            }
            else
            {
                printf("%s\n",A);
                error_handler(TOO_LONG_VARIABLE);
            }
        }
        if (is_digit(B) == 0)
        {
            adress_d = add_perem(' ', true);
            add_command("=", NULL, &basic_line[number], *adress_d, atoi(B), false);
            add_command("SUB", adress_d, &basic_line[number], RAM_adress, 0, false);
        }
        else
        {
            if (strlen(B) == 1)
            {
                adress_d = add_perem(B[0], false);
                printf("adr = %d\n", *adress_d);
                add_command("SUB", adress_d, &basic_line[number], RAM_adress, 0, false);
            }
            else
            {
                printf("%s\n",B);
                error_handler(TOO_LONG_VARIABLE);
            }
        }
        
        if_pars(com_if,param_if,parameters);
        int number_com = check_command(com_if);
        add_asm_line(com_if,param_if,basic_line,number,n,false);
        
        break;
    }

    case 0x0018:
        /* LET */
           
        break;
    case 0x0019:
        /* REM */

        break;
    case 0x001A:
        /* END */
        add_command("HALT", NULL, &basic_line[number], RAM_adress, 0, fl_b);
        break;

    default:
        error_handler(COMMAND_NOT_FOUND);
        break;
    }
}

void parser(struct basic_line *basic_line, size_t n)
{
    for (int i = 0; i < n; i++)
    {
        add_asm_line(basic_line[i].command, basic_line[i].parameters, basic_line, i, n,true);
    }
}

void dragging_out_line(char *name_file_asm, char *name_file_bas)
{
    FILE *file_bas = NULL;

    char str[256];
    int amount = amount_line_in_file(name_file_bas);
    printf("%d\n", amount);
    struct basic_line *basic_code = (struct basic_line *)calloc(amount + 1, sizeof(struct basic_line));
    if (basic_code == NULL)
    {
        error_handler(MEMORY_ERROR);
    }
    file_bas = fopen(name_file_bas, "rt");
    if (file_bas == NULL)
    {
        error_handler(FILE_IS_NOT_OPEN);
    }
    int index = 0;

    while (fgets(str, sizeof(char) * 256, file_bas))
    {
        char com[15] = {' '}, *param = NULL;
        int n = 0;
        if (!empty_str(str))
        {

            sscanf(str, "%d%s", &basic_code[index].number_line, basic_code[index].command);
            param = get_parameters(str);

            if (param != NULL)
            {
                int i = 0;
                for (i = 0; i < strlen(param); i++)
                {
                    basic_code[index].parameters[i] = param[i];
                }
                basic_code[index].parameters[i - 1] = '\0';
                basic_code[index].operand = -1;
            }
            else
            {
                basic_code[index].parameters[0] = '\0';
            }
            index++;
        }
    }
    fclose(file_bas);
    check_syntax(basic_code, amount);
    parser(basic_code, amount);
}

int main(int argc, char *argv[])
{
    check_name_file(argc, argv);
    dragging_out_line(argv[1], argv[2]);
    for (int i = 0; i < 15; i++)
    {
        printf("%d %s ", asm_code[i].number_cell, asm_code[i].command);
        if (asm_code[i].operand != NULL)
        {
            printf("%d", *asm_code[i].operand);
        }
        else
        {
            printf("%d", asm_code[i].value);
        }
        printf("\n");
    }
}

/*
    Проверки:
    1. на правильность аргументов командной строки
    2. на существования файла *.bas
    3. на возрастание номеров строк
    4. на наличие команды END
    5. верхнего регистра букв команд
    6. переменной на имя состоящее из одного символа (26 символов)
    7. правильности команды 
    8. на существование строки при GOTO
    9. на существование переменной при LET & PRINT
    10. синтаксис после команд
    11. правильность записи знаков < >  = * / + - (синтаксис)
    
    План работы:
    1. Стек
    2. Обратная польская запись 
    3. Получение строк из файла
    4. Парсинг строки
    5. Перевод в команды ассемблера

    |NUMBER|COMMAND|...|...|
    |40|LET|C = A – B|05|
    40 LET C = A – B


    10 REM  Это комментарий |10|REM|Это комментарий| |
    20 INPUT A              |20|INPUT|A|00|
    30 INPUT B              |30|INPUT|B|01|
    40 LET C = A – B        |40|LET|C = A – (B + D * 2)/F|02|
                                       
                                        A B D 2 * + F / -
                                        D2*
                                        LOAD D
                                        MUL 2
                                        STORE 99
                                        
    50 IF C < 0 GOTO 20
    60 PRINT C
    70 END
    
    6
    

    [10] 9
    [11] 12
    [12] 1
    [13] 3
    [14] 3
    [15] 5

    10 REM  Это комментарий
20 INPUT A
30 INPUT B
40 LET C = A – B
50 IF C < 0 GOTO 20
60 PRINT C
70 END


A < B
A-B<0
A>B
A > B
A-B>0
A = B
A-B=0 
*/

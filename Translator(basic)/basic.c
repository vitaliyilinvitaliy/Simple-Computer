#include "basic.h"

struct asm_line asm_code[100];
int RAM[100] = {0};
int perem[26] = {0};
int perem_adress = 98;

int RAM_adress = 0;
int *buf_obm = NULL;

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
    case TOO_LONG_VARIABLE:
    {
        error_print("TOO LONG VARIABLE\n");
        break;
    }
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

int check_sing(char sing)
{
    if (sing == '*' || sing == '/' || sing == '+' || sing == '-')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int lever_sign(int *operand, char sign, struct basic_line *basic_line, int number, bool flag_obm)
{
    switch (sign)
    {
    case '+':
        add_command("ADD", operand, &basic_line[number], RAM_adress, 0, false);
        break;
    case '-':
        if (flag_obm)
        {
            add_command("STORE", NULL, &basic_line[number], RAM_adress, 99, false);
            add_command("LOAD", operand, &basic_line[number], RAM_adress, 0, false);
            add_command("SUB", NULL, &basic_line[number], RAM_adress, 99, false);
        }
        else
        {
            add_command("SUB", operand, &basic_line[number], RAM_adress, 0, false);
        }

        break;
    case '/':
        if (flag_obm)
        {
            add_command("STORE", NULL, &basic_line[number], RAM_adress, 99, false);
            add_command("LOAD", operand, &basic_line[number], RAM_adress, 0, false);
            add_command("DIVIDE", NULL, &basic_line[number], RAM_adress, 99, false);
        }
        else
        {
            add_command("DIVIDE", operand, &basic_line[number], RAM_adress, 0, false);
        }

        break;
    case '*':
        add_command("MUL", operand, &basic_line[number], RAM_adress, 0, false);
        break;
    default:
        break;
    }
}

void overload_mem()
{
    if (RAM_adress + (100 - perem_adress) > 100)
    {
        error_handler(MEMORY_ERROR);
    }
}

int add_asm_line(char command[], char parameters[], struct basic_line *basic_line, const int number, const size_t n, bool fl_b)
{
    int code_com = check_command(command);
    char A[20], Sign[20], B[20];
    int *adress_d;
    int adr = 0;
    char com_if[256] = {0}, param_if[256] = {0}, let_line[256] = {0}, get_st[256]={0}, p1[256]={0}, p2[256]={0}, sign;
    struct stack *let = NULL, *calculate = NULL;

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
                add_command("LOAD", adress_d, &basic_line[number], RAM_adress, 0, false);
            }
            else
            {
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
                add_command("SUB", adress_d, &basic_line[number], RAM_adress, 0, false);
            }
            else
            {
                error_handler(TOO_LONG_VARIABLE);
            }
        }

        if (strlen(Sign) > 1)
        {
            exit(1);
        }
        switch (Sign[0])
        {
        case '>':
        {
            add_command("JNEG", &basic_line[number + 1].operand, basic_line, RAM_adress, 0, false);
            add_command("JZ", &basic_line[number + 1].operand, basic_line, RAM_adress, 0, false);
            break;
        }
        case '<':
        {
            add_command("JNS", &basic_line[number + 1].operand, basic_line, RAM_adress, 0, false);
            add_command("JZ", &basic_line[number + 1].operand, basic_line, RAM_adress, 0, false);
            break;
        }
        case '=':
        {
            add_command("JNEG", &basic_line[number + 1].operand, basic_line, RAM_adress, 0, false);
            add_command("JNS", &basic_line[number + 1].operand, basic_line, RAM_adress, 0, false);
            break;
        }
        default:
            exit(1);
            break;
        }
        if_pars(com_if, param_if, parameters);
        int number_com = check_command(com_if);
        add_asm_line(com_if, param_if, basic_line, number, n, false);

        break;
    }

    case 0x0018:
        /* LET */
        //printf("%s %s\n",command,parameters);
        sscanf(parameters, "%s %s", A, B);
        let_merge(parameters, let_line);
        buf_obm = add_perem(parameters[0], false);
        printf("%d %s\n", *buf_obm, let_line);
        if (calcpars(let_line) == 1)
        {
            exit(1);
        }
        conversion_rpn(parameters, &let);
        print_stack(let);
        printf("\n");
        bool flag_acc = false;
        int *store_adr1 = NULL, *store_adr2 = NULL, *store_adr = NULL, *adr_acc = NULL;
        while (!pop_del(&let, get_st,&store_adr))
        {
           if (!check_sing(get_st[0]))
            {
                push(get_st, &calculate, NULL);
                
            }
            else
            {
                sign = get_st[0];
            
                
                print_stack(calculate);
                printf("\n");
                
                pop_del(&calculate, p1, &store_adr1);
                pop_del(&calculate, p2, &store_adr2);
                printf("%s %s %c\n",p1,p2,get_st[0]);
                if(store_adr1!=NULL){
                    printf("%d ",*store_adr1);
                }
                if(store_adr2!=NULL){
                    printf("%d",*store_adr1);
                }
                printf("\n");
                if(store_adr2!=NULL){
                    
                }
                 int *adr_p1 = NULL, *adr_p2 = NULL;

               if ('A' <= p1[0] && p1[0] <= 'Z')
                {
                    adr_p1 = add_perem(p1[0], false);
                }
                else
                {
                    if ('0' <= p1[0] && p1[0] <= '9')
                    {
                        adr_p1 = add_perem(' ', true);
                        add_command("=", NULL, &basic_line[number], *adr_p1, atoi(p1), false);
                    }
                    else
                    {
                        if (strcmp(p1, "acc") == 0 && store_adr1 != NULL)
                        {
                            adr_p1 = store_adr1;
                        }
                    }
                }
                if ('A' <= p2[0] && p2[0] <= 'Z')
                {
                    adr_p2 = add_perem(p2[0], false);
                }
                else
                {
                    if ('0' <= p2[0] && p2[0] <= '9')
                    {
                        adr_p2 = add_perem(' ', true);
                        add_command("=", NULL, &basic_line[number], *adr_p2, atoi(p2), false);
                    }
                    else
                    {
                        if (strcmp(p2, "acc") == 0 && store_adr2 != NULL)
                        {      
                            adr_p2 = store_adr2;
                        }
                    }
                }
                if('0' <= p2[0] && p2[0] <= '9'&&'0' <= p1[0] && p1[0] <= '9'){
                    if(adr_acc!=NULL){
                        add_command("STORE", adr_acc, &basic_line[number], RAM_adress, 0, false);
                        
                    }
                    
                }
                

                if (strcmp(p1, "acc") != 0 && strcmp(p2, "acc") != 0)
                {
                    if (!flag_acc)
                    {
                        adr_acc = add_perem(' ', true);
                        flag_acc = true;
                        add_command("LOAD", adr_p2, &basic_line[number], RAM_adress, 0, false);
                        lever_sign(adr_p1, sign, basic_line, number, false);
                        push("acc", &calculate, adr_acc);
                    }
                    else
                    {
                        add_command("STORE", adr_acc, &basic_line[number], RAM_adress, 0, false);
                        add_command("LOAD", adr_p2, &basic_line[number], RAM_adress, 0, false);
                        adr_acc = add_perem(' ', true);
                        push("acc", &calculate, adr_acc);
                        lever_sign(adr_p1, sign, basic_line, number, false);
                        flag_acc = false;
                    }
                       
                }
                else
                {
                    
                    
                    
                    if (strcmp(p1, "acc") == 0)
                    {
                        lever_sign(adr_p2, sign, basic_line, number, true);
                        push("acc", &calculate, store_adr1);
                    }
                    else
                    {
                        lever_sign(adr_p1, sign, basic_line, number, false);
                        push("acc", &calculate, store_adr1);
                    }
                    
                }
                /*if(check_empty_stack(calculate)){
                    push("acc", &calculate, adr_acc);
                }*/
            }
            
        }
        add_command("STORE", buf_obm, &basic_line[number], RAM_adress, 0, false);
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
    overload_mem();
}

void parser(struct basic_line *basic_line, size_t n)
{
    for (int i = 0; i < n; i++)
    {
        add_asm_line(basic_line[i].command, basic_line[i].parameters, basic_line, i, n, true);
    }
}

void dragging_out_line(char *name_file_asm, char *name_file_bas)
{
    FILE *file_bas = NULL;

    char str[256];
    int amount = amount_line_in_file(name_file_bas);
    printf("Lines : %d\n\n", amount);
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
    for (int i = 0; i < 26; i++)
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
    for(int i=0;i<26;i++){
        printf("%d; ",perem[i]);
    }
    printf("\n");
   /* struct stack *m=NULL;
    int *store=NULL;
    char let[256]={0};
    int op[]={1,2,3,4,5};
    push("A",&m,&op[0]);
    push("+",&m,&op[1]);
    push("B",&m,&op[2]);
    push("/",&m,&op[3]);
    push("1234",&m,&op[4]);
    print_stack(m);
    printf("\n");
    pop_del(&m,let,&store);
    printf("store = %d\n",*store);*/
}

/*
96    A B C * + B A C + / 33 45 - / - 

 
A ACC


A ACC 

2 LOAD 97
3 MUL 95
4 ADD 94
5 STORE 94
6 LOAD 98
7 ADD 95
8 STORE 99
9 LOAD 94
10 DIVIDE 99
93 = 44
92 = 33
13 LOAD 92
14 SUB 93
15 STORE 99
16 LOAD 94
17 DIVIDE 99
18 STORE 99
19 LOAD 94
20 SUB 99
21 STORE 96
22 JUMP 23
23 WRITE 95
24 HALT 0



98; 97; 95; 0; 0; 0; 0; 0; 0; 0; 96; 0; 0; 0; 0; 0; 0; 0; 0; 0; 0; 0; 0; 0; 0; 0;  



*/

#include "sat.h"

int chek_arg(int argc, char *argv[])
{
    if (argc != 3)
    {
        return INVALID_NUMBER_OF_ARGUMENTS;
    }
    else
    {
        size_t obj = strlen(argv[1]);
        size_t asem = strlen(argv[2]);
        if (obj < 3 || asem < 4)
        {
            return INVALID_SIZE_OF_ARGUMENTS;
        }
        else
        {
            char *expansion_obj = strrchr(argv[1], '.');
            char *expansion_asm = strrchr(argv[2], '.');

            if (!strcmp(expansion_asm, ".sa") && !strcmp(expansion_obj, ".o"))
            {
                FILE *fp = fopen(argv[2], "r");
                if (fp == NULL)
                {
                    fclose(fp);
                    return FILE_NOT_FOUND;
                }
                else
                {
                    fclose(fp);
                    return SUCCESSFULLY;
                }
            }
            else
            {
                return WRONG_EXTENSION;
            }
        }
    }
}

int error_handler(int error)
{
    switch (error)
    {
    case SUCCESSFULLY:
    {
        mt_setfgcolor(green);
        printf("FILE FOUND");
        mt_setfgcolor(reset);
        printf("\n");
        return 0;
        break;
    }
    case INVALID_NUMBER_OF_ARGUMENTS:
    {
        mt_setfgcolor(red);
        printf("INVALID NUMBER OF ARGUMENTS");
        mt_setfgcolor(reset);
        printf("\n");
        exit(EXIT_FAILURE);
        break;
    }
    case INVALID_SIZE_OF_ARGUMENTS:
    {
        mt_setfgcolor(red);
        printf("INVALID SIZE OF ARGUMENTS");
        mt_setfgcolor(reset);
        printf("\n");
        exit(EXIT_FAILURE);
        break;
    }
    case WRONG_EXTENSION:
    {
        mt_setfgcolor(red);
        printf("WRONG EXTENSION");
        mt_setfgcolor(reset);
        printf("\n");
        exit(EXIT_FAILURE);
        break;
    }
    case FILE_NOT_FOUND:
    {
        mt_setfgcolor(red);
        printf("ASSEMBLER FILE NOT FOUND");
        mt_setfgcolor(reset);
        printf("\n");
        exit(EXIT_FAILURE);
        break;
    }
    case EMPTY_FILE:
    {
        mt_setfgcolor(red);
        printf("EMPTY FILE");
        mt_setfgcolor(reset);
        printf("\n");
        exit(EXIT_FAILURE);
        break;
    }
    case COMMAND_NOT_FOUND:
    {
        mt_setfgcolor(red);
        printf("COMMAND NOT FOUND");
        mt_setfgcolor(reset);
        printf("\n");
        exit(EXIT_FAILURE);
        break;
    }
    case ERROR_ENCODE:
    {
        mt_setfgcolor(red);
        printf("ERROR ENCODE");
        mt_setfgcolor(reset);
        printf("\n");
        exit(EXIT_FAILURE);
        break;
    }
    case INCORRECT_NUMBER_FORMAT:
    {
        mt_setfgcolor(red);
        printf("INCORRECT NUMBER FORMAT");
        mt_setfgcolor(reset);
        printf("\n");
        exit(EXIT_FAILURE);
        break;
    }
    case TOO_FEW_COMMANDS:
    {
        mt_setfgcolor(red);
        printf("TOO FEW COMMANS");
        mt_setfgcolor(reset);
        printf("\n");
        exit(EXIT_FAILURE);
        break;
    }
    case DIGIT_TOO_BIG:{
        mt_setfgcolor(red);
        printf("DIGIT TOO BIG");
        mt_setfgcolor(reset);
        printf("\n");
        exit(EXIT_FAILURE);
        break;
    }
    default:
    {
        return 1;
        break;
    }
    }
}

int ComCPU(char *command)
{
    if (strcmp(command, "=") == 0)
    {
        return 0x12;
    }
    else if (strcmp(command, "READ") == 0)
    {
        return 0x10;
    }
    else if (strcmp(command, "WRITE") == 0)
    {
        return 0x11;
    }
    else if (strcmp(command, "LOAD") == 0)
    {
        return 0x20;
    }
    else if (strcmp(command, "STORE") == 0)
    {
        return 0x21;
    }
    else if (strcmp(command, "ADD") == 0)
    {
        return 0x30;
    }
    else if (strcmp(command, "SUB") == 0)
    {
        return 0x31;
    }
    else if (strcmp(command, "DIVIDE") == 0)
    {
        return 0x32;
    }
    else if (strcmp(command, "MUL") == 0)
    {
        return 0x33;
    }
    else if (strcmp(command, "JUMP") == 0)
    {
        return 0x40;
    }
    else if (strcmp(command, "JNEG") == 0)
    {
        return 0x41;
    }
    else if (strcmp(command, "JZ") == 0)
    {
        return 0x42;
    }
    else if (strcmp(command, "HALT") == 0)
    {
        return 0x43;
    }
    else if (strcmp(command, "JP") == 0)
    {
        return 0x58;
    }
    else
        error_handler(COMMAND_NOT_FOUND);
}

void EncodeLine(int adress_cell, int command, int operand)
{

    int cell = 0;
    if (command != 0x12)
    {
        if (sc_commandEncode(command, operand, &cell) == 1)
        {
            error_handler(ERROR_ENCODE);
        }
        else
        {
            RAM[adress_cell] = cell;
        }
    }
    else
    {
        if (operand <= 0x3FFF && operand >= -0x3FFF)
        {
            if (operand > 0)
            {
                operand = operand | (1 << 14);
            }

            RAM[adress_cell] = operand;
        }else{
            error_handler(DIGIT_TOO_BIG);
        }
    }
}

int asm_parser(char *line)
{
    int adress_cell = 0, operand = 0;
    char com[10], operand_str[10];

    sscanf(line, "%d%s%s", &adress_cell, com, operand_str);

    if (strlen(operand_str) == 0)
    {
        error_handler(TOO_FEW_COMMANDS);
    }

    if (operand_str[0] == '+' || operand_str[0] == '-')
    {
        if (strlen(operand_str) == 5)
        {
            //operand_str[0] = ' ';
            sscanf(operand_str + 1, "%4X", &operand);
            if (operand_str[0] == '-')
            {
                operand *= (-1);
            }
        }
        else
        {
            error_handler(INCORRECT_NUMBER_FORMAT);
        }
    }
    else
    {
        sscanf(operand_str, "%d", &operand);
    }
    int dig_com = ComCPU(com);

    EncodeLine(adress_cell, dig_com, operand);
}

void interpretation(char *asembler, char *object)
{
    FILE *obj_file, *asem_file;

    char str[256], command[256];

    asem_file = fopen(asembler, "rt");
    obj_file = fopen(object, "wb");
    if (asem_file == NULL || obj_file == NULL)
    {
        mt_setfgcolor(red);
        printf("COULD NOT OPEN OR CREATE FILE");
        mt_setfgcolor(reset);
        printf("\n");
        fclose(asem_file);
        fclose(obj_file);
        exit(EXIT_FAILURE);
    }
    else
    {
        mt_setfgcolor(green);
        printf("ASSEMBLER FILE OPEN");
        fclose(obj_file);
        mt_setfgcolor(reset);
        printf("\n");
    }
    int number_str = 0;
    while (fgets(str, sizeof(char) * 256, asem_file))
    {

        int count_empty = 0;
        for (int i = 0; i < 256 && str[i] != '\0' && str[i] != ';' && str[i] != '\n'; i++)
        {
            if (str[i] == ' ')
            {
                count_empty++;
            }
            command[i] = str[i];
            command[i + 1] = '\0';
        }

        if (strtok(command, "\t") != NULL && count_empty != strlen(command))
        {
            printf("%s\t", command);

            asm_parser(command);

            mt_setfgcolor(green);
            printf("+");
            mt_setfgcolor(reset);
            printf("\n");
            number_str++;
        }
    }

    if (number_str == 0)
    {
        error_handler(EMPTY_FILE);
    }
    else
    {
        printf("Number of lines: %d\n", number_str);
    }

    fclose(asem_file);
}

int main(int argc, char *argv[])
{

    FILE *obj_file = NULL;

    int warning = chek_arg(argc, argv);

    if (!error_handler(warning))
    {
        interpretation(argv[2], argv[1]);
        obj_file = fopen(argv[1], "w");
        if (obj_file == NULL)
        {
            mt_setfgcolor(red);
            printf("COULD NOT OPEN OR CREATE FILE");
            mt_setfgcolor(reset);
            printf("\n");
            exit(EXIT_FAILURE);
        }
        fwrite(RAM, 1, 100 * sizeof(int), obj_file);
        fclose(obj_file);
    }
    else
    {
        exit(EXIT_FAILURE);
    }
    /* int k = -0x1111;
    printf("%d %04X %04X\n",k,k,~k);*/
}
/*INTERPRETATION IN THE ASSEMBLER CODE*/

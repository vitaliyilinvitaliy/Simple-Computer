#include "CPU.h"

bool flag_add = false;

int ALU(int command, int operand)
{
    int value_cell = 0, value_acc = 0;

    sc_accGet(&value_acc);
    sc_memoryGet(operand, &value_cell);
    if (value_cell > 0x0000)
        value_cell = value_cell & (0x3FFF);
    if (value_acc > 0x0000)
        value_acc = value_acc & (0x3FFF);

    switch (command)
    {

    case 0x30:
    { //ADD
        value_acc += value_cell;
        if (value_acc <= 0x3FFF && value_acc >= -0x3FFF)
        {
            sc_accSet(value_acc);
            flag_add=true;
        }
        else
        {
            sc_regSet(MEMORY_OVERFLOW, 1);
            return 1;
        }
        break;
    }
    case 0x31:
    { //SUB
        value_acc -= value_cell;
        if (value_acc <= 0x3FFF && value_acc >= -0x3FFF)
        {
            sc_accSet(value_acc);
        }
        else
        {
            sc_regSet(MEMORY_OVERFLOW, 1);
            return 1;
        }
        break;
    }
    case 0x32:
    { //DIVIDE
        if (value_cell == 0)
        {
            sc_regSet(DIVISION_ERROR_BY_0, 1);
            return 1;
        }
        else
        {
            value_acc /= value_cell;
            sc_accSet(value_acc);
        }

        break;
    }
    case 0x33:
    { //MUL
        value_acc *= value_cell;
        if (value_acc <= 0x3FFF && value_acc >= -0x3FFF)
        {
            sc_accSet(value_acc);
        }
        else
        {
            sc_regSet(MEMORY_OVERFLOW, 1);
            return 1;
        }
        break;
    }
    case 0x60:{
        //CHL Dima
        value_acc=value_cell<<1;
        if (value_acc <= 0x3FFF && value_acc >= -0x3FFF)
        {
            sc_accSet(value_acc);
        }
        else
        {
            sc_regSet(MEMORY_OVERFLOW, 1);
            return 1;
        }
        break;
    }
    case 0x61:{
        //SHL Vova
        value_acc=value_cell>>1;
        if (value_acc <= 0x3FFF && value_acc >= -0x3FFF)
        {
            sc_accSet(value_acc);
        }
        else
        {
            sc_regSet(MEMORY_OVERFLOW, 1);
            return 1;
        }
        break;
    }
    
    default:
        break;
    }
}

int check16(char *number)
{
    for (int i = 1; i < 5; i++)
    {
        if (number[i] != '0' && number[i] != '1' && number[i] != '2' && number[i] != '3' &&
            number[i] != '4' && number[i] != '5' && number[i] != '6' && number[i] != '7' &&
            number[i] != '8' && number[i] != '9' && number[i] != 'A' && number[i] != 'B' &&
            number[i] != 'C' && number[i] != 'D' && number[i] != 'E' && number[i] != 'F')
        {
            return 1;
        }
    }
    return 0;
}

int read_value(int operand)
{
    rk_mytermregime(0, 0, 1, 0, 1);

    int instr_c = 0;
    sc_instrcGet(&instr_c);
    bc_drawInterfase(instr_c);

    printf("%02d< ", operand);

    char value[10];

    fgets(value, 10, stdin);
    int val_copy = 0;

    value[strlen(value) - 1] = '\0';
    if (strlen(value) == 5 && (value[0] == '+' || value[0] == '-'))
    {
        sscanf(value + 1, "%04X", &val_copy);
        if (val_copy <= 0x3FFF && val_copy >= -0x3FFF && check16(value) != 1)
        {
            if (value[0] == '-')
            {
                val_copy *= (-1);
                if (val_copy == 0)
                {
                    val_copy = val_copy | (1 << 14);
                }
            }
            else
            {
                val_copy = val_copy | (1 << 14);
            }
            if (sc_memorySet(operand, val_copy) == 1)
            {
                printf("\nBorder overload");
                sc_regSet(IGNORING_TACT_PULSES, 1);
                rk_mytermregime(1, 1, 0, 1, 0);
                return 0;
            }
        }
        else
        {
            printf("\nError memory overflow or not number");
            getchar();
            return 1;
        }
    }
    else
    {
        printf("\nFew or many arguments");
        getchar();
        return 1;
    }
    rk_mytermregime(1, 1, 0, 1, 0);
    return 0;
}

int CU(void)
{
    int command = 0, operand = 0, value_cell = 0, adress_cell = 0;

    sc_instrcGet(&adress_cell);

    if (sc_memoryGet(adress_cell, &value_cell) == 1)
    {
        return 1;
    }

    if (sc_commandDecode(value_cell, &command, &operand) == 1)
    {
        sc_regSet(CODE_ERROR, 1);
        sc_regSet(IGNORING_TACT_PULSES, 1);
        alarm(0);
        return 1;
    }
    else
    {
        sc_regSet(CODE_ERROR, 0);
    }

    if (command >= 0x30 && command <= 0x33||command == 0x60 || command == 0x61)
    {
        if (ALU(command, operand))
        {
            sc_regSet(IGNORING_TACT_PULSES, 1);
            return 1;
        }
        else
        {
            return 0;
        }
    }

    int value = 0;
    switch (command)
    {
    case 0x10:
    { //READ
        int read = 0;
        do
        {
            read = read_value(operand);
        } while (read);
        break;
    }
    case 0x11:
    { //WRITE
        sc_memoryGet(operand, &value);
        if (value > 0)
        {
            value = value & 0x3FFF;
        }
        bc_settextterminal(value, operand);
        break;
    }
    case 0x20:
    { //LOAD
        sc_memoryGet(operand, &value);
        if (value > 0)
        {
            value = value & 0x3FFF;
        }
        sc_accSet(value);
        break;
    }
    case 0x21:
    { //STORE
        sc_accGet(&value);
        if (value >= 0)
        {
            value = value | (1 << 14);
        }
        sc_memorySet(operand, value);
        break;
    }
    case 0x40:
    { //JUMP
        sc_instrcSet(operand - 1);
        break;
    }
    case 0x41:
    { //JNEG
        sc_accGet(&value);
        if (value < 0x0000)
            sc_instrcSet(operand - 1);
        break;
    }
    case 0x42:
    { //JZ
        sc_accGet(&value);
        if (value == 0)
            sc_instrcSet(operand - 1);
        break;
    }
    case 0x55:
    {
        //JNS Maks
        sc_accGet(&value);
        if (value > 0x0000)
            sc_instrcSet(operand - 1);
        break;
    }
    case 0x43:
    { //HALT
        sc_regSet(IGNORING_TACT_PULSES, 1);
        break;
    }
    case 0x58:{
        //JP Vitaliy
        sc_accGet(&value);
        if(value%2 == 0)
            sc_instrcSet(operand - 1);
        break;
    }
    case 0x57:{
        //JC Nastya
        if(flag_add){
            flag_add = false;
            sc_instrcSet(operand - 1);
        }
        break;
    }
    default:
    {
        sc_regSet(CODE_ERROR, 1);
        sc_regSet(IGNORING_TACT_PULSES, 1);
    }
    break;
    }
}
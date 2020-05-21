#include "mySimpleComputer.h"

struct mySimpleComputer registers;

int sc_memoryInit()
{
    if (RandomAccessMemory != NULL)
    {
        for (int i = 0; i < 100; i++)
        {
            RandomAccessMemory[i] = 0;
        }
        return 0;
    }
    else
    {
        return 1;
    }
}

int sc_printMemory(int numberPaint)
{
    enum colors fon = black;
    for (int i = 0, stroka = 2; i < 100;)
    {
        mt_gotoXY(2, stroka);
        for (int j = 0; j < 10; j++, i++)
        {
            if (i == numberPaint)
            {
                mt_setbgcolor(red);
            }
            else
            {
                mt_setbgcolor(fon);
            }

            int oper = 0, com = 0, bit = 0;

            int buffer = RandomAccessMemory[i];

            bit = (buffer >> 14) & 1;

            if (!bit)
            {
                com = com | ((buffer >> 7) & 0x7F);
                oper = oper | (buffer & 0x7F);
                printf("+%02X%02X ", com, oper);
            }
            else
            {
                if (buffer < 0x0000)
                {
                    printf("-%04X ", buffer * (-1));
                }
                else
                {
                    buffer = buffer & 0x3FFF;
                    printf(" %04X ", buffer);
                }
            }

            mt_setbgcolor(black);
        }
        stroka++;
    }
}

int sc_memorySet(int address, int value)
{
    if (RandomAccessMemory != NULL)
    {
        if (address >= 0 && address <= 99)
        {
            RandomAccessMemory[address] = value;
            return 0;
        }
        else
        {
            sc_regSet(BORDER_OVERLOAD_ERROR, 1);
            return 1;
        }
    }
    else
    {
        return 1;
    }
}

int sc_memoryGet(int address, int *value)
{
    if (RandomAccessMemory != NULL)
    {
        if (address >= 0 && address <= 99)
        {
            (*value) = RandomAccessMemory[address];
            return 0;
        }
        else
        {
            sc_regSet(BORDER_OVERLOAD_ERROR, 1);
            return 1;
        }
    }
    else
    {
        return 1;
    }
}

int sc_memorySave(char *filename)
{
    FILE *RAMSave = NULL;

    RAMSave = fopen(filename, "wb");

    if (RAMSave == NULL)
    {
        return 1;
    }

    fwrite(&RandomAccessMemory, sizeof(int), 100, RAMSave);

    fclose(RAMSave);
    return 0;
}

int sc_memoryLoad(char *filename)
{

    if (RandomAccessMemory != NULL)
    {
        FILE *RAMLoad = NULL;

        RAMLoad = fopen(filename, "rb");

        if (RAMLoad == NULL)
        {
            return 1;
        }

        int LoadDataRam[100];
        fread(LoadDataRam, sizeof(int), 100, RAMLoad);
        fclose(RAMLoad);
        for (int i = 0; i < 100; i++)
        {
            RandomAccessMemory[i] = LoadDataRam[i];
        }

        return 0;
    }
    else
    {
        return 1;
    }
}

int sc_regInit(void)
{
    registers.register_flag = 0;
    return 0;
}

int sc_accInit(void)
{
    registers.accumulator = 0;
    return 0;
}

int sc_instrcInit(void)
{
    registers.instruction_counter = 0;
    return 0;
}

int sc_regSet(int reg, int value)
{

    if ((reg <= CODE_ERROR) && (reg >= MEMORY_OVERFLOW) && (value == 0 || value == 1))
    {
        if (value == 1)
        {
            registers.register_flag = registers.register_flag | (value << reg);

            return 0;
        }
        else if (value == 0)
        {
            registers.register_flag = registers.register_flag & (~(1 << reg));

            return 0;
        }
    }
    return 1;
}

int sc_accSet(int value)
{
    registers.accumulator = value;
    return 0;
}

int sc_instrcSet(int value)
{
    registers.instruction_counter = value;
    return 0;
}

int sc_regGet(int reg, int *value)
{
    if ((reg <= CODE_ERROR) && (reg >= MEMORY_OVERFLOW))
    {
        (*value) = (registers.register_flag >> reg) & 1;
        return 0;
    }
    return 1;
}

int sc_accGet(int *value)
{
    (*value) = registers.accumulator;
    return 0;
}

int sc_instrcGet(int *value)
{
    (*value) = registers.instruction_counter;
    return 0;
}

int sc_commandEncode(int command, int operand, int *value)
{
    if ((command <= 127) && (command >= 0) && (operand <= 99) && (operand >= 0))
    {

        if ((command >= 0x10 && command <= 0x11) ||
            (command >= 0x20 && command <= 0x21) ||
            (command >= 0x30 && command <= 0x33) ||
            (command >= 0x40 && command <= 0x43) ||
            (command >= 0x51 && command <= 0x76))
        {

            (*value) = 0;
            for (int i = 0, j = 7; i < 7 && j < 15; i++, j++)
            {
                int checkBitCommand = (command >> i) & 1;
                int checkBitOperand = (operand >> i) & 1;
                if (checkBitCommand == 1)
                {
                    (*value) = (*value) | (1 << j);
                }
                if (checkBitOperand == 1)
                {
                    (*value) = (*value) | (1 << i);
                }
            }
            sc_regSet(BORDER_OVERLOAD_ERROR, 0);
            return 0;
        }
        else
        {
            sc_regSet(CODE_ERROR, 0);
            return 1;
        }
    }
    else
    {
        sc_regSet(BORDER_OVERLOAD_ERROR, 1);
        return 1;
    }
}

int sc_commandDecode(int value, int *command, int *operand)
{
    int checkCom = (value >> 14) & 1;

    if (checkCom == 0)
    {
        (*operand) = 0;
        (*command) = 0;
        for (int i = 0, j = 7; i < 7 && j < 14; i++, j++)
        {
            int checkBitCommand = (value >> j) & 1;
            int checkBitOperand = (value >> i) & 1;
            if (checkBitCommand == 1)
            {
                (*command) = (*command) | (1 << i);
            }
            if (checkBitOperand == 1)
            {
                (*operand) = (*operand) | (1 << i);
            }
        }

        return 0;
    }
    else
    {
        return 1;
    }
}

void sc_printRegFlag(void)
{

    mt_gotoXY(64, 11);
    for (int i = 0; i < 5; i++)
    {
        int CheckBitRegister = (registers.register_flag >> i) & 1;
        char f = '\0';
        switch (i)
        {
        case 0:
            f = 'P'; //M
            break;
        case 1:
            f = '0'; //D
            break;
        case 2:
            f = 'E'; //B
            break;
        case 3:
            f = 'V'; //I
            break;
        case 4:
            f = 'M'; //C
            break;
        default:
            break;
        }
        mt_setbgcolor(black);
        if (CheckBitRegister == 1)
        {
            printf("%c   ", f);   
        }
        else
        {
            printf("    ");
        }
    }
}

void sc_printAccum(void)
{
    mt_setbgcolor(black);
    mt_gotoXY(70, 2);
    int buffer = registers.accumulator;
    if (buffer < 0x0000)
    {
        printf("-%04X ", buffer * (-1));
    }
    else
    {
        printf("+%04X ", buffer);
    }
}

void sc_printInstrCount(void)
{
    mt_setbgcolor(black);
    mt_gotoXY(71, 5);
    printf("%04X", registers.instruction_counter);
}

void sc_printOper(void)
{
    mt_setbgcolor(black);
    mt_gotoXY(69, 8);
    int command = 0, operand = 0, value = 0;
    sc_memoryGet(registers.instruction_counter, &value);
    sc_commandDecode(value, &command, &operand);
    printf("+%2.2x : %2.2x", command, operand);
}

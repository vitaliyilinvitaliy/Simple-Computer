#include "CPU.h"

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

    if (command >= 0x30 && command <= 0x33)
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
    case 0x55:{
        //JNS
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
    
    default:
    {
        sc_regSet(CODE_ERROR, 1);
        sc_regSet(IGNORING_TACT_PULSES, 1);
    }
    break;
    }
}

/*
    Арифметико-логическое устройство (англ. arithmetic and logic unit, ALU) — блок процессора, ко-
торый служит для выполнения логических и арифметических преобразований над данными. В каче-
стве данных могут использоваться значения, находящиеся в аккумуляторе, заданные в операнде ко-
манды или хранящиеся в оперативной памяти. Результат выполнения операции сохраняется в 
акку-муляторе или может помещаться в оперативную память. В ходе выполнения операций АЛУ устанав-
ливает значения флагов «деление на 0» и «переполнение».
    Управляющее устройство (англ. control unit, CU) координирует работу центрального процессора.
По сути, именно это устройство отвечает за выполнение программы, записанной в оперативной па-
мяти. В его функции входит: чтение текущей команды из памяти, еѐ декодирование, передача номе-
ра команды и операнда в АЛУ, определение следующей выполняемой команды и реализации взаи-
модействий с клавиатурой и монитором. Выбор очередной команды из оперативной памяти произ-
водится по сигналу от системного таймера. Если установлен флаг «игнорирование тактовых импуль-
сов», то эти сигналы устройством управления игнорируются. В ходе выполнения операций устрой-
ство управления устанавливает значения флагов «указана неверная команда» и «игнорирование так-
товых импульсов».
    Команды выполняются последовательно. Адрес ячейки памяти, в которой находится текущая
выполняемая команда, задается в регистре «Счетчик команд». Устройство управления запрашивает
содержимое указанной ячейки памяти и декодирует его согласно используемому формату команд.
Получив код операции, устройство управления определяет, является ли эта операция арифметико-
логической. Если да, то выполнение операции передается в АЛУ. В противном случае операция вы-
полняется устройством управления. Процедура выполняется до тех пор, пока флаг «останов» не бу-
дет равен 1.

    Для выполнения программ моделью Simple Computer необходимо реализовать две функции: 
    int ALU (int command, int operand) – реализует алгоритм работы арифметико-
логического устройства. Если при выполнении функции возникла ошибка, которая не
позволяет дальше выполнять программу, то функция возвращает -1, иначе 0;
    int CU (void) – обеспечивает работу устройства управления.
Обработку команд осуществляет устройство управления. Функция CU вызывается либо обра-
ботчиком сигнала от системного таймера, если не установлен флаг «игнорирование тактовых им-
пульсов», либо при нажатии на клавишу ―t‖. Алгоритм работы функции следующий:
    1. из оперативной памяти считывается ячейка, адрес которой храниться в регистре instruc-
    tionCounter;
    2. полученное значение декодируется как команда;
    3. если декодирование невозможно, то устанавливаются флаги «указана неверная команда» и
    «игнорирование тактовых импульсов» (системный таймер можно отключить) и работа
    функции прекращается.
    4. Если получена арифметическая или логическая операция, то вызывается функция ALU,
    иначе команда выполняется самим устройством управления.
    
    Логические операции!
    
    NOT AND XOR OR LOGLC LOGRC

    5. Определяется, какая команда должна быть выполнена следующей и адрес еѐ ячейки памя-
    ти заносится в регистр instructionCounter.
    6. Работа функции завершается.
*/

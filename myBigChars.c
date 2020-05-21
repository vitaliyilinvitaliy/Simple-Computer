#include "myBigChars.h"




int bc_printA(char *str)
{
    printf("\E(0%s\E(B", str);
}

int bc_box(int x1, int y1, int x2, int y2)
{
    mt_setbgcolor(black);
    if (x1 == 0)
        x1 = 1;
    for (int CoordX = x1 + 1; CoordX < x2; CoordX++)
    {
        mt_gotoXY(CoordX, y1);
        bc_printA("r");
    }
    for (int CoordY = y1 + 1; CoordY < y2; CoordY++)
    {
        mt_gotoXY(x1, CoordY);
        bc_printA("x");
    }

    for (int CoordX = x1 + 1; CoordX < x2; CoordX++)
    {
        mt_gotoXY(CoordX, y2);
        bc_printA("r");
    }
    for (int CoordY = y1 + 1; CoordY < y2; CoordY++)
    {
        mt_gotoXY(x2, CoordY);
        bc_printA("x");
    }
    mt_gotoXY(x1, y1);
    bc_printA("l");
    mt_gotoXY(x1, y2);
    bc_printA("m");
    mt_gotoXY(x2, y1);
    bc_printA("k");
    mt_gotoXY(x2, y2);
    bc_printA("j");
    mt_setbgcolor(black);
}

int bc_drawInterfase(int paint)
{
    
    //основная рамка
    mt_clrscr();
    
    sc_printMemory(paint);
    int m = 0;
    sc_memoryGet(paint, &m);
    bc_drawdigit(m);
    sc_printRegFlag();
    sc_printAccum();
    sc_printOper();
    sc_printInstrCount();

    bc_box(1, 1, 61, 12);
    bc_box(62, 1, 83, 3);
    bc_box(62, 4, 83, 6);
    bc_box(62, 7, 83, 9);
    bc_box(62, 10, 83, 12);
    bc_box(1, 13, 42, 22);
    bc_box(43, 13, 83, 22);
    mt_gotoXY(27, 1);
    printf(" Memory ");
    mt_gotoXY(67, 1);
    printf(" Accumulator ");
    mt_gotoXY(63, 4);
    printf(" InstructionCounter ");
    mt_gotoXY(68, 7);
    printf(" Operation ");
    mt_gotoXY(70, 10);
    printf(" Flags ");
    mt_gotoXY(51, 13);
    printf(" Keys: ");
    mt_gotoXY(44, 14);
    printf("l - load");

    mt_gotoXY(44, 15);
    printf("s - save");
    mt_gotoXY(44, 16);
    printf("r - run");
    mt_gotoXY(44, 17);
    printf("t - step");
    mt_gotoXY(44, 18);
    printf("i - reset");
    mt_gotoXY(44, 19);
    printf("F5 - accumulator");
    mt_gotoXY(44, 20);
    printf("F6 - instrictionComputer\n");
    mt_gotoXY(50, 24);
    printf("\n");
    mt_gotoXY(1, 23);
    printf("\nInput/Output:\n");

    if (TerminalWrite[2] == 1)
    {
        bc_printtextterminal();
    }
}

int bc_printbigchar(long int *a, int x, int y, enum colors colDig, enum colors colBack)
{

    if (x < 0 || y < 0)
    {
        return -1;
    }
    mt_gotoXY(x, y);
    for (int i = 0; i < 2; i++)
    {
        for (int l = 0; l < 32;)
        {
            for (int j = 0; j < 8 && l < 32; j++, l++)
            {
                if ((a[i] >> (l)) & 0x1)
                {
                    mt_setbgcolor(colBack);
                    bc_printA(" ");
                    mt_setfgcolor(reset);
                }
                else
                {
                    mt_setbgcolor(colDig);
                    bc_printA(" ");
                    mt_setbgcolor(reset);
                }
            }
            y++;
            mt_gotoXY(x, y);
        }
    }
}

int bc_setbigcharpos(int *big, int x, int y, int value)
{
    if ((y >= 0 && y < 8) && (x >= 0 && x < 8))
    {
        if (value == 1)
        {
            if (y < 4)
            {
                big[0] = big[0] | (1 << (8 * y + x));
            }
            if (y > 3)
            {
                y -= 4;
                big[1] = big[1] | (1 << (8 * y + x));
            }
        }
        else if (value == 0)
        {
            if (y < 4)
            {
                big[0] = (~(1 << (8 * y + x))) & big[0];
            }
            if (y > 3)
            {
                y -= 4;
                big[1] = (~(1 << (8 * y + x))) & big[1];
            }
        }
    }
    else
    {
        return -1;
    }
}

int bc_getbigcharpos(int *big, int x, int y, int *value)
{
    if ((y >= 0 && y < 8) && (x >= 0 && x < 8))
    {
        if (y < 4)
        {
            (*value) = (big[0] >> (y * 8 + x)) & 0x1;
        }
        if (y > 3)
        {
            y -= 4;
            (*value) = (big[1] >> (y * 8 + x)) & 0x1;
        }
    }
    else
    {
        return -1;
    }
}

int bc_bigcharwrite(int fd, long int *big, int count)
{
    int result;
    for (int i = 0; i < count; i++)
    {
        if ((result = write(fd, big, sizeof(long int) * 2)) == -1)
            return -1;
    }
}
int bc_bigcharread(int fd, long int *big, int need_count, int *count)
{
    for (int i = 0; i < need_count; i++, *count++)
    {
        int result;
        if ((result = read(fd, big, sizeof(long int) * 2)) == -1)
            return -1;
    }
}

void bc_setvaluebigchar(char s, long int *mas)
{
    struct myBigChars BCH={
        {1382171196, 1010976330},
        {269752336, 940576784},
        {270804024, 1006896136},
        {940580924, 943998016},
        {1041372184, 940576784},
        {1077675068, 943998016},
        {67388536, 943998012},
        {135274556, 67372036},
        {1010975292, 1010975298},
        {2084717116, 1010843712},
        {4279769112, 404232447},
        {1009001496, 606348324},
        {1007952924, 1011106884},
        {33694744, 405012994},
        {1145316380, 472138820},
        {470025276, 1006896132},
        {470025276, 67372036},
        {4278190080,255}
    };
    switch (s)
    {
    case '0':
        mas[0] = BCH.Zero[0];
        mas[1] = BCH.Zero[1];
        break;
    case '1':
        mas[0] = BCH.One[0];
        mas[1] = BCH.One[1];
        break;
    case '2':
        mas[0] = BCH.Two[0];
        mas[1] = BCH.Two[1];
        break;
    case '3':
        mas[0] = BCH.Three[0];
        mas[1] = BCH.Three[1];
        break;
    case '4':
        mas[0] = BCH.Four[0];
        mas[1] = BCH.Four[1];
        break;
    case '5':
        mas[0] = BCH.Five[0];
        mas[1] = BCH.Five[1];
        break;
    case '6':
        mas[0] = BCH.Six[0];
        mas[1] = BCH.Six[1];
        break;
    case '7':
        mas[0] = BCH.Seven[0];
        mas[1] = BCH.Seven[1];
        break;
    case '8':
        mas[0] = BCH.Eight[0];
        mas[1] = BCH.Eight[1];
        break;
    case '9':
        mas[0] = BCH.Nine[0];
        mas[1] = BCH.Nine[1];
        break;
    case 'A':
        mas[0] = BCH.A[0];
        mas[1] = BCH.A[1];
        break;
    case 'B':
        mas[0] = BCH.B[0];
        mas[1] = BCH.B[1];
        break;
    case 'C':
        mas[0] = BCH.C[0];
        mas[1] = BCH.C[1];
        break;
    case 'D':
        mas[0] = BCH.D[0];
        mas[1] = BCH.D[1];
        break;
    case 'E':
        mas[0] = BCH.E[0];
        mas[1] = BCH.E[1];
        break;
    case 'F':
        mas[0] = BCH.F[0];
        mas[1] = BCH.F[1];
        break;
    case '+':
        mas[0] = BCH.Plus[0];
        mas[1] = BCH.Plus[1];
        break;
    case ' ':
        mas[0] = 0;
        mas[1] = 0;
        break;
    case '-':
        mas[0] = BCH.Minus[0];
        mas[1] = BCH.Minus[1];
        break;
    default:
        break;
    }
}

void bc_drawdigit(int digit)
{
    char str[4] = {0};
    long int BigDigit[2] = {0};

    int bit = digit & (1 << 14);
    enum colors fon = black;
    if (!bit)
    {
        bc_setvaluebigchar('+', BigDigit);
        bc_printbigchar(BigDigit, 2, 14, fon, green);
        int oper = 0, com = 0;
        sc_commandDecode(digit, &com, &oper);

        sprintf(str, "%02X", com);
        for (int i = 0, x = 10; i < 2; i++, x += 8)
        {
            bc_setvaluebigchar(str[i], BigDigit);
            bc_printbigchar(BigDigit, x, 14, fon, green);
        }

        sprintf(str, "%02X", oper);
        for (int i = 0, x = 26; i < 2; i++, x += 8)
        {
            bc_setvaluebigchar(str[i], BigDigit);
            bc_printbigchar(BigDigit, x, 14, fon, green);
        }
    }
    else
    {

        if (digit < 0)
        {
            digit = digit * (-1);
            bc_setvaluebigchar('-', BigDigit);
            bc_printbigchar(BigDigit, 2, 14, fon, green);
        }
        else
        {
            digit = digit & 0x3FFF;
            bc_setvaluebigchar(' ', BigDigit);
            bc_printbigchar(BigDigit, 2, 14, fon, green);
        }
        snprintf(str,15,"%04X", digit);
        for (int i = 0, x = 10; i < 4; i++, x += 8)
        {
            bc_setvaluebigchar(str[i], BigDigit);
            bc_printbigchar(BigDigit, x, 14, fon, green);
        }
    }
}

void bc_settextterminal(int digit, int number)
{
    TerminalWrite[0] = number;
    TerminalWrite[1] = digit;
    TerminalWrite[2] = 1;
}

void bc_printtextterminal(void)
{
    if (TerminalWrite[1] >= 0)
    {
        printf("%02d> +%04X\n", TerminalWrite[0], TerminalWrite[1]);
    }
    else
    {
        printf("%02d> -%04X\n", TerminalWrite[0], (-1) * TerminalWrite[1]);
    }
}
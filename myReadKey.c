#include "myReadKey.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

struct termios savetty;

int rk_readkey(enum keys *Key)
{
    char buffer[8] = {0};
    //Дескриптор терминала STDIN_FILENO 0
    read(STDIN_FILENO, buffer, 8);

    if (buffer[0] == 'l')
    {
        (*Key) = KeyLoad;
    }
    else if (buffer[0] == 's')
    {
        (*Key) = KeySave;
    }
    else if (buffer[0] == 'r')
    {
        (*Key) = KeyRun;
    }
    else if (buffer[0] == 't')
    {
        (*Key) = KeyStep;
    }
    else if (buffer[0] == 'i')
    {
        (*Key) = KeyReset;
    }
    else if (buffer[0] == '\033')
    {
        if (buffer[2] == '1' && buffer[3] == '5')
        {
            (*Key) = KeyAccumulator;
        }
        else if (buffer[2] == '1' && buffer[3] == '7')
        {
            (*Key) = KeyInstructionCounter;
        }
        else if (buffer[2] == 'C')
        {
            (*Key) = KeyRight;
        }
        else if (buffer[2] == 'D')
        {
            (*Key) = KeyLeft;
        }
        else if (buffer[2] == 'A')
        {
            (*Key) = KeyUp;
        }
        else if (buffer[2] == 'B')
        {
            (*Key) = KeyDown;
        }
        else
            (*Key) = KeyQuit;
    }
    else if (buffer[0] == '.')
    {
        (*Key) = KeySetCellValue;
    }

    return 0;
}

int rk_mytermsave(void)
{
    struct termios tty;
    if (tcgetattr(STDIN_FILENO, &tty) == -1)
    {
        return 1;
    }
    else
    {
        savetty = tty;
        return 0;
    }
}

int rk_mytermrestore(void)
{
    if (tcsetattr(STDIN_FILENO, TCSANOW, &savetty) == -1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint)
{
    struct termios Settings;
    rk_mytermsave();
    Settings = savetty;

    if (regime == 1)
    {
        Settings.c_lflag &= (~ICANON);

        if (sigint == 1)
        {
            Settings.c_lflag &= (~ISIG);
        }
        else if (sigint == 0)
        {
            Settings.c_lflag |= ISIG;
        }

        if (echo == 1)
        {
            Settings.c_lflag &= (~ECHO);
        }
        else if (echo == 0)
        {
            Settings.c_lflag |= ECHO;
        }

        Settings.c_cc[VMIN] = vmin;
        Settings.c_cc[VTIME] = vtime;
    }
    else if (regime == 0)
    {
        Settings.c_lflag |= (ICANON | ECHO);
    }

    tcsetattr(0, TCSANOW, &Settings);
    return 0;
}
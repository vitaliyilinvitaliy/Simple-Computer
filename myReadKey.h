#pragma once
#include <fcntl.h>
#include <termios.h>

enum keys
{
    KeyLoad,               //l
    KeySave,               //s
    KeyRun,                //r
    KeyStep,               //t
    KeyReset,              //i
    KeyAccumulator,        //f5
    KeyInstructionCounter, //f6
    KeyQuit,               //q
    KeyRight,              //->
    KeyLeft,               //<-
    KeyUp,                 //^
    KeyDown,               // \/
    KeySetCellValue,       //.
    Default
};

int rk_readkey(enum keys *);
int rk_mytermsave(void);
int rk_mytermrestore(void);
int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint);
#include "Signal.h"

void s_signalhandler(int signal)
{
    switch (signal)
    {
    case SIGUSR1:
        alarm(0);
        sc_memoryInit();
        sc_regInit();
        sc_accInit();
        sc_instrcInit();
        sc_regSet(IGNORING_TACT_PULSES, 1);
        break;

    case SIGALRM:
    {
        CU();
        int instr = 0, flag = 0;
        sc_instrcGet(&instr);

        sc_regGet(IGNORING_TACT_PULSES, &flag);
        if (!flag)
        {
            alarm(1);
            instr++;
            sc_instrcSet(instr);
        }
    }
    }
}

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
/*
void s_starttimer(void){
    struct itimerval nval, oval;

    nval.it_interval.tv_sec = 3;
    nval.it_interval.tv_usec = 500;
    nval.it_value.tv_sec = 1;
    nval.it_value.tv_usec = 0;

    setitimer (ITIMER_REAL, &nval, &oval);
}*/
/*Обработчик прерываний реагирует на сигналы от системного таймера и кнопки «Reset». При по-
ступлении сигнала от кнопки «Reset» состояние процессора сбрасывается в начальное (значения всех
регистров обнуляется и устанавливается флаг «игнорирование сигналов от таймера»). При поступ-
лении сигнала от системного таймера, работать начинает устройство управления.

Доработайте консоль Simple Computer. Создайте обработчик прерываний от системного тай-
мера так, чтобы при каждом его срабатывании при нулевом значении флага «игнорирование
сигналов системного таймера» значение регистра ―instructionCounter‖ увеличивалось на 1, а
при поступлении сигнала SIGUSR1 состояние Simple Computer возвращалось в исходное. Об-
работка нажатых клавиш осуществляется только в случае, если сигналы от таймера не игно-
рируются.
*/
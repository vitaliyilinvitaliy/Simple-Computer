#pragma once
#include <signal.h>
#include "mySimpleComputer.h"
#include "myTerm.h"
#include <sys/time.h>
#include <unistd.h>
#include "CPU.h"

void s_signalhandler(int);
void s_starttimer(void);
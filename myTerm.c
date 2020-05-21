#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "myTerm.h"

int mt_clrscr(void)
{
	printf("\E[H\E[2J\E[H");
	return 0;
}

int mt_gotoXY(int x, int y)
{
	int rows = 0, cols = 0;
	mt_getscreensize(&rows, &cols);
	if (y <= rows && y >= 0 && x >= 0 && x <= cols)
	{
		printf("\033[%d;%dH", y, x);
		return 0;
	}
	else
	{
		return 1;
	}
}

int mt_getscreensize(int *rows, int *cols)
{
	struct winsize ws;
	if (!ioctl(1, TIOCGWINSZ, &ws))
	{
		(*rows) = ws.ws_row;
		(*cols) = ws.ws_col;
		return 0;
	}
	else
	{
		return -1;
	}
}

int mt_setfgcolor(enum colors COL)
{
	if (COL == 0 || (COL <= 37 && COL >= 30))
	{
		printf("\033[0;%dm", COL);
		return 0;
	}
	else
	{
		return -1;
	}
}

int mt_setbgcolor(enum colors COL)
{
	if (COL == 0 || (COL <= 37 && COL >= 30))
	{
		int c = (int)COL + 10;
		printf("\033[0;%dm", c);
		return 0;
	}
	else
	{
		return -1;
	}
}

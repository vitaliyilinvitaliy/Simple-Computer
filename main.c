#include <stdio.h>
#include <unistd.h>
#include "mySimpleComputer.h"
#include "myBigChars.h"
#include "myTerm.h"
#include "myReadKey.h"
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "Signal.h"
#include <sys/time.h>
#include "CPU.h"


int main()
{
	rk_mytermsave();
	enum keys Key = Default;
	int paintCount = 0;
	int regime = 0;
	enum colors regimecolor = green;

	char file_name[50];
	//rk_mytermregime(regime, 0, 0, 0, 0);

	rk_mytermregime(1, 1, 0, 1, 0);

	signal(SIGUSR1, s_signalhandler);
	signal(SIGALRM, s_signalhandler);

	sc_memoryInit();
	sc_accInit();

	sc_regInit();
	sc_instrcInit();
	sc_regSet(IGNORING_TACT_PULSES, 1);
	int flag_check = 0;

	while (Key != KeyQuit)
	{
		bc_drawInterfase(paintCount);
		sc_instrcGet(&paintCount);

		rk_readkey(&Key);

		sc_regGet(IGNORING_TACT_PULSES, &flag_check);
		if (flag_check)
		{
			switch (Key)
			{
			case KeyLoad:
			{
				bc_drawInterfase(paintCount);
				printf("Load file (*.o): ");
				rk_mytermregime(0, 0, 1, 0, 1);
				fgets(file_name, 50, stdin);

				file_name[strlen(file_name) - 1] = '\0';

				if (sc_memoryLoad(file_name))
				{
					printf("\nFile not open/found!");
					getchar();
				}
				Key = Default;
				rk_mytermregime(1, 1, 0, 1, 0);
				break;
			}

			case KeySave:
			{
				bc_drawInterfase(paintCount);
				printf("Save file: ");
				rk_mytermregime(0, 0, 1, 0, 1);
				fgets(file_name, 50, stdin);

				file_name[strlen(file_name) - 1] = '\0';

				if (sc_memorySave(file_name))
				{
					printf("\nFile not save!");
					getchar();
				}
				Key = Default;
				rk_mytermregime(1, 1, 0, 1, 0);
				break;
			}
			case KeyRight:
				if (paintCount < 99)
				{
					paintCount++;
					sc_instrcSet(paintCount);
				}
				else
				{
					paintCount = 0;
					sc_instrcSet(paintCount);
				}
				Key = Default;
				break;
			case KeyLeft:
				if (paintCount > 0)
				{
					paintCount--;
					sc_instrcSet(paintCount);
				}
				else
				{
					paintCount = 99;
					sc_instrcSet(paintCount);
				}
				Key = Default;
				break;
			case KeyUp:
			{
				if (paintCount - 10 >= 0)
				{
					paintCount -= 10;
					sc_instrcSet(paintCount);
				}
				else
				{
					paintCount = 90 + paintCount % 10;
					sc_instrcSet(paintCount);
				}
				Key = Default;
				break;
			}
			case KeyDown:
			{
				if (paintCount + 10 <= 99)
				{
					paintCount += 10;
					sc_instrcSet(paintCount);
				}
				else
				{
					paintCount = paintCount % 10;
					sc_instrcSet(paintCount);
				}
				Key = Default;
				break;
			}
			case KeySetCellValue:
			{
				rk_mytermregime(0, 0, 1, 0, 1);

				int instr_c = 0;
				sc_instrcGet(&instr_c);
				bc_drawInterfase(instr_c);

				printf("%02d< ", paintCount);

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
							if(val_copy == 0){
								val_copy = val_copy | (1 << 14);
							}
						}
						else
						{
							val_copy = val_copy | (1 << 14);
						}
						if (sc_memorySet(paintCount, val_copy) == 1)
						{
							printf("\nBorder overload");
							getchar();
						}
					}
					else
					{
						printf("\nError memory overflow or not number");
						getchar();
					}
				}
				else
				{
					printf("\nFew or many arguments");
					getchar();
				}
				rk_mytermregime(1, 1, 0, 1, 0);
				Key = Default;
				break;
			}
			case KeyAccumulator:
			{
				rk_mytermregime(0, 0, 1, 0, 1);

				int instr_c = 0;
				sc_instrcGet(&instr_c);
				bc_drawInterfase(instr_c);

				printf("Accumulator< ");

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
						}
						
						sc_accSet(val_copy);
					}
					else
					{
						printf("\nError memory overflow or not number");
						getchar();
					}
				}
				else
				{
					printf("\nFew or many arguments");
					getchar();
				}
				rk_mytermregime(1, 1, 0, 1, 0);
				Key = Default;
				break;
			}
			case KeyInstructionCounter:
			{
				rk_mytermregime(0, 0, 1, 0, 1);
				bc_drawInterfase(paintCount);
				char wrinstr[10];
				printf("Input value InstructionCounter: ");
				fgets(wrinstr, 10, stdin);

				int instr_copy = 0;

				wrinstr[strlen(wrinstr) - 1] = '\0';
				if (strlen(wrinstr) <= 5 && wrinstr[0] == '+' && check16(wrinstr) != 1)
				{
					sscanf(wrinstr + 1, "%04X", &instr_copy);

					if (instr_copy >= 0x0000 && instr_copy <= 0x63)
					{
						sc_instrcSet(instr_copy);
					}
					else
					{
						printf("\nError border");
						getchar();
					}
				}
				else
				{
					printf("\nError format number");
					getchar();
				}

				rk_mytermregime(1, 1, 0, 1, 0);
				Key = Default;
				break;
			}
			case KeyStep:
			{
				CU();
				Key = Default;
				break;
			}
			/*
				case KeyRegime:{
					if(regime==0){
						rk_mytermregime(1, 0, 1, 0, 1);
						regimecolor=red;
						regime=1;				
					}else{
						rk_mytermregime(0, 0, 1, 0, 1);
						regimecolor=green;
						regime=0;
					}
					break;
				}*/
			default:
				break;
			}
		}
		switch (Key)
		{
		case KeyReset:
		{
			raise(SIGUSR1);
			Key = Default;
			break;
		}

		case KeyRun:
		{
			int fl = 0;
			sc_regGet(IGNORING_TACT_PULSES, &fl);
			if (fl)
			{
				sc_regSet(IGNORING_TACT_PULSES, 0);
				raise(SIGALRM);
			}
			else
			{
				alarm(0);
				sc_regSet(IGNORING_TACT_PULSES, 1);
			}
			Key = Default;
			break;
		}

		default:
			break;
		}
		
	}

	rk_mytermregime(0, 0, 1, 1, 1);
	mt_setbgcolor(reset);
	mt_clrscr();
	printf("\n");
	return 0;
}

/*test file*/
/*mt_clrscr();
	struct myBigChars test;
	int fd = creat("BigChFile.dat",0666);
	//int fd = open("BigChFile.dat",O_WRONLY);
	int t1=bc_bigcharwrite(fd,test.Five,2);
	close(fd);
	printf("%d %d",fd,t1);
	long int m[2];
	int count=0;

	fd = open("BigChFile.dat",O_RDONLY);
	
	int t2=bc_bigcharread(fd,m,2,&count);
	close(fd);
	
	
	int t3=bc_printbigchar(m,5,5,black,white);
	printf(" %d %d",fd,t2);
*/
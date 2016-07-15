/*
Chip 8 emulator ^.^
Some code may have come from:
http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
*/

#ifndef LINUX
	#define LINUX 1
#endif

#define true TRUE
#define false FALSE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "chip8tables.c"
#include "chip8common.c"
#include "chip82ds.h"
#include "chip8input.h"  // custom input library ^.^
#include "chip8video.h"  // custom output library ^.^
//#include "chip8dbg.h"    // custom library made for ease of use debugging ^.^
#include "chip8cpu.h"    // custom chip8 cpu library ^.^

void CTRruncycle();

int main(int argc, char **argv) 
{
	initcpu();
	cpu.tick = 0;
	char fail;
	if(cpu.CTR == 1)
	{
		fail = CTRinit();
		if(fail != 0)
		{
			return 1;
		}
	}
	if(loadgame() == 1)
	{
		printf("Unable to load game.\n");
		refresh();
		hang(3);
		cpu.pc = 80;
	}
	consoleClear();
	if(cpu.CTR == 1)
	{
		CTRruncycle();
	}
	return 0;
} 

void CTRruncycle()
{
	for(;;)
	{
		if(cpu.ispaused == 0)
		{
			if((cpu.tick % 25) == 0)
			{
				if(cpu.dt > 0)
					cpu.dt--;
				if(cpu.st > 0)
					cpu.st--;
			}
			if(cycle() == 1)
			{
				hang(3);
				ExitHard();
			}
			if(cpu.drawflag == 1)
				CTRdrawscreen();
			CTRinput('n');
			cpu.tick += 1;
		}
		else
		{
			CTRinput('n');
		}
	}
}

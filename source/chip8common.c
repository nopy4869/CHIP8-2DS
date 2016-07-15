#include <3ds/types.h>

struct chipeight
	{
		unsigned char V[16];
		unsigned char memory[4096];
		unsigned char gfx[64][32];
		unsigned short stack[16];
		unsigned char key[16];
		unsigned short sp;
		unsigned char dt;
		unsigned char st;
		unsigned short I;
		unsigned short pc;
		unsigned char opcode[2];
		unsigned short op;
		unsigned char drawflag;
		unsigned char dbug;
		unsigned char SDL;
		unsigned char CTR;
		unsigned char ispaused;
		unsigned char classic;
		unsigned char stepthru;
		unsigned int romsize;
		unsigned int tick;
		unsigned char statenum;
		unsigned int touchdisabled;
		char romname[16];
		u64 currcount;
		u64 lastcount;
	} cpu;

int SCREEN_SCALE = 6;
int SCREEN_WIDTH = 400;
int SCREEN_HEIGHT = 240;
int i3D_INTENSITY = -4;

int touchbuttoncount = 24;
int typebuttoncount = 45;

int START_X;
int START_Y;

u32 colors[3];
u32* fbuff[4];

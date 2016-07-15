/*
	Much code here was butchered from the ctrulib examples
	Thank you based Smealum et al.
*/

//found this while browsing the PORTAL3D code, IS promising :3
#define CONFIG_3D_SLIDERSTATE (*(float*)0x1FF81080)

#include <3ds.h>
#include <3ds/services/hid.h>
#include <stdio.h>
#include <string.h>

void refresh();
void SetFBPixel(int pixx, int pixy, int pixc, u32* point);
void SetScrPixel(int pixx, int pixy, int pixc, u32* point);
void ExitHard();
int CTRgettouch(u16 tx, u16 ty);

int LoadState();
int SaveState();


void ScalePlus();
void ScaleMinus();
void StatePlus();
void StateMinus();
void reloadgame();

void CTRinput(char type)
{
	int x;
	int touchbutton;
	char paused;
	touchPosition pos2;
	hidScanInput();
	hidTouchRead(&pos2);
	//8 4 6 2 usually used for movement... hmmmm
	//Pong uses 1&4 for player 1 and C&D for player 2
	//Why not give a choice? :D
	if(cpu.ispaused != 1)
		for (x = 0; x < 16; x++)
		{
			cpu.key[x] = 0;
		}
	touchbutton = CTRgettouch(pos2.px, pos2.py);
	for (x = 0; x < 16; x++)
	{
		if (hidKeysDown() & BIT(CTRkeymap[x]))
			cpu.key[x] = 1;
		if (hidKeysHeld() & BIT(CTRkeymap[x]))
			cpu.key[x] = 1;
		if (hidKeysUp() & BIT(CTRkeymap[x]))
			cpu.key[x] = 0;
		if(x == touchbutton)
			cpu.key[x] = 1;
	}
	if ((hidKeysDown() & KEY_START)||(hidKeysHeld() & KEY_START)||(hidKeysUp() & KEY_START))
		ExitHard();
		
	if((cpu.touchdisabled)&&(touchbutton > 15))
		touchbutton = -1;
		
	if (((hidKeysDown() & KEY_SELECT)||(touchbutton == 18)) && (cpu.ispaused == 0))
		cpu.ispaused = 2;
	if (((hidKeysDown() & KEY_SELECT)||(touchbutton == 18)) && (cpu.ispaused == 1))
		cpu.ispaused = 0;
	if(touchbutton == 16)
		LoadState();
	if(touchbutton == 17)
		SaveState();
	if(touchbutton == 19)
		StateMinus();
	if(touchbutton == 20)
		StatePlus();
	if(touchbutton == 21)
		ScaleMinus();
	if(touchbutton == 22)
		ScalePlus();
	if(touchbutton == 23)
		reloadgame();

	if(touchbutton > 15)
		cpu.touchdisabled = 1;
	if(hidKeysUp() & KEY_TOUCH)
		cpu.touchdisabled = 0;

	if(cpu.ispaused == 2)
		cpu.ispaused = 1;
	if(cpu.ispaused == 0)
		paused = ' ';
	if(cpu.ispaused == 1)
		paused = 'X';
	printf("\x1b[1;31HKeys:");
	printf("\x1b[2;31H+-+-+-+-+");
	printf("\x1b[3;31H|%d|%d|%d|%d|",cpu.key[0x1],cpu.key[0x2],cpu.key[0x3],cpu.key[0xC]);
	printf("\x1b[4;31H+-+-+-+-+");
	printf("\x1b[5;31H|%d|%d|%d|%d|",cpu.key[0x4],cpu.key[0x5],cpu.key[0x6],cpu.key[0xD]);
	printf("\x1b[6;31H+-+-+-+-+");
	printf("\x1b[7;31H|%d|%d|%d|%d|",cpu.key[0x7],cpu.key[0x8],cpu.key[0x9],cpu.key[0xE]);
	printf("\x1b[8;31H+-+-+-+-+");
	printf("\x1b[9;31H|%d|%d|%d|%d|",cpu.key[0xA],cpu.key[0x0],cpu.key[0xB],cpu.key[0xF]);
	printf("\x1b[10;31H+-+-+-+-+");
	printf("\x1b[25;0HRegisters:");
	printf("\x1b[26;0HV0:%02X V1:%02X V2:%02X V3:%02X I:%03X",cpu.V[0x0],cpu.V[0x1],cpu.V[0x2],cpu.V[0x3],cpu.I);
	printf("\x1b[27;0HV4:%02X V5:%02X V6:%02X V7:%02X SP:%02X",cpu.V[0x4],cpu.V[0x5],cpu.V[0x6],cpu.V[0x7],cpu.sp);
	printf("\x1b[28;0HV8:%02X V9:%02X VA:%02X VB:%02X DT:%02X",cpu.V[0x8],cpu.V[0x9],cpu.V[0xA],cpu.V[0xB],cpu.dt);
	printf("\x1b[29;0HVC:%02X VD:%02X VE:%02X VF:%02X ST:%02X",cpu.V[0xC],cpu.V[0xD],cpu.V[0xE],cpu.V[0xF],cpu.st);
	printf("\x1b[1;0H+-+");
	printf("\x1b[2;0H| |Load State");
	printf("\x1b[3;0H+-+");
	printf("\x1b[4;0H| |Save State");
	printf("\x1b[5;0H+-+");
	
	printf("\x1b[7;0HCPU funcs: pause/reset");
	
	printf("\x1b[8;0H+-+ +-+");
	printf("\x1b[9;0H|%c| | |",paused);
	printf("\x1b[10;0H+-+ +-+");
	
	printf("\x1b[11;0H3D intensity:%02d",i3D_INTENSITY);
	
	printf("\x1b[12;0HState Number:%d",cpu.statenum);
	printf("\x1b[13;0H+-+ +-+");
	printf("\x1b[14;0H|-| |+|");
	printf("\x1b[15;0H+-+ +-+");
	
	printf("\x1b[17;0HScreen scale:%d",SCREEN_SCALE);
	printf("\x1b[18;0H+-+ +-+");
	printf("\x1b[19;0H|-| |+|");
	printf("\x1b[20;0H+-+ +-+");
	if(type == 'b')
	{
		int quit = 0;
		while(quit != 1)
		{
			for (x = 0; x < 16; x++)
			{
				cpu.key[x] = 0;
			}
			hidScanInput();
			touchbutton = CTRgettouch(pos2.px, pos2.py);
			for (x = 0; x < 16; x++)
			{
/*				if(hidKeysDown() & BIT(CTRkeymap[x]))
					cpu.key[x] = 1;
				if(hidKeysHeld() & BIT(CTRkeymap[x]))
					cpu.key[x] = 1;
				if(hidKeysUp() & BIT(CTRkeymap[x]))
					cpu.key[x] = 0;*/
				if(x == touchbutton)
					cpu.key[x] = 1;
			}
			if(hidKeysDown() & BIT(3))
				ExitHard();
			if(((hidKeysDown() & KEY_TOUCH)||(hidKeysHeld() & KEY_TOUCH))&&(touchbutton != -1))
			{
				for(;;)
				{
					hidScanInput();
					if(hidKeysUp() & KEY_TOUCH)
						break;
				}
			}
			for (x = 0; x < 16; x++)
			{
				if (cpu.key[x] == 1);
					quit = 1;
			}
		}
	}
}

int CTRgettouch(u16 tx, u16 ty)
{
	int x;
	for(x = 0; x < touchbuttoncount; x++)
	{
		if( ( (CTRtouchmap[x][0] < tx) && (tx < CTRtouchmap[x][2]) ) && ( (CTRtouchmap[x][1] < ty) && (ty < CTRtouchmap[x][3]) ) )
			return x;
	}
	return -1;
}

int CTRinit()
{
	int x;
//	colors[0] = 0x1E90FF00;
	colors[0] = 0x15151500;
	colors[1] = 0xA5303000;
	colors[2] = 0x15151500;
	gfxInit(GSP_RGBA8_OES,GSP_RGBA8_OES,false);
	gfxSetDoubleBuffering(GFX_TOP, true);
	gfxSet3D(true);
	consoleInit(GFX_BOTTOM, NULL);
	for(x = 0; x < 16; x++)
	{
		CTRkeymap[x] = CTRkeymap1[x];
	}
	if(strncmp(cpu.romname, "pong", 4) == 0)
	{
		for(x = 0; x < 16; x++)
		{
			CTRkeymap[x] = CTRkeymap2[x];
		}
	}
	if(strncmp(cpu.romname, "PONG", 4) == 0)
	{
		for(x = 0; x < 16; x++)
		{
			CTRkeymap[x] = CTRkeymap2[x];
		}
	}
	fbuff[0] = (u32*)(gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL));
	fbuff[2] = (u32*)(gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL));
	gfxFlushBuffers();
	gfxSwapBuffers();
	fbuff[1] = (u32*)(gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL));
	fbuff[3] = (u32*)(gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL));
	gfxFlushBuffers();
	gfxSwapBuffers();
	i3D_INTENSITY = (int)((CONFIG_3D_SLIDERSTATE)*-8);
	START_X = ( (400 - (64*SCREEN_SCALE) ) / 2);
	START_Y = ( (240 - (32*SCREEN_SCALE) ) / 2);
	cpu.touchdisabled = 0;
	return 0;
}

int CTR2drawscreen()
{
	colors[0] = 0x30303000;
	gspWaitForEvent(GSPEVENT_VBlank1, true);
	int x;
	int y;
	int sc1;
	int sc2;
	for(y = 0; y < 32; y++)
	{
		for(x = 0; x < 64; x++)
		{
			for(sc1 = 0; sc1 < SCREEN_SCALE; sc1++)
			{
				for(sc2 = 0; sc2 < SCREEN_SCALE; sc2++)
				{
					SetFBPixel(((x*SCREEN_SCALE)+sc1), ((y*SCREEN_SCALE)+sc2), 0, fbuff[0]);
					SetFBPixel(((x*SCREEN_SCALE)+sc1), ((y*SCREEN_SCALE)+sc2), 0, fbuff[2]);
				}
			}
			if(cpu.gfx[x][y] == 1)
			{
				for(sc1 = 0; sc1 < SCREEN_SCALE; sc1++)
				{
					for(sc2 = 0; sc2 < SCREEN_SCALE; sc2++)
					{
						SetFBPixel(((x*SCREEN_SCALE)+sc1), ((y*SCREEN_SCALE)+sc2), 1, fbuff[0]);
						SetFBPixel(((x*SCREEN_SCALE)+sc1), ((y*SCREEN_SCALE)+sc2), 1, fbuff[2]);
					}
				}
			}
		}
	}
	cpu.drawflag = 0;
	refresh();
	if(cpu.dt > 0)
		cpu.dt--;
	if(cpu.st > 0)
		cpu.st--;
	return 0;
}

int CTRclearscreen()
{
	int x;
	int y;
	for(y = 0; y < 240; y++)
	{
		for(x = 0; x < 400; x++)
		{	
			SetScrPixel(x, y, 2, fbuff[0]);
			SetScrPixel(x, y, 2, fbuff[2]);		
		}
	}
	cpu.drawflag = 1;
	refresh();
	return 0;
}

int CTRdrawscreen()
{
	if(CONFIG_3D_SLIDERSTATE == 0)
		return CTR2drawscreen();
	i3D_INTENSITY = (int)((CONFIG_3D_SLIDERSTATE)*-8);
	colors[0] = 0x15151500;
	colors[2] = colors[0];
	gspWaitForEvent(GSPEVENT_VBlank1, true);
	int x;
	int y;
	int sc1;
	int sc2;
	int temp3d;
	temp3d = i3D_INTENSITY;
	if(temp3d < 0)
		temp3d -= (2*temp3d);
	for(y = 0; y < 32; y++)
	{
		for(sc2 = 0; sc2 < SCREEN_SCALE; sc2++)
		{
			for(sc1 = 0; sc1 < 8; sc1++)								//max possible 3D value...
			{
				SetFBPixel((0 - sc1), ((y*SCREEN_SCALE)+sc2), 2, fbuff[0]);
				SetFBPixel(((64*SCREEN_SCALE)+sc1), ((y*SCREEN_SCALE)+sc2), 2, fbuff[0]);
				SetFBPixel((0 - sc1), ((y*SCREEN_SCALE)+sc2), 2, fbuff[2]);
				SetFBPixel(((64*SCREEN_SCALE)+sc1), ((y*SCREEN_SCALE)+sc2), 2, fbuff[2]);
			}
//			SetFBPixel((0 - 9), ((y*SCREEN_SCALE)+sc2), 2, fbuff[0]);
		}
		for(x = 0; x < 64; x++)
		{
			for(sc1 = 0; sc1 < SCREEN_SCALE; sc1++)
			{
				for(sc2 = 0; sc2 < SCREEN_SCALE; sc2++)
				{
					SetFBPixel(((x*SCREEN_SCALE)+sc1), ((y*SCREEN_SCALE)+sc2), 0, fbuff[0]);
					SetFBPixel(((x*SCREEN_SCALE)+sc1), ((y*SCREEN_SCALE)+sc2), 0, fbuff[2]);
				}
			}
		}
	}
	for(y = 0; y < 32; y++)
	{
		for(x = 0; x < 64; x++)
		{
			if(cpu.gfx[x][y] == 1)
			{
				for(sc1 = 0; sc1 < SCREEN_SCALE; sc1++)
				{
					for(sc2 = 0; sc2 < SCREEN_SCALE; sc2++)
					{
						SetFBPixel(((x*SCREEN_SCALE)+sc1)+i3D_INTENSITY, ((y*SCREEN_SCALE)+sc2), 1, fbuff[0]);
						SetFBPixel((((x*SCREEN_SCALE)+sc1)-i3D_INTENSITY), ((y*SCREEN_SCALE)+sc2), 1, fbuff[2]);
					}
				}
			}
		}
	}
	cpu.drawflag = 0;
	refresh();
	if(cpu.dt > 0)
		cpu.dt--;
	if(cpu.st > 0)
		cpu.st--;
	return 0;
}

void SetFBPixel(int pixx, int pixy, int pixc, u32* point)
{
	u32* point2;
	point2 = point;
	point2 += ((240*(pixx + 1 + START_X))+((-(pixy + 1 + START_Y))));
	*point2 = colors[pixc];
}

void SetScrPixel(int pixx, int pixy, int pixc, u32* point)
{
	u32* point2;
	point2 = point;
	point2 += ((240*(pixx + 1))+((-(pixy + 1))));
	*point2 = colors[pixc];
}

void refresh()
{
	u32* temp;
	temp     = fbuff[0];
	fbuff[0] = fbuff[1];
	fbuff[1] = temp;
	temp     = fbuff[2];
	fbuff[2] = fbuff[3];
	fbuff[3] = temp;
	gfxFlushBuffers();
	gfxSwapBuffers();
}

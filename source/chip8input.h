#include <stdio.h>
void ExitHard();
void resize(char ud);

int hang (int howlonghang)
{
	int x;
	for(x=0;x<(u64)(howlonghang*10000);x++)
	{
	}
	return 0;
};

void keypadclear()
{
	int x;
	for(x = 0; x < 16; x++)
	{
		cpu.key[x] = 0;
	}
};

int CTRkeytouch(u16 tx, u16 ty)
{
	int x;
	for(x = 0; x < typebuttoncount; x++)
	{
		if( ( (CTRtypemap[x][0] < tx) && (tx < CTRtypemap[x][2]) ) && ( (CTRtypemap[x][1] < ty) && (ty < CTRtypemap[x][3]) ) )
			return x;
	}
	return -1;
}

char keytoascii(int key)
{
	if(key == 0)
		return 13;
	if(key == 1)
		return 8;
	if(key == 2)
		return ' ';
	if(key == 3)
		return '\'';
	if((4 <= key)&&(key <= 6))
		return key+40;
	if((7 <= key)&&(key <= 16))
		return key+41;
	if(key == 17)
		return ';';
	if((18 <= key)&&(key <= 43))
		return key+47;
	if(key == 44)
		return '|';
	else
		return 0;
}

char CTRkeyboard()
{
	int touchbutton = 0;
	touchPosition pos2;
	hidScanInput();
	hidTouchRead(&pos2);
	
	int quit = 0;
	while(quit != 1)
	{
		for(;;)
		{
			printf("\x1b[19;0H+-+-+-+-+-+-+-+-+-+-+-+");
			printf("\x1b[20;0H|1|2|3|4|5|6|7|8|9|0|-|");
			printf("\x1b[21;0H+-+-+-+-+-+-+-+-+-+-+-+");
			printf("\x1b[22;0H|Q|W|E|R|T|Y|U|I|O|P|||");
			printf("\x1b[23;0H+-+-+-+-+-+-+-+-+-+-+-+");
			printf("\x1b[24;0H|A|S|D|F|G|H|J|K|L|;|<|");
			printf("\x1b[25;0H+-+-+-+-+-+-+-+-+-+-+-+");
			printf("\x1b[26;0H|<|Z|X|C|V|B|N|M|,|.|'|");
			printf("\x1b[27;0H+-+-+-+-+-+-+-+-+-+-+-+");
			printf("\x1b[28;0H|                     |");
			printf("\x1b[29;0H+---------------------+");
			
			hidScanInput();
			hidTouchRead(&pos2);
			
			if ((hidKeysDown() & KEY_START)||(hidKeysHeld() & KEY_START)||(hidKeysUp() & KEY_START))
				ExitHard();
			if(hidKeysHeld() & KEY_TOUCH)
			{
				touchbutton = CTRkeytouch(pos2.px, pos2.py);
			}
			if(hidKeysUp() & KEY_TOUCH)
				break;
		}
		if(touchbutton != -1)
		{
					
			printf("\x1b[19;0H                       ");
			printf("\x1b[20;0H                       ");
			printf("\x1b[21;0H                       ");
			printf("\x1b[22;0H                       ");
			printf("\x1b[23;0H                       ");
			printf("\x1b[24;0H                       ");
			printf("\x1b[25;0H                       ");
			printf("\x1b[26;0H                       ");
			printf("\x1b[27;0H                       ");
			printf("\x1b[28;0H                       ");
			printf("\x1b[29;0H                       ");

			return keytoascii(touchbutton);
		}
	}
	return keytoascii(touchbutton);
}

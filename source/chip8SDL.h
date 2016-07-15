void runcycle();
void Timer(int value);

Uint32 GlobPix[2];

void WriteGlobPix(int num, int r, int g, int b);
void SetFBPixel(int pixx, int pixy, int pixc);
int SDLAudioInit();

void drawscreen()
{
	int x;
	int y;
	int sc1;
	int sc2;
	for(y = 0; y < 32; y++)
	{
		for(x = 0; x < 64; x++)
		{
			if(cpu.gfx[x][y] == 0)
			{
				for(sc1 = 0; sc1 < SCREEN_SCALE; sc1++)
				{
					for(sc2 = 0; sc2 < SCREEN_SCALE; sc2++)
					{
						SetFBPixel(((x*SCREEN_SCALE)+sc1), ((y*SCREEN_SCALE)+sc2), 0);
					}
				}
			}
			if(cpu.gfx[x][y] == 1)
			{
				for(sc1 = 0; sc1 < SCREEN_SCALE; sc1++)
				{
					for(sc2 = 0; sc2 < SCREEN_SCALE; sc2++)
					{
						SetFBPixel(((x*SCREEN_SCALE)+sc1), ((y*SCREEN_SCALE)+sc2), 1);
					}
				}
			}
		}
	}
	cpu.drawflag = 0;
	SDL_UpdateWindowSurface( sWindow );
}

int SDLInit()
{
	SCREEN_WIDTH = 64*SCREEN_SCALE;
	SCREEN_HEIGHT = 32*SCREEN_SCALE;
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO | SDL_INIT_TIMER ) < 0 )
	{
		wprintw(stdscr, "SDL Error: %s\n", SDL_GetError() );
		return 1;
	}
	sWindow = SDL_CreateWindow( "Chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if(sWindow == NULL)
	{
		return 1;
	}
	sScreen = SDL_GetWindowSurface( sWindow );
	WriteGlobPix(0, 0, 0, 0);
	WriteGlobPix(1, 255, 255, 255);
	if(SDLAudioInit() != 0)
	{
		return 2;
	}
	return 0;
}

void WriteGlobPix(int num, int r, int g, int b)
{
	/* Extracting color components from a 32-bit color value */
	SDL_PixelFormat *fmt;
	Uint32 temp, pixel;
	
	fmt = sScreen->format;
	
	pixel = 0;
	
	temp = r;
	temp = temp >> fmt->Rloss;
	temp = temp << fmt->Rshift;
	pixel = pixel | (temp & fmt->Rmask);

	temp = g;
	temp = temp >> fmt->Gloss;
	temp = temp << fmt->Gshift;
	pixel = pixel | (temp & fmt->Gmask);
	
	temp = b;
	temp = temp >> fmt->Bloss;
	temp = temp << fmt->Bshift;
	pixel = pixel | (temp & fmt->Bmask);
	
	temp = 0;
	temp = temp >> fmt->Aloss;
	temp = temp << fmt->Ashift;
	pixel = pixel | (temp & fmt->Amask);
	
	
	GlobPix[num] = pixel;
}

void SetFBPixel(int pixx, int pixy, int pixc)
{
	SDL_PixelFormat *fmt;
	Uint32 sx;
	Uint8 Bpp;
	
	fmt = sScreen->format;
	Bpp = fmt->BytesPerPixel;
	sx = sScreen->w;
	
	int* point;
	point = sScreen->pixels+(((Bpp*sx)*(pixy))+(Bpp*pixx));
	*point = GlobPix[pixc];
}

void resize(char ud)
{
	SDL_DestroyWindow(sWindow);
	if((ud == 1) && (SCREEN_SCALE < 21))
	{
		SCREEN_SCALE += 1;
	}
	if((ud == 0) && (SCREEN_SCALE > 1))
	{
		SCREEN_SCALE -= 1;
	}
	SDL_Quit();
	SDLInit();
}

void SDLBeep(char beepstate)
{
	if(beepstate == 1)
	{
		if((SDL_GetAudioDeviceStatus(devicenum) == SDL_AUDIO_STOPPED) || (SDL_GetAudioDeviceStatus(devicenum) == SDL_AUDIO_PAUSED) || ((0 <= SDL_GetQueuedAudioSize(devicenum)) && (SDL_GetQueuedAudioSize(devicenum) <= 2048)))
		{
		 	SDL_QueueAudio(devicenum, *beepsound.buffer, beepsound.length);
		}
	}
	if(beepstate == 0)
	{
		SDL_ClearQueuedAudio(devicenum);
	}
}

int SDLAudioInit()
{
	SDL_AudioSpec temp;
	Uint8*	point1;
	Uint8*	point1b;
	Uint16*	point2;
	Uint32*	point3;
	int*	point4;
	int		tempint;
	char retval;
	
	retval = 0;
	beepsound.length = 8820;
	beepsound.buffer = SDL_malloc(beepsound.length);
	if(beepsound.buffer == NULL)
		return 1;
	SDL_zero(temp);
	temp.freq = 44100;
	temp.format = AUDIO_S16LSB;
	temp.channels = 1;
	temp.samples = 4410;
	devicenum = SDL_OpenAudioDevice(0, 0, &temp, &device, SDL_AUDIO_ALLOW_ANY_CHANGE);
	if(devicenum == 0)
		return 1;
	beepsound.meta = SDL_LoadWAV("beep.wav", &device, beepsound.buffer, &beepsound.length);
	if(beepsound.meta == NULL)
		return 1;
	SDL_AudioCVT beeps;
	SDL_zero(temp);
	
	point1 = (Uint8*)beepsound.meta;
	point2 = (Uint16*)beepsound.meta;
	point3 = (Uint32*)beepsound.meta;
	point4 = (Sint32*)beepsound.meta;
	
	temp.freq = *point4;
	point2 = (SDL_AudioFormat *)beepsound.meta + (&device.format - (SDL_AudioFormat *)&device);
	temp.format = *point2;
	point1 = (Uint8 *)beepsound.meta + (&device.channels - (Uint8 *)&device);
	temp.channels = *point1;
	point1 = (Uint8 *)beepsound.meta + (&device.silence - (Uint8 *)&device);
	temp.silence = *point1;
	point2 = (Uint16 *)beepsound.meta + (&device.samples - (Uint16 *)&device);
	temp.samples = *point2;
	point3 = (Uint32 *)beepsound.meta + (&device.size - (Uint32 *)&device);
	temp.size = *point3;
	wclear(stdscr);
	wrefresh(stdscr);
	SDL_BuildAudioCVT(&beeps, temp.format, temp.channels, temp.freq, device.format, device.channels, device.freq);
	if(beeps.needed == 1)
	{
		if((temp.format && 0xFF) > (device.format && 0xFF))
		{
			beeps.len = ((temp.format && 0xFF)/8) * temp.channels * temp.samples;
		}
		if((temp.format && 0xFF) <= (device.format && 0xFF))
		{
			beeps.len = ((device.format && 0xFF)/8) * device.channels * temp.samples;
		}
		beeps.buf = (Uint8 *)SDL_malloc(beeps.len * beeps.len_mult);
		
		point1 = NULL;
		point1b= NULL;
		
		point1 = *beepsound.buffer;
		point1b = beeps.buf;
		
		for(tempint = 0; tempint < beepsound.length; tempint++)
		{
			*point1b = *point1;
			point1++;
			point1b++;
		}
		retval = SDL_ConvertAudio(&beeps);
		if(retval != 0)
			return retval;
		point1 = NULL;
		point1b= NULL;
		
		point1 = *beepsound.buffer;
		point1b = beeps.buf;
		
		for(tempint = 0; tempint < beepsound.length; tempint++)
		{
			*point1 = *point1b;
			point1++;
			point1b++;
		}
		SDL_zero(temp);
		
		point1 = (Uint8*)beepsound.meta;
		point2 = (Uint16*)beepsound.meta;
		point3 = (Uint32*)beepsound.meta;
		point4 = (Sint32*)beepsound.meta;
		
		*point4 = device.freq;
		point2 = (SDL_AudioFormat *)beepsound.meta + (&device.format - (SDL_AudioFormat *)&device);
		*point2 = device.format;
		point1 = (Uint8 *)beepsound.meta + (&device.channels - (Uint8 *)&device);
		*point1 = device.channels;
		point1 = (Uint8 *)beepsound.meta + (&device.silence - (Uint8 *)&device);
		*point1 = device.silence;
		point2 = (Uint16 *)beepsound.meta + (&device.samples - (Uint16 *)&device);
		*point2 = device.samples;
		point3 = (Uint32 *)beepsound.meta + (&device.size - (Uint32 *)&device);
		*point3 = device.size;
	}
	
	SDL_PauseAudioDevice(devicenum, 0);
	return retval;
}

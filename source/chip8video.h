void ScaleMinus()
{
	if(SCREEN_SCALE > 1)
		SCREEN_SCALE--;
	START_X = ( (400 - (64*SCREEN_SCALE) ) / 2);
	START_Y = ( (240 - (32*SCREEN_SCALE) ) / 2);
	CTRclearscreen();
	CTRclearscreen();
	return;
}

void ScalePlus()
{
	if(SCREEN_SCALE < 6)
		SCREEN_SCALE++;
	START_X = ( (400 - (64*SCREEN_SCALE) ) / 2);
	START_Y = ( (240 - (32*SCREEN_SCALE) ) / 2);
	CTRclearscreen();
	CTRclearscreen();
	return;
}

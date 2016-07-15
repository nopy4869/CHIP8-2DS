/*
int colex;

void colorcheck()
{
	if(has_colors() == TRUE)
	{
		colex = 1;
		start_color();
		init_pair(1, COLOR_RED, COLOR_BLACK);
		init_pair(2, COLOR_BLUE, COLOR_BLACK);
		attron(COLOR_PAIR(1));
		wprintw(stdscr, "Terminal supports");
		attroff(COLOR_PAIR(1));
		attron(COLOR_PAIR(2));
		wprintw(stdscr, " COLOR!! :D\n");
		attroff(COLOR_PAIR(2));
		wrefresh(stdscr);
		hang(3);
	}
	else
	{
		wprintw(stdscr, "Terminal doesn't support color :( \n");
		wrefresh(stdscr);
		hang(3);
	}
}

void cyclemem()
{
	int x;
	int colorized = 0;
	if(colex == 1)
	{
		init_pair(1, COLOR_BLACK, COLOR_WHITE);
		init_pair(2, COLOR_BLUE, COLOR_BLACK);
	}
	mvwprintw(srcwin, 0, 0,"");
	for(x = 512;x < (cpu.romsize+512);x++)
	{
		colorized = 0;
		if((colex == 1) && ((x % 4) == 0))
		{
			colorized = 2;
			attron(COLOR_PAIR(2));
		}
		if((colex == 1) && (x == cpu.pc))
		{
			if(colorized == 2)
				attroff(COLOR_PAIR(2));
			colorized = 1;
			attron(COLOR_PAIR(1));
		}
		wprintw(srcwin, "%02X%02X", cpu.memory[x], cpu.memory[x+1]);
		if(colorized == 1)
			attroff(COLOR_PAIR(1));
		if(colorized == 2)
			attroff(COLOR_PAIR(2));
		x++;
	}
	mvwprintw(dbgwin, 0, 0,"");
	wprintw(dbgwin, "V[0] = %d (%04X)\n", cpu.V[0], cpu.V[0]);
	wprintw(dbgwin, "V[1] = %d (%04X)\n", cpu.V[1], cpu.V[1]);
	wprintw(dbgwin, "V[2] = %d (%04X)\n", cpu.V[2], cpu.V[2]);
	wprintw(dbgwin, "V[3] = %d (%04X)\n", cpu.V[3], cpu.V[3]);
	wprintw(dbgwin, "V[4] = %d (%04X)\n", cpu.V[4], cpu.V[4]);
	wprintw(dbgwin, "V[5] = %d (%04X)\n", cpu.V[5], cpu.V[5]);
	wprintw(dbgwin, "V[6] = %d (%04X)\n", cpu.V[6], cpu.V[6]);
	wprintw(dbgwin, "V[7] = %d (%04X)\n", cpu.V[7], cpu.V[7]);
	wprintw(dbgwin, "V[8] = %d (%04X)\n", cpu.V[8], cpu.V[8]);
	wprintw(dbgwin, "V[9] = %d (%04X)\n", cpu.V[9], cpu.V[9]);
	wprintw(dbgwin, "V[A] = %d (%04X)\n", cpu.V[10], cpu.V[10]);
	wprintw(dbgwin, "V[B] = %d (%04X)\n", cpu.V[11], cpu.V[11]);
	wprintw(dbgwin, "V[C] = %d (%04X)\n", cpu.V[12], cpu.V[12]);
	wprintw(dbgwin, "V[D] = %d (%04X)\n", cpu.V[13], cpu.V[13]);
	wprintw(dbgwin, "V[E] = %d (%04X)\n", cpu.V[14], cpu.V[14]);
	wprintw(dbgwin, "V[F] = %d (%04X)\n", cpu.V[15], cpu.V[15]);
	wprintw(dbgwin, "   I = %d (%04X)\n", cpu.I, cpu.I);
	wprintw(dbgwin, "S[0] = %d (%04X)\n", cpu.stack[0], cpu.stack[0]);
	wprintw(dbgwin, "S[1] = %d (%04X)\n", cpu.stack[1], cpu.stack[1]);
	wprintw(dbgwin, "S[2] = %d (%04X)\n", cpu.stack[2], cpu.stack[2]);
	wprintw(dbgwin, "S[3] = %d (%04X)\n", cpu.stack[3], cpu.stack[3]);
	wprintw(dbgwin, "S[4] = %d (%04X)\n", cpu.stack[4], cpu.stack[4]);
	wprintw(dbgwin, "S[5] = %d (%04X)\n", cpu.stack[5], cpu.stack[5]);
	wprintw(dbgwin, "S[6] = %d (%04X)\n", cpu.stack[6], cpu.stack[6]);
	wprintw(dbgwin, "S[7] = %d (%04X)\n", cpu.stack[7], cpu.stack[7]);
	wprintw(dbgwin, "S[8] = %d (%04X)\n", cpu.stack[8], cpu.stack[8]);
	wprintw(dbgwin, "S[9] = %d (%04X)\n", cpu.stack[9], cpu.stack[9]);
	wprintw(dbgwin, "S[A] = %d (%04X)\n", cpu.stack[10], cpu.stack[10]);
	wprintw(dbgwin, "S[B] = %d (%04X)\n", cpu.stack[11], cpu.stack[11]);
	wprintw(dbgwin, "S[C] = %d (%04X)\n", cpu.stack[12], cpu.stack[12]);
	wprintw(dbgwin, "S[D] = %d (%04X)\n", cpu.stack[13], cpu.stack[13]);
	wprintw(dbgwin, "S[E] = %d (%04X)\n", cpu.stack[14], cpu.stack[14]);
	wprintw(dbgwin, "S[F] = %d (%04X)\n", cpu.stack[15], cpu.stack[15]);
	wprintw(dbgwin, "  sp = %d\n", cpu.sp);
	wprintw(dbgwin, "  dt = %d\n", cpu.dt);
	wprintw(dbgwin, "  st = %d", cpu.st);
	mvwprintw(keywin, 0, 0,"");
	char exes[16] = {'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'};
	for(x=0;x<16;x++)
	{
		if(cpu.key[x] == 1)
			exes[x] = '#';
	}
	wprintw(keywin, "%c%c%c%c\n", exes[1],  exes[2],  exes[3],  exes[12]);
	wprintw(keywin, "%c%c%c%c\n", exes[4],  exes[5],  exes[6],  exes[13]);
	wprintw(keywin, "%c%c%c%c\n", exes[7],  exes[8],  exes[9],  exes[14]);
	wprintw(keywin, "%c%c%c%c\n", exes[10], exes[0],  exes[11], exes[15]);
	wrefresh(srcwin);
	wrefresh(dbgwin);
	wrefresh(keywin);
}
*/

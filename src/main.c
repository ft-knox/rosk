/**
 * rosk
 * A randomizing on screen keyboard using ncurses
 *
 * Copyright 2016 Ft. Knox <ftknox@riseup.net>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#define _GNU_SOURCE

#define KBD_SIZE 64
#define KBD_KPERL 10

#define MIN(X, Y) Y ^ ((X ^ Y) & -(X < Y))
#define MAX(X, Y) X ^ ((X ^ Y) & -(X < Y))

#include <config.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

char*
stradd(const char* a, const char* b)
{
	size_t len = strlen(a) + strlen(b);
	char *ret = (char*)malloc(len * sizeof(char) + 1);
	return strcat(strcat(ret, a), b);
}

void
shuffle(int *array, size_t n)
{
	if (n > 1)
		{
			size_t i;
			for (i = 0; i < n - 1; i++)
				{
					size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
					char t = array[j];
					array[j] = array[i];
					array[i] = t;
				}
		}
}

void
print_kdb(int *array, size_t n, int pos)
{
	if (n > 1)
		{
			size_t i;
			for (i = 0; i < n - 1; i++) {
				if (i == pos)
					{
						attron(COLOR_PAIR(1));
						attron(A_BOLD);
						printw("%c ",array[i]);
						attroff(A_BOLD);
						attroff(COLOR_PAIR(1));
					}
				else
					{
						printw("%c ",array[i]);
					}
			}
		}
}

int
main (void)
{
	char buffer[50];
	int length = 0;
	int ch = NULL;

	//97981226590484957
		
	int keys[KBD_SIZE] = {97,98,99,100,101,102,103,104,105,106,107,108,
	                      109,110,111,112,113,114,115,116,117,118,119,
	                      120,121,122,65,66,67,68,69,70,71,72,73,74,76,
	                      77,78,79,80,81,82,83,84,85,86,87,88,89,90,48,
	                      49,50,51,52,53,54,55,56,57};
	
	size_t klen = KBD_SIZE;
	const int KWIDTH = COLS / KBD_KPERL;
	const int KHEIGHT = LINES / KBD_KPERL;
	
	// Initialize random number generator
	time_t t;
	srand((unsigned) time(&t));

	// Initially shuffle keyboard
	shuffle(keys, klen);
	int kpos = rand() % KBD_SIZE;
			
	printf("\n" PACKAGE_STRING ".\n");
	printf("Copyright (C) 2016  Ft. Knox <ftknox@riseup.net>\n\n\
rosk is free software: you can redistribute it and/or modify\n\
it under the terms of the GNU General Public License as published by\n\
the Free Software Foundation, either version 3 of the License, or\n\
(at your option) any later version.\n\n\
rosk is distributed in the hope that it will be useful,\n\
but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\
GNU General Public License for more details.\n\n\
You should have received a copy of the GNU General Public License\n\
along with rosk.  If not, see <http://www.gnu.org/licenses/>.\n\n");

	// Set up ncurses
	initscr();
	noecho();
	raw();
	nonl();
	keypad(stdscr, TRUE);
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);

	// Show the initial keyboard
	print_kdb(keys, klen, kpos);
	
	refresh(); // Update the real screen

	ch = getch();
	while (ch != KEY_F(1) && length < 50)
		{
			switch(ch) {
			case KEY_UP:
				kpos = kpos - KBD_KPERL;				
				refresh();
				clear();
				print_kdb(keys, klen, kpos);
				break;
			case KEY_DOWN:
				kpos = kpos + KBD_KPERL;
				refresh();
				clear();
				print_kdb(keys, klen, kpos);
				break;
			case KEY_LEFT:
				kpos--;
				kpos = MAX(0, kpos);
				refresh();
				clear();
				print_kdb(keys, klen, kpos);
				break;
			case KEY_RIGHT:
				kpos++;
				kpos = MIN(kpos, (klen-2));
				refresh();
				clear();
				print_kdb(keys, klen, kpos);
				break;
			case 32: // Space
				sprintf(&buffer[length], "%c", keys[kpos % KBD_SIZE]);
				length++;
				kpos = rand() % KBD_SIZE;
				shuffle(keys, klen);
				refresh();
				clear();
				print_kdb(keys, klen, kpos);
				break;
			case 13: // Enter
				refresh();
				clear();
				printw("%s", buffer);
				break;
			default:
				//printw("%d", ch);
				break;
			}
			
			refresh();
			ch = getch(); // wait for user input
		}

	// Free memory
	//free(print_buf);
	
	// Tear down ncurses
	echo();
	keypad(stdscr, FALSE);
	endwin(); // Stop nurses mode
	return 0;
}

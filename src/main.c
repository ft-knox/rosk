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

#define KDB_SIZE 64
#define KDB_KPERL 10

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

int
main (void)
{
	char buffer[50];
	int length = 0;
	int ch = NULL;
	int kpos = 0;

	//97981226590484957
		
	int keys[KDB_SIZE] = {97,98,99,100,101,102,103,104,105,106,107,108,
	                      109,110,111,112,113,114,115,116,117,118,119,
	                      120,121,122,65,66,67,68,69,70,71,72,73,74,76,
	                      77,78,79,80,81,82,83,84,85,86,87,88,89,90,48,
	                      49,50,51,52,53,54,55,56,57};
	
	size_t klen = KDB_SIZE;
	const int KWIDTH = COLS / KDB_KPERL;
	const int KHEIGHT = LINES / KDB_KPERL;
	
	// Initialize random number generator
	time_t t;
	srand((unsigned) time(&t));
	
	// Initially shuffle keyboard
	shuffle(keys, klen);

	// Set up ncurses
	initscr();
	noecho();
	raw();
	nonl();
	keypad(stdscr, TRUE);
		
	printw("rosk\n");
	printw("This is " PACKAGE_STRING ".\n");

	refresh(); // Update the real screen

	ch = getch();
	while (ch != KEY_F(1) && length < 50)
		{
			switch(ch) {
			case KEY_UP:
				kpos = kpos - KDB_KPERL;
				break;
			case KEY_DOWN:
				kpos = kpos + KDB_KPERL;
				break;
			case KEY_LEFT:
				kpos--;
				break;
			case KEY_RIGHT:
				kpos++;
				break;
			case 13: // Enter
				refresh();
				clear();
				printw("%s", buffer);
				break;
			default:
				sprintf(&buffer[length], "%c", ch);
				length++;
				//printw("%d %s", length, buffer);
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

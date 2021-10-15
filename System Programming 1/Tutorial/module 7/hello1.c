/* hello1.c
 *	purpose	 show the minimal calls needed to use curses
 *	outline  initialize, draw stuff, wait for input, quit
 */

#include	<stdio.h>
#include	<unistd.h>
#include	<curses.h>

int main(int arg, char * msg[])
{
	initscr() ;		/* turn on curses	*/

				/* send requests	*/
	int num = 1;
	do{
		clear();			/* clear screen	*/
		move(LINES-1,0);
		refresh();
		sleep(1);
		move(10,20);			/* row10,col20	*/
		if(arg == 1)
			addstr("Hello, world");		/* add a string	*/
		else
			addstr(msg[1]);
		move(LINES-1,0);		/* move to LL	*/
		refresh();		/* update the screen	*/

		sleep(1);
		num++;
	} while(num <= 5);

	//getch();		/* wait for user input	*/

	endwin();		/* turn off curses	*/
}

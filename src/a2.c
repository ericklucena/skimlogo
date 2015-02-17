/* 
* Skim-logo Main file
* @Author: Erick Lucena Palmeira Silva
* @Date:   2015-01-23 15:46:17
* @Last Modified by:   Erick Lucena Palmeira Silva
* @Last Modified time: 2015-02-13 19:53:31
* This file contains the main function of the game
*/

#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include "logo.h"
#include "util.h"

int main () {

    LogoBoard* logoBoard;

    initscr();
    cbreak ();
    noecho ();
    start_color();
    curs_set(0);
    initColorsPairs();

    logoBoard = getLogoBoard();
    logoBoard->turtle.pen = enabled;

    while (logoBoard->running)
    {
        printBoard(logoBoard);
        readInput(logoBoard);
    }

    freeBoard(logoBoard);
    endwin();

    return 0;
}

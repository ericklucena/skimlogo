/* 
* Skim-logo Logo functions implementation
* @Author: Erick Lucena Palmeira Silva
* @Date:   2015-01-23 14:50:05
* @Last Modified by:   Erick Lucena Palmeira Silva
* @Last Modified time: 2015-02-17 00:29:20
* This file contains the implementation of the functions responsible for the
* graphics components of the Logo Program
*/

#include "logo.h"
#include "util.h"
#include "file.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>

Line* newLine (int length)
{
    Line* line;

    line = malloc(sizeof(Line));
    
    if (line != NULL)
    {
        line->length = length;
        line->position = malloc(sizeof(Coordinate)*length);
        if (line->position == NULL)
        {
            free(line);
        }
    }

    return line;
}

void freeLine (Line* line)
{
    if (line != NULL)
    {
        if (line->position != NULL)
        {
            free(line->position);
        }
        free(line);
    }
}

LogoBoard* newBoard (int x, int y)
{
    int i;
    LogoBoard* logoBoard;
    Square** board;

    board = malloc(sizeof(Square*)*x);

    if (board != NULL)
    {
        for (i=0; i < x; i++)
        {
            board[i] = malloc(sizeof(Square)*y);
            if (board[i] == NULL)
            {
                for (i-=1;i>=0;i++)
                {
                    free(board[i]);
                }
                free(board);
                board = NULL;
            }
        }
    }

    if (board != NULL)
    {
        logoBoard = malloc(sizeof(LogoBoard));

        if (logoBoard != NULL)
        {
            logoBoard->commandIndex = 0;

            for (i = 0; i <LOGO_COMMANDS_BUFFER; i++)
            {
                logoBoard->lastCommands[i][0] = '\0';
            }

            logoBoard->height = x;
            logoBoard->width = y;
            logoBoard->board = board;
            logoBoard->file.writing = false;
            logoBoard->running = true;
        }
        
    }

    return logoBoard;   
}

void initBoard (LogoBoard* logoBoard)
{
    logoBoard->turtle.position.x = 0;
    logoBoard->turtle.position.y = 0;
    logoBoard->turtle.visible = true;
    logoBoard->turtle.angle = 90;
    logoBoard->turtle.penColor = white;
}

void freeBoard (LogoBoard* logoBoard)
{
    int i;

    for (i=0; i<logoBoard->height; i++)
    {
        free(logoBoard->board[i]);
    }
    
    free(logoBoard->board);
    free(logoBoard);
}

LogoBoard* getLogoBoard ()
{
    static LogoBoard* logoBoard = NULL;

    // Will enter here on the first call
    if (logoBoard == NULL)
    {
        logoBoard = newBoard(BOARD_HEIGHT, BOARD_WIDTH);
        initBoard(logoBoard);
    }

    return logoBoard;
}

Coordinate toAbsolute (Coordinate position, int width, int height)
{
    Coordinate absolute;
    int maxAxisX = (width-1)/2;
    int maxAxisY = (height-1);

    absolute.x = (maxAxisY - position.y)/2;
    absolute.y = position.x + maxAxisX;

    return absolute;
}

Coordinate toCartesian (Coordinate position, int width, int height)
{
    Coordinate cartesian;
    int maxAxisX = width/2;
    int maxAxisY = height/2;

    cartesian.x = position.y - maxAxisY;
    cartesian.y = maxAxisX + position.x;

    return cartesian;
}

Coordinate calculateDestination (Coordinate position, int angle, int distance)
{
    int x;
    int y;
    Coordinate destination;

    angle %= 360;
    x = round(cos(angle%90*M_PI/180)*distance);
    y = round(sin(angle%90*M_PI/180)*distance);

    // Each quarter has an specific calculation
    if (angle>=0 && angle < 90)
    {
        destination.x = position.x + x;
        destination.y = position.y + y;
    }
    else if (angle>=90 && angle < 180)
    {
        destination.x = position.x - y;
        destination.y = position.y + x;
    }
    else if (angle>=180 && angle < 270)
    {
        destination.x = position.x - x;
        destination.y = position.y - y;
    }
    else if (angle>=270 && angle < 360)
    {
        destination.x = position.x + y;
        destination.y = position.y - x;
    }

    return destination;
}

Line* calculateLine (Coordinate origin, Coordinate destination)
{
    int i;
    int length;
    int xLength;
    int yLength;
    double step = 0;
    bool invertedX = false;
    bool invertedY = false;
    bool abscissa = true;
    Line* line;

    xLength = destination.x - origin.x;
    yLength = destination.y - origin.y;

    if (xLength < 0)
    {
        xLength *=-1;
        invertedX = true;
    }

    if (yLength < 0)
    {
        yLength*=-1;
        invertedY = true;
    }

    // Defines the size of the line (greater axis)
    if (xLength>=yLength)
    {
        length = xLength+1;
        abscissa = true;

        if (yLength != 0)
        {
            step = (double) yLength/xLength;
        }
    }
    else
    {
        length = yLength+1;
        abscissa = false;

        if (xLength != 0)
        {
            step = (double) xLength/yLength;
        }
    }

    line = newLine(length);

    for (i=0; i < length; i++)
    {
        if (step != 0)
        {
            if (invertedX)
            {
                line->position[i].x = origin.x-(!abscissa?round(i*step):i);
            }
            else
            {
                line->position[i].x = origin.x+(!abscissa?round(i*step):i);

            }

            if (invertedY)
            {
                line->position[i].y = origin.y-(abscissa?round(i*step):i);
            }
            else
            {
                line->position[i].y = origin.y+(abscissa?round(i*step):i);
            }
        }
        else
        {
            if (invertedX)
            {
                line->position[i].x = origin.x-(abscissa?i:0);
            }
            else
            {
                line->position[i].x = origin.x+(abscissa?i:0);

            }

            if (invertedY)
            {
                line->position[i].y = origin.y-(!abscissa > xLength?i:0);
            }
            else
            {
                line->position[i].y = origin.y+(!abscissa > xLength?i:0);
            }
        }
    }

    return line;
}

void initColorsPairs ()
{
    init_pair(BORDER_COLOR_PAIR, COLOR_BLUE, COLOR_WHITE);
    init_pair(TURTLE_COLOR_PAIR, COLOR_BLACK, COLOR_GREEN);
    init_pair(BLACK_COLOR_PAIR, COLOR_BLACK, COLOR_BLACK);
    init_pair(RED_COLOR_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN_COLOR_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(BLUE_COLOR_PAIR, COLOR_BLUE, COLOR_BLACK);
    init_pair(YELLOW_COLOR_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(MAGENTA_COLOR_PAIR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(CYAN_COLOR_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(WHITE_COLOR_PAIR, COLOR_WHITE, COLOR_BLACK);
}

void turnColorPairOn (Color color)
{
    switch (color)
    {
    case white:
        attron(COLOR_PAIR(WHITE_COLOR_PAIR));
        break;
    case red:
        attron(COLOR_PAIR(RED_COLOR_PAIR));
        break;
    case green:
        attron(COLOR_PAIR(GREEN_COLOR_PAIR));
        break;
    case yellow:
        attron(COLOR_PAIR(YELLOW_COLOR_PAIR));
        break;
    case blue:
        attron(COLOR_PAIR(BLUE_COLOR_PAIR));
        break;
    case magenta:
        attron(COLOR_PAIR(MAGENTA_COLOR_PAIR));
        break;
    case cyan:
        attron(COLOR_PAIR(CYAN_COLOR_PAIR));
        break;
    case black:
        attron(COLOR_PAIR(BLACK_COLOR_PAIR));
        break;
    default:
        break;
    }
}

void turnColorPairOff (Color color)
{
    switch (color)
    {
    case white:
        attroff(COLOR_PAIR(WHITE_COLOR_PAIR));
        break;
    case red:
        attroff(COLOR_PAIR(RED_COLOR_PAIR));
        break;
    case green:
        attroff(COLOR_PAIR(GREEN_COLOR_PAIR));
        break;
    case yellow:
        attroff(COLOR_PAIR(YELLOW_COLOR_PAIR));
        break;
    case blue:
        attroff(COLOR_PAIR(BLUE_COLOR_PAIR));
        break;
    case magenta:
        attroff(COLOR_PAIR(MAGENTA_COLOR_PAIR));
        break;
    case cyan:
        attroff(COLOR_PAIR(CYAN_COLOR_PAIR));
        break;
    case black:
        attroff(COLOR_PAIR(BLACK_COLOR_PAIR));
        break;
    default:
        break;
    }
}

void cleanBoard (LogoBoard* logoBoard)
{
    int i;
    int j;

    for (i=0; i< logoBoard->height; i++)
    {
        for (j=0; j< logoBoard->width; j++)
        {
            logoBoard->board[i][j].state = blank;
        }
    }
}

void validateTurtle (LogoBoard* logoBoard)
{
    if (logoBoard->turtle.position.x > (logoBoard->width-1)/2)
    {
        logoBoard->turtle.position.x = (logoBoard->width-1)/2;
    }
    
    if (logoBoard->turtle.position.x < (logoBoard->width/2)*-1)
    {
        logoBoard->turtle.position.x = logoBoard->width/2*-1;
    }
    
    if (logoBoard->turtle.position.y > logoBoard->height-1)
    {
        logoBoard->turtle.position.y = logoBoard->height-1;
    }
    
    if (logoBoard->turtle.position.y < (logoBoard->height-1)*-1)
    {
        logoBoard->turtle.position.y = (logoBoard->height-1)*-1;
    }
}

void drawLine (Line* line, LogoBoard* logoBoard)
{
    int i;
    Coordinate absolute;

    if (logoBoard->turtle.pen == enabled)
    {
        for (i=0; i<line->length; i++)
        {   
            absolute = toAbsolute(line->position[i], logoBoard->width, logoBoard->height);
            // If the line still in the board
            if ((absolute.x >= 0 && absolute.x < logoBoard->height) && (absolute.y >= 0 && absolute.y < logoBoard->width))
            {
                logoBoard->board[absolute.x][absolute.y].state = written;
                logoBoard->board[absolute.x][absolute.y].foreground = logoBoard->turtle.penColor;
            }
        }
    }
}

void printBoard (LogoBoard* logoBoard)
{
    int i;
    int j;
    int borderWidth;
    int borderHeight;
    Coordinate absolute;

    borderHeight = logoBoard->height+COMMANDBOX_HEIGHT+1;
    borderWidth = logoBoard->width+1;

    attron(COLOR_PAIR(BORDER_COLOR_PAIR));

    //Draws the horizontal lines
    for (i=0; i<=borderWidth + STATUS_WIDTH; i++)
    {
        mvaddch(BOARD_X_OFFSET, BOARD_Y_OFFSET+i, ACS_HLINE);
        mvaddch(BOARD_X_OFFSET+borderHeight-COMMANDBOX_HEIGHT, BOARD_Y_OFFSET+i, ACS_HLINE);
        mvaddch(BOARD_X_OFFSET+borderHeight, BOARD_Y_OFFSET+i, ACS_HLINE);

    }

    //Draws the vertical lines
    for (i=0; i<=borderHeight; i++)
    {
        mvaddch(BOARD_X_OFFSET+i, BOARD_Y_OFFSET, ACS_VLINE);
        mvaddch(BOARD_X_OFFSET+i, BOARD_Y_OFFSET+borderWidth, ACS_VLINE);
        mvaddch(BOARD_X_OFFSET+i, BOARD_Y_OFFSET+borderWidth+STATUS_WIDTH, ACS_VLINE);
    }

    //Draws board corners
    mvaddch(BOARD_X_OFFSET, BOARD_Y_OFFSET, ACS_ULCORNER);
    mvaddch(BOARD_X_OFFSET, BOARD_Y_OFFSET+borderWidth+STATUS_WIDTH, ACS_URCORNER);
    mvaddch(BOARD_X_OFFSET, BOARD_Y_OFFSET+borderWidth, ACS_TTEE);
    mvaddch(BOARD_X_OFFSET+borderHeight-COMMANDBOX_HEIGHT, BOARD_Y_OFFSET, ACS_LTEE);
    mvaddch(BOARD_X_OFFSET+borderHeight-COMMANDBOX_HEIGHT, BOARD_Y_OFFSET+borderWidth, ACS_PLUS);
    mvaddch(BOARD_X_OFFSET+borderHeight-COMMANDBOX_HEIGHT, BOARD_Y_OFFSET+borderWidth+STATUS_WIDTH, ACS_RTEE);
    mvaddch(BOARD_X_OFFSET+borderHeight, BOARD_Y_OFFSET, ACS_LLCORNER);
    mvaddch(BOARD_X_OFFSET+borderHeight, BOARD_Y_OFFSET+borderWidth, ACS_BTEE);
    mvaddch(BOARD_X_OFFSET+borderHeight, BOARD_Y_OFFSET+borderWidth+STATUS_WIDTH, ACS_LRCORNER);

    attroff(COLOR_PAIR(BORDER_COLOR_PAIR));
    



    //Draws board elements
    for (i=0; i<logoBoard->height; i++)
    {
        for (j=0; j<logoBoard->width; j++)
        {
            if (logoBoard->board[i][j].state == written)
            {
                turnColorPairOn(logoBoard->board[i][j].foreground);
                mvaddch(BOARD_X_OFFSET+i+1, BOARD_Y_OFFSET+j+1, '*');
                turnColorPairOff(logoBoard->board[i][j].foreground);
            }
            else
            {
                turnColorPairOn(black);
                mvaddch(BOARD_X_OFFSET+i+1, BOARD_Y_OFFSET+j+1, ' ');
                turnColorPairOff(black);
            }
        }
    }
    if (logoBoard->turtle.visible)
    {
        attron(COLOR_PAIR(TURTLE_COLOR_PAIR));
        absolute = toAbsolute(logoBoard->turtle.position, logoBoard->width, logoBoard->height);
        mvaddch(BOARD_X_OFFSET+absolute.x+1, BOARD_Y_OFFSET+absolute.y+1, 'T');
        attroff(COLOR_PAIR(TURTLE_COLOR_PAIR));
    }

    printStatus(logoBoard);
    printLastCommands(logoBoard);

    refresh();

}

void printStatus (LogoBoard* logoBoard)
{
    int i;
    int j;
    char aux[20];
    char output[20];

    attron(COLOR_PAIR(BORDER_COLOR_PAIR));
    
    sprintf(output, "   STATUS   ");
    mvwprintw(stdscr, BOARD_X_OFFSET+STATUS_TITLE_OFFSET, BOARD_Y_OFFSET+logoBoard->width+3, output);

    attroff(COLOR_PAIR(BORDER_COLOR_PAIR));

    turnColorPairOn(white);

    for (j=0; j<14; j++)
    {
        mvaddch(BOARD_X_OFFSET+COORDINATE_OFFSET-1, BOARD_Y_OFFSET+logoBoard->width+2+j, ACS_HLINE);
        mvaddch(BOARD_X_OFFSET+ANGLE_OFFSET-1, BOARD_Y_OFFSET+logoBoard->width+2+j, ACS_HLINE);
        mvaddch(BOARD_X_OFFSET+PEN_OFFSET-1, BOARD_Y_OFFSET+logoBoard->width+2+j, ACS_HLINE);
        mvaddch(BOARD_X_OFFSET+VISIBILITY_OFFSET-1, BOARD_Y_OFFSET+logoBoard->width+2+j, ACS_HLINE);
        mvaddch(BOARD_X_OFFSET+COLORS_OFFSET+1, BOARD_Y_OFFSET+logoBoard->width+2+j, ACS_HLINE);
    }

    sprintf(output, "%4d degrees", logoBoard->turtle.angle);
    mvwprintw(stdscr, BOARD_X_OFFSET+ ANGLE_OFFSET, BOARD_Y_OFFSET+logoBoard->width+3, output);
    sprintf(aux, "(%d, %d)", logoBoard->turtle.position.x, logoBoard->turtle.position.y);
    sprintf(output, "%12s", aux);
    mvwprintw(stdscr, BOARD_X_OFFSET+ COORDINATE_OFFSET, BOARD_Y_OFFSET+logoBoard->width+3, output);
    
    if (logoBoard->turtle.pen == enabled)
    {   
        strcpy(aux, "Pen down");
    }
    else
    {
        strcpy(aux, "Pen up");
    }
    sprintf(output, "%12s", aux);
    mvwprintw(stdscr, BOARD_X_OFFSET+PEN_OFFSET, BOARD_Y_OFFSET+logoBoard->width+3, output);

    if (logoBoard->turtle.visible)
    {   
        strcpy(aux, "Visible");
    }
    else
    {
        strcpy(aux, "Hidden");
    }
    sprintf(output, "%12s", aux);
    mvwprintw(stdscr, BOARD_X_OFFSET+VISIBILITY_OFFSET, BOARD_Y_OFFSET+logoBoard->width+3, output);

    turnColorPairOff(WHITE_COLOR_PAIR);

    for (i=1; i< MAX_COLOR; i++)
    {
        turnColorPairOn(i);
        if (logoBoard->turtle.penColor == i)
        {
            sprintf(output, "%10d<", i);
        }
        else
        {
            sprintf(output, "%10d ", i);
        }
        mvwprintw(stdscr, BOARD_X_OFFSET+COLORS_OFFSET+(2*i), BOARD_Y_OFFSET+logoBoard->width+4, output);
        for (j=6; j<8; j++)
        {
            mvaddch(BOARD_X_OFFSET+COLORS_OFFSET+(2*i), BOARD_Y_OFFSET+logoBoard->width+4+j, ACS_CKBOARD);
        }
        turnColorPairOff(i);
    }
}

void printLastCommands (LogoBoard* logoBoard)
{
    int i;
    int j;
    int index;

    for (i = 0; i < LOGO_COMMANDS_BUFFER ; i++)
    {
        for (j=0; j<logoBoard->width; j++)
        {
            mvaddch (BOARD_X_OFFSET+logoBoard->height+COMMANDCURSOR_OFFSET-i-1, BOARD_Y_OFFSET+1+j, ' ');
        }
        index = logoBoard->commandIndex-i-1;
        if (index < 0)
        {
            index = LOGO_COMMANDS_BUFFER + index;
        }

        mvwprintw(stdscr, BOARD_X_OFFSET+logoBoard->height+COMMANDCURSOR_OFFSET-i-1, BOARD_Y_OFFSET+2, logoBoard->lastCommands[index]);
    }
}

void readInput (LogoBoard* logoBoard)
{
    int i;
    int argc;
    char** args;
    char input[91];
    char lastCommand[92];
    bool wasWriting = false;
    ParseList* list;

    move(BOARD_X_OFFSET+logoBoard->height+COMMANDCURSOR_OFFSET, BOARD_Y_OFFSET+2);
    
    for (i=0; i<logoBoard->width; i++)
    {
        mvaddch (BOARD_X_OFFSET+logoBoard->height+COMMANDCURSOR_OFFSET, BOARD_Y_OFFSET+1+i, ' ');
    }
    mvaddch (BOARD_X_OFFSET+logoBoard->height+COMMANDCURSOR_OFFSET, BOARD_Y_OFFSET+2, '>');

    move(BOARD_X_OFFSET+logoBoard->height+COMMANDCURSOR_OFFSET, BOARD_Y_OFFSET+4);
    curs_set(1);
    echo ();
    getnstr(input, logoBoard->width-4);


    strcpy(lastCommand, "> ");
    strcat(lastCommand, input);
    writeMessage(logoBoard, lastCommand);

    strcpy(lastCommand, input);

    args = parseCommand(input, &argc);
    list = argsToList(argc, args);

    wasWriting = logoBoard->file.writing;

    evaluate(list);
    
    // Prevent the beginfile/endfile being written on the file
    if (logoBoard->file.writing && wasWriting)
    {
        appendLine(lastCommand, logoBoard->file.filename);
    }

    noecho ();
    curs_set(0);
    freeArgs(args, argc);
    freeList(list);
    refresh();
}

void writeMessage (LogoBoard* logoBoard, char* message)
{
    strcpy(logoBoard->lastCommands[logoBoard->commandIndex++], message);
    logoBoard->commandIndex %= LOGO_COMMANDS_BUFFER;
}

void executeFd (LogoBoard* logoBoard, int steps)
{
    Line* line;
    Coordinate destination;
 
    destination = calculateDestination(logoBoard->turtle.position, logoBoard->turtle.angle, steps);
    line = calculateLine(logoBoard->turtle.position, destination);
    drawLine(line, logoBoard);
    freeLine(line);
    logoBoard->turtle.position = destination;
    validateTurtle(logoBoard);
}

void executeBk (LogoBoard* logoBoard, int steps)
{
    Line* line;
    Coordinate destination;

    destination = calculateDestination(logoBoard->turtle.position, logoBoard->turtle.angle+180, steps);
    line = calculateLine(logoBoard->turtle.position, destination);
    drawLine(line, logoBoard);
    freeLine(line);
    logoBoard->turtle.position = destination;
    validateTurtle(logoBoard);    
}

void executeRt (LogoBoard* logoBoard, int angle)
{
    logoBoard->turtle.angle -= angle;
    logoBoard->turtle.angle %= 360;

    if (logoBoard->turtle.angle < 0)
    {
        logoBoard->turtle.angle += 360;
    }
}

void executeLt (LogoBoard* logoBoard, int angle)
{
    logoBoard->turtle.angle += angle;
    logoBoard->turtle.angle %= 360;

    if (logoBoard->turtle.angle < 0)
    {
        logoBoard->turtle.angle += 360;
    }
}

void executePu (LogoBoard* logoBoard)
{
    logoBoard->turtle.pen = disabled;
}

void executePd (LogoBoard* logoBoard)
{
    logoBoard->turtle.pen = enabled;
}

void executeSetxy (LogoBoard* logoBoard, int x, int y)
{
    Line* line;
    Coordinate origin;
    Coordinate destination;

    origin = logoBoard->turtle.position;
        
    logoBoard->turtle.position.x = x;
    logoBoard->turtle.position.y = y;
    validateTurtle(logoBoard);

    destination = logoBoard->turtle.position;

    line = calculateLine(origin, destination);
    drawLine(line, logoBoard);
    freeLine(line);
}

void executeHome (LogoBoard* logoBoard)
{
    Line* line;
    Coordinate origin;
    Coordinate destination;

    origin = logoBoard->turtle.position;
    logoBoard->turtle.position.x = 0;
    logoBoard->turtle.position.y = 0;
    validateTurtle(logoBoard);
    destination = logoBoard->turtle.position;

    line = calculateLine(origin, destination);
    drawLine(line, logoBoard);
    freeLine(line);
    logoBoard->turtle.angle = 90;   
}

void executeClean (LogoBoard* logoBoard)
{
    cleanBoard(logoBoard);
}

void executeSt (LogoBoard* logoBoard)
{
    logoBoard->turtle.visible = true;
}

void executeHt (LogoBoard* logoBoard)
{
    logoBoard->turtle.visible = false;
}

void executeSetpencolor (LogoBoard* logoBoard, int color)
{
    logoBoard->turtle.penColor = color % MAX_COLOR;
}

void executePrint (LogoBoard* logoBoard, char* string)
{
    writeMessage(logoBoard, string);
}

void executeBeginfile (LogoBoard* logoBoard, char* filename)
{
    FILE* file;

    strcpy(logoBoard->file.filename, "assets/");
    strcat(logoBoard->file.filename, filename);

    file = fopen(logoBoard->file.filename, "w"); // Create an empty file

    if (file != NULL)
    {
        logoBoard->file.writing = true;
        fclose(file);
        logoBoard->turtle.penColor = white;
        executeHome(logoBoard);
        executeClean(logoBoard);
    }
    else
    {
        writeMessage(logoBoard, ERROR_FILE);
    }
}

void executeEndfile (LogoBoard* logoBoard)
{
    logoBoard->file.writing = false;
}

void executeLoadfile (LogoBoard* logoBoard, char* filename)
{
    int argc;
    char** args;
    char input[100];
    ParseList* list;
    FILE* file;

    strcpy(logoBoard->file.filename, "assets/");
    strcat(logoBoard->file.filename, filename);

    file = fopen(logoBoard->file.filename, "r"); // Create an empty file

    if (file != NULL)
    {
        executeHome(logoBoard);
        executeClean(logoBoard);

        while (fgets(input, 99, file) != NULL)
        {
            input[strlen(input)-1] = '\0';
            if (input != NULL)
            {
                args = parseCommand(input, &argc);
                list = argsToList(argc, args);

                evaluate(list);

                freeArgs(args, argc);
                freeList(list);
            }

        }
    }
    else
    {
        writeMessage(logoBoard, ERROR_FILE);
    }

    refresh();
}

void executeExit (LogoBoard* logoBoard)
{
    logoBoard->running = false;
}

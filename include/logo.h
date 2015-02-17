/* 
* @Author: Erick Lucena Palmeira Silva
* @Date:   2015-01-23 14:32:31
* @Last Modified by:   Erick Lucena Palmeira Silva
* @Last Modified time: 2015-02-13 19:56:55
*/

#ifndef _ELUCENAP_LOGOH
#define _ELUCENAP_LOGOH

#include <stdbool.h>

#define M_PI 3.14159

#define LOGO_COMMANDS_BUFFER 4
#define LOGO_COMMANDS_MAX_SIZE 101

#define FILENAME_MAX_SIZE 101

#define BOARD_HEIGHT 31
#define BOARD_WIDTH 81

#define BOARD_X_OFFSET 0
#define BOARD_Y_OFFSET 0

#define STATUS_WIDTH 15
#define STATUS_HEIGTH 15

#define COMMANDBOX_HEIGHT 8
#define COMMANDCURSOR_OFFSET 7

#define STATUS_TITLE_OFFSET 0
#define COORDINATE_OFFSET 2
#define ANGLE_OFFSET 4
#define PEN_OFFSET 6
#define VISIBILITY_OFFSET 8
#define COLORS_OFFSET 8

#define WHITE_COLOR_PAIR 1
#define RED_COLOR_PAIR 2
#define GREEN_COLOR_PAIR 3
#define BLUE_COLOR_PAIR 4
#define YELLOW_COLOR_PAIR 5
#define MAGENTA_COLOR_PAIR 6
#define CYAN_COLOR_PAIR 7
#define BLACK_COLOR_PAIR 8

#define BORDER_COLOR_PAIR 9
#define TURTLE_COLOR_PAIR 10

#define MAX_COLOR 8

#define ERROR_FILE "can't create a new file"

typedef enum color
{
    undefined,
    white,
    red,
    green,
    blue,
    yellow,
    magenta,
    cyan,
    black
} Color;

typedef enum state
{
    blank,
    written

} State;

typedef enum pen
{
    disabled,
    enabled
} Pen;

typedef struct square 
{
    State state;
    Color background;
    Color foreground;
} Square;

typedef struct coordinate
{
    int x;
    int y;
} Coordinate;

typedef struct line
{
    int length;
    Coordinate* position;
} Line;

typedef struct turtle
{
    Coordinate position;
    bool visible;
    int angle;
    Pen pen;
    Color penColor;
} Turtle;

typedef struct logoFile
{
    bool writing;
    char filename[FILENAME_MAX_SIZE];
} LogoFile;

typedef struct logoBoard
{
    bool running;
    int commandIndex;
    int width;
    int height;
    Square** board;
    Turtle turtle;
    LogoFile file;
    char lastCommands[LOGO_COMMANDS_BUFFER][LOGO_COMMANDS_MAX_SIZE];
} LogoBoard;


/**
 NewLine: Allocates memory to a new line structure with the $length passed as parameter 
 Pre: Pass the legth of the line to be created
 Post: Returns a pointer for a line
*/
Line* newLine (int length);

/**
 FreeLine: Free the memory occupied by $line 
 Pre: Pass the line to be freed
 Post: The memory is occupied by line is free
*/
void freeLine (Line* line);


/**
 NewBoard: Allocates memoru to a new LogoBoard structure, with the dimensions $x and $y passed as parameters
 Pre: Pass the x and y parameters
 Post: Returns a pointer for a board
*/
LogoBoard* newBoard (int x, int y);

/**
 InitBoard: Sets the values of the $logoBoard to its initials values
 Pre: Pass the logoBoard as parameter
 Post: The values of logoBoard are set
*/
void initBoard (LogoBoard* logoBoard);

/**
 FreeBoard: Free the memory occupied by $logoboard  
 Pre: Pass the logoBoard to be freed
 Post: The memory occupied by logoBoard is free
*/
void freeBoard (LogoBoard* logoBoard);

/**
 GetLogoBoard: Returns a singleton instance of a LogoBoard 
 Pre: NA
 Post: Returns a pointer to a LogoBoard
*/
LogoBoard* getLogoBoard ();


/**
 ToAbsolute: Convert the cartesian $position values for its equivalent in ncurses on a $x X $y board size
 Pre: Pass the coordinate and ncurses board dimension
 Post: Returns a new coordinate with the converted values
*/
Coordinate toAbsolute (Coordinate position, int width, int height);

/**
 ToCartesian: Convert the absolute $position from a ncurses $x X $y board to a cartesian equivalent
 Pre: Pass the coordinate and ncurses board dimension
 Post: Returns a new coordinate with the converted values
*/
Coordinate toCartesian (Coordinate position, int width, int height);

/**
 CalculateDestination: Calculates the end point of a movement, based on the origin $position, $angle and $distance 
 Pre: Pass the parameters
 Post: Returns the coordinate of the end point
*/
Coordinate calculateDestination (Coordinate position, int angle, int distance);

/**
 CalculateLine: Calculate the line formed by a movement from $origin to $destination 
 Pre: Pass the parameters
 Post: Returns a pointer to a line with the calculated points
*/
Line* calculateLine (Coordinate origin, Coordinate destination);

/**
 InitColorsPairs: Creates the logo colors using the ncurses library
 Pre: NA
 Post: The logo colors are initialized
*/
void initColorsPairs ();

/**
 turnColorPairOn: Activates the $color received as parameter
 Pre: Pass the color
 Post: The programs starts to write on the screen with the selected color
*/
void turnColorPairOn (Color color);

/**
 turnColorPairOff: Deactivates the $color received as parameter
 Pre: Pass the color
 Post: The program stops writing on the screen with the selected color
*/
void turnColorPairOff (Color color);

/**
 CleanBoard: Clean the entire $logoboard passed as parameter 
 Pre: Pass the parameter
 Post: The board passed as parameter is clean
*/
void cleanBoard (LogoBoard* logoBoard);

/**
 ValidateTurtle: Validate/corrects the turtle of a $logoBoard
 Pre: Pass the parameter
 Post: The properties of the turtle are validated/corrected
*/
void validateTurtle (LogoBoard* logoBoard);

/**
 DrawLine: Draw a $line on the $logoboard 
 Pre: Pass the line and the logoBoard
 Post: The line is written on the board
*/
void drawLine (Line* line, LogoBoard* logoBoard);

/**
 PrintBoard: Prints the $logoBoard on the screen
 Pre: Pass the parameter
 Post: The board is printed on the screen
*/
void printBoard (LogoBoard* logoBoard);

/**
 PrintStatus: Prints the status of the $logoBoard on the screen 
 Pre: Pass the parameter
 Post: The status box and information are printed on the screen
*/
void printStatus (LogoBoard* logoBoard);

/**
 PrintLastCommands: Prints the last commands typed (which are stored on $logoBoard) on the command box
 Pre: Pass the logoBoard as parameter
 Post: The commands are printed on the screen
*/
void printLastCommands (LogoBoard* logoBoard);

/**
 ReadInput: Cleans the command line of $logoBoard and waits for the user's input, then calls the execution for that input
 Pre: Pass the parameter
 Post: The command is read and executed and the input is added to the last commands
*/
void readInput (LogoBoard* logoBoard);

/**
 WriteMessage: Write a $message on the command box of a $logoBoard
 Pre: Pass the parameters
 Post: The message is written on the command box
*/
void writeMessage (LogoBoard* logoBoard, char* message);


/**
 ExecuteFd: Moves the turtle foward on the $logoboard by the distance defined by $steps
 Pre: Pass the parameters
 Post: The turtle moves foward on the board
*/
void executeFd (LogoBoard* logoBoard, int steps);

/**
 ExecuteBk: Moves the turtle backwards on the $logoBoard by the distance defined by $steps 
 Pre: Pass the parameters
 Post: The turtle moves backwards on the board
*/
void executeBk (LogoBoard* logoBoard, int steps);

/**
 ExecuteRt: Rotates the turtle clockwise by $angle degrees on the $logoboard 
 Pre: Pass the parameters
 Post: The turtle is rotated
*/
void executeRt (LogoBoard* logoBoard, int angle);

/**
 ExecuteLt: Rotates the turtle couter-clockwise by $angle degrees on the $logoboard 
 Pre: Pass the parameters
 Post: The turtle is rotated
*/
void executeLt (LogoBoard* logoBoard, int angle);

/**
 ExecutePu: Deactivates the turtle's writing ability on the $logoboard
 Pre: Pass the parameter
 Post: The turtle don't writes on the board while moving
*/
void executePu (LogoBoard* logoBoard);

/**
 ExecutePd: Activates the turtle' writing ability on the $logoboard 
 Pre: Pass the parameter
 Post: The turtle writes on the board while moving
*/
void executePd (LogoBoard* logoBoard);

/**
 ExecuteSetxy: Moves the turtle on the $logoBoard to the ($x, $y) coordinate
 Pre: Pass the parameters
 Post: The turtle moves to the position
*/
void executeSetxy (LogoBoard* logoBoard, int x, int y);

/**
 ExecuteHome: Moves the turtle to the center of the board (0, 0) 
 Pre: Pass the parameter
 Post: The turtle is positioned at the center of the board
*/
void executeHome (LogoBoard* logoBoard);

/**
 ExecuteClean: Cleans the $logoBoard
 Pre: Pass the parameter
 Post: The board is clean
*/
void executeClean (LogoBoard* logoBoard);

/**
 ExecuteSt: If the turtle is hidden, makes it visible on the $logoBoard again
 Pre: Pass the parameter
 Post: The turtle is visible on the board
*/
void executeSt (LogoBoard* logoBoard);

/**
 ExecuteHt: If the turtle is visible on the $logoBoard, makes it hidden
 Pre: Pass the parameter
 Post: The turtle is no visible on the board
*/
void executeHt (LogoBoard* logoBoard);

/**
 ExecuteSetpencolor: Changes the turtle's pen $color on $logoBoard 
 Pre: Pass the parameters
 Post: The turtle starts to write with the selected color
*/
void executeSetpencolor (LogoBoard* logoBoard, int color);

/**
 ExecutePrint: Prints the $string on the $logoboard command box
 Pre: Pass the parameters
 Post: The string is written on the command box
*/
void executePrint (LogoBoard* logoBoard, char* string);

/**
 ExecuteBeginfile: Creates a new file with the selected $filename, and starts to write the typed commands on $logoBoard
 Pre: Pass the parameters
 Post: File is created and the programming stores the typed inputs until 'endfile' is called
*/
void executeBeginfile(LogoBoard* logoBoard, char* filename);

/**
 ExecuteEndfile: Stops writing the commands on the last file created (if any)
 Pre: Pass the parameters
 Post: Stop the input storing
*/
void executeEndfile (LogoBoard* logoBoard);

/**
 ExecuteLoadfile: Load a file with the selected $filename, and starts executes its contents
 Pre: Pass the parameters
 Post: File is loaded and the commands whitin it are executed
*/
void executeLoadfile (LogoBoard* logoBoard, char* filename);

/**
 ExecuteExit: Finishes the program execution
 Pre: Pass the parameter
 Post: The program is closed
*/
void executeExit (LogoBoard* logoBoard);

#endif  // _ELUCENAP_LOGO_H

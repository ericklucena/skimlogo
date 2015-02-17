# Skim-logo

## About
This program is a skim version of the Logo Programming Language, coded as assignment for the Intermediate Programming course at the University of Guelph.

## Compiling
To compile this program type "make". The binary executable will be created on the 'bin' folder.

## Running the program
To run the program type "./bin/runMe"

## How to use

To use this program, you need to type your inputs in the command line.
Each command and its parameters must be separated by a space. You can type more than one command per line, but the line length is limited.

### DATA TYPES


You can use 3 types of data. Any of these type can be stored in a variable (See the make command)

Int
----
Any parameter formed only by numbers.

	Examples:
	1
	-123
	9829

Boolean
-----------
Parameters true and false

	Examples:
	true
	false

Float
-------
Any parmeter formed only by numbers and a single dot.

	Examples:
	-1.0
	234.56
	3.14159

String
--------
Any parameter begining with a double quote.

	Examples:
	"CIS2500
	"LogoIsFun
	"ThatsWhatSheSaid


### Commands

Available commands.

####  Command: make
#####  Description: 
Creates a variable with the name and value passed as parameters

Usage: 
	
	make [ string ] [ int | float | string | bool ]

#### Command: + 
###### Description:
Plus arithmetic operation.

Usage: 
	
	[ int | float ] + [ int | float]

#### Command: -
##### Description:
Minus arithmetic operation.

Usage:

    [ int | float ] - [ int | float]

#### Command: * 
##### Description:
Times arithmetic operation.

Usage:

    [ int | float ] + [ int | float]

#### Command: / 
##### Description:
Divide arithmetic operation.

Usage:

    [ int | float ] / [ int | float]

#### Command: and
##### Description:
and boolean operation

Usage:

    and [ bool ] [ bool ]

#### Command: or
##### Description:
or boolean operation

Usage:

    or [ bool ] [ bool ]

#### Command: < 
##### Description:
Lesser than relational operation.

Usage:

    [ int | float ] < [ int | float]

#### Command: <=
##### Description:
Less than or equal relational operation.

Usage:

    [ int | float ] <= [ int | float]

#### Command: >
##### Description:
Greater than relational operation.

Usage:

    [ int | float ] > [ int | float]

#### Command: >= 
##### Description:
Greater than or equal relational operation.

Usage:

    [ int | float ] >= [ int | float]

#### Command: = 
##### Description:
Equal relational operation.

Usage:

    [ int | float ] = [ int | float]

#### Command: <> 
##### Description:
Not equal relational operation.

Usage:

    [ int | float ] <> [ int | float]

#### Command: fd
##### Description:
Moves the turtle foward

Usage:

    fd [ int | float ]

#### Command: bk
##### Description:
Moves the turtle backwards

Usage:

    bk [ int | float ]

#### Command: rt
##### Description:
Rotate the turtle clockwise by the angle passed as parameter

Usage:

    rt [ int | float ]

#### Command: lt
##### Description:
Rotate the turtle counter clockwise by the angle passed as parameter

Usage:

    lt [ int | float ]

#### Command: pu
##### Description:
Disable the writing ability of the turtle

Usage:

    pu

#### Command: pd
##### Description:
Enable the writing ability of the turtle

Usage:

    pu

#### Command: setxy
##### Description:
Moves the turtle to the coordinate passed as parameter

Usage:

    setxy [ int | float ] [ int | float ]

#### Command: home
##### Description:
Moves the turtle to the starting point of the board. Also resets its angle to 90 degrees

Usage:

    home

#### Command: clean
##### Description:
Cleans the entire board

Usage:

    clean

#### Command: st
##### Description:
If the turtle is hidden, makes it visible again

Usage:

    st

#### Command: ht
##### Description:
If the turtle is visible, makes it hidden

Usage:

    ht

#### Command: setpencolor
##### Description:
Changes the color of the turtle's pen

Usage:

    setpencolor [ int ]

#### Command: beginfile
##### Description:
Creates a new file (name passed as parameter) and starts writing the user's commands in it

Usage:

    beginfile [ string ]

#### Command: endfile
##### Description:
Stops the writing of a file

Usage:

    endfile

#### Command: loadfile
##### Description:
Load a file previously saved, and execute its commands

Usage:

    loadfile [ string ]

#### Command: exit
##### Description:
Closes the program

Usage:
	
	exit


### Restrictions

- The turtle can't go out of the screen. If the user make an input that finishes outside the board, the turtle will stop at the border
- The max input is 90 characters (But there's no limit on the number of commands per line)



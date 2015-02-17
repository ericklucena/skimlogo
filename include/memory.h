/* 
* @Author: Erick Lucena Palmeira Silva
* @Date:   2015-02-09 10:01:24
* @Last Modified by:   Erick Lucena Palmeira Silva
* @Last Modified time: 2015-02-13 19:57:07
*/

#ifndef _ELUCENAP_MEMORYH
#define _ELUCENAP_MEMORYH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef union varValue
{
    int intValue;
    bool boolValue;
    float floatValue;
    char* stringValue;
} VarValue;

typedef enum type {
    t_int,
    t_float,
    t_bool,
    t_string
} Type;

typedef struct variable {
    Type type;
    char* key;
    VarValue value;
} Variable;

typedef struct memory
{
    int size;
    Variable** variables;
} Memory;

/**
 NewVariable: Allocates memory for a new Variable structure using the $key and $value as initial values
 Pre: Pass the parameters
 Post: Returns a pointer to the new Variable
*/
Variable* newVariable (char* key, char* value);

/**
 NewMemory: Allocates memory for a new Memory structure
 Pre: NA
 Post: Returns a pointer to the new memory
*/
Memory* newMemory ();

/**
 GetMemory: Returns a singleton instance of a Memory structure
 Pre: NA
 Post: Returns a pointer to a Memory
*/
Memory* getMemory ();

/**
 ExpandMemory: Expands the size of $memory by one
 Pre: Pass the parameter
 Post: Returns true if successful, otherwise returns false
*/
bool expandMemory (Memory* memory);

/**
 ShrinkMemory: Retracts the size of $memory by one
 Pre: Pass the parameter
 Post: Returns true if successful, otherwise returns false
*/
bool shrinkMemory (Memory* memory);

/**
 SearchKeyIndex: Iterates over the $memory searching for a variable with the specified $key 
 Pre: Pass the parameters
 Post: Returns a valid index if successful, otherwise returns -1
*/
int searchKeyIndex (Memory* memory, char* key);

/**
 SetValue: Sets the value of the Variable at $index on the $memory as $var
 Pre: Pass the parameters
 Post: The variable value is set
*/
bool setValue (Memory* memory, int index, Variable* var);

/**
 GetVar: Gets the variable with the specified $key on the $memory
 Pre: Pass the parameters
 Post: Returns a pointer to a Variable if successful, otherwise returns NULL
*/
Variable* getVar(Memory* memory, char* key);

/**
 MakeVariable: Stores the $var on the $memory
 Pre: Pass the parameters
 Post: Returns true if successful, otherwise returns false
*/
bool makeVariable (Memory* memory, Variable* var);

/**
 PrintVar: Prints the contents of a $variable on the screen.
 Pre: Pass the parameter
 Post: The contents of the variable are printed on the screen
*/
void printVar (Variable* variable);

/**
 PrintMemory: Prints the contents of the $memory on the screen
 Pre: Pass the parameter
 Post: The contents of the memory are printed on the screen
*/
void printMemory (Memory* memory);

#endif // _ELUCENAP_MEMORYH

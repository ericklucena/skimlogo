/* 
* Skim-logo Util functions implementation
* @Author: Erick Lucena Palmeira Silva
* @Date:   2015-01-26 01:42:25
* @Last Modified by:   Erick Lucena Palmeira Silva
* @Last Modified time: 2015-02-17 01:14:35
* This file contains the implementation of the functions used by the libraries
*/


#include "util.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

bool isEqual (char* stringA, char* stringB)
{
    return strcmp(stringA, stringB) == 0;
}

bool isLogoString (char* string)
{
    return string[0] == '\"';

}

bool isBoolean (char* string)
{
    return isEqual(string, "true") || isEqual(string, "false");

}

bool isInteger (char* string)
{
    int i;
    int length;
    int hasNumber = false;

    length = strlen(string);

    // If the first char is a dash, skip it (negative number)
    for (i=(string[0]=='-'?1:0); i < length; i++)
    {
        //If not a number
        if (!(string[i]>='0' && string[i]<='9'))
        {
            return false;
        }
        else
        {
            hasNumber = true;
        }
    }

    return hasNumber;
}

bool isFloat (char* string)
{
    int i;
    int length;
    bool floatNumber = false;
    bool hasNumber = false;

    length = strlen(string);

    // If the first char is a dash, skip it (negative number)
    for (i=(string[0]=='-'?1:0); i < length; i++)
    {
        if (string[i] == '.')
        {
            floatNumber = true;
        }

        //If not a number or a dot
        if (!((string[i] >= '0' && string[i] <= '9') || (string[i])))
        {
            return false;
        }
        else
        {
            hasNumber = true;
        }

    }

    return floatNumber && hasNumber;
}

char* newString (int lenght)
{
    char *string;

    string = malloc (sizeof(char)*lenght+1);
    if (string != NULL)
    {
        string[0]= '\0';
    }

    return string;
}

char* copyString (char* string)
{
    char* newStr;

    newStr = newString(strlen(string));

    if (newStr != NULL)
    {
        strcpy(newStr, string);
    }

    return newStr;
}


char** newArgs (int argc)
{
    char ** args;

    args = calloc (sizeof(char*), argc);

    return args;
}

char** expandArgs (char** args, int argc)
{
    char** newArgs;

    newArgs = realloc (args, sizeof(char*)*argc);

    return newArgs;
}

void freeArgs (char **args, int argc)
{
    int i;

    for (i=0; i<argc; i++)
    {
        free(args[i]);
    }

    free(args);
}

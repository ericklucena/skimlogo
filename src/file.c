/* 
* Skim-logo File functions implementation
* @Author: Erick Lucena Palmeira Silva
* @Date:   2015-02-08 23:37:45
* @Last Modified by:   Erick Lucena Palmeira Silva
* @Last Modified time: 2015-02-17 00:29:02
* This file contains the implementation of the functions responsible for working on the file persistance
*/

#include "file.h"
#include <stdio.h>

void appendLine (char* line, char* filename)
{
    FILE* file;

    file = fopen(filename, "a");
    fputs(line, file);
    fputs("\n", file);
    fclose(file);
}

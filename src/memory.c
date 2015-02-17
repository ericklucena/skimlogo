/* 
* Skim-logo Memory functions implementation
* @Author: Erick Lucena Palmeira Silva
* @Date:   2015-02-09 10:03:27
* @Last Modified by:   Erick Lucena Palmeira Silva
* @Last Modified time: 2015-02-17 00:29:37
* This file contains the implementation of the functions used for storing/loading variables
*/

#include "memory.h"
#include "util.h"

Variable* newVariable (char* key, char* value)
{
    int intNum;
    float floatNum;
    bool error = false;
    Variable* variable;

    variable = malloc (sizeof(Variable));

    if (variable != NULL)
    {
        variable->key = copyString(key);
        if (variable->key != NULL)
        {
            if (isLogoString(value))
            {
                variable->type = t_string;
                variable->value.stringValue = copyString(value);
                if (variable->value.stringValue == NULL)
                {
                    error = true;
                    free(variable->key);
                }   
            }
            else if (isFloat(value))
            {
                variable->type = t_float;
                sscanf(value, "%f", &floatNum);
                variable->value.floatValue = floatNum;
            }
            else if (isInteger(value))
            {
                variable->type = t_int;
                sscanf(value, "%d", &intNum);
                variable->value.floatValue = floatNum;  
            }
            else
            {
                error = true;
            }
        }
        else
        {
            error = true;
        }
    }

    if (error)
    {
        free(variable);
        variable = NULL;
    }

    return variable;
}

Memory* newMemory ()
{
    Memory* memory = malloc (sizeof(Memory));
    memory->size = 0;
    memory->variables = NULL;

    return memory;
}

Memory* getMemory ()
{
    static Memory* memory = NULL;

    if (memory == NULL)
    {
        memory = newMemory();
    }

    return memory;
}

bool expandMemory (Memory* memory)
{
    int newSize;
    Variable** newVariables;
    bool expanded = false;

    newSize = memory->size+1;

    newVariables = realloc(memory->variables, sizeof(Variable*)*newSize);

    if (newVariables != NULL)
    {
        memory->size = newSize;
        memory->variables = newVariables;
        expanded = true;
    }

    return expanded;
}

bool shrinkMemory (Memory* memory)
{
    int newSize;
    Variable** newVariables;
    bool expanded = false;

    newSize = memory->size-1;

    newVariables = realloc(memory->variables, sizeof(Variable*)*newSize);

    if (newVariables != NULL)
    {
        memory->size = newSize;
        memory->variables = newVariables;
        expanded = true;
    }

    return expanded;
}

int searchKeyIndex (Memory* memory, char* key)
{
    int i;

    for (i=0; i < memory->size; i++)
    {
        if (strcmp(memory->variables[i]->key, key) == 0)
        {
            break;
        }
    }

    // If the key is not in the memory
    if (i >= memory->size)
    {
        i = -1;
    }

    return i;
}

bool setValue (Memory* memory, int index, Variable* var)
{
    bool set = true;

    memory->variables[index] = var;

    return set;
}

Variable* getVar (Memory* memory, char* key)
{
    int keyIndex;

    keyIndex = searchKeyIndex(memory, key);

    if (keyIndex >= 0)
    {
        return memory->variables[keyIndex];
    }
    else
    {
        return NULL;
    }
}

bool makeVariable (Memory* memory, Variable* var)
{
    int index;
    bool noMemory = false;

    index = searchKeyIndex(memory, var->key);

    if (index >= 0)
    {
        setValue(memory, index, var);
    }
    else
    {
        if (expandMemory(memory))
        {
            memory->variables[memory->size-1] = var;
        }
        else
        {
            noMemory = true;
        }
    }

    return !noMemory;
}

void printVar (Variable* variable)
{
    switch (variable->type)
    {
    case t_int:
        printf("| %20s | %20d | int\n", variable->key, variable->value.intValue);
        break;
    case t_bool:
        printf("| %20s | %20d | bool\n", variable->key, variable->value.boolValue);
        break;
    case t_float:
        printf("| %20s | %20f | float\n", variable->key, variable->value.floatValue);
        break;
    case t_string:
        printf("| %20s | %20s | string\n", variable->key, variable->value.stringValue);
        break;
    }
}

void printMemory (Memory* memory)
{
    int i;

    for (i=0;i<memory->size;i++)
    {
        printf("%3d ", i);
        printVar(memory->variables[i]);
    }
    printf("\n\n");
}

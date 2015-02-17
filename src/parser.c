/* 
* Skim-logo Parser functions implementation
* @Author: Erick Lucena Palmeira Silva
* @Date:   2015-02-10 02:01:31
* @Last Modified by:   Erick Lucena Palmeira Silva
* @Last Modified time: 2015-02-17 01:14:26
* This file contains the implementation of the functions used for parsing the user's inputs
*/

#include "parser.h"
#include "util.h"
#include "logo.h"
#include <stdio.h>
#include <string.h>


ParseNode* newParseNode (ParseVal info, ParseNode* previous, ParseNode* next)
{
    ParseNode *node;

    node = malloc (sizeof(ParseNode));

    if (node != NULL)
    {
        node->info = info;
        node->next = next;
        node->previous = previous;
    }

    return node;
}

void setNext (ParseNode* node, ParseNode *next)
{
    node->next = next;
}

void setPrevious (ParseNode* node, ParseNode *previous)
{
    node->previous = previous;
}

void cleanNode (ParseNode *node)
{
    if (node->info.type == pt_error || node->info.type == pt_string || node->info.type == pt_logo_operator || node->info.type == pt_arithmetic_operator)
    {
        if (node->info.value.stringValue != NULL)
        {
            free(node->info.value.stringValue);
        }
    }

    node->info.type = pt_empty;
}

void freeNode (ParseNode *node)
{
    if (node != NULL)
    {
        if (node->next != NULL)
        {
            freeNode(node->next);
        }

        if (node->info.type == pt_error || node->info.type == pt_string)
        {
            if (node->info.value.stringValue != NULL)
            {
                free(node->info.value.stringValue);
            }
        }

        free(node);
    }
}

ParseList* newParseList ()
{
    ParseList* list;

    list = malloc (sizeof(ParseList));

    if (list != NULL)
    {
        list->head = NULL;
        list->tail = NULL;
    }

    return list;
}

void freeList (ParseList* list)
{
    if (list != NULL)
    {
        freeNode(list->head);
        free(list);
    }

}

void printList (ParseList* list)
{
    ParseNode* iterator;

    iterator = list->head;

    printf("NULL\n");
    while (iterator != NULL)
    {
        printParseVal(&iterator->info);
        iterator = iterator->next;
    }
    printf("NULL\n\n");
}

void insertAtTail (ParseList* list, ParseNode* node)
{
    if (list->tail == NULL) //Empty list
    {
        list->tail = node;
        list->head = node;
        node->previous = NULL;
        node->next = NULL;
    }
    else
    {
        list->tail->next = node;
        node->previous = list->tail;
        node->next = NULL;
        list->tail = node;
    }
}

void removePrevious (ParseList* list, ParseNode* node)
{
    ParseNode* newPrevious;

    if (node->previous != NULL)
    {
        newPrevious = node->previous->previous;
        free(node->previous);
        node->previous = newPrevious;

        // The removed node was the head of the list
        if (node->previous == NULL)
        {
            list->head = node;
        }
        else
        {
            newPrevious->next = node;
        }
    }
}

void removeNext (ParseList* list, ParseNode* node)
{
    ParseNode* newNext;

    if (node->next != NULL)
    {
        newNext = node->next->next;
        free(node->next);
        node->next = newNext;

        // The removed node was the head of the list
        if (node->next == NULL)
        {
            list->tail = node;
        }
        else
        {
            newNext->previous = node;
        }
    }
}

ParseVal toParseVal (Variable* var)
{
    ParseVal parseVal;

    switch (var->type)
    {
    case t_int:
        parseVal.type = pt_int;
        break;
    case t_bool:
        parseVal.type = pt_bool;
        break;
    case t_float:
        parseVal.type = pt_float;
        break;
    case t_string:
        parseVal.type = pt_string;
        break;
    }
    parseVal.value = var->value;

    return parseVal;
}

Variable* toVariable (char* key, ParseVal parseVal)
{
    Variable* var;

    var = malloc(sizeof(Variable));

    if (var != NULL)
    {
        switch (parseVal.type)
        {
        case pt_int:
            var->type = t_int;
            break;
        case pt_bool:
            var->type = t_bool;
            break;
        case pt_float:
            var->type = t_float;
            break;
        case pt_string:
            var->type = t_string;
            break;
        default:
            break;
        }

        var->key = copyString(key);
        var->value = parseVal.value;
    }

    return var;
}

ParseVal errorParseVal (char* errorString)
{
    ParseVal error;

    error.type = pt_error;
    error.value.stringValue = copyString(errorString);

    if (error.value.stringValue == NULL)
    {
        // Not enough memory to send an error. Error signal will be emitted empty 
    }

    return error;
}

ParseVal emptyParseVal ()
{
    ParseVal empty;

    empty.type = pt_empty;
    empty.value.stringValue = NULL;

    return empty;
}

void printParseVal (ParseVal* variable)
{
    switch (variable->type)
    {
    case pt_int:
        printf("| int      | %20d |\n", variable->value.intValue);
        break;
    case pt_bool:
        printf("| bool     | %20d |\n", variable->value.boolValue);
        break;
    case pt_float:
        printf("| float    | %20f |\n", variable->value.floatValue);
        break;
    case pt_string:
        printf("| string   | %20s |\n", variable->value.stringValue);
        break;
    case pt_relational_operator:
    case pt_logo_operator:
    case pt_arithmetic_operator:
        printf("| operator | %20s |\n", variable->value.stringValue);
        break;
    case pt_variable:
        printf("| variable | %20s |\n", variable->value.stringValue);
        break;
    case pt_error:
        printf("| error    | %20s |\n", variable->value.stringValue);
        break;
    case pt_empty:
        printf("| empty    |\n");
        break;
    default:
        printf("Error signal\n");
        break;
    }
}

char** parseCommand (char* command, int *argc)
{
    int i;
    char *aux;
    char *arg;
    char **args;

    args = newArgs(1);
    aux = strtok(command, " ");
    
    for (i=0; aux != NULL;)
    {
        arg = copyString(aux);
        args[i++] = arg;
        args = expandArgs(args, i+1);
        aux = strtok(NULL, " ");
    }
    
    *argc = i;

    return args;
}

ParseList* argsToList (int argc, char** argv)
{
    int i;
    ParseVal parseVal;
    ParseList* list;
    ParseNode* node;

    list = newParseList();

    if (list != NULL)
    {
        for (i=0; i<argc; i++)
        {
            if (isLogoString(argv[i]))
            {
                parseVal.type = pt_string;
                parseVal.value.stringValue = copyString(argv[i]+1); //Discard the '\"' character
            }
            else if (isBoolean(argv[i]))
            {
                parseVal.type = pt_bool;
                if (isEqual(argv[i], LOGO_TRUE))
                {
                    parseVal.value.boolValue = true;   
                }
                else if (isEqual(argv[i], LOGO_FALSE))
                {
                    parseVal.value.boolValue = false;
                }
            }
            else if (isInteger(argv[i]))
            {
                parseVal.type = pt_int;
                sscanf(argv[i], "%d", &(parseVal.value.intValue));
            }
            else if (isFloat(argv[i]))
            {
                parseVal.type = pt_float;
                sscanf(argv[i], "%f", &(parseVal.value.floatValue));
            }
            else if (isArithmeticOperator(argv[i]))
            {
                parseVal.type = pt_arithmetic_operator;
                parseVal.value.stringValue = copyString(argv[i]);
            }
            else if (isRelationalOperator(argv[i]))
            {
                parseVal.type = pt_relational_operator;
                parseVal.value.stringValue = copyString(argv[i]);
            }
            else if (isLogoOperator(argv[i]))
            {
                parseVal.type = pt_logo_operator;
                parseVal.value.stringValue = copyString(argv[i]);
            }
            else
            {
                parseVal.type = pt_variable;
                parseVal.value.stringValue = copyString(argv[i]);
            }

            node = newParseNode(parseVal, NULL, NULL);

            if (node == NULL)
            {
                freeList(list);
                return NULL;
            }

            insertAtTail(list, node);
        }
    }

    return list;
}

bool isOperator (char* string)
{
    if (isArithmeticOperator(string) || isLogoOperator(string))
    {
        return true;
    }

    return false;
}

bool isRelationalOperator (char* string)
{
    if (isEqual(string, OPER_LESS) || isEqual(string, OPER_LESS_EQUAL) || isEqual(string, OPER_GREAT) || isEqual(string, OPER_GREAT_EQUAL) || isEqual(string, OPER_EQUAL) || isEqual(string, OPER_NOT_EQUAL))
    {
        return true;
    }

    return false;   
}

bool isArithmeticOperator (char* string)
{
    if (isEqual(string, OPER_PLUS) || isEqual(string, OPER_MINUS) || isEqual(string, OPER_TIMES) || isEqual(string, OPER_DIVIDE))
    {
        return true;
    }

    return false;
}

bool isLogoOperator (char* string)
{
    if (isEqual(string, OPER_FD) || isEqual(string, OPER_BK) || isEqual(string, OPER_RT) || isEqual(string, OPER_LT) || isEqual(string, OPER_PU) || isEqual(string, OPER_PD) || isEqual(string, OPER_SETXY) || isEqual(string, OPER_HOME) || isEqual(string, OPER_CLEAN) || isEqual(string, OPER_ST) || isEqual(string, OPER_HT) || isEqual(string, OPER_SETPENCOLOR) || isEqual(string, OPER_PRINT) || isEqual(string, OPER_MAKE) || isEqual(string, OPER_BEGINFILE) || isEqual(string, OPER_ENDFILE) || isEqual(string, OPER_LOADFILE) || isEqual(string, OPER_EXIT) || isEqual(string, OPER_OR) || isEqual(string, OPER_AND))
    {
        return true;
    }

    return false;
}

bool isVariable (char* string)
{
    if (!isInteger(string) && !isFloat(string) && !isLogoString(string) && !isOperator(string))
    {
        return true;
    }

    return false;
}

void evaluate (ParseList* list)
{
    char error[101];
    bool evaluating = true;
    LogoBoard* logoBoard;
    ParseNode* iterator;
    
    do
    {
        evaluateArithmetic(list);
        evaluateRelational(list);
        evaluating = evaluateCommands(list);
    } while (evaluating);

    iterator = list->head;

    // Checks for a values left unused in the list and replaces with an error
    for (iterator = list->head; iterator != NULL; iterator = iterator->next)
    {
        if (iterator->info.type == pt_int)
        {
            sprintf (error, ERROR_NO_ACTION_INT, iterator->info.value.intValue);
            iterator->info = errorParseVal(error);
        }
        else if (iterator->info.type == pt_bool)
        {
            if (iterator->info.value.boolValue)
            {
                sprintf (error, ERROR_NO_ACTION_BOOL, LOGO_TRUE);
            }
            else
            {
                sprintf (error, ERROR_NO_ACTION_BOOL, LOGO_FALSE);
            }
            iterator->info = errorParseVal(error);
        }
        else if (iterator->info.type == pt_float)
        {
            sprintf (error, ERROR_NO_ACTION_FLOAT, iterator->info.value.floatValue);
            iterator->info = errorParseVal(error);
        }
        else if (iterator->info.type == pt_string)
        {
            sprintf (error, ERROR_NO_ACTION_STRING, iterator->info.value.stringValue);
            iterator->info = errorParseVal(error);
        }
        else if (iterator->info.type == pt_variable)
        {
            evaluateNode(list, iterator); // Evaluate a variable left in the list

            if (iterator->info.type == pt_int)
            {
                sprintf (error, ERROR_NO_ACTION_INT, iterator->info.value.intValue);
                iterator->info = errorParseVal(error);
            }
            else if (iterator->info.type == pt_bool)
            {
                if (iterator->info.value.boolValue)
                {
                    sprintf (error, ERROR_NO_ACTION_BOOL, LOGO_TRUE);
                }
                else
                {
                    sprintf (error, ERROR_NO_ACTION_BOOL, LOGO_FALSE);
                }
                    iterator->info = errorParseVal(error);
                }
            else if (iterator->info.type == pt_float)
            {
                sprintf (error, ERROR_NO_ACTION_FLOAT, iterator->info.value.floatValue);
                iterator->info = errorParseVal(error);
            }
            else if (iterator->info.type == pt_string)
            {
                sprintf (error, ERROR_NO_ACTION_STRING, iterator->info.value.stringValue);
                iterator->info = errorParseVal(error);
            }
        }
    }

    // Checks for a error message in the list
    for (iterator = list->head; iterator != NULL; iterator = iterator->next)
    {
        if (iterator->info.type == pt_error)
        {
            logoBoard = getLogoBoard();
            executePrint(logoBoard, iterator->info.value.stringValue);
            break;
        }
    }
}

bool evaluateCommands (ParseList *list)
{
    bool evaluated = false;
    ParseNode *iterator;

    iterator = list->head;

    while (iterator != NULL)
    {
        if (iterator->info.type == pt_logo_operator)
        {
            evaluated = true;
            evaluateNode(list, iterator);
            break;
        }
        iterator = iterator->next;
    }

    return evaluated;
}

bool evaluateArithmetic (ParseList *list)
{
    bool inCommand = false;
    bool evaluated = false;
    ParseNode *iterator;

    iterator = list->head;

    // Evaluate first multiplication and division
    while (iterator != NULL && !(inCommand && iterator->info.type == pt_logo_operator))
    {
        if (iterator->info.type == pt_logo_operator)
        {
            inCommand = true;
        }

        if (iterator->info.type == pt_arithmetic_operator)
        {
            if (isEqual(iterator->info.value.stringValue, OPER_TIMES) || isEqual(iterator->info.value.stringValue, OPER_DIVIDE))
            {
                evaluated = true;
                evaluateNode(list, iterator);
            }
        }
        iterator = iterator->next;
    }

    iterator = list->head;
    inCommand = false;

    // Then, evaluates sums and substractions
    while (iterator != NULL && !(inCommand && iterator->info.type == pt_logo_operator))
    {
        if (iterator->info.type == pt_arithmetic_operator)
        {
            inCommand = true;
        }
        
        if (iterator->info.type == pt_arithmetic_operator)
        {
            if (isEqual(iterator->info.value.stringValue, OPER_PLUS) || isEqual(iterator->info.value.stringValue, OPER_MINUS))
            {
                evaluated = true;
                evaluateNode(list, iterator);
            }
        }
        iterator = iterator->next;
    }

    return evaluated;
}

bool evaluateRelational (ParseList *list)
{
    bool inCommand = false;
    bool evaluated = false;
    ParseNode *iterator;

    iterator = list->head;

    // Evaluate first multiplication and division
    while (iterator != NULL && !(inCommand && iterator->info.type == pt_logo_operator))
    {
        if (iterator->info.type == pt_logo_operator)
        {
            inCommand = true;
        }

        if (iterator->info.type == pt_relational_operator)
        {
            evaluated = true;
            evaluateNode(list, iterator);
        }
        iterator = iterator->next;
    }

    return evaluated;
}

ParseVal evaluateNode (ParseList *list, ParseNode* node)
{
    ParseVal result;
    Memory* memory;
    Variable* var;
    char error[101];

    memory = getMemory();

    if (node != NULL)
    {
        switch (node->info.type)
        {
        case pt_int:
        case pt_bool:
        case pt_float:
        case pt_string:
            result = node->info;
            break;

        case pt_arithmetic_operator:
            if (isEqual(node->info.value.stringValue, OPER_PLUS))
            {
                result = evaluatePlus(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_MINUS))
            {
                result = evaluateMinus(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_TIMES))
            {
                result = evaluateTimes(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_DIVIDE))
            {
                result = evaluateDivide(list, node);
            }
            break;

        case pt_relational_operator:
            if (isEqual(node->info.value.stringValue, OPER_LESS))
            {
                result = evaluateLess(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_LESS_EQUAL))
            {
                result = evaluateLessEqual(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_GREAT))
            {
                result = evaluateGreat(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_GREAT_EQUAL))
            {
                result = evaluateGreatEqual(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_EQUAL))
            {
                result = evaluateEqual(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_NOT_EQUAL))
            {
                result = evaluateNotEqual(list, node);
            }
            break;

        case pt_logo_operator:
            if (isEqual(node->info.value.stringValue, OPER_FD))
            {
                result = evaluateFd(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_BK))
            {
                result = evaluateBk(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_RT))
            {
                result = evaluateRt(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_LT))
            {
                result = evaluateLt(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_PU))
            {
                result = evaluatePu(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_PD))
            {
                result = evaluatePd(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_SETXY))
            {
                result = evaluateSetxy(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_HOME))
            {
                result = evaluateHome(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_CLEAN))
            {
                result = evaluateClean(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_ST))
            {
                result = evaluateSt(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_HT))
            {
                result = evaluateHt(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_SETPENCOLOR))
            {
                result = evaluateSetpencolor(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_PRINT))
            {
                result = evaluatePrint(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_MAKE))
            {
                result = evaluateMake(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_BEGINFILE))
            {
                result = evaluateBeginfile(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_ENDFILE))
            {
                result = evaluateEndfile(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_LOADFILE))
            {
                result = evaluateLoadfile(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_EXIT))
            {
                result = evaluateExit(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_OR))
            {
                result = evaluateOr(list, node);
            }
            else if (isEqual(node->info.value.stringValue, OPER_AND))
            {
                result = evaluateAnd(list, node);
            }
            break;

        case pt_variable:
            var = getVar(memory, node->info.value.stringValue);

            // If the variable is in the memory
            if (var != NULL)
            {
                result = toParseVal(var);
                cleanNode(node);
                node->info = result;
            }
            else
            {
                sprintf (error, ERROR_UNDEFINED, node->info.value.stringValue);
                result = errorParseVal(error);
                cleanNode(node);
                node->info = result;
            }
            break;

        case pt_error:
        case pt_empty:
            result = node->info;
            break;
        }
    }
    else
    {
        result = emptyParseVal();
    }


    return result;
}

ParseVal evaluateLess (ParseList* list, ParseNode* node)
{
    ParseVal left;
    ParseVal right;
    ParseVal parseVal;

    left = evaluateNode (list, node->previous);
    right = evaluateNode (list, node->next);

    if (left.type == pt_int)
    {
        if (right.type == pt_int)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.intValue < right.value.intValue;
        }
        else if (right.type == pt_float)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.intValue < right.value.floatValue;
        }
        else if (right.type == pt_empty)
        {
            parseVal = errorParseVal(ERROR_LESS_NOT_ENOUGH_INPUTS);
        }
        else if (right.type == pt_error)
        {
            parseVal = right;
        }
        else
        {
            parseVal = errorParseVal(ERROR_LESS);
        }
    }
    else if (left.type == pt_float)
    {
        if (right.type == pt_int)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.floatValue < right.value.intValue;
        }
        else if (right.type == pt_float)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.floatValue < right.value.floatValue;
        }
        else if (right.type == pt_empty)
        {
            parseVal = errorParseVal(ERROR_LESS_NOT_ENOUGH_INPUTS);
        }
        else if (right.type == pt_error)
        {
            parseVal = right;
        }
        else
        {
            parseVal = errorParseVal(ERROR_LESS);
        }
    }
    else if (left.type == pt_empty)
    {
        parseVal = errorParseVal(ERROR_LESS_NOT_ENOUGH_INPUTS);
    }
    else if (left.type == pt_error)
    {
        parseVal = left;
    }
    else
    {
        parseVal = errorParseVal(ERROR_LESS);
    }
    removePrevious(list, node);
    removeNext(list, node);

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateLessEqual (ParseList* list, ParseNode* node)
{
    ParseVal left;
    ParseVal right;
    ParseVal parseVal;

    left = evaluateNode (list, node->previous);
    right = evaluateNode (list, node->next);

    if (left.type == pt_int)
    {
        if (right.type == pt_int)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.intValue <= right.value.intValue;
        }
        else if (right.type == pt_float)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.intValue <= right.value.floatValue;
        }
        else if (right.type == pt_empty)
        {
            parseVal = errorParseVal(ERROR_LESS_EQUAL_NOT_ENOUGH_INPUTS);
        }
        else if (right.type == pt_error)
        {
            parseVal = right;
        }
        else
        {
            parseVal = errorParseVal(ERROR_LESS_EQUAL);
        }
    }
    else if (left.type == pt_float)
    {
        if (right.type == pt_int)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.floatValue <= right.value.intValue;
        }
        else if (right.type == pt_float)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.floatValue <= right.value.floatValue;
        }
        else if (right.type == pt_empty)
        {
            parseVal = errorParseVal(ERROR_LESS_EQUAL_NOT_ENOUGH_INPUTS);
        }
        else if (right.type == pt_error)
        {
            parseVal = right;
        }
        else
        {
            parseVal = errorParseVal(ERROR_LESS_EQUAL);
        }
    }
    else if (left.type == pt_empty)
    {
        parseVal = errorParseVal(ERROR_LESS_EQUAL_NOT_ENOUGH_INPUTS);
    }
    else if (left.type == pt_error)
    {
        parseVal = left;
    }
    else
    {
        parseVal = errorParseVal(ERROR_LESS_EQUAL);
    }
    removePrevious(list, node);
    removeNext(list, node);

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateGreat (ParseList* list, ParseNode* node)
{
    ParseVal left;
    ParseVal right;
    ParseVal parseVal;

    left = evaluateNode (list, node->previous);
    right = evaluateNode (list, node->next);

    if (left.type == pt_int)
    {
        if (right.type == pt_int)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.intValue > right.value.intValue;
        }
        else if (right.type == pt_float)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.intValue > right.value.floatValue;
        }
        else if (right.type == pt_empty)
        {
            parseVal = errorParseVal(ERROR_GREAT_NOT_ENOUGH_INPUTS);
        }
        else if (right.type == pt_error)
        {
            parseVal = right;
        }
        else
        {
            parseVal = errorParseVal(ERROR_GREAT);
        }
    }
    else if (left.type == pt_float)
    {
        if (right.type == pt_int)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.floatValue > right.value.intValue;
        }
        else if (right.type == pt_float)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.floatValue > right.value.floatValue;
        }
        else if (right.type == pt_empty)
        {
            parseVal = errorParseVal(ERROR_GREAT_NOT_ENOUGH_INPUTS);
        }
        else if (right.type == pt_error)
        {
            parseVal = right;
        }
        else
        {
            parseVal = errorParseVal(ERROR_GREAT);
        }
    }
    else if (left.type == pt_empty)
    {
        parseVal = errorParseVal(ERROR_GREAT_NOT_ENOUGH_INPUTS);
    }
    else if (left.type == pt_error)
    {
        parseVal = left;
    }
    else
    {
        parseVal = errorParseVal(ERROR_GREAT);
    }
    removePrevious(list, node);
    removeNext(list, node);

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateGreatEqual (ParseList* list, ParseNode* node)
{
    ParseVal left;
    ParseVal right;
    ParseVal parseVal;

    left = evaluateNode (list, node->previous);
    right = evaluateNode (list, node->next);

    if (left.type == pt_int)
    {
        if (right.type == pt_int)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.intValue >= right.value.intValue;
        }
        else if (right.type == pt_float)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.intValue >= right.value.floatValue;
        }
        else if (right.type == pt_empty)
        {
            parseVal = errorParseVal(ERROR_GREAT_EQUAL_NOT_ENOUGH_INPUTS);
        }
        else if (right.type == pt_error)
        {
            parseVal = right;
        }
        else
        {
            parseVal = errorParseVal(ERROR_GREAT_EQUAL);
        }
    }
    else if (left.type == pt_float)
    {
        if (right.type == pt_int)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.floatValue >= right.value.intValue;
        }
        else if (right.type == pt_float)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.floatValue >= right.value.floatValue;
        }
        else if (right.type == pt_empty)
        {
            parseVal = errorParseVal(ERROR_GREAT_EQUAL_NOT_ENOUGH_INPUTS);
        }
        else if (right.type == pt_error)
        {
            parseVal = right;
        }
        else
        {
            parseVal = errorParseVal(ERROR_GREAT_EQUAL);
        }
    }
    else if (left.type == pt_empty)
    {
        parseVal = errorParseVal(ERROR_GREAT_EQUAL_NOT_ENOUGH_INPUTS);
    }
    else if (left.type == pt_error)
    {
        parseVal = left;
    }
    else
    {
        parseVal = errorParseVal(ERROR_GREAT_EQUAL);
    }
    removePrevious(list, node);
    removeNext(list, node);

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateEqual (ParseList* list, ParseNode* node)
{
    ParseVal left;
    ParseVal right;
    ParseVal parseVal;

    left = evaluateNode (list, node->previous);
    right = evaluateNode (list, node->next);

    if (left.type == pt_int)
    {
        if (right.type == pt_int)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.intValue == right.value.intValue;
        }
        else if (right.type == pt_float)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.intValue == right.value.floatValue;
        }
        else if (right.type == pt_empty)
        {
            parseVal = errorParseVal(ERROR_EQUAL_NOT_ENOUGH_INPUTS);
        }
        else if (right.type == pt_error)
        {
            parseVal = right;
        }
        else
        {
            parseVal = errorParseVal(ERROR_EQUAL);
        }
    }
    else if (left.type == pt_float)
    {
        if (right.type == pt_int)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.floatValue == right.value.intValue;
        }
        else if (right.type == pt_float)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.floatValue == right.value.floatValue;
        }
        else if (right.type == pt_empty)
        {
            parseVal = errorParseVal(ERROR_EQUAL_NOT_ENOUGH_INPUTS);
        }
        else if (right.type == pt_error)
        {
            parseVal = right;
        }
        else
        {
            parseVal = errorParseVal(ERROR_EQUAL);
        }
    }
    else if (left.type == pt_empty)
    {
        parseVal = errorParseVal(ERROR_EQUAL_NOT_ENOUGH_INPUTS);
    }
    else if (left.type == pt_error)
    {
        parseVal = left;
    }
    else
    {
        parseVal = errorParseVal(ERROR_EQUAL);
    }
    removePrevious(list, node);
    removeNext(list, node);

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateNotEqual (ParseList* list, ParseNode* node)
{
    ParseVal left;
    ParseVal right;
    ParseVal parseVal;

    left = evaluateNode (list, node->previous);
    right = evaluateNode (list, node->next);

    if (left.type == pt_int)
    {
        if (right.type == pt_int)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.intValue != right.value.intValue;
        }
        else if (right.type == pt_float)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.intValue != right.value.floatValue;
        }
        else if (right.type == pt_empty)
        {
            parseVal = errorParseVal(ERROR_NOT_EQUAL_NOT_ENOUGH_INPUTS);
        }
        else if (right.type == pt_error)
        {
            parseVal = right;
        }
        else
        {
            parseVal = errorParseVal(ERROR_NOT_EQUAL);
        }
    }
    else if (left.type == pt_float)
    {
        if (right.type == pt_int)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.floatValue != right.value.intValue;
        }
        else if (right.type == pt_float)
        {
            parseVal.type = pt_bool;
            parseVal.value.boolValue = left.value.floatValue != right.value.floatValue;
        }
        else if (right.type == pt_empty)
        {
            parseVal = errorParseVal(ERROR_NOT_EQUAL_NOT_ENOUGH_INPUTS);
        }
        else if (right.type == pt_error)
        {
            parseVal = right;
        }
        else
        {
            parseVal = errorParseVal(ERROR_NOT_EQUAL);
        }
    }
    else if (left.type == pt_empty)
    {
        parseVal = errorParseVal(ERROR_NOT_EQUAL_NOT_ENOUGH_INPUTS);
    }
    else if (left.type == pt_error)
    {
        parseVal = left;
    }
    else
    {
        parseVal = errorParseVal(ERROR_NOT_EQUAL);
    }
    removePrevious(list, node);
    removeNext(list, node);

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluatePlus (ParseList* list, ParseNode* node)
{
    ParseVal left;
    ParseVal right;
    ParseVal parseVal;

    left = evaluateNode (list, node->previous);
    right = evaluateNode (list, node->next);

    if (left.type == pt_int)
    {
        if (right.type == pt_int)
        {
            parseVal.type = pt_int;
            parseVal.value.intValue = left.value.intValue + right.value.intValue;
        }
        else if (right.type == pt_float)
        {
            parseVal.type = pt_float;
            parseVal.value.floatValue = left.value.intValue + right.value.floatValue;
        }
        else if (right.type == pt_empty)
        {
            parseVal = errorParseVal(ERROR_PLUS_NOT_ENOUGH_INPUTS);
        }
        else if (right.type == pt_error)
        {
            parseVal = right;
        }
        else
        {
            parseVal = errorParseVal(ERROR_PLUS);
        }
    }
    else if (left.type == pt_float)
    {
        if (right.type == pt_int)
        {
            parseVal.type = pt_float;
            parseVal.value.floatValue = left.value.floatValue + right.value.intValue;
        }
        else if (right.type == pt_float)
        {
            parseVal.type = pt_float;
            parseVal.value.floatValue = left.value.floatValue + right.value.floatValue;
        }
        else if (right.type == pt_empty)
        {
            parseVal = errorParseVal(ERROR_PLUS_NOT_ENOUGH_INPUTS);
        }
        else if (right.type == pt_error)
        {
            parseVal = right;
        }
        else
        {
            parseVal = errorParseVal(ERROR_PLUS);
        }
    }
    else if (left.type == pt_empty)
    {
        parseVal = errorParseVal(ERROR_PLUS_NOT_ENOUGH_INPUTS);
    }
    else if (left.type == pt_error)
    {
        parseVal = left;
    }
    else
    {
        parseVal = errorParseVal(ERROR_PLUS);
    }
    removePrevious(list, node);
    removeNext(list, node);

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateMinus (ParseList* list, ParseNode* node)
{
    ParseVal left;
    ParseVal right;
    ParseVal parseVal;

    left = evaluateNode (list, node->previous);
    right = evaluateNode (list, node->next);


    if (left.type == pt_int)
    {
        if (right.type == pt_int)
        {
            parseVal.type = pt_int;
            parseVal.value.intValue = left.value.intValue - right.value.intValue;
        }
        else if (right.type == pt_float)
        {
            parseVal.type = pt_float;
            parseVal.value.floatValue = left.value.intValue - right.value.floatValue;
        }
        else if (right.type == pt_empty)
        {
            parseVal = errorParseVal(ERROR_MINUS_NOT_ENOUGH_INPUTS);
        }
        else if (right.type == pt_error)
        {
            parseVal = right;
        }
        else
        {
            parseVal = errorParseVal(ERROR_MINUS);
        }
    }
    else if (left.type == pt_float)
    {
        if (right.type == pt_int)
        {
            parseVal.type = pt_float;
            parseVal.value.floatValue = left.value.floatValue - right.value.intValue;
        }
        else if (right.type == pt_float)
        {
            parseVal.type = pt_float;
            parseVal.value.floatValue = left.value.floatValue - right.value.floatValue;
        }
        else if (right.type == pt_empty)
        {
            parseVal = errorParseVal(ERROR_MINUS_NOT_ENOUGH_INPUTS);
        }
        else if (right.type == pt_error)
        {
            parseVal = right;
        }
        else
        {
            parseVal = errorParseVal(ERROR_MINUS);
        }
    }
    else if (left.type == pt_empty)
    {
        parseVal = errorParseVal(ERROR_MINUS_NOT_ENOUGH_INPUTS);
    }
    else if (left.type == pt_error)
    {
        parseVal = left;
    }
    else
    {
        parseVal = errorParseVal(ERROR_MINUS);
    }
    removePrevious(list, node);
    removeNext(list, node);

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateTimes (ParseList* list, ParseNode* node)
{
    ParseVal left;
    ParseVal right;
    ParseVal parseVal;

    left = evaluateNode (list, node->previous);
    right = evaluateNode (list, node->next);


    if (left.type == pt_int)
    {
        if (right.type == pt_int)
        {
            parseVal.type = pt_int;
            parseVal.value.intValue = left.value.intValue * right.value.intValue;
        }
        else if (right.type == pt_float)
        {
            parseVal.type = pt_float;
            parseVal.value.floatValue = left.value.intValue * right.value.floatValue;
        }
        else if (right.type == pt_empty)
        {
            parseVal = errorParseVal(ERROR_TIMES_NOT_ENOUGH_INPUTS);
        }
        else if (right.type == pt_error)
        {
            parseVal = right;
        }
        else
        {
            parseVal = errorParseVal(ERROR_TIMES);
        }
    }
    else if (left.type == pt_float)
    {
        if (right.type == pt_int)
        {
            parseVal.type = pt_float;
            parseVal.value.floatValue = left.value.floatValue * right.value.intValue;
        }
        else if (right.type == pt_float)
        {
            parseVal.type = pt_float;
            parseVal.value.floatValue = left.value.floatValue * right.value.floatValue;
        }
        else if (right.type == pt_empty)
        {
            parseVal = errorParseVal(ERROR_TIMES_NOT_ENOUGH_INPUTS);
        }
        else if (right.type == pt_error)
        {
            parseVal = right;
        }
        else
        {
            parseVal = errorParseVal(ERROR_TIMES);
        }
    }
    else if (left.type == pt_empty)
    {
        parseVal = errorParseVal(ERROR_TIMES_NOT_ENOUGH_INPUTS);
    }
    else if (left.type == pt_error)
    {
        parseVal = left;
    }
    else
    {
        parseVal = errorParseVal(ERROR_TIMES);
    }
    removePrevious(list, node);
    removeNext(list, node);

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateDivide (ParseList* list, ParseNode* node)
{
    ParseVal left;
    ParseVal right;
    ParseVal parseVal;

    left = evaluateNode (list, node->previous);
    right = evaluateNode (list, node->next);


    if (left.type == pt_int)
    {
        if (right.type == pt_int)
        {
            parseVal.type = pt_int;
            parseVal.value.intValue = left.value.intValue / right.value.intValue;
        }
        else if (right.type == pt_float)
        {
            parseVal.type = pt_float;
            parseVal.value.floatValue = left.value.intValue / right.value.floatValue;
        }
        else if (right.type == pt_empty)
        {
            parseVal = errorParseVal(ERROR_DIVIDE_NOT_ENOUGH_INPUTS);
        }
        else if (right.type == pt_error)
        {
            parseVal = right;
        }
        else
        {
            parseVal = errorParseVal(ERROR_DIVIDE);
        }
    }
    else if (left.type == pt_float)
    {
        if (right.type == pt_int)
        {
            parseVal.type = pt_float;
            parseVal.value.floatValue = left.value.floatValue / right.value.intValue;
        }
        else if (right.type == pt_float)
        {
            parseVal.type = pt_float;
            parseVal.value.floatValue = left.value.floatValue / right.value.floatValue;
        }
        else if (right.type == pt_empty)
        {
            parseVal = errorParseVal(ERROR_DIVIDE_NOT_ENOUGH_INPUTS);
        }
        else if (right.type == pt_error)
        {
            parseVal = right;
        }
        else
        {
            parseVal = errorParseVal(ERROR_DIVIDE);
        }
    }
    else if (left.type == pt_empty)
    {
        parseVal = errorParseVal(ERROR_DIVIDE_NOT_ENOUGH_INPUTS);
    }
    else if (left.type == pt_error)
    {
        parseVal = left;
    }
    else
    {
        parseVal = errorParseVal(ERROR_DIVIDE);
    }
    removePrevious(list, node);
    removeNext(list, node);

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateFd (ParseList* list, ParseNode* node)
{
    int steps;
    ParseVal right;
    ParseVal parseVal;
    LogoBoard* logoBoard;

    logoBoard = getLogoBoard();
    parseVal = emptyParseVal();

    right = evaluateNode (list, node->next);

    if (right.type == pt_int)
    {
        steps = right.value.intValue;
    }
    else if (right.type == pt_float)
    {
        steps = right.value.floatValue;
    }
    else if (right.type == pt_empty)
    {
        parseVal =  errorParseVal(ERROR_FD_NOT_ENOUGH_INPUTS);
    }
    else if (right.type == pt_error)
    {
        parseVal = right;
    }
    else
    {
        parseVal = errorParseVal(ERROR_FD);
    }
    removeNext(list, node);

    if (parseVal.type == pt_empty)
    {
        executeFd(logoBoard, steps);
    }

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateBk (ParseList* list, ParseNode* node)
{
    int steps;
    ParseVal right;
    ParseVal parseVal;
    LogoBoard* logoBoard;

    logoBoard = getLogoBoard();

    parseVal = emptyParseVal();


    right = evaluateNode(list, node->next);

    if (right.type == pt_int)
    {
        steps = right.value.intValue;
    }
    else if (right.type == pt_float)
    {
        steps = right.value.floatValue;
    }
    else if (right.type == pt_empty)
    {
        parseVal =  errorParseVal(ERROR_BK_NOT_ENOUGH_INPUTS);
    }
    else if (right.type == pt_error)
    {
        parseVal = right;
    }
    else
    {
        parseVal = errorParseVal(ERROR_BK);
    }
    removeNext(list, node);

    if (parseVal.type == pt_empty)
    {
        executeBk(logoBoard, steps);
    }

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateRt (ParseList* list, ParseNode* node)
{
    int angle;
    ParseVal right;
    ParseVal parseVal;
    LogoBoard* logoBoard;

    logoBoard = getLogoBoard();

    parseVal = emptyParseVal();

    right = evaluateNode (list, node->next);

    if (right.type == pt_int)
    {
        angle = right.value.intValue;
    }
    else if (right.type == pt_float)
    {
        angle = right.value.floatValue;
    }
    else if (right.type == pt_empty)
    {
        parseVal =  errorParseVal(ERROR_RT_NOT_ENOUGH_INPUTS);
    }
    else if (right.type == pt_error)
    {
        parseVal = right;
    }
    else
    {
        parseVal = errorParseVal(ERROR_RT);
    }
    removeNext(list, node);

    if (parseVal.type == pt_empty)
    {
        executeRt(logoBoard, angle);
    }

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateLt (ParseList* list, ParseNode* node)
{
    int angle;
    ParseVal right;
    ParseVal parseVal;
    LogoBoard* logoBoard;

    logoBoard = getLogoBoard();

    parseVal = emptyParseVal();


    right = evaluateNode (list, node->next);

    if (right.type == pt_int)
    {
        angle = right.value.intValue;
    }
    else if (right.type == pt_float)
    {
        angle = right.value.floatValue;
    }
    else if (right.type == pt_empty)
    {
        parseVal =  errorParseVal(ERROR_LT_NOT_ENOUGH_INPUTS);
    }
    else if (right.type == pt_error)
    {
        parseVal = right;
    }
    else
    {
        parseVal = errorParseVal(ERROR_LT);
    }
    removeNext(list, node);

    if (parseVal.type == pt_empty)
    {
        executeLt(logoBoard, angle);
    }

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluatePu (ParseList* list, ParseNode* node)
{
    ParseVal parseVal;
    LogoBoard* logoBoard;

    logoBoard = getLogoBoard();

    parseVal = emptyParseVal();

    executePu(logoBoard);

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluatePd (ParseList* list, ParseNode* node)
{
    ParseVal parseVal;
    LogoBoard* logoBoard;

    logoBoard = getLogoBoard();

    parseVal = emptyParseVal();

    executePd(logoBoard);

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateSetxy (ParseList* list, ParseNode* node)
{
    int x;
    int y;
    ParseVal right;
    ParseVal parseVal;
    LogoBoard* logoBoard;

    logoBoard = getLogoBoard();

    parseVal = emptyParseVal();

    right = evaluateNode (list, node->next);

    if (right.type == pt_int)
    {
        x = right.value.intValue;
    }
    else if (right.type == pt_float)
    {
        x = right.value.floatValue;
    }
    else if (right.type == pt_empty)
    {
        parseVal =  errorParseVal(ERROR_SETXY_NOT_ENOUGH_INPUTS);
    }
    else if (right.type == pt_error)
    {
        parseVal = right;
    }
    else
    {
        parseVal = errorParseVal(ERROR_SETXY);
    }

    removeNext(list, node);

    right = evaluateNode (list, node->next);

    if (right.type == pt_int)
    {
        y = right.value.intValue;
    }
    else if (right.type == pt_float)
    {
        y = right.value.floatValue;
    }
    else if (right.type == pt_empty)
    {
        parseVal =  errorParseVal(ERROR_SETXY_NOT_ENOUGH_INPUTS);
    }
    else if (right.type == pt_error)
    {
        parseVal = right;
    }
    else
    {
        parseVal = errorParseVal(ERROR_SETXY);
    }
    removeNext(list, node);

    if (parseVal.type == pt_empty)
    {
        executeSetxy(logoBoard, x, y);
    }

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateHome (ParseList* list, ParseNode* node)
{
    ParseVal parseVal;
    LogoBoard* logoBoard;

    logoBoard = getLogoBoard();

    parseVal = emptyParseVal();

    executeHome(logoBoard);

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateClean (ParseList* list, ParseNode* node)
{
    ParseVal parseVal;
    LogoBoard* logoBoard;

    logoBoard = getLogoBoard();

    parseVal = emptyParseVal();

    executeClean(logoBoard);

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateSt (ParseList* list, ParseNode* node)
{
    ParseVal parseVal;
    LogoBoard* logoBoard;

    logoBoard = getLogoBoard();

    parseVal = emptyParseVal();

    executeSt(logoBoard);
    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateHt (ParseList* list, ParseNode* node)
{
    ParseVal parseVal;
    LogoBoard* logoBoard;

    logoBoard = getLogoBoard();

    parseVal = emptyParseVal();

    executeHt(logoBoard);

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateSetpencolor (ParseList* list, ParseNode* node)
{
    int color;
    ParseVal right;
    ParseVal parseVal;
    LogoBoard* logoBoard;

    logoBoard = getLogoBoard();

    parseVal = emptyParseVal();


    right = evaluateNode (list, node->next);

    if (right.type == pt_int)
    {
        color = right.value.intValue;
    }
    else if (right.type == pt_empty)
    {
        parseVal =  errorParseVal(ERROR_SETPENCOLOR_NOT_ENOUGH_INPUTS);
    }
    else if (right.type == pt_error)
    {
        parseVal = right;
    }
    else
    {
        parseVal = errorParseVal(ERROR_SETPENCOLOR);
    }

    if (parseVal.type == pt_empty)
    {
        executeSetpencolor(logoBoard, color);
    }
    removeNext(list, node);

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluatePrint (ParseList* list, ParseNode* node)
{

    ParseVal right;
    ParseVal parseVal;
    LogoBoard* logoBoard;
    char output[MAX_OUTPUT];

    logoBoard = getLogoBoard();

    parseVal = emptyParseVal();

    right = evaluateNode (list, node->next);

    if (right.type == pt_int)
    {
        sprintf(output, "%d", right.value.intValue);
    }
    else if (right.type == pt_float)
    {
        sprintf(output, "%f", right.value.floatValue);
    }
    else if (right.type == pt_string)
    {
        sprintf(output, "%s", right.value.stringValue);
    }
    else if (right.type == pt_empty)
    {
        parseVal =  errorParseVal(ERROR_PRINT_NOT_ENOUGH_INPUTS);
    }
    else if (right.type == pt_error)
    {
        parseVal = right;
    }
    else
    {
        parseVal =  errorParseVal(ERROR_PRINT);
    }
    removeNext(list, node);

    if (parseVal.type == pt_empty)
    {
        executePrint(logoBoard, output);
    }

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateBeginfile (ParseList* list, ParseNode* node)
{

    ParseVal right;
    ParseVal parseVal;
    LogoBoard* logoBoard;

    logoBoard = getLogoBoard();

    parseVal = emptyParseVal();

    right = evaluateNode (list, node->next);

    if (right.type == pt_string)
    {
        executeBeginfile(logoBoard, right.value.stringValue);
    }
    else if (right.type == pt_empty)
    {
        parseVal =  errorParseVal(ERROR_BEGINFILE_NOT_ENOUGH_INPUTS);
    }
    else if (right.type == pt_error)
    {
        parseVal = right;
    }
    else
    {
        parseVal =  errorParseVal(ERROR_BEGINFILE);
    }
    removeNext(list, node);

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateEndfile (ParseList* list, ParseNode* node)
{
    ParseVal parseVal;
    LogoBoard* logoBoard;

    logoBoard = getLogoBoard();
    parseVal = emptyParseVal();

    executeEndfile(logoBoard);

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateLoadfile (ParseList* list, ParseNode* node)
{

    ParseVal right;
    ParseVal parseVal;
    LogoBoard* logoBoard;

    logoBoard = getLogoBoard();
    parseVal = emptyParseVal();

    right = evaluateNode (list, node->next);

    if (right.type == pt_string)
    {
        executeLoadfile(logoBoard, right.value.stringValue);
    }
    else if (right.type == pt_empty)
    {
        parseVal =  errorParseVal(ERROR_LOADFILE_NOT_ENOUGH_INPUTS);
    }
    else if (right.type == pt_error)
    {
        parseVal = right;
    }
    else
    {
        parseVal =  errorParseVal(ERROR_LOADFILE);
    }
    removeNext(list, node);

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateExit (ParseList* list, ParseNode* node)
{
    ParseVal parseVal;
    LogoBoard* logoBoard;

    logoBoard = getLogoBoard();
    parseVal = emptyParseVal();

    executeExit(logoBoard);

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateMake (ParseList* list, ParseNode* node)
{
    char* key;
    Variable* var;
    ParseVal right;
    ParseVal parseVal;

    Memory* memory = getMemory();

    parseVal = emptyParseVal();

    if (node->next->info.type != pt_variable)
    {
        right = evaluateNode (list, node->next);

        if (right.type == pt_string)
        {
            key = right.value.stringValue;
        }
        else if (right.type == pt_empty)
        {
            parseVal =  errorParseVal(ERROR_MAKE_NOT_ENOUGH_INPUTS);
        }
        else if (right.type == pt_error)
        {
            parseVal = right;
        }
        else
        {
            parseVal = errorParseVal(ERROR_MAKE);
        }
    }
    else
    {
        key = node->next->info.value.stringValue;
    }   

    right = evaluateNode (list, node->next->next);

    if ( right.type == pt_int || right.type == pt_float || right.type == pt_string || right.type == pt_bool)
    {
        var = toVariable(key, right);
        if (var != NULL)
        {
            makeVariable(memory, var);
        }
        else
        {
            parseVal =  errorParseVal(ERROR_MAKE_MEMORY);
        }
    }
    else if (right.type == pt_empty)
    {
        parseVal =  errorParseVal(ERROR_MAKE_NOT_ENOUGH_INPUTS);
    }
    else if (right.type == pt_error)
    {
        parseVal = right;
    }
    else
    {
        parseVal = errorParseVal(ERROR_MAKE);
    }

    removeNext(list, node);
    removeNext(list, node);

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateOr (ParseList* list, ParseNode* node)
{
    bool x;
    bool y;
    ParseVal right;
    ParseVal parseVal;

    parseVal = emptyParseVal();

    right = evaluateNode (list, node->next);

    if (right.type == pt_bool)
    {
        x = right.value.boolValue;
    }
    else if (right.type == pt_empty)
    {
        parseVal =  errorParseVal(ERROR_OR_NOT_ENOUGH_INPUTS);
    }
    else if (right.type == pt_error)
    {
        parseVal = right;
    }
    else
    {
        parseVal = errorParseVal(ERROR_OR);
    }

    removeNext(list, node);

    right = evaluateNode (list, node->next);

    if (right.type == pt_bool)
    {
        y = right.value.boolValue;
    }
    else if (right.type == pt_empty)
    {
        parseVal =  errorParseVal(ERROR_OR_NOT_ENOUGH_INPUTS);
    }
    else if (right.type == pt_error)
    {
        parseVal = right;
    }
    else
    {
        parseVal = errorParseVal(ERROR_OR);
    }
    removeNext(list, node);

    if (parseVal.type == pt_empty)
    {
        parseVal.type = pt_bool;
        parseVal.value.boolValue = x || y;
    }

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

ParseVal evaluateAnd (ParseList* list, ParseNode* node)
{
    bool x;
    bool y;
    ParseVal right;
    ParseVal parseVal;

    parseVal = emptyParseVal();

    right = evaluateNode (list, node->next);

    if (right.type == pt_bool)
    {
        x = right.value.boolValue;
    }
    else if (right.type == pt_empty)
    {
        parseVal =  errorParseVal(ERROR_AND_NOT_ENOUGH_INPUTS);
    }
    else if (right.type == pt_error)
    {
        parseVal = right;
    }
    else
    {
        parseVal = errorParseVal(ERROR_AND);
    }

    removeNext(list, node);

    right = evaluateNode (list, node->next);

    if (right.type == pt_bool)
    {
        y = right.value.boolValue;
    }
    else if (right.type == pt_empty)
    {
        parseVal =  errorParseVal(ERROR_AND_NOT_ENOUGH_INPUTS);
    }
    else if (right.type == pt_error)
    {
        parseVal = right;
    }
    else
    {
        parseVal = errorParseVal(ERROR_AND);
    }
    removeNext(list, node);

    if (parseVal.type == pt_empty)
    {
        parseVal.type = pt_bool;
        parseVal.value.boolValue = x && y;
    }

    cleanNode(node);
    node->info = parseVal;

    return parseVal;
}

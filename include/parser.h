/* 
* @Author: Erick Lucena Palmeira Silva
* @Date:   2015-02-10 01:55:56
* @Last Modified by:   Erick Lucena Palmeira Silva
* @Last Modified time: 2015-02-13 23:06:17
*/

#ifndef _ELUCENAP_PARSERH
#define _ELUCENAP_PARSERH

#include "memory.h"

#define MAX_OUTPUT 1001

#define OPER_LESS "<"
#define OPER_LESS_EQUAL "<="
#define OPER_GREAT ">"
#define OPER_GREAT_EQUAL ">="
#define OPER_EQUAL "="
#define OPER_NOT_EQUAL "<>"
#define OPER_PLUS "+"
#define OPER_MINUS "-"
#define OPER_TIMES "*"
#define OPER_DIVIDE "/"
#define OPER_OR "or"
#define OPER_AND "and"
#define OPER_FD "fd"
#define OPER_BK "bk"
#define OPER_RT "rt"
#define OPER_LT "lt"
#define OPER_PU "pu"
#define OPER_PD "pd"
#define OPER_SETXY "setxy"
#define OPER_HOME "home"
#define OPER_CLEAN "clean"
#define OPER_ST "st"
#define OPER_HT "ht"
#define OPER_SETPENCOLOR "setpencolor"
#define OPER_PRINT "print"
#define OPER_MAKE "make"
#define OPER_BEGINFILE "beginfile"
#define OPER_ENDFILE "endfile"
#define OPER_LOADFILE "loadfile"
#define OPER_EXIT "exit"

#define LOGO_TRUE "true"
#define LOGO_FALSE "false"

#define ERROR_UNDEFINED "I don't know how to %s"
#define ERROR_NO_ACTION_INT "You don't say what to do with %d"
#define ERROR_NO_ACTION_BOOL "You don't say what to do with %s"
#define ERROR_NO_ACTION_FLOAT "You don't say what to do with %f"
#define ERROR_NO_ACTION_STRING "You don't say what to do with %s"
#define ERROR_TYPE "unknown type"

#define ERROR_LESS "< doesn't like the input"
#define ERROR_LESS_NOT_ENOUGH_INPUTS "not enough inputs to <"

#define ERROR_LESS_EQUAL "<= doesn't like the input"
#define ERROR_LESS_EQUAL_NOT_ENOUGH_INPUTS "not enough inputs to <="

#define ERROR_GREAT "> doesn't like the input"
#define ERROR_GREAT_NOT_ENOUGH_INPUTS "not enough inputs to >"

#define ERROR_GREAT_EQUAL ">= doesn't like the input"
#define ERROR_GREAT_EQUAL_NOT_ENOUGH_INPUTS "not enough inputs to >="

#define ERROR_EQUAL "= doesn't like the input"
#define ERROR_EQUAL_NOT_ENOUGH_INPUTS "not enough inputs to ="

#define ERROR_NOT_EQUAL "<> doesn't like the input"
#define ERROR_NOT_EQUAL_NOT_ENOUGH_INPUTS "not enough inputs to <>"

#define ERROR_PLUS "+ doesn't like the input"
#define ERROR_PLUS_NOT_ENOUGH_INPUTS "not enough inputs to +"

#define ERROR_MINUS "- doesn't like the input"
#define ERROR_MINUS_NOT_ENOUGH_INPUTS "not enough inputs to -"

#define ERROR_TIMES "* doesn't like the input"
#define ERROR_TIMES_NOT_ENOUGH_INPUTS "not enough inputs to *"

#define ERROR_DIVIDE "/ doesn't like the input"
#define ERROR_DIVIDE_NOT_ENOUGH_INPUTS "not enough inputs to /"

#define ERROR_FD "fd doesn't like the input"
#define ERROR_FD_NOT_ENOUGH_INPUTS "not enough inputs to fd"

#define ERROR_BK "bk doesn't like the input"
#define ERROR_BK_NOT_ENOUGH_INPUTS "not enough inputs to bk"

#define ERROR_RT "rt doesn't like the input"
#define ERROR_RT_NOT_ENOUGH_INPUTS "not enough inputs to rt"

#define ERROR_LT "lt doesn't like the input"
#define ERROR_LT_NOT_ENOUGH_INPUTS "not enough inputs to lt"

#define ERROR_SETXY "setxy doesn't like the input"
#define ERROR_SETXY_NOT_ENOUGH_INPUTS "not enough inputs to setxy"

#define ERROR_SETPENCOLOR "setpencolor doesn't like the input"
#define ERROR_SETPENCOLOR_NOT_ENOUGH_INPUTS "not enough inputs to setpencolor"

#define ERROR_PRINT "print doesn't like the input"
#define ERROR_PRINT_NOT_ENOUGH_INPUTS "not enough inputs to print"

#define ERROR_MAKE "make doesn't like the input"
#define ERROR_MAKE_MEMORY "had a problem saving the value"
#define ERROR_MAKE_NOT_ENOUGH_INPUTS "not enough inputs to make"

#define ERROR_BEGINFILE "beginfile doesn't like the input"
#define ERROR_BEGINFILE_NOT_ENOUGH_INPUTS "not enough inputs to beginfile"

#define ERROR_LOADFILE "loadfile doesn't like the input"
#define ERROR_LOADFILE_NOT_ENOUGH_INPUTS "not enough inputs to loadfile"

#define ERROR_OR "or doesn't like the input"
#define ERROR_OR_NOT_ENOUGH_INPUTS "not enough inputs to or"

#define ERROR_AND "and doesn't like the input"
#define ERROR_AND_NOT_ENOUGH_INPUTS "not enough inputs to and"


typedef enum parseType
{
    pt_empty, 
    pt_error,
    pt_logo_operator,
    pt_relational_operator,
    pt_arithmetic_operator,
    pt_variable,
    pt_int,
    pt_bool,
    pt_float,
    pt_string
} ParseType;

typedef struct parseVal
{
    ParseType type;
    VarValue value;
} ParseVal;

typedef struct parseNode
{
    ParseVal info;
    struct parseNode *previous;
    struct parseNode *next;
} ParseNode;

typedef struct parseList
{
    ParseNode* head;
    ParseNode* tail;
} ParseList;

/**
 NewParseNode: Allocates memory for a new ParseNode structure, initializing it with the values passed as parameters 
 Pre: Pass the parameters
 Post: Returns a pointer to a ParseNode if sucessful, otherwise returns NULL
*/
ParseNode* newParseNode (ParseVal info, ParseNode* previous, ParseNode* next);

/**
 SetNext: Sets the value of the next property of $node to $next
 Pre: Pass the parameters
 Post: The new value of next is set
*/
void setNext (ParseNode* node, ParseNode *next);

/**
 SetPrevious: Sets the value of the previous property of $node to $previous
 Pre: Pass the parameters
 Post: The new value of previous is set
*/
void setPrevious (ParseNode* node, ParseNode *previous);

/**
 CleanNode: Returns the node to an empty state, and frees any unnecessary memory pointed by it
 Pre: Pass the parameter
 Post: The node passed as parameter is now an empty node
*/
void cleanNode (ParseNode *node);

/**
 FreeNode: Recursively frees the memory occupied by $node and its next and previous nodes
 Pre: Pass the parameter
 Post: The memory is freed
*/
void freeNode (ParseNode *node);


/**
 NewParseList: Allocates for a new ParseList structure
 Pre: NA
 Post: Returns a pointer to a ParseList if successful, otherwise returns NULL
*/
ParseList* newParseList ();

/**
 FreeList: Recursively frees the memory occupied by $list and its nodes
 Pre: Pass the parameter
 Post: The memory occupied by the list and its nodes is free
*/
void freeList (ParseList* list);

/**
 PrintList: Prints the contents of list on the screen
 Pre: Pass the parameter
 Post: The contents of the list are printed on the screen
*/
void printList (ParseList* list);

/**
 InsertAtTail: Inserts the $node at the end of the $list
 Pre: Pass the parameters
 Post: The node is inserted at the end of the list
*/
void insertAtTail (ParseList* list, ParseNode* node);

/**
 RemovePrevious: Remove the node located at the left of $node from the $list
 Pre: Pass the parameters
 Post: The node is removed from the list and its memory freed
*/
void removePrevious (ParseList* list, ParseNode* node);

/**
 RemoveNext: Remove the node located at the right of $node from the list
 Pre: Pass the parameters
 Post: The node is removed from the list and its memory freed
*/
void removeNext (ParseList* list, ParseNode* node);


/**
 ToParseVal: Converts the contents of $var to a ParseVal 
 Pre: Pass the parameter
 Post: Returns a ParseVal structure
*/
ParseVal toParseVal (Variable* var);

/**
 ToVariable: Converts the contents of $parseval to a Variable, using $key as the variable key
 Pre: Pass the parameters
 Post: Returns a pointer to a Variable if successful, otherwise returns NULL
*/
Variable* toVariable (char* key, ParseVal parseVal);

/**
 ErrorParseVal: Returns an error (type) ParseVal, with $errorString as error message
 Pre: Pass the parameters
 Post: Returns a error (type) ParseVal
*/
ParseVal errorParseVal (char* errorString);

/**
 EmptyParseVal: Return an empty ParseVal
 Pre: NA
 Post: Returns a empty (type) ParseVal
*/
ParseVal emptyParseVal ();

/**
 PrintParseVal: Prints the contents of $variable on the screen
 Pre: Pass the parameter
 Post: The contents of the ParseVal are printed on the screen
*/
void printParseVal (ParseVal* variable);


/**
 ParseCommand: Tokenize the items on $command, and stores the number of tokens in $argc
 Pre: Pass the parameters
 Post: Returns an array of strings (char**) with the tokens, and stores the number of tokens on $argc
*/
char** parseCommand (char* command, int *argc);

/**
 ArgsToList: Converts the $argv array of arguments (tokens) to a ParseList, using $argc to control its execution 
 Pre: Pass the parameters
 Post: Returns a pointer to ParseList if successful, otherwise returns NULL
*/
ParseList* argsToList (int argc, char** argv);


/**
 IsOperator: Checks if $string is a valid operator 
 Pre: Pass the parameter
 Post: Returns true if string is an operator, false otherwise
*/
bool isOperator (char* string);

/**
 IsRelationalOperator: Checks if $string is a valid relational operator
 Pre: Pass the parameter
 Post: Returns true if string is an arithmetic operator, false otherwise
*/
bool isRelationalOperator (char* string);

/**
 IsArithmeticOperator: Checks if $string is a valid arithmetic operator
 Pre: Pass the parameter
 Post: Returns true if string is an arithmetic operator, false otherwise
*/
bool isArithmeticOperator (char* string);

/**
 IsLogoOperator: Checks if $string is a valid logo operator
 Pre: Pass the parameter
 Post: Returns true if string is a logo operator, false otherwise
*/
bool isLogoOperator (char* string);

/**
 IsVariable: Checks if $string is an valid variable identifier
 Pre: Pass the parameter
 Post: Returns true if string is a variable identifier, false otherwise
*/
bool isVariable (char* string);

/**
 Evaluate: Starts the evaluation of the $list
 Pre: Pass the parameter
 Post: Evaluates the expression represented by list
*/
void evaluate (ParseList* list);

/**
 EvaluateCommands: Evaluates logo commands on the $list
 Pre: Pass the parameter
 Post: Returns true if found and evaluated a logo command, returns false otherwise
*/
bool evaluateCommands (ParseList *list);

/**
 EvaluateArithmetic: Evaluates arithmetics operations on the $list
 Pre: Pass the parameters
 Post: Returns true if found and evaluated an arithmetic operation, returns false otherwise
*/
bool evaluateArithmetic (ParseList *list);

/**
 EvaluateRelational: Evaluates relational operations on the $list
 Pre: Pass the parameters
 Post: Returns true if found and evaluated an relational operation, returns false otherwise
*/
bool evaluateRelational (ParseList *list);

/**
 EvaluateNode: Recursively evaluates the value of a $node on the $list. 
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateNode (ParseList *list, ParseNode *node);

/**
 EvaluateLess: Recursively evaluates a less than operation
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateLess (ParseList *list, ParseNode *node);

/**
 EvaluateLessEqual: Recursively evaluates a less than or equal operation
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateLessEqual (ParseList *list, ParseNode *node);

/**
 EvaluateGreat: Recursively evaluates a greater than operation
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateGreat (ParseList *list, ParseNode *node);

/**
 EvaluateGreatEqual: Recursively evaluates a greater than or equal operation
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateGreatEqual (ParseList *list, ParseNode *node);

/**
 EvaluateEqual: Recursively evaluates a equal operation
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateEqual (ParseList *list, ParseNode *node);

/**
 EvaluateNotEqual: Recursively evaluates a not equal operation
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateNotEqual (ParseList *list, ParseNode *node);

/**
 EvaluatePlus: Recursively evaluates a plus operation
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluatePlus (ParseList *list, ParseNode *node);

/**
 EvaluatePlus: Recursively evaluates a plus operation
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluatePlus (ParseList *list, ParseNode *node);


/**
 EvaluateMinus: Recursively evaluates a minus operation
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateMinus (ParseList *list, ParseNode *node);

/**
 EvaluateTimes: Recursively evaluates a times operation
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateTimes (ParseList *list, ParseNode *node);

/**
 EvaluateDivide: Recursively evaluates a division operation
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateDivide (ParseList *list, ParseNode *node);


/**
 EvaluateFd: Recursively evaluates a fd command
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateFd (ParseList* list, ParseNode* node);

/**
 EvaluateBk: Recursively evaluates a bk command
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateBk (ParseList* list, ParseNode* node);

/**
 EvaluateRt: Recursively evaluates a rt command
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateRt (ParseList* list, ParseNode* node);

/**
 EvaluateLt: Recursively evaluates a lt command
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateLt (ParseList* list, ParseNode* node);

/**
 EvaluatePu: Recursively evaluates a pu command
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluatePu (ParseList* list, ParseNode* node);

/**
 EvaluatePd: Evaluates a Pd command
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluatePd (ParseList* list, ParseNode* node);

/**
 EvaluateSetxy: Recuservely valuates a setxy command
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateSetxy (ParseList* list, ParseNode* node);

/**
 EvaluateHome: Evaluates a home command
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateHome (ParseList* list, ParseNode* node);

/**
 EvaluateClean: Evaluates a clean command
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateClean (ParseList* list, ParseNode* node);

/**
 EvaluateSt: Evaluates a st command
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateSt (ParseList* list, ParseNode* node);

/**
 EvaluateHt: Evaluates a ht command
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateHt (ParseList* list, ParseNode* node);

/**
 EvaluateSetpencolor: Recursively evaluates a setpencolor command
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateSetpencolor (ParseList* list, ParseNode* node);

/**
 EvaluatePrint: Recursively evaluates a print command
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluatePrint (ParseList* list, ParseNode* node);

/**
 EvaluateMake: Recursively evaluates a make command
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateMake (ParseList* list, ParseNode* node);

/**
 EvaluateBeginfile: Recursively evaluates an beginfile command
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateBeginfile (ParseList* list, ParseNode* node);

/**
 EvaluateEndfile: Recursively evaluates a endfile command
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateEndfile (ParseList* list, ParseNode* node);

/**
 EvaluateEndfile: Recursively evaluates a loadfile command
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateLoadfile (ParseList* list, ParseNode* node);

/**
 EvaluateExit: Evaluates an exit command
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateExit (ParseList* list, ParseNode* node);

/**
 EvaluateOr: Recursively evaluates an or command
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateOr (ParseList* list, ParseNode* node);

/**
 EvaluateOr: Recursively evaluates an and command
 Pre: Pass the parameters
 Post: Returns the result of the evaluation
*/
ParseVal evaluateAnd (ParseList* list, ParseNode* node);

#endif // _ELUCENAP_PARSERH

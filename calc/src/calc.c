/*Name: Ahmad Wattad
  Project: calculator using fsm machine
  Reviewer: ??
  Date: 03/10/2023
*/
#include <stddef.h> /*size_t*/
#include <assert.h> /*assert*/
#include <string.h> /*strlen*/
#include <stdlib.h> /*strtod*/

#include "calc.h" 
#include "stack.h" 

#define ASCII_SIZE 256
#define MIN_SIZE_OPERANDS 2

/******************************** ENUMS ***************************************/
typedef enum {WAIT_OPERAND, WAIT_OPERATOR, ERROR, NUM_STATES} state_ty;
typedef enum {HASH_OPERATOR = 0, PLUS_OPERATOR, MINUS_OPERATOR , MULTIPLY_OPERATOR ,DIVIDE_OPERATOR, OPENED_PARANTHESIS, NUM_OPERATORS, INVALID, OPERAND, CLOSED_PARANTHESIS, TERMINATE_CHAR, NUM_INPUTS} input_ty;
typedef enum {HASH_P = 0, PARAN_P, PLUS_P, MINUS_P = PLUS_P, MULTIPLY_P ,DIVIDE_P = MULTIPLY_P} op_priority_ty;

/******************************** FUNCS ***************************************/
typedef state_ty (*handler_ty)(int *status, char **input);
typedef double (*operator_func_ty)(double *left_data, double right_data, int *status);

/******************************** STRUCT **************************************/
typedef struct operator
{
	op_priority_ty priority;
	operator_func_ty action_func;
}operator_ty;

/********************************* LUTS ***************************************/
handler_ty handlers[NUM_STATES][NUM_INPUTS]; 
int input_lut[ASCII_SIZE];
operator_ty operators[NUM_OPERATORS]; 


/******************************** STACKS **************************************/
stack_ty *operand_stack = NULL;
stack_ty *operator_stack = NULL; 

/*----------------------------------------------------------------------------*/

/***************************** FORWARDED FUNCS ********************************/

/*HANDLERS*/
static state_ty OperandHandler(int *status, char **input);
static state_ty OperatorHandler(int *status, char **input);
static state_ty OpenedParanHandler(int *status, char **input);
static state_ty ClosedParanHandler(int *status, char **input);
static state_ty ErrorHandler(int *status, char **input);
static state_ty EOIHandler(int *status, char **input);

/*ACTION FUNCS*/
double MultiplyActionFunc(double *left_data, double right_data, int *status);
double DivideActionFunc(double *left_data, double right_data, int *status);
double PLusActionFunc(double *left_data, double right_data, int *status);
double MinusActionFunc(double *left_data, double right_data, int *status);

/*inits*/
static int InitStacks(char *str, int *status);
static void InitLuts();

/*helper*/
static char Resolver(char operator, int *status);
/******************************** API FUNC ************************************/

double Calculate(char *str, int *status)
{
	int state = WAIT_OPERAND;
	char *input = NULL;
	static int init = 0;
	double calculation = 0.0;
  	char curr_char = 0;	
		
	assert(NULL != str);
	
	input = str;
	*status = SUCCESS;
		
	if(0 == init)
	{
		InitLuts();
		init = 1;
	}

	if(FAILURE == InitStacks(str, status))
	{
		return 0;
	}
	
	while ( ERROR != state)
	{
	    	curr_char = *input;
		state = handlers[state][input_lut[curr_char]](status , &input);

	}

	if(SUCCESS == *status)
	{
	
		calculation = *(double*)StackPeek(operand_stack);

	}

	StackDestroy(operand_stack);
  	StackDestroy(operator_stack);

	return calculation;
}

/*------------------------------ HANDLERS ------------------------------------*/

static state_ty OperandHandler(int *status, char **input)
{
	double operand = 0.0;
	
	assert(NULL != status);
	assert(NULL != input);
	
	operand = strtod(*input, input);
	StackPush(operand_stack, &operand);

	return WAIT_OPERATOR;
}

static state_ty OperatorHandler(int *status, char **input)
{
	char operator = 0;
	char prev_op = 0;
	double right_number = 0;
	double *left_number = 0;
	double result = 0;
	
	assert(NULL != status);
	assert(NULL != input);
			
	operator = **input;
	prev_op = *(char*)StackPeek(operator_stack);
	
	while( operators[input_lut[**input]].priority <= operators[input_lut[prev_op]].priority)
	{

		prev_op = Resolver(prev_op, status);	
		
		if(*status == MATH_ERR)
    		{
      			return ERROR;
    		}
	}
	
	StackPush(operator_stack, &operator);	

	++(*input);
	
	return WAIT_OPERAND;
}


static state_ty OpenedParanHandler(int *status, char **input)
{
	char c = '(';
	
	assert(NULL != status);
	assert(NULL != input);
		
	StackPush(operator_stack, &c);	
	++(*input);
	return 	WAIT_OPERAND;
}



static state_ty ClosedParanHandler(int *status, char **input)
{
	char operator = 0;
	double right_number = 0;
	double *left_number = 0;
	double result = 0;
			
	assert(NULL != status);
	assert(NULL != input);
	
	operator = *(char*)StackPeek(operator_stack);
	
	while ('(' != operator)
	{

		if('#' == operator)
   		{
      			*status = SYNTAX_ERROR;
		      	return ERROR;
    		}
    		
		operator = Resolver(operator, status);
	
		if(*status == MATH_ERR)
    		{
      			return ERROR;
    		}
	}	
	
	StackPop(operator_stack);		
	++(*input);
	
	return WAIT_OPERATOR;
}


static state_ty ErrorHandler(int *status, char **input)
{
	assert(NULL != status);
	assert(NULL != input);
	
	*status = SYNTAX_ERROR;
	
	return ERROR;
}

static state_ty EOIHandler(int *status, char **input)
{
	assert(NULL != status);
	assert(NULL != input);
	
	char operator = 0;		

	operator = *(char*)StackPeek(operator_stack);

	while ( '#' != operator)
	{	
	 	if(MIN_SIZE_OPERANDS > StackSize(operand_stack))
   		{
      			*status = SYNTAX_ERROR;
      			return ERROR;
    		}	
    		
		operator = Resolver(operator, status);
	}	
	

  	
  	return ERROR;
}
/*------------------------------ ACTION FUNCS --------------------------------*/

double MultiplyActionFunc(double *left_data, double right_data, int *status)
{
	assert(NULL != left_data);

	return (*left_data * right_data);
}

double DivideActionFunc(double *left_data, double right_data, int *status)
{
	assert(NULL != left_data);
	
	if(0 == right_data)
	{
		*status = MATH_ERR;	
	}
	
	return (*left_data / right_data);
}

double PLusActionFunc(double *left_data, double right_data, int *status)
{
	assert(NULL != left_data);
	return (*left_data + right_data);
}

double MinusActionFunc(double *left_data, double right_data, int *status)
{
	assert(NULL != left_data);

	return (*left_data - right_data);
}

/*------------------------------ InitLuts ------------------------------------*/

static void InitLuts()
{
	int i = 0;

	for(i = 0; i < ASCII_SIZE; ++i)
	{
		input_lut[i] = 	INVALID;	
	}
	
	for(i = '0'; i < '9'; ++i)
	{
		input_lut[i] = OPERAND;	
	}

	input_lut['#'] = HASH_OPERATOR;			
	input_lut['+'] = PLUS_OPERATOR;	
	input_lut['-'] = MINUS_OPERATOR;
	input_lut['/'] = DIVIDE_OPERATOR;
	input_lut['*'] = MULTIPLY_OPERATOR;
	input_lut[')'] = CLOSED_PARANTHESIS;	
	input_lut['('] = OPENED_PARANTHESIS;		
	input_lut['\0'] = TERMINATE_CHAR;

	
	handlers[WAIT_OPERAND][HASH_OPERATOR] = ErrorHandler;
	handlers[WAIT_OPERAND][PLUS_OPERATOR] = ErrorHandler;
	handlers[WAIT_OPERAND][MINUS_OPERATOR] = ErrorHandler;
	handlers[WAIT_OPERAND][MULTIPLY_OPERATOR] = ErrorHandler;
	handlers[WAIT_OPERAND][DIVIDE_OPERATOR] = ErrorHandler;
	handlers[WAIT_OPERAND][OPENED_PARANTHESIS] = OpenedParanHandler;
	handlers[WAIT_OPERAND][INVALID] = ErrorHandler;
	handlers[WAIT_OPERAND][OPERAND] = OperandHandler;
	handlers[WAIT_OPERAND][CLOSED_PARANTHESIS] = ErrorHandler;
	handlers[WAIT_OPERAND][TERMINATE_CHAR] = ErrorHandler;	
						
	handlers[WAIT_OPERATOR][HASH_OPERATOR] = ErrorHandler;
	handlers[WAIT_OPERATOR][PLUS_OPERATOR] = OperatorHandler;
	handlers[WAIT_OPERATOR][MINUS_OPERATOR] = OperatorHandler;
	handlers[WAIT_OPERATOR][MULTIPLY_OPERATOR] = OperatorHandler;
	handlers[WAIT_OPERATOR][DIVIDE_OPERATOR] = OperatorHandler;
	handlers[WAIT_OPERATOR][OPENED_PARANTHESIS] = ErrorHandler;	
	handlers[WAIT_OPERATOR][INVALID] = ErrorHandler;
	handlers[WAIT_OPERATOR][OPERAND] = ErrorHandler;
	handlers[WAIT_OPERATOR][CLOSED_PARANTHESIS] = ClosedParanHandler;
	handlers[WAIT_OPERATOR][TERMINATE_CHAR] = EOIHandler;	

	handlers[ERROR][HASH_OPERATOR] = ErrorHandler;
	handlers[ERROR][PLUS_OPERATOR] = ErrorHandler;
	handlers[ERROR][MINUS_OPERATOR] = OperandHandler;
	handlers[ERROR][MULTIPLY_OPERATOR] = OpenedParanHandler;
	handlers[ERROR][DIVIDE_OPERATOR] = ErrorHandler;
	handlers[ERROR][OPENED_PARANTHESIS] = ErrorHandler;	
	handlers[ERROR][INVALID] = ErrorHandler;
	handlers[ERROR][OPERAND] = ErrorHandler;
	handlers[ERROR][CLOSED_PARANTHESIS] = ErrorHandler;
	handlers[ERROR][TERMINATE_CHAR] = ErrorHandler;	

	operators[HASH_OPERATOR].priority = HASH_P;
	operators[HASH_OPERATOR].action_func = NULL;
		
	operators[OPENED_PARANTHESIS].priority = PARAN_P;
	operators[OPENED_PARANTHESIS].action_func = NULL;
	
	operators[PLUS_OPERATOR].priority = PLUS_P;
	operators[PLUS_OPERATOR].action_func = PLusActionFunc;
	
	operators[MINUS_OPERATOR].priority = MINUS_P;
	operators[MINUS_OPERATOR].action_func = MinusActionFunc;
	
	operators[MULTIPLY_OPERATOR].priority = MULTIPLY_P;
	operators[MULTIPLY_OPERATOR].action_func = MultiplyActionFunc;
	
	operators[DIVIDE_OPERATOR].priority = DIVIDE_P;
	operators[DIVIDE_OPERATOR].action_func = DivideActionFunc;


}

/*------------------------------ InitStack -----------------------------------*/

static int InitStacks(char *str, int *status)
{
	char operator = '#';
	operand_stack = StackCreate(strlen(str), sizeof(double));

	if(NULL == operand_stack)
	{
		*status = FAILURE;	
		return FAILURE;
	}

	operator_stack = StackCreate(strlen(str), sizeof(char));

	if(NULL == operand_stack)
	{
		*status = FAILURE;
		StackDestroy(operand_stack);	
		return FAILURE;
	}

  	StackPush(operator_stack, &operator);

  	return SUCCESS;
		
}


static char Resolver(char operator, int *status)
{
	double right_number = 0;
	double *left_number = 0;
	double result = 0;
		
	right_number = *(double*)StackPeek(operand_stack);
	StackPop(operand_stack);
	left_number = &(*(double*)StackPeek(operand_stack));
	operators[input_lut[operator]].action_func(left_number, right_number, status);
	result = operators[input_lut[operator]].action_func(left_number, right_number, status);
	*left_number = result;
	StackPop(operator_stack);
	
	return *(char*)StackPeek(operator_stack);	
}



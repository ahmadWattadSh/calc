/*Name: Ahmad Wattad
  Project: sorting
  Reviewer: ??
  Date: 26/09/2023
*/


/******************************************************************************/
/******************************** LIBRARIES  **********************************/
/******************************************************************************/

#include <stdio.h>/*printf*/
#include <stdlib.h>
#include <time.h>


#include "calc.h"

/******************************************************************************/
/******************************** Definitions  ********************************/
/******************************************************************************/

#define ErrorMsg(expected, result, name) \
	{\
		if (expected == result)\
		{\
			printf("%s has succeeded\n",  name);\
		}\
		else\
		{\
			printf("%s has failed\n",  name);\
		}\
}

#define NUM_OF_EXPRESSIONS 18
#define MAX_EXPRESSION_SIZE 50

#define REPORT(expected , result, str) \
  {\
      if (expected != result)\
      {\
        printf("%s has failed\n", str);\
      }\
  }

/************************** Forwarded declarations ****************************/

static void UnitTesting();
static void CalculateTest();


/******************************************************************************/
/******************************* Main *****************************************/
/******************************************************************************/

int main()
{
	UnitTesting();

	return 0;
}

/******************************************************************************/

static void UnitTesting()
{
	CalculateTest();
}
/******************************************************************************/	

static void CalculateTest()
{
    int status_info[NUM_OF_EXPRESSIONS] = {SUCCESS, SUCCESS, SUCCESS, SUCCESS, SUCCESS, MATH_ERR, SUCCESS, SUCCESS, SUCCESS, SUCCESS, SYNTAX_ERROR, SYNTAX_ERROR, SYNTAX_ERROR, SYNTAX_ERROR, SYNTAX_ERROR, SUCCESS, SYNTAX_ERROR, SYNTAX_ERROR};
    double results[NUM_OF_EXPRESSIONS] = {2.0, -1.0, 0.0, 4.0, 2.0, 0.0, 2.0, 16.0, 4.0, 16.0, 0.0, 0.0, 0.0, 0.0, 0.0, 6.0, 0.0, 0.0};
    char expressions[NUM_OF_EXPRESSIONS][MAX_EXPRESSION_SIZE] = {"1+1", "1-1-1", "1-1", "1+1*3", "10/5", "10/0", "(10/5)", "1+1*3*5", "1+(1*3)", "1+5*(1*3)", "(8+3", "8+3)", "(8+3))", "2A6t+6", "32+1    -3", "1+(((5)))", "1+((", "1++1"};
    size_t i = 0;
    int status;
    double res = 0.0;

    while(NUM_OF_EXPRESSIONS > i)
    {
        res = Calculate(expressions[i], &status);
        REPORT(results[i], res, expressions[i]);
        REPORT(status_info[i], status, expressions[i]);
        ++i;
    }

    printf("Calculator tested!\n");
}




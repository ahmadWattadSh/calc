/*Name: Ahmad Wattad
  Project: calculator using fsm machine
  Reviewer: ??
  Date: 03/10/2023
  
/******************************************************************************/

#ifndef __ILRD_CALC_H__
#define __ILRD_CALC_H__

/******************************************************************************/

#define EXP_MAX_LEN  100 

typedef enum  {SUCCESS = 0, FAILURE, MATH_ERR, SYNTAX_ERROR} calc_status_ty;
/******************************************************************************/


/*A Function that calculates an expression.

  Arguments: 'str' - a string provided by the user,
	     'status' - status provided from the user which will be updated 
	     	with the status of calculation at the end of execution.

  Return value: calculated outcome of the provided str. On failure and errors, \
  	it is provided in the status var.

  Complexity: 
*/
double Calculate(char *str, int *status);




/******************************************************************************/

#endif /*__ILRD_CALC_H____*/

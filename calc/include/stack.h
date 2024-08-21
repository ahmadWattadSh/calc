/*Name: HRD 32
  Project: Stack Implementation
  Reviewer: Liel
  Date: 09/08/2023
  Version: 1.0
*/
/******************************************************************************/
#ifndef __STACK_H__
#define __STACK_H__

#include <stddef.h> /*size_t*/
/******************************************************************************/

enum status_ty {FALSE = 0, TRUE = 1};

typedef struct stack stack_ty;

/******************************************************************************/

/*Function that inits the stack struct
  Arguments: capacity of the stack, element size.
  Return value: pointer to the created stack struct
  Undefined B.: element_size = 0, capacity = 0*/
stack_ty *StackCreate(size_t capacity, size_t element_size);

/*Function that deletes the stack struct
  Arguments: pointer to the stack struct.
  Return value: void.
  Undefined B.: NULL ptr of stack*/
void StackDestroy(stack_ty *stack);

/*Function that removes the head of the stack struct
  Arguments: pointer to the stack struct.
  Return value: void.
  Undefined B.: NULL ptr of stack*/
void StackPop(stack_ty *stack);

/*Function that adds data to the head of the stack struct
  Arguments: pointer to the stack struct, data to be added.
  Return value: void.
  Undefined B.: size of data > element_size, NULL ptr of stack*/
void StackPush(stack_ty *stack, const void *data);

/*Function that gets the data of the head of the stack struct
  Arguments: pointer to the stack struct.
  Return value: void* that represent the data.
  Undefined B.: NULL ptr of stack*/
void *StackPeek(const stack_ty *stack);

/*Function that returns the size of the stack struct
  Arguments: pointer to the stack struct.
  Return value: size of the stack struct.
  Undefined B.: NULL ptr of stack*/
size_t StackSize(const stack_ty *stack);

/*Function that checks if the stack struct is empty
  Arguments: pointer to the stack struct.
  Return value: TRUE/FALSE.
  Undefined B.: NULL ptr of stack*/
int StackIsEmpty(const stack_ty *stack);

/*Function that returns the capacity of the stack struct
  Arguments: pointer to the stack struct.
  Return value: capacity of the stack struct.
  Undefined B.: NULL ptr of stack*/
size_t StackCapacity(const stack_ty *stack);

/******************************************************************************/
#endif /*(__STACK_H__)*/

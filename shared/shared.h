#ifndef __SHARED_H__
#define __SHARED_H__

#include <stdio.h>

#define TRUE 1
#define FALSE 0
#define BOOL int // values TRUE and FALSE are intended
#define ERR -1
#define OK 1
#define ERRSTATE int // values OK and ERR intended
#define LT -1 // less than
#define EQ 0 // equal
#define GT 1 //greater than
#define ORD int // values LT, EQ, GT intended

void clearBuffer(FILE* stream);

unsigned long djb2Hash(char* str);

#endif

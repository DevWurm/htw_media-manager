/*
    MIT License

    Copyright (c) 2017 Leo Lindhorst

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

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

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

#ifndef __LIST_H__
#define __LIST_H__

#include <stdarg.h>

#include "shared/shared.h"

typedef struct LIST_EL {
    void* value;
    struct LIST_EL* next;
    struct LIST_EL* prev;
} tListEl;

typedef struct {
    tListEl* next;
} tIterator;

typedef struct {
    tListEl* head;
} tList;

// a predicate takes a value and addtional arguments via argc and an va_list argv and returns a BOOL value
typedef BOOL (*tPredicate)(void* val, int argc, va_list argv);

// a deleter takes a value and correctly frees its allocated memory
typedef ERRSTATE (*tDeleter)(void* val);

// a serializer takes a value and produces a single-line string which can be used for persisting the record
typedef char* (*tSerializer)(void* val);

// a deserializer takes a single-line serialiazation string and creates a record value
typedef void* (*tDeserializer)(char* val);

// a comperator returns LT if left is semantically smaller than right, EQ if they are equal and GT if left is greater than right
typedef ORD (*tComperator)(void* left, void* right);

// List operations
tList* createList();
ERRSTATE deleteList(tList* target, tDeleter deleter);

ERRSTATE insert(tList* list, void* data);

tList* getWhere(tList* list, tPredicate pred, int argc, ...);

ERRSTATE deleteWhere(tList* list, tDeleter deleter, tPredicate pred, int argc, ...);

tIterator* toIterator(tList* list);
ERRSTATE deleteIterator(tIterator* target);
void* getNext(tIterator* it);

// Persistance Operators
ERRSTATE storeList(tList* list, FILE* dest, tSerializer serializer);
ERRSTATE restoreList(tList* list, FILE* src, tDeserializer deserializer);

// sortation operators
void sort(tList* list, tComperator comp);

#endif

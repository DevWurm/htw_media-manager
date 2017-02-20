#ifndef __LIST_H__
#define __LIST_H__

#include <stdarg.h>

#include "shared/shared.h"

typedef struct LIST_EL {
    void* value;
    struct LIST_EL* next;
} tListEl;

typedef struct {
    tListEl* next;
} tIterator;

typedef struct {
    tListEl* head;
} tList;

typedef BOOL (*tPredicate)(void* val, int argc, va_list argv);
typedef ERRSTATE (*tDeleter)(void* val);
typedef char* (*tSerializer)(void* val);
typedef void* (*tDeserializer)(char* val);

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

#endif

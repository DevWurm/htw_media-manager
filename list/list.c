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

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "shared/shared.h"
#include "list.h"

#define BUF_SIZE 2048

// @private
// delete the head element of a list; if a deleter is specified also the list values are deleted;
// otherwise only the list infrastructure elements are deleted
ERRSTATE deleteHead(tList* target, tDeleter deleter) {
    if (target == NULL) return ERR;

    tListEl* head = target->head;

    if (head == NULL) return ERR;

    target->head = head->next;
    if (target->head != NULL) {
        target->head->prev = NULL;
    }

    if (deleter != NULL) {
        if (!deleter(head->value)) return ERR;
    }

    free(head);

    return OK;
}

// create a new empty list
tList* createList() {
    tList* list = malloc(sizeof(tList));
    if (list == NULL) return NULL;

    list->head = NULL;
    return list;
}

// delete a whole list; if a deleter is specified also the list values are deleted;
// otherwise only the list infrastructure elements are deleted
ERRSTATE deleteList(tList* target, tDeleter deleter) {
    if (target == NULL) return ERR;

    while (target->head != NULL) {
        if (deleteHead(target, deleter) == ERR) return ERR;
    }

    free(target);

    return OK;
}

// insert a new value at the head of a list
ERRSTATE insert(tList* target, void* data) {
    if (target == NULL) return ERR;

    tListEl* el = malloc(sizeof(tListEl));
    if (el == NULL) return ERR;

    el->prev = NULL;
    el->value = data;
    el->next = target->head;
    if (target->head != NULL) {
        target->head->prev = el;
    }
    target->head = el;

    return OK;
}

// get all list elements, where a given predicate matches; 
// the predicate receives a list value and optionally additional arguments; (argc: number of arguments; varargs: arguments)
tList* getWhere(tList* list, tPredicate pred, int argc, ...) {
    if (list == NULL || pred == NULL) return NULL;

    tList* result = createList();
    if (result == NULL) return NULL;

    va_list vl;

    tListEl* p;
    for(p = list->head; p != NULL; p = p->next) {
        va_start(vl, argc);
        if (pred(p->value, argc, vl)) {
            insert(result, p->value);
        }
    }

    return result;
}

// delete all elements in a list where a specified predicate matches; if a deleter is specified also the list values are deleted
// otherwise only the list infrastructure elements are deleted
// the predicate receives a list value and optionally additional arguments; (argc: number of arguments; varargs: arguments)
ERRSTATE deleteWhere(tList* list, tDeleter deleter, tPredicate pred, int argc, ...) {
    if (list == NULL || pred == NULL) return ERR;

    va_list vl;
    va_start(vl, argc);

    // handle all cases where the target elements are head elements
    while (list->head != NULL && pred(list->head->value, argc, vl)) {
        va_start(vl, argc);
        if (!deleteHead(list, deleter)) return ERR;
    }

    tListEl* p;
    p = list->head; 
    while (p != NULL && p->next != NULL ) {
        va_start(vl, argc);
        if (pred(p->next->value, argc, vl)) {
            tListEl* el = p->next;
            p->next = p->next->next;
            if (p->next != NULL) {
                p->next->prev = p;
            }

            if(deleter != NULL) {
                if (!deleter(el->value)) return ERR;
            }
            
            free(el);
        } else {
            p = p->next;
        }
    }

    return OK;
}

// generates an interator of a list
tIterator* toIterator(tList* list) {
    if (list == NULL) return NULL;

    tIterator* it = malloc(sizeof(tIterator));
    if (it == NULL) return NULL;

    it->next = list->head;

    return it;
}

// deletes an iterator
ERRSTATE deleteIterator(tIterator* target) {
    free(target);

    return OK;
};

// returns the next value of the iterator; the iterator is moved to the next element
void* getNext(tIterator* it) {
    if (it->next == NULL) return NULL;

    void* data = it->next->value;
    it->next = it->next->next;

    return data;
};

// Persistance operators

// strores a list to a given file using the given serializer
ERRSTATE storeList(tList* list, FILE* dest, tSerializer serializer) {
    if (list == NULL || dest == NULL || serializer == NULL) return ERR;

    tIterator* it;
    void* val;

    for (it = toIterator(list); (val = getNext(it));) {
        char* serialization = serializer(val);

        if (serialization == NULL || fputs(serialization, dest) == EOF || putc('\n', dest) == EOF) return ERR;

        free(serialization);
    } 

    return OK;
}

// resotres a list from a given file using the specified deserializer
ERRSTATE restoreList(tList* list, FILE* src, tDeserializer deserializer) {
    char buf[BUF_SIZE] = "";

    while(fgets(buf, BUF_SIZE, src)) {
        buf[strlen(buf) - 1] = '\0';
        if (strlen(buf) > 0) {
            if (!insert(list, deserializer(buf))) return ERR;
        }
    }

    return OK;
}

// Sortation operators

// sorts a list by using the specified comperator
void sort(tList* list, tComperator comp) {
    // sorts a linked list of tListEl elements using bubblesort;
    // to reduce complexity and to improve performanc the values of
    // the elements are swapped insgtead of the elements itself;
      
    tListEl *i, *j, *last;
    void* buf;

    // search last element of list
    for (last = list->head; last->next != NULL; last = last->next);

    for (i = last; i->prev != NULL; i = i->prev) {
        for (j = list->head; j != i; j = j->next) {
            if (comp(j->value, j->next->value) == GT) {
                // swap values
                buf = j->value;
                j->value = j->next->value;
                j->next->value = buf;
            }
        }
    }
}

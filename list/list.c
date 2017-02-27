#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "shared/shared.h"
#include "list.h"

#define BUF_SIZE 2048

// @private
// delete the head element of a list; if a deleter is specified also the list values are deleted
ERRSTATE deleteHead(tList* target, tDeleter deleter) {
    if (target == NULL) return ERR;

    tListEl* head = target->head;

    if (head == NULL) return ERR;

    target->head = head->next;

    if (deleter != NULL) {
        if (!deleter(head->value)) return ERR;
    }

    free(head);

    return OK;
}

tList* createList() {
    tList* list = malloc(sizeof(tList));
    if (list == NULL) return NULL;

    list->head = NULL;
    return list;
}

// delete a list; if a deleter is specified also the list values are deleted
ERRSTATE deleteList(tList* target, tDeleter deleter) {
    if (target == NULL) return ERR;

    while (target->head != NULL) {
        if (deleteHead(target, deleter) == ERR) return ERR;
    }

    free(target);

    return OK;
}

ERRSTATE insert(tList* target, void* data) {
    if (target == NULL) return ERR;

    tListEl* el = malloc(sizeof(tListEl));
    if (el == NULL) return ERR;

    el->value = data;
    el->next = target->head;
    target->head = el;

    return OK;
}

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

tIterator* toIterator(tList* list) {
    if (list == NULL) return NULL;

    tIterator* it = malloc(sizeof(tIterator));
    if (it == NULL) return NULL;

    it->next = list->head;

    return it;
}
ERRSTATE deleteIterator(tIterator* target) {
    free(target);

    return OK;
};

void* getNext(tIterator* it) {
    if (it->next == NULL) return NULL;

    void* data = it->next->value;
    it->next = it->next->next;

    return data;
};

// Persistance operators
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

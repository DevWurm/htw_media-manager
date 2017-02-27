#include <stdio.h>

#include "shared.h"

void clearBuffer(FILE* stream) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

/**
 * djb2 Hash Algorithm by Dan Bernstein
 * IMPLEMENTATION COPIED FROM http://www.cse.yorku.ca/~oz/hash.html
 */
unsigned long djb2Hash(char* str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = hash * 33 ^ c;

    return hash;
}
